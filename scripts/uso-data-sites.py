#!/usr/bin/env python3
"""bootup.uso Text (bootup_uso + game_libs) data-site census, resolver and accessor-macro checker.

Every game_libs data reference is spelled off a zero placeholder (`(char *)&D_00000000 + K`,
or a D_00000000_xxx / gl_data_00000000 alias), because the decomp links the USO at VRAM 0 and
the ROM holds pre-relocation words. What K means is only in bootup.uso's TextReloc table: the
reloc at that instruction names a Sym entry, and the Sym's tag byte says whether it is a
SECTION BASE (K = section offset) or a named OBJECT (K = field offset inside it).
See docs/READABILITY_PLAN.md 9.1 and docs/USO_FORMAT.md 6-7.

This tool reads the ROM (bootup.uso Sym / TextReloc / DataReloc / RoDataReloc), the linker map
(function placement, as Text section offsets: bootup_uso = VRAM, game_libs = VRAM + 0x1466C) and the built game_libs .o files
(which placeholder symbol each site uses), and resolves every HI16/LO16 site to
(section, Sym, object, offset-in-object). K is computed exactly the way uso_relocate pairs
relocs: a HI16 remembers its half, each following LO16 folds hi<<16 + sext(lo).

Subcommands:
  sites [FUNC ...]   per-function site list (all functions if none given)
  calls [--to] FUNC..
                     resolved jal targets from the TextReloc R_MIPS_26 entries (what FUNC calls,
                     or with --to who calls FUNC inside bootup.uso); names the placeholder calls
  census [-n N] [--direct]
                     objects ranked by LO16 site count (+ functions, distinct offsets, size
                     inferred from the next named object, DataReloc words inside it: N/MT =
                     N pointers of which M to Text, i.e. a vtable). "[via base]" rows are
                     section-base sites that fall inside a named object's span (same-TU or
                     string data after it); --direct keeps only object-relocated sites.
  object SYM|SEC:OFF which functions touch one object (and at which offsets)
  check              every placeholder accessor macro (`#define NAME ... &D_00000000 ...`)
                     in include/ and src/game_libs: resolve the Sym it claims (comment
                     `Sym N`, or GL_DATA/GL_RODATA/GL_BSS(K) = section base) and verify each
                     function that uses it has a site against that Sym. Exit 1 on a mismatch.
  convert NAME FILE.. [--exclude RE] [--only RE] [--bare] [--write]
                     respell the plain-D_00000000 sites of one header object alias
                     (`#define gX D_00000000 /* ... Sym N ... */`) where the relocs prove
                     the site is that object (see cmd_convert); adds the gl_data.h include
                     (relative path in NCC .cpp donors, which build without -I include)
  spell-base FILE.. [--exclude RE] [--only RE] [--write]
  spell-base --owner-report [OUT] [FILE..]
                     per owner area (OWNERS table: agent-b/c/h, unassigned), the exact
                     `spell-base` / `convert NAME` commands, one per file with --only naming that
                     owner's functions, that respell every provable plain site (dry-run plans of
                     both subcommands; `convert ... --bare` when bare value uses are included)
                     respell plain section-base sites (`(char *)&D_00000000 + K` whose relocs
                     all name ONE section-base Sym) as GL_DATA_STR(K, "text") /
                     GL_RODATA_STR(K, "text") when K starts a string in that section, else
                     GL_DATA(K) / GL_RODATA(K) / GL_BSS(K). `check` verifies every direct
                     GL_*(K) use (a base site with that section and K in the function) and
                     every *_STR text against the ROM bytes.
  selftest           invariants on the tables (counts from USO_FORMAT.md, the DMA prototype),
                     then the game.uso ones.

game.uso (agent-g run 538): `--uso game` before the subcommand runs sites / calls / census /
object on game.uso instead (inner header 0x7CC900; its Text = src/game_uso, read from the
built block object GAME_TEXT_O, whose .text offsets ARE Text section offsets). game_uso sites
are spelled `(char *)&game_uso_D_<addr> + K` (one zero symbol per address); `check` also
verifies the named game.uso section offsets in include/game/game_uso_data.h (check_game:
`kName 0xK /* Data+0xK: PMF -> FUNC */`, the target read from the DataReloc), and
`--uso game check` runs only that. Two game.uso subcommands:
  pmf-table START END [--sect Data|RoData]
                     the #define lines naming a table of 8-byte PMF records by their target
  const-table [--write]
                     the #define lines naming the strings ("Parms" group names, class names, assert
                     texts), 3-letter parm tags and float / double constants the game_uso sites reach
                     by a literal K, each with a value claim (`"text"`, `f32 V`, `f64 V`) that `check`
                     verifies against the section bytes; --write replaces the generated block of
                     include/game/game_uso_data.h (agent-g run 543)
  spell FILE.. [-v] [--write]
                     respell the literal K of each game_uso site (`&game_uso_D_x + K`,
                     `AT(T, &game_uso_D_x, K)`, `&D_00000000 + K`) with the header constant
                     whose section the relocs prove it is (adds the header include)

Mode USOs (agent-g run 589): `--uso titproc|eddproc|mgrproc|arcproc|h2hproc|timproc|trkproc|genproc`
(and since run 617 the new modules trnproc|viewrec|nameent|balloon|heli|fire|checkpoint|snoweffects)
runs the same game.uso machinery (sites / calls / census / object / pmf-table / const-table / spell /
check) on that module (MODULES: timproc / trkproc / genproc are src/timproc_uso_b1 / b3 / b5). Each
module's named section offsets go in its own header (include/game/<src dir>_data.h, created by
`const-table --write`); the bootup `check` (gate step 5) also checks every such header that exists.

Run from the project root. Needs pyelftools + crunch64 (the repo .venv); any other python re-execs
itself under /home/danjs/code/decomp/.venv/bin/python, so `python3 scripts/uso-data-sites.py` works too:
  /home/danjs/code/decomp/.venv/bin/python scripts/uso-data-sites.py census -n 40
Needs a built tree (build/tenshoe.map + build/src/game_libs/*.o). A missing or stale one (map older
than any src/game_libs or src/bootup_uso file) is refreshed with an incremental
`make RUN_CC_CHECK=0 -j8` first; `--no-build` (before the subcommand) only warns. The readability
gate leaves a full build behind, so `check` right after the gate needs no make and no --keep-build.
"""
import argparse, bisect, collections, glob, os, re, struct, sys

# pyelftools + crunch64 live in the repo .venv: re-exec there when run with another python
# (the readability gate's build-venv, a bare `python3`).
_VENV_PY = '/home/danjs/code/decomp/.venv/bin/python'
try:
    import crunch64, elftools  # noqa: F401,E401
except ImportError:
    if os.path.exists(_VENV_PY) and not os.environ.get('USO_DATA_SITES_REEXEC') and __name__ == '__main__':
        os.environ['USO_DATA_SITES_REEXEC'] = '1'
        os.execv(_VENV_PY, [_VENV_PY] + sys.argv)

ROM = 'baserom.z64'
MAP = 'build/tenshoe.map'
INNER_HDR = 0xD9FE28          # bootup.uso inner module header (magic 0x12345678)
TEXT_DELTA = 0x1466C          # game_libs VRAM -> bootup.uso Text section offset
SECT_NAME = {5: 'Text', 6: 'Data', 7: 'RoData', 8: 'Bss'}
SECT_ABBR = {5: 'T', 6: 'D', 7: 'R', 8: 'B'}
BASE_MACRO = {'GL_DATA': 6, 'GL_RODATA': 7, 'GL_BSS': 8, 'GL_TEXT': 5}
# string-documenting section-base spellings: GL_DATA_STR(K, "text") == GL_DATA(K)
STR_MACRO = {'GL_DATA_STR': 6, 'GL_RODATA_STR': 7}
# game.uso: inner module header, and the block object whose .text is its Text section (the
# Makefile's game_uso_block1_yay0 recipe: game_uso.c.o after drop-text-word, before the clip)
GAME_INNER_HDR = 0x7CC900
GAME_TEXT_O = 'build/assets/game_uso_block1_yay0.lnk.o'
GAME_SRC = 'src/game_uso'
# Mode USOs (agent-g run 589): `--uso NAME` runs the game.uso machinery (sites / calls / census /
# object / pmf-table / const-table / spell / check) on another module. Per module: the inner
# header (magic 0x12345678, ROM offset), where its Text comes from, the source dirs and the
# section-offset constants header (const-table --write / spell / check; created by --write).
#   ('map', SEGMENTS): Text linked into tenshoe.elf as those VRAM-0 segments; each function's Text
#                      offset = VRAM + (segment load address - Text data ROM offset).
#   ('yay0', OBJS):    a Yay0 code block built from OBJS concatenated in order (each at the sum
#                      of the earlier .text sizes; an `ld -r` block is its one .lnk.o).
# timproc_uso_b1 / b3 / b5 are the Text of timproc.uso / trkproc.uso / genproc.uso.
MODULES = {
    'game': (0x7CC900, ('yay0', (GAME_TEXT_O,)), (GAME_SRC,), 'include/game/game_uso_data.h'),
    'titproc': (0x595408, ('map', ('.titproc_uso',)), ('src/titproc_uso',),
                'include/game/titproc_uso_data.h'),
    'eddproc': (0x59F21C, ('map', ('.eddproc_uso',)), ('src/eddproc_uso',),
                'include/game/eddproc_uso_data.h'),
    'mgrproc': (0x5A0184, ('yay0', ('build/assets/mgrproc_uso_block1_yay0.lnk.o',)),
                ('src/mgrproc_uso',), 'include/game/mgrproc_uso_data.h'),
    'arcproc': (0x5A5C08, ('map', ('.arcproc_uso',)), ('src/arcproc_uso',),
                'include/game/arcproc_uso_data.h'),
    'h2hproc': (0x5AB230, ('map', ('.h2hproc_uso',)), ('src/h2hproc_uso',),
                'include/game/h2hproc_uso_data.h'),
    'timproc': (0x5AF114, ('yay0', ('build/src/timproc_uso_b1/timproc_uso_b1_o0_0.c.o',
                                    'build/src/timproc_uso_b1/timproc_uso_b1.c.o')),
                ('src/timproc_uso_b1',), 'include/game/timproc_uso_b1_data.h'),
    'trkproc': (0x5B3DE2, ('yay0', ('build/assets/timproc_uso_block3_yay0.lnk.o',)),
                ('src/timproc_uso_b3',), 'include/game/timproc_uso_b3_data.h'),
    'genproc': (0x5BCA4E, ('yay0', ('build/src/timproc_uso_b5/timproc_uso_b5.c.o',)),
                ('src/timproc_uso_b5',), 'include/game/timproc_uso_b5_data.h'),
    # the 2026-11-16 new modules (docs/1080_NEW_MODULES.md): uncompressed, c segment = Text payload
    'trnproc': (0x5B8D06, ('map', ('.trnproc_uso',)), ('src/trnproc_uso',), 'include/game/trnproc_uso_data.h'),
    'viewrec': (0x642346, ('map', ('.viewrec_uso',)), ('src/viewrec_uso',), 'include/game/viewrec_uso_data.h'),
    'nameent': (0x64C6C2, ('map', ('.nameent_uso',)), ('src/nameent_uso',), 'include/game/nameent_uso_data.h'),
    'balloon': (0x7DECE6, ('map', ('.balloon_uso',)), ('src/balloon_uso',), 'include/game/balloon_uso_data.h'),
    'heli': (0x7E3D9A, ('map', ('.heli_uso',)), ('src/heli_uso',), 'include/game/heli_uso_data.h'),
    'fire': (0x7F3E5E, ('map', ('.fire_uso',)), ('src/fire_uso',), 'include/game/fire_uso_data.h'),
    'checkpoint': (0x7F6D42, ('map', ('.checkpoint_uso',)), ('src/checkpoint_uso',), 'include/game/checkpoint_uso_data.h'),
    'snoweffects': (0x9023D2, ('map', ('.snoweffects_uso',)), ('src/snoweffects_uso',), 'include/game/snoweffects_uso_data.h'),
}


def mod_srcs(uso):
    """The .c / .cpp sources of a MODULES entry (bootup: src/game_libs + src/bootup_uso), sorted."""
    dirs = ('src/game_libs', 'src/bootup_uso') if uso == 'bootup' else MODULES[uso][2]
    return [p for d in dirs for e in ('c', 'cpp') for p in sorted(glob.glob(f'{d}/*.{e}'))]


def import_srcs(uso):
    """The src/match_imports ports of a module's functions (agent-g run 593): each port includes
    `context_<src dir>[_part].h` and keeps the module function's name, so its sites are that
    function's sites (by_func) and `check` verifies its constant / GL_* uses like the module's own
    (bootup: context_bootup_uso* / context_game_libs*)."""
    dirs = ('bootup_uso', 'game_libs') if uso == 'bootup' else \
        tuple(os.path.basename(d) for d in MODULES[uso][2])
    out = []
    for p in sorted(glob.glob('src/match_imports/*.c')):
        m = re.search(r'^#include "context_(\w+)\.h"', open(p, errors='replace').read(), re.M)
        if m and any(m.group(1) == d or m.group(1).startswith(d + '_') for d in dirs):
            out.append(p)
    return out


def mod_text_objs(uso, text_rom=None):
    """[(text offset, size, obj path)] and {text offset: name} from the map, for a MODULES entry."""
    kind, what = MODULES[uso][1]
    if kind == 'map':
        return load_map(MAP, what, text_rom)
    from elftools.elf.elffile import ELFFile
    objs, off = [], 0
    for path in what:
        size = 0
        if os.path.exists(path):
            with open(path, 'rb') as f:
                t = ELFFile(f).get_section_by_name('.text')
                size = t['sh_size'] if t is not None else 0
        objs.append((off, size, path))
        off += size
    return {}, objs
TYPES = {1: 'Sym', 2: 'TextReloc', 3: 'DataReloc', 4: 'RoDataReloc', 5: 'Text', 6: 'Data',
         7: 'RoData', 9: 'EntrySym', 11: 'End'}


def u32(b, o):
    return struct.unpack_from('>I', b, o)[0]


def parse_uso(rom, mod, roms=None):
    """Section table walk (same layout as scripts/uso-sym-oracle.py). Returns name -> bytes;
    `roms` (a dict), when given, gets name -> ROM offset of the section data."""
    import crunch64
    # snoweffects.uso stores 0 for the module magic; modules without Bss (nameent, balloon,
    # heli, fire, checkpoint) put their first section record right after it (run 617)
    assert u32(rom, mod) in (0x12345678, 0)
    o = mod + 16 if u32(rom, mod + 4) == 8 else mod + 4
    secs = {}
    while o < len(rom) - 12:
        for k in range(4):
            t = u32(rom, o + k); fl = u32(rom, o + k + 8)
            if t in TYPES and (fl >> 16) in (0x1000, 0x1001, 0x0800, 0x2000, 0x2001):
                o += k
                break
        else:
            break
        t = u32(rom, o); size = u32(rom, o + 4); flag = u32(rom, o + 8); d0 = o + 12
        if flag & 0x10000:
            data = crunch64.yay0.decompress(rom[d0:d0 + size + 0x40000])
            nxt = d0 + len(crunch64.yay0.compress(data))
        else:
            data = rom[d0:d0 + size]; nxt = d0 + size
        secs.setdefault(TYPES[t], data)
        if roms is not None:
            roms.setdefault(TYPES[t], d0)
        o = nxt
        if t in (9, 11) and 'Sym' in secs:
            break
    return secs


class Sym:
    __slots__ = ('idx', 'tag', 'value', 'kind', 'sect', 'name')

    def __init__(self, idx, tag, value):
        self.idx, self.tag, self.value = idx, tag, value
        info = tag >> 16
        self.kind = info & 7            # 1 import, 2 object, 3 absolute, 4 section base
        self.sect = info >> 4           # kinds 2/4: section type; kind 1: exporting module id
        self.name = None

    @property
    def is_obj(self):
        return self.kind == 2

    @property
    def is_base(self):
        return self.kind == 4

    def label(self):
        if self.kind == 4:
            return f'{SECT_NAME.get(self.sect, self.sect)}-base(sym{self.idx})'
        if self.kind == 2:
            n = f' {self.name}' if self.name else ''
            return f'{SECT_NAME.get(self.sect, self.sect)}+0x{self.value:X}(sym{self.idx}{n})'
        if self.kind == 1:
            return f'import mod{self.sect}(sym{self.idx})'
        return f'abs 0x{self.value:X}(sym{self.idx})'


class Site:
    __slots__ = ('off', 'rtype', 'sym', 'k', 'func', 'placeholder', 'obj', 'objoff')

    def key(self):
        """(section, object start) this site lands in."""
        return self.obj


TEXT_ROM = 0xDD0A6C           # bootup.uso Text section data in the ROM
SEGMENTS = ('.bootup_uso', '.game_libs_head', '.game_libs')   # = bootup.uso's Text section


def load_map(path, segments=SEGMENTS, text_rom=TEXT_ROM):
    """Function symbols of the segments that make up bootup.uso's Text (bootup_uso then
    game_libs), keyed by TEXT SECTION OFFSET (= load address - TEXT_ROM + (vram - section vram)),
    plus each .o's (text offset, size, path)."""
    funcs = {}            # text offset -> name
    objs = []             # (text offset, size, objpath)
    cur = None
    lines = open(path).read().split('\n')
    for i, line in enumerate(lines):
        if line.startswith('.'):
            cur = line.split()[0]
            hdr = line if 'load address' in line else line + ' ' + lines[i + 1]
            m = re.search(r'0x([0-9a-f]+)\s+0x[0-9a-f]+\s+load address 0x([0-9a-f]+)', hdr)
            delta = (int(m.group(2), 16) - text_rom - int(m.group(1), 16)) if m else None
            continue
        if cur not in segments:
            continue
        m = re.match(r'\s+\.text\s+0x([0-9a-f]+)\s+0x([0-9a-f]+)\s+(\S+\.o)$', line)
        if m:
            objs.append((int(m.group(1), 16) + delta, int(m.group(2), 16), m.group(3)))
            continue
        m = re.match(r'\s+0x([0-9a-f]+)\s+([A-Za-z_]\w*)$', line)
        if m and not m.group(2).endswith(('_START', '_END', '_SIZE')) \
                and not m.group(2).startswith(('_binary_', '__romPos')):
            funcs.setdefault(int(m.group(1), 16) + delta, m.group(2))
    return funcs, objs


def load_objfiles(objs):
    """Local function symbols + placeholder name of each HI16/LO16 reloc, keyed by text offset."""
    from elftools.elf.elffile import ELFFile
    funcs, ph = {}, {}
    for base, size, path in objs:
        if not os.path.exists(path):
            continue
        with open(path, 'rb') as f:
            e = ELFFile(f)
            symtab = e.get_section_by_name('.symtab')
            tidx = [i for i, s in enumerate(e.iter_sections()) if s.name == '.text'][0]
            for s in symtab.iter_symbols():
                if s['st_shndx'] == tidx and s.name and s['st_info']['type'] in ('STT_FUNC', 'STT_NOTYPE') \
                        and not s.name.startswith(('$', '.L', 'jtbl')):
                    funcs.setdefault(base + s['st_value'], s.name)
            for sec in e.iter_sections():
                if sec.name in ('.rel.text', '.rela.text'):
                    for r in sec.iter_relocations():
                        if r['r_info_type'] in (5, 6):
                            ph[base + r['r_offset']] = symtab.get_symbol(r['r_info_sym']).name
    return funcs, ph


BUILD_VENV = '/home/danjs/code/decomp/tools/build-venv/bin'


def ensure_build(no_build=False, uso='bootup'):
    """The census reads the built map + .o relocs, so they must be at least as new as the
    sources. A missing or stale build is refreshed with an incremental
    `make RUN_CC_CHECK=0 -j8` (the readability gate leaves a full build behind, so right
    after a gate this is a no-op; `--keep-build` is NOT needed). --no-build only warns."""
    if uso in MODULES:
        kind, what = MODULES[uso][1]
        dirs, target = MODULES[uso][2], (MAP if kind == 'map' else what[-1])
    else:
        dirs, target = ('src/game_libs', 'src/bootup_uso'), MAP
    srcs = [p for d in dirs for e in ('c', 'cpp', 's')
            for p in glob.glob(f'{d}/*.{e}')]
    # headers are not make dependencies here, and the census only reads relocs (which header
    # aliases cannot change: they expand to the same tokens); `check` reads headers live.
    newest = max((os.path.getmtime(p) for p in srcs), default=0)
    have = os.path.exists(target)
    if have and os.path.getmtime(target) >= newest:
        return
    why = 'missing' if not have else 'older than the newest src file'
    if no_build:
        if not have:
            sys.exit(f'{target} {why}: build first (make RUN_CC_CHECK=0 -j8)')
        print(f'warning: {target} is {why}; results may be stale (--no-build)', file=sys.stderr)
        return
    import subprocess
    print(f'{target} {why}: running make RUN_CC_CHECK=0 -j8 ...', file=sys.stderr)
    env = dict(os.environ)
    if os.path.isdir(BUILD_VENV):
        env['PATH'] = BUILD_VENV + os.pathsep + env.get('PATH', '')
    r = subprocess.run(['make', 'RUN_CC_CHECK=0', '-j8'], env=env,
                       stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
    if r.returncode != 0:
        print('\n'.join(r.stdout.splitlines()[-15:]), file=sys.stderr)
        if not os.path.exists(target) or os.path.getmtime(target) < newest:
            sys.exit(f'make failed and {target} is not current')
        print('warning: make failed after linking (ROM mismatch?); using the fresh map', file=sys.stderr)


class Census:
    def __init__(self, root='.', no_build=False, uso='bootup'):
        os.chdir(root)
        ensure_build(no_build, uso)
        self.uso = uso
        self.rom = open(ROM, 'rb').read()
        self.sec_rom = {}
        secs = parse_uso(self.rom, MODULES[uso][0] if uso in MODULES else INNER_HDR, self.sec_rom)
        self.secs = secs
        sd = secs['Sym']
        self.syms = [Sym(i, *struct.unpack_from('>II', sd, i * 12)) for i in range(len(sd) // 12)]
        # section-base Syms (kind 4) by section type: bootup 1 RoData / 2 Data / 3 Text / 4 Bss,
        # game.uso 2 RoData / 3 Data / 4 Bss / 1 Text
        self.base = {s.sect: s.idx for s in self.syms if s.kind == 4}
        self.text = secs['Text']
        if uso in MODULES:
            # game.uso: one object whose .text offsets are game.uso Text offsets (local + global
            # symbols); the mode USOs: their map segments / Yay0 block objects (mod_text_objs)
            mfuncs, objs = mod_text_objs(uso, self.sec_rom.get('Text'))
            ofuncs, self.ph = load_objfiles(objs)
        else:
            mfuncs, objs = load_map(MAP)
            ofuncs, self.ph = load_objfiles(objs)
        allf = dict(ofuncs)
        allf.update(mfuncs)       # global names win over local aliases at the same address
        # text Sym -> function name
        self.by_text = {}
        for s in self.syms:
            if s.kind == 2 and s.sect == 5:
                n = allf.get(s.value)
                if n:
                    s.name = n
        self.fstarts = sorted(allf)
        self.fnames = [allf[a] for a in self.fstarts]
        # named objects per section, sorted by offset
        self.objs = collections.defaultdict(list)
        for s in self.syms:
            if s.kind == 2:
                self.objs[s.sect].append((s.value, s.idx))
        for v in self.objs.values():
            v.sort()
        self.sect_size = {6: len(secs.get('Data', b'')), 7: len(secs.get('RoData', b''))}
        self._sites()
        self._data_ptrs()

    def func_at(self, vram):
        i = bisect.bisect_right(self.fstarts, vram) - 1
        return self.fnames[i] if i >= 0 else '?'

    def resolve(self, sym, k):
        """-> (object key, offset in object). Object key: ('sym', idx) or ('anon', sect, off)."""
        if sym.kind == 2:
            return ('sym', sym.idx), k
        if sym.kind == 4:
            lst = self.objs.get(sym.sect, [])
            i = bisect.bisect_right(lst, (k, 1 << 30)) - 1
            if i >= 0:
                start, idx = lst[i]
                end = lst[i + 1][0] if i + 1 < len(lst) else self.sect_size.get(sym.sect, 1 << 32)
                if k < end and k - start < 0x10000:
                    # a section-base site inside a named object's span: same TU / static
                    # access. Identity is by position only (object sizes are inferred from
                    # the next named object), so keep it distinguishable from a direct reloc.
                    return ('via', idx), k - start
            return ('anon', sym.sect, k), 0
        return ('ext', sym.idx), k

    def _sites(self):
        tr = self.secs['TextReloc']
        self.sites = []
        hi = 0
        for i in range(len(tr) // 12):
            info, off, _orig = struct.unpack_from('>III', tr, i * 12)
            rtype, si = info & 7, info >> 4
            if rtype not in (2, 3):
                continue
            word = u32(self.text, off)
            if rtype == 3:
                hi = word & 0xFFFF
                k = hi << 16
            else:
                lo = word & 0xFFFF
                k = ((hi << 16) + (lo - 0x10000 if lo & 0x8000 else lo)) & 0xFFFFFFFF
            s = Site()
            s.off, s.rtype, s.sym, s.k = off, rtype, self.syms[si], k
            s.func = self.func_at(off)
            s.placeholder = self.ph.get(off)
            if rtype == 2:
                s.obj, s.objoff = self.resolve(s.sym, k)
            else:
                s.obj, s.objoff = None, None
            self.sites.append(s)
        self.by_func = collections.defaultdict(list)
        for s in self.sites:
            self.by_func[s.func].append(s)

    def _data_ptrs(self):
        """DataReloc words inside each Data object: vtables / string tables / fn tables."""
        self.dptr = collections.defaultdict(list)    # obj key -> [(off, target sym)]
        for secname, sect in (('DataReloc', 6), ('RoDataReloc', 7)):
            if sect not in self.base:       # a mode USO without that section (eddproc: no RoData)
                continue
            base = self.syms[self.base[sect]]
            dr = self.secs.get(secname, b'')
            for i in range(len(dr) // 12):
                info, off, _ = struct.unpack_from('>III', dr, i * 12)
                if info & 7 != 4:
                    continue
                key, o = self.resolve(base, off)
                if key[0] == 'via':
                    key = ('sym', key[1])
                self.dptr[key].append((o, self.syms[info >> 4]))

    def obj_label(self, key):
        if key[0] in ('sym', 'via'):
            s = self.syms[key[1]]
            return s.label() + (' [via base]' if key[0] == 'via' else '')
        if key[0] == 'anon':
            return f'{SECT_NAME.get(key[1], key[1])}-base+0x{key[2]:X}'
        return self.syms[key[1]].label()

    def obj_size(self, key):
        if key[0] not in ('sym', 'via'):
            return None
        s = self.syms[key[1]]
        lst = self.objs[s.sect]
        i = lst.index((s.value, s.idx))
        end = lst[i + 1][0] if i + 1 < len(lst) else self.sect_size.get(s.sect)
        return None if end is None else end - s.value


def cmd_sites(c, a):
    names = a.funcs or sorted(c.by_func, key=lambda f: c.by_func[f][0].off)
    for f in names:
        ss = [s for s in c.by_func.get(f, []) if s.rtype == 2]
        if not ss:
            continue
        print(f'{f}:')
        for s in ss:
            ph = s.placeholder or '-'
            print(f'  T+0x{s.off:05X} K=0x{s.k:X} {ph:22s} {c.obj_label(s.obj)}'
                  f'{"" if not s.objoff else f" +0x{s.objoff:X}"}')


def cmd_calls(c, a):
    """Resolved jal targets (TextReloc type 1 = R_MIPS_26: the reloc names the callee's Sym, so the
    `jal 0` / gl_func_00000000 placeholder calls get their real callee). `calls F..` lists what F
    calls, in address order; `calls --to F..` lists who calls F inside bootup.uso. Callers in other
    USOs go through import hashes and are not found here."""
    want = set(a.funcs)
    tr = c.secs['TextReloc']
    for i in range(len(tr) // 12):
        info, off, _orig = struct.unpack_from('>III', tr, i * 12)
        if info & 7 != 1:
            continue
        sym = c.syms[info >> 4]
        caller = c.func_at(off)
        callee = sym.name or sym.label()
        if (callee if a.to else caller) in want:
            print(f'{caller:40s} T+0x{off:05X} -> {callee}')


def census_rows(c):
    agg = collections.OrderedDict()
    for s in c.sites:
        if s.rtype != 2 or s.obj[0] == 'ext':
            continue
        r = agg.setdefault(s.obj, [0, set(), set()])
        r[0] += 1
        r[1].add(s.func)
        r[2].add(s.objoff)
    return sorted(agg.items(), key=lambda kv: (-kv[1][0], kv[0]))


def cmd_census(c, a):
    rows = census_rows(c)
    if a.direct:
        rows = [r for r in rows if r[0][0] == 'sym']
    tot = sum(r[0] for _, r in rows)
    print(f'{len(rows)} objects, {tot} LO16 sites')
    print(f'{"sites":>5} {"fns":>4} {"offs":>4} {"size":>6} {"ptrs":>4}  object  [sample functions]')
    for key, (n, fs, offs) in rows[:a.n]:
        sz = c.obj_size(key)
        ptrs = c.dptr.get(('sym', key[1]) if key[0] == 'via' else key, [])
        tptr = sum(1 for _, t in ptrs if t.kind == 2 and t.sect == 5)
        pdesc = f'{len(ptrs)}' + (f'/{tptr}T' if tptr else '')
        sample = ', '.join(sorted(fs)[:3])
        print(f'{n:5d} {len(fs):4d} {len(offs):4d} {sz if sz is not None else "-":>6} {pdesc:>4}  '
              f'{c.obj_label(key)}  [{sample}]')


def parse_objarg(c, s):
    m = re.match(r'(?:sym)?(\d+)$', s)
    if m:
        return ('sym', int(m.group(1)))
    m = re.match(r'(Data|RoData|Bss|D|R|B):(0x[0-9A-Fa-f]+|\d+)$', s)
    if m:
        sect = {'Data': 6, 'D': 6, 'RoData': 7, 'R': 7, 'Bss': 8, 'B': 8}[m.group(1)]
        key, _ = c.resolve(Sym(-1, (sect << 20) | 0x40000, 0), int(m.group(2), 0))
        return key
    sys.exit(f'bad object {s}')


def cmd_object(c, a):
    key = parse_objarg(c, a.obj)
    print(c.obj_label(key), 'size', c.obj_size(key))
    for o, t in sorted(c.dptr.get(key, [])):
        print(f'  ptr +0x{o:X} -> {t.label()}')
    byf = collections.defaultdict(list)
    for s in c.sites:
        if s.rtype == 2 and (s.obj == key or (key[0] == 'sym' and s.obj == ('via', key[1]))):
            byf[s.func].append(s.objoff)
    for f in sorted(byf, key=lambda f: -len(byf[f])):
        print(f'  {len(byf[f]):3d} {f}: ' + ' '.join(f'+0x{o:X}' for o in sorted(set(byf[f]))))


# ---------------------------------------------------------------- macro checker
DEF_RE = re.compile(r'^\s*#\s*define\s+(\w+)(\([^)]*\))?\s+(.*?)\s*$')


def scan_macros():
    """Accessor macros over the zero placeholder, in include/ and src/game_libs."""
    out = []
    files = sorted(glob.glob('include/**/*.h', recursive=True)) + \
        [p for d in ('game_libs', 'bootup_uso') for e in ('c', 'cpp')
         for p in sorted(glob.glob(f'src/{d}/*.{e}'))]
    pending = []
    for path in files:
        lines = open(path, errors='replace').read().split('\n')
        i = 0
        while i < len(lines):
            line = lines[i]
            start = i
            full = line
            while full.endswith('\\') and i + 1 < len(lines):
                i += 1
                full = full[:-1] + ' ' + lines[i]
            m = DEF_RE.match(full)
            i += 1
            if not m:
                continue
            name, params, body = m.group(1), m.group(2), m.group(3)
            if name in BASE_MACRO or name in STR_MACRO:
                continue
            cm = re.search(r'/\*(.*?)(\*/|$)', body)
            comment = cm.group(1) if cm else ''
            # the comment block directly above the #define also counts
            j = start - 1
            if j >= 0 and lines[j].rstrip().endswith('*/'):
                blk = []
                while j >= 0:
                    blk.append(lines[j])
                    if '/*' in lines[j]:
                        break
                    j -= 1
                comment += ' ' + ' '.join(reversed(blk))
            body_nc = re.sub(r'/\*.*?(\*/|$)', '', body)
            bm = re.search(r'\b(GL_DATA|GL_RODATA|GL_BSS|GL_TEXT)\s*\(\s*(0x[0-9A-Fa-f]+|\d+)\s*\)', body_nc)
            uses_ph = re.search(r'&\s*\(?\s*(D_00000000\w*|gl_data_00000000\w*)\b', body_nc) or \
                re.fullmatch(r'\(?\s*(D_00000000\w*|gl_data_00000000\w*)\s*\)?', body_nc.strip())
            if not bm and not uses_ph:
                continue
            claim = None
            sm = re.search(r'\b[Ss]ym\s*#?(\d+)\b', comment)
            if bm:
                claim = ('base', BASE_MACRO[bm.group(1)], int(bm.group(2), 0))
            elif sm:
                claim = ('sym', int(sm.group(1)))
            plain = bool(re.fullmatch(r'\(?\s*(D_00000000|gl_data_00000000)\s*\)?', body_nc.strip()))
            out.append(dict(name=name, params=params, path=path, line=i, claim=claim,
                            local=not path.startswith('include/'), via=None, plain=plain))
        pending.extend((path, l) for l in _alias_users(lines, path))
    # second pass: macros spelled over a HEADER OBJECT ALIAS (`(*(T *)&gAudioContext)`) or over a
    # renamed per-site alias of one (`&gAudioContext_345c0`) inherit that alias's Sym claim.
    aliases = header_aliases(out)
    seen = {(m['path'], m['name']) for m in out}
    for path, (name, params, body, line, own) in pending:
        if (path, name) in seen or name in BASE_MACRO or name in STR_MACRO:
            continue
        hit = None
        for tok in re.findall(r'\b[A-Za-z_]\w*\b', body):
            a = alias_of(aliases, tok)
            if a:
                hit = a
                break
        if not hit:
            continue
        sm = re.search(r'\b[Ss]ym\s*#?(\d+)\b', own)
        claim = ('sym', int(sm.group(1))) if sm else hit[1]
        seen.add((path, name))
        out.append(dict(name=name, params=params, path=path, line=line, claim=claim,
                        local=not path.startswith('include/'), via=hit[0]))
    return out


def _alias_users(lines, path):
    """(name, params, body_without_comments, line, comment) of every #define in `lines`."""
    i = 0
    while i < len(lines):
        full = lines[i]
        while full.endswith('\\') and i + 1 < len(lines):
            i += 1
            full = full[:-1] + ' ' + lines[i]
        i += 1
        m = DEF_RE.match(full)
        if m:
            body = m.group(3)
            cm = re.search(r'/\*(.*?)(\*/|$)', body)
            yield (m.group(1), m.group(2), re.sub(r'/\*.*?(\*/|$)', '', body), i,
                   cm.group(1) if cm else '')


def header_aliases(macros):
    """name -> claim for the plain header object aliases (`#define gX D_00000000 /* Sym N */`)."""
    return {m['name']: m['claim'] for m in macros
            if not m['local'] and m['claim'] and m['claim'][0] == 'sym' and not m['params']
            and not m.get('via')}


def alias_of(aliases, tok):
    """(alias, claim) when `tok` is a header object alias, or a per-site zero alias renamed
    after one (`<alias>_<suffix>`, READABILITY_PLAN 9.1.5); the longest alias prefix wins."""
    if tok in aliases:
        return tok, aliases[tok]
    best = None
    for a in aliases:
        if tok.startswith(a + '_') and (best is None or len(a) > len(best)):
            best = a
    return (best, aliases[best]) if best else None


FUNC_RE = re.compile(r'^(?!\s)(?:[\w\*]+[\s\*]+)+?\**(\w+)\s*\(([^;{]*?)\)\s*\{', re.M | re.S)


def _skip_brace_body(src, i):
    """src[i-1] == '{'; return the index of the matching '}' (skips comments, strings, chars)."""
    depth, n = 1, len(src)
    while i < n:
        ch = src[i]
        if src.startswith('/*', i):
            i = src.find('*/', i + 2)
            if i < 0:
                return -1
            i += 2
            continue
        if src.startswith('//', i):
            i = src.find('\n', i)
            if i < 0:
                return -1
            continue
        if ch in '"\'':
            j = i + 1
            while j < n and src[j] != ch:
                j += 2 if src[j] == '\\' else 1
            i = j + 1
            continue
        if ch == '{':
            depth += 1
        elif ch == '}':
            depth -= 1
            if depth == 0:
                return i
        i += 1
    return -1


def func_spans(src):
    """[(name, body_start, body_end)] for top-level C function definitions (column-0
    heuristic for the header, brace matching for the body; preprocessor-conditional
    braces are assumed balanced per branch)."""
    out = []
    pos = 0
    for m in FUNC_RE.finditer(src):
        name = m.group(1)
        if m.start() < pos or name in ('if', 'while', 'for', 'switch', 'return', 'sizeof'):
            continue
        end = _skip_brace_body(src, m.end())
        if end < 0:
            continue
        out.append((name, m.end(), end))
        pos = end
    return out


def func_bodies(path):
    """name -> body text for top-level C function definitions."""
    src = open(path, errors='replace').read()
    out = {}
    for name, a, b in func_spans(src):
        out.setdefault(name, src[a:b])
    return out


SITE_RE = re.compile(r'&D_00000000\b(?:(\s*\+\s*)(0x[0-9A-Fa-f]+|\d+)\b)?')
# a bare value use (`D_00000000 |= mask`, `(Thread *)D_00000000`): a K = 0 site, unless a member
# access follows (`D_00000000.f`, whose K would be the field offset)
BARE_RE = re.compile(r'(?<![&\w])D_00000000\b(?!\s*\.)')


GL_DATA_REL = '../../include/gdl/gl_data.h'


def add_gl_data_include(path, src):
    """Include gl_data.h once. C files: after common.h, as "gdl/gl_data.h". NCC .cpp donors
    build without -I include, so they get the relative path, placed after the file's own
    `extern char D_00000000;` (else after `extern "C" {`, else after the last #include)."""
    if 'gdl/gl_data.h"' in src:
        return src
    if not path.endswith('.cpp'):
        i = src.find('#include "common.h"')
        i = src.find('\n', i) + 1 if i >= 0 else 0
        return src[:i] + '#include "gdl/gl_data.h"\n' + src[i:]
    line = f'#include "{GL_DATA_REL}"  /* object aliases (NCC donors build without -I include) */\n'
    m = re.search(r'^extern\s+\w+\s+D_00000000\s*;.*\n', src, re.M) or \
        re.search(r'^extern\s+"C"\s*\{.*\n', src, re.M)
    if not m:
        ms = list(re.finditer(r'^#include .*\n', src, re.M))
        m = ms[-1] if ms else None
    i = m.end() if m else 0
    return src[:i] + line + src[i:]


def _fn_selected(f, excl, only):
    return not (excl and excl.search(f)) and not (only and not only.fullmatch(f))


def plan_convert(c, n, src, excl=None, only=None, bare=False):
    """(start, end, func) spans of `D_00000000` tokens in `src` that the relocs prove are Sym n
    (proof rule in cmd_convert). `bare` adds K = 0 value uses (`D_00000000 |= m`)."""
    edits = []
    for f, b0, b1 in func_spans(src):
        if not _fn_selected(f, excl, only):
            continue
        ss = [x for x in c.by_func.get(f, []) if x.rtype == 2]
        if not ss or any(x.placeholder is None for x in ss):
            continue
        byk = collections.defaultdict(set)
        for x in ss:
            if x.placeholder == 'D_00000000':
                byk[x.k].add(x.sym.idx)
        for mm in SITE_RE.finditer(src, b0, b1):
            k = int(mm.group(2), 0) if mm.group(2) else 0
            if byk.get(k) == {n}:
                edits.append((mm.start() + 1, mm.start() + 1 + len('D_00000000'), f))
        if bare and byk.get(0) == {n}:
            for mm in BARE_RE.finditer(src, b0, b1):
                edits.append((mm.start(), mm.end(), f))
    return edits


def cmd_convert(c, a):
    """Spell plain-D_00000000 sites of one named object with its object-alias macro.
    A site `&D_00000000` (K = 0) is converted only when EVERY K=0 D_00000000-relocated
    instruction of the function names the macro's Sym; `&D_00000000 + K` only when every
    D_00000000 instruction with that K names it. Functions whose .o carries no relocs
    (donor splices) are skipped. The rewrite is a pure token substitution
    (D_00000000 -> NAME, NAME defined as D_00000000), so it is byte-safe by construction;
    the census only decides which spelling is TRUE."""
    m = [x for x in scan_macros() if x['name'] == a.name and not x['local']]
    if not m or not m[0]['claim'] or m[0]['claim'][0] != 'sym':
        sys.exit(f'{a.name}: no header object alias with a `Sym N` claim')
    if not m[0].get('plain'):
        sys.exit(f'{a.name}: not a plain object alias (`#define NAME D_00000000`); a typed accessor '
                 'cannot replace the bare token, convert its sites by hand')
    n = m[0]['claim'][1]
    excl = re.compile(a.exclude) if a.exclude else None
    only = re.compile(a.only) if a.only else None
    total = 0
    for path in a.files:
        src = open(path, errors='replace').read()
        edits = plan_convert(c, n, src, excl, only, a.bare)
        if not edits:
            continue
        fs = sorted({e[2] for e in edits})
        print(f'{path}: {len(edits)} sites in {len(fs)} functions: {", ".join(fs)}')
        total += len(edits)
        if a.write:
            for s0, s1, _ in sorted(edits, reverse=True):
                src = src[:s0] + a.name + src[s1:]
            src = add_gl_data_include(path, src)
            open(path, 'w').write(src)
    print(f'{a.name} (Sym {n}): {total} sites' + ('' if a.write else ' (dry run; --write to apply)'))


def claim_hits(c, func, cl):
    """LO16 sites of `func` consistent with a macro's claim:
    ('sym', N) -> relocated against Sym N, or a section-base site inside N's span;
    ('base', sect, K) -> relocated against that section's base Sym with exactly K."""
    ss = [s for s in c.by_func.get(func, []) if s.rtype == 2]
    if cl[0] == 'sym':
        return [s for s in ss if s.sym.idx == cl[1] or s.obj in (('sym', cl[1]), ('via', cl[1]))]
    return [s for s in ss if s.sym.is_base and s.sym.sect == cl[1] and s.k == cl[2]]


# `(char *)&D_00000000 + K`, or `&D_00000000 + K` in a TU whose D_00000000 is plain `char`
BASE_SITE_RE = re.compile(r'(\(\s*char\s*\*\s*\)\s*)?&D_00000000\s*\+\s*(0x[0-9A-Fa-f]+|\d+)\b')
# operators around the site that must not bind tighter than the `+` (the macro adds parentheses)
_BAD_BEFORE = re.compile(r'(\)|[-*/%!~&]|\bsizeof)\s*$')
_BAD_AFTER = re.compile(r'^\s*(\[|\(|->|\.|\+\+|--|[*/%](?!=)|[*/%]=)')
_GOOD_BEFORE = re.compile(r'([(,=?:{;]|\breturn|\|\||==|!=|<|>)\s*$')


def rom_string(c, sect, k):
    """The NUL-terminated printable string starting at section offset k, or None."""
    d = c.secs.get(SECT_NAME.get(sect))
    if d is None or k >= len(d) or (k and d[k - 1] != 0):
        return None
    e = d.find(b'\0', k)
    if e <= k:
        return None
    b = d[k:e]
    if len(b) < 3 or any((x < 0x20 and x not in (9, 10)) or x > 0x7E for x in b):
        return None
    return b.decode()


def c_literal(t):
    return '"' + t.replace('\\', '\\\\').replace('"', '\\"').replace('\n', '\\n').replace('\t', '\\t') + '"'


def c_unliteral(lit):
    body = lit[1:-1]
    return re.sub(r'\\(.)', lambda m: {'n': '\n', 't': '\t'}.get(m.group(1), m.group(1)), body)


def char_decl_only(src):
    decls = re.findall(r'^\s*extern\s+([\w ]+?)\s+D_00000000\s*(\[\s*\])?\s*;', src, re.M)
    return bool(decls) and all(t.strip() == 'char' and not arr for t, arr in decls)


def plan_spell_base(c, src, excl=None, only=None):
    """(start, end, replacement, func, macro) for the provable plain section-base sites in `src`."""
    plain_char = char_decl_only(src)
    edits = []
    for f, b0, b1 in func_spans(src):
        if not _fn_selected(f, excl, only):
            continue
        ss = [x for x in c.by_func.get(f, []) if x.rtype == 2]
        if not ss or any(x.placeholder is None for x in ss):
            continue
        byk = collections.defaultdict(set)
        for x in ss:
            if x.placeholder == 'D_00000000':
                byk[x.k].add((x.sym.is_base, x.sym.sect))
        for mm in BASE_SITE_RE.finditer(src, b0, b1):
            if not mm.group(1) and not plain_char:
                continue
            k = int(mm.group(2), 0)
            kinds = byk.get(k)
            if not kinds or len(kinds) != 1:
                continue
            isb, sect = next(iter(kinds))
            if not isb or sect not in (6, 7, 8):
                continue
            before = src[max(b0, mm.start() - 12):mm.start()]
            if _BAD_BEFORE.search(before) or not _GOOD_BEFORE.search(before):
                continue
            if _BAD_AFTER.match(src[mm.end():mm.end() + 3]):
                continue
            t = rom_string(c, sect, k) if sect in (6, 7) else None
            if t is not None:
                name = {6: 'GL_DATA_STR', 7: 'GL_RODATA_STR'}[sect]
                rep = f'{name}(0x{k:X}, {c_literal(t)})'
            else:
                name = {6: 'GL_DATA', 7: 'GL_RODATA', 8: 'GL_BSS'}[sect]
                rep = f'{name}(0x{k:X})'
            edits.append((mm.start(), mm.end(), rep, f, name))
    return edits


# Owner areas for `spell-base --owner-report` (readability phase, run 504). First match wins:
# (owner, description, path regex or None, function regex or None); both must match when given.
_GFX_FN = (r'(Anim|anim_|Skin|skin_|BaseLight|BaseMat|BasePolyShape|ColourItem|DisplayMgr|FogParms|'
           r'GfxLib|LightTarget|Mat_|MorphPoly|PolyShape|RcpMgr|RcpTask|ScreenWindow|Shadow|Sprite|'
           r'Window|ZoneShape|gfx_|shape_|light_|console_|CollShape|absrot).*')
OWNERS = [
    ('n', 'the 2026-11-16 new modules (src/<m>_uso/*, docs/1080_NEW_MODULES.md)',
     r'^src/(trnproc|viewrec|nameent|balloon|heli|fire|checkpoint|snoweffects)_uso/', None),
    ('agent-c', 'audio: Audio* / snd_* / Snd* functions, any file', None, r'(Audio|snd_|Snd|Note_|Seq_).*'),
    ('agent-b', 'game_libs_post0b.c / game_libs_post1b.c', r'game_libs_post[01]b\.c$', None),
    ('agent-b', 'gfx / anim / shape / window / light / CollShape functions in other files', None, _GFX_FN),
    ('agent-h', 'bootup_uso (src/bootup_uso/*)', r'^src/bootup_uso/', None),
    ('agent-h', 'game_libs.c / game_libs_tail.c', r'game_libs(_tail)?\.c$', None),
    ('unassigned', 'other game_libs files (coordinator or any free agent)', r'.', None),
]


def owner_of(path, func):
    for o, desc, pr, fr in OWNERS:
        if (pr is None or re.search(pr, path)) and (fr is None or re.fullmatch(fr, func)):
            return o, desc
    return 'unassigned', OWNERS[-1][1]


def _only_re(funcs):
    return "'^(" + '|'.join(sorted(funcs)) + ")$'"


def owner_report(c, a):
    """Per owner area, the exact `spell-base` / `convert` commands (one per file, restricted with
    --only to that owner's functions) that respell every provable plain site. Each command is the
    dry-run-verified set: run it with --write, then `check` and the gate."""
    srcs = a.files or [p for d in ('bootup_uso', 'game_libs') for e in ('c', 'cpp')
                       for p in sorted(glob.glob(f'src/{d}/*.{e}'))]
    objs = [(m['name'], m['claim'][1]) for m in scan_macros()
            if not m['local'] and m['claim'] and m['claim'][0] == 'sym' and m.get('plain')
            and not m['params']]
    # (owner, desc) -> list of (cmd, nsites, funcs)
    rows = collections.defaultdict(list)
    for path in srcs:
        src = open(path, errors='replace').read()
        groups = collections.defaultdict(lambda: collections.defaultdict(list))
        bare = set()
        for e in plan_spell_base(c, src):
            groups[owner_of(path, e[3])][('spell-base',)].append(e[3])
        for name, n in objs:
            plain = {(e[0], e[1]) for e in plan_convert(c, n, src)}
            for e in plan_convert(c, n, src, bare=True):
                own = owner_of(path, e[2])
                groups[own][('convert', name)].append(e[2])
                if (e[0], e[1]) not in plain:
                    bare.add((own, name))
        for own, g in groups.items():
            for key, fl in sorted(g.items()):
                fs = sorted(set(fl))
                if key[0] == 'spell-base':
                    cmd = f'spell-base {path} --only {_only_re(fs)}'
                else:
                    cmd = f'convert {key[1]} {path} --only {_only_re(fs)}' + \
                        (' --bare' if (own, key[1]) in bare else '')
                rows[own].append((cmd, len(fl), fs))
    out = ['PY=/home/danjs/code/decomp/.venv/bin/python   # run from the project root; then `check` and the gate', '']
    order = []
    for o, desc, _, _ in OWNERS:
        if (o, desc) not in order:
            order.append((o, desc))
    tot = collections.Counter()
    for key in order:
        if key not in rows:
            continue
        merged = collections.OrderedDict()
        for cmd, n, fs in rows[key]:
            if cmd in merged:
                merged[cmd] = (merged[cmd][0] + n, sorted(set(merged[cmd][1]) | set(fs)))
            else:
                merged[cmd] = (n, fs)
        nsites = sum(v[0] for v in merged.values())
        tot[key[0]] += nsites
        out.append(f'### {key[0]}: {key[1]} ({nsites} sites, {len(merged)} commands)')
        for cmd, (n, fs) in merged.items():
            out.append(f'$PY scripts/uso-data-sites.py {cmd} --write   # {n} sites, {len(fs)} functions')
        out.append('')
    out.append('totals: ' + ', '.join(f'{o} {n}' for o, n in tot.items()) + f'; all {sum(tot.values())}')
    text = '\n'.join(out) + '\n'
    if a.owner_report == '-':
        sys.stdout.write(text)
    else:
        open(a.owner_report, 'w').write(text)
        print(f'owner report: {a.owner_report} ({sum(tot.values())} sites)')


def cmd_spell_base(c, a):
    """Spell plain section-base sites with GL_DATA/GL_RODATA/GL_BSS(K), or with
    GL_DATA_STR/GL_RODATA_STR(K, "text") when K starts a string. Proof rule (as convert):
    every D_00000000-relocated LO16 of the function with that K names the same section-base
    Sym. The expansion is the old expression plus parentheses, so it is byte-safe when no
    neighbouring operator binds tighter than `+` (checked on both sides)."""
    if a.owner_report:
        return owner_report(c, a)
    if not a.files:
        sys.exit('spell-base: give FILE.. (or --owner-report)')
    excl = re.compile(a.exclude) if a.exclude else None
    only = re.compile(a.only) if a.only else None
    total = collections.Counter()
    for path in a.files:
        src = open(path, errors='replace').read()
        edits = plan_spell_base(c, src, excl, only)
        if not edits:
            continue
        fs = sorted({e[3] for e in edits})
        print(f'{path}: {len(edits)} sites in {len(fs)} functions')
        if a.verbose:
            for e in edits:
                print(f'    {e[3]}: {src[e[0]:e[1]]} -> {e[2]}')
        for e in edits:
            total[e[4]] += 1
        if a.write:
            for s0, s1, rep, _, _ in sorted(edits, reverse=True):
                src = src[:s0] + rep + src[s1:]
            src = add_gl_data_include(path, src)
            open(path, 'w').write(src)
    print(f'spell-base: {sum(total.values())} sites {dict(total)}' + ('' if a.write else ' (dry run; --write to apply)'))


DIRECT_RE = re.compile(r'\b(GL_DATA|GL_RODATA|GL_BSS|GL_TEXT)\s*\(\s*(0x[0-9A-Fa-f]+|\d+)\s*\)'
                       r'|\b(GL_DATA_STR|GL_RODATA_STR)\s*\(\s*(0x[0-9A-Fa-f]+|\d+)\s*,\s*("(?:[^"\\]|\\.)*")\s*\)')


# RoData constant pools (agent-a run 504): a header pool name `kRo_<Func>` (gl_data.h, defined as
# GL_RODATA(first K), comment `RoData+0xLO..0xHI`) = every non-string RoData-base K that <Func> reads;
# a per-site alias `kRo_<Func>_<K>` = one constant of it.
RO_ALIAS_RE = re.compile(r'^kRo_(\w+)_([0-9A-F]{2,6})$')
RO_POOL_RE = re.compile(r'RoData\+0x([0-9A-Fa-f]+)\.\.0x([0-9A-Fa-f]+)')


def ro_pool_ks(c, func):
    """The non-string RoData-base K values that `func` reads (its constant pool)."""
    return sorted({s.k for s in c.by_func.get(func, []) if s.rtype == 2 and s.sym.is_base
                   and s.sym.sect == 7 and rom_string(c, 7, s.k) is None})


STR_ALIAS_RE = re.compile(r'^s(Str|Fmt|Tag)_(?:(\w*?)_)?([0-9A-F]{3,6})(?:_\d+)?$')


def check_direct(c, bodies, verbose=False):
    """Direct GL_*(K) / GL_*_STR(K, "text") uses inside function bodies."""
    ok = bad = 0
    for p, fb in bodies.items():
        for f, b in fb.items():
            for mm in DIRECT_RE.finditer(b):
                if mm.group(1):
                    sect, k, lit = BASE_MACRO[mm.group(1)], int(mm.group(2), 0), None
                else:
                    sect, k, lit = STR_MACRO[mm.group(3)], int(mm.group(4), 0), mm.group(5)
                ss = [s for s in c.by_func.get(f, []) if s.rtype == 2]
                if not ss:
                    continue
                why = None
                if not claim_hits(c, f, ('base', sect, k)):
                    why = f'no {SECT_NAME[sect]}-base site with K=0x{k:X}'
                elif lit is not None and rom_string(c, sect, k) != c_unliteral(lit):
                    why = f'ROM string is {rom_string(c, sect, k)!r}'
                if why:
                    bad += 1
                    print(f'MISMATCH {mm.group(0)[:60]} in {f} ({p}): {why}')
                else:
                    ok += 1
    return ok, bad


def cmd_check(c, a):
    if c.uso != 'bootup':   # game.uso or a mode USO: only its own header (run 593: a mode USO ran
        return 1 if check_game(a.no_build, a.verbose, c.uso) else 0   # the bootup checks on its relocs)
    macros = scan_macros()
    claimed = [m for m in macros if m['claim']]
    srcs = [p for d in ('game_libs', 'bootup_uso') for e in ('c', 'cpp')
            for p in sorted(glob.glob(f'src/{d}/*.{e}'))] + import_srcs('bootup')
    bodies = {p: func_bodies(p) for p in srcs}
    bad = ok = unused = 0
    for m in claimed:
        pat = re.compile(r'\b%s\b' % re.escape(m['name']))
        files = [m['path']] if m['local'] else srcs
        users = [(p, f) for p in files for f, b in bodies.get(p, {}).items() if pat.search(b)]
        if m['local']:
            # a local macro is in scope from its #define to its #undef (or the end of the file)
            src = open(m['path'], errors='replace').read()
            d0 = sum(len(l) + 1 for l in src.split('\n')[:m['line'] - 1])
            um = re.compile(r'^[ \t]*#[ \t]*undef[ \t]+%s\b' % re.escape(m['name']), re.M).search(src, d0)
            d1 = um.start() if um else len(src)
            live = {f for f, f0, f1 in func_spans(src) if f0 < d1 and f1 > d0}
            users = [(p, f) for p, f in users if f in live]
        if not users:
            unused += 1
            if a.verbose:
                print(f'unused   {m["name"]} ({m["path"]}:{m["line"]})')
            continue
        for p, f in users:
            ss = [s for s in c.by_func.get(f, []) if s.rtype == 2]
            if not ss:
                continue   # not a game_libs function (name not in the map), or no sites
            cl = m['claim']
            if claim_hits(c, f, cl):
                ok += 1
                if a.verbose:
                    print(f'ok       {m["name"]} in {f}')
                continue
            bad += 1
            if cl[0] == 'sym':
                want = c.syms[cl[1]].label() if cl[1] < len(c.syms) else f'sym{cl[1]}?'
            else:
                want = f'{SECT_NAME[cl[1]]}-base K=0x{cl[2]:X}'
            near = sorted({c.obj_label(s.obj) for s in ss})[:6]
            print(f'MISMATCH {m["name"]} ({m["path"]}:{m["line"]}) claims {want}; '
                  f'{f} has no such site. Its objects: {near}')
    # per-site zero aliases renamed after an object (`gAudioContext_345c0`, `MNode_vtbl_546E8_a`):
    # the .o reloc names the symbol directly, so every LO16 site spelled with one must resolve
    # to the alias's Sym (or a section-base site inside its span).
    aliases = header_aliases(macros)
    ps_ok = ps_bad = 0
    ps_names = set()
    for s in c.sites:
        if s.rtype != 2 or not s.placeholder or s.placeholder.startswith('D_00000000'):
            continue
        hit = alias_of(aliases, s.placeholder)
        if not hit:
            continue
        ps_names.add(s.placeholder)
        n = hit[1][1]
        if s.sym.idx == n or s.obj in (('sym', n), ('via', n)):
            ps_ok += 1
            continue
        ps_bad += 1
        print(f'MISMATCH per-site alias {s.placeholder} (named after {hit[0]} = sym{n}) in {s.func} '
              f'T+0x{s.off:05X}: the reloc names {c.obj_label(s.obj)}')
    # per-site aliases named after a section-base STRING (`sStr_rom_gui_uso_C5E4`, `sTag_xps_CC74`,
    # `sFmt_Dup_parm_1E6B0`, agent-a run 498): every LO16 site must be a Data/RoData base site with K =
    # the hex suffix, and an sTag_ name must spell the ROM string exactly.
    str_ok = str_bad = 0
    for s in c.sites:
        if s.rtype != 2 or not s.placeholder:
            continue
        sm = STR_ALIAS_RE.match(s.placeholder)
        if not sm:
            continue
        k = int(sm.group(3), 16)
        t = rom_string(c, s.sym.sect, s.k) if s.sym.is_base else None
        if s.sym.is_base and s.sym.sect in (6, 7) and s.k == k and t is not None and \
                (sm.group(1) != 'Tag' or sm.group(2) == t):
            str_ok += 1
            continue
        str_bad += 1
        print(f'MISMATCH string alias {s.placeholder} in {s.func} T+0x{s.off:05X}: the reloc names '
              f'{c.obj_label(s.obj)} K=0x{s.k:X} ({t!r})')
    print(f'check: {str_ok} LO16 sites on sStr_/sFmt_/sTag_ string aliases verified, {str_bad} mismatches')
    # RoData pools: header `kRo_<Func>` spans exactly <Func>'s RoData constants; per-site
    # `kRo_<Func>_<K>` aliases are RoData-base sites with that K, inside <Func>.
    pools = {}
    for m in macros:
        if m['local'] or not m['name'].startswith('kRo_') or m['params'] or not m['claim']:
            continue
        txt = open(m['path'], errors='replace').read().split('\n')[m['line'] - 1]
        pm = RO_POOL_RE.search(txt)
        f = m['name'][4:]
        ks = ro_pool_ks(c, f)
        lo_hi = (int(pm.group(1), 16), int(pm.group(2), 16)) if pm else None
        if not pm or not ks or m['claim'] != ('base', 7, ks[0]) or lo_hi != (ks[0], ks[-1]):
            str_bad += 1
            print(f'MISMATCH RoData pool {m["name"]}: comment says {lo_hi}, {f} reads '
                  f'{[hex(k) for k in ks]}')
        pools[f] = lo_hi
    ro_ok = 0
    for s in c.sites:
        if s.rtype != 2 or not s.placeholder:
            continue
        rm = RO_ALIAS_RE.match(s.placeholder)
        if not rm:
            continue
        f, k = rm.group(1), int(rm.group(2), 16)
        lo_hi = pools.get(f)
        if s.sym.is_base and s.sym.sect == 7 and s.k == k and s.func == f and \
                (lo_hi is None or lo_hi[0] <= k <= lo_hi[1]):
            ro_ok += 1
            continue
        str_bad += 1
        print(f'MISMATCH RoData alias {s.placeholder} in {s.func} T+0x{s.off:05X}: the reloc names '
              f'{c.obj_label(s.obj)} K=0x{s.k:X}')
    print(f'check: {len(pools)} RoData pools (kRo_<Func>), {ro_ok} LO16 sites on kRo_<Func>_<K> aliases verified')
    d_ok, d_bad = check_direct(c, bodies, a.verbose)
    print(f'check: {d_ok} direct GL_DATA/GL_RODATA/GL_BSS(_STR) uses verified, {d_bad} mismatches')
    nvia = sum(1 for m in claimed if m.get('via'))
    print(f'check: {len(macros)} placeholder macros ({nvia} spelled over a header alias), '
          f'{len(claimed)} with a Sym/section claim; {ok} (macro, function) uses verified, '
          f'{bad} mismatches, {unused} claimed macros unused')
    print(f'check: {len(ps_names)} per-site aliases named after an object, {ps_ok} LO16 sites verified, '
          f'{ps_bad} mismatches')
    rb = check_plain_ratchet(c, srcs, a.update_ratchet)
    gb = 0
    if c.uso == 'bootup':
        # game.uso always; a mode USO once its constants header exists (agent-g run 589)
        for m in ['bootup'] + list(MODULES):
            if m == 'game' or os.path.exists(mod_hdr(m)):
                gb += check_game(a.no_build, a.verbose, m)
    return 1 if bad or ps_bad or d_bad or str_bad or rb or gb else 0


PLAIN_RATCHET = 'scripts/uso-data-sites.plain-base.txt'


def check_plain_ratchet(c, srcs, update=False):
    """Regression guard for spelled section-base sites (agent-a run 508). Every provable plain
    `(char *)&D_00000000 + K` site that `spell-base` would respell must be listed in PLAIN_RATCHET
    (the backlog). A plain site that is NOT listed is new: usually a merge that silently dropped a
    GL_DATA_STR / GL_DATA spelling back to the raw form (run 504 landing lost four MemTracker /
    DefMNode strings in post0b this way). Respell it with spell-base, or (for a deliberate new
    plain site) refresh the list with `check --update-ratchet`. Listed sites that are now spelled
    are only counted (shrink the list with --update-ratchet)."""
    cur = set()
    for p in srcs:
        src = open(p, errors='replace').read()
        for e in plan_spell_base(c, src):
            cur.add(f'{p}\t{e[3]}\t{src[e[0]:e[1]]}\t{e[4]}')
    if update:
        with open(PLAIN_RATCHET, 'w') as f:
            f.write('# provable plain section-base sites not yet spelled (uso-data-sites.py check '
                    '--update-ratchet)\n# file\tfunction\tsite\tmacro spell-base would use\n')
            f.write(''.join(x + '\n' for x in sorted(cur)))
        print(f'check: ratchet {PLAIN_RATCHET} rewritten ({len(cur)} plain sites)')
        return 0
    if not os.path.exists(PLAIN_RATCHET):
        print(f'check: no {PLAIN_RATCHET}; run check --update-ratchet')
        return 1
    allowed = {ln.rstrip('\n') for ln in open(PLAIN_RATCHET) if ln.strip() and not ln.startswith('#')}
    new = sorted(cur - allowed)
    for x in new:
        p, f, site, mac = x.split('\t')
        print(f'MISMATCH plain section-base site in {p} {f}: {site} is provably {mac} but spelled raw '
              f'(dropped by a merge?): run spell-base {p} --only \'^{f}$\' --write')
    print(f'check: {len(cur)} provable plain base sites, {len(new)} not in the ratchet, '
          f'{len(allowed - cur)} ratchet entries now spelled')
    return len(new)


def cmd_selftest(c, a):
    lo = [s for s in c.sites if s.rtype == 2]
    hi = [s for s in c.sites if s.rtype == 3]
    n_bss_obj = sum(1 for s in hi if s.sym.kind == 2 and s.sym.sect == 8)
    n_data_obj = sum(1 for s in hi if s.sym.kind == 2 and s.sym.sect == 6)
    print(f'HI16 {len(hi)}  LO16 {len(lo)}  HI16->Bss objects {n_bss_obj}  ->Data objects {n_data_obj}')
    assert n_bss_obj == 1950 and n_data_obj == 778, 'USO_FORMAT.md census changed?'
    assert c.syms[2].kind == 4 and c.syms[2].sect == 6 and c.syms[4].sect == 8
    # DMA prototype: gDmaSlots = Bss object sym 1504, dma_* strings at Data-base K 0x1E2C0..
    d = [s for s in c.by_func.get('dma_startRead', []) if s.rtype == 2]
    assert any(s.sym.idx == 1504 for s in d), d
    assert any(s.sym.is_base and s.sym.sect == 6 and s.k == 0x1E2C0 for s in d)
    ph = collections.Counter(s.placeholder for s in lo)
    print('placeholder names on LO16 sites:', ph.most_common(8))
    nofunc = sum(1 for s in lo if s.func == '?')
    assert nofunc == 0
    # the checker must accept the real claims and reject swapped ones
    assert claim_hits(c, 'dma_startRead', ('sym', 1504))
    assert claim_hits(c, 'dma_startRead', ('base', 6, 0x1E2C0))
    assert not claim_hits(c, 'dma_startRead', ('base', 8, 0x1E2C0)), 'Bss-base K accepted for a Data string'
    assert not claim_hits(c, 'dma_startRead', ('sym', 1255)), 'audio heap accepted for the DMA table'
    assert not claim_hits(c, 'dma_startRead', ('base', 6, 0x1E2C4))
    assert claim_hits(c, 'heap_tempAllocDefault_000333F4', ('sym', 73))
    assert not claim_hits(c, 'heap_tempAllocDefault_000333F4', ('sym', 338))
    # brace-matched spans: a one-line function must not swallow its successor
    sp = func_spans('int a(int x) { return f(&D_00000000, x); }\nvoid b(void) {\n    if (1) { g("}"); }\n}\n')
    assert [n for n, _, _ in sp] == ['a', 'b'], sp
    # per-site resolution: every Bss-object site in dma_startRead names sym 1504 with K < 0x44*10
    for s_ in c.by_func['dma_startRead']:
        if s_.rtype == 2 and s_.sym.kind == 2:
            assert s_.sym.idx == 1504 and s_.k < 0x2A8, (s_.sym.idx, hex(s_.k))
    # claims inherited through header aliases (run 491)
    al = {'MNode_vtbl': ('sym', 315), 'MNode_vtbl_546E8': ('sym', 9)}
    assert alias_of(al, 'MNode_vtbl') == ('MNode_vtbl', ('sym', 315))
    assert alias_of(al, 'MNode_vtbl_546E8_a') == ('MNode_vtbl_546E8', ('sym', 9))
    assert alias_of(al, 'MNode_vtblx') is None
    ms = {(m['path'], m['name']): m for m in scan_macros()}
    ac = [m for m in ms.values() if m.get('via') == 'gAudioContext']
    assert ac and all(m['claim'] == ('sym', 1255) for m in ac), ac[:2]
    # convert's include placement (.cpp donors get the relative path)
    t = add_gl_data_include('x.cpp', 'typedef int s32;\nextern "C" {\nextern char D_00000000;\nint f(void);\n')
    assert t.split('\n')[3].startswith('#include "../../include/gdl/gl_data.h"'), t
    assert add_gl_data_include('x.cpp', t) == t
    t = add_gl_data_include('x.c', '#include "common.h"\nint f(void);\n')
    assert t.split('\n')[1] == '#include "gdl/gl_data.h"', t
    print('selftest OK')
    if c.uso == 'bootup':
        selftest_game(a)
        selftest_modes(a)


# Text section sizes of the mode USOs (the Makefile's YAY0_TEXT_SIZE for the Yay0 blocks)
MODE_TEXT_SIZE = {'titproc': 0x2A20, 'eddproc': 0x480, 'mgrproc': 0x3420, 'arcproc': 0x2980,
                  'h2hproc': 0x1B30, 'timproc': 0x2EE0, 'trkproc': 0x30E0, 'genproc': 0xE620}


def selftest_modes(a):
    """Mode USO invariants (agent-g run 589): each module's Text size, and every LO16 site lands
    in a function of the module's built objects."""
    for m, n in MODE_TEXT_SIZE.items():
        g = Census('.', a.no_build, m)
        assert len(g.text) == n, (m, hex(len(g.text)))
        lo = [s for s in g.sites if s.rtype == 2]
        assert lo and not [s for s in lo if s.func == '?'], m
        assert sum(1 for s in lo if s.placeholder) * 10 >= len(lo) * 8, m   # objects line up
    print(f'selftest mode USOs OK ({len(MODE_TEXT_SIZE)} modules)')


def selftest_game(a):
    """game.uso invariants (agent-g run 538): base Syms, the Text object, the RiderAnim PMF table."""
    g = Census('.', a.no_build, 'game')
    assert g.base == {7: 2, 6: 3, 8: 4, 5: 1}, g.base
    assert len(g.text) == 0x11B30 and len(g.secs['Data']) == 0x1310
    lo = [s for s in g.sites if s.rtype == 2]
    print(f'game.uso: HI16 {sum(1 for s in g.sites if s.rtype == 3)}  LO16 {len(lo)}')
    assert not [s for s in lo if s.func == '?']
    assert game_ptr_target(g, 6, 0xE44) == 'RiderAnim_stateRide'
    assert game_ptr_target(g, 6, 0xDCC) == 'null'
    assert game_ptr_target(g, 6, 0xEC4) == 'RiderAnim_stateTrickWindup'
    # RiderAnim_stateTrickStart queues the windup PMF through a Data-base site K = 0xEC0
    assert any(s.sym.is_base and s.sym.sect == 6 and s.k == 0xEC0 for s in g.by_func['RiderAnim_stateTrickStart'])
    m = GSITE_RE.search('AT(PM, &game_uso_D_807FF4B0, 0xEC0)')
    assert m and m.group(1) == 'game_uso_D_807FF4B0' and m.group(3) == '0xEC0'
    assert [n for n, _, _ in game_spans('extern "C" int f(char *a) {\n    return 0;\n}\n')] == ['f']
    m = GSITE_RE.search('*(float*)((char*)&D_44F4_iterA + 0xA0)')
    assert m and m.group(1) == 'D_44F4_iterA' and m.group(3) == '0xA0'
    m = GSITE_RE.search('*(Pair2*)((char*)&D_00000000 + 0xE40)')
    assert m and m.group(1) == 'D_00000000' and m.group(3) == '0xE40'
    assert value_claim_error(g, 6, 0xD54, ': "Parms"') is None
    assert value_claim_error(g, 6, 0xD54, ': "Parm"') is not None
    assert value_claim_error(g, 7, 0x158, ': f64 0.1') is None
    assert value_claim_error(g, 7, 0x158, ': f64 0.2') is not None
    assert const_name(g, 6, 0x320, 'str') == 'kTag_cof_320'
    assert SITE_TYPE_RE.search('if (AT(double, ') and SITE_TYPE_RE.search('*(f64 *)((char *)')
    print('selftest game.uso OK')


# ---------------------------------------------------------------- game.uso (agent-g run 538)
# game_uso sites keep their per-address zero symbol (`&game_uso_D_807FF430 + 0xE40`, or a plain
# `&D_00000000 + K`): the symbol is part of the matched spelling. What gets a NAME is the
# section offset K: include/game/game_uso_data.h defines
#     #define kPmf_RiderAnim_stateRide 0xE40 /* Data+0xE40: PMF -> RiderAnim_stateRide */
# and the site reads `AT(PM, &game_uso_D_807FF430, kPmf_RiderAnim_stateRide)` (an integer
# constant: byte-safe by construction). `check` verifies every such constant against the relocs
# (each function using it has a section-base LO16 site of that section with K = the value) and
# a `PMF -> FUNC` / `ptr -> FUNC` comment against the DataReloc at K + 4 / K + 0 (`-> null`:
# no reloc and a zero word).
GAME_HDR = 'include/game/game_uso_data.h'
GAME_HDR_REL = '../../include/game/game_uso_data.h'
GCONST_RE = re.compile(r'^\s*#\s*define\s+(k\w+)\s+(0x[0-9A-Fa-f]+)\s*/\*\s*(Data|RoData|Bss)\+0x([0-9A-Fa-f]+)\b'
                       r'([^*]*(?:\*(?!/)[^*]*)*)\*/', re.M)
GSECT = {'Data': 6, 'RoData': 7, 'Bss': 8}
# a site literal K next to a zero placeholder: `&S + K`, `(char *)&S + K`, `(&S) + K`, `AT(T, &S, K)`
# (agent-g run 551: also a per-site zero alias `D_<tag>_<name>` such as SnowAI_ctor's D_44F4_iterA; the
# relocs still decide: a site counts only when the .o's placeholder for it IS that token and K matches)
# (agent-g run 589, mode USOs: `<seg>_uso[_bN]_D_<addr>` per-address symbols, `D_<tag>_<name>` aliases,
# `import_<addr>`)
GSITE_RE = re.compile(r'&\s*\(?\s*([A-Za-z]\w*_uso(?:_b\d)?_D_[0-9A-Fa-f]+|D_\w+|import_[0-9A-Fa-f]+)\s*\)?\s*\)?\s*([+,])\s*(0x[0-9A-Fa-f]+|\d+)\b')


def game_spans(src):
    """func_spans that also sees NCC donor definitions (`extern "C" int f(...) {`): the linkage
    prefix is blanked to a same-length word so offsets stay valid."""
    return func_spans(re.sub(r'^extern "C" (?=[\w*])', 'extern_C_x ', src, flags=re.M))


def game_bodies(path):
    src = open(path, errors='replace').read()
    out = {}
    for name, a, b in game_spans(src):
        out.setdefault(name, src[a:b])
    return out


BOOTUP_HDR = 'include/game/bootup_uso_data.h'


def mod_hdr(uso):
    """The section-offset constants header of a MODULES entry (game: GAME_HDR; bootup: BOOTUP_HDR,
    run 593: named offsets read through a local base pointer, where no GL_DATA spelling fits)."""
    return BOOTUP_HDR if uso == 'bootup' else MODULES[uso][3]


def game_consts(uso='game'):
    """[(name, value, sect, comment, line)] from the module's header (GAME_HDR for game.uso)."""
    hdr = mod_hdr(uso)
    if not os.path.exists(hdr):
        return []
    src = open(hdr).read()
    out = []
    for m in GCONST_RE.finditer(src):
        out.append((m.group(1), int(m.group(2), 16), GSECT[m.group(3)], int(m.group(4), 16),
                    m.group(5), src.count('\n', 0, m.start()) + 1))
    return out


def game_ptr_target(c, sect, k):
    """(section-relative) name of the Text function the R_MIPS_32 reloc at sect+k points at,
    'null' when there is no reloc there and the word is 0, else None."""
    secname = {6: 'DataReloc', 7: 'RoDataReloc'}.get(sect)
    for i in range(len(c.secs.get(secname, b'')) // 12):
        info, off, _ = struct.unpack_from('>III', c.secs[secname], i * 12)
        if off == k and info & 7 == 4:
            t = c.syms[info >> 4]
            return t.name or t.label()
    d = c.secs.get(SECT_NAME[sect], b'')
    return 'null' if k + 4 <= len(d) and u32(d, k) == 0 else None


def check_game(no_build=False, verbose=False, uso='game'):
    """game.uso section-offset constants (GAME_HDR) and their uses in src/game_uso; with `uso`, the
    same for a mode USO's header and sources."""
    consts = game_consts(uso)
    GAME_HDR = mod_hdr(uso)
    if not consts:
        print(f'check: {uso}.uso: no named constants')
        return 0
    g = Census('.', no_build, uso)
    bad = ok = tgt = 0
    srcs = mod_srcs(uso) + import_srcs(uso)
    bodies = {p: game_bodies(p) for p in srcs}
    for name, val, sect, coff, comment, line in consts:
        if val != coff:
            bad += 1
            print(f'MISMATCH {name} ({GAME_HDR}:{line}) = 0x{val:X} but its comment says +0x{coff:X}')
            continue
        vbad = value_claim_error(g, sect, val, comment)
        if vbad:
            bad += 1
            print(f'MISMATCH {name} ({GAME_HDR}:{line}): {vbad}')
            continue
        if VALUE_CLAIM_RE.search(comment):
            tgt += 1
        pm = re.search(r'\b(PMF|ptr)\s*->\s*(\w+)', comment)
        if pm:
            got = game_ptr_target(g, sect, val + (4 if pm.group(1) == 'PMF' else 0))
            if got != pm.group(2):
                bad += 1
                print(f'MISMATCH {name} ({GAME_HDR}:{line}): {pm.group(1)} -> {pm.group(2)}, '
                      f'the ROM has {got}')
            else:
                tgt += 1
        pat = re.compile(r'\b%s\b' % re.escape(name))
        for p, fb in bodies.items():
            for f, b in fb.items():
                if not pat.search(b):
                    continue
                ss = [x for x in g.by_func.get(f, []) if x.rtype == 2]
                if any(x.sym.is_base and x.sym.sect == sect and x.k == val for x in ss):
                    ok += 1
                    if verbose:
                        print(f'ok       {name} in {f}')
                    continue
                bad += 1
                print(f'MISMATCH {name} in {f} ({p}): no {SECT_NAME[sect]}-base site with K=0x{val:X}')
    print(f'check: {uso}.uso: {len(consts)} named section offsets ({GAME_HDR}), {tgt} pointer targets / '
          f'values verified, {ok} (constant, function) uses verified, {bad} mismatches')
    return bad


def plan_game_const(c, src, consts):
    """(start, end, name, func) of literal site offsets in `src` that a GAME_HDR constant names:
    the literal follows a zero placeholder S, and every LO16 site of the function with placeholder S
    and that K is a section-base site of the constant's section."""
    byval = collections.defaultdict(list)
    for name, val, sect, *_ in consts:
        byval[val].append((name, sect))
    edits = []
    for f, b0, b1 in game_spans(src):
        ss = [x for x in c.by_func.get(f, []) if x.rtype == 2]
        if not ss:
            continue
        for mm in GSITE_RE.finditer(src, b0, b1):
            k = int(mm.group(3), 0)
            if k not in byval:
                continue
            hit = [x for x in ss if x.placeholder == mm.group(1) and x.k == k]
            secs_ = {x.sym.sect for x in hit if x.sym.is_base}
            if not hit or len(secs_) != 1 or any(not x.sym.is_base for x in hit):
                continue
            names = [n for n, sct in byval[k] if sct in secs_]
            if len(names) == 1:
                edits.append((mm.start(3), mm.end(3), names[0], f))
    return edits


def cmd_game_spell(c, a):
    """Respell the literal K of game_uso sites with their GAME_HDR constant (`--uso game spell`)."""
    consts = game_consts(c.uso)
    hdr = mod_hdr(c.uso)
    total = 0
    for path in a.files:
        src = open(path, errors='replace').read()
        edits = plan_game_const(c, src, consts)
        if not edits:
            continue
        fs = sorted({e[3] for e in edits})
        print(f'{path}: {len(edits)} sites in {len(fs)} functions')
        if a.verbose:
            for e in edits:
                print(f'  {e[3]}: {src[e[0]:e[1]]} -> {e[2]}')
        total += len(edits)
        if a.write:
            for s0, s1, n, _ in sorted(edits, reverse=True):
                src = src[:s0] + n + src[s1:]
            inc = hdr[len('include/'):]
            if f'{inc}"' not in src:
                if path.endswith('.cpp'):
                    ms = list(re.finditer(r'^#include .*\n', src, re.M))
                    i = ms[-1].end() if ms else 0
                    src = src[:i] + f'#include "../../{hdr}"\n' + src[i:]
                else:
                    i = src.find('#include "common.h"')
                    i = src.find('\n', i) + 1 if i >= 0 else 0
                    src = src[:i] + f'#include "{inc}"\n' + src[i:]
            open(path, 'w').write(src)
    print(f'{total} sites' + ('' if a.write else ' (dry run; --write to apply)'))


# value claims in a GAME_HDR comment (agent-g run 543), checked against the section bytes at K:
#   "text"      the NUL-terminated string starting at K (rom_string)
#   f32 V / f64 V   the big-endian float / double at K, printed with repr()
VALUE_CLAIM_RE = re.compile(r':\s*("(?:[^"\\]|\\.)*"|f32 [^\s,;:]+|f64 [^\s,;:]+)')   # run 593: `f32 V, text`


def scalar_at(c, sect, k, typ):
    d = c.secs.get(SECT_NAME.get(sect), b'')
    n = 4 if typ == 'f32' else 8
    if k % n or k + n > len(d):
        return None
    return struct.unpack_from('>f' if typ == 'f32' else '>d', d, k)[0]


def fmt_scalar(typ, v):
    if typ == 'f32':
        # shortest repr that rounds back to the same single
        for p in range(1, 18):
            t = '%.*g' % (p, v)
            try:
                if struct.pack('>f', float(t)) == struct.pack('>f', v):
                    return repr(float(t))
            except OverflowError:  # rounds past FLT_MAX
                continue
    return repr(v)


def value_claim_error(c, sect, k, comment):
    """None when the comment's value claim (if any) matches the section bytes at k, else why not."""
    m = VALUE_CLAIM_RE.search(comment)
    if not m:
        return None
    cl = m.group(1)
    if cl.startswith('"'):
        got = rom_string(c, sect, k)
        want = c_unliteral(cl)
        return None if got == want else f'claims {cl}, the ROM has {c_literal(got) if got else "no string"}'
    typ, txt = cl.split(' ', 1)
    v = scalar_at(c, sect, k, typ)
    if v is None:
        return f'claims {cl}, but +0x{k:X} is not an aligned {typ}'
    want = struct.pack('>f' if typ == 'f32' else '>d', float(txt))
    have = struct.pack('>f' if typ == 'f32' else '>d', v)
    return None if want == have else f'claims {cl}, the ROM has {typ} {fmt_scalar(typ, v)}'


TAG_RE = re.compile(r'[a-z][a-z0-9]{2}')
# the C type a site reads its K as: a cast / AT() type within the 48 characters before the placeholder
SITE_TYPE_RE = re.compile(r'(?:\(\s*(f64|double|f32|float)\s*\*\s*\)|\bAT\s*\(\s*(f64|double|f32|float)\s*,)[^;]{0,48}$')


def const_name(c, sect, k, typ):
    if typ == 'str':
        t = rom_string(c, sect, k)
        if TAG_RE.fullmatch(t):
            return f'kTag_{t}_{k:X}'
        ident = re.sub(r'_+', '_', re.sub(r'[^A-Za-z0-9]', '_', t)).strip('_')[:32].rstrip('_')
        return f'kStr_{ident}_{k:X}'
    v = scalar_at(c, sect, k, typ)
    vt = fmt_scalar(typ, v)
    vt = 'FLT_MAX' if typ == 'f32' and vt == '3.4028234663852886e+38' or vt == '3.4028235e+38' else vt
    vt = 'mFLT_MAX' if typ == 'f32' and vt == '-3.4028235e+38' else vt
    vt = re.sub(r'\.0$', '', re.sub(r'\.0(?=e)', '', vt)).replace('-', 'm').replace('.', 'p').replace('+', '')
    vt = re.sub(r'[^A-Za-z0-9]', '_', vt)
    return f'k{"Ro" if sect == 7 else "Data"}{"F" if typ == "f32" else "D"}_{vt}_{k:X}'


def plan_const_table(c):
    """{(sect, k): (typ, [func..])} for every literal game_uso site K that no GAME_HDR constant names
    yet and whose relocs prove ONE section base: a string start (Data / RoData), or a scalar whose
    site casts it as float / double (the cast within the statement just before the placeholder)."""
    have = {(sect, val) for _, val, sect, *_ in game_consts(c.uso)}
    out = {}
    srcs = mod_srcs(c.uso)
    for path in srcs:
        src = open(path, errors='replace').read()
        for f, b0, b1 in game_spans(src):
            ss = [x for x in c.by_func.get(f, []) if x.rtype == 2]
            for mm in GSITE_RE.finditer(src, b0, b1):
                k = int(mm.group(3), 0)
                hit = [x for x in ss if x.placeholder == mm.group(1) and x.k == k]
                secs_ = {x.sym.sect for x in hit if x.sym.is_base}
                if not hit or len(secs_) != 1 or any(not x.sym.is_base for x in hit):
                    continue
                sect = secs_.pop()
                if (sect, k) in have or sect not in (6, 7):
                    continue
                st = rom_string(c, sect, k)
                if st is not None and '*/' not in st and (TAG_RE.fullmatch(st) or len(st) > 3):
                    typ = 'str'
                else:
                    tm = SITE_TYPE_RE.search(src, max(b0, mm.start() - 64), mm.start())
                    if not tm:
                        continue
                    typ = {'double': 'f64', 'float': 'f32'}.get(tm.group(1) or tm.group(2), tm.group(1) or tm.group(2))
                    if scalar_at(c, sect, k, typ) is None:
                        continue
                e = out.setdefault((sect, k), (typ, []))
                if e[0] != typ:
                    out[(sect, k)] = ('conflict', e[1])
                elif f not in e[1]:
                    e[1].append(f)
    return {key: v for key, v in out.items() if v[0] != 'conflict'}


CONST_BEGIN = '/* const-table:begin (uso-data-sites.py --uso game const-table --write; do not edit by hand) */'
CONST_END = '/* const-table:end */'


def const_begin(uso):
    return CONST_BEGIN.replace('--uso game', f'--uso {uso}')


def cmd_const_table(c, a):
    """#define lines naming the game.uso strings, parm tags and float / double constants that the
    game_uso sites reach by a literal K (`--uso game const-table [--write]`). Each line carries its
    value claim ("text" / f32 V / f64 V), which `check` verifies against the ROM. --write replaces
    the generated block of GAME_HDR. `--uso MODE const-table` does the same for a mode USO's header
    (MODULES; --write creates it)."""
    GAME_HDR, CONST_BEGIN = mod_hdr(c.uso), const_begin(c.uso)
    plan = plan_const_table(c)
    groups = {'str': [], 'tag': [], 'num': []}
    for (sect, k), (typ, fs) in sorted(plan.items()):
        n = const_name(c, sect, k, typ)
        if typ == 'str':
            val = c_literal(rom_string(c, sect, k))
            grp = 'tag' if n.startswith('kTag_') else 'str'
        else:
            val = f'{typ} {fmt_scalar(typ, scalar_at(c, sect, k, typ))}'
            grp = 'num'
        groups[grp].append(f'#define {n} 0x{k:X} /* {SECT_NAME[sect]}+0x{k:X}: {val} */')
    # keep the lines an earlier --write generated (their sites are spelled by name now, so the plan
    # no longer sees them; agent-g run 551: a second --write used to drop them)
    src0 = open(GAME_HDR).read() if os.path.exists(GAME_HDR) else ''
    i0, j0 = src0.find(CONST_BEGIN), src0.find(CONST_END)
    if i0 >= 0 and j0 > i0:
        for l in src0[i0 + len(CONST_BEGIN):j0].splitlines():
            if not l.startswith('#define '):
                continue
            grp = 'tag' if l.split()[1].startswith('kTag_') else 'str' if l.split()[1].startswith('kStr_') else 'num'
            groups[grp].append(l)
    for g_ in groups.values():
        g_.sort(key=lambda l: (l.split('/*')[1].split('+')[0], int(l.split()[2], 0)))
    names = [l.split()[1] for g_ in groups.values() for l in g_]
    dup = [n for n, cnt in collections.Counter(names).items() if cnt > 1]
    if dup:
        print('duplicate names:', dup)
        return 1
    head = {'str': '/* strings (class names, "Parms" group names, assert texts, file names) */',
            'tag': '/* 3-letter parm tags (Parms_addParm / the parms-file keys) */',
            'num': '/* float / double constants (the type is the one the site reads) */'}
    body = [CONST_BEGIN]
    for gname in ('str', 'tag', 'num'):
        if groups[gname]:
            body += [head[gname]] + groups[gname]
    body.append(CONST_END)
    text = '\n'.join(body) + '\n'
    if not a.write:
        print(text, end='')
        print(f'{len(names)} constants (dry run; --write to update {GAME_HDR})')
        return 0
    if not os.path.exists(GAME_HDR):
        g_ = re.sub(r'\W', '_', os.path.basename(GAME_HDR)).upper()
        open(GAME_HDR, 'w').write(f'#ifndef {g_}\n#define {g_}\n\n/* {c.uso}.uso section offsets named by '
                                  f'scripts/uso-data-sites.py --uso {c.uso} (const-table / pmf-table); '
                                  f'`check` verifies each one against the relocs */\n\n#endif\n')
    src = open(GAME_HDR).read()
    i, j = src.find(CONST_BEGIN), src.find(CONST_END)
    if i >= 0 and j > i:
        src = src[:i] + text + src[j + len(CONST_END) + 1:]
    else:
        e = src.rfind('#endif')
        src = src[:e] + text + '\n' + src[e:]
    open(GAME_HDR, 'w').write(src)
    print(f'{len(names)} constants written to {GAME_HDR}')
    return 0


def cmd_pmf_table(c, a):
    """#define lines naming a table of 8-byte PMF records {s16 delta, s16 index, fn} by the
    function each one points at (`--uso game pmf-table 0xDB0 0xF60`)."""
    sect = GSECT[a.sect]
    for k in range(int(a.start, 0), int(a.end, 0), 8):
        t = game_ptr_target(c, sect, k + 4)
        if t is None:
            print(f'/* {a.sect}+0x{k:X}: not a PMF record */')
            continue
        n = f'kPmf_{t}' if t != 'null' else f'kPmfNull_{k:X}'
        print(f'#define {n} 0x{k:X} /* {a.sect}+0x{k:X}: PMF -> {t} */')


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--no-build', action='store_true',
                    help='never run make; only warn when build/tenshoe.map is older than the sources')
    ap.add_argument('--uso', choices=('bootup',) + tuple(MODULES), default='bootup',
                    help='module: bootup.uso (bootup_uso + game_libs, default), game.uso (src/game_uso) '
                         'or a mode USO (MODULES: timproc / trkproc / genproc = timproc_uso_b1 / b3 / b5)')
    sub = ap.add_subparsers(dest='cmd', required=True)
    p = sub.add_parser('sites'); p.add_argument('funcs', nargs='*')
    p = sub.add_parser('calls'); p.add_argument('funcs', nargs='+')
    p.add_argument('--to', action='store_true', help='list the callers of FUNCS instead of their callees')
    p = sub.add_parser('census'); p.add_argument('-n', type=int, default=50)
    p.add_argument('--direct', action='store_true')
    p = sub.add_parser('object'); p.add_argument('obj')
    p = sub.add_parser('check'); p.add_argument('-v', '--verbose', action='store_true')
    p.add_argument('--update-ratchet', action='store_true',
                   help=f'rewrite {PLAIN_RATCHET} from the current provable plain base sites')
    p = sub.add_parser('convert'); p.add_argument('name'); p.add_argument('files', nargs='+')
    p.add_argument('--exclude', help='regex of function names to leave alone (owned by another area)')
    p.add_argument('--only', help='regex (full match) of the only function names to touch')
    p.add_argument('--bare', action='store_true',
                   help='also respell bare value uses (`D_00000000 = x`) when every K = 0 site names the Sym')
    p.add_argument('--write', action='store_true')
    p = sub.add_parser('spell-base'); p.add_argument('files', nargs='*')
    p.add_argument('--exclude', help='regex of function names to leave alone (owned by another area)')
    p.add_argument('--only', help='regex (full match) of the only function names to touch')
    p.add_argument('--owner-report', metavar='OUT', nargs='?', const='-',
                   help='write, per owner area, the exact spell-base / convert commands that respell '
                        'every provable plain site (OUT, default stdout); FILE.. narrows the files')
    p.add_argument('-v', '--verbose', action='store_true')
    p.add_argument('--write', action='store_true')
    sub.add_parser('selftest')
    p = sub.add_parser('pmf-table'); p.add_argument('start'); p.add_argument('end')
    p.add_argument('--sect', default='Data', choices=('Data', 'RoData'))
    p = sub.add_parser('const-table'); p.add_argument('--write', action='store_true')
    p = sub.add_parser('spell'); p.add_argument('files', nargs='+')
    p.add_argument('-v', '--verbose', action='store_true')
    p.add_argument('--write', action='store_true')
    a = ap.parse_args()
    c = Census('.', a.no_build, a.uso)
    sys.exit({'sites': cmd_sites, 'calls': cmd_calls, 'census': cmd_census, 'object': cmd_object,
              'check': cmd_check, 'selftest': cmd_selftest, 'convert': cmd_convert,
              'spell-base': cmd_spell_base, 'pmf-table': cmd_pmf_table, 'spell': cmd_game_spell,
              'const-table': cmd_const_table}[a.cmd](c, a) or 0)


if __name__ == '__main__':
    main()
