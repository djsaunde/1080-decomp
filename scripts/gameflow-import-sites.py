#!/usr/bin/env python3
"""Spell per-offset GameFlow import sites as typed fields: `INPUT_MGR(tok).field`.

USO code reaches the GameFlow object (bootup.uso Bss 0xF8, typedef InputMgr in
include/game/input.h) through zero-pinned import tokens named after the ADDRESS the
original disassembly saw at the site: import_80020098 is GameFlow itself, and
import_800200CC / import_80020110 / import_800201D0 ... are the same object reached at
+0x34 / +0x78 / +0x138 (address = 0x80020098 + the instruction's offset). In the build every
such token is 0, so the C offset K in `*(int *)((char *)&import_800201D0 + 0x138)` is the
whole immediate, and the relocation names the token. This tool rewrites

    *(T *)((char *)&tok + K)   *(T *)(&tok + K)   *(T *)&tok   tok[i]   tok.v   tok (scalar)

to `INPUT_MGR(tok).field[idx]` (or `*(T *)&INPUT_MGR(tok).field` when the access type is not
the field's type). The token is kept, so the relocated symbol, the offset, and IDO's
per-symbol address CSE are unchanged.

Proof, per site
---------------
  1. ROM: the site's function, in the real build object, has relocations against tok, and
     every one of them sits at a module Text offset whose ROM TextReloc targets bootup Bss
     0xF8 (GameFlow; kind-2 object Sym, so the immediate K is the offset INSIDE GameFlow).
     The function's reloc-free words must equal the module Text there (ROM guard: a donor
     copy that is not spliced is refused). So `&tok + K` is GameFlow + K at run time.
  2. Type: K lands exactly on a named InputMgr field element (pad / node bytes refused) and
     the access size equals the element size.
  3. Compile: a verify copy with the rewrite is compiled with the object's own recipe (the
     name-placeholder-calls.py pipeline: asm-processor + IDO flags from the Makefile, or the
     NCC .cpp recipe) and must produce the SAME .text/.data/.rodata bytes, the same relocation
     list (offset, type, symbol) and the same defined symbols as a verify copy of the
     original file. If the whole file differs, each function's sites are tried alone and only
     the functions that compile identically are kept.
  Sites inside `#ifdef NON_MATCHING` / `#if 0` blocks (not compiled) are left.

Usage (project root, after a full build):
  scripts/gameflow-import-sites.py plan [FILE..] [--write] [-v]
      default FILE.. = every src .c/.cpp outside src/match_imports that uses an
      import_8002xxxx / gInputMgr_8002xxxx token in GameFlow's address range.
  scripts/gameflow-import-sites.py check [FILE..]
      every INPUT_MGR(tok) use (tok = an address-range token, not the shared D_00000000)
      must have tok's relocs in its function target GameFlow in the ROM (fast, no compile).
      Exit 1 on a mismatch. Shared / per-site tokens (D_00000000 / gInputMgr, import_00020098,
      D_<mod>_<addr>_b) name other objects too, so each such use is proven on its own site: an
      LO16 reloc against the token in the function whose ROM immediate falls on the field (or a
      base `addiu %lo(tok)` with immediate 0) must target bootup Bss+0xF8; if they all target
      something else it is a mismatch, if there is none (donor-spliced / -O0 alias) it is
      counted as unproven (`check -v` lists them). agent-a run 540. Tested: INPUT_MGR(import_8002022C) in MGRRaceProcess_ctor (that
      token's relocs target bootup Bss+0x28C) fails with the 8 reloc sites listed.

src/match_imports units are native relink objects (no module Text mapping) and are skipped.
agent-a run 526.
"""
import argparse
import collections
import concurrent.futures
import importlib.util
import os
import re
import sys

VENV = '/home/danjs/code/decomp/.venv'
try:
    import crunch64  # noqa: F401
    import elftools  # noqa: F401
except ImportError:
    if os.path.exists(VENV + '/bin/python') and not os.environ.get('GFI_REEXEC'):
        os.environ['GFI_REEXEC'] = '1'
        os.execv(VENV + '/bin/python', [VENV + '/bin/python'] + sys.argv)
    sys.exit('needs pyelftools + crunch64: run with /home/danjs/code/decomp/.venv/bin/python')

from elftools.elf.elffile import ELFFile

HERE = os.path.dirname(os.path.abspath(__file__))


def _load(name, fn):
    spec = importlib.util.spec_from_file_location(name, os.path.join(HERE, fn))
    m = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(m)
    return m


NPC = _load('npc', 'name-placeholder-calls.py')

BASE = 0x80020098
HEADER = 'include/game/input.h'
STRUCT = 'InputMgr'
MACRO = 'INPUT_MGR'
TARGET = ('bootup', 'Bss+0xF8')
TOK_RE = re.compile(r'\b(?:import|gInputMgr)_(8002[0-9A-F]{4})\b')
INCLUDE_C = '#include "game/input.h"'
INCLUDE_CPP = '#include "../../include/game/input.h"  /* NCC donors build without -I include */'

SIZE = {'char': 1, 'signed char': 1, 'unsigned char': 1, 'u8': 1, 's8': 1,
        'short': 2, 'unsigned short': 2, 's16': 2, 'u16': 2,
        'int': 4, 'unsigned int': 4, 'unsigned': 4, 's32': 4, 'u32': 4, 'long': 4, 'unsigned long': 4,
        'float': 4, 'f32': 4}
NORM = {'s32': 'int', 'long': 'int', 'u32': 'unsigned int', 'unsigned': 'unsigned int',
        'unsigned long': 'unsigned int', 'u8': 'unsigned char', 's8': 'signed char',
        's16': 'short', 'u16': 'unsigned short', 'f32': 'float'}


def norm_type(t):
    t = ' '.join(t.replace('*', ' * ').split())
    t = re.sub(r"\bconst\s+", "", t)
    stars = t.count('*')
    base = t.replace('*', '').strip()
    return NORM.get(base, base) + ' ' + '*' * stars if stars else NORM.get(base, base)


def type_size(t):
    n = norm_type(t)
    if '*' in n:
        return 4
    return SIZE.get(n)


# ---------------------------------------------------------------- struct layout
def parse_struct():
    src = open(HEADER).read()
    mm = re.search(r'typedef struct ' + STRUCT + r' \{(.*?)\n\} ' + STRUCT + ';', src, re.S)
    body = NPC.mask(mm.group(1))
    fields, off = [], 0
    for decl in body.split(';'):
        d = ' '.join(decl.split())
        if not d:
            continue
        dm = re.fullmatch(r'(.+?)\s*(\**)\s*(\w+)\s*(?:\[\s*([^\]]+)\s*\])?', d)
        if not dm:
            sys.exit('cannot parse InputMgr member: ' + d)
        base, stars, name, dim = dm.groups()
        typ = (base + ' ' + stars).strip() if stars else base
        es = type_size(typ)
        if es is None:
            sys.exit('unknown member type: ' + typ)
        n = int(eval(dim, {'__builtins__': {}})) if dim else 1
        off = (off + es - 1) // es * es
        fields.append((off, name, typ, es, n if dim else None))
        off += es * n
    if off != 0x194:
        sys.exit('InputMgr size parsed as 0x%X, expected 0x194' % off)
    return fields


FIELDS = None


def field_at(k, size):
    """-> (name, type, index or None) for a size-byte access at offset k, else None."""
    for off, name, typ, es, n in FIELDS:
        if name.startswith('pad') or name == 'node':
            continue
        cnt = n or 1
        if off <= k < off + es * cnt and (k - off) % es == 0 and es == size:
            return name, typ, (k - off) // es if n else None
    return None


# ---------------------------------------------------------------- source scanning
TYPE = r'((?:(?:unsigned|signed|volatile|const|struct)\s+)*\w+(?:\s*\*)*)'
NUM = r'((?:0[xX][0-9A-Fa-f]+|\d+)(?:\s*[-+*/]\s*(?:0[xX][0-9A-Fa-f]+|\d+))*)'


def cval(e):
    if not re.fullmatch(r'[0-9A-Fa-fxX+\-*/ ()]+', e):
        return None
    v = eval(re.sub(r'(?<![\w])0[xX]', '0x', e).replace('/', '//'), {'__builtins__': {}})
    return int(v)


def nm_regions(src):
    """[(a, b)] spans of `#ifdef NON_MATCHING` / `#if 0` true branches (not compiled)."""
    out, stack = [], []
    for mt in re.finditer(r'^[ \t]*#[ \t]*(ifdef|ifndef|if|elif|else|endif)\b([^\n]*)', src, re.M):
        kw, rest = mt.group(1), mt.group(2).strip()
        if kw in ('ifdef', 'ifndef', 'if'):
            dead = (kw == 'ifdef' and rest.startswith('NON_MATCHING')) or \
                (kw == 'if' and re.match(r'(0\b|defined\s*\(?\s*NON_MATCHING)', rest))
            stack.append([mt.end() if dead else None, kw == 'ifndef' and rest.startswith('NON_MATCHING')])
        elif kw in ('else', 'elif') and stack:
            if stack[-1][0] is not None:
                out.append((stack[-1][0], mt.start()))
                stack[-1][0] = None
            elif stack[-1][1]:
                stack[-1][0] = mt.end()
                stack[-1][1] = False
        elif kw == 'endif' and stack:
            st = stack.pop()
            if st[0] is not None:
                out.append((st[0], mt.start()))
    return out


def decls(m, tok):
    """[(pos, kind, info)] declarations of tok: kind 'scalar' (elem type), 'array' (elem
    type), 'struct' ({field: (offset, type)})."""
    res = []
    for mt in re.finditer(r'extern\s+([^;{}()]*?|struct\s*\{[^{}]*\})\s*\b' + tok + r'\b\s*(\[\s*\])?\s*;', m):
        t = ' '.join(mt.group(1).split())
        if t.startswith('struct') and '{' in t:
            flds, off = {}, 0
            for d in t[t.index('{') + 1:t.rindex('}')].split(';'):
                dm = re.fullmatch(r'\s*(.+?)\s*(\**)\s*(\w+)\s*(?:\[\s*([^\]]+)\s*\])?\s*', d)
                if not dm:
                    continue
                typ = (dm.group(1) + ' ' + dm.group(2)).strip() if dm.group(2) else dm.group(1)
                es = type_size(typ)
                if es is None:
                    flds = None
                    break
                n = cval(dm.group(4)) if dm.group(4) else 1
                off = (off + es - 1) // es * es
                flds[dm.group(3)] = (off, typ)
                off += es * n
            if flds:
                res.append((mt.start(), 'struct', flds))
        elif type_size(t) is not None:
            res.append((mt.start(), 'array' if mt.group(2) else 'scalar', t))
    return res


def decl_at(dl, pos):
    best = None
    for p, kind, info in dl:
        if p < pos:
            best = (kind, info)
    return best


class GSite:
    __slots__ = ('pos', 'end', 'tok', 'k', 'typ', 'func', 'repl', 'why')

    def __init__(self, pos, end, tok, k, typ):
        self.pos, self.end, self.tok, self.k, self.typ = pos, end, tok, k, typ
        self.func = self.repl = self.why = None


def scan(src, toks):
    m = NPC.mask(src)
    if not toks:    # an empty alternation `()` would match everywhere (KeyError '' in dls)
        return [], m
    dead = nm_regions(src)
    blocks = [b for b in NPC.top_blocks(m) if NPC.func_header_name(m, b[0], b[1])]
    T = '(' + '|'.join(sorted(map(re.escape, toks))) + ')'
    out, taken = [], []

    def add(mt, tok, k, typ):
        if any(a < mt.end() and mt.start() < b for a, b in taken):
            return
        taken.append((mt.start(), mt.end()))
        s = GSite(mt.start(), mt.end(), tok, k, typ)
        out.append(s)

    dls = {t: decls(m, t) for t in toks}
    # R1 *(T *)((char *)&tok + K)
    for mt in re.finditer(r'\*\s*\(\s*' + TYPE + r'\s*\*\s*\)\s*\(\s*\(\s*(?:char|u8|s8|unsigned\s+char)\s*\*\s*\)\s*&\s*'
                          + T + r'\s*\+\s*' + NUM + r'\s*\)', m):
        add(mt, mt.group(2), cval(mt.group(3)), mt.group(1))
    # R2 *(T *)(&tok + K)   (scaled by tok's scalar type)
    for mt in re.finditer(r'\*\s*\(\s*' + TYPE + r'\s*\*\s*\)\s*\(\s*&\s*' + T + r'\s*\+\s*' + NUM + r'\s*\)', m):
        d = decl_at(dls[mt.group(2)], mt.start())
        if d and d[0] == 'scalar':
            add(mt, mt.group(2), cval(mt.group(3)) * type_size(d[1]), mt.group(1))
    # R3 *(T *)&tok
    for mt in re.finditer(r'\*\s*\(\s*' + TYPE + r'\s*\*\s*\)\s*&\s*' + T + r'\b(?!\s*[\[.])', m):
        d = decl_at(dls[mt.group(2)], mt.start())
        if d and d[0] == 'scalar':
            add(mt, mt.group(2), 0, mt.group(1))
    # R4 tok[i]   (array of a scalar type; not under &)
    for mt in re.finditer(r'\b' + T + r'\s*\[\s*' + NUM + r'\s*\]', m):
        d = decl_at(dls[mt.group(1)], mt.start())
        if d and d[0] == 'array' and not re.search(r'&\s*$', m[:mt.start()]):
            add(mt, mt.group(1), cval(mt.group(2)) * type_size(d[1]), d[1])
    # R5 tok.f   (anonymous padded struct)
    for mt in re.finditer(r'\b' + T + r'\s*\.\s*(\w+)\b', m):
        d = decl_at(dls[mt.group(1)], mt.start())
        if d and d[0] == 'struct' and mt.group(2) in d[1] and not re.search(r'&\s*$', m[:mt.start()]):
            off, typ = d[1][mt.group(2)]
            add(mt, mt.group(1), off, typ)
    # R7 accessor macros of this file: FW(&tok, K) / I(&tok, K) = an int at tok + K
    accs = {mt.group(1) for mt in re.finditer(
        r'^[ \t]*#[ \t]*define[ \t]+(\w+)\((\w+),\s*(\w+)\)\s*(?:\(\*\(\s*(?:int|s32)\s*\*\)\s*\(\(char \*\)\s*\(?\2\)?\s*\+\s*\(?\3\)?\)\)'
        r'|AT\(\s*(?:int|s32)\s*,\s*\2\s*,\s*\3\s*\))\s*$', src, re.M)}
    if accs:
        for mt in re.finditer(r'\b(' + '|'.join(sorted(accs)) + r')\s*\(\s*&\s*' + T + r'\s*,\s*' + NUM + r'\s*\)', m):
            add(mt, mt.group(2), cval(mt.group(3)), 'int')
    # R8 an address: (char *)&tok + K / &tok + K (out-parameter, list head ...): &field
    for mt in re.finditer(r'(\(\s*char\s*\*\s*\)\s*)?&\s*' + T + r'\s*\+\s*' + NUM + r'(?![\w\s]*[*/\[])', m):
        d = decl_at(dls[mt.group(2)], mt.start())
        if mt.group(1):
            k = cval(mt.group(3))
        elif d and d[0] == 'scalar':
            k = cval(mt.group(3)) * type_size(d[1])
        else:
            continue
        add(mt, mt.group(2), k, '&' + ('char *' if mt.group(1) else ''))
    # R6 bare scalar tok (a value, not &tok / a call / a member)
    for mt in re.finditer(r'\b' + T + r'\b', m):
        d = decl_at(dls[mt.group(1)], mt.start())
        if not d or d[0] != 'scalar' or NPC.is_decl(m, mt.start()):
            continue
        if re.search(r'(&|\b' + MACRO + r'\s*\()\s*$', m[max(0, mt.start() - 40):mt.start()]) or \
                re.match(r'\s*[\[.(]', m[mt.end():]):
            continue
        add(mt, mt.group(1), 0, d[1])
    res = []
    for s in sorted(out, key=lambda s: s.pos):
        blk = next((b for b in blocks if b[1] < s.pos < b[2]), None)
        if blk is None:
            continue
        s.func = NPC.func_header_name(m, blk[0], blk[1])
        if any(a <= s.pos < b for a, b in dead):
            s.why = 'inside #ifdef NON_MATCHING / #if 0 (not compiled)'
        res.append(s)
    return res, m


def replacement(s):
    if s.typ.startswith('&'):
        f = next(((name, typ, None) for off, name, typ, es, n in FIELDS
                  if off == s.k and not name.startswith('pad') and name != 'node'), None)
        if f is None:
            return None
        ref = f'&{MACRO}({s.tok}).{f[0]}' + ('[0]' if any(x[1] == f[0] and x[4] for x in FIELDS) else '')
        return f'(char *){ref}' if s.typ == '&char *' else ref
    size = type_size(s.typ)
    f = field_at(s.k, size) if size and s.k is not None else None
    if f is None:
        return None
    name, ftyp, idx = f
    ref = f'{MACRO}({s.tok}).{name}' + (f'[{idx}]' if idx is not None else '')
    if norm_type(ftyp) == norm_type(s.typ):
        return ref
    t = norm_type(s.typ) if False else ' '.join(s.typ.replace('*', ' * ').split())
    stars = t.count('*')
    t = t.replace('*', '').strip()
    return f'*({t} {"*" * (stars + 1)})&{ref}'


# ---------------------------------------------------------------- objects
def obj_digest(path):
    """bytes of the allocated sections, every relocation (section, offset, type, symbol) and the
    defined symbols (name, section, value, size)."""
    with open(path, 'rb') as f:
        e = ELFFile(f)
        secs = list(e.iter_sections())
        st = e.get_section_by_name('.symtab')
        d = {}
        for s in secs:
            if s.name in ('.text', '.data', '.rodata', '.sdata', '.sbss', '.bss', '.late_rodata'):
                d[s.name] = s.data() if s['sh_type'] != 'SHT_NOBITS' else s['sh_size']
            if s['sh_type'] in ('SHT_REL', 'SHT_RELA') and not s.name.startswith('.rel.mdebug'):
                d[s.name] = sorted((r['r_offset'], r['r_info_type'],
                                    st.get_symbol(r['r_info_sym']).name or
                                    ('sect:' + secs[st.get_symbol(r['r_info_sym'])['st_shndx']].name
                                     if isinstance(st.get_symbol(r['r_info_sym'])['st_shndx'], int) else ''))
                                   for r in s.iter_relocations())
        d['syms'] = sorted((x.name, secs[x['st_shndx']].name, x['st_value'], x['st_size'])
                           for x in st.iter_symbols()
                           if x.name and isinstance(x['st_shndx'], int) and x['st_shndx'] > 0)
    return d


def verify_obj(path, text, mv):
    NPC.compile_verify(path, text, mv)
    base = os.path.join(NPC.SCRATCH, path)
    return obj_digest(base + '.v.o')


def real_relocs(path):
    """{func: [offset in func of each reloc against sym]} keyed by sym."""
    funcs, _ = NPC.elf_info(path)
    out = collections.defaultdict(lambda: collections.defaultdict(list))
    with open(path, 'rb') as f:
        e = ELFFile(f)
        st = e.get_section_by_name('.symtab')
        for sec in e.iter_sections():
            if sec.name in ('.rel.text', '.rela.text'):
                for r in sec.iter_relocations():
                    sym = st.get_symbol(r['r_info_sym']).name
                    o = r['r_offset']
                    fn = next((n for n, (v, sz, _) in funcs.items() if v <= o < v + sz), None)
                    if fn:
                        out[sym][fn].append(o - funcs[fn][0])
    return funcs, out


def rom_proof(path, rom, md, toks):
    """{(tok, func): None if proven else reason}."""
    obj = NPC.obj_of(path)
    funcs, rel = real_relocs(obj)
    rall = NPC.reloc_offsets(obj)
    rmap = {o: f1 for f1, o, _ in md.relocs('Text')}
    res = {}
    for tok in toks:
        for fn, offs in rel.get(tok, {}).items():
            base = rom.func_off(md, fn)
            if base is None:
                res[(tok, fn)] = f'{fn} not (uniquely) in the {md.short} name table'
                continue
            v, sz, b = funcs[fn]
            if not NPC.rom_bytes_match(md, base, b, v, rall):
                res[(tok, fn)] = f'{fn}: real object bytes are not the {md.short} Text (unspliced donor?)'
                continue
            bad = []
            for o in offs:
                f1 = rmap.get(base + o)
                if f1 is None:
                    bad.append('no ROM reloc at Text+0x%X' % (base + o))
                    continue
                ex, nm = rom.res.target(md, f1 >> 4)
                if (ex.short if ex else None, nm) != TARGET:
                    bad.append('Text+0x%X -> %s:%s' % (base + o, ex.short if ex else '?', nm))
            res[(tok, fn)] = '; '.join(sorted(set(bad))) or None
    return res


def file_tokens(src):
    return sorted({mt.group(0) for mt in TOK_RE.finditer(NPC.mask(src))
                   if BASE <= int(mt.group(1), 16) < BASE + 0x194})


def default_files():
    out = []
    for root, _, fs in os.walk('src'):
        if root.startswith('src/match_imports'):
            continue
        for f in fs:
            if f.endswith(('.c', '.cpp')):
                p = os.path.join(root, f)
                if file_tokens(open(p, errors='replace').read()):
                    out.append(p)
    return sorted(out)


def apply(src, sites):
    out = src
    for s in sorted(sites, key=lambda s: s.pos, reverse=True):
        out = out[:s.pos] + s.repl + out[s.end:]
    return out


def add_include(path, src):
    if 'game/input.h"' in src:
        return src
    line = (INCLUDE_CPP if path.endswith('.cpp') else INCLUDE_C) + '\n'
    i = NPC.include_point(src)
    if i and src[i - 1] != '\n':
        line = '\n' + line
    return src[:i] + line + src[i:]


def plan_file(path, rom, mv):
    out = {'path': path, 'done': [], 'left': [], 'error': None, 'text': None}
    src = open(path, errors='replace').read()
    toks = file_tokens(src)
    if not toks:
        return out
    sites, m = scan(src, toks)
    if not sites:
        return out
    md = rom.module_for(path)
    if not os.path.exists(NPC.obj_of(path)):
        out['error'] = 'no built object (never-built reference donor)'
        out['left'] = [(s, 'no object') for s in sites]
        return out
    if md is None or not md.funcs:
        out['error'] = 'no USO module mapped'
        out['left'] = [(s, 'no module') for s in sites]
        return out
    proof = rom_proof(path, rom, md, toks)
    cand = []
    for s in sites:
        if s.why:
            out['left'].append((s, s.why))
            continue
        p = proof.get((s.tok, s.func), 'no reloc against the token in the real function')
        if p:
            out['left'].append((s, p))
            continue
        s.repl = replacement(s)
        if s.repl is None:
            out['left'].append((s, 'offset 0x%X / %s is not a named InputMgr field element' % (s.k, s.typ)))
            continue
        cand.append(s)
    if not cand:
        return out
    try:
        ref = verify_obj(path, src, mv)
    except RuntimeError as ex:
        out['error'] = 'original does not compile in the verify pipeline: ' + str(ex)
        out['left'] += [(s, 'verify') for s in cand]
        return out

    def ok(ss):
        try:
            return verify_obj(path, add_include(path, apply(src, ss)), mv) == ref
        except RuntimeError:
            return False
    if not ok(cand):
        keep = []
        byf = collections.defaultdict(list)
        for s in cand:
            byf[s.func].append(s)
        for fn, ss in byf.items():
            if ok(ss):
                keep += ss
            else:
                out['left'] += [(s, f'{fn}: rewritten copy compiles differently') for s in ss]
        cand = keep
        if cand and not ok(cand):
            out['left'] += [(s, 'combined rewrite compiles differently') for s in cand]
            cand = []
    out['done'] = cand
    if cand:
        out['text'] = add_include(path, apply(src, cand))
    return out


def cmd_plan(a, rom, mv):
    files = a.files or default_files()
    with concurrent.futures.ThreadPoolExecutor(max_workers=8) as ex:
        res = list(ex.map(lambda p: plan_file(p, rom, mv), files))
    tot_d = tot_l = 0
    for r in res:
        d, l = len(r['done']), len(r['left'])
        tot_d += d
        tot_l += l
        if not (d or l or r['error']):
            continue
        print(f"{r['path']}: {d} spelled, {l} left" + (f"  [{r['error']}]" if r['error'] else ''))
        if a.verbose:
            src = open(r['path'], errors='replace').read()
            for s in r['done']:
                print(f"   + {s.func}: {' '.join(src[s.pos:s.end].split())}  ->  {s.repl}")
        reasons = collections.Counter(why.split(': ', 1)[-1] if ': ' in why else why for _, why in r['left'])
        for why, n in reasons.most_common():
            print(f'   - {n} x {why}')
        if a.write and r['text']:
            open(r['path'], 'w').write(r['text'])
    print(f'total: {tot_d} spelled, {tot_l} left' + ('' if a.write else ' (dry run; --write to apply)'))


USE_RE = re.compile(r'\b' + MACRO + r'\s*\(\s*(\w+)\s*\)')
FIELD_USE_RE = re.compile(r'\s*\.\s*(\w+)\s*(?:\[\s*([^\]]*)\])?')
# source spellings that are macros for a relocated symbol (include/gdl/gl_data.h)
SYM_ALIAS = {'gInputMgr': 'D_00000000'}


def use_range(m, end):
    """byte range [lo, hi) inside GameFlow that `INPUT_MGR(tok)` + the member access after `end`
    names (a constant index narrows it to one element), or None"""
    fm = FIELD_USE_RE.match(m, end)
    if not fm:
        return None
    f = next((x for x in FIELDS if x[1] == fm.group(1)), None)
    if f is None:
        return None
    off, name, typ, es, n = f
    if n and fm.group(2) is not None:
        i = cval(fm.group(2))
        if i is not None:
            return off + es * i, off + es * (i + 1)
    return off, off + es * (n or 1)


def lo16_proof(path, rom, md, sym, fn, rng, cache):
    """A shared / per-site token (D_00000000, import_00020098, D_<mod>_<addr>_b) names other
    objects too, so the function-wide rom_proof is not enough: prove the SITE. Its LO16 relocs
    against sym in fn whose ROM immediate (the offset inside the target, kind-2 Sym) falls in
    rng: ok if one of them targets GameFlow; 'mismatch: ...' if they all target something else;
    'unproven: ...' if there is none (or the function is not provably the module Text)."""
    obj = NPC.obj_of(path)
    if obj not in cache:
        funcs, _ = NPC.elf_info(obj)
        rel = collections.defaultdict(lambda: collections.defaultdict(list))
        with open(obj, 'rb') as f:
            e = ELFFile(f)
            st = e.get_section_by_name('.symtab')
            for sec in e.iter_sections():
                if sec.name in ('.rel.text', '.rela.text'):
                    for r in sec.iter_relocations():
                        if r['r_info_type'] != 6:     # R_MIPS_LO16
                            continue
                        o = r['r_offset']
                        f1 = next((nm for nm, (v, sz, _) in funcs.items() if v <= o < v + sz), None)
                        if f1:
                            rel[st.get_symbol(r['r_info_sym']).name][f1].append(o - funcs[f1][0])
        cache[obj] = (funcs, rel, NPC.reloc_offsets(obj), {o: f1 for f1, o, _ in md.relocs('Text')})
    funcs, rel, rall, rmap = cache[obj]
    if fn not in funcs:
        return 'unproven: %s not in the object' % fn
    base = rom.func_off(md, fn)
    v, sz, b = funcs[fn]
    if base is None or not NPC.rom_bytes_match(md, base, b, v, rall):
        return 'unproven: %s is not provably the %s Text' % (fn, md.short)
    text = md.secs.get('Text', b'')
    hit, other, bhit, bother = False, set(), False, set()
    for o in rel.get(sym, {}).get(fn, []):
        if base + o + 4 > len(text) or base + o not in rmap:
            continue
        imm = int.from_bytes(text[base + o + 2:base + o + 4], 'big', signed=True)
        if not (rng[0] <= imm < rng[1] or imm == 0):
            continue
        ex, nm = rom.res.target(md, rmap[base + o] >> 4)
        ok = (ex.short if ex else None, nm) == TARGET
        why = 'Text+0x%X -> %s:%s+0x%X' % (base + o, ex.short if ex else '?', nm, imm)
        if rng[0] <= imm < rng[1]:
            hit, other = hit or ok, other if ok else other | {why}
        else:
            # `addiu rX, %lo(sym)` = the object's base, the field offset in the load / store after it
            bhit, bother = bhit or ok, bother if ok else bother | {why}
    if hit:
        return None
    if other:
        return 'mismatch: ' + '; '.join(sorted(other))
    if bhit and not bother:
        return 'base'
    if bhit:    # sym is GameFlow's base at one site and another object's at another: not provable here
        return 'unproven: %s is a base for GameFlow and for %s' % (sym, '; '.join(sorted(bother)))
    if bother:
        return 'mismatch: ' + '; '.join(sorted(bother))
    return 'unproven: no LO16 reloc against %s at GameFlow+[0x%X, 0x%X)' % (sym, rng[0], rng[1])


def cmd_check(a, rom, mv):
    files = a.files or sorted({p for p in default_files()} |
                              {os.path.join(r, f) for r, _, fs in os.walk('src') for f in fs
                               if f.endswith(('.c', '.cpp')) and not r.startswith('src/match_imports')
                               and MACRO + '(' in open(os.path.join(r, f), errors='replace').read()})
    bad = n = ns = nsp = nsb = nsu = 0
    for path in files:
        src = open(path, errors='replace').read()
        m = NPC.mask(src)
        dead = nm_regions(src)
        blocks = [b for b in NPC.top_blocks(m) if NPC.func_header_name(m, b[0], b[1])]
        uses, shared = [], []
        for mt in USE_RE.finditer(m):
            blk = next((b for b in blocks if b[1] < mt.start() < b[2]), None)
            if not blk or any(x <= mt.start() < y for x, y in dead):
                continue
            fn = NPC.func_header_name(m, blk[0], blk[1])
            tm = TOK_RE.fullmatch(mt.group(1))
            if tm and BASE <= int(tm.group(1), 16) < BASE + 0x194:
                uses.append((mt.group(1), fn))
            else:
                # a shared placeholder / per-site token: proven per site (lo16_proof)
                line = src.count('\n', 0, mt.start()) + 1
                shared.append((mt.group(1), fn, use_range(m, mt.end()), line))
        if not uses and not shared:
            continue
        md = rom.module_for(path)
        if md is None or not os.path.exists(NPC.obj_of(path)):
            continue
        cache = {}
        for tok, fn, rng, line in shared:
            ns += 1
            if rng is None:
                p = 'unproven: no InputMgr member access after %s(%s)' % (MACRO, tok)
            else:
                p = lo16_proof(path, rom, md, SYM_ALIAS.get(tok, tok), fn, rng, cache)
            if p is None:
                nsp += 1
            elif p == 'base':
                nsb += 1
            elif p.startswith('mismatch'):
                bad += 1
                print(f'{path}:{line}: {fn}: {MACRO}({tok}): {p}')
            else:
                nsu += 1
                if a.verbose:
                    print(f'{path}:{line}: {fn}: {MACRO}({tok}): {p}')
        if not uses:
            continue
        proof = rom_proof(path, rom, md, {t for t, _ in uses})
        for tok, fn in uses:
            n += 1
            p = proof.get((tok, fn), 'no reloc against the token in the real function')
            # a donor-spliced or otherwise unprovable function is not a mismatch: only a ROM
            # target that is NOT GameFlow is
            if p and '->' in p:
                bad += 1
                print(f'{path}: {fn}: {MACRO}({tok}): {p}')
    print(f'check: {n} {MACRO} uses of GameFlow address tokens + {ns} of shared / per-site tokens '
          f'({nsp} proven on the field\'s LO16 reloc, {nsb} on a base `addiu %lo` reloc, {nsu} unproven{"" if a.verbose else " (-v lists them)"}), '
          f'{bad} mismatches')
    return 1 if bad else 0


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--no-build', action='store_true')
    sp = ap.add_subparsers(dest='cmd', required=True)
    p = sp.add_parser('plan')
    p.add_argument('files', nargs='*')
    p.add_argument('--write', action='store_true')
    p.add_argument('-v', '--verbose', action='store_true')
    p = sp.add_parser('check')
    p.add_argument('files', nargs='*')
    p.add_argument('-v', '--verbose', action='store_true')
    a = ap.parse_args()
    global FIELDS
    FIELDS = parse_struct()
    NPC.ensure_build(a.no_build)
    rom, mv = NPC.Rom(), NPC.MakeVars()
    if a.cmd == 'plan':
        cmd_plan(a, rom, mv)
    else:
        sys.exit(cmd_check(a, rom, mv))


if __name__ == '__main__':
    main()
