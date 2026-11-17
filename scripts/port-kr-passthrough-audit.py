#!/usr/bin/env python3
"""PC port: find K&R argument pass-throughs (plan 18.9).

IDO code often calls a placeholder with FEWER arguments than the callee reads: the missing
ones ride through in $a1..$a3 from the caller's own arguments (or from an earlier call), and
the matched C reproduces that by just not passing them.  On the port (i386 cdecl) the callee
then reads stack garbage.  This lists every CALL / FCALL / GCALL / XCALL / SCALL site (in the
PORT-active text, i.e. not in the #else of an #ifdef PORT) whose argument count is below the
parameter count of the callee's selected definition (port/sourcemap.tsv).

  scripts/port-kr-passthrough-audit.py [--all]     # --all: also sites in unselected copies
Output: file:line  caller  callee  passed/params
"""
import argparse
import re
import sys
from collections import defaultdict

SITE = re.compile(r"\b(?:CALL|FCALL|GCALL)\(\s*(\w+)\s*\)\s*\(|\b(?:XCALL|SCALL)\(\s*\w+\s*,\s*(\w+)\s*\)\s*\(")
DEF = re.compile(r"^[A-Za-z_][\w \t\*]*?\b(\w+)\s*\(([^;{}()]*)\)\s*\{", re.M)


def strip_comments(s):
    # keep newlines so line numbers stay right
    s = re.sub(r"/\*.*?\*/", lambda m: "\n" * m.group(0).count("\n"), s, flags=re.S)
    return re.sub(r"//[^\n]*", "", s)


def port_active(s):
    """blank the text in the inactive branch of #ifdef PORT / #ifndef PORT (simple nesting)."""
    out, stack = [], []   # stack of (is_port_cond, active)
    for ln in s.split("\n"):
        t = ln.strip()
        m = re.match(r"#\s*(ifdef|ifndef|if)\b\s*(.*)", t)
        active = all(a for _, a in stack)
        if m:
            cond = m.group(2).strip()
            if m.group(1) in ("ifdef", "ifndef") and cond == "PORT":
                stack.append((True, m.group(1) == "ifdef"))
            elif m.group(1) == "if" and re.fullmatch(r"defined\s*\(?\s*PORT\s*\)?", cond):
                stack.append((True, True))
            else:
                stack.append((False, True))
            out.append("")
            continue
        if re.match(r"#\s*else\b", t) and stack:
            p, a = stack[-1]
            stack[-1] = (p, (not a) if p else a)
            out.append("")
            continue
        if re.match(r"#\s*endif\b", t) and stack:
            stack.pop()
            out.append("")
            continue
        out.append(ln if active else "")
    s = "\n".join(out)
    # the PORT expansions of the uso_call.h KR_ macros: each adds an argument / parameter
    s = re.sub(r"\bKR_PARAM0\(([^()]*)\)", r"\1", s)
    s = re.sub(r"\bKR_PARAM\(([^()]*)\)", r", \1", s)
    s = re.sub(r"\bKR_ARG0\(", "(", s)
    return re.sub(r"\bKR_ARG\(", ", (", s)


def nparams(plist):
    p = plist.strip()
    if p in ("", "void"):
        return 0 if p == "void" else None   # () = K&R / unknown
    if "..." in p:
        return None
    depth, n = 0, 1
    for c in p:
        depth += c in "(["
        depth -= c in ")]"
        n += c == "," and depth == 0
    return n


def nargs(s, i):
    """s[i] is just past '(' of the call; count top-level args."""
    depth, n, empty = 1, 1, True
    while i < len(s) and depth:
        c = s[i]
        if c in "([{":
            depth += 1
        elif c in ")]}":
            depth -= 1
        elif c == "," and depth == 1:
            n += 1
        if not c.isspace() and depth >= 1 and not (c == ")" and depth == 0):
            empty = False
        i += 1
    return 0 if empty and n == 1 else n


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--all", action="store_true")
    a = ap.parse_args()
    sel = {}
    for ln in open("port/sourcemap.tsv"):
        p = ln.rstrip("\n").split("\t")
        if len(p) >= 6 and p[3] == "c" and not ln.startswith("#") and p[0] != "function":
            sel[p[0]] = p[5]
    texts = {}
    for f in set(sel.values()):
        try:
            texts[f] = port_active(strip_comments(open(f, errors="replace").read()))
        except OSError:
            pass
    params = {}
    defs = defaultdict(list)   # file -> [(start, name)]
    for f, s in texts.items():
        for m in DEF.finditer(s):
            name = m.group(1)
            if name in ("if", "while", "for", "switch", "return", "sizeof"):
                continue
            defs[f].append((m.start(), name))
            if sel.get(name) == f:
                params[name] = nparams(m.group(2))
    hits = 0
    for f, s in sorted(texts.items()):
        starts = defs[f]
        for m in SITE.finditer(s):
            callee = m.group(1) or m.group(2)
            want = params.get(callee)
            if want is None:
                continue
            got = nargs(s, m.end())
            if got >= want:
                continue
            caller = None
            for st, nm in starts:
                if st <= m.start():
                    caller = nm
            if not a.all and sel.get(caller) != f:
                continue
            line = s.count("\n", 0, m.start()) + 1
            print("%s:%d\t%s\t%s\t%d/%d" % (f, line, caller, callee, got, want))
            hits += 1
    print("# %d K&R pass-through sites (args < params)" % hits, file=sys.stderr)


if __name__ == "__main__":
    main()
