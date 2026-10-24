#!/usr/bin/env python3
"""Find in-game sibling functions of every non-exact function.

For each function in report.json with fuzzy < 100, compare its opcode
sequence (primary opcode, SPECIAL funct, COP1 fmt/funct) against every other
.s in asm/nonmatchings with difflib, and print the best partners:

    target size fuzzy partner size partner_fuzzy ratio

A partner that is already EXACT with ratio >= ~0.75 is a template: port its C
(or its NCC / match_imports donor) to the target, adjusting offsets/constants.
Run 154 (kit LXXXIII) turned 4 of the top 6 hits into exacts.

usage: scripts/sibling-scan.py [--min 0.65] [--size-tol 0.35] [--exact-only]
"""
import argparse, difflib, json, os, re

ap = argparse.ArgumentParser()
ap.add_argument('--min', type=float, default=0.65)
ap.add_argument('--size-tol', type=float, default=0.35)
ap.add_argument('--exact-only', action='store_true', help='only report partners at 100%%')
ap.add_argument('--top', type=int, default=3)
a = ap.parse_args()

root = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
rep = json.load(open(os.path.join(root, 'report.json')))
fz = {}
for u in rep['units']:
    for f in u.get('functions', []):
        fz[f['name']] = f.get('fuzzy_match_percent', 0)

words = {}
for dp, dn, fs in os.walk(os.path.join(root, 'asm', 'nonmatchings')):
    for fn in fs:
        if not fn.endswith('.s'):
            continue
        ws = []
        for line in open(os.path.join(dp, fn), errors='ignore'):
            m = re.search(r'/\* [0-9A-F]+ [0-9A-F]+ ([0-9A-F]{8}) \*/', line)
            if m:
                ws.append(int(m.group(1), 16))
        if len(ws) >= 12:
            words[fn[:-2]] = ws


def sig(ws):
    out = []
    for w in ws:
        op = w >> 26
        if op == 0:
            out.append(('s', w & 63))
        elif op == 0x11:
            out.append(('f', (w >> 21) & 31, w & 63))
        else:
            out.append(op)
    return out


sigs = {k: sig(v) for k, v in words.items()}
for t in sorted(k for k in words if k in fz and fz[k] < 100 and len(words[k]) >= 16):
    best = []
    n = len(words[t])
    for k, s in sigs.items():
        if k == t or abs(len(s) - n) > a.size_tol * n:
            continue
        if a.exact_only and fz.get(k, 0) < 100:
            continue
        sm = difflib.SequenceMatcher(None, sigs[t], s, autojunk=False)
        if sm.real_quick_ratio() < a.min or sm.quick_ratio() < a.min:
            continue
        r = sm.ratio()
        if r >= a.min:
            best.append((r, k))
    best.sort(reverse=True)
    for r, k in best[:a.top]:
        print(t, n * 4, fz[t], k, len(words[k]) * 4, fz.get(k), round(r, 3), flush=True)
