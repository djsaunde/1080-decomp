#!/usr/bin/env python3
"""PC port: bind the placeholder calls that have no CALL name, per function, from the ROM
(docs/1080_PC_PORT_PLAN.md section 14).

    scripts/port-gen-binds.py [--undefined port/undefined.txt]    # after a full matching make

A call spelled with a placeholder token (`timproc_uso_b5_alias(0x4F0)`, `func_00000000(self)`,
an address token `game_uso_func_055750(8)` ...) instead of CALL(f) names no callee in the source.
The callee is in the ROM: the module's TextReloc R_MIPS_26 entry at that jal.  For each kept
function (port/sourcemap.tsv) whose shipped object is the ROM code (name-placeholder-calls.py's
byte guard), every jal reloc against a token the port library leaves undefined is resolved; when
ALL of one function's jals through one token reach the same callee, the pair is bound:

    /* port-bind: FUNC TOKEN TARGET */      (in the unit's port/gen/tu header)

and port/tools/cc-select.py renames TOKEN to TARGET inside FUNC's assembly (and its gcc clones
FUNC.part.N / .constprop.N).  The call is then a plain call of the real function.  A token with
several callees in one function needs a CALL name in the source (listed as `ambiguous`).
Targets that are not a plain symbol are listed, not bound:
  mid-entry   `sym+0xK` (a jal into the middle of a build symbol: libultra entry points);
  sentinel    a module's Text+0 function (`*_00000000`);
  unresolved  no ROM reloc at the site.
Data references (HI16 / LO16 / R_MIPS_32 relocs against a token the port leaves undefined) are
resolved the same way, to the target module's blob: `port_<mod>_data+0xK` / `_rodata` / `_bss`
(port/uso, plan section 16.5); K = the Sym value + the token's matching-link value (the port
asm keeps the source's own addend).  A jal into a libultra copy the port does not compile
(`gl_osSetIntMask+0x8`) becomes the host entry point `gl_osSetIntMask_plus8` (port/ultra).
A jal through a Text-base Sym is resolved from its instruction field.

A token whose every ROM site (in every proven function, calls and data) is one target gets a
global alias instead (port/gen/aliases.ld, `tok = target;`, an implicit linker script); the
per-function binds are only for tokens with several targets (gl_func_00000000, D_00000000 ...).
The CALL-named subtraction runs on C and C++ units (the scanner is lexical).
Writes port/gen/binds.tsv (unit, function, token, target, sites), port/gen/binds-left.tsv
and port/gen/aliases.ld.
"""
import argparse
import collections
import importlib.util
import os
import re
import struct
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
os.chdir(ROOT)
spec = importlib.util.spec_from_file_location("npc", str(ROOT / "scripts/name-placeholder-calls.py"))
npc = importlib.util.module_from_spec(spec)
sys.argv, _argv = [sys.argv[0]], sys.argv
spec.loader.exec_module(npc)
sys.argv = _argv


def read_tsv(path):
    return [l.rstrip("\n").split("\t") for l in open(path)
            if l.strip() and not l.startswith(("#", "function\t"))]


SEC_SYM = {"Data": "data", "RoData": "rodata", "Bss": "bss"}
R_MIPS = {2: "word", 4: "jal", 5: "hi", 6: "lo"}


def blob_sym(short, sec):
    """the port/uso blob holding a module's section (port/tools/gen-uso-blobs.py)"""
    return "port_%s_%s" % (short, SEC_SYM[sec])


def obj_relocs(path):
    """[(offset, type, symbol name)] of an object's .text relocations against named symbols"""
    with open(path, "rb") as f:
        e = npc.ELFFile(f)
        st = e.get_section_by_name(".symtab")
        out = []
        for sec in e.iter_sections():
            if sec.name in (".rel.text", ".rela.text"):
                for r in sec.iter_relocations():
                    s = st.get_symbol(r["r_info_sym"])
                    if s.name and not s.name.startswith(".") and r["r_info_type"] in R_MIPS:
                        out.append((r["r_offset"], r["r_info_type"], s.name))
        return out


class RomTargets:
    """ROM side of a reloc site: the module's TextReloc entry at a Text offset, resolved to the
    exporting module's section and value (USO_FORMAT sections 6 and 7)"""

    def __init__(self, rom):
        self.rom, self.byid, self._rel = rom, {m.id: m for m in rom.mods}, {}

    def site(self, md, off):
        if md.short not in self._rel:
            self._rel[md.short] = {o: (f1 & 7, f1 >> 4) for f1, o, _ in md.relocs("Text")}
        return self._rel[md.short].get(off)

    def raw(self, md, idx):
        """-> (module, section, value) of Sym idx (imports followed to the exporter), or None"""
        if idx >= len(md.syms):
            return None
        info, v, w3 = md.syms[idx]
        info >>= 16
        if info & 7 == 1:
            ex = self.byid.get(info >> 4)
            return self.raw(ex, w3 >> 16) if ex is not None and ex is not md else None
        sec = npc.UI.SEC_OF_TAG.get((info >> 4) & 0xF)
        if info & 7 not in (2, 4) or sec is None:
            return None
        return md, sec, (v if info & 7 == 2 else 0)

    # N64 boot globals at fixed kernel low-memory addresses (set by IPL3 / osInitialize): a USO
    # imports them from bootup.uso as ABSOLUTE Syms (kind 3, value = the KSEG0 address; e.g.
    # mgr_openSession's `== 0x17D7` read is bootup Sym 1820 = 0x80000310), which raw() does not
    # resolve.  The port defines them under their libultra names (port/ultra/misc.c).
    LOWMEM = {0x80000300: "osTvType", 0x80000304: "osRomType", 0x80000308: "osRomBase",
              0x8000030C: "osResetType", 0x80000310: "osCicId", 0x80000318: "osMemSize",
              0x8000031C: "osAppNMIBuffer"}

    def absolute(self, md, idx):
        """-> the libultra name of Sym idx when it is (an import of) an absolute low-memory
        boot global, else None"""
        if idx >= len(md.syms):
            return None
        info, v, w3 = md.syms[idx]
        info >>= 16
        if info & 7 == 1:
            ex = self.byid.get(info >> 4)
            return self.absolute(ex, w3 >> 16) if ex is not None and ex is not md else None
        return self.LOWMEM.get(v) if info & 7 == 3 else None

    def target(self, md, off, pin=0):
        """the port spelling of what the ROM reloc at Text offset off points at, or None:
        a function name (`f`, `f+0xK` mid-entry, `Text+0xK` unnamed) or `port_<mod>_<sec>+0xK`.
        pin is the matching link's value of the token: the instruction field holds pin + the
        source's addend, which the port asm keeps, so the token stands for base + value + pin."""
        r = self.site(md, off)
        if r is None:
            return None
        typ, idx = r
        t = self.raw(md, idx)
        if t is None:
            a = self.absolute(md, idx) if typ != 1 else None
            return a if a is None or not pin else "%s+0x%X" % (a, pin)
        ex, sec, v = t
        if sec == "Text":
            if typ == 1:     # R_MIPS_26: the jal field holds the offset (sym value + field << 2)
                ins = struct.unpack_from(">I", md.secs["Text"], off)[0]
                return ex.name_text(v + ((ins & 0x3FFFFFF) << 2))
            return text_data(ex, v + pin)
        return "%s+0x%X" % (blob_sym(ex.short, sec), v + pin)


# Modules whose Text the port has as an address space (port_<mod>_text, port/uso/uso.c): bootup
# embeds the RSP microcode (F3DEX family IMEM, aspMain) in Text outside every function of its
# table; the game only hands those addresses to the RSP (OSTask_t.ucode), plan 18.9.
TEXT_IMAGES = {"bootup"}


def text_data(ex, off):
    """the port spelling of a data address in ex's Text: the function (`f`, `f+0xK`), or
    port_<mod>_text+0xK outside every function of a TEXT_IMAGES module (`Text+0xK` otherwise)"""
    f = ex.name_text(off)
    if f is not None and f.startswith("Text+") and ex.short in TEXT_IMAGES:
        return "port_%s_text+0x%X" % (ex.short, off)
    return f


def load_pins():
    """token -> its matching-link value (undefined_syms_auto.txt; absolute symbols of the link)"""
    pins = {}
    for line in open("undefined_syms_auto.txt", errors="replace"):
        m = re.match(r"\s*(\w+)\s*=\s*(0x[0-9A-Fa-f]+|\d+)\s*;", line)
        if m:
            pins[m.group(1)] = int(m.group(2), 0)
    return pins


def donor_pins(src, pins):
    """pins for one unit.  A match_imports donor is linked alone first, each token at its value in
    scripts/match-origins-<f>.ld (--emit-relocs keeps the relocs, the field now holds that value
    + the source's addend); scripts/bind-native-relocations.py renames each reloc's symbol
    (scripts/match-bindings-<f>.json `renames`), and the ROM link adds the renamed symbol's
    value (undefined_syms_auto.txt, = the json's bindings).  So a token's pin is the sum of the
    two, not undefined_syms_auto's value of the token (`bootup_27E8_ref_7334` links at 0 and its
    carrier `sStr_rom_game_uso_7334` at 0x7334; plan 18.11)."""
    p = Path("scripts/match-bindings-%s.json" % Path(src).stem)
    o = Path("scripts/match-origins-%s.ld" % Path(src).stem)
    if not src.startswith("src/match_imports/") or not p.exists() or not o.exists():
        return pins
    import json
    d = json.load(open(p))
    b = d.get("original_ROM_minus_curated_addend_bindings", {})
    first = {}
    for line in open(o):
        m = re.match(r"\s*(\w+)\s*=\s*(0x[0-9A-Fa-f]+|\d+)\s*;", line)
        if m:
            first[m.group(1)] = int(m.group(2), 0)
    out = dict(pins)
    for tok, ren in d.get("renames", {}).items():
        v = (first.get(tok, 0) + pins.get(ren, b.get(ren, 0))) & 0xFFFFFFFF   # 32-bit fields
        out[tok] = v - (1 << 32) if v & 0x80000000 else v
    return out


def port_defined(lib):
    """(global symbols libgame.a defines, {data object it defines: its member object}).  The
    data objects (B / D: the objects the C++ donors define for their static initialisers,
    gGfxLib_412E8_a ...) are tokens too: the ROM has them in a module's Data / Bss, which the
    game code reaches through the blob, so the port definition is dropped (port-dropdata) and
    the name aliased to the blob, and the initialiser and the readers share memory."""
    out = subprocess.run(["nm", "-A", "--defined-only", lib], capture_output=True, text=True).stdout
    defs, data = set(), {}
    for l in out.splitlines():
        f = l.split()
        if len(f) == 3 and f[1].isupper():
            sym = f[2]
            defs.add(sym)
            if f[1] in "BD" and not sym.startswith("_Z"):
                data[sym] = f[0].split(":")[1]
    return defs, data


def asm_pad(sourcemap):
    """excluded asm function -> the bytes of alignment padding its build symbol starts with
    (splat starts a libultra .s copy at the zero words that pad the preceding object, so the
    real entry point is `sym+pad`; the shim defines the function itself)"""
    pad = {}
    for r in sourcemap:
        if r[2].endswith(".s") and os.path.exists(r[2]):
            n, inside = 0, False
            for line in open(r[2], errors="replace"):
                code = line.split("#")[0].strip()
                if code.startswith("glabel ") and code.split()[1] == r[0]:
                    inside = True
                elif inside and code:
                    if code != "nop":
                        break
                    n += 1
            pad[r[0]] = 4 * n
    return pad


def shim_defined():
    """the placeholder names port/ultra and the host layer define themselves (PORT_ALIAS):
    the shim's definition wins, aliases.ld must not name them again"""
    names = set()
    for f in list(Path("port/ultra").glob("*.c")) + list(Path("port").glob("*.c")):
        names |= set(re.findall(r"PORT_ALIAS\(\s*\w+\s*,\s*(\w+)\s*\)", f.read_text(errors="replace")))
    return names


def shim_entry(target, excluded, pad=None):
    """`gl_osSetIntMask+0x8` (a jal into a libultra copy the port does not compile) -> the shim's
    `gl_osSetIntMask` when the offset only skips the copy's leading alignment padding, else the
    host entry point `gl_osSetIntMask_plus8` (not provided: a real mid-function entry), else None"""
    m = re.fullmatch(r"(\w+)\+0x([0-9A-F]+)", target or "")
    if m and m.group(1) in excluded:
        if pad and pad.get(m.group(1)) == int(m.group(2), 16):
            return m.group(1)
        return "%s_plus%s" % (m.group(1), m.group(2).lower())
    return None


XDATA_RE = re.compile(r'\bXDATA\s*\(\s*(\w+)\s*,\s*(\w+)\s*,\s*(-?\s*(?:0x[0-9A-Fa-f]+|\d+))\s*\)')
ALIAS_DEF_RE = re.compile(r'^[ \t]*#[ \t]*define[ \t]+(\w+)[ \t]+(\w+)[ \t]*(?:/\*.*?\*/[ \t]*|//.*)?$', re.M)
_INCLUDE_ALIASES = None


def xdata_named(text):
    """{(function, token): {ROM target spelling}} of the XDATA(tok, sym, off) sites
    (scripts/name-data-sites.py): those sites reach their own object in the port, so their
    targets leave the function's multiset.  tok may be an alias macro of the token."""
    global _INCLUDE_ALIASES
    if _INCLUDE_ALIASES is None:
        import glob
        _INCLUDE_ALIASES = {}
        for p in glob.glob("include/**/*.h", recursive=True):
            _INCLUDE_ALIASES.update(dict(ALIAS_DEF_RE.findall(open(p, errors="replace").read())))
    out = collections.defaultdict(set)
    if "XDATA" not in text:
        return out
    al = dict(_INCLUDE_ALIASES)
    al.update(dict(ALIAS_DEF_RE.findall(text)))
    m = npc.mask(text)
    blocks = [b for b in npc.top_blocks(m) if npc.func_header_name(m, b[0], b[1])]
    for x in XDATA_RE.finditer(m):
        blk = next((b for b in blocks if b[1] < x.start() < b[2]), None)
        if blk is None:
            continue
        tok, seen = x.group(1), set()
        while tok in al and tok not in seen:
            seen.add(tok)
            tok = al[tok]
        sym, off = x.group(2), int(x.group(3).replace(" ", ""), 0)
        t = "%s+0x%X" % (sym, off) if sym.startswith("port_") else ("%s%+d" % (sym, off) if off else sym)
        out[(npc.func_header_name(m, blk[0], blk[1]), tok)].add(t)
    return out


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--lib", default="/tmp/claude-1000/port-build/libgame.a",
                    help="the port library (its undefined symbols are the tokens to bind)")
    a = ap.parse_args()
    excluded = {r[0] for r in read_tsv("port/gen/excluded.tsv")}
    defined, datadefs = port_defined(a.lib)
    if os.path.exists("port/gen/dropdata.tsv"):     # dropped by an earlier run: still tokens
        datadefs.update({r[0]: r[1] for r in read_tsv("port/gen/dropdata.tsv")})
    defined -= set(datadefs)
    by_src = collections.defaultdict(list)
    smap = read_tsv("port/sourcemap.tsv")
    pad = asm_pad(smap)
    shimdef = shim_defined()
    for r in smap:
        if r[0] not in excluded:
            by_src[r[2]].append(r[0])
    shipped = {f for fs in by_src.values() for f in fs}
    pins = load_pins()
    rom, mv = npc.Rom(), npc.MakeVars()
    rt = RomTargets(rom)
    calls = []          # (src, fn, tok, Counter(target)) after the CALL-named subtraction
    data = []           # (src, fn, tok, Counter(target))
    gcall = collections.defaultdict(collections.Counter)   # every ROM jal site, per token
    gdata = collections.defaultdict(collections.Counter)
    gtext = set()       # data tokens with a site into Text: bound per addend, never aliased
    stats = collections.Counter()
    for src in sorted(by_src):
        obj = npc.obj_of(src)
        if not os.path.exists(obj):
            stats["no object"] += 1
            continue
        md = rom.module_for(src) or npc.donor_module(src, rom, mv)
        if md is None or not md.funcs:
            stats["no module"] += 1
            continue
        rfuncs, _ = npc.elf_info(obj)
        rall = npc.reloc_offsets(obj)
        upins = donor_pins(src, pins)
        orel = [r for r in obj_relocs(obj) if r[2] not in defined]
        # sites the source already names (CALL macros) call the real name in the port object:
        # take their callees out of the ROM multiset first (C and C++: the scanner is lexical)
        named = collections.defaultdict(collections.Counter)
        text = open(src, errors="replace").read()
        for st in npc.scan(text)[0]:
            if st.name is not None:
                named[(st.func, st.tok)][npc.current_name(st.name) or st.name] += 1
        for (fnm, tok), c in npc.macro_named(text).items():
            named[(fnm, tok)] += c
        xnamed = xdata_named(text)
        pool = collections.defaultdict(collections.Counter)
        for (_f, tok), c in named.items():
            pool[tok] += c
        for fn in by_src[src]:
            fo = fn if fn in rfuncs else ("gl_" + fn) if ("gl_" + fn) in rfuncs else None
            if fo is None:
                continue
            base = rom.func_off(md, fo)
            rv, rsz, rb = rfuncs[fo]
            if base is None or not npc.rom_bytes_match(md, base, rb, rv, rall):
                stats["function not proven"] += 1
                continue
            per = collections.defaultdict(collections.Counter)
            perd = collections.defaultdict(collections.Counter)
            # per data token: full addend (the source's `tok+N`: HI16 field << 16 + the paired
            # LO16's signed field; the .o holds it in the instruction, REL) -> ROM targets
            pera = collections.defaultdict(lambda: collections.defaultdict(collections.Counter))
            hi = {}
            textdata = set()
            for off, typ, sym in orel:
                if not rv <= off < rv + rsz:
                    continue
                t = rt.target(md, base + off - rv, upins.get(sym, 0))
                if typ == 4:
                    per[sym][t] += 1
                    gcall[sym][t] += 1
                    continue
                field = struct.unpack_from(">I", rb, off - rv)[0]
                if typ != 6:        # count a HI16 / LO16 pair once
                    perd[sym][t] += 1
                    gdata[sym][t] += 1
                if typ == 5:
                    hi[sym] = (field & 0xFFFF) << 16
                elif typ == 6:
                    lo = field & 0xFFFF
                    lo -= 0x10000 if lo & 0x8000 else 0
                    full = (hi.get(sym, 0) + lo + 0x80000000) % 0x100000000 - 0x80000000
                    raw = rt.site(md, base + off - rv)
                    raw = raw and rt.raw(md, raw[1])
                    if raw and raw[1] == "Text":
                        # an address in Text (a callback): the addend picks the function, and the
                        # port spelling drops it (`f-N` + the source's `+N`)
                        f = text_data(raw[0], raw[2] + upins.get(sym, 0) + full)
                        if f is not None and f.startswith("port_"):
                            t = text_data(raw[0], raw[2] + upins.get(sym, 0))   # the ucode: the source keeps the addend
                        else:
                            t = None if f is None or "+" in f else "%s%+d" % (f, -full) if full else f
                            textdata.add(sym)
                    pera[sym][full][t] += 1
                elif typ == 2:
                    pera[sym][field - (0x100000000 if field & 0x80000000 else 0)][t] += 1
            for tok in list(per):
                if src.endswith(".cpp"):
                    # C++: a site's function is a source block (inline member, `operator new`,
                    # class name), not the object function: the file's named sites are one pool,
                    # each name used once by the function whose jal it matches (run 618)
                    take = per[tok] & pool[tok]
                    pool[tok] -= take
                    per[tok] -= take
                else:
                    per[tok] -= named.get((fn, tok), collections.Counter())
                if per[tok]:
                    calls.append((src, fn, tok, per[tok]))
            for tok in list(perd):
                gone = xnamed.get((fn, tok)) or xnamed.get((fo, tok))
                if gone:
                    for t in gone:
                        perd[tok].pop(t, None)
                        for c in pera.get(tok, {}).values():
                            c.pop(t, None)
                    if tok in pera:
                        for k in [k for k, c in pera[tok].items() if not c]:
                            del pera[tok][k]
                    if not perd[tok]:
                        del perd[tok]
            for tok, tg in perd.items():
                data.append((src, fn, tok, tg, pera.get(tok, {}), tok in textdata))
                if tok in textdata:
                    gtext.add(tok)

    def usable(t):
        # a `*_00000000` name is a placeholder token, unless it is the shipped Text+0 function of
        # a module (timproc_uso_b5_func_00000000: GenCourse_ctor's callback)
        return t is not None and not t.startswith("Text+") and \
            (not t.endswith("_00000000") or t in shipped) and ("+" not in t or t.startswith("port_"))

    # global aliases: every ROM site of the token, in every proven function, is one target
    aliases = {}
    for tok, tg in list(gcall.items()) + list(gdata.items()):
        if tok in aliases or tok in shimdef or tok in gtext:
            continue
        both = gcall.get(tok, collections.Counter()) + gdata.get(tok, collections.Counter())
        if len(both) == 1:
            t = next(iter(both))
            host = shim_entry(t, excluded, pad)
            if host:
                aliases[tok] = host
            elif usable(t) and (t.startswith("port_") or t in shipped or t in excluded or t in defined):
                aliases[tok] = t
    binds, left = [], []
    nbyaddend = 0
    for kind, rows in (("call", calls), ("data", data)):
        for row in rows:
            src, fn, tok, tg = row[:4]
            n = sum(tg.values())
            if tok in aliases or tok in shimdef:
                continue
            if kind == "data" and (len(tg) > 1 or row[5]) and row[4]:
                # several blob objects, told apart by the addend: bind each `tok+N` whose ROM
                # sites all reach one target (cc-select); list the addends that do not
                def ok(c):
                    t = next(iter(c))
                    return len(c) == 1 and t is not None and (
                        usable(t) and (t.startswith("port_") or t in shipped) or
                        re.fullmatch(r"\w+[+-]\d+", t) and t.split("+")[0].split("-")[0] in shipped)
                good = {a: c for a, c in row[4].items() if ok(c)}
                for addend, c in sorted(good.items()):
                    binds.append((src, fn, "%s%+d" % (tok, addend), next(iter(c)), sum(c.values())))
                bad = {a: c for a, c in row[4].items() if a not in good}
                if bad:
                    left.append((src, fn, tok, "unnamed" if all(set(c) == {None} for c in bad.values())
                                 else "ambiguous", " ".join(
                        "%+d:%s" % (a, "|".join("%s*%d" % kv for kv in c.items() if kv[0]))
                        for a, c in sorted(bad.items())), sum(sum(c.values()) for c in bad.values())))
                nbyaddend += 1
                continue
            if len(tg) > 1:
                left.append((src, fn, tok, "ambiguous", " ".join("%s*%d" % kv for kv in tg.items() if kv[0]), n))
                continue
            target = next(iter(tg))
            host = shim_entry(target, excluded, pad)
            if host:
                binds.append((src, fn, tok, host, n))
            elif target is None:
                left.append((src, fn, tok, "unresolved", "", n))
            elif target.startswith("Text+"):
                left.append((src, fn, tok, "unnamed", target, n))
            elif "+" in target and not target.startswith("port_"):
                left.append((src, fn, tok, "mid-entry", target, n))
            elif target.endswith("_00000000") and target not in shipped:
                left.append((src, fn, tok, "sentinel", target, n))
            else:
                binds.append((src, fn, tok, target, n))
    with open("port/gen/binds.tsv", "w") as fh:
        fh.write("# generated by scripts/port-gen-binds.py: unit\tfunction\ttoken\ttarget\tsites\n")
        for r in binds:
            fh.write("\t".join(map(str, r)) + "\n")
    with open("port/gen/binds-left.tsv", "w") as fh:
        fh.write("# generated by scripts/port-gen-binds.py: unit\tfunction\ttoken\tkind\ttarget(s)\tsites\n")
        for r in left:
            fh.write("\t".join(map(str, r)) + "\n")
    # gcc's names of the C++ runtime entry points the NCC donors reach under their cfront names:
    # the cfront name's ROM target is theirs too (global operator new = mem_new)
    for gnu, cfront in (("_Znwj", "__nw__FUi"),):
        if cfront in aliases and gnu not in defined:
            aliases[gnu] = aliases[cfront]
    dropdata = sorted((t, datadefs[t]) for t in datadefs
                      if t in aliases or any(r[2] == t for r in binds))
    with open("port/gen/dropdata.tsv", "w") as fh:
        fh.write("# generated by scripts/port-gen-binds.py: data objects the port units define that the\n"
                 "# ROM keeps in a module's blob (port-dropdata; aliased / bound to the blob)\n"
                 "# symbol\tobject\n")
        for r in dropdata:
            fh.write("%s\t%s\n" % r)
    with open("port/gen/aliases.ld", "w") as fh:
        fh.write("/* generated by scripts/port-gen-binds.py: placeholder / alias tokens whose every ROM\n"
                 "   site is one target (an implicit linker script of the port link) */\n")
        for tok in sorted(aliases):
            fh.write("%s = %s;\n" % (tok, aliases[tok]))
    kinds = collections.Counter(r[3] for r in left)
    print("data tokens bound per addend: %d (function, token) pairs" % nbyaddend)
    print("dropped data definitions %d: %s" % (len(dropdata), " ".join(t for t, _ in dropdata)))
    print("aliases %d (call %d, data %d); bound %d (function, token) pairs, %d sites; left %d: %s; %s" % (
        len(aliases), sum(1 for t in aliases if t in gcall), sum(1 for t in aliases if t not in gcall),
        len(binds), sum(r[4] for r in binds), len(left), dict(kinds), dict(stats)))
    return 0


if __name__ == "__main__":
    sys.exit(main())
