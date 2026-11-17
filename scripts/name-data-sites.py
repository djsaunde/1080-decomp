#!/usr/bin/env python3
"""Name the ambiguous data placeholder sites for the PC port: `&D_00000000` ->
`&XDATA(D_00000000, port_bootup_bss, 0x58)` (docs/1080_PC_PORT_PLAN.md 16.7, agent-a run 625).

A data reference through a zero placeholder (D_00000000, or an object-like alias macro of it
such as `#define MNode_vtbl D_00000000`) names no object in the source: the object is the
target of the module's TextReloc entry at that HI16 / LO16.  scripts/port-gen-binds.py binds
the token per function (or per function and addend) when every site of it reaches one target;
port/gen/binds-left.tsv lists the functions where several objects share the token and the
addend (`ambiguous`).  Those sites need the target at the site:

    XDATA(tok, sym, off)       include/uso_call.h: `tok` in the matching build (the bytes cannot
                               change); under PORT the object at sym + off with tok's type

sym is the port/uso blob of the target module's section (`port_<mod>_data|rodata|bss`), a
function (an address in Text: a callback) or `port_<mod>_text` (an address in Text outside every
function: the embedded RSP microcode); off is the Sym value + the token's pinned value
(for a Text target, minus the site's full addend, which the source keeps).

Which source site is which reloc: the same proof as name-placeholder-calls.py `plan`.  A verify
copy of the file spells site #N `npd_N` (declared like the token's nearest preceding
declaration), compiled with the object's own Makefile recipe; a function is used only when its
verify bytes are IDENTICAL to the real object's function and the real function is the ROM Text
(rom_bytes_match).  Every reloc of npd_N must resolve to one target.

    scripts/name-data-sites.py plan FILE.. [--funcs port/gen/binds-left.tsv] [--tok D_00000000]
                                           [--write] [-v]
    scripts/name-data-sites.py check [FILE..]     # every XDATA names a ROM target of its
                                                  # function (fast, no compile); exit 1 on a
                                                  # mismatch.  Default: every file using XDATA.

Run from the project root after a full matching make.
"""
import argparse
import collections
import glob
import importlib.util
import os
import re
import struct
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
os.chdir(ROOT)
_spec = importlib.util.spec_from_file_location("pgb", str(ROOT / "scripts/port-gen-binds.py"))
pgb = importlib.util.module_from_spec(_spec)
sys.argv, _argv = [sys.argv[0]], sys.argv
_spec.loader.exec_module(pgb)
sys.argv = _argv
npc = pgb.npc
ELFFile = npc.ELFFile

XDATA_RE = re.compile(r'\bXDATA\s*\(\s*(\w+)\s*,\s*(\w+)\s*,\s*(-?\s*(?:0x[0-9A-Fa-f]+|\d+))\s*\)')
DATA_RELOCS = (2, 5, 6)       # R_MIPS_32, HI16, LO16


def alias_macros():
    """object-like macros whose whole body is one identifier (an alias of a placeholder):
    {macro: identifier}, from include/ (the file's own are added per file)"""
    out = {}
    for p in glob.glob("include/**/*.h", recursive=True):
        out.update(file_aliases(open(p, errors="replace").read()))
    return out


def file_aliases(text):
    return {m.group(1): m.group(2) for m in re.finditer(
        r'^[ \t]*#[ \t]*define[ \t]+(\w+)[ \t]+(\w+)[ \t]*(?:/\*.*?\*/[ \t]*|//.*)?$', text, re.M)}


def makefile_renames():
    """{source path: {token: name}}: the `objcopy --redefine-sym tok=name` renames of the
    recipes whose rule lists that source (an NCC donor's D_00000000_j -> gGfxLib_48AEC_src):
    the object's relocations carry the new name"""
    out = collections.defaultdict(dict)
    srcs = []
    for line in open("Makefile", errors="replace"):
        if not line.startswith("\t") and ":" in line and not line.lstrip().startswith("#"):
            srcs = re.findall(r'\bsrc/\S+\.(?:c|cpp)\b', line)
        elif line.startswith("\t") and "--redefine-sym" in line:
            for a, b in re.findall(r'--redefine-sym\s+(\w+)=(\w+)', line):
                for sp in srcs:
                    out[sp][a] = b
    return out


def resolve_tok(tok, aliases):
    seen = set()
    while tok in aliases and tok not in seen:
        seen.add(tok)
        tok = aliases[tok]
    return tok


def data_relocs(path):
    """{offset: (type, symbol)} of the object's .text data relocations"""
    out = {}
    with open(path, "rb") as f:
        e = ELFFile(f)
        st = e.get_section_by_name(".symtab")
        for sec in e.iter_sections():
            if sec.name in (".rel.text", ".rela.text"):
                for r in sec.iter_relocations():
                    if r["r_info_type"] in DATA_RELOCS:
                        s = st.get_symbol(r["r_info_sym"])
                        out[r["r_offset"]] = (r["r_info_type"], s.name or "sect:%d" % s["st_shndx"])
    return out


def site_targets(md, rt, base, rb, rv, rsz, rel, want, pins, sym_of=None):
    """{key: set of (sym, off)} for the data relocs inside the function [rv, rv + rsz) whose
    symbol satisfies want(sym) (key = that symbol).  sym_of(off) gives the real object's token
    at off (for the pin; default: the reloc's own symbol)."""
    out = collections.defaultdict(set)
    hi = {}
    for off in sorted(o for o in rel if rv <= o < rv + rsz):
        typ, sym = rel[off]
        if not want(sym):
            continue
        tok = sym_of(off) if sym_of else sym
        pin = pins.get(tok, 0)
        r = rt.site(md, base + off - rv)
        raw = r and rt.raw(md, r[1])
        if not raw:
            ab = r and rt.absolute(md, r[1])     # a low-memory boot global (osCicId ...)
            out[sym].add((ab, pin) if ab else None)
            continue
        ex, sec, v = raw
        field = struct.unpack_from(">I", rb, off - rv)[0]
        if typ == 5:
            hi[sym] = (field & 0xFFFF) << 16
        if sec != "Text":
            out[sym].add((pgb.blob_sym(ex.short, sec), v + pin))
            continue
        if typ == 5:
            continue        # its LO16 names the function (the pair's full addend picks it)
        if typ == 6:
            lo = field & 0xFFFF
            lo -= 0x10000 if lo & 0x8000 else 0
            full = (hi.get(sym, 0) + lo + 0x80000000) % 0x100000000 - 0x80000000
        else:
            full = field - (0x100000000 if field & 0x80000000 else 0)
        f = ex.name_text(v + pin + full)
        if f is not None and f.startswith("Text+"):
            # an address in Text outside every function of the module's table: the embedded
            # RSP microcode (bootup Text +0x28F10 .. F3DEX family IMEM, +0x46470 aspMain, plan
            # 18.9); the port spells it in the module's Text image, port_<mod>_text (port/uso/uso.c)
            out[sym].add(("port_%s_text" % ex.short, v + pin))
            continue
        out[sym].add(None if f is None or "+" in f else (f, -full))
    return out


def decl_for(m, tok, pos, name, cpp):
    """`extern <type> name[];` from tok's nearest preceding single-declarator declaration"""
    best = None
    for d in re.finditer(r'\bextern\s+(?:"C"\s+)?((?:[A-Za-z_]\w*[ \t\*]+)+?)\**\s*' + re.escape(tok) +
                         r'\s*(\[[^\]]*\])?\s*;', m):
        if d.start() >= pos:
            break
        star = re.search(r'(\**)\s*' + re.escape(tok), m[d.start():d.end()]).group(1)
        best = '%s %s%s%s' % (' '.join(d.group(1).split()), star, name, d.group(2) or '')
    if best is None:
        return None
    return ('extern "C" { extern %s; } ' if cpp else 'extern %s; ') % best


def scan_sites(src, m, funcs, toks):
    """[(pos, end, text, tok, func)] raw uses of toks (or their alias macros) inside the
    function blocks of funcs; XDATA arguments are skipped"""
    blocks = [b for b in npc.top_blocks(m) if npc.func_header_name(m, b[0], b[1]) in funcs]
    spans = [(x.start(), x.end()) for x in XDATA_RE.finditer(m)]
    out = []
    if not toks:
        return out
    tre = re.compile(r'\b(' + '|'.join(sorted(map(re.escape, toks))) + r')\b')
    for mt in tre.finditer(m):
        p = mt.start()
        if any(a <= p < b for a, b in spans) or npc.is_decl(m, p):
            continue
        blk = next((b for b in blocks if b[1] < p < b[2]), None)
        if blk is None:
            continue
        out.append((p, mt.end(), mt.group(1), npc.func_header_name(m, blk[0], blk[1])))
    return out


def fmt_off(off):
    return "-0x%X" % -off if off < 0 else "0x%X" % off


def plan_file(path, funcs, base_toks, rom, mv, rt, pins, aliases, verbose):
    res = {"named": [], "left": [], "error": None}
    src = open(path, errors="replace").read()
    m = npc.mask(src)
    al = dict(aliases)
    al.update(file_aliases(src))
    toks = {a for a in al if resolve_tok(a, al) in base_toks} | set(base_toks)
    sites = scan_sites(src, m, funcs, toks)
    if not sites:
        res["error"] = "no sites"
        return res
    cpp = path.endswith(".cpp")
    md = rom.module_for(path) or npc.donor_module(path, rom, mv)
    obj = npc.obj_of(path)
    if md is None or not os.path.exists(obj):
        res["error"] = "no module / object"
        return res
    edits = []
    for k, (p, e, tok, fn) in enumerate(sites):
        real = resolve_tok(tok, al)
        d = decl_for(m, real, p, "npd_%d" % k, cpp)
        if d is None:
            d = ('extern "C" { extern char npd_%d; } ' if cpp else 'extern char npd_%d; ') % k
        edits.append((p, e, "npd_%d" % k))
        # declare at file scope just before the enclosing function
        blk = next(b for b in npc.top_blocks(m) if b[1] < p < b[2])
        edits.append((blk[0], blk[0], "\n" + d + "\n"))
    npc.strip_static_funcs(m, edits)
    vt = src
    for p, e, t in sorted(edits, key=lambda x: (x[0], x[1]), reverse=True):
        vt = vt[:p] + t + vt[e:]
    try:
        vfuncs, _ = npc.compile_verify(path, vt, mv, tag="xd")
    except RuntimeError as ex:
        res["error"] = str(ex)
        return res
    vobj = os.path.join(npc.SCRATCH, path) + (".xd.v.o")
    vrel = data_relocs(vobj)
    rfuncs, _ = npc.elf_info(obj)
    rrel = data_relocs(obj)
    rall = npc.reloc_offsets(obj)
    names = {}
    for fn in sorted({s[3] for s in sites}):
        fo = fn if fn in rfuncs else ("gl_" + fn) if ("gl_" + fn) in rfuncs else None
        if fo is None or fo not in vfuncs:
            res["left"].append((fn, "not in the object"))
            continue
        rv, rsz, rb = rfuncs[fo]
        vv, vsz, vb = vfuncs[fo]
        base = rom.func_off(md, fo)
        if vb != rb:
            res["left"].append((fn, "verify bytes differ (donor splice / -O0 recipe)"))
            continue
        if base is None or not npc.rom_bytes_match(md, base, rb, rv, rall):
            res["left"].append((fn, "not the ROM Text"))
            continue
        # the verify object's npd relocs sit at the same offsets as the real token's
        shifted = {o - vv + rv: t for o, t in vrel.items() if vv <= o < vv + vsz}
        tg = site_targets(md, rt, base, rb, rv, rsz, shifted, lambda s: s.startswith("npd_"), pins,
                          sym_of=lambda o: rrel.get(o, (0, ""))[1])
        for sym, ts in tg.items():
            k = int(sym[4:])
            if len(ts) == 1 and None not in ts:
                names[k] = next(iter(ts))
            else:
                res["left"].append((fn, "site %d (%s): targets %s" % (k, sites[k][2], sorted(map(str, ts)))))
    out = src
    for k in sorted(names, key=lambda k: sites[k][0], reverse=True):
        p, e, tok, fn = sites[k]
        sym, off = names[k]
        out = out[:p] + "XDATA(%s, %s, %s)" % (tok, sym, fmt_off(off)) + out[e:]
        res["named"].append((fn, tok, sym, off))
    for k, s in enumerate(sites):
        if k not in names and not any(l[0] == s[3] for l in res["left"]):
            res["left"].append((s[3], "site %d (%s): no reloc in the build" % (k, s[2])))
    res["text"] = npc.add_include(path, out) if names else None
    return res


def cmd_plan(a):
    rom, mv = npc.Rom(), npc.MakeVars()
    rt = pgb.RomTargets(rom)
    pins = pgb.load_pins()
    aliases = alias_macros()
    want = collections.defaultdict(set)
    for r in pgb.read_tsv(a.funcs):
        if r[3] == "ambiguous" and r[2] in a.tok:
            want[r[0]].add(r[1])
    files = a.files or sorted(want)
    tot = collections.Counter()
    for path in files:
        if path not in want:
            continue
        res = plan_file(path, want[path], set(a.tok), rom, mv, rt, pins, aliases, a.v)
        tot["named"] += len(res["named"])
        tot["left"] += len(res["left"])
        print("%s: %d named, %d left%s" % (path, len(res["named"]), len(res["left"]),
                                         " -- " + res["error"] if res["error"] else ""))
        if a.v:
            for fn, tok, sym, off in res["named"]:
                print("   %s: %s -> %s%+#x" % (fn, tok, sym, off))
        for fn, why in res["left"]:
            print("   left %s: %s" % (fn, why))
        if a.write and res.get("text"):
            open(path, "w").write(res["text"])
    print("total: %(named)d sites named, %(left)d left" % tot)
    return 0


def cpp_alias(path, fn, rfuncs):
    """a C++ donor's member function (`Mgr::run`: the header name is `run`) under its shipped
    name: the port's `port-alias: NEW _ZN3Mgr3runEv` line (port/gen/tu) whose mangled target
    holds fn, when exactly one does and NEW is in the object"""
    if not fn or not path.endswith(".cpp"):
        return None
    hdr = ROOT / "port/gen/tu" / (path.replace("/", "__") + ".h")
    if not hdr.exists():
        return None
    hits = {new for new, tgt in re.findall(r"port-alias:\s*(\w+)\s+(\w+)", hdr.read_text())
            if tgt.startswith("_Z") and ("%d%s" % (len(fn), fn)) in tgt and new in rfuncs}
    return hits.pop() if len(hits) == 1 else None


def cmd_check(a):
    rom, mv = npc.Rom(), npc.MakeVars()
    rt = pgb.RomTargets(rom)
    pins = pgb.load_pins()
    aliases = alias_macros()
    renames = makefile_renames()
    files = a.files or sorted(p for p in glob.glob("src/**/*.c", recursive=True) +
                              glob.glob("src/**/*.cpp", recursive=True)
                              if "XDATA" in open(p, errors="replace").read())
    bad = n = 0
    for path in files:
        src = open(path, errors="replace").read()
        m = npc.mask(src)
        al = dict(aliases)
        al.update(file_aliases(src))
        blocks = [b for b in npc.top_blocks(m) if npc.func_header_name(m, b[0], b[1])]
        uses = []
        for x in XDATA_RE.finditer(m):
            blk = next((b for b in blocks if b[1] < x.start() < b[2]), None)
            fn = blk and npc.func_header_name(m, blk[0], blk[1])
            uses.append((fn, x.group(1), x.group(2), int(x.group(3).replace(" ", ""), 0)))
        if not uses:
            continue
        md = rom.module_for(path) or npc.donor_module(path, rom, mv)
        obj = npc.obj_of(path)
        rfuncs, _ = npc.elf_info(obj)
        rrel = data_relocs(obj)
        cache = {}
        for fn, tok, sym, off in uses:
            n += 1
            real = resolve_tok(tok, al)
            real = renames.get(path, {}).get(real, real)
            fo = fn and (fn if fn in rfuncs else ("gl_" + fn) if ("gl_" + fn) in rfuncs else
                         cpp_alias(path, fn, rfuncs))
            if fo is None:
                print("%s: XDATA(%s, %s, %s) outside a built function (%s)" % (path, tok, sym, fmt_off(off), fn))
                bad += 1
                continue
            if fo not in cache:
                rv, rsz, rb = rfuncs[fo]
                base = rom.func_off(md, fo)
                cache[fo] = {} if base is None else site_targets(
                    md, rt, base, rb, rv, rsz, rrel, lambda s: True, pins)
            sym_now = npc.current_name(sym) or sym
            if (sym, off) not in cache[fo].get(real, ()):
                print("%s: %s: XDATA(%s, %s, %s) is not a ROM target of %s there (%s)%s" % (
                    path, fn, tok, sym, fmt_off(off), real,
                    ", ".join(sorted("%s%+#x" % t for t in cache[fo].get(real, ()) if t)) or "none",
                    "" if sym_now == sym else " [renamed: %s]" % sym_now))
                bad += 1
    print("XDATA check: %d sites, %d mismatches" % (n, bad))
    return 1 if bad else 0


def main():
    ap = argparse.ArgumentParser()
    sub = ap.add_subparsers(dest="cmd", required=True)
    p = sub.add_parser("plan")
    p.add_argument("files", nargs="*")
    p.add_argument("--funcs", default="port/gen/binds-left.tsv")
    p.add_argument("--tok", action="append", default=None)
    p.add_argument("--write", action="store_true")
    p.add_argument("-v", action="store_true")
    c = sub.add_parser("check")
    c.add_argument("files", nargs="*")
    a = ap.parse_args()
    if a.cmd == "plan":
        a.tok = a.tok or ["D_00000000"]
        return cmd_plan(a)
    return cmd_check(a)


if __name__ == "__main__":
    sys.exit(main())
