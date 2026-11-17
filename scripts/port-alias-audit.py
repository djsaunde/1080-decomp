#!/usr/bin/env python3
"""PC port: list the functions where gcc can reorder a write and a read of the same blob bytes
because they are spelled through two different objects (docs/1080_PC_PORT_PLAN.md 18.7).

    scripts/port-alias-audit.py BUILD_DIR [--work DIR] [-j N] [--reuse] [--boot] [--all]

The hazard (found in SnowBoard_ctor, run 623): a placeholder token (`D_00000000`, `gl_data_...`)
is renamed to its blob address by port/tools/cc-select.py AFTER gcc compiled the function
(port-bind, per function), or by the linker (port/gen/aliases.ld, global). At compile time gcc
sees two distinct extern objects, assumes they cannot overlap, and may move a load of one above a
store to the other, or reuse a loaded value across that store, although at run time both name
the same bytes. -fno-strict-aliasing (already in PORT_ARCH_FLAGS) does not help: it only turns off
type-based aliasing, and gcc's decl-based disambiguation (two different extern decls never
overlap; one decl at disjoint offsets never overlaps) has no switch -- `weak`, -fno-tree-pta,
-fno-tree-fre / -pre / -fno-gcse do not change it either (gcc 11, measured run 812). So every
such pair must be spelled through ONE object under PORT.

Method: every unit cc-select compiles is compiled again to assembly WITHOUT the cc-select rename
(the compile command from `ninja -t compdb`, -S, no -g), so each data reference still names the
object gcc reasoned about. Per function (gcc clones FUNC.part.N etc. fold into FUNC), each
reference `SYM+N` is resolved to its run-time blob bytes:
  - a port-bind of FUNC in the unit's port/gen/tu header (`TOK TGT`: TGT + N; `TOK+N TGT`: TGT + N);
  - else an alias of port/gen/aliases.ld (`TOK = port_<mod>_<sec>+0xK`);
  - else a direct `port_<mod>_<sec>` spelling.
Its width comes from the instruction (suffix, x87 / SSE form), its kind from the operand position
(R read, W write, RW, A address taken: lea / `$SYM+N`). A HAZARD is a pair of accesses (not A) in
one function whose blob bytes overlap, where gcc thinks they cannot (different symbols, or the
same symbol at disjoint offsets), and at least one of them writes. `--all` also lists the
read-only pairs (harmless for ordering) and the address-taken pairs (an `&SYM+N` and another
object's access within 0x100 bytes after it: a pointer to one object used to reach the other's
bytes; listed for review, not proven).

Not visible here: two decls of the same assembler name in one unit (an asm-label decl such as
XDATA's next to a plain `extern` of that name are distinct objects to gcc). src/ has no XDATA
site today; if they come back, compare their sym with the plain spellings by hand.

--boot restricts the report to the functions reachable by direct calls from App_main plus the
title flow roots below (the boot walk of plan 17.8); the default lists every function and marks
the boot ones.
"""
import argparse
import collections
import concurrent.futures
import json
import os
import re
import shlex
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
BLOB_RE = re.compile(r"port_(\w+?)_(data|rodata|bss)$")
TGT_RE = re.compile(r"(port_\w+?_(?:data|rodata|bss))(?:\+0x([0-9A-Fa-f]+))?$")
BIND_RE = re.compile(r"/\*\s*port-bind:\s*(\S+)\s+(\w+)([+-]\d+)?\s+(\S+)\s*\*/")
SYM = r"[A-Za-z_][\w$]*(?:\.[\w$]+)*"
MEM_RE = re.compile(r"^(?:%\w+:)?(" + SYM + r")?([+-]\d+)?(\([^)]*\))?$")
IMM_RE = re.compile(r"^\$(%s)([+-]\d+)?$" % SYM)

# the title-flow roots of the plan 17.8 boot walk (reached through vtables / process tables, so a
# direct-call closure from App_main alone misses them)
BOOT_ROOTS = ["App_main", "app_runInit", "app_runOneFrame", "App_softReset", "SnowBoard_ctor",
              "GameFlow_create", "GameFlow_startMode", "GameFlow_restart_000116C8",
              "GameFlow_loadProcUso_000117FC", "Process_update", "Process_fadeOut_0000F954",
              "Fader_display", "Mem_init"]
# and every function of the N64-logo / title process modules
BOOT_UNITS = re.compile(r"src/(n64proc_uso|titproc_uso)/")

NOWRITE = ("cmp", "test", "bt", "ucomis", "comis", "push", "imul", "mul", "div", "idiv")
RMW = ("add", "sub", "and", "or", "xor", "adc", "sbb", "shl", "shr", "sar", "sal", "rol", "ror",
       "inc", "dec", "neg", "not", "xchg", "xadd")
STORE1 = ("fst", "fist", "fisttp", "set", "pop")


def width(mn):
    if mn.startswith(("movz", "movs")) and len(mn) == 6 and mn[4] in "bw":
        return {"b": 1, "w": 2}[mn[4]]
    if mn.startswith("fi"):                        # fild / fist / fistp / fisttp
        return 8 if mn.endswith("ll") else {"s": 2, "l": 4}.get(mn[-1], 4)
    if mn.startswith("f"):
        return {"s": 4, "l": 8, "t": 10}.get(mn[-1], 4)
    if mn in ("cvtss2sd", "cvttss2si", "cvtss2si") or mn.endswith("ss"):
        return 4
    if mn in ("cvtsd2ss", "cvttsd2si", "cvtsd2si") or mn.endswith("sd"):
        return 8
    if mn in ("movd",):
        return 4
    if mn in ("movq",):
        return 8
    if mn.startswith(("movap", "movup", "movdq")):
        return 16
    if mn.startswith("cvtsi2s"):
        return 4
    return {"b": 1, "w": 2, "l": 4, "q": 8}.get(mn[-1], 4)


def kind(mn, idx, nops):
    if mn.startswith("lea"):
        return "A"
    if mn.startswith(NOWRITE) or (mn.startswith("f") and not mn.startswith(STORE1)):
        return "R"
    if nops == 1:
        if mn.startswith(STORE1):
            return "W"
        if mn.startswith(RMW):
            return "RW"
        return "R"
    if idx != nops - 1:
        return "R"
    if mn.startswith(RMW):
        return "RW"
    return "W"


def split_ops(s):
    ops, depth, cur = [], 0, ""
    for ch in s:
        if ch == "(":
            depth += 1
        elif ch == ")":
            depth -= 1
        if ch == "," and depth == 0:
            ops.append(cur.strip())
            cur = ""
        else:
            cur += ch
    if cur.strip():
        ops.append(cur.strip())
    return ops


def parse_asm(text):
    """{function: [(sym, addend, width, kind)]}"""
    out = collections.defaultdict(list)
    cur = None
    for line in text.splitlines():
        if not line or line[0] not in "\t ":
            m = re.match(r"^(%s):" % SYM, line)
            if m and not m.group(1).startswith(".L"):
                cur = m.group(1).split(".")[0]
            continue
        s = line.strip()
        if not s or s.startswith((".", "#")) or cur is None:
            continue
        parts = s.split(None, 1)
        mn = parts[0]
        if mn.startswith(("call", "jmp", "j")) or len(parts) == 1:
            continue
        ops = split_ops(parts[1])
        for i, op in enumerate(ops):
            m = IMM_RE.match(op)
            if m:
                out[cur].append((m.group(1), int(m.group(2) or 0), 0, "A"))
                continue
            if op.startswith(("%", "$", "*")):
                continue
            m = MEM_RE.match(op)
            if m and m.group(1) and not m.group(1).startswith(".L"):
                k = kind(mn, i, len(ops))
                out[cur].append((m.group(1), int(m.group(2) or 0), 0 if k == "A" else width(mn), k))
    return out


def read_binds(hdr):
    binds = collections.defaultdict(dict)
    if hdr and os.path.exists(hdr):
        for m in BIND_RE.finditer(open(hdr).read()):
            fn, tok, n, tgt = m.groups()
            t = TGT_RE.match(tgt)
            if not t:
                continue
            val = (t.group(1), int(t.group(2) or "0", 16))
            if n is None:
                binds[fn][tok] = val
            else:
                binds[fn].setdefault(tok, {})[int(n)] = val
    return binds


def read_aliases(path):
    out = {}
    for m in re.finditer(r"^(\w+)\s*=\s*([^;]+);", open(path).read(), re.M):
        t = TGT_RE.match(m.group(2).strip())
        if t:
            out[m.group(1)] = (t.group(1), int(t.group(2) or "0", 16))
    return out


def resolve(fn, sym, n, binds, aliases):
    b = binds.get(fn, {}).get(sym)
    if isinstance(b, dict):
        b = b.get(n)
    if b is None:
        b = aliases.get(sym)
    if b is None and BLOB_RE.match(sym):
        b = (sym, 0)
    if b is None:
        return None
    return b[0], b[1] + n


def compile_s(entry, work):
    args = shlex.split(entry["command"])
    i = next(k for k, a in enumerate(args) if a.endswith("cc-select.py"))
    args = args[i + 1:]
    out, hdr, cmd, skip = None, None, [], 0
    for k, a in enumerate(args):
        if skip:
            skip -= 1
            continue
        if a in ("-MT", "-MF"):
            skip = 1
            continue
        if a in ("-MD", "-g", "-c"):
            continue
        if a == "-o":
            out = args[k + 1]
            skip = 1
            continue
        if a == "-include" and "/port/gen/tu/" in args[k + 1]:
            hdr = args[k + 1]
        cmd.append(a)
    src = entry["file"]
    dst = Path(work) / (re.sub(r"[^\w.]+", "_", os.path.relpath(src, ROOT)) + ".s")
    return src, hdr, dst, cmd + ["-S", "-w", "-o", str(dst)]


def call_graph(lib):
    """{function: {direct callees}} of the cc-select (renamed) objects in libgame.a"""
    txt = subprocess.run(["objdump", "-dr", "--no-show-raw-insn", lib], capture_output=True,
                         text=True).stdout
    g = collections.defaultdict(set)
    cur = None
    for line in txt.splitlines():
        m = re.match(r"^[0-9a-f]+ <([^>]+)>:", line)
        if m:
            cur = m.group(1).split(".")[0]
            continue
        m = re.search(r"R_386_(?:PC32|PLT32)\s+(\S+)", line)
        if m and cur:
            g[cur].add(m.group(1).split(".")[0])
    return g


def emit(funcs, rows, units, aliases):
    """The fix: every token of a hazard pair is spelled through its blob for the function
    (`#define TOK (*(__typeof__(TOK) *)(port_<mod>_<sec> + 0xK))` before it, #undef after it).
    gcc then sees one object at known offsets and keeps the order. An aliases.ld token means the
    same bytes in every function, a plain port-bind token the same bytes in this one; a
    per-addend bind (`TOK+N TGT`) cannot be one #define and is left for a hand spelling."""
    hdrs = {os.path.relpath(src, ROOT): hdr for src, hdr, _, _ in units}
    for fn in funcs:
        mine = [r for r in rows if r[2] == fn and r[0] == "HAZARD"]
        if not mine:
            print("/* %s: no hazard */" % fn)
            continue
        unit = mine[0][3]
        binds = read_binds(hdrs.get(unit)).get(fn, {})
        toks = sorted({x.split("+")[0].split("-")[0] for r in mine for x in (r[6], r[7])})
        defs, blobs, manual = [], set(), []
        # a per-addend bind token in the pairs is the canonical spelling where one of its addends
        # lands on the same bytes: `(char *)&TOK + N` keeps the addend cc-select renames
        perad = {tok: binds[tok] for tok in toks if isinstance(binds.get(tok), dict)}
        for tok in toks:
            b = binds.get(tok)
            if isinstance(b, dict):
                continue
            b = b or aliases.get(tok)
            if b is None:
                continue                          # a direct port_* spelling already
            via = [(pt, n) for pt, d in perad.items() for n, tg in d.items()
                   if tg[0] == b[0] and tg[1] + n == b[1]]
            if via:
                defs.append("#define %s (*(__typeof__(%s) *)((char *)&%s + %d))" % (tok, tok, *via[0]))
                continue
            blobs.add(b[0])
            defs.append("#define %s (*(__typeof__(%s) *)(%s + 0x%X))" % (tok, tok, b[0], b[1]))
        if perad and not any("&" in d for d in defs):
            manual += sorted(perad)
        ext = 'extern "C" ' if unit.endswith(".cpp") else "extern "
        print("/* %s (%s) */" % (fn, unit))
        print("#ifdef PORT /* one object for the bytes these tokens share, so gcc keeps the order"
              " (scripts/port-alias-audit.py, plan 18.7) */")
        for b in sorted(blobs):
            print("%schar %s[];" % (ext, b))
        print("\n".join(defs))
        print("#endif")
        print("#ifdef PORT\n" + "\n".join("#undef %s" % d.split()[1] for d in defs) + "\n#endif")
        if manual:
            print("/* per-addend binds, spell by hand: %s */" % " ".join(manual))


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("build", help="the port's cmake build directory (ninja)")
    ap.add_argument("--work", default="/tmp/claude-1000/port-alias-audit")
    ap.add_argument("-j", type=int, default=8)
    ap.add_argument("--reuse", action="store_true", help="keep existing .s files in --work")
    ap.add_argument("--boot", action="store_true", help="only the boot / title-flow functions")
    ap.add_argument("--all", action="store_true", help="also read-only and address-taken pairs")
    ap.add_argument("--emit", nargs="+", metavar="FUNC",
                    help="print the PORT respelling blocks for these functions (see below)")
    a = ap.parse_args()
    os.makedirs(a.work, exist_ok=True)
    db = json.loads(subprocess.run(["ninja", "-C", a.build, "-t", "compdb"], capture_output=True,
                                   text=True, check=True).stdout)
    units = [compile_s(e, a.work) for e in db if "cc-select.py" in e["command"]]

    def run(u):
        src, hdr, dst, cmd = u
        if a.reuse and dst.exists():
            return 0
        return subprocess.run(cmd, cwd=a.build, capture_output=True).returncode
    with concurrent.futures.ThreadPoolExecutor(a.j) as ex:
        rcs = list(ex.map(run, units))
    bad = [u[0] for u, rc in zip(units, rcs) if rc]
    if bad:
        print("# %d units failed to compile: %s" % (len(bad), " ".join(bad[:5])), file=sys.stderr)
    aliases = read_aliases(ROOT / "port/gen/aliases.ld")
    graph = call_graph(os.path.join(a.build, "libgame.a"))
    boot, todo = set(), list(BOOT_ROOTS)
    for src, hdr, dst, _ in units:
        if BOOT_UNITS.search(src) and dst.exists():
            todo += parse_asm(dst.read_text(errors="replace")).keys()
    while todo:
        f = todo.pop()
        if f not in boot:
            boot.add(f)
            todo += graph.get(f, ())
    rows = []
    for src, hdr, dst, _ in units:
        if not dst.exists():
            continue
        binds = read_binds(hdr)
        for fn, refs in parse_asm(dst.read_text(errors="replace")).items():
            acc = []
            for sym, n, w, k in refs:
                r = resolve(fn, sym, n, binds, aliases)
                if r:
                    acc.append((sym, n, w, k, r[0], r[1]))
            seen = set()
            for i in range(len(acc)):
                for j in range(i + 1, len(acc)):
                    s1, n1, w1, k1, b1, o1 = acc[i]
                    s2, n2, w2, k2, b2, o2 = acc[j]
                    if b1 != b2:
                        continue
                    if "A" in (k1, k2):
                        if k1 == k2 or not a.all or s1 == s2:
                            continue
                        base, o = (o1, o2) if k1 == "A" else (o2, o1)
                        if not (base <= o < base + 0x100):
                            continue
                        cls = "addr"
                    else:
                        if not (o1 < o2 + w2 and o2 < o1 + w1):
                            continue                         # disjoint at run time
                        if s1 == s2 and n1 < n2 + w2 and n2 < n1 + w1:
                            continue                         # gcc sees the overlap too
                        cls = "HAZARD" if "W" in k1 + k2 else "read"
                        if cls == "read" and not a.all:
                            continue
                    key = (cls, s1, n1, k1, s2, n2, k2)
                    if key in seen:
                        continue
                    seen.add(key)
                    rows.append((cls, fn in boot, fn, os.path.relpath(src, ROOT), b1, min(o1, o2),
                                 "%s%+d %s%d" % (s1, n1, k1, w1), "%s%+d %s%d" % (s2, n2, k2, w2)))
    rows.sort(key=lambda r: (r[0] != "HAZARD", not r[1], r[3], r[2]))
    if a.emit:
        emit(a.emit, rows, units, aliases)
        return
    print("# class\tboot\tfunction\tunit\tblob\toffset\tref 1\tref 2")
    for r in rows:
        if a.boot and not r[1]:
            continue
        print("%s\t%s\t%s\t%s\t%s\t0x%X\t%s\t%s" % (r[0], "boot" if r[1] else "-", *r[2:]))
    fns = collections.defaultdict(set)
    for r in rows:
        fns[(r[0], r[1])].add(r[2])
    print("# functions: " + ", ".join("%s%s %d" % (c, " (boot)" if b else "", len(v))
                                     for (c, b), v in sorted(fns.items())), file=sys.stderr)


if __name__ == "__main__":
    main()
