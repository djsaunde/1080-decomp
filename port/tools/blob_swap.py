"""Byte-order hook of the USO blob build (docs/1080_PC_PORT_PLAN.md, "M1/M2: byte order").

port/tools/gen-uso-blobs.py calls swap_section() on every Data / RoData section before it emits
the section's `_init` copy.  port/gen/blob_types.tsv (scripts/port-gen-blob-types.py) types
each byte range; u16 / u32 / f32 / u64 / f64 runs are reversed per element, everything else
(u8, str, zero, unknown, conflict, ptr: port/uso writes relocated words in host order, audio:
port/audio swaps those bootup ranges at run time) stays as on the cartridge.
"""
import os

SWAP = {"u16": 2, "u32": 4, "f32": 4, "u64": 8, "f64": 8}
_cache = {}


def load(tsv):
    if tsv not in _cache:
        runs = {}
        if os.path.exists(tsv):
            for line in open(tsv):
                if not line.strip() or line.startswith("#"):
                    continue
                mod, sec, off, size, typ = line.rstrip("\n").split("\t")[:5]
                if typ in SWAP:
                    runs.setdefault((mod, sec), []).append((int(off, 16), int(size, 16), SWAP[typ]))
        _cache[tsv] = runs
    return _cache[tsv]


def swap_section(tsv, mod, sec, data):
    """-> (bytes in host order where typed, bytes swapped)"""
    runs = load(tsv).get((mod, sec))
    if not runs:
        return data, 0
    b = bytearray(data)
    n = 0
    for off, size, w in runs:
        if off + size > len(b) or size % w:
            raise ValueError("blob_types: %s %s run 0x%X+0x%X outside the section or not whole" % (mod, sec, off, size))
        for o in range(off, off + size, w):
            b[o:o + w] = b[o:o + w][::-1]
        n += size
    return bytes(b), n
