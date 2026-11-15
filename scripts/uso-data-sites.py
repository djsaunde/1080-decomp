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
  selftest           invariants on the tables (counts from USO_FORMAT.md, the DMA prototype).

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
TYPES = {1: 'Sym', 2: 'TextReloc', 3: 'DataReloc', 4: 'RoDataReloc', 5: 'Text', 6: 'Data',
         7: 'RoData', 9: 'EntrySym', 11: 'End'}


def u32(b, o):
    return struct.unpack_from('>I', b, o)[0]


def parse_uso(rom, mod):
    """Section table walk (same layout as scripts/uso-sym-oracle.py). Returns name -> bytes."""
    import crunch64
    assert u32(rom, mod) == 0x12345678
    o = mod + 16
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


def load_map(path):
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
            delta = (int(m.group(2), 16) - TEXT_ROM - int(m.group(1), 16)) if m else None
            continue
        if cur not in SEGMENTS:
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


def ensure_build(no_build=False):
    """The census reads the built map + .o relocs, so they must be at least as new as the
    sources. A missing or stale build is refreshed with an incremental
    `make RUN_CC_CHECK=0 -j8` (the readability gate leaves a full build behind, so right
    after a gate this is a no-op; `--keep-build` is NOT needed). --no-build only warns."""
    srcs = [p for d in ('src/game_libs', 'src/bootup_uso') for e in ('c', 'cpp', 's')
            for p in glob.glob(f'{d}/*.{e}')]
    # headers are not make dependencies here, and the census only reads relocs (which header
    # aliases cannot change: they expand to the same tokens); `check` reads headers live.
    newest = max((os.path.getmtime(p) for p in srcs), default=0)
    have = os.path.exists(MAP)
    if have and os.path.getmtime(MAP) >= newest:
        return
    why = 'missing' if not have else 'older than the newest src file'
    if no_build:
        if not have:
            sys.exit(f'{MAP} {why}: build first (make RUN_CC_CHECK=0 -j8)')
        print(f'warning: {MAP} is {why}; results may be stale (--no-build)', file=sys.stderr)
        return
    import subprocess
    print(f'{MAP} {why}: running make RUN_CC_CHECK=0 -j8 ...', file=sys.stderr)
    env = dict(os.environ)
    if os.path.isdir(BUILD_VENV):
        env['PATH'] = BUILD_VENV + os.pathsep + env.get('PATH', '')
    r = subprocess.run(['make', 'RUN_CC_CHECK=0', '-j8'], env=env,
                       stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
    if r.returncode != 0:
        print('\n'.join(r.stdout.splitlines()[-15:]), file=sys.stderr)
        if not os.path.exists(MAP) or os.path.getmtime(MAP) < newest:
            sys.exit('make failed and the map is not current')
        print('warning: make failed after linking (ROM mismatch?); using the fresh map', file=sys.stderr)


class Census:
    def __init__(self, root='.', no_build=False):
        os.chdir(root)
        ensure_build(no_build)
        self.rom = open(ROM, 'rb').read()
        secs = parse_uso(self.rom, INNER_HDR)
        self.secs = secs
        sd = secs['Sym']
        self.syms = [Sym(i, *struct.unpack_from('>II', sd, i * 12)) for i in range(len(sd) // 12)]
        self.text = secs['Text']
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
        self.sect_size = {6: len(secs['Data']), 7: len(secs['RoData'])}
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
        for secname, base in (('DataReloc', self.syms[2]), ('RoDataReloc', self.syms[1])):
            dr = self.secs[secname]
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
    macros = scan_macros()
    claimed = [m for m in macros if m['claim']]
    srcs = [p for d in ('game_libs', 'bootup_uso') for e in ('c', 'cpp')
            for p in sorted(glob.glob(f'src/{d}/*.{e}'))]
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
    return 1 if bad or ps_bad or d_bad or str_bad or rb else 0


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


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--no-build', action='store_true',
                    help='never run make; only warn when build/tenshoe.map is older than the sources')
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
    a = ap.parse_args()
    c = Census('.', a.no_build)
    sys.exit({'sites': cmd_sites, 'calls': cmd_calls, 'census': cmd_census, 'object': cmd_object,
              'check': cmd_check, 'selftest': cmd_selftest, 'convert': cmd_convert,
              'spell-base': cmd_spell_base}[a.cmd](c, a) or 0)


if __name__ == '__main__':
    main()
