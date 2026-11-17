#!/usr/bin/env python3
"""PC port: list the loads / stores that touch PART of a multi-byte field (docs/1080_PC_PORT_PLAN.md
18.12). On big-endian MIPS a 16-bit read at +2 of an int is its low half; on little-endian x86 the
same C (`((short *)p)[1]`) reads the high half. The blob typing (18.1) cannot fix these: they are
in code. Found first in FogParms_emit (run 819: int min / max read as shorts at +0x12 / +0x16).

    scripts/port-partial-read-audit.py [BUILD_DIR] [--lib LIBGAME_A] [--boot] [--tsv OUT] [--all]

Method (the ROM-exact objects, so the widths are the ones the matched C really uses):
  - every function of build/src/**/*.o is disassembled (objdump -dr, no-aliases) and a forward
    dataflow over its basic blocks gives each register a symbolic value `(root, offset)`:
      roots are the incoming registers (a0..a3, s0..s8), `sp`, a %hi/%lo symbol (`&sym`), an
      absolute KSEG address (`@abs`), a call result (`ret@pc`), or a loaded word `[root+off]`
      (pointer chains, up to 4 deep); addiu / addu of a constant / move keep the root; spills to
      the frame (sw / lw at the same sp offset) are tracked; a merge of two different values
      forgets the register;
  - every load / store then is an access (root, offset, width, L/S). Roots built from a global
    symbol (`&gGfx`, `[&gGfx+0x10]`, ...) are compared across ALL functions of the segment; the
    others within their function, except that a root built from a0..a3 is also compared across
    the functions declaring that parameter as one named struct type (`Foo *p`), and a0 across a
    `Class_method` family (FogParms_set's int field vs FogParms_emit's `short *` read). Functions
    the port does not compile (port/gen/excluded.tsv) and the second copy of a donor are
    skipped. The argument home slots (sp offsets past the frame) are skipped:
    IDO stores a short parameter as a word and reads its half (codegen, not C);
  - for each 4-byte window of a root that has both a word access (lw / sw / lwc1 / swc1) and a
    narrower one (lh / lhu / sh / lb / lbu / sb) inside it:
      narrow-in-word   the narrow accesses all sit at ONE sub-offset: a field read / written
                       partly (an int read as a short: FogParms);
      word-over-narrow two or more sub-offsets are accessed narrow: the word spans s16 / u8
                       fields (a lw covering two s16);
      harmless         every word access is a copy (lw only stored back with sw, or sw of such a
                       value: struct copies), or stores a byte-symmetric constant (0, -1);
    and for each 2-byte window with a half and a byte access inside it: byte-in-half.
  - the C side: the source of each hit function is shown with its sub-word casts, and
    `--csrc` lists every `*(s16 *)&x`, `((u8 *)&x)[n]`, `(short *)` / `(char *)` re-view and
    `union` in src/ (the spellings that make such accesses), marked when the function has a hit.

Not covered: a narrow access whose wide counterpart is in a function with neither a shared
parameter type nor class prefix (caller-side widths: calls go through placeholder relocs, so
there is no interprocedural link), `D_00000000` data references with addend 0 (one root per
site: their targets are only named in C, by XDATA), the IDO argument home slots (a C
`((u16 *)&param)[1]` there is caught by the C scan instead), bitfield containers (a u32 bitfield
word next to u8 fields shows as narrow-in-word: consistent C, plan 18.3), and 8-byte words.

--boot keeps the functions reachable by direct calls (in the port's libgame.a) from the boot /
title roots of plan 17.8 and every function of bootup / gui / n64proc / titproc (their
nodes' methods are reached through vtables); the default lists all and
marks the boot ones with `B`.
"""
import argparse
import collections
import concurrent.futures
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
OBJDUMP = str(Path.home() / ".local/bin/mips-linux-gnu-objdump")

LOADS = {"lb": 1, "lbu": 1, "lh": 2, "lhu": 2, "lw": 4, "lwc1": 4, "ll": 4, "lwu": 4,
         "ld": 8, "ldc1": 8}
STORES = {"sb": 1, "sh": 2, "sw": 4, "swc1": 4, "sc": 4, "sd": 8, "sdc1": 8}
UNALIGNED = {"lwl", "lwr", "swl", "swr", "ldl", "ldr", "sdl", "sdr"}
NODEST = {"j", "jr", "jal", "jalr", "b", "bal", "mult", "multu", "div", "divu", "dmult", "dmultu",
          "ddiv", "ddivu", "mthi", "mtlo", "mtc1", "dmtc1", "ctc1", "mtc0", "dmtc0", "sync",
          "cache", "break", "syscall", "nop", "teq", "tne", "tge", "tlt", "tgeu", "tltu",
          "teqi", "tnei", "eret", "tlbp", "tlbr", "tlbwi", "tlbwr"} | set(STORES) | UNALIGNED
CALLER_SAVED = ["at", "v0", "v1", "a0", "a1", "a2", "a3", "t0", "t1", "t2", "t3", "t4", "t5",
                "t6", "t7", "t8", "t9"]
ENTRY = ["a0", "a1", "a2", "a3", "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7", "s8", "gp"]
PLACEHOLDER = re.compile(r"^(D_00000000|\w*_00000000)$")
BOOT_ROOTS = ["App_main", "app_runInit", "app_runOneFrame", "App_softReset", "SnowBoard_ctor",
              "GameFlow_create", "GameFlow_startMode", "GameFlow_restart_000116C8",
              "GameFlow_loadProcUso_000117FC", "Process_update", "Process_fadeOut_0000F954",
              "Fader_display", "Mem_init", "App_Display", "SnowBoard_InitInstance"]
BOOT_UNITS = re.compile(r"src/(bootup_uso|n64proc_uso|titproc_uso|gui_uso)/")
AGENT_G = re.compile(r"^(RcpMgr_|gfx_|Gfx_|GfxLib_)")

INSN_RE = re.compile(r"^\s*([0-9a-f]+):\s+(\S+)\s*(.*)$")
FUNC_RE = re.compile(r"^([0-9a-f]+) <([^>]+)>:$")
RELOC_RE = re.compile(r"^\s*([0-9a-f]+):\s+(R_MIPS_\w+)\s+(\S+)$")
MEM_RE = re.compile(r"^(-?\w+)\((\w+)\)$")


def parse_obj(path):
    """{func: [(pc, op, [operands], reloc or None)]}"""
    txt = subprocess.run([OBJDUMP, "-dr", "--no-show-raw-insn", "-M", "no-aliases", str(path)],
                         capture_output=True, text=True).stdout
    funcs, cur, sect = {}, None, None
    for line in txt.splitlines():
        if line.startswith("Disassembly of section"):
            sect = line.split()[-1].rstrip(":")
            cur = None
            continue
        if sect != ".text":
            continue
        m = FUNC_RE.match(line)
        if m:
            cur = funcs.setdefault(m.group(2), [])
            continue
        if cur is None:
            continue
        m = RELOC_RE.match(line)
        if m:
            if cur:
                pc, op, ops, _ = cur[-1]
                cur[-1] = (pc, op, ops, (m.group(2), m.group(3)))
            continue
        m = INSN_RE.match(line)
        if m:
            ops = [o.strip() for o in m.group(3).split(",")] if m.group(3) else []
            cur.append((int(m.group(1), 16), m.group(2), ops, None))
    return funcs


def imm(s):
    return int(s, 0)


def is_branch(op):
    return (op.startswith("b") and op not in ("break",)) or op in ("j", "jr")


def branch_target(op, ops):
    if op == "jr":
        return None
    t = ops[-1].split()[0] if ops else ""
    try:
        return int(t, 16)
    except ValueError:
        return None


def unconditional(op, ops):
    return op in ("j", "jr", "b") or (op == "beq" and len(ops) == 3 and ops[0] == ops[1] == "zero")


class Func:
    def __init__(self, name, seg, insns, unit=""):
        self.name, self.seg, self.insns, self.unit = name, seg, insns, unit
        self.frame = 0
        for _, op, ops, _ in insns[:4]:
            if op == "addiu" and ops[:2] == ["sp", "sp"] and imm(ops[2]) < 0:
                self.frame = -imm(ops[2])
                break

    def blocks(self):
        ins = self.insns
        if not ins:
            return [], {}
        base = ins[0][0]
        idx = {pc: i for i, (pc, _, _, _) in enumerate(ins)}
        leaders = {0}
        for i, (pc, op, ops, _) in enumerate(ins):
            if is_branch(op):
                t = branch_target(op, ops)
                if t is not None and t in idx:
                    leaders.add(idx[t])
                if i + 2 < len(ins):
                    leaders.add(i + 2)
        ls = sorted(leaders)
        blocks = []
        for k, s in enumerate(ls):
            e = ls[k + 1] if k + 1 < len(ls) else len(ins)
            blocks.append((s, e))
        start = {s: n for n, (s, e) in enumerate(blocks)}
        succ = {}
        for n, (s, e) in enumerate(blocks):
            out = []
            last = None
            for i in (e - 2, e - 1):
                if s <= i < e and is_branch(ins[i][1]):
                    last = i
                    break
            if last is None:
                if e < len(ins):
                    out.append(start[e])
            else:
                op, ops = ins[last][1], ins[last][2]
                t = branch_target(op, ops)
                if t is not None and t in idx and idx[t] in start:
                    out.append(start[idx[t]])
                if not unconditional(op, ops) and e < len(ins) and e in start:
                    out.append(start[e])
            succ[n] = out
        del base
        return blocks, succ


def val_str(v):
    root, off = v
    return "[%s%+#x]" % (root, off) if off else "[%s]" % root


def step(f, i, st, rec=None):
    """transfer one instruction; rec(kind, root, off, width, pc, op, extra) collects accesses"""
    pc, op, ops, rel = f.insns[i]
    g = lambda r: ("#", 0) if r == "zero" else st.get(r)
    if op in LOADS or op in STORES or op in UNALIGNED:
        if len(ops) < 2:
            return
        m = MEM_RE.match(ops[1])
        if not m:
            return
        try:
            d = imm(m.group(1))
        except ValueError:
            return
        b = g(m.group(2))
        acc = None
        if b is not None:
            root, off = b
            if root == "#":
                acc = ("@abs", off + d) if (off + d) & 0xFFFFFFFF >= 0x80000000 else None
            else:
                acc = (root, off + d)
        w = LOADS.get(op) or STORES.get(op) or 4
        if acc and rec:
            rec("L" if op in LOADS or op.startswith("l") else "S", acc[0], acc[1], w, pc, op,
                ops[0])
        if op in LOADS:
            dst = ops[0]
            if dst.startswith("f") or dst.startswith("$f"):
                pass
            elif acc and acc[0] == "sp" and w == 4 and ("S%d" % acc[1]) in st:
                st[dst] = st["S%d" % acc[1]]
            elif acc and w == 4 and acc[0].count("[") < 4 and acc[0] != "sp":
                st[dst] = (val_str(acc), 0)
            else:
                st.pop(dst, None)
        elif op in UNALIGNED and op.startswith("l"):
            st.pop(ops[0], None)
        elif op in STORES and acc and acc[0] == "sp":
            key = "S%d" % acc[1]
            v = g(ops[0])
            if w == 4 and v is not None and not ops[0].startswith("f"):
                st[key] = v
            else:
                st.pop(key, None)
        return
    if op in ("jal", "jalr", "bal", "bgezal", "bltzal"):
        for r in CALLER_SAVED:
            st.pop(r, None)
        st["v0"] = ("ret@%x" % pc, 0)
        return
    if op in NODEST or is_branch(op) or not ops:
        return
    dst = ops[0]
    if dst.startswith("f") or dst.startswith("$f") or dst in ("hi", "lo"):
        return
    nv = None
    try:
        if op == "lui":
            v = imm(ops[1]) << 16
            if rel and rel[0] == "R_MIPS_HI16":
                sym = rel[1]
                root = ("&%s@%x" % (sym, pc)) if PLACEHOLDER.match(sym) else "&" + sym
                if sym.startswith("."):
                    root = "&%s@%s" % (sym, f.unit)          # a section symbol: this object only
                if PLACEHOLDER.match(sym) and v:
                    root = "&%s:%s" % (f.seg, sym)
                nv = (root, v)
            else:
                nv = ("#", v)
        elif op in ("addiu", "addi", "daddiu", "daddi"):
            b = g(ops[1])
            if b is not None:
                k = imm(ops[2])
                if rel and rel[0] == "R_MIPS_LO16" and b[0] == "#" and not b[1]:
                    sym = rel[1]
                    nv = (("&%s@%x" % (sym, pc)) if PLACEHOLDER.match(sym) else
                          ("&%s@%s" % (sym, f.unit)) if sym.startswith(".") else "&" + sym, k)
                else:
                    nv = (b[0], b[1] + k)
                    if b[0] == "#":
                        nv = ("#", (b[1] + k) & 0xFFFFFFFF)
        elif op == "ori":
            b = g(ops[1])
            if b is not None and b[0] == "#":
                nv = ("#", b[1] | imm(ops[2]))
            elif b is not None and rel and rel[0] == "R_MIPS_LO16":
                nv = (b[0], b[1] + imm(ops[2]))
        elif op in ("or", "addu", "daddu", "move", "xor"):
            a, b = g(ops[1]), g(ops[2]) if len(ops) > 2 else ("#", 0)
            if a is not None and b is not None:
                if op == "xor" and not (a == ("#", 0) or b == ("#", 0)):
                    nv = None
                elif a[0] == "#" and b[0] == "#":
                    nv = ("#", (a[1] | b[1]) if op == "or" else (a[1] + b[1]) & 0xFFFFFFFF)
                elif b[0] == "#" and (op != "or" or b[1] == 0):
                    nv = (a[0], a[1] + b[1])
                elif a[0] == "#" and (op != "or" or a[1] == 0):
                    nv = (b[0], b[1] + a[1])
    except ValueError:
        nv = None
    if nv is None:
        st.pop(dst, None)
    else:
        st[dst] = nv


def analyse(f):
    blocks, succ = f.blocks()
    if not blocks:
        return []
    entry = {r: (r, 0) for r in ENTRY}
    entry["sp"] = ("sp", 0)
    ins_state = {0: dict(entry)}
    visits = collections.Counter()
    work = [0]
    while work:
        n = work.pop()
        visits[n] += 1
        if visits[n] > 30:
            continue
        st = dict(ins_state[n])
        s, e = blocks[n]
        for i in range(s, e):
            if f.insns[i][1] == "addiu" and f.insns[i][2][:2] == ["sp", "sp"]:
                continue                 # keep sp the frame base (prologue / epilogue)
            step(f, i, st)
        for m in succ.get(n, ()):
            old = ins_state.get(m)
            new = st if old is None else {k: v for k, v in old.items() if st.get(k) == v}
            if old is None or new != old:
                ins_state[m] = dict(new)
                work.append(m)
    accs = []
    for n, (s, e) in enumerate(blocks):
        if n not in ins_state:
            continue
        st = dict(ins_state[n])
        live = {}                              # reg -> index into accs of the load that set it

        def rec(kind, root, off, w, pc, op, reg, _l=live):
            accs.append([kind, root, off, w, pc, op, reg, set()])
        for i in range(s, e):
            pc, op, ops, _ = f.insns[i]
            if op == "addiu" and ops[:2] == ["sp", "sp"]:
                continue
            # uses of loaded values (copy detection)
            isstore = op in STORES or op in ("swl", "swr")
            srcs = ops[1:] if not isstore else ops[:1] + [MEM_RE.match(ops[1]).group(2)
                                                              if len(ops) > 1 and MEM_RE.match(ops[1]) else ""]
            if isstore:
                val = ops[0]
                if val in live:
                    accs[live[val]][7].add("copied")
                    # the store copies a loaded value
                    st_copy = True
                else:
                    st_copy = False
                srcs = srcs[1:]
            for r in srcs:
                r = MEM_RE.match(r).group(2) if MEM_RE.match(r) else r
                if r in live:
                    accs[live[r]][7].add("used")
            nacc = len(accs)
            v = st.get(ops[0]) if op in STORES and ops and ops[0] != "zero" else (
                ("#", 0) if op in STORES and ops and ops[0] == "zero" else None)
            step(f, i, st, rec)
            if len(accs) > nacc:
                a = accs[-1]
                if op in STORES:
                    if st_copy:
                        a[7].add("copy")
                    if v is not None and v[0] == "#":
                        a[7].add("const:%#x" % (v[1] & 0xFFFFFFFF))
                    if ops[0].startswith("f"):
                        a[7].add("float")
            if op in LOADS or (ops and op not in NODEST and not is_branch(op)):
                if ops:
                    live.pop(ops[0], None)
            if op in LOADS and len(accs) > nacc and not ops[0].startswith("f"):
                live[ops[0]] = len(accs) - 1
            if op in ("jal", "jalr"):
                for r in CALLER_SAVED:
                    live.pop(r, None)
        for r, k in live.items():
            accs[k][7].add("used")
    out = []
    for kind, root, off, w, pc, op, reg, tags in accs:
        if root == "sp" and off >= f.frame:
            continue                               # argument home slots
        if root.startswith("ret@") and not root.startswith("["):
            pass
        out.append((kind, root, off, w, pc, op, tags))
    return out


def innermost(root):
    return root.lstrip("[").split("+")[0].split("-")[0].rstrip("]")


def is_global(root):
    r = innermost(root)
    return (r.startswith("&") and "@" not in r) or r == "@abs" or ("@src/" in r)


GENERIC = {"D", "Proc", "Process", "Node", "Obj", "Func", "Backup", "Audio", "AudioSeq",
           "AudioHeap", "AudioLoad", "AudioSynth", "AudioThread"}   # module families, not classes


def class_of(name):
    """`Class_method...` -> Class: functions of one class prefix take the same `this` in a0
    (the C++ donors and the C families named after them), so their a0 roots are compared together.
    Node / Process are base classes whose a0 is any subclass, and Audio* / Backup are module
    prefixes over several structs: they are kept per function."""
    m = re.match(r"^([A-Z][A-Za-z0-9]+)_", name)
    if not m or m.group(1) in GENERIC or re.match(r"^[A-Z]_", name):
        return None
    return m.group(1)


def symmetric(c):
    c &= 0xFFFFFFFF
    b = c.to_bytes(4, "big")
    return len(set(b)) == 1


def classify(word, narrow):
    """word / narrow: lists of (func, kind, off, w, pc, op, tags)"""
    harmless = []
    for a in word:
        tags = a[6]
        if a[5] in UNALIGNED:
            harmless.append("unaligned copy")     # lwl / lwr / swl / swr: a memcpy of bytes
        elif "copy" in tags or ("copied" in tags and "used" not in tags):
            harmless.append("copy")
        elif any(t.startswith("const:") and symmetric(int(t[6:], 16)) for t in tags):
            harmless.append("const")
        else:
            harmless.append(None)
    if all(harmless):
        return "harmless"
    subs = {(a[2] & 3, a[3]) for a in narrow}
    if len({s for s, _ in subs}) >= 2:
        return "word-over-narrow"
    return "narrow-in-word"


SCALAR = {"char", "short", "int", "long", "void", "float", "double", "u8", "s8", "u16", "s16",
          "u32", "s32", "u64", "s64", "f32", "f64", "unsigned", "signed", "Gfx", "Mtx", "Vtx",
          "OSMesgQueue", "OSMesg", "OSThread", "OSTask", "OSIoMesg", "OSPfs", "OSContPad",
          "OSContStatus", "OSTime", "OSTimer", "uintptr_t", "size_t", "FILE"}
DEF_RE = re.compile(r"^(?!static\s+inline)[A-Za-z_][\w\s\*]*?\b(\w+)\s*\(([^;{]*)\)\s*\{?\s*$")


def param_types():
    """{func: {aN: struct type}} from the C definitions: the a0..a3 that point to one named,
    non-scalar type (`Foo *p`, `struct Foo *p`). Functions taking the same type compare their
    accesses through it (FogParms_set's `int` field vs FogParms_emit's short)."""
    out = {}
    for c in list(ROOT.glob("src/**/*.c")) + list(ROOT.glob("src/**/*.cpp")):
        for l in c.read_text(errors="replace").splitlines():
            m = DEF_RE.match(l)
            if not m or l.startswith(("if", "while", "for", "switch", "return", "else")):
                continue
            types = {}
            for i, prm in enumerate(m.group(2).split(",")[:4]):
                t = re.match(r"^\s*(?:const\s+)?(?:struct\s+|union\s+)?(\w+)\s*\*\s*\w*\s*$", prm)
                if t and t.group(1) not in SCALAR and "float" not in prm and "double" not in prm:
                    types["a%d" % i] = t.group(1)
                elif "float" in prm or "double" in prm:
                    break                # FPU arguments shift the a-register numbering
            if types:
                out.setdefault(m.group(1), types)
    return out


def unit_of(path):
    p = str(path.relative_to(ROOT / "build"))
    return p[:-2] if p.endswith(".o") else p


def boot_set(lib, funcs_by_unit):
    g = collections.defaultdict(set)
    if lib and Path(lib).exists():
        txt = subprocess.run(["objdump", "-dr", "--no-show-raw-insn", lib], capture_output=True,
                             text=True).stdout
        cur = None
        for line in txt.splitlines():
            m = re.match(r"^[0-9a-f]+ <([^>]+)>:", line)
            if m:
                cur = m.group(1).split(".")[0]
                continue
            m = re.search(r"R_386_(?:PC32|PLT32)\s+(\S+)", line)
            if m and cur:
                g[cur].add(m.group(1).split(".")[0])
    todo = list(BOOT_ROOTS)
    for u, fs in funcs_by_unit.items():
        if BOOT_UNITS.search(u):
            todo += fs
    boot = set()
    while todo:
        f = todo.pop()
        if f not in boot:
            boot.add(f)
            todo += g.get(f, ())
    return boot


CSRC_RE = re.compile(
    r"\*\s*\(\s*(?:volatile\s+)?(?:s16|u16|short|unsigned short|s8|u8|char|unsigned char|signed char)\s*\*\s*\)\s*&"
    r"|\(\s*\(\s*(?:u8|s8|char|unsigned char|u16|s16|short|unsigned short)\s*\*\s*\)\s*&[^)]*\)\s*\["
    r"|\bunion\b")


def find_func_src(src, name):
    """(start line, end line) of the definition of name in src text lines"""
    pat = re.compile(r"^[^\s#/][^;=]*\b%s\s*\(" % re.escape(name))
    for i, l in enumerate(src):
        if pat.match(l) and not l.rstrip().endswith(";"):
            j = i
            depth, seen = 0, False
            while j < len(src):
                depth += src[j].count("{") - src[j].count("}")
                seen = seen or "{" in src[j]
                if seen and depth <= 0:
                    return i, j
                j += 1
            return i, len(src) - 1
    return None


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("build", nargs="?", default=str(ROOT / "build"))
    ap.add_argument("--lib", default="/tmp/claude-1000/agent-g/pbuild/libgame.a",
                    help="the port's libgame.a (x86), for the boot call graph")
    ap.add_argument("--boot", action="store_true")
    ap.add_argument("--all", action="store_true", help="also list the harmless windows")
    ap.add_argument("--tsv")
    ap.add_argument("--csrc", action="store_true", help="list the C sub-word re-view spellings")
    ap.add_argument("-j", type=int, default=8)
    ap.add_argument("--dump", help=argparse.SUPPRESS)
    a = ap.parse_args()

    smap, excl = {}, set()
    for l in (ROOT / "port/sourcemap.tsv").read_text().splitlines()[2:]:
        c = l.split("\t")
        if len(c) > 2:
            smap[c[0]] = c[2]
    for l in (ROOT / "port/gen/excluded.tsv").read_text().splitlines()[2:]:
        excl.add(l.split("\t")[0])
    ptypes = param_types()
    objs = sorted(p for p in Path(a.build, "src").rglob("*.o")
                  if "non_matching" not in str(p))
    with concurrent.futures.ThreadPoolExecutor(a.j) as ex:
        parsed = list(ex.map(parse_obj, objs))
    funcs_by_unit = {}
    allacc = collections.defaultdict(list)     # key -> [(func, unit, kind, off, w, pc, op, tags)]
    for path, funcs in zip(objs, parsed):
        u = unit_of(path)
        seg = u.split("/")[1] if u.count("/") >= 2 else u
        funcs_by_unit[u] = list(funcs)
        for name, insns in funcs.items():
            if name in excl or (name in smap and smap[name] != u):
                continue                  # not compiled by the port / a second copy of a donor
            f = Func(name, seg, insns, u)
            for kind, root, off, w, pc, op, tags in analyse(f):
                cls = class_of(name)
                arg = innermost(root)
                ptype = ptypes.get(name, {}).get(arg)
                keys = []
                if is_global(root):
                    keys.append((seg, root))
                else:
                    if ptype:
                        keys.append((seg, ptype + " *", root.replace(arg, "p", 1)))
                    if cls and arg == "a0":
                        keys.append((seg, cls + "::", root.replace("a0", "this", 1)))
                    if not keys:
                        keys.append((u, name, root))
                for key in keys:
                    allacc[key].append((name, u, kind, off, w, pc, op, tags))
    boot = boot_set(a.lib, funcs_by_unit)

    hits = []
    if a.dump:
        import pickle
        pickle.dump(dict(allacc), open(a.dump, "wb"))
    for key, acc in allacc.items():
        win = collections.defaultdict(lambda: ([], [], []))   # word base -> (words, halves, bytes)
        for x in acc:
            off, w = x[3], x[4]
            if w == 4 and off % 4 == 0:
                win[off][0].append(x)
            elif w == 2:
                win[off & ~3][1].append(x)
            elif w == 1:
                win[off & ~3][2].append(x)
        for wb, (words, halves, bytes_) in win.items():
            if words and (halves or bytes_):
                cls = classify([(x[0], x[2], x[3], x[4], x[5], x[6], x[7]) for x in words],
                               [(x[0], x[2], x[3], x[4], x[5], x[6], x[7]) for x in halves + bytes_])
                hits.append((cls, key, wb, words, halves + bytes_))
            for hb in (wb, wb + 2):
                hs = [x for x in halves if x[3] == hb]
                bs = [x for x in bytes_ if x[3] in (hb, hb + 1)]
                if hs and bs and not words:
                    hits.append(("byte-in-half", key, hb, hs, bs))

    counts = collections.Counter()
    fcounts = collections.defaultdict(set)
    rows = []
    seenhit = set()
    for cls, key, wb, wide, narrow in sorted(hits, key=lambda h: (h[0], str(h[1]), h[2])):
        sig = frozenset((x[1], x[5]) for x in wide + narrow)
        if sig in seenhit:
            continue
        seenhit.add(sig)
        fns = sorted({x[0] for x in wide + narrow})
        units = sorted({x[1] for x in wide + narrow})
        isboot = any(fn in boot for fn in fns)
        if a.boot and not isboot:
            continue
        counts[cls] += 1
        for fn in fns:
            fcounts[cls].add(fn)
        if cls == "harmless" and not a.all:
            continue
        root = key[-1]
        desc = lambda xs: " ".join("%s:%s@%+#x(%s)%s" % (x[0], x[6], x[3], "%x" % x[5],
                                                       "".join("/" + t for t in sorted(x[7])))
                                   for x in xs[:8]) + (" ..." if len(xs) > 8 else "")
        rows.append((cls, "B" if isboot else "-",
                     "G" if any(AGENT_G.match(fn) for fn in fns) else "-",
                     ",".join(units), root, "%#x" % wb, desc(wide), desc(narrow)))
    print("# port-partial-read-audit: %d objects, %d functions, %d roots" %
          (len(objs), sum(len(v) for v in funcs_by_unit.values()), len(allacc)))
    for cls in ("narrow-in-word", "word-over-narrow", "byte-in-half", "harmless"):
        print("# %-17s %4d windows, %4d functions" % (cls, counts[cls], len(fcounts[cls])))
    print("# class\tboot\tagent-g\tunit\troot\twindow\twide accesses\tnarrow accesses")
    for r in rows:
        print("\t".join(r))
    if a.tsv:
        Path(a.tsv).write_text("".join("\t".join(r) + "\n" for r in rows))
    if a.csrc:
        hitfns = set().union(*fcounts.values()) if fcounts else set()
        print("\n# C sub-word re-views (src/, `*` = function has an object hit)")
        n = 0
        for c in sorted(ROOT.glob("src/**/*.c")) + sorted(ROOT.glob("src/**/*.cpp")):
            lines = c.read_text(errors="replace").splitlines()
            cur = None
            for i, l in enumerate(lines):
                m = re.match(r"^[^\s#/][^;=]*?\b(\w+)\s*\([^;]*$", l)
                if m:
                    cur = m.group(1)
                if CSRC_RE.search(l):
                    n += 1
                    print("%s%s:%d\t%s\t%s" % ("*" if cur in hitfns else " ",
                                               c.relative_to(ROOT), i + 1, cur, l.strip()[:140]))
        print("# %d C lines" % n)


if __name__ == "__main__":
    sys.exit(main())
