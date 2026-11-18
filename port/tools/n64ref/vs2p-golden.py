#!/usr/bin/env python3
"""The N64's per-step rider positions for port/host/vs2p.input (plan 24), from an n64ref --watch run.

    n64ref --rom baserom.z64 --out DIR --input port/host/vs2p.input --max-vi 17500 --watch-from 1780 \\
        --watch 0x8032FAD8:0xA8C,0x80313958:0xA8C
    port/tools/n64ref/vs2p-golden.py DIR/watch.bin port/tools/n64ref/vs2p-golden.tsv [--every 8] [--first 1792] [--last 7799]

The two addresses are the 2P VS riders' SnowBoarders (P1, P2: rival links each other) on a 4 MiB console
with this script; the heap puts both rounds' riders there. The last --watch record of a step holds that step's
results (the port's state.log line of the step). Writes `step r0x r0y r0z r1x r1y r1z` (float bits, hex,
dnode.vqs.trans at +0xB4) every --every steps; modes-check.py's vs2p check compares the port's r0= / r1=.
"""
import argparse, struct

SZ = 0xA8C
TRANS = 0xB4


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('watch')
    ap.add_argument('out')
    ap.add_argument('--every', type=int, default=8)
    ap.add_argument('--first', type=int, default=1792)
    ap.add_argument('--last', type=int, default=7799)
    a = ap.parse_args()
    b = open(a.watch, 'rb').read()
    rec = 8 + 2 * SZ
    last = {}
    for o in range(0, len(b) - rec + 1, rec):
        _vi, st = struct.unpack_from('<II', b, o)
        last[st] = o + 8
    with open(a.out, 'w') as f:
        f.write('# N64 (n64ref, plan 24) rider positions for port/host/vs2p.input: step, P1 x y z, P2 x y z '
                '(f32 bits); port/tools/n64ref/vs2p-golden.py\n')
        n = 0
        for st in range(a.first, a.last + 1, a.every):
            if st not in last:
                continue
            o = last[st]
            w = [struct.unpack_from('>I', b, o + r * SZ + TRANS + 4 * k)[0] for r in (0, 1) for k in range(3)]
            f.write('%d %s\n' % (st, ' '.join('%08x' % x for x in w)))
            n += 1
    print('vs2p-golden: %d steps -> %s' % (n, a.out))


if __name__ == '__main__':
    main()
