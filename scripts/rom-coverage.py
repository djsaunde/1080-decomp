#!/usr/bin/env python3
"""ROM coverage audit: assign EVERY byte of baserom.z64 to exactly one category.

Why: the project once claimed "3288/3288 = 100%" while eight code-bearing USO
modules (~108 KB of code) had never been split, because that 100% only covered
the segments the build config defined.  This audit starts from the ROM, not from
the build config, so a module the config forgot shows up as a gap.

Categories (each byte gets exactly one):
  code-c          code compiled from C / C++ (plain, donor, match_import bodies;
                  port-sourcemap.py decides which body ships)
  code-asm        code from assembly, split three ways:
                    src       hand-written .s / libultra asm (GLOBAL_ASM sources;
                              "decompiled" in the sense that it IS the source)
                    undecomp  INCLUDE_ASM functions not yet decompiled
                    vendor    IPL3 boot code (Nintendo, not game code)
  data-c          initialised data / rodata emitted by a C object
  data-asm        data from asm or binary blobs: ROM header, splat bin data, USO
                  Data / RoData records of code modules, USO metadata (record
                  headers, Info, Sym, *Reloc, EntrySym)
  asset           data-only USOs (audio banks / sequences / samples, textures,
                  meshes, maps) and the rom/*.bin files (BinaryDataSection)
  ucode           RSP microcode text / data (F3DEX family, SGI gfx, aspMain)
  padding         alignment zeros, 0xFF fill, device / ROM tail fill
  GAP             none of the above: the audit could not attribute the byte

Sources of truth:
  * ROM filesystem: kernel device table -> devices -> USO files -> records
    (port/tools/extract/extract.py's walker, Yay0 decoded);
  * build/tenshoe.map: which object (C .o, .s .o, bin .o, linker fill) produced
    each linked ROM byte (the ROM is byte-identical, so the map IS the ROM);
  * report.json + objdiff.json: every function (unit, offset, size);
  * port-sourcemap.py (rerun into a scratch dir): which body ships per function
    (c / cpp / asm; unresolved = INCLUDE_ASM, not decompiled);
  * the Makefile's Yay0 block recipes: which C objects form each compressed code
    block.  The ROM links the block the C build compressed (crunch64 reproduces the
    original Yay0 stream), and this audit checks that provenance, so the
    byte-identical ROM check proves the C; it also lays the functions out inside the
    decompressed block and word-compares them against it.

Hidden code: every Data / RoData / BinaryDataSection payload (decompressed when
Yay0) and every non-code linked blob is scanned for MIPS function shapes
(`addiu sp,sp,-N` followed by `sw ra,K(sp)` and a later `jr ra`).  A section with
any hit is reported (--check fails unless it is allowlisted).

Allowlist: rom-coverage.allow (project root).  Lines:
    RANGE 0xSTART 0xEND <category> <reason...>   assign a reviewed category to a ROM range
    HIDDEN <section-label> <reason...>           accept a hidden-code scan hit
  Every entry needs a reason; an entry that no longer matches anything is an
  error under --check (stale allowlists hide nothing).

Usage:
  scripts/rom-coverage.py               summary table
  scripts/rom-coverage.py --gaps        also list every unaccounted range
  scripts/rom-coverage.py --verbose     also list per-module / per-region detail
  scripts/rom-coverage.py --check       exit 1 on any gap, any USO Text without a
                                        segment, any unreviewed hidden-code hit
  --sourcemap TSV   use an existing sourcemap.tsv instead of regenerating it
Needs a full build (build/tenshoe.map, build/**/*.o).  ~20 s.
"""
import argparse
import json
import os
import re
import struct
import subprocess
import sys
import tempfile
from collections import defaultdict

ROOT = os.path.normpath(os.path.join(os.path.dirname(os.path.abspath(__file__)), ".."))
sys.path.insert(0, os.path.join(ROOT, "port", "tools", "extract"))
import extract as fsx  # noqa: E402  (stdlib-only ROM filesystem walker)

# ---- categories ---------------------------------------------------------------
GAP, CODE_C, ASM_SRC, ASM_UNDEC, ASM_VENDOR, DATA_C, DATA_ASM, ASSET, UCODE, PAD = range(10)
CAT_NAMES = {GAP: "GAP", CODE_C: "code-c", ASM_SRC: "code-asm/src", ASM_UNDEC: "code-asm/undecomp",
             ASM_VENDOR: "code-asm/vendor", DATA_C: "data-c", DATA_ASM: "data-asm", ASSET: "asset",
             UCODE: "ucode", PAD: "padding"}
CAT_BY_NAME = {v: k for k, v in CAT_NAMES.items()}
CODE_CATS = (CODE_C, ASM_SRC, ASM_UNDEC, ASM_VENDOR)

# provenance (from the linker map)
P_NONE, P_BIN, P_CTEXT, P_CDATA, P_STEXT, P_SDATA, P_FILL = range(7)
# filesystem role
R_NONE, R_META, R_TEXT, R_UDATA, R_ASSET, R_DEVTAIL, R_CTEXT_Z = range(7)

UCODE_NAME = re.compile(r"(ucode|_imem$|F3D)")
# ucode that no segment name says: aspMain (PC port plan section 15.2; tenshoe.yaml).
# GLOBAL_ASM'd RSP code inside a CPU Text section: (label, .s file giving its ROM words)
UCODE_SYMBOLS = [
    ("game_libs_rsp_ucode_31DF8", "asm/nonmatchings/game_libs/game_libs/game_libs_rsp_ucode_31DF8.s"),
]
UCODE_EXTRA = [
    # (rom start, size, why)
    (0xDBF730, 0x2E0, "aspMain DMEM data image (bootup.uso Data +0x1D340)"),
]

IPL3 = (0x40, 0x1000)
HEADER = (0x0, 0x40)


def u32(b, o):
    return struct.unpack_from(">I", b, o)[0]


# ---- linker map ---------------------------------------------------------------
OUT_RE = re.compile(r"^(\.\S+)(?:\s+0x([0-9a-f]+)\s+0x([0-9a-f]+)(?:\s+load address 0x([0-9a-f]+))?)?\s*$")
OUT_CONT = re.compile(r"^\s+0x([0-9a-f]+)\s+0x([0-9a-f]+)(?:\s+load address 0x([0-9a-f]+))?\s*$")
IN_RE = re.compile(r"^ (\.\S+|\*fill\*)(?:\s+0x([0-9a-f]+)\s+0x([0-9a-f]+)(?:\s+(\S+))?)?\s*$")
IN_CONT = re.compile(r"^\s+0x([0-9a-f]+)\s+0x([0-9a-f]+)(?:\s+(\S+))?\s*$")


def parse_map(path):
    """Return (outsecs, insecs).  outsecs: name -> (vma, size, lma).
    insecs: list of (rom, size, secname, file, outsec)."""
    lines = open(path).read().split("\n")
    i = next(n for n, l in enumerate(lines) if l.startswith("Linker script and memory map"))
    outsecs, insecs, syms = {}, [], {}
    SYM_RE = re.compile(r"^\s+0x([0-9a-f]+)\s+([A-Za-z_][\w.]*)\s*$")
    cur = None
    while i < len(lines):
        l = lines[i]
        m = OUT_RE.match(l)
        if m and not l.startswith(" "):
            name = m.group(1)
            if m.group(2) is None:
                m2 = OUT_CONT.match(lines[i + 1]) if i + 1 < len(lines) else None
                if not m2:
                    i += 1
                    cur = None
                    continue
                vma, size, lma = m2.group(1), m2.group(2), m2.group(3)
                i += 1
            else:
                vma, size, lma = m.group(2), m.group(3), m.group(4)
            vma, size = int(vma, 16), int(size, 16)
            lma = int(lma, 16) if lma else vma
            outsecs[name] = (vma, size, lma)
            cur = name
            i += 1
            continue
        ms = SYM_RE.match(l)
        if ms and cur:
            ovma, _, olma = outsecs[cur]
            syms.setdefault(ms.group(2), olma + int(ms.group(1), 16) - ovma)
            i += 1
            continue
        m = IN_RE.match(l)
        if m and cur:
            sec = m.group(1)
            if m.group(2) is None:
                m2 = IN_CONT.match(lines[i + 1]) if i + 1 < len(lines) else None
                if m2:
                    vma, size, f = m2.group(1), m2.group(2), m2.group(3)
                    i += 1
                else:
                    i += 1
                    continue
            else:
                vma, size, f = m.group(2), m.group(3), m.group(4)
            vma, size = int(vma, 16), int(size, 16)
            if size:
                ovma, _, olma = outsecs[cur]
                insecs.append((olma + vma - ovma, size, sec, f or "", cur))
        i += 1
    return outsecs, insecs, syms


# ---- tiny ELF32 big-endian reader ---------------------------------------------
def elf_sections(path):
    b = open(path, "rb").read()
    shoff, = struct.unpack_from(">I", b, 0x20)
    shentsize, shnum, shstrndx = struct.unpack_from(">HHH", b, 0x2E)
    secs = []
    for k in range(shnum):
        secs.append(struct.unpack_from(">IIIIIIIIII", b, shoff + k * shentsize))
    stro = secs[shstrndx][4]

    def nm(off):
        return b[stro + off:b.index(b"\0", stro + off)].decode()
    out = {}
    for s in secs:
        out.setdefault(nm(s[0]), {"type": s[1], "offset": s[4], "size": s[5], "align": s[8]})
    return b, out


def elf_text_symbols(path):
    """[(offset, size, name)] of every symbol defined in .text (any binding), sorted."""
    b = open(path, "rb").read()
    shoff, = struct.unpack_from(">I", b, 0x20)
    shentsize, shnum, shstrndx = struct.unpack_from(">HHH", b, 0x2E)
    sh = [struct.unpack_from(">IIIIIIIIII", b, shoff + k * shentsize) for k in range(shnum)]
    stro = sh[shstrndx][4]
    names = [b[stro + s[0]:b.index(b"\0", stro + s[0])].decode() for s in sh]
    if ".text" not in names or ".symtab" not in names:
        return []
    ti = names.index(".text")
    st = sh[names.index(".symtab")]
    stro2 = sh[st[6]][4]
    out = []
    for k in range(st[5] // 16):
        nm, val, size, info, other, shndx = struct.unpack_from(">IIIBBH", b, st[4] + k * 16)
        if shndx != ti or (info & 0xF) == 3:   # skip section symbols
            continue
        name = b[stro2 + nm:b.index(b"\0", stro2 + nm)].decode()
        if name and not name.startswith("$"):
            out.append((val, size, name))
    return sorted(set(out))


def elf_text(path):
    b, secs = elf_sections(path)
    t = secs.get(".text")
    if not t:
        return b"", 1
    return b[t["offset"]:t["offset"] + t["size"]], max(t["align"], 1)


# ---- inputs -------------------------------------------------------------------
def load_sourcemap(path):
    kinds = {}
    for line in open(path):
        if line.startswith("#") or line.startswith("function\t"):
            continue
        p = line.rstrip("\n").split("\t")
        if len(p) >= 4:
            kinds[p[0]] = p[3]
    return kinds


def run_sourcemap(tmpdir):
    out = os.path.join(tmpdir, "sm")
    os.makedirs(out, exist_ok=True)
    # exits non-zero while functions are unresolved (INCLUDE_ASM); that is expected here
    subprocess.run([sys.executable, os.path.join(ROOT, "scripts", "port-sourcemap.py"),
                    "--out-dir", out, "--quiet"], cwd=ROOT, stdout=subprocess.DEVNULL,
                   stderr=subprocess.DEVNULL)
    p = os.path.join(out, "sourcemap.tsv")
    if not os.path.exists(p):
        sys.exit("port-sourcemap.py produced no sourcemap.tsv")
    return p


def load_units():
    """unit name -> {'obj': build path, 'funcs': [(name, addr, size)]}"""
    od = {u["name"]: u for u in json.load(open(os.path.join(ROOT, "objdiff.json")))["units"]}
    rep = json.load(open(os.path.join(ROOT, "report.json")))
    units = {}
    for u in rep["units"]:
        tp = od.get(u["name"], {}).get("target_path", "")
        obj = re.sub(r"^expected/", "build/", tp)
        units[u["name"]] = {
            "obj": obj,
            "funcs": [(f["name"], int(f.get("address", 0) or 0), int(f["size"]))
                      for f in u.get("functions", [])],
        }
    return units


def yay0_recipes():
    """Parse the Makefile's Yay0 code-block rules: block -> (objs, mode, drop offsets, text size)."""
    mk = open(os.path.join(ROOT, "Makefile")).read()
    sizes = {m.group(1): int(m.group(2), 16) for m in
             re.finditer(r"^build/assets/(\w+_yay0)\.bin: YAY0_TEXT_SIZE := 0x([0-9A-Fa-f]+)", mk, re.M)}
    out = {}
    for m in re.finditer(r"^build/assets/(\w+_yay0)\.bin: ([^\n]*)\n((?:\t[^\n]*\n)+)", mk, re.M):
        name, prereq, recipe = m.group(1), m.group(2), m.group(3)
        objs = [t for t in prereq.split() if t.endswith(".o")]
        if not objs or "crunch64" not in recipe:
            continue
        mode = "ldr" if "$(LD) -r" in recipe else "cat"
        drops = [int(x, 16) for x in re.findall(r"drop-text-word\.py \S+ \S+ (0x[0-9A-Fa-f]+)", recipe)]
        out[name] = {"objs": objs, "mode": mode, "drops": drops, "text_size": sizes.get(name)}
    return out


def load_allow(path):
    ranges, hidden = [], []
    if not os.path.exists(path):
        return ranges, hidden
    for n, line in enumerate(open(path), 1):
        line = line.split("#", 1)[0].strip() if line.lstrip().startswith("#") else line.strip()
        if not line:
            continue
        p = line.split(None, 4)
        if p[0] == "RANGE" and len(p) == 5:
            if p[3] not in CAT_BY_NAME:
                sys.exit(f"{path}:{n}: unknown category {p[3]}")
            ranges.append({"lo": int(p[1], 16), "hi": int(p[2], 16), "cat": CAT_BY_NAME[p[3]],
                           "why": p[4], "line": n, "used": 0})
        elif p[0] == "HIDDEN" and len(p) >= 3:
            hidden.append({"label": p[1], "why": line.split(None, 2)[2], "line": n, "used": 0})
        else:
            sys.exit(f"{path}:{n}: malformed allowlist line (needs a reason): {line}")
    return ranges, hidden


# ---- hidden-code scan ---------------------------------------------------------
def code_shapes(buf, base=0):
    """Count MIPS function shapes in buf (word-aligned relative to its start)."""
    hits = []
    n = len(buf) - len(buf) % 4
    words = struct.unpack(">%dI" % (n // 4), buf[:n])
    jr = [i for i, w in enumerate(words) if w == 0x03E00008]
    if not jr:
        return hits
    import bisect
    for i, w in enumerate(words):
        if (w >> 16) != 0x27BD:
            continue
        imm = w & 0xFFFF
        if not (imm & 0x8000):
            continue
        frame = 0x10000 - imm
        if frame % 8 or frame > 0x1000:
            continue
        ok = False
        for j in range(i + 1, min(i + 10, len(words))):
            v = words[j]
            if (v >> 16) == 0xAFBF and (v & 0xFFFF) < frame:
                ok = True
                break
        if not ok:
            continue
        k = bisect.bisect_left(jr, i)
        if k < len(jr) and jr[k] - i < 2000:
            hits.append(base + i * 4)
    return hits


# ---- main audit ---------------------------------------------------------------
class Audit:
    def __init__(self, a):
        self.a = a
        self.rom = open(os.path.join(ROOT, "baserom.z64"), "rb").read()
        N = len(self.rom)
        self.N = N
        self.cat = bytearray(N)          # final category
        self.prov = bytearray(N)
        self.role = bytearray(N)
        self.problems = []               # --check failures
        self.notes = []
        self.sub = defaultdict(lambda: defaultdict(int))  # region label -> cat -> bytes
        self.decomp = []                 # (label, {cat: bytes}) decompressed audits
        self.neighbor_runs = []
        self.symcache = {}
        self.yay0_text = []              # (rom lo, hi, decompressed {cat: bytes}) per Yay0 Text record

    # -- build provenance
    def load_map(self):
        mp = os.path.join(ROOT, "build", "tenshoe.map")
        if not os.path.exists(mp):
            sys.exit("no build/tenshoe.map: run make first")
        self.outsecs, self.insecs, self.map_symbols = parse_map(mp)
        self.obj_text = {}   # obj path -> rom of its .text
        self.text_objs = []  # (lo, hi, obj) of every linked C / asm .text input section
        self.ucode = []
        for rom, size, sec, f, out in self.insecs:
            if rom >= self.N:
                continue
            end = min(rom + size, self.N)
            if sec == "*fill*":
                p = P_FILL
            elif f.endswith(".bin.o"):
                p = P_BIN
            elif f.endswith((".c.o", ".cpp.o")):
                p = P_CTEXT if sec.startswith(".text") else P_CDATA
                if sec.startswith(".text"):
                    self.obj_text.setdefault(f, rom)
            elif f.endswith(".s.o") or f.endswith(".o"):
                p = P_STEXT if sec.startswith(".text") else P_SDATA
            else:
                continue
            self.prov[rom:end] = bytes([p]) * (end - rom)
            if p == P_BIN and UCODE_NAME.search(os.path.basename(f)[:-len(".bin.o")]):
                self.ucode.append((rom, end, os.path.basename(f)[:-len(".bin.o")]))
            self.text_objs.append((rom, end, f)) if p in (P_CTEXT, P_STEXT) else None
        for lo, sz, why in UCODE_EXTRA:
            self.ucode.append((lo, lo + sz, why))
        for sym, sfile in UCODE_SYMBOLS:
            # a local GLOBAL_ASM label (no map symbol): take the ROM range from the .s's
            # `/* ROM VRAM WORD */` columns; leading zero words are alignment padding
            sp = os.path.join(ROOT, sfile)
            words = re.findall(r"/\*\s*([0-9A-F]+)\s+[0-9A-F]+\s+([0-9A-F]{8})\s*\*/",
                               open(sp).read()) if os.path.exists(sp) else []
            if not words:
                self.problems.append(f"ucode {sym}: no ROM words in {sfile}")
                continue
            first = next(int(r, 16) for r, w in words if int(w, 16))
            self.ucode.append((first, int(words[-1][0], 16) + 4, sym))

    # -- filesystem roles
    def load_fs(self):
        rom = self.rom
        devs = fsx.read_devices(rom)
        probs = []
        self.files = []
        self.dev_ranges = []
        for dev in devs:
            files, eof_end = fsx.walk_device(rom, dev, probs)
            self.dev_ranges.append((dev["rom_start"], dev["rom_end"], dev["name"]))
            # everything from the device start to EndOfFile's payload is records
            o = dev["rom_start"]
            self.role[o:eof_end] = bytes([R_META]) * (eof_end - o)
            self.role[eof_end:dev["rom_end"]] = bytes([R_DEVTAIL]) * (dev["rom_end"] - eof_end)
            for f in files:
                kind = fsx.classify(f)
                f["kind"] = kind
                self.files.append(f)
                for r in f["records"]:
                    r["payload"] = r["rom_offset"] + 12
                    if r["type"] in (8, 11):
                        continue
                    lo, hi = r["payload"], r["payload"] + r["size"]
                    t = r["type"]
                    if t == 5:
                        rl = R_TEXT
                    elif t in (6, 7):
                        rl = R_UDATA if kind == "uso-code" else R_ASSET
                    elif t == 12:
                        rl = R_ASSET
                    else:
                        rl = R_META
                    self.role[lo:hi] = bytes([rl]) * (hi - lo)
        self.problems += [f"filesystem: {p}" for p in probs]

    # -- functions
    def place_functions(self, kinds, units):
        """Return list of (rom, size, name, kind) for linked (uncompressed) functions and
        block -> list of (offset, size, name, kind, obj bytes) for Yay0 blocks."""
        self.linked_funcs = []
        recipes = yay0_recipes()
        block_objs = {o for r in recipes.values() for o in r["objs"]}
        obj_units = defaultdict(list)
        for un, u in units.items():
            obj_units[u["obj"]].append(un)
        for un, u in units.items():
            obj = u["obj"]
            if not u["funcs"]:
                continue   # data-only unit (e.g. kernel_026's .data): its bytes come from the map
            if obj in self.obj_text:
                base = self.obj_text[obj]
                for name, addr, size in u["funcs"]:
                    self.linked_funcs.append((base + addr, size, name, kinds.get(name, "?")))
            elif obj not in block_objs:
                self.problems.append(f"unit {un}: object {obj} is neither linked nor in a Yay0 block")
        # Yay0 blocks
        self.block_funcs = {}
        for blk, r in recipes.items():
            off = 0
            funcs = []
            for obj in r["objs"]:
                p = os.path.join(ROOT, obj)
                if not os.path.exists(p):
                    self.problems.append(f"{blk}: missing {obj} (run a full make)")
                    continue
                text, align = elf_text(p)
                if r["mode"] == "ldr":
                    off = (off + align - 1) // align * align
                for un in obj_units.get(obj, []):
                    for name, addr, size in units[un]["funcs"]:
                        funcs.append([off + addr, size, name, kinds.get(name, "?"), text[addr:addr + size]])
                off += len(text)
            for d in sorted(r["drops"], reverse=True):
                for fn in funcs:
                    if fn[0] > d:
                        fn[0] -= 4
            self.block_funcs[blk] = funcs

    def symbol_kind(self, sym, obj):
        base = sym.split(".")[0]
        if base in self.kinds:
            return self.kinds[base]
        src = os.path.join(ROOT, re.sub(r"^build/", "", obj)[:-2])
        if os.path.exists(src):
            txt = open(src, errors="replace").read()
            if re.search(r"^[^#\n]*\b%s\s*\([^;]*\)\s*\{" % re.escape(base), txt, re.M):
                return "c"
        return "?"

    def covering_symbol(self, obj, sec_rom, k, sec_end):
        """Name of the ELF .text symbol of obj covering ROM byte k (unsized symbols run to the next)."""
        if obj not in self.symcache:
            p = os.path.join(ROOT, obj)
            self.symcache[obj] = elf_text_symbols(p) if os.path.exists(p) else []
        syms = self.symcache[obj]
        off = k - sec_rom
        best = None
        for idx, (val, size, name) in enumerate(syms):
            end = val + size if size else (syms[idx + 1][0] if idx + 1 < len(syms) else sec_end - sec_rom)
            if val <= off < end:
                best = name
        return best

    def kind_cat(self, kind, data):
        if kind in ("c", "cpp"):
            return CODE_C
        if kind == "asm":
            return PAD if not any(data) else ASM_SRC
        return ASM_UNDEC

    # -- classify linked (uncompressed) bytes
    def classify(self):
        cat, rom, N = self.cat, self.rom, self.N
        prov, role = self.prov, self.role
        # function bytes first (code-c / asm / undecomp); sorted so overlaps are visible
        fcover = bytearray(N)
        for lo, size, name, kind in sorted(self.linked_funcs):
            hi = lo + size
            if any(fcover[lo:hi]):
                self.problems.append(f"function {name} at 0x{lo:X} overlaps another function")
            fcover[lo:hi] = b"\1" * size
            c = self.kind_cat(kind, rom[lo:hi])
            cat[lo:hi] = bytes([c]) * size
        self.fcover = fcover
        # a USO record header word that a function claims (PREFIX_BYTES: the Text record's
        # flag word compiled as the function's first "instruction") is metadata, not code
        meta_in_fn = 0
        for lo, size, name, kind in self.linked_funcs:
            for k in range(lo, lo + size):
                if role[k] == R_META:
                    cat[k] = DATA_ASM
                    meta_in_fn += 1
            if any(role[lo:lo + size]) and role[lo] == R_META:
                self.notes.append(f"{name}: its first {sum(1 for k in range(lo, lo + size) if role[k] == R_META)}"
                                  f" bytes are the USO Text record header (PREFIX_BYTES), counted as data-asm")
        self.meta_in_fn = meta_in_fn
        # walk every byte range by (prov, role) runs
        i = 0
        while i < N:
            if cat[i]:
                i += 1
                continue
            j = i
            p, r = prov[i], role[i]
            while j < N and not cat[j] and prov[j] == p and role[j] == r:
                j += 1
            self.classify_run(i, j, p, r)
            i = j
        # Unreported code words inside a C / asm object's .text (left GAP by the run walk):
        #   * covered by an ELF symbol the report does not list -> that symbol's body (sourcemap
        #     kind of its base name; else a C definition in the object's source = C; else asm
        #     from asm/nonmatchings = not decompiled);
        #   * no symbol, and the previous word is jr / j (a delay slot past the symbol size) ->
        #     the previous word's category.
        # Anything else stays a GAP for review (e.g. a function with no symbol at all).
        self.neighbor_runs = []
        for lo, hi, obj in self.text_objs:
            for k in range(lo, hi, 4):
                if cat[k] != GAP or role[k] not in (R_TEXT, R_NONE):
                    continue
                w = rom[k:k + 4]
                if w == b"\0\0\0\0":
                    continue
                sym = self.covering_symbol(obj, lo, k, hi)
                if sym is not None:
                    c = self.kind_cat(self.symbol_kind(sym, obj), w)
                elif k >= 4 and cat[k - 4] in CODE_CATS and (
                        u32(rom, k - 4) & 0xFC1FFFFF == 0x00000008 or (u32(rom, k - 4) >> 26) == 2):
                    c, sym = cat[k - 4], "(delay slot)"
                else:
                    continue
                cat[k:k + 4] = bytes([c]) * 4
                self.neighbor_runs.append((k, 4, sym))
        # header / IPL3 are outside every device and are bin segments
        cat[HEADER[0]:HEADER[1]] = bytes([DATA_ASM]) * (HEADER[1] - HEADER[0])
        cat[IPL3[0]:IPL3[1]] = bytes([ASM_VENDOR]) * (IPL3[1] - IPL3[0])
        for lo, hi, why in self.ucode:
            cat[lo:hi] = bytes([UCODE]) * (hi - lo)
        # what is left unattributed and all zero / 0xFF is fill, wherever it sits
        i = 0
        while i < N:
            if cat[i] != GAP:
                i += 1
                continue
            j = i
            while j < N and cat[j] == GAP:
                j += 1
            seg = self.rom[i:j]
            if not seg.strip(b"\0") or not seg.strip(b"\xff"):
                cat[i:j] = bytes([PAD]) * (j - i)
            i = j

    def classify_run(self, i, j, p, r):
        cat, rom = self.cat, self.rom
        seg = rom[i:j]

        def fill_or(c_else):
            # zeros / 0xFF fill -> padding, else c_else
            if not seg.strip(b"\0") or not seg.strip(b"\xff"):
                return PAD
            return c_else
        if r == R_TEXT:
            c = fill_or(GAP)
            # Text bytes that no function covers: zero = alignment, else unattributed code.
            # Compressed Text payloads are handled in audit_yay0 (they overwrite this).
        elif r == R_META:
            c = DATA_ASM
        elif r == R_UDATA:
            c = DATA_C if p == P_CDATA else (ASM_UNDEC if p in (P_CTEXT, P_STEXT) else DATA_ASM)
            if p in (P_CTEXT, P_STEXT):
                c = fill_or(GAP)   # a C .text object linked over a Data record: wrong split
        elif r == R_ASSET:
            c = ASSET
        elif r == R_DEVTAIL:
            c = fill_or(GAP)
        else:  # outside the filesystem: kernel, header, IPL3, ROM tail
            if p == P_CTEXT or p == P_STEXT:
                c = fill_or(GAP)   # .text bytes no function covers
            elif p == P_CDATA:
                c = DATA_C
            elif p == P_SDATA:
                c = DATA_ASM
            elif p == P_BIN:
                c = fill_or(DATA_ASM)
            else:
                c = fill_or(GAP)
        cat[i:j] = bytes([c]) * (j - i)

    # -- Yay0 code blocks: decompressed audit + compressed-byte apportioning
    def audit_text_records(self):
        for f in self.files:
            for r in f["records"]:
                if r["type"] != 5:
                    continue
                lo, hi = r["payload"], r["payload"] + r["size"]
                label = f"{f['name']} Text"
                if not r["compressed"]:
                    # must be produced by a code segment (C .o text), not a bin blob
                    p = self.prov
                    nbin = sum(1 for k in range(lo, hi, 4) if p[k] == P_BIN)
                    nf = sum(1 for k in range(lo, hi, 4) if self.fcover[k])
                    if nf == 0:
                        self.problems.append(f"{label} (ROM 0x{lo:X}, 0x{hi - lo:X} bytes): no code segment")
                    elif nbin:
                        self.notes.append(f"{label}: {nbin * 4:#x} bytes linked from bin blobs")
                    continue
                # compressed: payload rom = Yay0 stream of a block linked as a bin.  Pick the
                # recipe whose function layout matches the decompressed bytes best.
                data = r["data"]
                blk, best = None, 0.5
                for b, funcs in self.block_funcs.items():
                    tot = same = 0
                    for off, size, name, kind, ob in funcs:
                        for k in range(0, size - 3, 4):
                            tot += 1
                            same += data[off + k:off + k + 4] == ob[k:k + 4]
                    if tot and same / tot > best and sum(f[1] for f in funcs) <= len(data):
                        blk, best = b, same / tot
                if blk is None:
                    self.problems.append(f"{label} (Yay0, ROM 0x{lo:X}): no Yay0 code-block recipe")
                    counts = {ASM_UNDEC: len(data)}
                else:
                    counts = self.audit_block(label, blk, data)
                    self.check_block_provenance(label, blk, lo, hi)
                self.apportion(lo, hi, counts)
                self.yay0_text.append((lo, hi, counts))

    def check_block_provenance(self, label, blk, lo, hi):
        """The compressed Text must be linked from the block the C build compressed
        (build/assets/<blk>.bin, wrapped by objcopy -I binary, whose symbols name the
        source file), not from splat's baserom extract assets/<blk>.bin.  Then the
        byte-identical ROM check proves the C of the whole block."""
        want = f"build/assets/{blk}.bin.o"
        cover = [(rom_, rom_ + size) for rom_, size, sec, f, out in self.insecs
                 if f == want and rom_ <= lo < rom_ + size]
        if not cover:
            self.problems.append(f"{label} (Yay0, ROM 0x{lo:X}): not linked from {want}")
            return
        # (the walker's payload range can run a few bytes past the Yay0 stream into the
        # next record; those bytes belong to that record's bin)
        strtab = open(os.path.join(ROOT, want), "rb").read()
        if f"_binary_build_assets_{blk}_bin_start".encode() + b"\0" not in strtab:
            self.problems.append(f"{label} (Yay0, ROM 0x{lo:X}): {want} was not wrapped from the "
                                 f"C-built build/assets/{blk}.bin (baserom extract linked?)")

    def audit_block(self, label, blk, data):
        dcat = bytearray(len(data))
        mism = []
        for off, size, name, kind, obytes in self.block_funcs.get(blk, []):
            if off + size > len(data):
                self.problems.append(f"{blk}: {name} placed past the block end")
                continue
            if any(dcat[off:off + size]):
                self.problems.append(f"{blk}: {name} at +0x{off:X} overlaps another function")
            want = data[off:off + size]
            nw = size // 4
            same = sum(1 for k in range(0, size - 3, 4) if want[k:k + 4] == obytes[k:k + 4])
            if nw and same < nw * 0.75:
                mism.append(f"{name}@+0x{off:X} {same}/{nw}")
            dcat[off:off + size] = bytes([self.kind_cat(kind, want)]) * size
        if mism:
            self.problems.append(f"{blk}: layout check failed for {len(mism)} functions: {mism[:4]}")
        i = 0
        while i < len(dcat):
            if dcat[i]:
                i += 1
                continue
            j = i
            while j < len(dcat) and not dcat[j]:
                j += 1
            c = PAD if not data[i:j].strip(b"\0") else GAP
            dcat[i:j] = bytes([c]) * (j - i)
            if c == GAP:
                self.problems.append(f"{label} (decompressed {blk}) +0x{i:X}..+0x{j:X}: no function")
            i = j
        counts = defaultdict(int)
        for b in dcat:
            counts[b] += 1
        self.decomp.append((f"{label} [{blk}, Yay0 0x{len(data):X} decompressed]", dict(counts)))
        return counts

    def apportion(self, lo, hi, counts):
        """Split the compressed payload bytes in proportion to the decompressed content."""
        total = sum(counts.values()) or 1
        n = hi - lo
        pos = lo
        items = sorted(counts.items())
        for k, (c, v) in enumerate(items):
            take = n - (pos - lo) if k == len(items) - 1 else round(n * v / total)
            self.cat[pos:pos + take] = bytes([c]) * take
            pos += take

    # -- allowlist
    def apply_allow(self, ranges):
        for e in ranges:
            seg = self.cat[e["lo"]:e["hi"]]
            e["used"] = seg.count(GAP)
            if e["used"]:
                # only GAP bytes are reassigned: the allowlist never overrides a real attribution
                for k in range(e["lo"], e["hi"]):
                    if self.cat[k] == GAP:
                        self.cat[k] = e["cat"]

    # -- hidden code
    def scan_hidden(self, hidden_allow):
        hits = []
        for f in self.files:
            for r in f["records"]:
                if r["type"] not in (6, 7, 12):
                    continue
                h = code_shapes(r["data"])
                if h:
                    label = f"{f['name']}:{r['name']}"
                    hits.append((label, len(h), r["payload"], r["compressed"], h[:3]))
        # linked bin blobs outside the filesystem (kernel data etc.) and inside Text
        for rom_, size, sec, fobj, out in self.insecs:
            if not fobj.endswith(".bin.o") or rom_ >= self.N:
                continue
            name = out.lstrip(".")
            if UCODE_NAME.search(name) or name in ("ipl3", "header"):
                continue
            # only blobs not covered by the filesystem walk (those are scanned per record)
            if any(self.role[rom_:rom_ + size:4]):
                continue
            h = code_shapes(self.rom[rom_:rom_ + size])
            if h:
                hits.append((f"bin:{name}", len(h), rom_, False, h[:3]))
        self.hidden = []
        for label, n, lo, comp, first in hits:
            al = [e for e in hidden_allow if e["label"] == label]
            for e in al:
                e["used"] += 1
            self.hidden.append((label, n, lo, comp, first, al[0]["why"] if al else None))
            if not al:
                self.problems.append(f"hidden code? {label} (ROM 0x{lo:X}{', Yay0' if comp else ''}): "
                                     f"{n} function shapes, first at +{', +'.join(hex(x) for x in first)}")

    # -- report
    def gaps(self):
        out = []
        i, N, cat = 0, self.N, self.cat
        while i < N:
            if cat[i] != GAP:
                i += 1
                continue
            j = i
            while j < N and cat[j] == GAP:
                j += 1
            out.append((i, j))
            i = j
        return out

    def where(self, a):
        for f in self.files:
            if f["rom_offset"] <= a < f.get("rom_end", f["rom_offset"]):
                for r in f["records"]:
                    if r["rom_offset"] <= a < r["payload"] + (0 if r["type"] == 8 else r["size"]):
                        return f"{f['name']} {r['name']}+0x{a - r['payload']:X}" if a >= r["payload"] \
                            else f"{f['name']} {r['name']} header"
                return f"{f['name']}"
        best = None
        for rom_, size, sec, fobj, out in self.insecs:
            if rom_ <= a < rom_ + size:
                best = f"{out} {sec} {os.path.basename(fobj)}+0x{a - rom_:X}"
        return best or "outside every device and linked section"


def fmt_table(counts, total, title):
    L = [title, f"  {'category':20s} {'bytes':>12s} {'hex':>10s} {'%':>8s}"]
    for c in range(10):
        v = counts.get(c, 0)
        if v or c == GAP:
            L.append(f"  {CAT_NAMES[c]:20s} {v:12,d} {v:#10x} {100.0 * v / total:7.3f}%")
    L.append(f"  {'total':20s} {total:12,d} {total:#10x}")
    return "\n".join(L)


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--gaps", action="store_true", help="list every unaccounted range")
    ap.add_argument("--check", action="store_true", help="exit 1 on any gap / problem")
    ap.add_argument("--verbose", "-v", action="store_true")
    ap.add_argument("--sourcemap", help="existing sourcemap.tsv (default: regenerate)")
    ap.add_argument("--allow", default=os.path.join(ROOT, "rom-coverage.allow"))
    a = ap.parse_args()

    A = Audit(a)
    with tempfile.TemporaryDirectory() as td:
        sm = a.sourcemap or run_sourcemap(td)
        kinds = load_sourcemap(sm)
    units = load_units()
    allow_ranges, allow_hidden = load_allow(a.allow)
    A.load_map()
    A.load_fs()
    A.kinds = kinds
    A.place_functions(kinds, units)
    A.classify()
    A.audit_text_records()
    A.apply_allow(allow_ranges)
    A.scan_hidden(allow_hidden)
    for e in allow_ranges + allow_hidden:
        if not e["used"]:
            A.problems.append(f"{a.allow}:{e['line']}: stale allowlist entry (matches nothing)")

    counts = defaultdict(int)
    for c in range(10):
        counts[c] = A.cat.count(c)
    N = A.N
    print(fmt_table(counts, N, f"ROM coverage: baserom.z64, {N:#x} bytes"))
    code = sum(counts[c] for c in CODE_CATS)
    game = code - counts[ASM_VENDOR]
    asm = counts[ASM_SRC] + counts[ASM_UNDEC] + counts[ASM_VENDOR]
    print(f"  (code-asm total {asm:,d} B = src + undecomp + vendor)")
    print(f"\n  code total {code:,d} B (game code excl. IPL3 {game:,d} B; ucode {counts[UCODE]:,d} B kept apart)")
    print(f"  decompiled C:        {counts[CODE_C]:10,d} B = {100.0 * counts[CODE_C] / game:6.2f}% of game code")
    print(f"  hand-written asm:    {counts[ASM_SRC]:10,d} B = {100.0 * counts[ASM_SRC] / game:6.2f}%  (libultra / handwritten .s)")
    print(f"  NOT decompiled:      {counts[ASM_UNDEC]:10,d} B = {100.0 * counts[ASM_UNDEC] / game:6.2f}%  (INCLUDE_ASM)")
    print(f"  honest decomp %:     {100.0 * counts[CODE_C] / game:6.2f}% (C / game code);"
          f" {100.0 * (counts[CODE_C] + counts[ASM_SRC]) / game:6.2f}% incl. hand-written asm;"
          f" {100.0 * counts[CODE_C] / code:6.2f}% of all code incl. IPL3")
    # the same with Yay0 Text counted at its decompressed (instruction) size
    un = dict(counts)
    for lo, hi, dc in A.yay0_text:
        seg = A.cat[lo:hi]
        for c in range(10):
            un[c] = un.get(c, 0) - seg.count(c) + dc.get(c, 0)
    ugame = sum(un[c] for c in (CODE_C, ASM_SRC, ASM_UNDEC))
    print(f"  by instruction bytes (Yay0 Text decompressed): C {un[CODE_C]:,d} / {ugame:,d} B ="
          f" {100.0 * un[CODE_C] / ugame:6.2f}%; not decompiled {un[ASM_UNDEC]:,d} B")
    nfun = len(A.linked_funcs) + sum(len(v) for v in A.block_funcs.values())
    nund = sum(1 for f in A.linked_funcs if f[3] not in ("c", "cpp", "asm")) + \
        sum(1 for v in A.block_funcs.values() for f in v if f[3] not in ("c", "cpp", "asm"))
    print(f"  functions: {nfun} placed, {nfun - nund} with a C / asm source body, {nund} INCLUDE_ASM")

    print("\nDecompressed Yay0 Text blocks (compressed ROM bytes apportioned by this content;"
          " linked from the C build's own Yay0 output, so proven by the byte-identical ROM):")
    for label, c in A.decomp:
        tot = sum(c.values())
        print(f"  {label}: " + ", ".join(f"{CAT_NAMES[k]} {v:#x}" for k, v in sorted(c.items())))

    # per-module code view
    if a.verbose:
        print("\nPer USO module with Text (ROM bytes):")
        for f in A.files:
            if f["kind"] != "uso-code":
                continue
            lo, hi = f["rom_offset"], f["rom_end"]
            seg = A.cat[lo:hi]
            parts = ", ".join(f"{CAT_NAMES[c]} {seg.count(c):#x}" for c in range(10) if seg.count(c))
            print(f"  {f['name']:18s} 0x{lo:07X}..0x{hi:07X}: {parts}")
        for n_ in A.notes:
            print("  note: " + n_)

    if A.hidden:
        print("\nHidden-code scan (data / asset payloads with MIPS function shapes):")
        for label, n, lo, comp, first, why in A.hidden:
            print(f"  {label:32s} ROM 0x{lo:07X}{' Yay0' if comp else '     '} {n:4d} shapes  "
                  + (f"allowlisted: {why}" if why else "UNREVIEWED"))

    gl = A.gaps()
    gb = sum(j - i for i, j in gl)
    print(f"\nGaps: {len(gl)} ranges, {gb:,d} bytes" + ("" if a.gaps or not gl else "  (--gaps lists them)"))
    if a.gaps:
        for i, j in gl:
            print(f"  0x{i:07X}..0x{j:07X} {j - i:#8x}  {A.where(i)}")
    used = [e for e in allow_ranges if e["used"]]
    if used:
        print(f"Allowlisted ranges ({a.allow}): {len(used)}, "
              f"{sum(e['used'] for e in used):,d} bytes")
    if gl:
        A.problems.insert(0, f"{len(gl)} unaccounted ranges ({gb:,d} bytes)")
    print(f"\nproblems: {len(A.problems)}")
    for p in A.problems[:60]:
        print("  " + p)
    if len(A.problems) > 60:
        print(f"  ... {len(A.problems) - 60} more")
    if a.check:
        print("rom-coverage check: " + ("FAIL" if A.problems else "PASS"))
        return 1 if A.problems else 0
    return 0


if __name__ == "__main__":
    sys.exit(main())
