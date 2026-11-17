#!/usr/bin/env python3
"""Contact sheet of f3dreplay PNGs (PC port plan 19.8), stdlib only (no PIL).

    contact_sheet.py OUT.png IN.png... [--cols N] [--scale 1|2]

Reads 8-bit RGB / RGBA PNGs (any filter, as f3dreplay's stored-deflate writer and most
others produce), shrinks each by --scale (nearest; default 2, so 320x240 -> 160x120), tiles
them --cols wide (default 8) and writes one RGB PNG. Each tile is labelled with the number
in its file name (the gfx task number, e.g. t000120.png -> 120) in a 3x5 pixel font.
"""

import argparse
import re
import struct
import sys
import zlib

FONT = {  # 3x5, rows top-down, bit 2 = left column
    "0": [7, 5, 5, 5, 7], "1": [2, 6, 2, 2, 7], "2": [7, 1, 7, 4, 7], "3": [7, 1, 7, 1, 7],
    "4": [5, 5, 7, 1, 1], "5": [7, 4, 7, 1, 7], "6": [7, 4, 7, 5, 7], "7": [7, 1, 1, 1, 1],
    "8": [7, 5, 7, 5, 7], "9": [7, 5, 7, 1, 7], "?": [7, 1, 3, 0, 2],
}


def read_png(path):
    data = open(path, "rb").read()
    if data[:8] != b"\x89PNG\r\n\x1a\n":
        raise ValueError(f"{path}: not a PNG")
    pos, idat, w = 8, [], None
    while pos < len(data):
        n, typ = struct.unpack(">I4s", data[pos:pos + 8])
        body = data[pos + 8:pos + 8 + n]
        if typ == b"IHDR":
            w, h, depth, ctype, _, _, interlace = struct.unpack(">IIBBBBB", body)
            if depth != 8 or ctype not in (2, 6) or interlace:
                raise ValueError(f"{path}: only 8-bit RGB/RGBA, non-interlaced")
            bpp = 4 if ctype == 6 else 3
        elif typ == b"IDAT":
            idat.append(body)
        pos += 12 + n
    raw = zlib.decompress(b"".join(idat))
    stride = w * bpp
    rows, prev = [], bytearray(stride)
    for y in range(h):
        f = raw[y * (stride + 1)]
        cur = bytearray(raw[y * (stride + 1) + 1:(y + 1) * (stride + 1)])
        if f == 1:
            for i in range(bpp, stride):
                cur[i] = (cur[i] + cur[i - bpp]) & 255
        elif f == 2:
            for i in range(stride):
                cur[i] = (cur[i] + prev[i]) & 255
        elif f == 3:
            for i in range(stride):
                cur[i] = (cur[i] + ((cur[i - bpp] if i >= bpp else 0) + prev[i]) // 2) & 255
        elif f == 4:
            for i in range(stride):
                a = cur[i - bpp] if i >= bpp else 0
                b, c = prev[i], prev[i - bpp] if i >= bpp else 0
                p = a + b - c
                pa, pb, pc = abs(p - a), abs(p - b), abs(p - c)
                cur[i] = (cur[i] + (a if pa <= pb and pa <= pc else b if pb <= pc else c)) & 255
        rows.append(bytes(cur) if bpp == 3 else bytes(b for i in range(0, stride, 4) for b in cur[i:i + 3]))
        prev = cur
    return w, h, rows  # rows of RGB bytes


def write_png(path, w, h, rows):
    raw = b"".join(b"\x00" + bytes(r) for r in rows)

    def chunk(t, b):
        return struct.pack(">I", len(b)) + t + b + struct.pack(">I", zlib.crc32(t + b) & 0xFFFFFFFF)

    with open(path, "wb") as f:
        f.write(b"\x89PNG\r\n\x1a\n" + chunk(b"IHDR", struct.pack(">IIBBBBB", w, h, 8, 2, 0, 0, 0)) +
                chunk(b"IDAT", zlib.compress(raw, 6)) + chunk(b"IEND", b""))


def label_of(path):
    m = re.findall(r"\d+", path.rsplit("/", 1)[-1])
    return str(int(m[-1])) if m else "?"


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("out")
    ap.add_argument("files", nargs="+")
    ap.add_argument("--cols", type=int, default=8)
    ap.add_argument("--scale", type=int, default=2)
    a = ap.parse_args()
    tw = th = None
    tiles = []
    for p in a.files:
        try:
            w, h, rows = read_png(p)
        except (OSError, ValueError, zlib.error) as e:
            print(f"contact_sheet: skip {e}", file=sys.stderr)
            continue
        s = a.scale
        small = [b"".join(r[x * 3:x * 3 + 3] for x in range(0, w, s)) for r in rows[::s]]
        if tw is None:
            tw, th = len(small[0]) // 3, len(small)
        tiles.append((label_of(p), small))
    if not tiles:
        print("contact_sheet: no images", file=sys.stderr)
        return 1
    gap, lab = 2, 14  # label strip: the 5-px font at 2x plus padding
    cols = min(a.cols, len(tiles))
    nrows = (len(tiles) + cols - 1) // cols
    W, H = cols * (tw + gap) + gap, nrows * (th + lab + gap) + gap
    canvas = [bytearray(b"\x30\x30\x30" * W) for _ in range(H)]
    for i, (name, small) in enumerate(tiles):
        x0, y0 = gap + (i % cols) * (tw + gap), gap + (i // cols) * (th + lab + gap)
        for y in range(min(th, len(small))):
            r = small[y][:tw * 3]
            canvas[y0 + lab + y][x0 * 3:x0 * 3 + len(r)] = r
        cx = x0 + 2
        for ch in name:  # the font at 2x
            for gy, bits in enumerate(FONT.get(ch, FONT["?"])):
                for gx in range(3):
                    if bits & (4 >> gx):
                        for dy in range(2):
                            o = (cx + 2 * gx) * 3
                            canvas[y0 + 2 + 2 * gy + dy][o:o + 6] = b"\xff\xff\x60" * 2
            cx += 8
    write_png(a.out, W, H, canvas)
    print(f"contact_sheet: {a.out}: {len(tiles)} frames, {cols}x{nrows}, {W}x{H}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
