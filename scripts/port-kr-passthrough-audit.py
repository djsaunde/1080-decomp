#!/usr/bin/env python3
"""PC port: find K&R argument pass-throughs (plan 18.9).

IDO code often calls a placeholder with FEWER arguments than the callee reads: the missing
ones ride through in $a1..$a3 from the caller's own arguments (or from an earlier call), and
the matched C reproduces that by just not passing them.  On the port (i386 cdecl) the callee
then reads stack garbage.  This lists every CALL / FCALL / GCALL / XCALL / SCALL site (in the
PORT-active text, i.e. not in the #else of an #ifdef PORT) whose argument count is below the
parameter count of the callee's selected definition (port/sourcemap.tsv).

Scanned (run 823): the SHIPPED body of every function (sourcemap column `source`: the .c / .cpp
donor or the match_imports port, not the host placeholder), C and C++;
  - CALL / FCALL / GCALL / XCALL / SCALL sites;
  - direct calls `f(...)` of a defined function (--no-direct to leave them out);
  - `this`-only virtual calls (`VT_SLOT(p, N).fn(this)` and the cfront CF_V* / `(*fp)(x)` spellings
    through a vtable): listed as `vt:N` with the largest parameter count of any function the
    ROM vtables put in slot N (from the DataRelocs), when that is > 1 (--no-vt to leave them out).
--rom adds the ROM evidence for each direct / CALL site: what the caller's first missing
register holds at the jal (a backwards linear walk of the caller's ROM Text from the jal and
its delay slot): `in:aN` (the caller's own incoming $aN, untouched), `call:<f>` (clobbered by an
earlier call: garbage), or the instruction that last wrote it.  `used` / `unused` says whether
the callee's shipped body names the first missing parameter at all.

  scripts/port-kr-passthrough-audit.py [--all] [--rom]   # --all: also sites in unselected copies
Output: file:line  caller  callee  passed/params  [rom evidence]
"""
import argparse
import importlib.util
import struct
import re
import sys
from collections import defaultdict

SITE = re.compile(r"\b(?:CALL|FCALL|GCALL)\(\s*(\w+)\s*\)\s*\(|\b(?:XCALL|SCALL)\(\s*\w+\s*,\s*(\w+)\s*\)\s*\(")
DIRECT = re.compile(r"(?<![\w.>])(\w+)\s*\(")
VTCALL = re.compile(r"VT_SLOT\(\s*(\w+)\s*,\s*(\d+|0x[0-9A-Fa-f]+)\s*\)\s*\.fn\s*\)?\s*\(")
NOTCALL = {"if", "while", "for", "switch", "return", "sizeof", "do", "else", "case", "defined",
           "CALL", "FCALL", "GCALL", "XCALL", "SCALL", "VT_SLOT", "XDATA", "KR_ARG", "KR_ARG0",
           "KR_PARAM", "KR_PARAM0", "__typeof__", "typeof", "__attribute__", "asm", "__asm__"}
DEF = re.compile(r"^[A-Za-z_][\w \t\*]*?\b(\w+)\s*\(([^;{}()]*)\)\s*"
                 r"(?:[A-Za-z_][^;{}()]*;\s*)*\{", re.M)      # ANSI, or K&R with its declaration list


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


def load(name, path):
    spec = importlib.util.spec_from_file_location(name, path)
    mod = importlib.util.module_from_spec(spec)
    argv, sys.argv = sys.argv, [sys.argv[0]]
    spec.loader.exec_module(mod)
    sys.argv = argv
    return mod


def is_decl_or_def(s, pos):
    """a direct-call candidate at pos is really a declaration / definition head when the text
    since the previous statement boundary is only type words (`extern void *f(`, `int f(`)"""
    j = pos
    while j > 0 and s[j - 1] not in ";{}":
        j -= 1
    head = s[j:pos]
    if re.search(r"[=(),?:!<>+\-/%&|^\[]", head):
        return False
    words = re.findall(r"\w+", head)
    if not words or words[-1] in ("return", "else", "do") or re.search(r"\bcase\b|\bdefault\b|\w+\s*:", head):
        return False
    return True


# ------------------------------------------------------------------ ROM evidence
def rd_of(w):
    """-> the GPR a MIPS instruction writes, or None"""
    op = w >> 26
    rs, rt, rd = (w >> 21) & 31, (w >> 16) & 31, (w >> 11) & 31
    if op == 0:
        fn = w & 63
        if fn in (8, 12, 13, 24, 25, 26, 27, 17, 19):   # jr syscall break mult(u) div(u) mthi mtlo
            return None
        return rd or None
    if op == 3:
        return 31
    if 8 <= op <= 15 or 32 <= op <= 38 or op in (24, 25, 26, 27, 55):
        return rt or None
    if op == 17 and rs in (0, 2):                        # mfc1 / cfc1
        return rt or None
    return None


class RomEvidence:
    def __init__(self):
        self.npc = load("npc", "scripts/name-placeholder-calls.py")
        self.rom = self.npc.Rom()
        self.byname = {}
        for md in self.rom.mods:
            for off, sz, nm in md.funcs:
                self.byname.setdefault(nm, (md, off, sz))
        self.seen = defaultdict(int)

    def jals(self, md, off, sz, callee):
        t = md.secs["Text"]
        out = []
        for pc in range(off, off + sz, 4):
            w = struct.unpack_from(">I", t, pc)[0]
            if w >> 26 == 3 and self.rom.jal_target(md, pc) == callee:
                out.append(pc)
        return out

    def reg_at(self, md, off, pc, reg):
        """what reg holds at the jal at pc: walk back linearly to the function start"""
        t = md.secs["Text"]
        w = struct.unpack_from(">I", t, pc + 4)[0]          # the delay slot runs first
        if rd_of(w) == reg:
            return "slot:%08X" % w
        q = pc - 4
        while q >= off:
            w = struct.unpack_from(">I", t, q)[0]
            if w >> 26 == 3 or (w >> 26 == 0 and w & 63 == 9):
                tgt = self.rom.jal_target(md, q) if w >> 26 == 3 else "jalr"
                d = struct.unpack_from(">I", t, q + 4)[0]
                return "call:%s" % tgt
            if rd_of(w) == reg:
                # `lw aN, frame + 4N(sp)`: the reload of the caller's own aN from its home slot
                f0 = struct.unpack_from(">I", t, off)[0]
                frame = -((f0 & 0xFFFF) - 0x10000 if f0 & 0x8000 else f0 & 0xFFFF) if f0 >> 16 == 0x27BD else None
                if frame and w >> 16 == (0x8FA0 | reg) and (w & 0xFFFF) == frame + 4 * (reg - 4):
                    return "in:a%d" % (reg - 4)
                return "set@+0x%X:%08X" % (q - off, w)
            q -= 4
        return "in:a%d" % (reg - 4)

    def site(self, caller, callee, first_missing, want=None):
        e = self.byname.get(caller)
        if e is None or first_missing > 3:
            return "-"
        want = min(want or first_missing + 1, 4)
        md, off, sz = e
        js = self.jals(md, off, sz, callee)
        if not js:
            return "nojal"
        k = self.seen[(caller, callee)]
        self.seen[(caller, callee)] += 1
        pc = js[min(k, len(js) - 1)]
        regs = ",".join(self.reg_at(md, off, pc, 4 + r) for r in range(first_missing, want))
        return "%s%s" % (regs, "" if len(js) == 1 else " (jal %d/%d)" % (min(k, len(js) - 1) + 1, len(js)))

    def vtables(self, params):
        """the ROM's cfront vtables: [{slot: (function, parameter count or None)}] (DataReloc
        words typed `vtbl` in port/gen/blob_types.tsv; a run of slots 8 apart is one table)"""
        pgb = load("pgb", "scripts/port-gen-binds.py")
        rt = pgb.RomTargets(self.rom)
        vt = defaultdict(set)
        for ln in open("port/gen/blob_types.tsv"):
            f = ln.rstrip("\n").split("\t")
            if len(f) > 5 and f[5] == "vtbl":
                vt[(f[0], f[1])].add(int(f[2], 16))
        tables = []
        for (short, sec), heads in vt.items():
            md = self.rom.by_short.get(short)
            if md is None:
                continue
            data = md.secs[sec]
            rel = {o: f1 for f1, o, _ in md.relocs(sec)}
            run = None
            for h in sorted(heads):
                if run is None or h - 8 not in heads:
                    run = h
                    tables.append({})
                f1 = rel.get(h + 4)
                t = rt.raw(md, f1 >> 4) if f1 is not None else None
                if not t or t[1] != "Text":
                    continue
                fn = t[0].func_at((t[2] + struct.unpack_from(">I", data, h + 4)[0]) & 0xFFFFFFFF)
                tables[-1][(h - run) // 8] = (fn, params.get(fn))
        return tables


def vt_arity(tables, caller, n):
    """-> (max parameter count, function) of slot n over the vtables `this` of caller can have
    (the tables holding any `<Class>_` function of the caller's class), else over all tables
    (marked `any`)"""
    cls = caller.split("_")[0] + "_" if caller and "_" in caller else None
    mine = [t for t in tables if cls and any(fn and fn.startswith(cls) for fn, _ in t.values())]
    best, who = 0, None
    if mine:
        for t in mine:
            fn, np_ = t.get(n, (None, None))
            if np_ is not None and np_ > best:
                best, who = np_, fn
        return best, "%s (%s tables)" % (who, cls[:-1])
    # class unknown: the parameter count most tables agree on for slot n (the max is noise:
    # slot n means different methods in unrelated hierarchies)
    votes = defaultdict(list)
    for t in tables:
        fn, np_ = t.get(n, (None, None))
        if np_ is not None:
            votes[np_].append(fn)
    if not votes:
        return 0, None
    best = max(votes, key=lambda k: len(votes[k]))
    return best, "%s (%d of %d tables)" % (votes[best][0], len(votes[best]), sum(map(len, votes.values())))


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--all", action="store_true")
    ap.add_argument("--rom", action="store_true", help="add the ROM register evidence")
    ap.add_argument("--no-direct", action="store_true")
    ap.add_argument("--no-vt", action="store_true")
    a = ap.parse_args()
    sel = {}
    for ln in open("port/sourcemap.tsv"):
        p = ln.rstrip("\n").split("\t")
        if len(p) >= 6 and p[3] in ("c", "cpp") and not ln.startswith("#") and p[0] != "function":
            sel[p[0]] = p[2]       # the shipped body (a donor / import, not the host placeholder)
    texts = {}
    for f in set(sel.values()):
        try:
            # #define bodies are not call sites (their uses are scanned as direct calls)
            texts[f] = re.sub(r"(?m)^[ \t]*#[ \t]*define(?:[^\n]*\\\n)*[^\n]*",
                              lambda m: "\n" * m.group(0).count("\n"),
                              port_active(strip_comments(open(f, errors="replace").read())))
            texts[f] = re.sub(r'"(?:\\.|[^"\\\n])*"', '""', texts[f])     # nor string literals
        except OSError:
            pass
    params = {}
    bodies = {}
    defs = defaultdict(list)   # file -> [(start, name)]
    for f, s in texts.items():
        for m in DEF.finditer(s):
            name = m.group(1)
            if name in NOTCALL:
                continue
            defs[f].append((m.start(), name))
            if sel.get(name) == f:
                params[name] = nparams(m.group(2))
                bodies[name] = (m.group(2), s, m.end())
    ev = RomEvidence() if a.rom or not a.no_vt else None
    tables = ev.vtables(params) if not a.no_vt else []

    def used(callee, k):
        """does the callee's shipped body name parameter k (0-based)?"""
        if callee not in bodies:
            return "?"
        plist, s, st = bodies[callee]
        ps = [x.strip() for x in re.split(r",(?![^()]*\))", plist)]
        if k >= len(ps):
            return "?"
        nm = re.findall(r"(\w+)\s*(?:\[[^]]*\])?\s*$", re.sub(r"\(\s*\*\s*(\w+)\s*\)\s*\(.*", r"\1", ps[k]))
        if not nm:
            return "unnamed"
        d, i = 1, st
        while i < len(s) and d:
            d += s[i] == "{"
            d -= s[i] == "}"
            i += 1
        return "used" if re.search(r"\b%s\b" % re.escape(nm[0]), s[st:i]) else "unused"

    hits = defaultdict(int)
    out = []
    for f, s in sorted(texts.items()):
        starts = defs[f]
        found = []
        for m in SITE.finditer(s):
            found.append((m.start(), m.end(), m.group(1) or m.group(2), "call"))
        if not a.no_direct:
            for m in DIRECT.finditer(s):
                nm = m.group(1)
                if nm in NOTCALL or nm not in params:
                    continue
                if is_decl_or_def(s, m.start()):
                    continue
                found.append((m.start(), m.end(), nm, "direct"))
        if not a.no_vt:
            for m in VTCALL.finditer(s):
                found.append((m.start(), m.end(), "vt:%d" % int(m.group(2), 0), "vt"))
            for m in re.finditer(r"\bCF_V(\d)\(\s*[^,]+,\s*(\w+)\s*,\s*(\d+)", s):
                k, meth, n = int(m.group(1)), m.group(2), int(m.group(3))
                if re.search(r"(?:->|\.)\s*%s\s*\(" % meth, s):
                    found.append((m.start(), None, "vt:%d" % n, "cf:%d:%s" % (k + 1, meth)))
        for st, en, callee, kind in sorted(found):
            caller = None
            for d0, nm in starts:
                if d0 <= st:
                    caller = nm
            if not a.all and sel.get(caller) != f:
                continue
            if kind.startswith("cf:"):
                got = int(kind.split(":")[1])
                want, who = vt_arity(tables, caller, int(callee[3:]))
                kind = "vt"
            elif kind == "vt":
                want, who = vt_arity(tables, caller, int(callee[3:]))
                got = nargs(s, en)
                if got != 1:
                    continue
            else:
                want = params.get(callee)
                if want is None:
                    continue
                got = nargs(s, en)
            if got >= want:
                continue
            line = s.count("\n", 0, st) + 1
            extra = ""
            if kind == "vt":
                extra = "\tslot max: %s" % who
            elif a.rom:
                extra = "\t%s\t%s" % (ev.site(caller, callee, got, want), used(callee, got))
            out.append("%s:%d\t%s\t%s\t%d/%d%s" % (f, line, caller, callee, got, want, extra))
            hits[kind] += 1
    print("\n".join(out))
    print("# %d K&R pass-through sites (args < params): %s" % (
        sum(hits.values()), ", ".join("%s %d" % kv for kv in sorted(hits.items()))), file=sys.stderr)


if __name__ == "__main__":
    main()
