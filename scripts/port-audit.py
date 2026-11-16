#!/usr/bin/env python3
"""Port-readiness audit: pointer <-> int casts that break on a 64-bit host (agent-a run 536).

  scripts/port-audit.py [--list PATTERN] [--owner X] [--top N] [--md]

Run from the project root after a full build (the readability gate leaves one). Report only:
nothing is changed. Scans COMPILED code the same way scripts/readability-census.py does (files
with a matching-build object; NON_MATCHING bodies, `#if 0` and never-defined-macro blocks blanked;
comments and strings masked), plus every header under include/ and src/ for pattern H.

Patterns (each site is attributed to its function and its owner area with the census owner()):
  A  addr->int     `(s32)&X` / `(int)&X` / `(u32)&X`: an address squeezed into a 32-bit int
                   (A* = the cast is the right-hand side of a store `= (s32)&X`)
  B  ptrexpr->int  `(s32)(... (char *)p + K ...)` / `(int)ptr_expr`: int cast of a pointer-typed
                   expression (a `(T *)` cast, `&`, or a string literal inside the cast operand)
  C  int-as-ptr    `*(int *)(p + K)` (int / s32 / u32) loads whose value is used as a pointer:
                   cast to a pointer type `(T *)*(int *)(...)`, or used as the base of another
                   raw access `*(T *)(*(int *)(...) + K)` / `((T *)*(int *)(...))`
  D  ptr@offset    `*(T **)(p + K)`: a pointer loaded at a hard-coded byte offset (the layout
                   assumes 4-byte pointers; a struct field moves with the pointer size, K does not)
  H  int field     a header struct field declared s32 / int / u32 whose name or comment says it
                   holds a pointer (`Foo *` / pointer / ptr / -> / vtable in the comment, or a
                   name such as vptr / parent / next / children / owner / *Ptr)

A, B and H truncate (or sign-extend) a 64-bit pointer; C and D read a pointer with the wrong
width or at the wrong offset. Many of these spellings are load-bearing for the matching build
(an int-cast base colours a different register, see IDO_CODEGEN.md), so a 64-bit port needs them
behind a PORT / NON_MATCHING path or typed once the matching constraint is lifted.

Without --list / --top: a Markdown summary (pattern x owner table, the top 15 files and functions).
--list P prints every site of pattern P (A, B, C, D or H) as path:line function: text.
--top N prints a work list instead: the N functions with the most A-D sites, one per line,
        `total  A B C D  owner  path:line  function` (line = the function header; --owner X
        narrows it to one area). With --md, --top N sets the length of the summary's tables.
"""
import collections
import importlib.util
import os
import re
import sys

HERE = os.path.dirname(os.path.abspath(__file__))


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


CEN = _load('census', 'readability-census.py')
NPC, DB = CEN.NPC, CEN.DB

INT_T = r'(?:s32|int|u32|unsigned\s+int|unsigned|signed\s+int|long|unsigned\s+long|uintptr_t)'
# A: (s32)&X
ADDR_CAST = re.compile(r'\(\s*' + INT_T + r'\s*\)\s*(?:\(\s*)?&(?!&)')
# B: (s32)( ... ) whose operand holds a pointer cast / & / string
PTR_CAST_OPEN = re.compile(r'\(\s*' + INT_T + r'\s*\)\s*\(')
PTR_HINT = re.compile(r'\(\s*(?:const\s+|volatile\s+|unsigned\s+|signed\s+|struct\s+|union\s+)*\w+\s*\*+\s*\)|"')
# B operand: starts with a pointer cast that is not dereferenced, `(T *)p + K` / `((char *)p + K)`
PTR_OPERAND = re.compile(r'\s*(?:\(\s*)*\(\s*(?:const\s+|volatile\s+|unsigned\s+|signed\s+|struct\s+|union\s+)*\w+\s*\*+\s*\)')
# unary & (address-of): the previous non-space character is not an operand end
UNARY_AMP = re.compile(r'(?:^|[^\w)\]\s&]|[^\w)\]&]\s+)&(?!&)\s*[A-Za-z_(]')
# C / D: *(int *)( and *(T **)(
INT_LOAD = re.compile(r'\*\s*\(\s*' + INT_T + r'\s*\*\s*\)\s*\(')
PTR_LOAD = re.compile(r'\*\s*\(\s*(?:const\s+|volatile\s+|unsigned\s+|signed\s+|struct\s+|union\s+)*\w+\s*\*\s*\*+\s*\)\s*\(')
TAIL_OFF = re.compile(r'[+-]\s*(?:0x[0-9A-Fa-f]+|\d+)\s*[uUlL]*\s*$')
PTR_TYPE_CAST_BEFORE = re.compile(r'\(\s*(?:const\s+|volatile\s+|unsigned\s+|signed\s+|struct\s+|union\s+)*\w+\s*\*+\s*\)\s*$')

FIELD = re.compile(r'^[ \t]*(?:/\*[^\n]*?\*/[ \t]*)?(?:volatile\s+)?(' + INT_T + r')[ \t]+(\w+)[ \t]*(\[[^\]\n]*\])?[ \t]*;[ \t]*(/\*.*)?$', re.M)
PTR_NAME = re.compile(r'(?i:(?:^|_)(?:vptr|vtbl|vtable|parent|children|next|prev|owner|ptr))$|Ptr$|^p[A-Z]')
PTR_COMMENT = re.compile(r'\b[A-Z]\w*[A-Za-z0-9]\s*\*+(?=[\s),;]|$)|\bchar\s*\*|\bvoid\s*\*|\bpointer\b|\bptr\b')

NAMES = {'A': '`(s32)&X` address -> int', 'B': 'pointer expression -> int', 'C': '`*(int *)` load used as a pointer',
         'D': '`*(T **)(p + K)` pointer at a fixed offset', 'H': 'header field: int that holds a pointer'}


def close_paren(code, i):
    """index just past the ')' matching the '(' before position i (i = just after the '(')."""
    depth = 1
    while i < len(code) and depth:
        depth += {'(': 1, ')': -1}.get(code[i], 0)
        i += 1
    return i


def scan_code(code):
    """[(pattern, pos, store?)] for one masked compiled source."""
    out = []
    for m in ADDR_CAST.finditer(code):
        pre = code[max(0, m.start() - 4):m.start()]
        out.append(('A', m.start(), bool(re.search(r'[^=!<>]=\s*$', pre))))
    for m in PTR_CAST_OPEN.finditer(code):
        end = close_paren(code, m.end())
        inner = code[m.end():end - 1]
        if PTR_OPERAND.match(inner) and not ADDR_CAST.match(code, m.start()):
            out.append(('B', m.start(), False))
    for m in INT_LOAD.finditer(code):
        end = close_paren(code, m.end())
        before = code[max(0, m.start() - 80):m.start()]
        after = code[end:end + 40]
        as_ptr = bool(PTR_TYPE_CAST_BEFORE.search(before))
        # base of another raw access: `*(T *)(` <this> ` + K)` or `(char *)` <this> handled above
        if not as_ptr and re.match(r'\s*[+-]\s*(?:0x[0-9A-Fa-f]+|\d+)\s*\)', after):
            k = before.rstrip()
            if re.search(r'\*\s*\(\s*[\w\s]+\*+\s*\)\s*\(\s*$', k):
                as_ptr = True
        if not as_ptr and re.match(r'\s*\)\s*->', after):
            as_ptr = True
        if as_ptr:
            out.append(('C', m.start(), False))
    for m in PTR_LOAD.finditer(code):
        end = close_paren(code, m.end())
        inner = code[m.end():end - 1]
        if TAIL_OFF.search(inner) and 'D_00000000' not in inner and 'gl_data' not in inner:
            out.append(('D', m.start(), False))
    return out


def scan_headers():
    """[(path, line, struct-field text)] for pattern H."""
    import glob
    out = []
    hdrs = sorted(set(glob.glob('include/**/*.h', recursive=True) + glob.glob('src/**/*.h', recursive=True)))
    for h in hdrs:
        src = open(h, errors='replace').read()
        for m in FIELD.finditer(src):
            name, cmt = m.group(2), m.group(4) or ''
            # the comment may continue on the next lines (` * ...`): take up to its end
            if cmt and '*/' not in cmt:
                e = src.find('*/', m.end())
                cmt = cmt + src[m.end():e if e >= 0 else m.end()]
            cmt = re.sub(r'\n[ \t]*\*(?!/)', ' ', cmt.replace('/*', '', 1)).replace('*/', ' ')
            if PTR_NAME.search(name) or PTR_COMMENT.search(cmt):
                out.append((h, src.count('\n', 0, m.start()) + 1, m.group(0).strip()))
    return out


def header_owner(path):
    if path.startswith('src/'):
        return CEN.owner(path, '')
    return 'hdr'


def main():
    args = sys.argv[1:]
    lst = args[args.index('--list') + 1].upper() if '--list' in args else None
    only = args[args.index('--owner') + 1] if '--owner' in args else None
    top = int(args[args.index('--top') + 1]) if '--top' in args else 15
    if not os.path.exists('build/tenshoe.elf'):
        sys.exit('no build/tenshoe.elf: run a full make (or the readability gate) first')

    defined = DB.defined_macros('.')
    by = collections.defaultdict(collections.Counter)       # pattern -> owner -> n
    stores = collections.Counter()                          # owner -> A* stores
    per_file = collections.defaultdict(collections.Counter)
    per_fn = collections.Counter()
    fn_pat = collections.defaultdict(collections.Counter)   # (p, fn, own) -> pattern -> n
    fn_line = {}
    sites = []
    for p in CEN.compiled_sources():
        src = open(p, errors='replace').read()
        live = CEN.live_mask(src, p.endswith('.cpp'), defined)
        m = NPC.mask(live)
        found = scan_code(m)
        if not found:
            continue
        blks = CEN.block_names(m)
        for pat, pos, st in found:
            b = next((x for x in blks if x[0] < pos < x[1]), None)
            fn = b[2] if b else '<file scope>'
            own = CEN.owner(p, fn)
            if only and own != only:
                continue
            by[pat][own] += 1
            if st:
                stores[own] += 1
            per_file[p][pat] += 1
            per_fn[(p, fn, own)] += 1
            fn_pat[(p, fn, own)][pat] += 1
            if b and (p, fn, own) not in fn_line:
                fn_line[(p, fn, own)] = live.count('\n', 0, b[0]) + 1
            if lst == pat:
                line = live.count('\n', 0, pos) + 1
                text = live.split('\n')[line - 1].strip()
                sites.append(f'{p}:{line} {fn}: {text[:140]}')
    hsites = scan_headers()
    for h, line, text in hsites:
        own = header_owner(h)
        if only and own != only:
            continue
        by['H'][own] += 1
        if lst == 'H':
            sites.append(f'{h}:{line}: {text[:140]}')
    if lst:
        print('\n'.join(sites))
        return
    if '--top' in args and '--md' not in args:
        for key, n in per_fn.most_common(top):
            p, fn, own = key
            c = fn_pat[key]
            print(f'{n:4d}  {c["A"]:3d} {c["B"]:3d} {c["C"]:3d} {c["D"]:3d}  {own:3s}  {p}:{fn_line.get(key, 0)}  {fn}')
        return

    owners = ['a', 'b', 'c', 'g', 'h', 'hdr']
    print('| pattern | ' + ' | '.join(owners) + ' | total |')
    print('|---|' + '--:|' * (len(owners) + 1))
    for pat in 'ABCDH':
        row = [by[pat][o] for o in owners]
        print(f'| {pat}: {NAMES[pat]} | ' + ' | '.join(str(x) for x in row) + f' | {sum(row)} |')
    print(f'\nA* (the `(s32)&X` is a store right-hand side): ' +
          ', '.join(f'{o} {stores[o]}' for o in owners if stores[o]) + f'; total {sum(stores.values())}.')
    print(f'\nOwners: the census owner() table (a / b / c / g / h); hdr = include/ headers (pattern H only).')
    print(f'\nTop {top} files (A+B+C+D):\n')
    print('| file | A | B | C | D | total |')
    print('|---|--:|--:|--:|--:|--:|')
    for p, c in sorted(per_file.items(), key=lambda kv: -sum(kv[1].values()))[:top]:
        print(f'| `{p}` | {c["A"]} | {c["B"]} | {c["C"]} | {c["D"]} | {sum(c.values())} |')
    print(f'\nTop {top} functions:\n')
    print('| function | file | owner | sites |')
    print('|---|---|--:|--:|')
    for (p, fn, own), n in per_fn.most_common(top):
        print(f'| `{fn}` | `{p}` | {own} | {n} |')
    hd = collections.Counter(os.path.dirname(h) for h, _, _ in hsites)
    print('\nPattern H per header directory: ' + ', '.join(f'`{d}` {n}' for d, n in sorted(hd.items())) + '.')


if __name__ == '__main__':
    main()
