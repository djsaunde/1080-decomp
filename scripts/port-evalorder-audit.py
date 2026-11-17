#!/usr/bin/env python3
"""PC port: find calls whose callee depends on a side effect inside their own argument list
(plan 18.9, run 823).

  VT_SLOT(p, N).fn(VT_SLOT(p = o->vptr, N).delta + o)       (and fn-pointer / PMF spellings)

IDO evaluates the arguments first (so p is assigned before the function word is loaded); gcc
loads the function designator first and calls through a stale p.  The fix is to assign p
before the call under `#ifdef PORT`.  The reverse order (the callee expression assigns a
variable that an argument reads) is listed too (REV).  Scans the PORT-active text of every
game source the port compiles (port/gen/sources.cmake, C and C++); a site whose variable is
already assigned by the statement right before the call (the PORT fix) is not listed.

  scripts/port-evalorder-audit.py
Output: [FWD|REV] file:line  callee-expression  [variable]
"""
import importlib.util
import re
import sys

spec = importlib.util.spec_from_file_location("kr", "scripts/port-kr-passthrough-audit.py")
KR = importlib.util.module_from_spec(spec)
spec.loader.exec_module(KR)

KW = {"if", "while", "for", "switch", "return", "sizeof", "else", "do", "case"}
ID = re.compile(r"[A-Za-z_]\w*")
ASG = r"(?<![=!<>+\-*/%&|^])\b{v}\s*(?:[+\-*/%&|^]|<<|>>)?=(?!=)|\b{v}\s*(?:\+\+|--)|(?:\+\+|--)\s*{v}\b"
SKIP = {"VT_SLOT", "fn", "delta", "CALL", "FCALL", "GCALL", "XCALL", "SCALL", "char", "int", "void"}


def callee_back(s, i):
    """s[i] == '(' of a call: -> the postfix expression before it, or None"""
    j = i - 1
    while j >= 0 and s[j].isspace():
        j -= 1
    end = j + 1
    if j < 0 or not (s[j] in ")]" or s[j].isalnum() or s[j] == "_"):
        return None
    while j >= 0:
        while j >= 0 and s[j].isspace():
            j -= 1
        c = s[j]
        if c in ")]":
            d = 0
            while j >= 0:
                d += s[j] in ")]"
                d -= s[j] in "(["
                if d == 0:
                    break
                j -= 1
            j -= 1
        elif c.isalnum() or c == "_":
            k = j
            while k >= 0 and (s[k].isalnum() or s[k] == "_"):
                k -= 1
            if s[k + 1:j + 1] in KW:
                return None if j + 1 == end else s[j + 1:end]
            j = k
        else:
            break
        k = j
        while k >= 0 and s[k].isspace():
            k -= 1
        if k >= 0 and s[k] == ".":
            j = k - 1
            continue
        if k >= 1 and s[k - 1:k + 1] == "->":
            j = k - 2
            continue
        if k >= 0 and (s[k].isalnum() or s[k] == "_") and s[j + 1:j + 2] == "(":
            j = k
            continue
        j = k
        break
    return s[j + 1:end]


def main():
    files = [l.strip().replace("${GAME_ROOT}/", "") for l in open("port/gen/sources.cmake")
             if "${GAME_ROOT}/src" in l]
    hits = 0
    for f in files:
        try:
            s = KR.port_active(KR.strip_comments(open(f, errors="replace").read()))
        except OSError:
            continue
        s = re.sub(r"(?m)^[ \t]*#[ \t]*define(?:[^\n]*\\\n)*[^\n]*", lambda m: "\n" * m.group(0).count("\n"), s)
        for m in re.finditer(r"\(", s):
            i = m.start()
            cal = callee_back(s, i)
            if not cal or cal.strip() in KW:
                continue
            cal = cal.strip()
            d, k = 1, i + 1
            while k < len(s) and d:
                d += s[k] in "([{"
                d -= s[k] in ")]}"
                k += 1
            args = s[i + 1:k - 1]
            ids = set(ID.findall(cal)) - SKIP
            prev = s[max(s.rfind(";", 0, s.rfind(";", 0, i - len(cal))), 0):i - len(cal)]
            for v in sorted(ids):
                kind = None
                if re.search(ASG.format(v=re.escape(v)), args):
                    kind = "FWD"
                elif re.search(ASG.format(v=re.escape(v)), cal) and re.search(r"\b%s\b" % re.escape(v), args):
                    kind = "REV"
                if kind is None:
                    continue
                if kind == "FWD" and re.search(r"(?:^|[;{}\s])%s\s*=(?!=)" % re.escape(v), prev):
                    continue          # fixed: the PORT statement before the call assigns v
                print("%s %s:%d\t%s\t[%s]" % (kind, f, s.count("\n", 0, i) + 1, re.sub(r"\s+", " ", cal)[:80], v))
                hits += 1
                break
    print("# %d unsequenced callee / argument sites" % hits, file=sys.stderr)


if __name__ == "__main__":
    main()
