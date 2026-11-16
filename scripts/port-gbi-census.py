#!/usr/bin/env python3
"""Static census of the F3DEX 1.x GBI commands the game emits (PC port, M0).

Reads the splat function asm (asm/nonmatchings/**.s, the exact ROM words) and does a
per-function linear constant propagation over lui / ori / addiu / or / addu / move.
A constant (or an OR-composed value whose top byte comes from a `lui`) is recorded as a
Gfx w0 candidate when it is

  * stored with `sw` to a non-sp base at an 8-aligned-looking pair (w0 at off, w1 at
    off + 4 of the same base within a few instructions), or
  * passed in a1..a3 to a `jal` (the `add(hi, lo)` / `put(g, hi, lo)` helper idiom),

and its top byte is an F3DEX 1.x opcode.  The audio driver (game_libs 0x1C9E4-0x2D374)
and the libultra kernel are skipped: their command lists are the audio ABI, whose
opcodes overlap the low GBI ones.  Floats moved to the FPU are dropped.

The static data side (--rom) scans the ROM for 8-aligned G_ENDDL (B8000000 00000000)
and walks backwards over words that decode as GBI commands, listing static display
lists embedded in data.

Usage:
  scripts/port-gbi-census.py [--asm asm/nonmatchings] [--renames renames.tsv]
                             [--rom baserom.z64] [--detail OPC] [--sites]

This is a census tool for the PC port plan; it is heuristic (no CFG), so every row it
prints should be confirmed in the emitter's source (docs/1080_PC_PORT_PLAN.md, M0
results).
"""
import argparse
import collections
import os
import re
import struct
import sys

GBI1 = {
    0x00: "G_SPNOOP", 0x01: "G_MTX", 0x03: "G_MOVEMEM", 0x04: "G_VTX", 0x06: "G_DL",
    0xAF: "G_LOAD_UCODE", 0xB0: "G_BRANCH_Z", 0xB1: "G_TRI2", 0xB2: "G_MODIFYVTX",
    0xB3: "G_RDPHALF_2", 0xB4: "G_RDPHALF_1", 0xB5: "G_LINE3D", 0xB6: "G_CLEARGEOMETRYMODE",
    0xB7: "G_SETGEOMETRYMODE", 0xB8: "G_ENDDL", 0xB9: "G_SETOTHERMODE_L",
    0xBA: "G_SETOTHERMODE_H", 0xBB: "G_TEXTURE", 0xBC: "G_MOVEWORD", 0xBD: "G_POPMTX",
    0xBE: "G_CULLDL", 0xBF: "G_TRI1", 0xC0: "G_NOOP",
    0xC8: "G_TRI_FILL", 0xC9: "G_TRI_FILL_ZBUFF", 0xCA: "G_TRI_TXTR", 0xCB: "G_TRI_TXTR_ZBUFF",
    0xCC: "G_TRI_SHADE", 0xCD: "G_TRI_SHADE_Z", 0xCE: "G_TRI_SHADE_TXTR", 0xCF: "G_TRI_SHADE_TXTR_Z",
    0xE4: "G_TEXRECT", 0xE5: "G_TEXRECTFLIP", 0xE6: "G_RDPLOADSYNC", 0xE7: "G_RDPPIPESYNC",
    0xE8: "G_RDPTILESYNC", 0xE9: "G_RDPFULLSYNC", 0xEA: "G_SETKEYGB", 0xEB: "G_SETKEYR",
    0xEC: "G_SETCONVERT", 0xED: "G_SETSCISSOR", 0xEE: "G_SETPRIMDEPTH", 0xEF: "G_RDPSETOTHERMODE",
    0xF0: "G_LOADTLUT", 0xF2: "G_SETTILESIZE", 0xF3: "G_LOADBLOCK", 0xF4: "G_LOADTILE",
    0xF5: "G_SETTILE", 0xF6: "G_FILLRECT", 0xF7: "G_SETFILLCOLOR", 0xF8: "G_SETFOGCOLOR",
    0xF9: "G_SETBLENDCOLOR", 0xFA: "G_SETPRIMCOLOR", 0xFB: "G_SETENVCOLOR", 0xFC: "G_SETCOMBINE",
    0xFD: "G_SETTIMG", 0xFE: "G_SETZIMG", 0xFF: "G_SETCIMG",
}
LOW = {0x01, 0x03, 0x04, 0x06}

AUDIO_LO, AUDIO_HI = 0x1C9E4, 0x2D374  # game_libs OoT audio driver (1080_AUDIO.md)
# non-graphics functions whose constants alias GBI words (audio ABI, SP DMEM / PI addresses)
SKIP_NAMES = re.compile(r"^(snd_|Audio|osSp|fs_pi|Mem_init)")
SP, ZERO, RA = 29, 0, 31
TEMPS = set(range(1, 16)) | {24, 25, 31}

LINE = re.compile(r"/\*\s*([0-9A-F]+)\s+([0-9A-F]+)\s+([0-9A-F]{8})\s*\*/")


def load_renames(path):
    ren = {}
    if path and os.path.exists(path):
        for ln in open(path):
            if ln.startswith("#") or "\t" not in ln:
                continue
            a, b = ln.rstrip("\n").split("\t")[:2]
            ren[a] = b
    return ren


def resolve(name, ren):
    seen = set()
    while name in ren and name not in seen:
        seen.add(name)
        name = ren[name]
    return name


def read_funcs(asm_dir):
    for root, _, files in os.walk(asm_dir):
        seg = os.path.relpath(root, asm_dir).split(os.sep)[0]
        if seg == "kernel":
            continue
        for fn in sorted(files):
            if not fn.endswith(".s"):
                continue
            name, words = None, []
            for ln in open(os.path.join(root, fn)):
                if ln.startswith("glabel") and name is None:
                    name = ln.split()[1]
                m = LINE.search(ln)
                if m:
                    words.append((int(m.group(1), 16), int(m.group(2), 16), int(m.group(3), 16)))
            if name and words:
                yield seg, name, words


class Val:
    """A register value: full constant, or a partial one (top byte known from a lui)."""
    __slots__ = ("v", "full")

    def __init__(self, v, full):
        self.v, self.full = v & 0xFFFFFFFF, full


def sx16(i):
    return i - 0x10000 if i & 0x8000 else i


def analyse(words):
    regs = {}
    out = []         # (kind, idx, w0 Val, w1 Val|None)
    stores = []      # (idx, base, off, Val|None)
    pending_jal = None
    for idx, (_, _, w) in enumerate(words):
        op, rs, rt, rd = w >> 26, (w >> 21) & 31, (w >> 16) & 31, (w >> 11) & 31
        imm = w & 0xFFFF
        get = lambda r: Val(0, True) if r == ZERO else regs.get(r)
        dst = None
        newv = None
        if op == 0x0F:      # lui
            dst, newv = rt, Val(imm << 16, True)
        elif op == 0x0D:    # ori
            a = get(rs)
            dst = rt
            if a and a.full:
                newv = Val(a.v | imm, True)
            elif a:
                newv = Val(a.v | imm, False)
        elif op == 0x09:    # addiu
            a = get(rs)
            dst = rt
            if a and a.full:
                newv = Val(a.v + sx16(imm), True)
        elif op == 0x00:
            fn = w & 0x3F
            if fn in (0x25, 0x21):   # or / addu
                a, b = get(rs), get(rt)
                dst = rd
                if a and b and a.full and b.full:
                    newv = Val((a.v | b.v) if fn == 0x25 else (a.v + b.v), True)
                elif fn == 0x25 and (a or b):
                    k = a if a else b
                    if (k.v >> 24) in GBI1 and (k.v >> 24) >= 0xAF or ((k.v >> 24) in LOW):
                        newv = Val(k.v, False)
            elif fn in (0x08, 0x09, 0x0C, 0x0D, 0x18, 0x19, 0x1A, 0x1B, 0x11, 0x13):
                dst = rd if fn == 0x09 else None
            else:
                dst = rd
        elif op == 0x2B:    # sw
            v = get(rt)
            if rs != SP:
                stores.append((idx, rs, sx16(imm), v))
        elif op == 0x11 and rs == 4:   # mtc1: drop float constants
            v = get(rt)
            if v is not None:
                regs[rt] = Val(v.v, v.full)
                regs[rt].full = None  # mark as float use
        elif op == 0x11 and rs in (0, 1):  # mfc1 / dmfc1
            dst = rt
        elif op in (0x02, 0x03, 0x04, 0x05, 0x01, 0x14, 0x15, 0x16, 0x17, 0x06, 0x07):
            if op == 0x03:
                pending_jal = idx + 1
        elif 0x08 <= op <= 0x0F or 0x20 <= op <= 0x27 or op in (0x37, 0x1A, 0x1B):
            dst = rt
        if dst is not None and dst != ZERO:
            if newv is None:
                regs.pop(dst, None)
            else:
                regs[dst] = newv
        if pending_jal == idx:
            args = [regs.get(r) for r in (5, 6, 7)]
            for i, a in enumerate(args):
                if a is not None and a.full is not None and is_gbi_w0(a):
                    lo = args[i + 1] if i + 1 < 3 else None
                    out.append(("arg", idx, a, lo))
            for r in TEMPS:
                regs.pop(r, None)
            pending_jal = None
    # pair stores: w0 at (base, off), w1 at (base, off + 4) within 24 insns either side
    for j, (idx, base, off, v) in enumerate(stores):
        if v is None or v.full is None or not is_gbi_w0(v):
            continue
        w1 = None
        paired = False
        best = None
        for idx2, base2, off2, v2 in stores:
            if base2 == base and off2 == off + 4 and abs(idx2 - idx) <= 24:
                d = abs(idx2 - idx)
                if best is None or d < best:
                    best, paired, w1 = d, True, v2
        if paired and off % 8 == 0:
            out.append(("sw", idx, v, w1))
    return out


def is_gbi_w0(v):
    op = v.v >> 24
    if op not in GBI1 or op == 0x00:
        return False
    if op in LOW:
        # G_MTX / G_MOVEMEM / G_VTX / G_DL: param byte + length field
        if op == 0x06:
            return (v.v & 0x00FFFFFF) in (0, 0x010000) or not v.full
        if op == 0x01:
            return (v.v & 0xFFFF) in (0x40,)
        if op == 0x03:
            return ((v.v >> 16) & 0xFF) in (0x80, 0x82, 0x84, 0x86, 0x88, 0x8A, 0x8C, 0x8E,
                                             0x90, 0x92, 0x94, 0x96, 0x98, 0x9A, 0x9C, 0x9E)
        if op == 0x04:
            return True
    if v.full and op in (0xBF, 0xC0) and (v.v & 0xFFFFFF):
        return False   # -1.0f / -2.0f style floats
    if v.full and op in (0xB8, 0xE6, 0xE7, 0xE8, 0xE9) and (v.v & 0xFFFFFF):
        return False
    if v.full and op in range(0xC1, 0xC8):
        return False
    if v.full and op in (0xFD, 0xFE, 0xFF) and (v.v & 0x0007F000):
        return False   # SETxIMG: bits 12-18 are zero (fmt / siz / width-1 only); -1 etc.
    if v.full and op == 0xB1 and any(((v.v >> k) & 0xFF) >= 0x80 for k in (0, 8, 16)):
        return False   # F3DEX1 G_TRI2 indices are v*2 < 0x80 (0xB1FFxxxx = PI addresses)
    if v.full and op == 0x04 and (v.v & 0xF) != 0xF:
        return False   # F3DEX1 G_VTX low 10 bits = n*16 - 1 (0x04000FC0 = SP DMEM addresses)
    if v.full and v.v == 0xFF000000:
        return False
    if v.full and op == 0xB0:
        return False   # 0xB0000B70-style cartridge (PI) addresses, not G_BRANCH_Z
    return True


def fmt(v):
    if v is None:
        return "?"
    return ("%08X" % v.v) if v.full else ("%02X......" % (v.v >> 24))


def load_blob(path):
    data = open(path, "rb").read()
    if data[:4] == b"Yay0":
        import crunch64  # the repo venv has it (scripts/extract-uso-yay0.py)
        data = crunch64.yay0.decompress(data)
    return data


def rom_static_dls(rom_path):
    data = load_blob(rom_path)
    hits = []
    for off in range(0, len(data) - 8, 8):
        if data[off:off + 8] != b"\xB8\x00\x00\x00\x00\x00\x00\x00":
            continue
        start = off
        while start >= 8:
            w0, w1 = struct.unpack(">II", data[start - 8:start])
            op = w0 >> 24
            if op not in GBI1 or op in (0x00, 0xB8):
                break
            if op in (0xE6, 0xE7, 0xE8, 0xE9) and (w0 & 0xFFFFFF or w1):
                break
            if op in (0x01, 0x03, 0x04, 0x06) and not (0x80000000 <= w1 < 0x80800000 or w1 >> 24 < 0x10):
                break
            start -= 8
        n = (off - start) // 8
        if n >= 2:
            cmds = [struct.unpack(">II", data[p:p + 8]) for p in range(start, off + 8, 8)]
            hits.append((start, cmds))
    return hits


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--asm", default="asm/nonmatchings")
    ap.add_argument("--renames", default="renames.tsv")
    ap.add_argument("--rom", default=None, help="also scan the ROM for static G_ENDDL-terminated lists")
    ap.add_argument("--scan", nargs="*", default=[], help="more blobs to scan like --rom (Yay0 is decompressed)")
    ap.add_argument("--detail", default=None, help="hex opcode: list every site with w0 / w1")
    ap.add_argument("--sites", action="store_true", help="print every site")
    a = ap.parse_args()
    ren = load_renames(a.renames)

    by_op = collections.defaultdict(lambda: {"n": 0, "funcs": collections.Counter(), "w": collections.Counter()})
    sites = []
    seen_rom = set()
    for seg, name, words in read_funcs(a.asm):
        if words[0][0] in seen_rom:
            continue
        seen_rom.add(words[0][0])
        if seg == "game_libs" and AUDIO_LO <= words[0][1] < AUDIO_HI:
            continue
        fname = resolve(name, ren)
        if SKIP_NAMES.match(fname):
            continue
        for kind, idx, w0, w1 in analyse(words):
            op = w0.v >> 24
            e = by_op[op]
            e["n"] += 1
            e["funcs"]["%s:%s" % (seg, fname)] += 1
            e["w"][(fmt(w0), fmt(w1))] += 1
            sites.append((op, seg, fname, words[idx][1], kind, fmt(w0), fmt(w1)))

    if a.detail:
        want = int(a.detail, 16)
        for op, seg, fname, va, kind, w0, w1 in sites:
            if op == want:
                print("%-12s %-44s %06X %-3s %s %s" % (seg, fname, va, kind, w0, w1))
        return
    if a.sites:
        for s in sorted(sites):
            print("%02X %-12s %-44s %06X %-3s %s %s" % s)
        return
    print("op  name                   sites funcs  distinct (w0, w1) [top 6]")
    for op in sorted(by_op):
        e = by_op[op]
        top = ", ".join("%s/%s x%d" % (k[0], k[1], c) for k, c in e["w"].most_common(6))
        print("%02X  %-22s %5d %5d  %s" % (op, GBI1[op], e["n"], len(e["funcs"]), top))
    for blob in ([a.rom] if a.rom else []) + a.scan:
        hits = rom_static_dls(blob)
        print("\nstatic G_ENDDL-terminated lists in %s: %d" % (blob, len(hits)))
        for start, cmds in hits:
            ops = " ".join("%02X" % (c[0] >> 24) for c in cmds)
            print("  0x%06X  %2d cmds  %s" % (start, len(cmds), ops))


if __name__ == "__main__":
    main()
