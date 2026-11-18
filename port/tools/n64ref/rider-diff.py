#!/usr/bin/env python3
"""Field-by-field diff of the riders' SnowBoarders: the port (PORT_TRACE_RIDERBIN) against the N64 (n64ref --watch).

    PORT_TRACE_RIDERBIN=riders.bin run32 <i386 build>/tenshoe --rom R --input S --frames N --trace state.log
    n64ref --rom R --out DIR --input S --max-vi V --watch-from F --watch 0x8032FAD8:0xA8C,0x80313958:0xA8C
    port/tools/n64ref/rider-diff.py DIR/watch.bin riders.bin [--from S] [--to S] [--max N]

Prints, per step and rider, every word that differs (offset: N64 / port), first steps first (plan 24). Equal
modulo the port's conventions: N64 pointers (0x80xxxxxx) are skipped, and a word equal byte-swapped, halfword-
swapped or with its byte pairs swapped (the port's host-order tags, s16 pairs and u8 pairs) counts as equal.
Only the i386 build has the N64 layout. The --watch addresses are the 2P VS riders of port/host/vs2p.input on a
4 MiB console (find others: dump RDRAM with --dump-ram and search for a state.log r<i>= position, minus 0xB4).
"""
import argparse, struct

SZ = 0xA8C


def n64(path, nw):
    out, rec = {}, 8 + SZ * nw
    b = open(path, 'rb').read()
    for o in range(0, len(b) - rec + 1, rec):
        _vi, st = struct.unpack_from('<II', b, o)
        out[st] = [b[o + 8 + i * SZ:o + 8 + (i + 1) * SZ] for i in range(nw)]  # the step's last VI wins
    return out


def port(path):
    out, rec = {}, 8 + SZ
    b = open(path, 'rb').read()
    for o in range(0, len(b) - rec + 1, rec):
        st, i = struct.unpack_from('<II', b, o)
        out.setdefault(st, {})[i] = b[o + 8:o + 8 + SZ]
    return out


def same(x, y):
    bs = struct.unpack('<I', struct.pack('>I', x))[0]
    hs = ((x >> 16) | (x << 16)) & 0xFFFFFFFF
    ps = ((x & 0x00FF00FF) << 8 | (x >> 8) & 0x00FF00FF) & 0xFFFFFFFF
    return y in (x, bs, hs, ps) or (x >> 24) == 0x80


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('watch')
    ap.add_argument('riders')
    ap.add_argument('--riders-n', type=int, default=2, help='objects per --watch record')
    ap.add_argument('--from', dest='lo', type=int, default=0)
    ap.add_argument('--to', dest='hi', type=int, default=1 << 30)
    ap.add_argument('--max', type=int, default=20, help='stop after this many differing (step, rider) lines')
    a = ap.parse_args()
    N, P = n64(a.watch, a.riders_n), port(a.riders)
    steps = [s for s in sorted(P) if a.lo <= s <= a.hi and s in N]
    shown = 0
    for st in steps:
        for i in sorted(P[st]):
            if i >= a.riders_n:
                continue
            x = struct.unpack('>%dI' % (SZ // 4), N[st][i])
            y = struct.unpack('>%dI' % (SZ // 4), P[st][i])
            d = [(k * 4, x[k], y[k]) for k in range(len(x)) if not same(x[k], y[k])]
            if d:
                print('%d r%d %d words: %s' % (st, i, len(d), ' '.join('%x:%08x/%08x' % t for t in d[:16])))
                shown += 1
        if shown >= a.max:
            break
    print('rider-diff: %d steps compared (%s..%s), %d differing lines shown' % (
        len(steps), steps[0] if steps else '-', steps[-1] if steps else '-', shown))


if __name__ == '__main__':
    main()
