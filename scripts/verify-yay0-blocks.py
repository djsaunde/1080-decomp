#!/usr/bin/env python3
"""Verify every built Yay0 code block against its ROM-extracted ground truth.

Strict since 2026-11-16: for each block,
  - the decompressed build must equal the decompressed ROM block byte for byte
    (reloc fields included: jal / lo16 diffs used to be tolerated, but there are
    none, so any diff is now a FAILURE), and
  - the compressed build must equal splat's compressed extract
    assets/<blk>_yay0.bin byte for byte (crunch64 reproduces the original Yay0
    encoder's stream).
The ROM itself links the built blocks (Makefile YAY0_O_FILES), so `make` /
`make verify` already prove this; this script names the block and word that
differ when they do not.

Usage: python3 scripts/verify-yay0-blocks.py   (after `make <blocks>` or with
       --build to make the block targets first)
"""
import struct, subprocess, sys
import crunch64

PAIRS = [
    ("assets/mgrproc_uso_block_1.bin",  "build/assets/mgrproc_uso_block1_yay0.bin"),
    ("assets/game_uso_block_1.bin",     "build/assets/game_uso_block1_yay0.bin"),
    ("assets/timproc_uso_block_1.bin",  "build/assets/timproc_uso_block1_yay0.bin"),
    ("assets/timproc_uso_block_3.bin",  "build/assets/timproc_uso_block3_yay0.bin"),
    ("assets/timproc_uso_block_5.bin",  "build/assets/timproc_uso_block5_yay0.bin"),
    ("assets/map4_data_uso_block_2.bin","build/assets/map4_data_uso_block2_yay0.bin"),
]

def classify(o, b):
    if (o >> 26) == 3 and (b >> 26) == 3:
        return "jal"
    if (o >> 16) == (b >> 16):
        return "lo16"
    return "other"

def main():
    if "--build" in sys.argv:
        subprocess.check_call(["make"] + [b for _, b in PAIRS])
    fail = 0
    for orig_p, built_p in PAIRS:
        try:
            orig = open(orig_p, "rb").read()
            built = crunch64.yay0.decompress(open(built_p, "rb").read())
        except FileNotFoundError as e:
            print(f"SKIP  {built_p}: {e}")
            continue
        counts = {"jal": 0, "lo16": 0, "other": 0}
        others = []
        for i in range(0, min(len(orig), len(built)), 4):
            if orig[i:i+4] != built[i:i+4]:
                o, = struct.unpack(">I", orig[i:i+4])
                b, = struct.unpack(">I", built[i:i+4])
                k = classify(o, b)
                counts[k] += 1
                if k == "other":
                    others.append((i, o, b))
        comp_p = built_p.replace("build/assets/", "assets/")
        comp_ok = open(comp_p, "rb").read() == open(built_p, "rb").read()
        ok = len(orig) == len(built) and not any(counts.values()) and comp_ok
        fail += not ok
        print(f'{"OK  " if ok else "FAIL"}  {built_p}: len {len(orig):#x}/{len(built):#x}, '
              f'jal={counts["jal"]} lo16={counts["lo16"]} other={counts["other"]}, '
              f'compressed {"identical" if comp_ok else "DIFFERS from " + comp_p}')
        for i, o, b in others[:12]:
            print(f"        OTHER at {i:#07x}: {o:08x} vs {b:08x}")
    return 1 if fail else 0

if __name__ == "__main__":
    sys.exit(main())
