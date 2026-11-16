#!/usr/bin/env python3
"""PC port: the per-module type map of the USO data blobs (docs/1080_PC_PORT_PLAN.md, "M1/M2:
byte order").

    scripts/port-gen-blob-types.py [--report]

The USO Data / RoData sections are big-endian on the cartridge.  port/tools/gen-uso-blobs.py
byte-swaps them at blob-build time from the map this script writes, port/gen/blob_types.tsv
(module, section, offset, size, type, source; offsets / sizes / types only, no ROM bytes).

Every typing source is the ROM itself (the module records parsed by scripts/uso-imports.py),
so the map covers modules with no src/ yet:

  access   every HI16/LO16 TextReloc pair of every module's Text that targets a Data / RoData
           Sym (imports followed to the exporter): the LO16 instruction's opcode is the access
           width (lb/lbu/sb 1, lh/lhu/sh 2, lw/sw 4, lwc1/swc1 f32, ldc1/sdc1/ld/sd 8).  An addiu
           (address taken) is followed forward through the function: loads / stores off that
           register (or a copy, or base + constant) type base + imm ("field"); off base + index
           ("index") they type one element and the run is extended (below).
  reloc    DataReloc / RoDataReloc R_MIPS_32 words: port/uso writes them in host order, so they
           are never swapped here (type ptr, kept as on the cartridge).
  gfx      display lists: a reloc'd word at +4 of a GBI (F3DEX, GBI1) command whose opcode
           takes an address, walked back and forward over GBI1 opcodes to G_ENDDL: u32 pairs.
           G_VTX targets: Vtx (s16 x 6 + u8 x 4).  G_MTX targets: Mtx (u32 x 16).
           G_SETTIMG targets and G_MOVEMEM lights stay bytes (libultraship's Fast3D reads
           texels, TLUTs and lights byte by byte, big-endian).
  str      NUL-terminated printable runs (>= 4 chars), at an aligned or referenced start.
  zero     all-zero bytes outside every typed run (any swap is a no-op).
  audio    bootup Data ranges port/audio swaps at run time (plan 15.6): excluded here, a second
           swap would undo them.
  index    an indexed-access element type extended forward to the next symbol / reloc target /
           typed byte (lower confidence; the source column says so).

Conflicts (one byte typed with two widths) are written as type `conflict` and not swapped;
--report lists them.  Untyped non-zero bytes are left as on the cartridge (`unknown`).
"""
import argparse
import collections
import importlib.util
import os
import re
import struct
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
os.chdir(ROOT)
spec = importlib.util.spec_from_file_location("npc", str(ROOT / "scripts/name-placeholder-calls.py"))
npc = importlib.util.module_from_spec(spec)
spec.loader.exec_module(npc)
UI = npc.UI

# opcode -> (width, type) of a load / store
MEMOP = {0x20: (1, "u8"), 0x24: (1, "u8"), 0x28: (1, "u8"),
         0x21: (2, "u16"), 0x25: (2, "u16"), 0x29: (2, "u16"),
         0x23: (4, "u32"), 0x2B: (4, "u32"),
         0x31: (4, "f32"), 0x39: (4, "f32"),
         0x35: (8, "f64"), 0x3D: (8, "f64"), 0x37: (8, "u64"), 0x3F: (8, "u64")}
WIDTH = {"u8": 1, "str": 1, "bytes": 1, "u16": 2, "s16": 2, "u32": 4, "f32": 4, "ptr": 4,
         "u64": 8, "f64": 8}
SECS = ("Data", "RoData")

# plan 15.6: bootup Data swapped by port_audio_swap_static at run time
AUDIO_RANGES = [(0x17D50, 0x1A2E0), (0x1CB00, 0x1CDE0), (0x1D620, 0x1E0D0)]

# GBI1 (F3DEX) opcodes; address-taking ones: G_MTX 01, G_MOVEMEM 03, G_VTX 04, G_DL 06,
# G_SETTIMG FD, G_SETZIMG FE, G_SETCIMG FF
GBI1_OPS = set(range(0x00, 0x08)) | set(range(0xAF, 0xC0)) | set(range(0xE4, 0x100))
ADDR_OPS = {0x01, 0x03, 0x04, 0x06, 0xFD, 0xFE, 0xFF}
G_ENDDL = 0xB8


def plausible(d, o, typ):
    """a float element worth extending an indexed run over: 0 or a normal exponent near 1, and
    not text (bootup RoData 0x2588 "NaN\0Inf\0" ends the powers-of-ten table)"""
    el = d[o:o + (4 if typ == "f32" else 8)]
    if re.search(rb"[\x20-\x7e]{3}\x00", bytes(el)):
        return False
    if typ == "f32":
        w = struct.unpack_from(">I", d, o)[0]
        e = (w >> 23) & 0xFF
        return w & 0x7FFFFFFF == 0 or 127 - 40 <= e <= 127 + 40
    w = struct.unpack_from(">Q", d, o)[0]
    e = (w >> 52) & 0x7FF
    return w & 0x7FFFFFFFFFFFFFFF == 0 or 1023 - 900 <= e <= 1023 + 900


def sext(v):
    return v - 0x10000 if v & 0x8000 else v


class Map:
    """per (module, section): byte -> (type, source)"""

    def __init__(self, data):
        self.data = data
        self.n = len(data)
        self.t = [None] * self.n
        self.src = [None] * self.n
        self.st = [None] * self.n
        self.conf = []

    def put(self, off, typ, src, count=1, force=False):
        """type count elements of typ at off.  Relocated words and audio ranges are never
        retyped; an element overlapping one of another width or start is a conflict."""
        w = WIDTH.get(typ, 1)
        for e in range(count):
            o = off + e * w
            if o < 0 or o + w > self.n:
                return
            cur = self.t[o:o + w]
            if not force and any(c in ("ptr", "audio") for c in cur):
                continue
            if force or all(c is None for c in cur):
                for b in range(w):
                    self.t[o + b], self.src[o + b], self.st[o + b] = typ, src, o
                continue
            if all(c is not None and c != "conflict" and WIDTH.get(c, 1) == w and self.st[o + b] == o
                   for b, c in enumerate(cur)):
                continue          # same width and start (u32 / f32 swap alike)
            if w == 1 and all(WIDTH.get(c, 1) == 1 for c in cur if c is not None):
                continue
            first = next(b for b, c in enumerate(cur) if c is not None)
            self.conf.append((o, cur[first], typ, src, self.src[o + first]))
            for b in range(w):
                if self.t[o + b] is not None:
                    self.t[o + b] = "conflict"


def analyse_text(md, rt, maps, stats):
    """HI16/LO16 pairs in md's Text that land in a Data / RoData section; follow addiu bases"""
    text = md.secs.get("Text")
    if not text:
        return
    rel = sorted(((o, f1 & 7, f1 >> 4) for f1, o, _ in md.relocs("Text")))
    hi = {}
    for off, typ, idx in rel:
        if typ == 3:
            hi[idx] = struct.unpack_from(">I", text, off)[0] & 0xFFFF
            continue
        if typ != 2 or idx not in hi:
            continue
        t = rt.raw(md, idx)
        if t is None:
            continue
        ex, sec, v = t
        if sec not in SECS or (ex.short, sec) not in maps:
            continue
        m = maps[(ex.short, sec)]
        ins = struct.unpack_from(">I", text, off)[0]
        op, rs, rtg, imm = ins >> 26, (ins >> 21) & 31, (ins >> 16) & 31, sext(ins & 0xFFFF)
        addr = (v + (hi[idx] << 16) + imm) & 0xFFFFFFFF
        if addr >= m.n:
            stats["access outside its section"] += 1
            continue
        if op in MEMOP:
            m.put(addr, MEMOP[op][1], "access")
            stats["direct access"] += 1
        elif op == 0x09:      # addiu: address taken
            stats["address taken"] += 1
            follow(md, text, off + 4, rtg, addr, m, stats)


def func_end(md, off):
    for v, sz, nm in md.funcs:
        if v <= off < v + max(sz, 4):
            return v + sz
    return off + 256


def follow(md, text, pc, reg, base, m, stats):
    """linear forward walk: regs holding base + k (exact) or base + index (idx)"""
    regs = {reg: (0, False)}
    end = min(func_end(md, pc - 4), pc + 4 * 96, len(text))
    while pc < end and regs:
        ins = struct.unpack_from(">I", text, pc)[0]
        op, rs, rt_, rd, imm = ins >> 26, (ins >> 21) & 31, (ins >> 16) & 31, (ins >> 11) & 31, sext(ins & 0xFFFF)
        fn = ins & 0x3F
        if op in MEMOP and rs in regs:
            k, idx = regs[rs]
            a = base + k + imm
            if 0 <= a < m.n:
                m.put(a, MEMOP[op][1], "index" if idx else "field")
                if idx:
                    m.idx_hints.append((a, MEMOP[op][1]))
                stats["field" if not idx else "index"] += 1
        # register definitions
        dst = None
        if op == 0x09:                                  # addiu
            dst = rt_
            if rs in regs:
                k, idx = regs[rs]
                regs[rt_] = (k + imm, idx)
                pc += 4
                continue
        elif op == 0 and fn in (0x21, 0x25, 0x2D):      # addu / or / daddu
            dst = rd
            a, b = rs in regs, rt_ in regs
            if (a and rt_ == 0) or (b and rs == 0):
                regs[rd] = regs[rs if a else rt_]
                pc += 4
                continue
            if a != b:
                k, _ = regs[rs if a else rt_]
                regs[rd] = (k, True)
                pc += 4
                continue
        elif op == 0:
            if fn == 0x08 or fn == 0x09:                # jr / jalr: one delay slot more
                end = min(end, pc + 8)
            dst = rd
        elif op in (0x02, 0x03):                        # j / jal
            if op == 0x02:
                end = min(end, pc + 8)
            # calls clobber the caller-saved registers after the delay slot
            for r in list(regs):
                if not (16 <= r <= 23 or r in (29, 30)):
                    regs.pop(r)
            pc += 4
            continue
        elif op in (0x0F, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E) or op in MEMOP and op in (0x20, 0x21, 0x23, 0x24, 0x25, 0x37):
            dst = rt_
        elif op == 0x0F:
            dst = rt_
        if dst is not None and dst in regs:
            regs.pop(dst)
        pc += 4


def gfx_walk(md, sec, m, relocs, stats):
    """display lists around reloc'd GBI address words; Vtx / Mtx targets"""
    d = m.data
    vtx, mtx = [], []
    seen = set()
    for off in relocs:
        if off % 8 != 4 or off < 4 or d[off - 4] not in ADDR_OPS:
            continue
        start = off - 4
        if start in seen:
            continue
        # back to the DL start
        s = start
        while s >= 8 and d[s - 8] in GBI1_OPS and d[s - 8] != G_ENDDL and (s - 8) not in seen:
            s -= 8
        e = start
        ok = True
        while e + 8 <= m.n and d[e] != G_ENDDL:
            if d[e] not in GBI1_OPS:
                ok = False
                break
            e += 8
        if not ok or e + 8 > m.n:
            continue
        e += 8
        for c in range(s, e, 8):
            seen.add(c)
            m.put(c, "u32", "gfx", 2)
            op, w0 = d[c], struct.unpack_from(">I", d, c)[0]
            k = struct.unpack_from(">I", d, c + 4)[0]
            if (c + 4) not in relocs:
                continue
            if op == 0x04:
                lo = w0 & 0xFFFF
                n = lo >> 10 if (lo & 0x3FF) == ((lo >> 10) * 16 - 1) & 0x3FF else lo // 16
                vtx.append((c + 4, n))
            elif op == 0x01:
                mtx.append(c + 4)
        stats["gfx: display-list commands"] += (e - s) // 8
    return vtx, mtx


# shape command stream (GRAPHICS 7.1, BasePolyShape_parse): word = argc << 16 | op, args, op 2 ends.
# per op: (argc, [(arg index of the pointer, element type, count source)]); count source:
# "n" = arg 0, "verts" / "tris" = this stream's op 27 / 28 counts, int = fixed
SHAPE_OPS = {0: 0, 1: 0, 4: 1, 6: 1, 7: 1, 8: 1, 9: 1, 10: 1, 11: 1, 12: 1, 18: 1, 19: 1, 20: 1,
             21: 1, 25: 1, 26: 2, 27: 2, 28: 2, 29: 2, 30: 2, 31: 0, 32: 2, 34: 2, 35: 1, 36: 3,
             3: 1, 13: 1, 14: 1, 15: 1, 17: 1, 22: 1, 23: 1, 24: 1}
# GDL_MC material commands (GRAPHICS 6.2) and their argument words
MAT_ARGS = {200: 1, 201: 4, 202: 5, 203: 7, 204: 1, 205: 1, 206: 1, 207: 1, 208: 1, 209: 1, 210: 1, 211: 1}


def parse_shape(m, o):
    """-> list of (op, arg offset) up to op 2, or None if o is not a shape stream"""
    d, cmds = m.data, []
    while o + 4 <= m.n and len(cmds) < 4096:
        w = struct.unpack_from(">I", d, o)[0]
        op, argc = w & 0xFFFF, w >> 16
        if op == 2 and argc == 0:
            cmds.append((2, o))
            return cmds
        if op not in SHAPE_OPS or argc != SHAPE_OPS[op]:
            return None
        cmds.append((op, o))
        o += 4 + 4 * argc
    return None


def walk_assets(maps, ptrs, stats):
    """shape streams at every Data symbol / pointer target; material streams and textures from them"""
    def word(m, o):
        return struct.unpack_from(">I", m.data, o)[0]

    def tgt(key, o):
        return ptrs[key].get(o)

    def typed_run(key, o, typ, count, src):
        if key in maps and count > 0:
            maps[key].put(o, typ, src, count)

    for key, m in maps.items():
        for o in sorted(m.starts):
            if o % 4 or m.t[o] not in (None, "zero"):
                continue
            cmds = parse_shape(m, o)
            if not cmds or not any(op in (26, 27, 28) for op, _ in cmds):
                continue
            stats["shape streams"] += 1
            end = cmds[-1][1] + 4
            m.put(o, "u32", "shape", (end - o) // 4)
            nv = nt = 0
            for op, c in cmds:
                if op in (27, 28, 26):
                    if op == 27:
                        nv = word(m, c + 4)
                    else:
                        nt = word(m, c + 4)
            for op, c in cmds:
                a = [c + 4 + 4 * i for i in range(SHAPE_OPS.get(op, 0))]
                if op == 27 and tgt(key, a[1]):
                    typed_run(*tgt(key, a[1]), "u16", 3 * nv, "shape:verts")
                elif op == 28 and tgt(key, a[1]):
                    typed_run(*tgt(key, a[1]), "u16", 4 * nt, "shape:tris")
                elif op == 26 and tgt(key, a[1]):
                    typed_run(*tgt(key, a[1]), "u16", 3 * nt, "shape:triList")
                elif op == 25 and tgt(key, a[0]):
                    # charsel 0x60CE4: 4 verts x (s, t) s16 (the "palette" field holds UVs on
                    # textured shapes); RGB888 palettes would be bytes: typed only when 4 * nv fits
                    pass
                elif op == 4 and tgt(key, a[0]):
                    typed_run(*tgt(key, a[0]), "u8", 3 * nv, "shape:op4 s8x3")
                elif op in (7, 8) and tgt(key, a[0]):
                    typed_run(*tgt(key, a[0]), "u8", nt if op == 7 else nv, "shape:bytes")
                elif op == 9 and tgt(key, a[0]):
                    typed_run(*tgt(key, a[0]), "u32", nv, "shape:vtxColours")
                elif op == 34:
                    for i, n in ((0, nv), (1, nt)):
                        if tgt(key, a[i]):
                            typed_run(*tgt(key, a[i]), "f32", 3 * n, "shape:normals")
                elif op == 32 and tgt(key, a[1]):
                    typed_run(*tgt(key, a[1]), "u16", 3 * nv, "shape:remap")
                elif op == 29 and tgt(key, a[1]):
                    walk_mats(maps, ptrs, *tgt(key, a[1]), stats)


def walk_mats(maps, ptrs, key, o, stats):
    """parseMats token loop: 100 (a material: GDL_MC commands up to 0), 101, 0 ends"""
    if key not in maps:
        return
    m = maps[key]
    start = o
    while o + 4 <= m.n:
        tok = struct.unpack_from(">I", m.data, o)[0]
        o += 4
        if tok == 0:
            break
        if tok == 101:
            continue
        if tok != 100:
            stats["mat stream: bad token"] += 1
            return
        while o + 4 <= m.n:
            op = struct.unpack_from(">I", m.data, o)[0]
            o += 4
            if op == 0:
                break
            if op not in MAT_ARGS:
                stats["mat stream: unknown op"] += 1
                return
            if op == 203:
                fmt, w, h, fl = [struct.unpack_from(">I", m.data, o + 4 * i)[0] for i in (0, 2, 3, 4)]
                t = ptrs[key].get(o + 4)
                if t:
                    texture(maps, ptrs, t, fmt, w, h, fl, stats)
            o += 4 * MAT_ARGS[op]
    m.put(start, "u32", "mats", (o - start) // 4)
    stats["mat streams"] += 1


def texture(maps, ptrs, t, fmt, w, h, fl, stats):
    """texels stay bytes (Fast3D reads texels / TLUTs big-endian); records are u32 words"""
    key, o = t
    if key not in maps:
        return
    m = maps[key]
    bpp = fmt & 0xFF
    if fl & 0x10:          # image record {image, uls, ult, width, height}
        m.put(o, "u32", "tex:record", 5)
        img = ptrs[key].get(o)
        if img and img[0] in maps:
            iw, ih = [struct.unpack_from(">I", m.data, o + 4 * i)[0] for i in (3, 4)]
            texels(maps, ptrs, img, fmt, iw, ih, stats)
        return
    texels(maps, ptrs, t, fmt, w, h, stats)


def texels(maps, ptrs, t, fmt, w, h, stats):
    key, o = t
    m = maps[key]
    bpp = fmt & 0xFF
    if fmt == 0x508:       # CI record {?, ?, pixels, palette}
        m.put(o, "u32", "tex:ci-record", 4)
        for i, n in ((2, w * h), (3, 0x200)):
            p = ptrs[key].get(o + 4 * i)
            if p and p[0] in maps:
                maps[p[0]].put(p[1], "u8", "tex:texels", n)
        stats["textures"] += 1
        return
    if bpp in (4, 8, 16, 32) and 0 < w <= 1024 and 0 < h <= 1024:
        m.put(o, "u8", "tex:texels", w * h * bpp // 8)
        stats["textures"] += 1
    else:
        stats["texture: unknown format"] += 1


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--report", action="store_true")
    ap.add_argument("--out", default="port/gen/blob_types.tsv")
    a = ap.parse_args()
    rom = npc.Rom()
    pgb_spec = importlib.util.spec_from_file_location("pgb", str(ROOT / "scripts/port-gen-binds.py"))
    pgb = importlib.util.module_from_spec(pgb_spec)
    argv, sys.argv = sys.argv, [sys.argv[0]]
    pgb_spec.loader.exec_module(pgb)
    sys.argv = argv
    rt = pgb.RomTargets(rom)
    keep = []
    for line in open("port/gen/uso_blobs.tsv"):
        if line.strip() and not line.startswith("#"):
            short, _, sec, size = line.split("\t")
            if sec in SECS:
                keep.append((short, sec))
    maps = {}
    for short, sec in keep:
        m = Map(rom.by_short[short].secs[sec])
        m.idx_hints = []
        maps[(short, sec)] = m
    stats = collections.Counter()
    # 1. relocated words + audio ranges first: they win
    reloc_offs = {}
    targets = collections.defaultdict(set)
    for (short, sec), m in maps.items():
        md = rom.by_short[short]
        offs = set()
        for f1, off, _ in md.relocs(sec):
            if f1 & 7 == 4:
                offs.add(off)
                m.put(off, "ptr", "reloc", force=True)
                t = rt.raw(md, f1 >> 4)
                if t and t[1] in SECS:
                    k = struct.unpack_from(">I", m.data, off)[0]
                    targets[(t[0].short, t[1])].add((t[2] + k) & 0xFFFFFFFF)
        reloc_offs[(short, sec)] = offs
        if short == "bootup" and sec == "Data":
            for lo, hi in AUDIO_RANGES:
                for b in range(lo, hi):
                    m.t[b], m.src[b] = "audio", "audio"
    # 2. code accesses (every module's Text, imports followed)
    for md in rom.mods:
        analyse_text(md, rt, maps, stats)
    # 3. display lists
    for (short, sec), m in maps.items():
        vtx, mtx = gfx_walk(rom.by_short[short], sec, m, reloc_offs[(short, sec)], stats)
        md = rom.by_short[short]
        rel = {off: f1 for f1, off, _ in md.relocs(sec)}
        for off, n in vtx + [(o, None) for o in mtx]:
            t = rt.raw(md, rel[off] >> 4)
            if not t or (t[0].short, t[1]) not in maps:
                continue
            tm = maps[(t[0].short, t[1])]
            p = (t[2] + struct.unpack_from(">I", m.data, off)[0]) & 0xFFFFFFFF
            if n is None:
                tm.put(p, "u32", "gfx:Mtx", 16)
                stats["gfx: Mtx"] += 1
            else:
                for i in range(n):
                    tm.put(p + 16 * i, "u16", "gfx:Vtx", 6)
                    tm.put(p + 16 * i + 12, "u8", "gfx:Vtx", 4)
                stats["gfx: Vtx"] += n
    # 3b. asset streams: shapes, materials, textures
    ptrs = {}
    for (short, sec), m in maps.items():
        md = rom.by_short[short]
        pm = {}
        for f1, off, _ in md.relocs(sec):
            if f1 & 7 != 4:
                continue
            t = rt.raw(md, f1 >> 4)
            if t and t[1] in SECS:
                pm[off] = ((t[0].short, t[1]), (t[2] + struct.unpack_from(">I", m.data, off)[0]) & 0xFFFFFFFF)
        ptrs[(short, sec)] = pm
        m.starts = set(targets[(short, sec)])
        for info, v, w3 in md.syms:
            if (info >> 16) & 7 == 2 and UI.SEC_OF_TAG.get((info >> 20) & 0xF) == sec:
                m.starts.add(v)
    walk_assets(maps, ptrs, stats)
    # 4. strings, index extension, zeros
    for (short, sec), m in maps.items():
        md = rom.by_short[short]
        bounds = set(targets[(short, sec)])
        for info, v, w3 in md.syms:
            k = (info >> 16) & 7
            if k == 2 and UI.SEC_OF_TAG.get((info >> 20) & 0xF) == sec:
                bounds.add(v)
        d = m.data
        i = 0
        while i < m.n:
            if m.t[i] is None and 0x20 <= d[i] < 0x7F and (i % 4 == 0 or i in bounds):
                j = i
                while j < m.n and (0x20 <= d[j] < 0x7F or d[j] in (9, 10, 13, 0x1B)) and m.t[j] in (None, "u8"):
                    j += 1
                if j < m.n and d[j] == 0 and j - i >= 4:
                    for b in range(i, j + 1):
                        if m.t[b] in (None, "u8"):
                            m.t[b], m.src[b] = "str", "str"
                    stats["strings"] += 1
                    i = j + 1
                    continue
            i += 1
        for a0, typ in sorted(set(m.idx_hints)):
            w = WIDTH[typ]
            p = a0 + w
            while p + w <= m.n and p not in bounds and all(m.t[p + b] is None for b in range(w)):
                if typ in ("f32", "f64") and not plausible(d, p, typ):
                    break
                for b in range(w):
                    m.t[p + b], m.src[p + b] = typ, "index"
                p += w
        b = 0
        while b < m.n:     # zero runs of 8+ bytes; shorter ones stay inside their unknown run
            e = b
            while e < m.n and m.t[e] is None and d[e] == 0:
                e += 1
            if e - b >= 8:
                for z in range(b, e):
                    m.t[z], m.src[z] = "zero", "zero"
            b = e + 1 if e == b else e
    # 5. write runs
    out = ["# generated by scripts/port-gen-blob-types.py -- do not edit.  Plan \"M1/M2: byte order\".",
           "# module\tsection\toffset\tsize\ttype\tsource   (types u8 str zero ptr audio conflict: not swapped; u16 u32 f32 u64 f64: swapped per element)"]
    cov = collections.Counter()
    covmod = {}
    conflicts = []
    for (short, sec), m in sorted(maps.items()):
        b = 0
        mc = collections.Counter()
        while b < m.n:
            t = m.t[b] or "unknown"
            s = m.src[b] or "-"
            e = b + 1
            while e < m.n and (m.t[e] or "unknown") == t and (m.src[e] or "-") == s:
                e += 1
            w = WIDTH.get(t, 1)
            if w > 1:   # align the run to whole elements
                e = b + (e - b) // w * w
                if e == b:
                    t, e = "conflict", b + 1
                    m.t[b] = "conflict"
            out.append("%s\t%s\t0x%X\t0x%X\t%s\t%s" % (short, sec, b, e - b, t, s))
            cov[t] += e - b
            mc[t] += e - b
            b = e
        covmod[(short, sec)] = mc
        conflicts += [(short, sec) + c for c in m.conf]
    Path(a.out).write_text("\n".join(out) + "\n")
    total = sum(cov.values())
    print("blob bytes %d; by type: %s" % (total, ", ".join("%s %d (%.1f%%)" % (k, v, 100.0 * v / total) for k, v in cov.most_common())))
    print("analysis: %s" % dict(stats))
    print("conflict bytes: %d (%d first-seen sites)" % (cov["conflict"], len(conflicts)))
    if a.report:
        for (short, sec), mc in sorted(covmod.items(), key=lambda x: -x[1]["unknown"]):
            n = sum(mc.values())
            print("  %-10s %-6s %7d  unknown %6d (%.0f%%)  %s" % (short, sec, n, mc["unknown"], 100.0 * mc["unknown"] / n,
                  " ".join("%s=%d" % kv for kv in mc.most_common(5))))
        for c in conflicts[:60]:
            print("  conflict %s %s +0x%X: %s vs %s (%s / %s)" % c)
    return 0


if __name__ == "__main__":
    sys.exit(main())
