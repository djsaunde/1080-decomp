#!/usr/bin/env python3
"""zero-lever-grid.py: rerunnable folded zero-code lever grid for NCC (C++) donors.

Builds a spelling-grid template from a donor and (optionally) runs scripts/spelling-grid.py on it.
Per site it offers one grid dimension (kit CLXXIX / CLXXVII / CLXI levers, all zero emitted code):
  * expression ends (return E / if(E) / while(E) / for-cond / assignment rhs / call args):
        ''  |  ' + 0'  |  ' + 0 * (int)(Z)' for each pure no-arg inline call Z seen earlier in the body
  * rvalue member chains a->b / a.b.c (not calls, not written, not &-taken):   X  |  (&X)[0]
Sites after a numeric literal, in comments or outside function bodies are skipped.

usage:
  scripts/zero-lever-grid.py DONOR.cpp OUT.tpl [--no-plus] [--no-addr] [--no-zero]
  scripts/zero-lever-grid.py DONOR.cpp OUT.tpl --run FN TARGET_OBJ [--rounds 2]
      (TARGET_OBJ relative to the worktree, e.g. build/src/game_libs/game_libs_post0b.c.o;
       spelling-grid runs from the monorepo, -I <donor dir> -I include are added; the best source is OUT.tpl.best)
Never point OUT at src/. cfe (.c) bodies fold all three spellings away before uopt: use this on NCC donors only.
"""
import re, sys
argv = sys.argv[1:]
run = None; rounds = '2'
if '--run' in argv:
    k = argv.index('--run'); run = (argv[k + 1], argv[k + 2]); del argv[k:k + 3]
if '--rounds' in argv:
    k = argv.index('--rounds'); rounds = argv[k + 1]; del argv[k:k + 2]
args = [a for a in argv if not a.startswith('--')]
flags = set(a for a in argv if a.startswith('--'))
s = open(args[0]).read()
cs = [(m.start(), m.end()) for m in re.finditer(r'/\*.*?\*/|//[^\n]*|"(?:\\.|[^"\\])*"', s, re.S)]
def in_comment(p): return any(a <= p < b for a, b in cs)
def match_paren(t, p):
    d = 0
    for k in range(p, len(t)):
        if t[k] in '([': d += 1
        elif t[k] in ')]':
            d -= 1
            if d == 0: return k
    return -1
# function bodies: top-level '{' after a ')' (skip struct bodies: the inline member fns inside are included too)
bodies = []
for m in re.finditer(r'\)\s*(const\s*)?\{', s):
    if in_comment(m.start()): continue
    b = m.end() - 1; d = 0
    for k in range(b, len(s)):
        if s[k] == '{': d += 1
        elif s[k] == '}':
            d -= 1
            if d == 0: bodies.append((b, k)); break
def in_body(p): return any(a < p < b for a, b in bodies)
edits = []  # (start, end, [alts]) ; insertion: start == end, alts[0] == ''
ZC = re.compile(r'\b([A-Za-z_]\w*(?:\[[^\]]*\])?(?:\.|->)([A-Za-z_]\w*)\(\))')
PURE = set()
for m in re.finditer(r'\b([A-Za-z_]\w*)\s*\(\s*\)\s*(?:const\s*)?\{\s*return\s+([^;]*);\s*\}', s):
    if not re.search(r'\+\+|--|[^=!<>]=[^=]|\w\s*\(', m.group(2)): PURE.add(m.group(1))
def zero_terms(pos):
    if '--no-zero' in flags: return []
    # body containing pos
    bb = [a for a, b in bodies if a < pos < b]
    if not bb: return []
    a = max(bb)
    seen = []
    for m in ZC.finditer(s, a, pos):
        if in_comment(m.start()): continue
        if m.group(2) in PURE and m.group(1) not in seen: seen.append(m.group(1))
    return [' + 0 * (int)(%s)' % z for z in seen[-4:]]
def ins(p, typ_ptr_ok=True):
    if in_comment(p) or not in_body(p): return
    if re.search(r'(?<![\w.])(-?(0x[0-9a-fA-F]+|\d+)[uUlLfF]*)\s*$', s[max(0, p-24):p]): return
    alts = ['']
    if '--no-plus' not in flags: alts.append(' + 0')
    alts += zero_terms(p)
    if len(alts) > 1: edits.append((p, p, alts))
for m in re.finditer(r'\b(if|while)\s*\(', s):
    q = match_paren(s, m.end() - 1)
    if q > 0 and s[m.end():q].strip() not in ('0', '1'): ins(q)
for m in re.finditer(r'\bfor\s*\(', s):
    q = match_paren(s, m.end() - 1); parts = s[m.end():q].split(';')
    if len(parts) == 3 and parts[1].strip(): ins(m.end() + len(parts[0]) + 1 + len(parts[1].rstrip()))
for m in re.finditer(r'\breturn\s+([^;]+);', s):
    if m.group(1).strip() != '0': ins(m.end() - 1)
for m in re.finditer(r'(?<![=!<>+\-*/&|^%])=(?!=)\s*([^;{}]+);', s):
    ls = s.rfind('\n', 0, m.start()) + 1
    if re.match(r"\s*(#|struct|typedef|extern)", s[ls:]): continue
    ins(m.end() - 1)
KW = ('if', 'while', 'for', 'switch', 'return', 'sizeof')
for m in re.finditer(r'\b([A-Za-z_]\w*)\s*\(', s):
    if m.group(1) in KW: continue
    q = match_paren(s, m.end() - 1)
    if q < 0: continue
    inner = s[m.end():q]
    if not inner.strip() or '{' in inner or ';' in inner: continue
    ls = s.rfind('\n', 0, m.start()) + 1; pre = s[ls:m.start()]
    if re.match(r'\s*(extern|inline|struct|void|int|float|char|unsigned|short|typedef|#|[A-Z]\w*\s*\*?\s*$)', pre) and '=' not in pre and 'return' not in pre: continue
    d = 0
    for k, ch in enumerate(inner):
        if ch in '([': d += 1
        elif ch in ')]': d -= 1
        elif ch == ',' and d == 0: ins(m.end() + k)
    ins(q)
if '--no-addr' not in flags:
    for m in re.finditer(r'(?<![\w.>&])([A-Za-z_]\w*(?:(?:->|\.)[A-Za-z_]\w*)+)(?!\s*\()(?![\w])', s):
        a, b = m.span(1)
        if in_comment(a) or not in_body(a): continue
        if re.match(r'\s*(=[^=]|\+\+|--|[-+*/|&^]=)', s[b:b+3]) : continue  # lvalue writes: keep
        if re.search(r'(\+\+|--)\s*$', s[max(0, a-3):a]): continue
        if re.match(r'^\d', m.group(1)) or '.' in m.group(1) and re.match(r'^\d+\.', m.group(1)): continue
        edits.append((a, b, [m.group(1), '(&%s)[0]' % m.group(1)]))
edits.sort(key=lambda e: (e[0], e[1]))
keep = []; last_end = -1
for e in edits:
    if e[0] < last_end or (keep and e[0] == keep[-1][0] and e[1] == keep[-1][1]): continue
    if keep and keep[-1][0] == keep[-1][1] == e[0] and e[0] != e[1]: pass
    keep.append(e); last_end = max(last_end, e[1]) if e[0] != e[1] else last_end
out = []; pos = 0
for a, b, alts in keep:
    out.append(s[pos:a]); out.append('@{ ' + ' @| '.join(alts) + ' }@'); pos = b
out.append(s[pos:])
open(args[1], 'w').write(''.join(out))
print(len(keep), 'sites')

if run:
    import os, subprocess
    wt = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    mono = os.path.dirname(os.path.dirname(wt))
    donor = os.path.abspath(args[0])
    inc = os.path.relpath(os.path.dirname(donor), wt)
    cmd = ['python3', os.path.join(mono, 'scripts', 'spelling-grid.py'), '--wt', wt, '--src', os.path.abspath(args[1]),
           '--fn', run[0], '--target', run[1], '--rounds', rounds, '--out', os.path.abspath(args[1]) + '.best',
           '--flags', '-G 0 -non_shared -Wab,-r4300_mul -O2 -mips2 -32 -I %s -I include' % inc]
    sys.exit(subprocess.call(cmd, cwd=mono))
