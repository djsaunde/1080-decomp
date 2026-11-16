#!/usr/bin/env python3
"""Generate per-function .s files for a relocatable USO segment as raw .word
directives, matching the game_libs/bootup_uso style.

Splat's auto-disassembler can't be used for relocatable USO text because
JAL/branch immediates are placeholders (HI16=0, LO16=0) that get patched at
runtime. Disassembled mnemonics introduce phantom branch labels that don't
resolve. Raw .word directives sidestep this — the linker produces the same
bytes either way.

Usage:
    python3 scripts/generate-uso-asm.py <segment_name> <text_rom_start> <text_size>
            [--sym SYM_ROM:SYM_SIZE]

With --sym (the module's Sym payload ROM offset and byte size, e.g. from the
port extractor manifest: section rom_offset + 12), function starts are the union
of the module's Text symbols (kind 2, section 5; docs/USO_FORMAT.md section 6),
the prologues, and leaf ends (a `jr ra` that no earlier branch of the function
jumps past, then any zero padding). The segment must then start at the Text
payload (record + 12) so names equal USO Text offsets. Used for the 2026-11-16
new modules (docs/1080_NEW_MODULES.md).

Example (gui_uso):
    python3 scripts/generate-uso-asm.py gui_uso 0x58D530 0x4880

Writes:
    asm/nonmatchings/<segment>/<segment>/<segment>_func_<offset>.s
    src/<segment>/<segment>.c       (INCLUDE_ASM lines for each function)

Function boundaries: scans for `addiu $sp, -N` prologues. The first chunk
(before the first prologue) is treated as one function (leaf-only code).
"""
import struct
import sys
from pathlib import Path


def leaf_ends(data, text_start, text_end, starts):
    """Ends of functions found by a linear scan: a `jr ra` that no branch of the
    current function targets past (and no `jr <reg>` jumptable seen), followed by
    zero padding. Returns the next-function starts (ROM offsets) not in `starts`."""
    out = set()
    o, maxt, jt = text_start, text_start, False
    while o < text_end:
        if o in starts:
            maxt, jt = o, False
        x = struct.unpack(">I", data[o:o+4])[0]
        op = x >> 26
        if op in (1, 4, 5, 6, 7, 20, 21, 22, 23) or (op == 17 and ((x >> 21) & 31) == 8):
            off = struct.unpack(">h", struct.pack(">H", x & 0xFFFF))[0]
            maxt = max(maxt, o + 4 + off * 4)
        if op == 0 and (x & 0x3F) == 8 and ((x >> 21) & 31) != 31:
            jt = True
        if x == 0x03E00008 and maxt <= o + 4 and not jt:
            e = o + 8
            while e < text_end and data[e:e+4] == b"\0\0\0\0":
                e += 4
            if e < text_end and e not in starts:
                out.add(e)
            o, maxt, jt = e, e, False
            continue
        o += 4
    return out


def main():
    args = sys.argv[1:]
    sym = None
    if "--sym" in args:
        i = args.index("--sym")
        sym = [int(v, 0) for v in args[i + 1].split(":")]
        del args[i:i + 2]
    if len(args) != 3:
        print(__doc__)
        sys.exit(1)
    sys.argv = [sys.argv[0]] + args
    seg = sys.argv[1]
    text_start = int(sys.argv[2], 0)
    text_size = int(sys.argv[3], 0)
    text_end = text_start + text_size
    prefix = f"{seg}_func"

    data = open("baserom.z64", "rb").read()

    # Find prologues (addiu $sp, $sp, -N where N has top bit set)
    prols = []
    for i in range(text_start, text_end, 4):
        w = struct.unpack(">I", data[i:i+4])[0]
        if (w >> 16) == 0x27BD and (w & 0x8000):
            prols.append(i)

    if sym:
        starts = {text_start} | set(prols)
        s0, ssz = sym
        for k in range(0, ssz, 12):
            info, val = struct.unpack(">II", data[s0 + k:s0 + k + 8])
            info >>= 16
            if info & 7 == 2 and (info >> 4) & 0xF == 5 and val < text_size:
                starts.add(text_start + val)
        starts |= leaf_ends(data, text_start, text_end, starts)
        prols = sorted(starts - {text_start})
    boundaries = [text_start] + prols + [text_end]

    # asm-processor inserts a trailing nop into .s files with < 2 words. Merge
    # any too-small chunk forward into the next function so each emitted .s
    # file has at least 2 words (8 bytes). The first chunk (text_start to
    # first prologue) is sometimes a tiny placeholder branch; if so, fold it
    # into the first prologue function.
    cleaned = [boundaries[0]]
    for b in boundaries[1:]:
        if b - cleaned[-1] >= 8:
            cleaned.append(b)
        # else: drop b — its chunk merges into the previous (cleaned[-1])
    if cleaned[-1] != text_end:
        cleaned.append(text_end)
    boundaries = cleaned

    asm_dir = Path(f"asm/nonmatchings/{seg}/{seg}")
    asm_dir.mkdir(parents=True, exist_ok=True)
    for f in asm_dir.glob("*.s"):
        f.unlink()

    c_lines = ['#include "common.h"', '']
    for idx in range(len(boundaries) - 1):
        fn_start = boundaries[idx]
        fn_end = boundaries[idx+1]
        if fn_start == fn_end:
            continue
        addr = fn_start - text_start
        name = f"{prefix}_{addr:08X}"
        size = fn_end - fn_start

        out = [f"nonmatching {name}, 0x{size:X}\n", f"glabel {name}"]
        for off in range(fn_start, fn_end, 4):
            w = struct.unpack(">I", data[off:off+4])[0]
            seg_off = off - text_start
            out.append(f"    /* {off:06X} {seg_off:08X} {w:08X} */  .word 0x{w:08X}")
        out.append(f"endlabel {name}")
        (asm_dir / f"{name}.s").write_text("\n".join(out) + "\n")

        c_lines.append(f'INCLUDE_ASM("asm/nonmatchings/{seg}/{seg}", {name});')
        c_lines.append('')

    src_dir = Path(f"src/{seg}")
    src_dir.mkdir(parents=True, exist_ok=True)
    (src_dir / f"{seg}.c").write_text("\n".join(c_lines) + "\n")

    nfn = len([b for i, b in enumerate(boundaries[:-1]) if boundaries[i+1] > b])
    print(f"Wrote {nfn} function .s files for {seg} ({len(prols)} non-zero starts) + src/{seg}/{seg}.c")


if __name__ == "__main__":
    main()
