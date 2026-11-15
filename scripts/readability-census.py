#!/usr/bin/env python3
"""ROM-wide readability census -> a dashboard section in docs/READABILITY_PLAN.md (agent-a run 517).

  scripts/readability-census.py [--no-build] [--write [PLAN.md]] [--top N]

Run from the project root after a full build (the readability gate leaves one). Without --write
the dashboard (Markdown) goes to stdout; with --write it replaces the block between
`<!-- census:begin -->` and `<!-- census:end -->` in the plan (default
/home/danjs/code/decomp/docs/READABILITY_PLAN.md), or inserts it before `## 1. Inventory`.

Measures (per segment = src/<segment>/ or the report.json unit's segment):
  * Functions (report.json, names mapped through renames.tsv so a stale report still counts the
    current names): placeholder (`func_<addr>`, `<segment>_func_<addr>`, `gl_func_<addr>`),
    weak (`<Kind>_func_<addr>`, `<Kind>_vfnN_<addr>`, `<Kind>_vslotN_<addr>`: the class/area
    is known, the role is not), real (everything else, address-suffixed or not).
  * Calls (scripts/name-placeholder-calls.py scan): CALL/FCALL/GCALL/XCALL-named sites vs raw
    placeholder call sites left, in compiled code.
  * Data sites (scripts/uso-data-sites.py, bootup.uso Text = game_libs + bootup_uso, ROM relocs):
    LO16 sites on named objects / on section bases; provable raw sites left (`convert` and
    `spell-base` dry-run plans); source spellings (GL_*_STR, GL_*, sStr_/sFmt_/sTag_, kRo_, raw
    `&D_00000000`).
  * Raw struct offsets in compiled code: `*(T *)(expr + K)` casts and uses of offset-accessor
    macros (any `#define NAME(p, o ...)` whose body is `*(T *)((char *)(p) + (o))`, e.g. FW / FF).
  * Globals: distinct `D_<8 hex>` names (not the D_00000000 sentinels) in compiled code.
  * Headers: include/ headers and the struct / union / class definitions in them.
  * Dead code: never-compiled `#if 0` / undefined-macro blocks (scripts/deadblocks.py).
  * Docs: each area doc in /home/danjs/code/decomp/docs with its size.

"Compiled code" = .c/.cpp files whose build/<path>.o exists (the matching build), with the
regions a matching build does not compile blanked: `#ifdef NON_MATCHING` bodies, `#if 0`,
blocks on never-defined macros, `#ifndef __cplusplus` in .cpp / `#ifdef __cplusplus` in .c.
Unknown #if expressions count as compiled (both branches).
"""
import collections
import glob
import importlib.util
import json
import os
import re
import sys

VENV = '/home/danjs/code/decomp/.venv'
try:
    import crunch64  # noqa: F401
    import elftools  # noqa: F401
except ImportError:
    if os.path.exists(VENV + '/bin/python') and not os.environ.get('CENSUS_REEXEC'):
        os.environ['CENSUS_REEXEC'] = '1'
        os.execv(VENV + '/bin/python', [VENV + '/bin/python'] + sys.argv)
    sys.exit('needs pyelftools + crunch64: run with /home/danjs/code/decomp/.venv/bin/python')

HERE = os.path.dirname(os.path.abspath(__file__))
DOCS = '/home/danjs/code/decomp/docs'
PLAN = os.path.join(DOCS, 'READABILITY_PLAN.md')
BEGIN, END = '<!-- census:begin -->', '<!-- census:end -->'


def _load(name, fn):
    argv = sys.argv
    sys.argv = [fn]
    try:
        spec = importlib.util.spec_from_file_location(name, os.path.join(HERE, fn))
        m = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(m)
    finally:
        sys.argv = argv
    return m


NPC = _load('npc', 'name-placeholder-calls.py')
DB = _load('deadblocks', 'deadblocks.py')


# ---------------------------------------------------------------- compiled regions
def live_mask(src, cpp, defined):
    """src with the lines a matching build does not compile blanked (newlines kept)."""
    lines = src.split('\n')
    stack = []          # [parent_live, cond (True/False/None), taken]
    live = True
    out = []

    def ev(expr):
        e = expr.strip()
        e = re.sub(r'/\*.*?\*/|//.*', '', e).strip()
        if re.fullmatch(r'0+', e):
            return False
        if re.fullmatch(r'[1-9]\d*', e):
            return True
        m = re.fullmatch(r'(!?)\s*defined\s*\(?\s*(\w+)\s*\)?', e)
        if m:
            d = is_def(m.group(2))
            return None if d is None else (d != bool(m.group(1)))
        return None

    def is_def(name):
        if name == '__cplusplus':
            return cpp
        if name in ('NON_MATCHING', 'PERMUTER', 'M2CTX'):
            return False
        return True if name in defined else False

    for ln in lines:
        s = ln.strip()
        m = re.match(r'#\s*(ifdef|ifndef|if|elif|else|endif)\b(.*)', s)
        if m:
            kw, rest = m.group(1), m.group(2)
            if kw in ('ifdef', 'ifndef', 'if'):
                if kw == 'if':
                    c = ev(rest)
                else:
                    name = rest.split()[0] if rest.split() else ''
                    d = is_def(name)
                    # #ifndef GUARD: include guards and "not yet defined" fallbacks stay live
                    c = d if kw == 'ifdef' else (None if name not in ('__cplusplus', 'NON_MATCHING') else not d)
                stack.append([live, c, c is True])
                live = live and c is not False
            elif kw == 'elif' and stack:
                top = stack[-1]
                c = False if top[2] else ev(rest)
                top[1] = c
                top[2] = top[2] or c is True
                live = top[0] and c is not False
            elif kw == 'else' and stack:
                top = stack[-1]
                c = None if top[1] is None and not top[2] else (not top[2])
                top[1] = c
                live = top[0] and c is not False
            elif kw == 'endif' and stack:
                live = stack.pop()[0]
            out.append(ln)
            continue
        out.append(ln if live else ' ' * len(ln))
    return '\n'.join(out)


def compiled_sources():
    files = sorted(p for e in ('c', 'cpp') for p in glob.glob(f'src/**/*.{e}', recursive=True))
    return [p for p in files if os.path.exists(f'build/{p}.o')]


def segment_of(path):
    parts = path.split('/')
    return parts[1] if len(parts) > 2 else parts[0]


# ---------------------------------------------------------------- measures
WEAK_FN = re.compile(r'_(?:func_[0-9A-Fa-f]{5,8}|vfn\d+(?:_[0-9A-Fa-f]{5,8})?|vslot\d+(?:_[0-9A-Fa-f]{5,8})?)$')


def fn_class(name, seg_prefixes):
    m = re.match(r'^(?:(\w+?)_)?func_[0-9A-Fa-f]{8}$', name)
    if m and (m.group(1) is None or m.group(1) in seg_prefixes):
        # `<segment>_func_00000000` / `func_00000000`: the zero sentinel (never renamed)
        return 'sentinel' if name.endswith('_00000000') else 'placeholder'
    if WEAK_FN.search(name):
        return 'weak'
    return 'real'


def functions():
    r = json.load(open('report.json'))
    segs = sorted({u['name'].split('/')[1] for u in r['units'] if u['name'].count('/') >= 2})
    prefixes = set(segs) | {s.replace('_uso', '') for s in segs} | {'gl', 'game_libs', 'gui'}
    rows = collections.defaultdict(collections.Counter)
    for u in r['units']:
        seg = u['name'].split('/')[1] if u['name'].count('/') >= 2 else u['name']
        for f in u.get('functions', []):
            name = NPC.current_name(f['name']) or f['name']
            rows[seg][fn_class(name, prefixes)] += 1
    return rows


ACCESSOR_DEF = re.compile(r'#\s*define\s+(\w+)\s*\(\s*(\w+)\s*,\s*(\w+)[^)]*\)\s*\(?\s*\*\s*\(\s*[\w\s]+\*\s*\)\s*\(\s*\(\s*char\s*\*\s*\)')
CAST_RE = re.compile(r'\*\s*\(\s*(?:(?:const|volatile|unsigned|signed|struct|union)\s+)*\w+(?:\s*\*)+\s*\)\s*\(')
TAIL_OFF = re.compile(r'[+-]\s*(?:0x[0-9A-Fa-f]+|\d+)\s*[uUlL]*\s*$')


def accessor_macros():
    names = set()
    for p in glob.glob('include/**/*.h', recursive=True) + glob.glob('src/**/*.[ch]', recursive=True) + \
            glob.glob('src/**/*.cpp', recursive=True):
        t = open(p, errors='replace').read()
        for m in ACCESSOR_DEF.finditer(t):
            names.add(m.group(1))
    return names


def raw_offsets(code, acc_re):
    """Count `*(T *)(expr + K)` casts (not data-placeholder sites) and accessor-macro uses."""
    n_cast = 0
    for m in CAST_RE.finditer(code):
        i, depth = m.end(), 1
        while i < len(code) and depth:
            depth += {'(': 1, ')': -1}.get(code[i], 0)
            i += 1
        inner = code[m.end():i - 1]
        if TAIL_OFF.search(inner) and 'D_00000000' not in inner and 'gl_data' not in inner:
            n_cast += 1
    n_acc = len(acc_re.findall(code)) if acc_re else 0
    return n_cast, n_acc


D_NAME = re.compile(r'\bD_([0-9A-Fa-f]{8})\b(?!_)')
STR_ALIAS = re.compile(r'\bs(?:Str|Fmt|Tag)_\w+')
KRO = re.compile(r'\bkRo_\w+')
GL_STR = re.compile(r'\bGL_(?:DATA|RODATA)_STR\s*\(')
GL_BASE = re.compile(r'\bGL_(?:DATA|RODATA|BSS|TEXT)\s*\(')
RAW_BASE = re.compile(r'&\s*D_00000000\b(?!_)')


def headers():
    rows = collections.Counter()
    nstruct = collections.Counter()
    for p in sorted(glob.glob('include/**/*.h', recursive=True)):
        d = os.path.dirname(p)[len('include'):].lstrip('/') or '.'
        rows[d] += 1
        t = NPC.mask(open(p, errors='replace').read())
        nstruct[d] += len(re.findall(r'\b(?:struct|union|class)\s+\w*\s*(?::[^{;()]*)?\{', t))
    return rows, nstruct


def data_sites(compiled):
    UDS = _load('uso_data_sites', 'uso-data-sites.py')
    c = UDS.Census('.', True)
    obj = base = obj_named = base_named = 0
    zero = re.compile(r'^(?:D_00000000|gl_data_00000000)$')
    for s in c.sites:
        if s.rtype != 2 or s.obj is None or s.obj[0] == 'ext':
            continue
        named = s.placeholder is not None and not zero.match(s.placeholder)
        if s.sym.kind == 2:
            obj += 1
            obj_named += named
        elif s.sym.kind == 4:
            base += 1
            base_named += named
    srcs = [p for p in compiled if p.startswith(('src/game_libs/', 'src/bootup_uso/'))]
    objs = [(m['name'], m['claim'][1]) for m in UDS.scan_macros()
            if not m['local'] and m['claim'] and m['claim'][0] == 'sym' and m.get('plain') and not m['params']]
    left_base = left_obj = 0
    for p in srcs:
        src = open(p, errors='replace').read()
        left_base += len(UDS.plan_spell_base(c, src))
        for name, n in objs:
            left_obj += len(UDS.plan_convert(c, n, src, bare=True))
    return dict(obj=obj, obj_named=obj_named, base=base, base_named=base_named,
                left_obj=left_obj, left_base=left_base)


def docs():
    out = []
    for p in sorted(glob.glob(os.path.join(DOCS, '1080_*.md'))) + \
            [os.path.join(DOCS, f) for f in ('READABILITY_PLAN.md', 'READABILITY_AGENT_RULES.md', 'USO_FORMAT.md')]:
        if os.path.exists(p):
            t = open(p, errors='replace').read()
            out.append((os.path.basename(p), t.count('\n'), len(t.encode()) / 1024))
    return out


def tbl(head, rows):
    o = ['| ' + ' | '.join(head) + ' |', '|' + '|'.join('---' if i == 0 else '--:' for i in range(len(head))) + '|']
    o += ['| ' + ' | '.join(str(x) for x in r) + ' |' for r in rows]
    return o


def pct(a, b):
    return f'{100.0 * a / b:.1f}%' if b else '-'


def main():
    args = sys.argv[1:]
    no_build = '--no-build' in args
    top = 25
    if '--top' in args:
        top = int(args[args.index('--top') + 1])
    write = None
    if '--write' in args:
        i = args.index('--write')
        write = args[i + 1] if i + 1 < len(args) and not args[i + 1].startswith('--') else PLAN
    if not os.path.exists('build/tenshoe.elf'):
        sys.exit('no build/tenshoe.elf: run a full make (or the readability gate) first')
    if not no_build:
        NPC.ensure_build(False)

    defined = DB.defined_macros('.')
    compiled = compiled_sources()
    acc = accessor_macros()
    acc_re = re.compile(r'\b(?:' + '|'.join(sorted(acc)) + r')\s*\(') if acc else None

    calls = collections.defaultdict(collections.Counter)
    offs = collections.defaultdict(collections.Counter)
    seg_off = collections.defaultdict(collections.Counter)
    dnames = collections.defaultdict(set)
    spell = collections.Counter()
    for p in compiled:
        src = open(p, errors='replace').read()
        live = live_mask(src, p.endswith('.cpp'), defined)
        m = NPC.mask(live)
        seg = segment_of(p)
        sites, _ = NPC.scan(live, m)
        for s in sites:
            calls[seg]['named' if s.name else 'left'] += 1
        # C code without the preprocessor lines (masked), but keep string text out too
        nc, na = raw_offsets(m, acc_re)
        offs[p]['cast'] += nc
        offs[p]['macro'] += na
        seg_off[seg]['cast'] += nc
        seg_off[seg]['macro'] += na
        for mm in D_NAME.finditer(m):
            if mm.group(1) != '00000000':
                dnames[seg].add(mm.group(0))
        if seg in ('game_libs', 'bootup_uso'):
            spell['str'] += len(GL_STR.findall(m))
            spell['base'] += len(GL_BASE.findall(m))
            spell['salias'] += len(STR_ALIAS.findall(m))
            spell['kro'] += len(KRO.findall(m))
            spell['raw'] += len(RAW_BASE.findall(m))

    fn = functions()
    ds = data_sites(compiled)
    hdr, nstruct = headers()
    dead = []
    for p in sorted(glob.glob('src/**/*.[ch]', recursive=True) + glob.glob('src/**/*.cpp', recursive=True)
                    + glob.glob('include/**/*.h', recursive=True)):
        _, found = DB.dead_blocks(open(p, errors='replace').read().split('\n'), defined)
        dead += [(p, f) for f in found]

    segs = sorted(set(fn) | set(calls) | set(seg_off), key=lambda s: (s != 'kernel', s))
    L = [BEGIN, '## 0. Dashboard (scripts/readability-census.py)', '',
         'Generated by `scripts/readability-census.py --write` (from the project root, after the gate\'s full '
         'build); do not edit by hand. Compiled code only: files with a matching-build object, '
         'NON_MATCHING-only and never-compiled blocks excluded.', '']
    tf = collections.Counter()
    rows = []
    for s in segs:
        f, c, o = fn.get(s, collections.Counter()), calls.get(s, collections.Counter()), seg_off.get(s, collections.Counter())
        tot = sum(f.values())
        tf.update(f)
        rows.append((s, tot, f['sentinel'], f['placeholder'], f['weak'], f['real'], pct(f['real'], tot - f['sentinel']),
                     c['named'], c['left'], o['cast'], o['macro'], len(dnames.get(s, ()))))
    allc = sum((calls[s] for s in calls), collections.Counter())
    allo = sum((seg_off[s] for s in seg_off), collections.Counter())
    rows.append(('**all**', sum(tf.values()), tf['sentinel'], tf['placeholder'], tf['weak'], tf['real'],
                 pct(tf['real'], sum(tf.values()) - tf['sentinel']), allc['named'], allc['left'], allo['cast'], allo['macro'],
                 len(set().union(*dnames.values())) if dnames else 0))
    L += ['### Per segment', '']
    L += tbl(['segment', 'fns', 'sentinel', 'placeholder', 'weak', 'real', 'real %', 'calls named', 'calls left',
              'raw `*(T *)(p+K)`', 'accessor macro uses', '`D_` globals'], rows)
    L += ['', 'Function classes: sentinel = `[<segment>_]func_00000000` (address-0 placeholder, never renamed); placeholder = `func_<addr>` / `<segment>_func_<addr>`; weak = '
          '`<Kind>_func_<addr>`, `<Kind>_vfnN[_<addr>]`, `<Kind>_vslotN[_<addr>]`; real = the rest; real % excludes sentinels. '
          f'Accessor macros counted: {", ".join(sorted(acc))}.', '']
    L += ['### Data sites (bootup.uso Text: game_libs + bootup_uso)', '']
    L += tbl(['', 'LO16 sites', 'named linker alias', 'provably raw left'],
             [('object-relocated (Sym kind 2)', ds['obj'], ds['obj_named'], ds['left_obj']),
              ('section-base (Sym kind 4)', ds['base'], ds['base_named'], ds['left_base'])])
    L += ['', f'Source spellings in compiled game_libs/bootup_uso: GL_*_STR {spell["str"]}, GL_DATA/RODATA/BSS/TEXT '
          f'{spell["base"]}, sStr_/sFmt_/sTag_ {spell["salias"]}, kRo_ {spell["kro"]}, raw `&D_00000000` '
          f'{spell["raw"]}. "Provably raw left" = the `convert --bare` / `spell-base` dry-run plans '
          '(sites the relocs prove but the source still spells raw).', '']
    L += [f'### Raw struct offsets: top {top} files', '']
    worst = sorted(offs.items(), key=lambda kv: -(kv[1]['cast'] + kv[1]['macro']))[:top]
    L += tbl(['file', 'casts', 'accessor uses', 'total'],
             [(f'`{p}`', c['cast'], c['macro'], c['cast'] + c['macro']) for p, c in worst if c['cast'] + c['macro']])
    L += ['', '### Headers', '']
    L += tbl(['include/ dir', 'headers', 'struct/union/class defs'],
             [(f'`{d}`', hdr[d], nstruct[d]) for d in sorted(hdr)] +
             [('**all**', sum(hdr.values()), sum(nstruct.values()))])
    L += ['', '### Dead code', '',
          f'{len(dead)} never-compiled blocks (`#if 0` / undefined macro; scripts/deadblocks.py) in src/ + include/'
          + (', '.join([''] + [f'`{p}:{f[0]}`' for p, f in dead[:20]]) if dead else '') + '.', '']
    L += ['### Docs (/home/danjs/code/decomp/docs)', '']
    L += tbl(['doc', 'lines', 'KB'], [(f'`{n}`', ln, f'{kb:.0f}') for n, ln, kb in docs()])
    L += ['', END]
    text = '\n'.join(L) + '\n'
    if not write:
        sys.stdout.write(text)
        return
    t = open(write).read()
    if BEGIN in t and END in t:
        a, b = t.index(BEGIN), t.index(END) + len(END)
        t = t[:a] + text.rstrip('\n') + t[b:]
    else:
        i = t.index('## 1. Inventory')
        t = t[:i] + text + '\n' + t[i:]
    open(write, 'w').write(t)
    print(f'{write}: dashboard written ({len(L)} lines)')


if __name__ == '__main__':
    main()
