#!/usr/bin/env python3
"""Metadata-only ELF32-BE edit: delete symbol NAME (and NAME.NON_MATCHING) from .symtab
and grow PRED's size by the deleted symbol's size (folding an all-zero pad symbol into
its predecessor's tail, matching an all-zero SUFFIX_BYTES_FORCE in the build).
Usage: elf-symtab-delete.py <in.o> <out.o> NAME[:PRED] ...
Relocation symbol indices and .symtab sh_info are renumbered. .text is untouched;
verify `objcopy --only-section=.text` and the reloc count are unchanged."""
import struct, sys

src, dst = sys.argv[1], sys.argv[2]
specs = [a.split(':') for a in sys.argv[3:]]
d = bytearray(open(src, 'rb').read())
shoff = struct.unpack_from('>I', d, 0x20)[0]
shentsize, shnum, shstrndx = struct.unpack_from('>HHH', d, 0x2E)
SH = '>IIIIIIIIII'
sh = [list(struct.unpack_from(SH, d, shoff + i * shentsize)) for i in range(shnum)]


def cstr(off):
    return d[off:d.index(b'\0', off)].decode()


names = [cstr(sh[shstrndx][4] + x[0]) for x in sh]
si = names.index('.symtab')
st = sh[si]
strt = sh[st[6]]
n = st[5] // 16
syms = [list(struct.unpack_from('>IIIBBH', d, st[4] + i * 16)) for i in range(n)]
sname = [cstr(strt[4] + s[0]) for s in syms]

dele = set()
for spec in specs:
    nm = spec[0]
    idx = [i for i, s in enumerate(sname) if s in (nm, nm + '.NON_MATCHING')]
    assert idx, nm
    size = syms[idx[0]][2]
    dele.update(idx)
    if len(spec) > 1:
        for j, s in enumerate(sname):
            if s in (spec[1], spec[1] + '.NON_MATCHING'):
                syms[j][2] += size
                print('grow', s, hex(syms[j][2]))
    print('delete', nm, hex(size))

newidx = {}
k = 0
for i in range(n):
    if i in dele:
        continue
    newidx[i] = k
    k += 1
# relocs must not reference deleted symbols
for i, x in enumerate(sh):
    if x[1] in (9, 4) and x[6] == si:  # SHT_REL / SHT_RELA linked to symtab
        ent = 8 if x[1] == 9 else 12
        for r in range(x[5] // ent):
            o = x[4] + r * ent
            info = struct.unpack_from('>I', d, o + 4)[0]
            sym, typ = info >> 8, info & 0xFF
            assert sym not in dele, 'reloc references deleted symbol'
            struct.pack_into('>I', d, o + 4, (newidx[sym] << 8) | typ)
out = b''.join(struct.pack('>IIIBBH', *syms[i]) for i in range(n) if i not in dele)
d[st[4]:st[4] + len(out)] = out
d[st[4] + len(out):st[4] + st[5]] = b'\0' * (st[5] - len(out))
nlocal = st[7]
nlocal -= sum(1 for i in dele if i < st[7])
st[5] = len(out)
st[7] = nlocal
struct.pack_into(SH, d, shoff + si * shentsize, *st)
open(dst, 'wb').write(d)
