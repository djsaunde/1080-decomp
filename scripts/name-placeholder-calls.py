#!/usr/bin/env python3
"""Name USO placeholder calls: `gl_func_00000000(a)` -> `CALL(AnimPlayer_start)(a)`.

Every call through a USO import or self-relocation is spelled with a zero placeholder
(gl_func_00000000, func_00000000, game_uso_func_00000000, per-site aliases such as
gl_func_00000000_send ...). The real callee is the target of the module's TextReloc
R_MIPS_26 entry at that jal (resolved with scripts/uso-imports.py: imports go to the
exporting module's Sym, self-relocations to the module's own function). This tool puts
that name at the call site with the macros in include/uso_call.h, which expand to the
exact original token (so the bytes cannot change):

    CALL(f) -> gl_func_00000000   FCALL(f) -> func_00000000
    GCALL(f) -> game_uso_func_00000000   XCALL(tok, f) -> tok

Which source call is which jal (the ordering proof)
----------------------------------------------------
Source order is NOT trusted (nested calls, loop conditions and for-increments are
emitted out of order, while-tests are duplicated). Instead the tool compiles a verify
copy of the file in which call site #N is spelled with its own symbol `npc_uc_N`
(declared like the placeholder's nearest preceding declaration), through the same
asm-processor + cc pipeline as the build (CC / CFLAGS / OPT_FLAGS / CC_ONLY_FLAGS /
MIPSISET from the Makefile's target-specific variables). A function is used only when
  * its verify bytes are IDENTICAL to the function in the real build/.../X.c.o (so the
    real object was compiled from this C: REPLACE_FUNC_BODY donor splices and other
    recipes differ and are refused), and
  * every placeholder jal reloc in the real function is covered by some npc_uc_N
    (call count == reloc count; otherwise the whole function is refused).
Then site N's reloc offset(s) give the module Text offset(s), and the ROM TextReloc
there gives the callee. A site whose jals (an inlined/duplicated call) resolve to two
callees is refused.

Subcommands (run from the project root after a full build; re-execs under the repo .venv
when pyelftools / crunch64 are missing):
  plan FILE.. [--write] [-v]   name every provable placeholder call (dry run unless
                               --write; adds the uso_call.h include)
  check [FILE..] [--strict]    every CALL/FCALL/GCALL/XCALL(name) must name a ROM callee of
                               its function: per function and token, the multiset of names
                               must be contained in the multiset of ROM jal targets of the
                               real .o's relocs against that token (fast, no compile).
                               --strict re-derives every site with the verify compile and
                               requires the exact name. Default FILE.. = every src file
                               that uses the macros. Exit 1 on a mismatch.

Names come from the BUILD (build/tenshoe.elf + map, and the Yay0 block .lnk.o files for the
compressed modules). Run it after the readability gate's clean build: an incremental make can
leave a block .lnk.o with pre-rename names (run 508: SnowAI_func_00006FA8, renamed to
SnowAI_pickFinishBranch on main, was still in game_uso_block1_yay0.lnk.o), and `check` on the
clean build then flags the stale CALL name.

Modules: bootup (src/game_libs, src/bootup_uso), game (game_uso), genproc
(timproc_uso_b5), gui, titproc, eddproc, arcproc, h2hproc, mgrproc, timproc
(timproc_uso_b1), trkproc (timproc_uso_b3). n64proc / boarderN / kernel are not
resolvable (n64proc and the character USOs use the other module layout; the kernel has
no placeholders). NCC .cpp files are not compiled by the verify path yet (left).
"""
import argparse
import collections
import concurrent.futures
import importlib.util
import os
import re
import shutil
import subprocess
import sys

VENV = '/home/danjs/code/decomp/.venv'
try:
    import crunch64  # noqa: F401
    import elftools  # noqa: F401
except ImportError:
    if os.path.exists(VENV + '/bin/python') and not os.environ.get('NPC_REEXEC'):
        os.environ['NPC_REEXEC'] = '1'
        os.execv(VENV + '/bin/python', [VENV + '/bin/python'] + sys.argv)
    sys.exit('needs pyelftools + crunch64: run with /home/danjs/code/decomp/.venv/bin/python')

from elftools.elf.elffile import ELFFile

HERE = os.path.dirname(os.path.abspath(__file__))


def _load(name, fn):
    spec = importlib.util.spec_from_file_location(name, os.path.join(HERE, fn))
    m = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(m)
    return m


UI = _load('uso_imports', 'uso-imports.py')

MACRO_OF = {'gl_func_00000000': 'CALL', 'func_00000000': 'FCALL', 'game_uso_func_00000000': 'GCALL'}
TOK_OF = {v: k for k, v in MACRO_OF.items()}
PH_RE = re.compile(r'\b(\w*func_00000000\w*)\b')
MAC_RE = re.compile(r'\b(?:(CALL|FCALL|GCALL)\s*\(\s*(\w+)\s*\)|XCALL\s*\(\s*(\w+)\s*,\s*(\w+)\s*\))')
IDENT = re.compile(r'[A-Za-z_]\w*')
KEYWORDS = {'return', 'else', 'case', 'goto', 'do', 'sizeof', 'default'}
MODULE_OF_DIR = {'game_libs': 'bootup', 'bootup_uso': 'bootup', 'game_uso': 'game',
                 'timproc_uso_b5': 'genproc', 'gui_uso': 'gui', 'titproc_uso': 'titproc',
                 'eddproc_uso': 'eddproc', 'arcproc_uso': 'arcproc', 'h2hproc_uso': 'h2hproc',
                 'mgrproc_uso': 'mgrproc', 'timproc_uso_b1': 'timproc', 'timproc_uso_b3': 'trkproc'}
INCLUDE_LINE = '#include "uso_call.h"'
INCLUDE_REL = '../../include/uso_call.h'
SCRATCH = 'build/npc'


# ---------------------------------------------------------------- source scanning
def mask(src):
    """Same length as src; comments, string / char literals and preprocessor directive lines
    (with continuations) blanked to spaces (newlines kept)."""
    out = list(src)
    n, i = len(src), 0
    bol = True
    while i < n:
        ch = src[i]
        if bol:
            j = i
            while j < n and src[j] in ' \t':
                j += 1
            if j < n and src[j] == '#':
                while j < n and src[j] != '\n':
                    if src[j] == '\\' and j + 1 < n and src[j + 1] == '\n':
                        j += 2
                        continue
                    if src.startswith('/*', j):     # a block comment may run past the line
                        k = src.find('*/', j + 2)
                        j = n if k < 0 else k + 2
                        continue
                    j += 1
                for k in range(i, j):
                    if out[k] != '\n':
                        out[k] = ' '
                i = j
                continue
        bol = False
        if src.startswith('/*', i):
            j = src.find('*/', i + 2)
            j = n if j < 0 else j + 2
        elif src.startswith('//', i):
            j = src.find('\n', i)
            j = n if j < 0 else j
        elif ch in '"\'':
            j = i + 1
            while j < n and src[j] != ch and src[j] != '\n':
                j += 2 if src[j] == '\\' else 1
            j += 1
        else:
            if ch == '\n':
                bol = True
            i += 1
            continue
        for k in range(i, min(j, n)):
            if out[k] != '\n':
                out[k] = ' '
        i = j
    return ''.join(out)


def top_blocks(m):
    """[(boundary, open, close)] for brace blocks at file scope; boundary = index after the last
    file-scope ';' or '}' before the block."""
    res, depth, boundary, op = [], 0, 0, None
    for i, ch in enumerate(m):
        if ch == '{':
            if depth == 0:
                op = i
            depth += 1
        elif ch == '}':
            depth -= 1
            if depth == 0:
                res.append((boundary, op, i))
                boundary = i + 1
            depth = max(depth, 0)
        elif ch == ';' and depth == 0:
            boundary = i + 1
    return res


class Site:
    __slots__ = ('i', 'pos', 'end', 'tok', 'name', 'func', 'block')

    def __init__(self, i, pos, end, tok, name):
        self.i, self.pos, self.end, self.tok, self.name = i, pos, end, tok, name
        self.func = self.block = None


def is_decl(m, pos):
    start = max(m.rfind(';', 0, pos), m.rfind('{', 0, pos), m.rfind('}', 0, pos)) + 1
    pre = ' '.join(m[start:pos].split())
    if not pre:
        return False
    if re.match(r'(extern|static|typedef)\b', pre):
        return True
    w = re.fullmatch(r'(?:(?:const|volatile|unsigned|signed|struct|union|enum)\s+)*([A-Za-z_]\w*)\s*\**', pre)
    return bool(w) and w.group(1) not in KEYWORDS


def func_header_name(m, boundary, op):
    h = m[boundary:op]
    if '=' in h:
        return None
    mm = re.search(r'([A-Za-z_]\w*)\s*\(', h)
    return mm.group(1) if mm and ')' in h else None


def scan(src, m=None):
    """Call sites: raw placeholder tokens (name None) and macro calls (name set), inside
    file-scope function blocks, in source order."""
    m = m if m is not None else mask(src)
    blocks = [b for b in top_blocks(m) if func_header_name(m, b[0], b[1])]
    sites = []
    for mt in PH_RE.finditer(m):
        if is_decl(m, mt.start()):
            continue
        sites.append((mt.start(), mt.end(), mt.group(1), None))
    for mt in MAC_RE.finditer(m):
        tok = TOK_OF[mt.group(1)] if mt.group(1) else mt.group(3)
        sites.append((mt.start(), mt.end(), tok, mt.group(2) or mt.group(4)))
    sites.sort()
    out = []
    for pos, end, tok, name in sites:
        blk = next((b for b in blocks if b[1] < pos < b[2]), None)
        if blk is None:
            continue
        s = Site(len(out), pos, end, tok, name)
        s.block = blk
        s.func = func_header_name(m, blk[0], blk[1])
        out.append(s)
    return out, m


_DECLS = {}


def decl_for(src, m, tok, pos, name):
    """`extern <type> name(<params>); ` copied from tok's nearest preceding declaration
    (default `extern int name();`, the K&R form most files use)."""
    key = (id(m), tok)
    if key not in _DECLS:
        lst = []
        for d in re.finditer(r'(?:(?<=[;{}])|^)[ \t\n]*((?:[A-Za-z_]\w*[ \t\*]+)+?)\**' + re.escape(tok) +
                             r'\s*\(([^;{}]*?)\)\s*;', m, re.M):
            head = d.group(1).split()
            if head and head[0] in KEYWORDS:
                continue
            full = ' '.join(m[d.start(1):d.end()].split())
            lst.append((d.start(), re.sub(r'^(extern|static)\s+', '', full)))
        _DECLS[key] = lst
    best = None
    for p0, full in _DECLS[key]:
        if p0 >= pos:
            break
        best = full
    if best is None:
        return f'extern int {name}(); '
    return 'extern ' + re.sub(r'\b' + re.escape(tok) + r'\b', name, best, count=1) + ' '


def strip_static_funcs(m, edits):
    """File-scope `static` on function declarations / definitions: IDO puts static functions
    in .mdebug only (asm-processor's post-process makes the ELF symbols), so the verify object
    would have no symbol for them. Dropping the keyword gives them one; the bytes are compared
    with the real object anyway."""
    depth = 0
    stmt = 0
    for i, ch in enumerate(m):
        if ch == '{':
            if depth == 0:
                _strip(m, stmt, i, edits)
            depth += 1
        elif ch == '}':
            depth = max(depth - 1, 0)
            if depth == 0:
                stmt = i + 1
        elif ch == ';' and depth == 0:
            _strip(m, stmt, i, edits)
            stmt = i + 1


def _strip(m, a, b, edits):
    t = m[a:b]
    mm = re.match(r'\s*static\b', t)
    if mm and '(' in t and ('=' not in t or t.index('(') < t.index('=')):
        p = a + mm.end() - 6
        edits.append((p, p + 6, '      '))


def insert_point(m, boundary):
    i = boundary
    while i < len(m) and m[i] in ' \t\n\r':
        i += 1
    return i


def verify_text(src, m, sites):
    edits = []      # (pos, end, text)
    by_block = collections.defaultdict(list)
    for s in sites:
        edits.append((s.pos, s.end, f'npc_uc_{s.i}'))
        by_block[s.block].append(s)
    for blk, ss in by_block.items():
        ip = insert_point(m, blk[0])
        decls = ''.join(decl_for(src, m, s.tok, s.pos, f'npc_uc_{s.i}') for s in ss)
        edits.append((ip, ip, decls))
    strip_static_funcs(m, edits)
    out = src
    for pos, end, text in sorted(edits, key=lambda e: (e[0], e[1]), reverse=True):
        out = out[:pos] + text + out[end:]
    return out


# ---------------------------------------------------------------- Makefile flags
class MakeVars:
    def __init__(self, path='Makefile'):
        self.glob, self.tgt = {}, []
        for line in open(path).read().replace('\\\n', ' ').split('\n'):
            if line.startswith('\t') or line.startswith('#'):
                continue
            mm = re.match(r'^([A-Za-z_][\w]*)\s*(:=|\?=|=|\+=)\s*(.*)$', line)
            if mm:
                k, op, v = mm.groups()
                if op == '+=':
                    self.glob[k] = self.glob.get(k, '') + ' ' + v
                elif op != '?=' or k not in self.glob:
                    self.glob[k] = v
                continue
            mm = re.match(r'^([^:=#]+?):\s*(?:private\s+)?(?:override\s+)?([A-Za-z_]\w*)\s*(:=|=|\+=)\s*(.*)$', line)
            if mm:
                self.tgt.append((mm.group(1).split(), mm.group(2), mm.group(3), mm.group(4)))

    def expand(self, v, local):
        for _ in range(10):
            nv = re.sub(r'\$\((\w+)\)', lambda x: local.get(x.group(1), self.glob.get(x.group(1), '')), v)
            if nv == v:
                break
            v = nv
        return v.strip()

    def for_target(self, obj):
        loc = {}
        for tgts, k, op, v in self.tgt:
            hit = any(t == obj or ('%' in t and re.fullmatch(re.escape(t).replace('%', '.*'), obj))
                      for t in tgts)
            if hit:
                loc[k] = (loc.get(k, self.glob.get(k, '')) + ' ' + v) if op == '+=' else v
        return {k: self.expand(loc.get(k, self.glob.get(k, '')), loc)
                for k in ('CC', 'CFLAGS', 'OPT_FLAGS', 'CC_ONLY_FLAGS', 'MIPSISET', 'CPPFLAGS')}


# ---------------------------------------------------------------- ELF
def elf_info(path):
    """{func: (value, size, bytes)}, {offset: symname} of R_MIPS_26 .text relocs."""
    with open(path, 'rb') as f:
        e = ELFFile(f)
        text_idx = [i for i, s in enumerate(e.iter_sections()) if s.name == '.text']
        if not text_idx:
            return {}, {}
        ti = text_idx[0]
        text = e.get_section(ti).data()
        st = e.get_section_by_name('.symtab')
        syms = sorted((s['st_value'], s['st_size'], s.name) for s in st.iter_symbols()
                      if s['st_shndx'] == ti and s.name and s['st_info']['type'] == 'STT_FUNC')
        funcs = {}
        for k, (v, sz, nm) in enumerate(syms):
            if not sz:
                sz = (syms[k + 1][0] if k + 1 < len(syms) else len(text)) - v
            funcs.setdefault(nm, (v, sz, text[v:v + sz]))
        rel = {}
        for sec in e.iter_sections():
            if sec.name in ('.rel.text', '.rela.text'):
                for r in sec.iter_relocations():
                    if r['r_info_type'] == 4:
                        s = st.get_symbol(r['r_info_sym'])
                        rel[r['r_offset']] = s.name or ('sect:' + str(s['st_shndx']))
        return funcs, rel


# ---------------------------------------------------------------- ROM resolution
class Rom:
    def __init__(self):
        rom = open('baserom.z64', 'rb').read()
        self.mods = UI.find_modules(rom)
        UI.attach_names(self.mods, '.')
        self.res = UI.Resolver(self.mods)
        self.by_short = {m.short: m for m in self.mods}
        self._jal = {}

    def module_for(self, path):
        d = path.split('/')[1] if path.startswith('src/') else None
        return self.by_short.get(MODULE_OF_DIR.get(d))

    def func_off(self, md, name):
        hits = [v for v, sz, nm in md.funcs if nm == name]
        return hits[0] if len(hits) == 1 else None

    def jal_target(self, md, off):
        if md.short not in self._jal:
            self._jal[md.short] = {o: f1 >> 4 for f1, o, _ in md.relocs('Text') if f1 & 7 == 1}
        idx = self._jal[md.short].get(off)
        if idx is None:
            return None
        ex, name = self.res.target(md, idx)
        return name


# ---------------------------------------------------------------- planning
def obj_of(path):
    return 'build/' + path + '.o'


def is_placeholder(symname, tok):
    return symname == tok or bool(PH_RE.fullmatch(symname or '')) or symname.startswith('sect:')


def compile_verify(path, text, mv):
    """-> (funcs, rel) of the verify object, or raises RuntimeError."""
    obj = obj_of(path)
    fl = mv.for_target(obj)
    base = os.path.join(SCRATCH, path)
    os.makedirs(os.path.dirname(base), exist_ok=True)
    vsrc, pre, vobj = base + '.v.c', base + '.pre.c', base + '.v.o'
    open(vsrc, 'w').write(text)
    asmproc = ['python3', 'tools/asm-processor/asm_processor.py'] + fl['OPT_FLAGS'].split()
    with open(pre, 'w') as fo:
        r = subprocess.run(asmproc + [vsrc], stdout=fo, stderr=subprocess.PIPE, text=True)
    if r.returncode:
        raise RuntimeError('asm-processor: ' + r.stderr.strip()[-300:])
    cmd = [fl['CC'] or 'tools/ido-static-recomp/build/7.1/out/cc', '-c'] + fl['CFLAGS'].split() + \
        fl['OPT_FLAGS'].split() + fl['CC_ONLY_FLAGS'].split() + fl['MIPSISET'].split() + \
        fl['CPPFLAGS'].split() + ['-o', vobj, pre]
    r = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
    if r.returncode or not os.path.exists(vobj):
        errs = [ln for ln in r.stdout.splitlines() if 'Error' in ln][:3]
        raise RuntimeError('cc: ' + ' | '.join(errs or r.stdout.splitlines()[-3:]))
    return elf_info(vobj)


def plan_file(path, rom, mv, verbose=False):
    """-> dict(path, sites=[Site], derived={i: name}, left={i: reason}, error=str|None)."""
    out = {'path': path, 'sites': [], 'derived': {}, 'left': {}, 'error': None}
    src = open(path, errors='replace').read()
    sites, m = scan(src)
    out['sites'] = sites
    if not sites:
        return out
    if path.endswith('.cpp'):
        out['error'] = 'NCC .cpp (verify compile not supported)'
        return out
    md = rom.module_for(path)
    if md is None or not md.funcs:
        out['error'] = 'module not resolvable by uso-imports'
        return out
    if not os.path.exists(obj_of(path)):
        out['error'] = 'no built object ' + obj_of(path)
        return out
    try:
        vfuncs, vrel = compile_verify(path, verify_text(src, m, sites), mv)
    except RuntimeError as ex:
        out['error'] = str(ex)
        return out
    rfuncs, rrel = elf_info(obj_of(path))
    # npc offsets per site, per function
    site_offs = collections.defaultdict(list)       # i -> [(func, reloff)]
    for off, sym in vrel.items():
        mm = re.fullmatch(r'npc_uc_(\d+)', sym)
        if not mm:
            continue
        fn = next((f for f, (v, sz, _) in vfuncs.items() if v <= off < v + sz), None)
        site_offs[int(mm.group(1))].append((fn, off - vfuncs[fn][0] if fn else None))
    bad_funcs = {}
    for fn, (v, sz, b) in vfuncs.items():
        mine = [i for i, l in site_offs.items() if any(f == fn for f, _ in l)]
        if not mine:
            continue
        if fn not in rfuncs:
            bad_funcs[fn] = 'not in the real object'
            continue
        rv, rsz, rb = rfuncs[fn]
        if rb != b:
            bad_funcs[fn] = 'verify bytes differ from the real object (donor splice / other recipe)'
            continue
        covered = {o for i in mine for f, o in site_offs[i] if f == fn}
        real_ph = {off - rv for off, s in rrel.items() if rv <= off < rv + rsz and
                   (PH_RE.fullmatch(s) or s.startswith('sect:'))}
        if real_ph - covered:
            bad_funcs[fn] = f'call count != reloc count ({len(covered)} sites, {len(real_ph)} placeholder jals)'
            continue
        if rom.func_off(md, fn) is None:
            bad_funcs[fn] = f'not (uniquely) in the {md.short} name table'
    for s in sites:
        offs = site_offs.get(s.i)
        if not offs:
            out['left'][s.i] = 'no jal in the build (not a call, or not compiled)'
            continue
        fns = {f for f, _ in offs}
        if len(fns) != 1 or None in fns:
            out['left'][s.i] = 'jal outside a function symbol'
            continue
        fn = fns.pop()
        if fn in bad_funcs:
            out['left'][s.i] = f'{fn}: {bad_funcs[fn]}'
            continue
        base = rom.func_off(md, fn)
        rv = rfuncs[fn][0]
        names = set()
        for _, o in offs:
            if not is_placeholder(rrel.get(rv + o, ''), s.tok):
                names.add(None)
                continue
            names.add(rom.jal_target(md, base + o))
        if len(names) != 1 or None in names:
            out['left'][s.i] = f'{fn}: callee unresolved or ambiguous {sorted(map(str, names))}'
            continue
        nm = names.pop()
        if not IDENT.fullmatch(nm):
            out['left'][s.i] = f'{fn}: callee {nm} is not a symbol name'
            continue
        if nm == s.tok:
            out['left'][s.i] = f'{fn}: callee is the function named {nm} itself'
            continue
        out['derived'][s.i] = nm
    return out


def spelled(tok, name):
    mac = MACRO_OF.get(tok)
    return f'{mac}({name})' if mac else f'XCALL({tok}, {name})'


def add_include(path, src):
    if 'uso_call.h"' in src:
        return src
    if path.endswith('.cpp'):
        line = f'#include "{INCLUDE_REL}"  /* NCC donors build without -I include */\n'
    else:
        line = INCLUDE_LINE + '\n'
    ms = list(re.finditer(r'^#include .*\n', src[:20000], re.M))
    i = ms[-1].end() if ms else 0
    return src[:i] + line + src[i:]


def run_plans(files, rom, mv, verbose):
    with concurrent.futures.ThreadPoolExecutor(max_workers=8) as ex:
        return list(ex.map(lambda p: plan_file(p, rom, mv, verbose), files))


def cmd_plan(a, rom, mv):
    tot_named = tot_left = 0
    for res in run_plans(a.files, rom, mv, a.verbose):
        path, sites = res['path'], res['sites']
        raw = [s for s in sites if s.name is None]
        if res['error']:
            print(f'{path}: {len(raw)} raw placeholder sites LEFT: {res["error"]}')
            tot_left += len(raw)
            continue
        named = [s for s in raw if s.i in res['derived']]
        left = [s for s in raw if s.i not in res['derived']]
        why = collections.Counter(re.sub(r'^\w+: ', '', res['left'][s.i]) for s in left)
        print(f'{path}: {len(named)} named, {len(left)} left')
        for r, n in why.most_common():
            print(f'    left {n:4d}: {r}')
        if a.verbose:
            for s in left:
                print(f'      {s.func}: {s.tok} -- {res["left"][s.i]}')
            for s in named:
                print(f'      {s.func}: {s.tok} -> {res["derived"][s.i]}')
        tot_named += len(named)
        tot_left += len(left)
        if a.write and named:
            src = open(path, errors='replace').read()
            for s in sorted(named, key=lambda s: s.pos, reverse=True):
                assert src[s.pos:s.end] == s.tok
                src = src[:s.pos] + spelled(s.tok, res['derived'][s.i]) + src[s.end:]
            open(path, 'w').write(add_include(path, src))
    print(f'total: {tot_named} named, {tot_left} left' + ('' if a.write else ' (dry run; --write to apply)'))


def macro_files():
    out = []
    for root, _, fs in os.walk('src'):
        for f in fs:
            if f.endswith(('.c', '.cpp')):
                p = os.path.join(root, f)
                if MAC_RE.search(mask(open(p, errors='replace').read())):
                    out.append(p)
    return sorted(out)


def check_fast(path, src, rom, quiet=False):
    """-> (named calls, mismatches) for one file's text."""
    sites, _ = scan(src)
    mac = [s for s in sites if s.name is not None]
    if not mac:
        return 0, 0
    md = rom.module_for(path)
    if md is None:
        print(f'{path}: module not resolvable')
        return len(mac), len(mac)
    rfuncs, rrel = elf_info(obj_of(path))
    want = collections.defaultdict(collections.Counter)
    for s in mac:
        want[(s.func, s.tok)][s.name] += 1
    n = bad = 0
    for (fn, tok), cnt in want.items():
        n += sum(cnt.values())
        have = collections.Counter()
        base = rom.func_off(md, fn) if fn in rfuncs else None
        if base is not None:
            rv, rsz, _ = rfuncs[fn]
            for off, sym in rrel.items():
                if rv <= off < rv + rsz and (sym == tok or sym.startswith('sect:')):
                    have[rom.jal_target(md, base + off - rv)] += 1
        over = cnt - have
        if over:
            bad += sum(over.values())
            if not quiet:
                print(f'{path}: {fn}: {dict(over)} not among the ROM callees of its {tok} jals {dict(have)}')
    return n, bad


def cmd_selftest(a, rom, mv):
    """check must catch a wrong name (fast) and a swapped pair inside one function (--strict)."""
    path = 'src/game_uso/game_uso.c'
    src = open(path).read()
    n, bad = check_fast(path, src, rom, quiet=True)
    assert n > 200 and bad == 0, (n, bad)
    i = src.index('CALL(fs_gdlReadFile)')
    wrong = src[:i] + 'CALL(mem_new)' + src[i + len('CALL(fs_gdlReadFile)'):]
    assert check_fast(path, wrong, rom, quiet=True)[1] == 1
    # cam_reset_000008FC calls three different callees: swap the first two names
    sites, m = scan(src)
    cr = [s for s in sites if s.func == 'cam_reset_000008FC' and s.name]
    x, y = cr[0], cr[1]
    assert x.name != y.name
    sw = src[:x.pos] + spelled(y.tok, y.name) + src[x.end:y.pos] + spelled(x.tok, x.name) + src[y.end:]
    assert check_fast(path, sw, rom, quiet=True)[1] == 0          # same multiset: fast check passes
    real = open(path).read()
    try:
        open(path, 'w').write(sw)
        res = plan_file(path, rom, mv)
    finally:
        open(path, 'w').write(real)
    got = [(s.name, res['derived'].get(s.i)) for s in res['sites'] if s.func == 'cam_reset_000008FC' and s.name]
    assert got[0][0] != got[0][1] and got[1][0] != got[1][1], got   # strict re-derivation catches it
    print('selftest OK')
    return 0


def cmd_check(a, rom, mv):
    files = a.files or macro_files()
    bad = n = 0
    if a.strict:
        for res in run_plans(files, rom, mv, False):
            for s in res['sites']:
                if s.name is None:
                    continue
                n += 1
                got = res['derived'].get(s.i)
                if got != s.name:
                    bad += 1
                    print(f'{res["path"]}: {s.func}: {spelled(s.tok, s.name)} -- '
                          f'{"ROM callee " + got if got else res["error"] or res["left"].get(s.i)}')
    else:
        for path in files:
            nn, bb = check_fast(path, open(path, errors='replace').read(), rom)
            n += nn
            bad += bb
    print(f'check: {n} named calls, {bad} mismatches')
    return 1 if bad else 0


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest='cmd', required=True)
    p = sub.add_parser('plan'); p.add_argument('files', nargs='+')
    p.add_argument('--write', action='store_true'); p.add_argument('-v', '--verbose', action='store_true')
    p = sub.add_parser('check'); p.add_argument('files', nargs='*')
    p.add_argument('--strict', action='store_true')
    sub.add_parser('selftest')
    a = ap.parse_args()
    rom, mv = Rom(), MakeVars()
    rc = {'plan': cmd_plan, 'check': cmd_check, 'selftest': cmd_selftest}[a.cmd](a, rom, mv)
    shutil.rmtree(SCRATCH, ignore_errors=True) if a.cmd == 'plan' and not a.verbose else None
    sys.exit(rc or 0)


if __name__ == '__main__':
    main()
