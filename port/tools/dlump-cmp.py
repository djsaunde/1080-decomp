#!/usr/bin/env python3
"""Compare two PORT_GFX_DUMP directories task by task, independent of host addresses (plan 21.9).

    port/tools/dlump-cmp.py DIR_A DIR_B [-v] [--unordered]

The i386 and the x86-64 builds lay the game out at different host addresses, so their DLMP files differ
in every address operand even when the frames are the same. This compares what the renderer is given,
minus where it lives: per task (matched by steps.txt order), the sequence of chunk kinds, every data
chunk's bytes (vertices, matrices, MOVEMEM records: lights / viewports / lookats, textures, TLUTs), the
display-list words with the address operands masked (G_MTX, G_MOVEMEM, G_VTX, G_DL, G_SETTIMG,
G_SETZIMG, G_SETCIMG w1, G_RDPHALF_1 = a G_BRANCH_Z target) and the chunk aux words (G_SETTIMG w0 /
counts). TASK chunks (OSTask_t: pointers) and the framebuffer records' addresses are skipped.
A float passed the wrong way across a 64-bit call (plan 21.8's abi-scan --x64 list) shows up here as a
matrix / vertex / light byte difference with state.log still equal. Exit 0 = identical, 1 = differences.
--unordered (the Windows build against Linux, plan 27): a DLMP lists its chunks in address order, and the
Windows executable's image lies below its heap where Linux's brk heap lies above the image, so each task's
chunks are compared as a multiset of their masked contents instead of in sequence.
"""
import os
import struct
import sys

ADDR_W1 = {0x01, 0x03, 0x04, 0x06, 0xFD, 0xFE, 0xFF, 0xB4}
KINDS = {1: 'TASK', 2: 'DL', 3: 'VTX', 4: 'MTX', 5: 'MOVEMEM', 6: 'TEXTURE', 7: 'TLUT', 8: 'IMAGE', 9: 'ERROR',
         10: 'FB_READ', 11: 'FB_DEPTH', 12: 'FB_WRITE'}


def chunks(path):
    b = open(path, 'rb').read()
    if b[:8] != b'1080DLMP':
        raise ValueError('%s: not a DLMP' % path)
    hsize = struct.unpack_from('<I', b, 12)[0]
    off, out = hsize, []
    while off + 16 <= len(b):
        kind, addr, size, aux = struct.unpack_from('<4I', b, off)
        off += 16
        if kind == 0:
            break
        out.append((kind, addr, aux, b[off:off + size]))
        off += (size + 3) & ~3
    return out


def norm(c):
    kind, addr, aux, data = c
    if kind == 1:
        return (kind, None, None)
    if kind == 2:
        w = list(struct.unpack('<%dI' % (len(data) // 4), data[:len(data) // 4 * 4]))
        for i in range(0, len(w) - 1, 2):
            if (w[i] >> 24) in ADDR_W1:
                w[i + 1] = 0
        return (kind, aux, tuple(w))
    if kind in (8, 10, 12):
        return (kind, aux if kind == 8 else None, None if kind != 12 else data[8:])
    if kind == 11:
        return (kind, None, data)
    return (kind, aux, data)


def steps(d):
    p = os.path.join(d, 'steps.txt')
    return [l.split() for l in open(p)] if os.path.exists(p) else [[f[4:10], '?'] for f in sorted(os.listdir(d))
                                                                  if f.startswith('gfx_')]


def main():
    a, b = sys.argv[1], sys.argv[2]
    verbose = '-v' in sys.argv
    unordered = '--unordered' in sys.argv
    sa, sb = steps(a), steps(b)
    bad, n = [], 0
    if [s for _, s in sa] != [s for _, s in sb]:
        print('task steps differ: %s / %s' % ([s for _, s in sa][:12], [s for _, s in sb][:12]))
        return 1
    for (ta, step), (tb, _) in zip(sa, sb):
        ca, cb = chunks(os.path.join(a, 'gfx_%s.dlmp' % ta)), chunks(os.path.join(b, 'gfx_%s.dlmp' % tb))
        n += 1
        why = None
        if len(ca) != len(cb):
            why = '%d / %d chunks' % (len(ca), len(cb))
        elif unordered:
            ka, kb = sorted(repr(norm(x)) for x in ca), sorted(repr(norm(y)) for y in cb)
            if ka != kb:
                why = '%d chunks differ (unordered)' % sum(1 for x, y in zip(ka, kb) if x != y)
        else:
            for i, (x, y) in enumerate(zip(ca, cb)):
                if norm(x) != norm(y):
                    why = 'chunk %d %s (A 0x%08x, B 0x%08x, aux %08x / %08x, %d / %d bytes)' % (
                        i, KINDS.get(x[0], x[0]), x[1], y[1], x[2], y[2], len(x[3]), len(y[3]))
                    if verbose and x[0] == y[0] and len(x[3]) == len(y[3]):
                        d = [j for j in range(len(x[3])) if x[3][j] != y[3][j]]
                        why += ' first byte %d of %d differing' % (d[0], len(d)) if d else ''
                    break
        if why:
            bad.append('step %s task %s/%s: %s' % (step, ta, tb, why))
    print('%d/%d tasks identical (address operands masked)%s' % (n - len(bad), n,
          '' if not bad else '; ' + '; '.join(bad[:4])))
    return 1 if bad or n == 0 else 0


if __name__ == '__main__':
    sys.exit(main())
