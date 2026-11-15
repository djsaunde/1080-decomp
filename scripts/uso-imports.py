#!/usr/bin/env python3
"""Static USO import / reloc resolver: who calls what across 1080's USO modules.

Reads every module's Sym table and Text/Data/RoData reloc sections straight from
baserom.z64 (no emulator), and names both ends from the current build:

  * the reloc SITE   -> the function containing it, in the module's own segment;
  * the reloc TARGET -> kind-1 import (module id = info >> 4, exporter's Sym index =
    top half of word3; docs/USO_FORMAT.md section 6) resolved to the exporter's Sym
    entry and then to a symbol name, or a local Sym (Text / Data / RoData / Bss
    object or section base).

Names come from build/tenshoe.elf + build/tenshoe.map for modules whose Text is
stored uncompressed (bootup = bootup_uso + game_libs, gui, titproc, arcproc, ...),
and from the Yay0 block objects in build/assets / build/src for the compressed ones
(mgrproc, timproc = b1, trkproc = b3, genproc = b5, game, map4_data). So run a
full `make` first. Data / Bss targets are printed as `Data+0xK` / `Bss+0xK`.

Usage (from the project root, e.g. projects/1080-agent-c):
  scripts/uso-imports.py list
      every module: id, name, Info / module / Text ROM offsets, Text size, whether
      function names are available.
  scripts/uso-imports.py sites <module> [--section text|data|rodata] [--imports]
                                         [--type jal|hi|lo|word] [--from MODULE]
      every reloc site: site offset, containing function, reloc type, target.
      --imports keeps only kind-1 imports; --from keeps imports from one module.
  scripts/uso-imports.py imports <module>
      the distinct imported symbols with their site counts and calling functions.
  scripts/uso-imports.py callers <regex> [--type jal]
      every site in every module whose target name matches <regex> (Python re,
      searched), e.g. `callers '^replay_saveGhost$'`.

<module> is a name (`genproc`, `genproc.uso`), a module id (`28`) or the ROM offset
of the module header (`0x5BCA4E`, magic 0x12345678; the Info record is 0x28 earlier).

Example: which modules call replay_saveGhost (docs/1080_REPLAY.md):
  scripts/uso-imports.py callers '^replay_saveGhost$'
  -> timproc  0x...  TimMenu_handleInput_00002A8C  jal  replay_saveGhost
     trkproc  0x...  TrkMenu_handleInput_00002C98  jal  replay_saveGhost

Module layout: the magic word is followed directly by the section records. Code modules
start with a Bss record (type 8, no payload: header word 1 reads 8); n64proc, gamegfx, the
boarderN and the data USOs start with Data (6) or RoData (7) and have no Bss. n64proc's
c segment starts 4 bytes before its Text payload (the Text record's flag word is the
`beq zero,zero` trampoline of n64proc_uso_func_00000000), so its first function shows at
Text-0x4.

Limits: trnproc and viewrec
have Text but no splat code segment, so their sites print as `Text+0xK`. A target
`gl_X+0x4` is a real Sym value inside the build's symbol X (a split boundary), not an
error of this script.

Promoted from agent-c run 495's /tmp xmod.py (run 499).
"""
import argparse
import os
import re
import struct
import subprocess
import sys

import crunch64

MAGIC = 0x12345678
TYPES = {1: 'Sym', 2: 'TextReloc', 3: 'DataReloc', 4: 'RoDataReloc', 5: 'Text',
         6: 'Data', 7: 'RoData', 8: 'Bss', 9: 'EntrySym', 11: 'End'}
SEC_OF_TAG = {5: 'Text', 6: 'Data', 7: 'RoData', 8: 'Bss'}
RELTYPE = {1: 'jal', 2: 'lo', 3: 'hi', 4: 'word'}

# Compressed (Yay0) code: the built Text image and the objects whose .text makes it,
# each with its base offset in the module Text ('+prev' = after the previous one).
COMPRESSED = [
    ('build/assets/mgrproc_uso_block1_yay0.text.bin', [('build/assets/mgrproc_uso_block1_yay0.lnk.o', 0)]),
    ('build/assets/timproc_uso_block3_yay0.text.bin', [('build/assets/timproc_uso_block3_yay0.lnk.o', 0)]),
    ('build/assets/game_uso_block1_yay0.text.bin', [('build/assets/game_uso_block1_yay0.lnk.o', 0)]),
    ('build/assets/timproc_uso_block1_yay0.text.bin',
     [('build/src/timproc_uso_b1/timproc_uso_b1_o0_0.c.o', 0),
      ('build/src/timproc_uso_b1/timproc_uso_b1.c.o', '+prev')]),
    ('build/assets/timproc_uso_block5_yay0.text.bin', [('build/src/timproc_uso_b5/timproc_uso_b5.c.o', 0)]),
    ('build/assets/map4_data_uso_block2_yay0.text.bin', [('build/src/map4_data_uso_b2/map4_data_uso_b2.c.o', 0)]),
]


def u32(b, o):
    return struct.unpack_from('>I', b, o)[0]


class Module:
    def __init__(self, rom, info, mid, name):
        self.info, self.id, self.name = info, mid, name
        self.hdr = info + 0x28
        self.short = name[:-4] if name.endswith('.uso') else name
        self.secs, self.secrom, self.compressed = {}, {}, set()
        self.funcs = []          # sorted [(text_off, size, name)]
        self.bss_size = 0
        self._parse(rom)

    def _parse(self, rom):
        assert u32(rom, self.hdr) == MAGIC, hex(self.hdr)
        # The magic is followed directly by section records. Code modules start with a Bss
        # record (type 8: no payload), so word 1 reads 8; n64proc, the boarderN / data USOs
        # start with Data (6) or RoData (7) (run 510).
        o = self.hdr + 4
        while o < len(rom) - 12:
            # section headers may sit 1-3 bytes off after a Yay0 payload
            for k in range(4):
                t, fl = u32(rom, o + k), u32(rom, o + k + 8)
                if t in TYPES and (fl >> 16) in (0x1000, 0x1001, 0x0800, 0x2000, 0x2001):
                    o += k
                    break
            else:
                break
            t, size, flag = u32(rom, o), u32(rom, o + 4), u32(rom, o + 8)
            d0 = o + 12
            if t == 8:                       # Bss: size only, no payload
                data, nxt = b'', d0
                self.bss_size = size
            elif flag & 0x10000:
                data = crunch64.yay0.decompress(rom[d0:d0 + size + 0x40000])
                nxt = d0 + len(crunch64.yay0.compress(data))
                self.compressed.add(TYPES[t])
            else:
                data, nxt = rom[d0:d0 + size], d0 + size
            if TYPES[t] not in self.secs:
                self.secs[TYPES[t]] = data
                self.secrom[TYPES[t]] = d0
            o = nxt
            if t in (9, 11) and 'Sym' in self.secs:
                break
        s = self.secs.get('Sym', b'')
        self.syms = [struct.unpack_from('>III', s, i * 12) for i in range(len(s) // 12)]

    def relocs(self, sec='Text'):
        r = self.secs.get(sec + 'Reloc', b'')
        return [struct.unpack_from('>III', r, i * 12) for i in range(len(r) // 12)]

    def func_at(self, off):
        lo, hi = 0, len(self.funcs)
        while lo < hi:
            mid = (lo + hi) // 2
            if self.funcs[mid][0] <= off:
                lo = mid + 1
            else:
                hi = mid
        if lo and off < self.funcs[lo - 1][0] + max(self.funcs[lo - 1][1], 4):
            return self.funcs[lo - 1][2]
        return None

    def name_text(self, off):
        f = self.func_at(off)
        if f is None:
            return 'Text+0x%X' % off
        start = [x for x in self.funcs if x[2] == f][0][0]
        return f if start == off else '%s+0x%X' % (f, off - start)


def find_modules(rom):
    mods = []
    for m in re.finditer(re.escape(struct.pack('>I', MAGIC)), rom):
        o = m.start()
        nm = rom[o + 12:o + 44].split(b'\0')[0]
        if nm.endswith(b'.uso') and u32(rom, o + 0x28) == MAGIC:
            mods.append(Module(rom, o, struct.unpack_from('>i', rom, o + 4)[0], nm.decode()))
    return mods


def readelf_funcs(path):
    out = subprocess.run(['readelf', '-sW', path], capture_output=True, text=True).stdout
    secs = subprocess.run(['readelf', '-SW', path], capture_output=True, text=True).stdout
    text_ndx = {m.group(1) for m in re.finditer(r'\[\s*(\d+)\]\s+\.text\s', secs)}
    res = []
    for line in out.splitlines():
        r = line.split()
        if len(r) > 7 and r[3] == 'FUNC' and r[6] in text_ndx:
            res.append((int(r[1], 16), int(r[2]), r[7]))
    return res


def text_size(path):
    secs = subprocess.run(['readelf', '-SW', path], capture_output=True, text=True).stdout
    m = re.search(r'\]\s+\.text\s+PROGBITS\s+\S+\s+\S+\s+(\S+)', secs)
    return int(m.group(1), 16) if m else 0


def attach_names(mods, root):
    elf, mapf = os.path.join(root, 'build/tenshoe.elf'), os.path.join(root, 'build/tenshoe.map')
    if not (os.path.exists(elf) and os.path.exists(mapf)):
        print('warning: no build/tenshoe.elf / .map: run make first; names unavailable', file=sys.stderr)
        return
    # map sections: name -> (vram, size, rom)
    msec = {}
    lines = open(mapf).read().splitlines()
    for i, l in enumerate(lines):
        m = re.match(r'^\.(\S+)\s*$', l)
        if m and i + 1 < len(lines):
            l = '.' + m.group(1) + ' ' + lines[i + 1].strip()
        m = re.match(r'^\.(\S+)\s+0x([0-9a-f]+)\s+0x([0-9a-f]+)\s+load address 0x([0-9a-f]+)', l)
        if m:
            msec[m.group(1)] = tuple(int(m.group(k), 16) for k in (2, 3, 4))
    hdrs = subprocess.run(['readelf', '-SW', elf], capture_output=True, text=True).stdout
    ndx2sec = {m.group(1): m.group(2) for m in re.finditer(r'\[\s*(\d+)\]\s+\.(\S+)', hdrs)}
    syms = subprocess.run(['readelf', '-sW', elf], capture_output=True, text=True).stdout
    elffuncs = []
    for line in syms.splitlines():
        r = line.split()
        if len(r) > 7 and r[3] == 'FUNC' and r[6] in ndx2sec and ndx2sec[r[6]] in msec:
            vram, size, rom = msec[ndx2sec[r[6]]]
            elffuncs.append((rom + int(r[1], 16) - vram, int(r[2]), r[7]))
    texts = {}
    for tb, objs in COMPRESSED:
        p = os.path.join(root, tb)
        if os.path.exists(p):
            texts[open(p, 'rb').read()] = objs
    for md in mods:
        if 'Text' not in md.secs:
            continue
        t = md.secs['Text']
        if 'Text' in md.compressed:
            objs = next((v for k, v in texts.items() if k[:len(t)] == t or t[:len(k)] == k), None)
            if objs is None:
                continue
            base = 0
            for path, b in objs:
                path = os.path.join(root, path)
                if b != '+prev':
                    base = b
                for v, sz, nm in readelf_funcs(path):
                    md.funcs.append((base + v, sz, nm))
                base += text_size(path)
        else:
            t0 = md.secrom['Text']
            md.funcs = [(r - t0, sz, nm) for r, sz, nm in elffuncs if t0 <= r < t0 + len(t)]
        fs = sorted(set(md.funcs))
        # size-0 symbols (GLOBAL_ASM / spliced bodies) run to the next function
        md.funcs = [(v, sz or ((fs[i + 1][0] if i + 1 < len(fs) else len(t)) - v), nm)
                    for i, (v, sz, nm) in enumerate(fs)]


class Resolver:
    def __init__(self, mods):
        self.mods = mods
        self.byid = {m.id: m for m in mods}

    def local(self, md, idx):
        if idx >= len(md.syms):
            return '%s:sym%d?' % (md.short, idx)
        info, v, _ = md.syms[idx]
        info >>= 16
        kind, sec = info & 7, SEC_OF_TAG.get((info >> 4) & 0xF)
        if kind in (2, 4) and sec == 'Text':
            return md.name_text(v)
        if kind in (2, 4) and sec:
            return '%s+0x%X' % (sec, v) if kind == 2 else '%s(base)' % sec
        return 'sym%d(tag%X,v%X)' % (idx, info, v)

    def target(self, md, idx):
        """-> (exporting module or None, name)"""
        if idx >= len(md.syms):
            return None, 'sym%d?' % idx
        info, v, w3 = md.syms[idx]
        info >>= 16
        if info & 7 == 1:
            ex = self.byid.get(info >> 4)
            if ex is None:
                return None, 'mod%d:sym%d' % (info >> 4, w3 >> 16)
            return ex, self.local(ex, w3 >> 16)
        return md, self.local(md, idx)


def pick(mods, key):
    for m in mods:
        if key in (m.name, m.short) or key == str(m.id):
            return m
    try:
        o = int(key, 0)
    except ValueError:
        o = None
    for m in mods:
        if o in (m.hdr, m.info):
            return m
    sys.exit('unknown module %r (try `list`)' % key)


def site_rows(res, md, sec='Text', imports=False, rtype=None, frm=None):
    for f1, off, _ in md.relocs(sec):
        k, idx = f1 & 7, f1 >> 4
        if rtype and RELTYPE.get(k) != rtype:
            continue
        ex, name = res.target(md, idx)
        imp = ex is not md
        if imports and not imp:
            continue
        if frm is not None and ex is not frm:
            continue
        where = md.func_at(off) if sec == 'Text' else None
        yield off, where or '%s+0x%X' % (sec, off), RELTYPE.get(k, str(k)), \
            ('%s:' % (ex.short if ex else '?') if imp else '') + name


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--root', default='.', help='project root (has baserom.z64 and build/)')
    sub = ap.add_subparsers(dest='cmd', required=True)
    sub.add_parser('list')
    s = sub.add_parser('sites')
    s.add_argument('module')
    s.add_argument('--section', default='text', choices=['text', 'data', 'rodata'])
    s.add_argument('--imports', action='store_true')
    s.add_argument('--type', choices=list(RELTYPE.values()))
    s.add_argument('--from', dest='frm')
    s = sub.add_parser('imports')
    s.add_argument('module')
    s = sub.add_parser('callers')
    s.add_argument('regex')
    s.add_argument('--type', choices=list(RELTYPE.values()))
    a = ap.parse_args()

    rom = open(os.path.join(a.root, 'baserom.z64'), 'rb').read()
    mods = find_modules(rom)
    attach_names(mods, a.root)
    res = Resolver(mods)

    if a.cmd == 'list':
        print('%3s  %-16s %9s %9s %9s %7s  %s' % ('id', 'name', 'info', 'module', 'text', 'size', 'names'))
        for m in sorted(mods, key=lambda m: m.id):
            t = m.secs.get('Text', b'')
            print('%3d  %-16s %9s %9s %9s %7s  %s' % (
                m.id, m.name, hex(m.info), hex(m.hdr),
                hex(m.secrom['Text']) + ('z' if 'Text' in m.compressed else '') if t else '-',
                hex(len(t)) if t else '-', '%d funcs' % len(m.funcs) if m.funcs else '-'))
    elif a.cmd == 'sites':
        md = pick(mods, a.module)
        frm = pick(mods, a.frm) if a.frm else None
        for off, where, k, tgt in site_rows(res, md, a.section.capitalize().replace('Rodata', 'RoData'),
                                            a.imports, a.type, frm):
            print('0x%05X  %-40s %-4s %s' % (off, where, k, tgt))
    elif a.cmd == 'imports':
        md = pick(mods, a.module)
        agg = {}
        for off, where, k, tgt in site_rows(res, md, imports=True):
            if k == 'lo':
                continue
            agg.setdefault(tgt, []).append(where)
        for tgt in sorted(agg):
            fs = sorted(set(agg[tgt]))
            print('%-50s %3d  %s' % (tgt, len(agg[tgt]), ' '.join(fs[:6]) + (' ...' if len(fs) > 6 else '')))
    elif a.cmd == 'callers':
        rx = re.compile(a.regex)
        for md in sorted(mods, key=lambda m: m.id):
            for off, where, k, tgt in site_rows(res, md, rtype=a.type):
                if k != 'lo' and rx.search(tgt.split(':', 1)[-1]):
                    print('%-10s 0x%05X  %-40s %-4s %s' % (md.short, off, where, k, tgt))


if __name__ == '__main__':
    main()
