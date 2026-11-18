#!/usr/bin/env python3
"""Port vs N64 reference frames (PC port plan 22): step alignment, difference scores, side-by-side sheets.

Needs Pillow + numpy:  uv run --with pillow --with numpy port/tools/n64ref/compare.py ...

  compare.py align  PORTDIR EMUDIR STEPS [--max-off 6]
      For each step S in STEPS (e.g. 300-400:10,700), the emulator step S+k (|k| <= max-off) that
      looks most like the port's frame S; prints the per-step best k and the score, and the median k.
  compare.py score  PORTDIR EMUDIR STEPS [--off K]
      Per step: mean |difference| (0..255) of the two frames (160x120, blurred) and the per-channel
      mean colour of each: a quick list of the steps that differ.
  compare.py sheet  OUT.png PORTDIR EMUDIR STEPS [--off K] [--cols 2] [--title T] [--diff]
      Side-by-side pairs: port (left) and N64 (right), labelled with the step (and a diff image with
      --diff).

PORTDIR holds native_sSSSSSS.png (PORT_NATIVE_PNG) or png/tNNNNNN_sSSSSSS.png / fNNNNNN_sSSSSSS.png
(capture.sh); EMUDIR holds n64ref's sSSSSSS.png. Every frame is brought to 320x240.
"""
import argparse
import glob
import os
import re
import sys

import numpy as np
from PIL import Image, ImageDraw, ImageFilter

W, H = 320, 240


def parse_steps(spec):
    out = []
    for part in spec.split(","):
        m = re.fullmatch(r"(\d+)(?:-(\d+)(?::(\d+))?)?", part.strip())
        if not m:
            sys.exit(f"bad steps {part}")
        a = int(m.group(1))
        b = int(m.group(2) or a)
        d = int(m.group(3) or 1)
        out.extend(range(a, b + 1, d))
    return out


def index_port(d):
    idx = {}
    for f in glob.glob(os.path.join(d, "**", "*.png"), recursive=True):
        m = re.search(r"(?:native_s|_s)(\d+)\.png$", f)
        if m:
            idx.setdefault(int(m.group(1)), f)
    return idx


def index_emu(d):
    idx = {}
    for f in glob.glob(os.path.join(d, "s*.png")):
        m = re.search(r"/s(\d+)\.png$", f)
        if m:
            idx[int(m.group(1))] = f
    return idx


_cache = {}


def load(path):
    if path not in _cache:
        im = Image.open(path).convert("RGB")
        if im.size != (W, H):
            im = im.resize((W, H), Image.BILINEAR)
        _cache[path] = im
    return _cache[path]


def feat(path):
    k = ("f", path)
    if k not in _cache:
        im = load(path).resize((160, 120), Image.BILINEAR).filter(ImageFilter.GaussianBlur(2))
        _cache[k] = np.asarray(im, dtype=np.float32)
    return _cache[k]


def diff(a, b):
    return float(np.abs(feat(a) - feat(b)).mean())


def cmd_align(a):
    P, E = index_port(a.port), index_emu(a.emu)
    offs = []
    for s in parse_steps(a.steps):
        if s not in P:
            continue
        best = None
        for k in range(-a.max_off, a.max_off + 1):
            if s + k in E:
                d = diff(P[s], E[s + k])
                if best is None or d < best[1]:
                    best = (k, d)
        if best:
            offs.append(best[0])
            zero = diff(P[s], E[s]) if s in E else float("nan")
            print(f"step {s:6d}: best k {best[0]:+d} diff {best[1]:6.2f} (k=0: {zero:6.2f})")
    if offs:
        print(f"median k {int(np.median(offs)):+d} over {len(offs)} steps")


def cmd_score(a):
    P, E = index_port(a.port), index_emu(a.emu)
    for s in parse_steps(a.steps):
        if s in P and s + a.off in E:
            fp, fe = feat(P[s]), feat(E[s + a.off])
            mp, me = fp.reshape(-1, 3).mean(0), fe.reshape(-1, 3).mean(0)
            print(f"step {s:6d}: diff {np.abs(fp - fe).mean():6.2f}  port rgb {mp[0]:5.1f} {mp[1]:5.1f} {mp[2]:5.1f}"
                  f"  n64 rgb {me[0]:5.1f} {me[1]:5.1f} {me[2]:5.1f}")


def cmd_sheet(a):
    P, E = index_port(a.port), index_emu(a.emu)
    pairs = [s for s in parse_steps(a.steps) if s in P and s + a.off in E]
    if not pairs:
        sys.exit("sheet: no steps with both frames")
    per = 3 if a.diff else 2
    tw = W * per + 8
    th = H + 14
    rows = (len(pairs) + a.cols - 1) // a.cols
    top = 18 if a.title else 0
    sheet = Image.new("RGB", (tw * a.cols, th * rows + top), (40, 40, 40))
    dr = ImageDraw.Draw(sheet)
    if a.title:
        dr.text((4, 3), a.title, fill=(255, 255, 255))
    for i, s in enumerate(pairs):
        x, y = (i % a.cols) * tw, top + (i // a.cols) * th
        p, e = load(P[s]), load(E[s + a.off])
        sheet.paste(p, (x, y + 14))
        sheet.paste(e, (x + W, y + 14))
        if a.diff:
            d = np.abs(np.asarray(p, np.int16) - np.asarray(e, np.int16)).clip(0, 255).astype(np.uint8)
            sheet.paste(Image.fromarray(d), (x + 2 * W, y + 14))
        dr.text((x + 2, y + 1), f"step {s}  port", fill=(255, 255, 160))
        dr.text((x + W + 2, y + 1), f"N64 (n64ref) step {s + a.off}  diff {diff(P[s], E[s + a.off]):.1f}",
                fill=(160, 255, 160))
    sheet.save(a.out)
    print(f"sheet: {a.out} ({len(pairs)} pairs)")


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sp = ap.add_subparsers(dest="cmd", required=True)
    p = sp.add_parser("align")
    p.add_argument("port"), p.add_argument("emu"), p.add_argument("steps")
    p.add_argument("--max-off", type=int, default=6)
    p = sp.add_parser("score")
    p.add_argument("port"), p.add_argument("emu"), p.add_argument("steps")
    p.add_argument("--off", type=int, default=0)
    p = sp.add_parser("sheet")
    p.add_argument("out"), p.add_argument("port"), p.add_argument("emu"), p.add_argument("steps")
    p.add_argument("--off", type=int, default=0)
    p.add_argument("--cols", type=int, default=2)
    p.add_argument("--title")
    p.add_argument("--diff", action="store_true")
    a = ap.parse_args()
    {"align": cmd_align, "score": cmd_score, "sheet": cmd_sheet}[a.cmd](a)


if __name__ == "__main__":
    main()
