#!/usr/bin/env python3
"""ROM-wide readability census -> a dashboard section in docs/READABILITY_PLAN.md (agent-a run 517).

  scripts/readability-census.py [--no-build] [--write [PLAN.md]] [--top N]
  scripts/readability-census.py [--no-build] --unjustified [--owner X]   (file:line function list)
  scripts/readability-census.py [--no-build] --unnamed-unjustified [--kind calls|functions|globals]
      (file:line name list of the unnamed placeholders without an `UNNAMED:` tag)
  scripts/readability-census.py [--no-build] --by-reason [--list KEYWORD [--owner X]]
      (justified sites per RAW: keyword per owner; --list prints one keyword's sites as file:line function)

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
    placeholder call sites left, in compiled code (address tokens such as game_uso_func_055750
    count as placeholders since run 520).
  * Data sites (scripts/uso-data-sites.py, bootup.uso Text = game_libs + bootup_uso, ROM relocs):
    LO16 sites on named objects / on section bases; provable raw sites left (`convert` and
    `spell-base` dry-run plans); source spellings (GL_*_STR, GL_*, sStr_/sFmt_/sTag_, kRo_, raw
    `&D_00000000`).
  * Raw struct offsets in compiled code: `*(T *)(expr + K)` casts and uses of offset-accessor
    macros (any `#define NAME(p, o ...)` whose body is `*(T *)((char *)(p) + (o))`, e.g. FW / FF),
    except accessor uses whose offset is a named constant (kPmf_/kRoD_/kStr_/kTag_ ..., run 568).
    Per file for the C++ donors (.cpp), and the top N functions by raw-site count with their
    owner area (owner(): b / c / g / h / a, run 520).
    Justified raw sites (run 577): a site is justified when its line, or the line just above it
    (or the comment that line closes), carries `RAW: <keyword> <reason>` with keyword one of
    byte-lock (every typed spelling changes the bytes; name the lever), data-table (a read from
    a data table / pool, not a struct), unknown-object (the object type is not proven yet),
    nm-only (a NON_MATCHING reference body), donor-mirror (a C++ donor that cannot include the
    header). Raw total / justified / UNJUSTIFIED per owner and per file; --unjustified lists the
    unjustified sites as `file:line function` (--owner X narrows to one owner).
  * Naming (run 594): placeholder calls left, placeholder / weak function names and `D_` globals,
    each named / justified / UNJUSTIFIED. An unnamed one is justified when its line, or the line
    just above it (or the comment that line closes), carries `UNNAMED: <keyword> <reason>` with
    keyword one of no-reloc (the call has no relocation to resolve), sentinel (an address-0
    placeholder that must stay), data-base (a symbol that doubles as a USO data base),
    unknown-role (the evidence does not determine a name), mid-entry (the callee is an entry
    point inside a build function, `f+0xK`, with no symbol of its own; say which). The line is
    the call site's, the function definition's name line, or for a `D_` global any compiled line
    that spells it (its extern / definition). --unnamed-unjustified lists the rest.
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


NAMED_OFF_HDRS = ('include/game/game_uso_data.h', 'include/gdl/gl_data.h',
                  'include/game/select.h', 'include/audio.h')   # run 585: genproc / audio section offsets
NAMED_OFF_DEF = re.compile(r'^\s*#\s*define\s+(k[A-Z]\w*)\s+\(?\s*(?:0x[0-9A-Fa-f]+|\d+)\s*\)?\s*(?:/\*.*)?$', re.M)


def named_offsets():
    """`kPmf_*` / `kRoD_*` / `kRoF_*` / `kStr_*` / `kTag_*` ...: the named section offsets
    (`#define kX <integer>`) in game_uso_data.h / gl_data.h (run 568, agent-g's rule), and select.h /
    audio.h (run 585: genproc and audio section offsets)."""
    names = set()
    for p in NAMED_OFF_HDRS:
        if os.path.exists(p):
            names |= set(NAMED_OFF_DEF.findall(open(p, errors='replace').read()))
    return names


NAMED_OFF = set()


def top_args(code, i):
    """Top-level comma-split arguments of the call whose `(` is at code[i - 1]."""
    args, depth, start = [], 1, i
    while i < len(code) and depth:
        ch = code[i]
        if ch in '([{':
            depth += 1
        elif ch in ')]}':
            depth -= 1
        elif ch == ',' and depth == 1:
            args.append(code[start:i])
            start = i + 1
        i += 1
    args.append(code[start:i - 1])
    return [a.strip() for a in args]


def raw_offset_sites(code, acc_re):
    """Positions of `*(T *)(expr + K)` casts (not data-placeholder sites) and of accessor-macro
    uses: ([cast pos], [accessor pos]). An accessor use whose offset argument is a named
    constant (NAMED_OFF: per-site token + named offset, the documented spelling) is not raw;
    the cast form already needs a literal K (TAIL_OFF)."""
    casts = []
    for m in CAST_RE.finditer(code):
        i, depth = m.end(), 1
        while i < len(code) and depth:
            depth += {'(': 1, ')': -1}.get(code[i], 0)
            i += 1
        inner = code[m.end():i - 1]
        if TAIL_OFF.search(inner) and 'D_00000000' not in inner and 'gl_data' not in inner:
            casts.append(m.start())
    accs = [m.start() for m in acc_re.finditer(code)
            if not any(a in NAMED_OFF for a in top_args(code, m.end()))] if acc_re else []
    return casts, accs


RAW_KEYWORDS = ('byte-lock', 'data-table', 'unknown-object', 'nm-only', 'donor-mirror')
RAW_TAG = re.compile(r'\bRAW:\s*(' + '|'.join(RAW_KEYWORDS) + r')\b')
UNNAMED_KEYWORDS = ('no-reloc', 'sentinel', 'data-base', 'unknown-role', 'mid-entry')
UNNAMED_TAG = re.compile(r'\bUNNAMED:\s*(' + '|'.join(UNNAMED_KEYWORDS) + r')\b')


def justified_lines(src, tag=RAW_TAG):
    """{1-based line: keyword} for the lines whose raw sites are justified: a `RAW: <keyword>` tag on the
    line itself, or on the line just above (when that line closes a multi-line comment, anywhere
    in that comment)."""
    lines = src.split('\n')
    tagged = [(tag.search(ln) or None) and tag.search(ln).group(1) for ln in lines]
    # a multi-line /* ... */ comment carries its tag to its closing line
    in_c, start = False, 0
    for i, ln in enumerate(lines):
        j = 0
        while True:
            if not in_c:
                k = ln.find('/*', j)
                if k < 0:
                    break
                in_c, start, j = True, i, k + 2
            else:
                k = ln.find('*/', j)
                if k < 0:
                    break
                in_c, j = False, k + 2
                if start != i and any(tagged[start:i + 1]):
                    tagged[i] = next(t for t in tagged[start:i + 1] if t)
    ok = {}                                  # line -> keyword (the site's own line wins)
    for i, tg in enumerate(tagged):
        if tg:
            ok.setdefault(i + 2, tg)
            ok[i + 1] = tg
    return ok


def raw_offsets(code, acc_re):
    """Count `*(T *)(expr + K)` casts (not data-placeholder sites) and accessor-macro uses."""
    c, a = raw_offset_sites(code, acc_re)
    return len(c), len(a)


def block_names(m):
    """[(open, close, name)] for file-scope brace blocks: functions by their (Class::)name, other
    blocks (C++ class bodies with inline members, initialisers) as `<struct X>` / `<block>`."""
    out = []
    for boundary, op, cl in NPC.top_blocks(m):
        h = m[boundary:op]
        if re.fullmatch(r'\s*(?:extern\s*|namespace\s+\w*\s*)', h):
            # `extern "C" { ... }` (the string is masked) / namespace: name the blocks inside
            out += [(o + op + 1, c + op + 1, n) for o, c, n in block_names(m[op + 1:cl])]
            continue
        f = re.search(r'((?:\w+\s*::\s*)*~?[A-Za-z_]\w*)\s*\(', h) if ')' in h and '=' not in h else None
        if f:
            name = re.sub(r'\s+', '', f.group(1))
        else:
            k = re.search(r'\b(?:struct|union|class)\s+(\w+)', h)
            name = f'<{k.group(0).split()[0]} {k.group(1)}>' if k else '<block>'
        out.append((op, cl, name))
    return out


AUDIO_FN = re.compile(r'(?:\w+::)?(?:Audio|snd_|Snd|Note_|Seq_)\w*')
HOME_H = re.compile(r'^src/(?:bootup_uso|titproc_uso|arcproc_uso|mgrproc_uso|h2hproc_uso|timproc_uso_b1|timproc_uso_b3)/'
                    r'|^src/game_libs/game_libs(?:_tail)?\.c$')


def owner(path, func):
    """Area owner for the readability phase (coordinator's table, run 520): b = game_libs_post0b /
    post1b*; c = timproc_uso_b5*, game_libs_post.c (run 536) and the audio driver; g = game_uso; h = bootup_uso / titproc /
    game_libs.c / game_libs_tail / arc / mgr / h2h / tim / trk; a = everything else."""
    if re.search(r'/game_libs_post(?:0b|1b)', path):
        return 'b'
    if path == 'src/game_libs/game_libs_post.c':
        return 'c'          # the whole file since run 536 (coordinator), not only its audio functions
    if path.startswith('src/timproc_uso_b5') or AUDIO_FN.fullmatch(func):
        return 'c'
    if path.startswith('src/game_uso/'):
        return 'g'
    if HOME_H.search(path):
        return 'h'
    return 'a'


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


def naming(un_calls, fn_defs, d_lines, calls):
    """-> (dashboard rows, [(kind, file, line, what)] unjustified) for calls / functions / globals."""
    unj = []
    named_calls = sum(c['named'] for c in calls.values())
    cj = sum(1 for x in un_calls if x[4])
    unj += [('calls', p, ln, f'{f}: {tok}') for p, ln, f, tok, k in un_calls if not k]
    r = json.load(open('report.json'))
    segs = sorted({u['name'].split('/')[1] for u in r['units'] if u['name'].count('/') >= 2})
    prefixes = set(segs) | {s.replace('_uso', '') for s in segs} | {'gl', 'game_libs', 'gui'}
    fcls = collections.Counter()
    fj = 0
    for u in r['units']:
        for f in u.get('functions', []):
            name = NPC.current_name(f['name']) or f['name']
            c = fn_class(name, prefixes)
            fcls[c] += 1
            if c in ('placeholder', 'weak'):
                ds = fn_defs.get(name, [])
                if any(k for _, _, k in ds):
                    fj += 1
                else:
                    d = next((x for x in ds if not x[0].startswith('src/match_imports/')), ds[0] if ds else None)
                    unj.append(('functions', d[0] if d else '?', d[1] if d else 0, f'{name} ({c})'))
    fun = fcls['placeholder'] + fcls['weak']
    gj = 0
    for nm, occ in sorted(d_lines.items()):
        if any(k for _, _, k in occ):
            gj += 1
        else:
            p, ln, _ = occ[0]
            unj.append(('globals', p, ln, nm))
    rows = [('placeholder calls', named_calls, len(un_calls), cj, len(un_calls) - cj),
            ('placeholder / weak functions', fcls['real'], fun, fj, fun - fj),
            ('`D_` globals', '-', len(d_lines), gj, len(d_lines) - gj)]
    return rows, sorted(unj)


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
    NAMED_OFF.update(named_offsets())
    acc_re = re.compile(r'\b(?:' + '|'.join(sorted(acc)) + r')\s*\(') if acc else None

    calls = collections.defaultdict(collections.Counter)
    offs = collections.defaultdict(collections.Counter)
    fn_off = collections.Counter()          # (file, function) -> raw sites (casts + accessor uses)
    just = collections.defaultdict(collections.Counter)   # file -> {'raw', 'just'} ; owner via sites
    own_just = collections.defaultdict(collections.Counter)
    unjust = []                              # (file, line, function, owner)
    reasons = []                             # (file, line, function, owner, keyword) justified sites
    seg_off = collections.defaultdict(collections.Counter)
    dnames = collections.defaultdict(set)
    un_calls = []                            # (file, line, function, token, keyword or None)
    fn_defs = {}                             # function name -> [(file, line, keyword or None)] definitions
    d_lines = collections.defaultdict(list)  # D_ name -> [(file, line, keyword or None)]
    spell = collections.Counter()
    for p in compiled:
        src = open(p, errors='replace').read()
        live = live_mask(src, p.endswith('.cpp'), defined)
        m = NPC.mask(live)
        seg = segment_of(p)
        # address tokens (game_uso_func_055750(...)) are placeholder calls too (run 520)
        sites, _ = NPC.scan(live, m, extra=NPC.addr_tokens(p))
        unl = justified_lines(src, UNNAMED_TAG)
        for s in sites:
            calls[seg]['named' if s.name else 'left'] += 1
            if not s.name:
                ln = src.count('\n', 0, s.pos) + 1
                un_calls.append((p, ln, s.func, s.tok, unl.get(ln)))
        for op, cl, nm in block_names(m):
            k = m.rfind(nm.split('::')[-1], 0, op)
            if k >= 0 and not nm.startswith('<'):
                ln = src.count('\n', 0, k) + 1
                fn_defs.setdefault(nm.split('::')[-1], []).append((p, ln, unl.get(ln)))
        for mm in D_NAME.finditer(m):
            if mm.group(1) != '00000000':
                ln = src.count('\n', 0, mm.start()) + 1
                d_lines[mm.group(0)].append((p, ln, unl.get(ln)))
        # C code without the preprocessor lines (masked), but keep string text out too
        cpos, apos = raw_offset_sites(m, acc_re)
        nc, na = len(cpos), len(apos)
        if cpos or apos:
            blks = block_names(m)
            okl = justified_lines(src)
            for pos in cpos + apos:
                b = next((x for x in blks if x[0] < pos < x[1]), None)
                fname = b[2] if b else '<file scope>'
                fn_off[(p, fname)] += 1
                line = src.count('\n', 0, pos) + 1
                o = owner(p, fname)
                ok = line in okl
                just[p]['raw'] += 1
                own_just[o]['raw'] += 1
                if ok:
                    reasons.append((p, line, fname, o, okl[line]))
                    just[p]['just'] += 1
                    own_just[o]['just'] += 1
                else:
                    unjust.append((p, line, fname, o))
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

    nm_rows, nm_unj = naming(un_calls, fn_defs, d_lines, calls)
    if '--unnamed-unjustified' in args:
        kind = args[args.index('--kind') + 1] if '--kind' in args else None
        rows = [r for r in nm_unj if kind is None or r[0] == kind]
        for k, p, ln, what in rows:
            print(f'{p}:{ln} {what} [{k}]')
        print(f'{len(rows)} unjustified unnamed ' + (kind or 'calls / functions / globals'), file=sys.stderr)
        return

    if '--by-reason' in args:
        # justified raw sites per RAW: keyword per owner (run 582); --list KEY [--owner X] lists them
        if '--list' in args:
            key = args[args.index('--list') + 1]
            only = args[args.index('--owner') + 1] if '--owner' in args else None
            for p, line, f, o, k in sorted(reasons):
                if k == key and (only is None or o == only):
                    print(f'{p}:{line} {f} [{o}]')
            return
        cnt = collections.Counter((k, o) for _, _, _, o, k in reasons)
        owners = 'abcgh'
        for r in tbl(['keyword'] + list(owners) + ['total'],
                     [[f'`{k}`'] + [cnt[(k, o)] for o in owners] + [sum(cnt[(k, o)] for o in owners)]
                      for k in RAW_KEYWORDS] +
                     [['**all**'] + [sum(cnt[(k, o)] for k in RAW_KEYWORDS) for o in owners] + [len(reasons)]]):
            print(r)
        print(f'{len(unjust)} unjustified raw sites', file=sys.stderr)
        return

    if '--unjustified' in args:
        only = args[args.index('--owner') + 1] if '--owner' in args else None
        rows = [u for u in sorted(unjust) if only is None or u[3] == only]
        for p, line, f, o in rows:
            print(f'{p}:{line} {f} [{o}]')
        print(f'{len(rows)} unjustified raw sites' + (f' (owner {only})' if only else ''), file=sys.stderr)
        return

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
          f'Accessor macros counted: {", ".join(sorted(acc))}; a use whose offset argument is one of the '
          f'{len(NAMED_OFF)} named offsets (`#define kX <int>` in game_uso_data.h / gl_data.h / select.h / audio.h: kPmf_, kRoD_, kRoF_, '
          'kStr_, kTag_ ...) is not counted.', '']
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
    cpp = sorted(((p, c) for p, c in offs.items() if p.endswith('.cpp') and c['cast'] + c['macro']),
                 key=lambda kv: (-(kv[1]['cast'] + kv[1]['macro']), kv[0]))
    ncpp = sum(1 for p in offs if p.endswith('.cpp'))
    L += ['', f'### Raw struct offsets in C++ donors (.cpp, per file): {len(cpp)} of {ncpp} compiled donors, '
          f'{sum(c["cast"] + c["macro"] for _, c in cpp)} sites', '']
    L += tbl(['file', 'owner', 'casts', 'accessor uses', 'total'],
             [(f'`{p}`', owner(p, ''), c['cast'], c['macro'], c['cast'] + c['macro']) for p, c in cpp])
    L += ['', f'### Raw struct offsets: top {top} functions', '']
    wf = sorted(fn_off.items(), key=lambda kv: (-kv[1], kv[0]))[:top]
    L += tbl(['function', 'file', 'owner', 'raw sites'], [(f'`{f}`', f'`{p}`', owner(p, f), n) for (p, f), n in wf])
    byown = collections.Counter()
    for (p, f), n in fn_off.items():
        byown[owner(p, f)] += n
    L += ['', 'Owners: b = game_libs_post0b / post1b*; c = timproc_uso_b5*, game_libs_post.c and audio (Audio* / snd_* / Snd* / Note_ / Seq_ '
          'functions); g = game_uso; h = bootup_uso / titproc / game_libs.c / game_libs_tail / arc / mgr / h2h / tim / trk; '
          'a = everything else. Raw sites per owner: ' + ', '.join(f'{o} {byown[o]}' for o in 'abcgh') + '.']
    L += ['', '### Raw struct offsets: justified (`RAW:` tags)', '',
          'A raw site is justified when its line, or the line just above it, carries a `RAW: <keyword> <reason>` '
          'comment; keywords: ' + ', '.join(f'`{k}`' for k in RAW_KEYWORDS) + ' (docs/READABILITY_AGENT_RULES.md). '
          '`scripts/readability-census.py --no-build --unjustified [--owner X]` lists the rest as file:line function.', '']
    tot = sum((own_just[o] for o in own_just), collections.Counter())
    L += tbl(['owner', 'raw total', 'justified', 'UNJUSTIFIED'],
             [(o, own_just[o]['raw'], own_just[o]['just'], own_just[o]['raw'] - own_just[o]['just']) for o in 'abcgh'] +
             [('**all**', tot['raw'], tot['just'], tot['raw'] - tot['just'])])
    jf = sorted(((p, c) for p, c in just.items() if c['raw'] - c['just']), key=lambda kv: (-(kv[1]['raw'] - kv[1]['just']), kv[0]))
    L += ['', f'Files with unjustified raw sites: {len(jf)} (top {top}):', '']
    L += tbl(['file', 'owner', 'raw total', 'justified', 'UNJUSTIFIED'],
             [(f'`{p}`', owner(p, ''), c['raw'], c['just'], c['raw'] - c['just']) for p, c in jf[:top]])
    L += ['', '### Naming: unnamed placeholders (`UNNAMED:` tags)', '',
          'An unnamed call / function / `D_` global is justified when its line (call site, definition name '
          'line, or any line spelling the global), or the line just above it, carries `UNNAMED: <keyword> <reason>`; '
          'keywords: ' + ', '.join(f'`{k}`' for k in UNNAMED_KEYWORDS) + ' (docs/READABILITY_AGENT_RULES.md). '
          '`scripts/readability-census.py --no-build --unnamed-unjustified [--kind calls|functions|globals]` lists the rest. '
          'Functions: placeholder + weak names (report.json, current names); one with no compiled definition '
          'found counts as unjustified.', '']
    L += tbl(['category', 'named', 'unnamed', 'justified', 'UNJUSTIFIED'], nm_rows)
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
