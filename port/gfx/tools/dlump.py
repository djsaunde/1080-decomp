#!/usr/bin/env python3
"""Read and check DLMP display-list dumps (port/gfx/dlump.h, PC port plan 19).

    dlump.py FILE.dlmp ...            summary + F3DEX command histogram per file
    dlump.py --check FILE.dlmp ...    also re-walk every list from the dump's own
                                      memory image and verify that each range a
                                      command references is in the dump (exit 1 if not)
    dlump.py --list FILE.dlmp         print the walked command stream
    dlump.py --total DIR/*.dlmp       one histogram summed over all files

The re-walk is an independent implementation of the dumper's rules (G_DL push /
branch, G_ENDDL, SEGMENT, the F3DEX 1 G_VTX length, LOADBLOCK / LOADTILE /
LOADTLUT sizes), so --check cross-validates the C writer.
"""
import argparse
import collections
import struct
import sys

KINDS = {0: "END", 1: "TASK", 2: "DL", 3: "VTX", 4: "MTX", 5: "MOVEMEM", 6: "TEXTURE", 7: "TLUT",
         8: "IMAGE", 9: "ERROR"}

# F3DEX 1.x (GBI 1) opcode names; 0xB5 is G_LINE3D in 1080's usage (plan 11.1, 14.3)
OPS = {
    0x00: "G_SPNOOP", 0x01: "G_MTX", 0x03: "G_MOVEMEM", 0x04: "G_VTX", 0x06: "G_DL", 0x07: "G_DMA_IO?",
    0x09: "G_SPRITE2D_BASE", 0xAF: "G_LOAD_UCODE", 0xB0: "G_BRANCH_Z", 0xB1: "G_TRI2", 0xB2: "G_MODIFYVTX",
    0xB3: "G_RDPHALF_2", 0xB4: "G_RDPHALF_1", 0xB5: "G_LINE3D", 0xB6: "G_CLEARGEOMETRYMODE",
    0xB7: "G_SETGEOMETRYMODE", 0xB8: "G_ENDDL", 0xB9: "G_SETOTHERMODE_L", 0xBA: "G_SETOTHERMODE_H",
    0xBB: "G_TEXTURE", 0xBC: "G_MOVEWORD", 0xBD: "G_POPMTX", 0xBE: "G_CULLDL", 0xBF: "G_TRI1",
    0xC0: "G_NOOP", 0xE4: "G_TEXRECT", 0xE5: "G_TEXRECTFLIP", 0xE6: "G_RDPLOADSYNC", 0xE7: "G_RDPPIPESYNC",
    0xE8: "G_RDPTILESYNC", 0xE9: "G_RDPFULLSYNC", 0xEA: "G_SETKEYGB", 0xEB: "G_SETKEYR",
    0xEC: "G_SETCONVERT", 0xED: "G_SETSCISSOR", 0xEE: "G_SETPRIMDEPTH", 0xEF: "G_RDPSETOTHERMODE",
    0xF0: "G_LOADTLUT", 0xF2: "G_SETTILESIZE", 0xF3: "G_LOADBLOCK", 0xF4: "G_LOADTILE", 0xF5: "G_SETTILE",
    0xF6: "G_FILLRECT", 0xF7: "G_SETFILLCOLOR", 0xF8: "G_SETFOGCOLOR", 0xF9: "G_SETBLENDCOLOR",
    0xFA: "G_SETPRIMCOLOR", 0xFB: "G_SETENVCOLOR", 0xFC: "G_SETCOMBINE", 0xFD: "G_SETTIMG",
    0xFE: "G_SETZIMG", 0xFF: "G_SETCIMG",
}
# The 39 opcodes plan 11.1 says the game emits; anything else is reported.
CENSUS = {0x01, 0x03, 0x04, 0x06, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBE, 0xBF,
          0xC0, 0xE4, 0xE6, 0xE7, 0xE8, 0xE9, 0xED, 0xEE, 0xF0, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8,
          0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF}
DL_STACK = 10
MAX_CMDS = 1 << 22


class Dump:
    def __init__(self, path):
        self.path = path
        data = open(path, "rb").read()
        if len(data) < 64 or data[:8] != b"1080DLMP":
            raise ValueError("not a DLMP file")
        bom_le = struct.unpack_from("<I", data, 16)[0]
        self.e = "<" if bom_le == 0x01020304 else ">"
        if struct.unpack_from(self.e + "I", data, 16)[0] != 0x01020304:
            raise ValueError("bad byte-order mark")
        f = struct.unpack_from(self.e + "15I", data, 8)
        (self.version, hsize, _bom, self.ptr_size, self.seq, self.root, self.root_size, self.nchunks,
         self.cmd_count, self.error_count, self.flags, self.data_bytes) = f[:12]
        if self.version != 1 or hsize != 64:
            raise ValueError("unsupported version %d / header %d" % (self.version, hsize))
        self.chunks = []
        off = hsize
        for i in range(self.nchunks + 1):
            if off + 16 > len(data):
                raise ValueError("chunk %d header past end of file" % i)
            kind, addr, size, aux = struct.unpack_from(self.e + "4I", data, off)
            off += 16
            if i == self.nchunks:
                if kind != 0:
                    raise ValueError("missing END chunk")
                break
            if kind == 0 or kind not in KINDS:
                raise ValueError("chunk %d: bad kind %d" % (i, kind))
            if off + size > len(data):
                raise ValueError("chunk %d data past end of file" % i)
            self.chunks.append((kind, addr, size, aux, data[off:off + size]))
            off += (size + 3) & ~3
        if off != len(data):
            raise ValueError("%d trailing bytes" % (len(data) - off))
        got = sum(c[2] for c in self.chunks if c[0] != 9)
        if got != self.data_bytes:
            raise ValueError("dataBytes %d != sum of chunks %d" % (self.data_bytes, got))
        self.mem = sorted((a, a + s, d) for k, a, s, x, d in self.chunks if k not in (8, 9) and s)

    def covered(self, addr, size):
        """True if [addr, addr+size) is inside one chunk."""
        for a, e, _ in self.mem:
            if a <= addr and addr + size <= e:
                return True
        return False

    def read(self, addr, size):
        for a, e, d in self.mem:
            if a <= addr and addr + size <= e:
                return d[addr - a:addr - a + size]
        return None

    def walk(self):
        """Yield (pc, depth, w0, w1); records missing ranges in self.missing."""
        self.missing = []
        seg = [0] * 16
        timg_w0 = timg = 0
        stack = []
        pc = self.root
        n = 0

        def res(a):
            b = seg[(a >> 24) & 15]
            return (b + (a & 0xFFFFFF)) & 0xFFFFFFFF if b else a

        def need(pc, what, addr, size):
            if size and not self.covered(addr, size):
                self.missing.append((pc, what, addr, size))

        def tbytes(n, siz):
            return (n + 1) // 2 if siz == 0 else n << (siz - 1)

        while n < MAX_CMDS:
            raw = self.read(pc, 8)
            if raw is None:
                self.missing.append((pc, "DL", pc, 8))
                return
            w0, w1 = struct.unpack(self.e + "2I", raw)
            n += 1
            yield pc, len(stack), w0, w1
            op = w0 >> 24
            pc += 8
            if op == 0x06:
                if (w0 >> 16) & 0xFF == 0:
                    if len(stack) >= DL_STACK:
                        return
                    stack.append(pc)
                pc = res(w1)
            elif op == 0xB8:
                if not stack:
                    return
                pc = stack.pop()
            elif op == 0xBC:
                if w0 & 0xFF == 6:
                    seg[((w0 >> 8) & 0xFFFF) // 4 & 15] = w1
            elif op == 0x01:
                need(pc - 8, "MTX", res(w1), (w0 & 0xFFFF) or 0x40)
            elif op == 0x03:
                need(pc - 8, "MOVEMEM", res(w1), w0 & 0xFFFF)
            elif op == 0x04:
                need(pc - 8, "VTX", res(w1), (w0 & 0x3FF) + 1)
            elif op == 0xFD:
                timg_w0, timg = w0, res(w1)
            elif op == 0xF3:
                siz, width = (timg_w0 >> 19) & 3, (timg_w0 & 0xFFF) + 1
                uls, ult, lrs = (w0 >> 12) & 0xFFF, w0 & 0xFFF, (w1 >> 12) & 0xFFF
                if lrs >= uls:
                    need(pc - 8, "TEXTURE", timg + tbytes(ult * width + uls, siz), tbytes(lrs - uls + 1, siz))
            elif op == 0xF4:
                siz, width = (timg_w0 >> 19) & 3, (timg_w0 & 0xFFF) + 1
                uls, ult = ((w0 >> 12) & 0xFFF) >> 2, (w0 & 0xFFF) >> 2
                lrs, lrt = ((w1 >> 12) & 0xFFF) >> 2, (w1 & 0xFFF) >> 2
                if lrs >= uls and lrt >= ult:
                    s = tbytes(ult * width + uls, siz)
                    need(pc - 8, "TEXTURE", timg + s, tbytes(lrt * width + lrs + 1, siz) - s)
            elif op == 0xF0:
                uls, lrs = (w0 >> 14) & 0x3FF, (w1 >> 14) & 0x3FF
                if lrs >= uls:
                    need(pc - 8, "TLUT", timg + uls * 2, (lrs - uls + 1) * 2)


def summary(d, hist):
    kinds = collections.Counter(KINDS[c[0]] for c in d.chunks)
    print("%s: seq %d root 0x%08x (%d bytes) ptr%d, %d chunks / %d data bytes, %d commands, %d errors%s" % (
        d.path, d.seq, d.root, d.root_size, d.ptr_size * 8, d.nchunks, d.data_bytes, d.cmd_count,
        d.error_count, " TRUNCATED" if d.flags & 1 else ""))
    print("  chunks: " + ", ".join("%s %d" % kv for kv in sorted(kinds.items())))
    for k, a, s, x, data in d.chunks:
        if k == 9:
            print("  error @0x%08x: %s" % (a, data.rstrip(b"\0").decode("latin-1")))
    print("  %-22s %8s" % ("command", "count"))
    for op, cnt in sorted(hist.items(), key=lambda kv: -kv[1]):
        flag = "" if op in CENSUS else "   <- not in the plan 11.1 census"
        print("  %02X %-19s %8d%s" % (op, OPS.get(op, "?"), cnt, flag))


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("files", nargs="+")
    ap.add_argument("--check", action="store_true")
    ap.add_argument("--list", action="store_true")
    ap.add_argument("--total", action="store_true")
    a = ap.parse_args()
    bad = 0
    total = collections.Counter()
    for path in a.files:
        try:
            d = Dump(path)
        except (ValueError, OSError, struct.error) as ex:
            print("%s: INVALID: %s" % (path, ex))
            bad += 1
            continue
        hist = collections.Counter()
        cmds = 0
        for pc, depth, w0, w1 in d.walk():
            hist[w0 >> 24] += 1
            cmds += 1
            if a.list:
                print("%08x %s%08x %08x  %s" % (pc, "  " * depth, w0, w1, OPS.get(w0 >> 24, "?")))
        total.update(hist)
        if not a.total:
            summary(d, hist)
        if a.check:
            problems = []
            if cmds != d.cmd_count:
                problems.append("re-walk saw %d commands, header says %d" % (cmds, d.cmd_count))
            for pc, what, addr, size in d.missing:
                problems.append("cmd @0x%08x: %s 0x%08x + 0x%x not in the dump" % (pc, what, addr, size))
            if problems and d.error_count == 0:
                bad += 1
                for p in problems[:20]:
                    print("  CHECK: " + p)
            elif not problems:
                print("  check: ok (every referenced range is present)")
            else:
                print("  check: %d gaps, all explained by the dump's own %d errors" % (len(problems),
                                                                                       d.error_count))
    if a.total:
        print("total over %d files:" % len(a.files))
        for op, cnt in sorted(total.items(), key=lambda kv: -kv[1]):
            print("  %02X %-19s %8d" % (op, OPS.get(op, "?"), cnt))
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main())
