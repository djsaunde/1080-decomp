#!/usr/bin/env python3
"""blockcut-search.py -- brute-force uopt basic-block cut placement for ONE function.

uopt (IDO 7.1 -O2) decides register interference per basic block, and a basic
block ends at every label AND after 20 counted variable loads in straight-line
code (`varreflimit`, docs/IDO_CODEGEN.md kit XXXV).  Where the boundaries fall
picks the colours.  An empty `if (1) {}` emits no code but ends a block, so a
missing boundary can be PLACED.  This script inserts emission-free cuts at
every combination of statement positions (up to K at once), compiles each
variant in parallel OUT OF TREE (the source file is never modified), scores it
with `scripts/rdiff.py --raw --pins` (diffwords; 0 = exact) and prints the best
layouts.  It never runs `make`.

Inputs (pick one):

  --unit src/<seg>/<file> FUNC
        cfe body in-tree.  The body is taken from FUNC's `#ifdef NON_MATCHING`
        branch (or a plain definition).  Default = PROMOTE mode: only that one
        wrap is switched to `#if 1`, every other wrap stays INCLUDE_ASM, and the
        variant goes through the matching-build pipeline (asm-processor pass 1,
        cc, asm-processor post-process).  That is exactly what landing the
        function as plain C would build, rodata offsets included.  Scored vs
        expected/src/<seg>/<file>.c.o.  `--nm-tree` instead defines
        NON_MATCHING for the whole unit (the report's NM-tree object).
  --file PATH FUNC --expected expected/src/<seg>/<file>.c.o
        standalone draft or in-tree donor.  *.cpp = NCC (edg) with
        `-G 0 -non_shared -Wab,-r4300_mul -O2 -mips2 -32` (the donor rule);
        *.c = cc with the unit CFLAGS / CPPFLAGS (asm-processor pipeline if the
        file uses INCLUDE_ASM / GLOBAL_ASM).  `-I <dir of PATH>` is added.
        `--opt -O1` etc. overrides the opt level.

Compile flags for --unit come from the Makefile (global `:=` values plus the
target-specific lines naming build/src/<unit>.c.o or build/non_matching/...);
the Makefile is parsed, never executed.

Search options:
  --k K           cuts per variant, all 0..K-subsets of the positions (default 1)
  --ins 'A|B'     alternative cut texts (default 'if (1) {}'); every chosen
                  position gets one of them
  --swap 'X=>Y'   (or 'X=Y' when X has no '=') also offer replacing each occurrence of X in the body by Y
                  (e.g. 'next()=>nextb()' for an inline member that has the
                  cut between the index bump and the slot address); counts
                  towards K.  Repeatable.  An empty Y deletes X (e.g. drop
                  an existing `if (1) {}`: --swap 'if (1) {}=>').
  --lines A-B     only positions on source lines A..B (repeatable)
  --also NAME     also cut inside the body of NAME (an inline member or helper
                  such as `next()`; a cut there lands in every inlined copy,
                  e.g. between the index bump and the slot address).  For cuts
                  inside a macro, expand the draft first (`gcc -E -P`).
                  Staged searches (agent-c run 126, 1304C): run --k 1..2 on the
                  packets, keep the best, then search --also next(), then
                  inside the expanded macros.
  --greedy        after the singles, grow the best single greedily (beam 4)
                  instead of enumerating every K-subset (for big functions)
  --max N         above N exhaustive variants, switch to --greedy (default 20000)
  --jobs N        parallel compiles (default 8)
  --top N         print the N best layouts (default 10)
  --write OUT     write the best variant's full source to OUT
  --keep OBJ      keep the unmodified build's object at OBJ (source at OBJ.src)
  --verbose       print the base build's compiler errors
  --varref        DIAGNOSTIC ONLY: also compile the unmodified source with
                  `-Wo,-varref,N` for N in 12..1000 using the dump toolchain
                  (tools/ido-static-recomp-dump) and print N:score.  If some
                  N != 20 is much closer, the residual is block placement:
                  larger N helps -> the target has FEWER counted loads before
                  a boundary (remove loads); smaller N helps -> it has more
                  (place a cut).  Never put -varref in a build.

Positions are after every `;` at paren depth 0, after every `{` and after
every `}` inside FUNC's body.  In C mode a position directly followed by a
declaration is skipped (C89).  Variants that fail to compile are ignored.

Examples:
  scripts/blockcut-search.py --unit src/game_libs/game_libs_post0b gl_func_00051ED8 --k 2
  scripts/blockcut-search.py --file src/gui_uso/gui_uso_ncc_16A4.cpp gui_uso_func_000016A4 \\
      --expected expected/src/gui_uso/gui_uso.c.o --k 2 --swap 'next()=>nextb()'
  scripts/blockcut-search.py --unit src/game_uso/game_uso game_uso_func_00001DDC --varref --k 0

Caveats: an explicit cut that a live PARAMETER crosses can home the
parameters at entry (kit XXXV #4); the 20-load route has no labels.  A result
of 0 in promote mode is a landable exact (still run the full gate).
"""
import argparse
import itertools
import math
import os
import re
import shlex
import shutil
import subprocess
import sys
import tempfile
from concurrent.futures import ThreadPoolExecutor

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
DUMP_DIR = os.path.join(ROOT, '..', '..', 'tools', 'ido-static-recomp-dump', 'build', '7.1', 'out')
CTYPES = {'int', 'char', 'short', 'long', 'float', 'double', 'void', 'unsigned', 'signed',
          'struct', 'union', 'enum', 'register', 'static', 'const', 'volatile', 'extern',
          's8', 'u8', 's16', 'u16', 's32', 'u32', 's64', 'u64', 'f32', 'f64', 'typedef'}


# ---------------------------------------------------------------- Makefile
def make_vars(target):
    """Expand CC / flags for `target` from the Makefile text (never executed)."""
    txt = open(os.path.join(ROOT, 'Makefile')).read().replace('\\\n', ' ')
    glob_ = {}
    for m in re.finditer(r'^([A-Z0-9_]+)\s*(:=|\+=|=)\s*(.*)$', txt, re.M):
        k, op, v = m.group(1), m.group(2), m.group(3).strip()
        glob_[k] = (glob_.get(k, '') + ' ' + v).strip() if op == '+=' else v
    tv = dict(glob_)
    for m in re.finditer(r'^([^\n#:=]+?):\s*([A-Z0-9_]+)\s*(:=|\+=|=)\s*(.*)$', txt, re.M):
        if target not in m.group(1).split():
            continue
        k, op, v = m.group(2), m.group(3), m.group(4).strip()
        tv[k] = (tv.get(k, '') + ' ' + v).strip() if op == '+=' else v

    def ex(s, d=0):
        if d > 20:
            return s
        return re.sub(r'\$\(([A-Za-z0-9_]+)\)', lambda mm: ex(tv.get(mm.group(1), ''), d + 1), s)
    return {k: ex(tv.get(k, '')) for k in ('CC', 'NCC', 'CFLAGS', 'NCC_CFLAGS', 'OPT_FLAGS',
                                           'CC_ONLY_FLAGS', 'MIPSISET', 'CPPFLAGS', 'AS',
                                           'ASFLAGS', 'ASM_PROC', 'ASM_PRELUDE')}


# ---------------------------------------------------------------- C scanning
def comment_mask(t):
    m = bytearray(len(t))
    i, n = 0, len(t)
    while i < n:
        if t.startswith('//', i):
            j = t.find('\n', i); j = n if j < 0 else j
        elif t.startswith('/*', i):
            j = t.find('*/', i + 2); j = n if j < 0 else j + 2
        elif t[i] in '"\'':
            j = i + 1
            while j < n and t[j] != t[i]:
                j += 2 if t[j] == '\\' else 1
            j += 1
        else:
            i += 1
            continue
        m[i:j] = b'\1' * (min(j, n) - i)
        i = j
    return m


def in_nm_branch(t, pos):
    """Index of the `#ifdef NON_MATCHING`-style directive whose true branch holds pos, else None."""
    stack = []
    for mm in re.finditer(r'^[ \t]*#[ \t]*(if|ifdef|ifndef|elif|else|endif)\b(.*)$', t[:pos], re.M):
        d, rest = mm.group(1), mm.group(2)
        if d in ('if', 'ifdef', 'ifndef'):
            nm = (d == 'ifdef' and re.match(r'\s*NON_MATCHING\b', rest)) or \
                 (d == 'if' and re.match(r'\s*defined\s*\(?\s*NON_MATCHING\b', rest))
            stack.append((mm.start(), mm.end()) if nm else None)
        elif d in ('else', 'elif') and stack:
            stack[-1] = None
        elif d == 'endif' and stack:
            stack.pop()
    live = [s for s in stack if s]
    return live[-1] if live else None


def locate_def(t, func, want_nm):
    """(body_open_brace, body_end, nm_directive_span or None) of func's definition."""
    cm = comment_mask(t)
    found = []
    for mm in re.finditer(r'\b' + re.escape(func) + r'\s*\(', t):
        if cm[mm.start()]:
            continue
        d, k = 0, mm.end() - 1
        while k < len(t):
            if not cm[k]:
                if t[k] == '(':
                    d += 1
                elif t[k] == ')':
                    d -= 1
                    if d == 0:
                        break
            k += 1
        k += 1
        while k < len(t) and (t[k].isspace() or cm[k]):
            k += 1
        if k >= len(t) or t[k] in ';,=)':
            continue
        ob = t.find('{', k)
        if ob < 0 or ';' in t[k:ob] and not re.search(r'\)\s*$', t[mm.start():k]):
            # K&R parameter declarations end in ';' -- allow them
            pass
        if ob < 0:
            continue
        # body extent
        d = 0
        for j in range(ob, len(t)):
            if cm[j]:
                continue
            if t[j] == '{':
                d += 1
            elif t[j] == '}':
                d -= 1
                if d == 0:
                    break
        found.append((ob, j + 1, in_nm_branch(t, mm.start())))
    nm = [f for f in found if f[2]]
    if want_nm and nm:
        return nm[0]
    return (nm or found or [None])[0]


def positions(t, ob, end, cmode, typedefs):
    cm = comment_mask(t)
    pos = []
    par = 0
    for i in range(ob, end - 1):
        if cm[i]:
            continue
        c = t[i]
        if c == '(':
            par += 1
        elif c == ')':
            par -= 1
        elif par == 0 and c in ';{}':
            ls = t.rfind('\n', 0, i) + 1
            if t[ls:i].lstrip().startswith('#'):
                continue
            pos.append(i + 1)
    if cmode:
        keep = []
        for p in pos:
            j = p
            while j < end and (t[j].isspace() or cm[j]):
                j += 1
            m = re.match(r'[A-Za-z_]\w*', t[j:])
            w = m.group(0) if m else ''
            if w in CTYPES or w in typedefs:
                # declaration follows (a cast-free expression never starts with a type word)
                if not re.match(r'[A-Za-z_]\w*\s*(=|\(|\[|->|\.|\+\+|--|;)', t[j:]) or w in CTYPES:
                    continue
            keep.append(p)
        pos = keep
    return sorted(set(pos))


# ---------------------------------------------------------------- compile
class Compiler:
    def __init__(self, a):
        self.a = a
        self.tmp = tempfile.mkdtemp(prefix='blockcut-')
        if a.unit:
            self.src_path = os.path.join(ROOT, a.unit + '.c')
            tgt = ('build/non_matching/' if a.nm_tree else 'build/') + a.unit + '.c.o'
            self.v = make_vars(tgt)
            self.kind = 'c'
            self.expected = os.path.join(ROOT, 'expected', a.unit + '.c.o')
        else:
            self.src_path = os.path.abspath(a.file)
            self.kind = 'cpp' if self.src_path.endswith(('.cpp', '.cc', '.C')) else 'c'
            self.v = make_vars('build/src/x.c.o')
            self.expected = os.path.abspath(a.expected)
        if a.opt:
            self.v['OPT_FLAGS'] = a.opt
        self.incdir = os.path.dirname(self.src_path)
        self.text = open(self.src_path).read()
        self.asmproc = self.kind == 'c' and re.search(r'\b(INCLUDE_ASM|GLOBAL_ASM|INCLUDE_RODATA)\b', self.text)

    def cmd_cc(self, cc_override=None, extra=()):
        v = self.v
        if self.kind == 'cpp':
            cc = cc_override or os.path.join(ROOT, v['NCC'])
            return [cc, '-c'] + shlex.split(v['NCC_CFLAGS']) + list(extra) + shlex.split(v['OPT_FLAGS']) + \
                shlex.split(v['MIPSISET'])
        cc = cc_override or os.path.join(ROOT, v['CC'])
        flags = shlex.split(v['CFLAGS']) + shlex.split(v['OPT_FLAGS']) + shlex.split(v['CC_ONLY_FLAGS']) + \
            shlex.split(v['MIPSISET']) + shlex.split(v['CPPFLAGS']) + ['-I', self.incdir]
        if self.a.unit and self.a.nm_tree:
            flags.append('-DNON_MATCHING')
        return [cc, '-c'] + list(extra) + flags

    def build(self, text, tag, cc_override=None, extra=()):
        d = os.path.join(self.tmp, tag)
        os.makedirs(d, exist_ok=True)
        base = os.path.basename(self.src_path)
        src = os.path.join(d, base)
        obj = os.path.join(d, 'o.o')
        open(src, 'w').write(text)
        try:
            if self.asmproc:
                ap = shlex.split(self.v['ASM_PROC'])
                ap[1] = os.path.join(ROOT, ap[1])
                opt = shlex.split(self.v['OPT_FLAGS'])
                pre = os.path.join(d, 'pre_' + base)
                with open(pre, 'w') as f:
                    r = subprocess.run(ap + opt + [src], cwd=ROOT, stdout=f, stderr=subprocess.DEVNULL)
                if r.returncode:
                    return None
                r = subprocess.run(self.cmd_cc(cc_override, extra) + ['-o', obj, pre], cwd=ROOT,
                                   capture_output=True)
                if r.returncode or not os.path.exists(obj):
                    if self.a.verbose:
                        print(r.stderr.decode(errors='replace')[-3000:])
                    return None
                r = subprocess.run(ap + opt + [src, '--post-process', obj, '--assembler',
                                               self.v['AS'] + ' ' + self.v['ASFLAGS'],
                                               '--asm-prelude', os.path.join(ROOT, self.v['ASM_PRELUDE'])],
                                   cwd=ROOT, capture_output=True)
                if r.returncode:
                    if self.a.verbose:
                        print(r.stderr.decode(errors='replace')[-3000:])
                    return None
            else:
                r = subprocess.run(self.cmd_cc(cc_override, extra) + ['-o', obj, src], cwd=ROOT,
                                   capture_output=True)
                if r.returncode or not os.path.exists(obj):
                    if self.a.verbose:
                        print(r.stderr.decode(errors='replace')[-3000:])
                    return None
            q = subprocess.run([sys.executable, os.path.join(ROOT, 'scripts', 'rdiff.py'), obj,
                                self.expected, self.a.score_func or self.a.func, '--raw', '--pins', '-q'],
                               cwd=ROOT, capture_output=True, text=True).stdout
            m = re.search(r'diffwords (\d+)', q)
            return int(m.group(1)) if m else None
        finally:
            if self.a.keep and tag == 'base' and os.path.exists(obj):
                shutil.copy(obj, self.a.keep)
                if os.path.exists(src):
                    shutil.copy(src, self.a.keep + '.src')
            shutil.rmtree(d, ignore_errors=True)


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    g = ap.add_mutually_exclusive_group(required=True)
    g.add_argument('--unit')
    g.add_argument('--file')
    ap.add_argument('func')
    ap.add_argument('--expected')
    ap.add_argument('--score-func')
    ap.add_argument('--nm-tree', action='store_true')
    ap.add_argument('--opt')
    ap.add_argument('--k', type=int, default=1)
    ap.add_argument('--ins', default='if (1) {}')
    ap.add_argument('--swap', action='append', default=[])
    ap.add_argument('--lines', action='append', default=[])
    ap.add_argument('--also', action='append', default=[])
    ap.add_argument('--greedy', action='store_true')
    ap.add_argument('--max', type=int, default=20000)
    ap.add_argument('--jobs', type=int, default=8)
    ap.add_argument('--top', type=int, default=10)
    ap.add_argument('--write')
    ap.add_argument('--varref', action='store_true')
    ap.add_argument('--keep', help='copy the base object to KEEP (and its source to KEEP.src)')
    ap.add_argument('--verbose', action='store_true', help='print compiler errors of the base build')
    a = ap.parse_args()
    if a.unit:
        a.unit = re.sub(r'\.c$', '', a.unit)
    elif not a.expected:
        ap.error('--file needs --expected')
    C = Compiler(a)
    t = C.text
    loc = locate_def(t, a.func, bool(a.unit))
    if not loc:
        sys.exit(f'{a.func}: no definition in {C.src_path}')
    ob, end, nmspan = loc
    if a.unit and not a.nm_tree and nmspan:
        # promote: this one wrap becomes `#if 1` (same length keeps offsets simple)
        t = t[:nmspan[0]] + '#if 1' + ' ' * (nmspan[1] - nmspan[0] - 5) + t[nmspan[1]:]
    typedefs = set(re.findall(r'typedef[^;{}]*?\b([A-Za-z_]\w*)\s*(?:\[[^\]]*\])?\s*;', t)) | \
        set(re.findall(r'\}\s*([A-Za-z_]\w*)\s*;', t))
    pos = positions(t, ob, end, C.kind == 'c', typedefs)
    for other in a.also:
        lo = locate_def(t, other, False)
        if not lo:
            sys.exit(f'--also {other}: no definition found')
        pos += positions(t, lo[0], lo[1], C.kind == 'c', typedefs)
    if a.lines:
        rng = [tuple(int(x) for x in s.split('-')) for s in a.lines]
        pos = [p for p in pos if any(lo <= t.count('\n', 0, p) + 1 <= hi for lo, hi in rng)]
    edits = [('ins', p, s) for p in pos for s in a.ins.split('|')]
    for sw in a.swap:
        x, y = sw.split('=>', 1) if '=>' in sw else sw.split('=', 1)
        i = t.find(x, t.rfind('\n', 0, t.rfind(a.func, 0, ob)) + 1)  # header included
        while 0 <= i < end:
            edits.append(('swap', i, (x, y)))
            i = t.find(x, i + 1)

    def lineof(p):
        return t.count('\n', 0, p) + 1

    def describe(combo):
        out = []
        for e in combo:
            kind, p, s = edits[e]
            ls = t.rfind('\n', 0, p) + 1
            snip = t[ls:p].strip()[-40:]
            out.append(f'L{lineof(p)}:{"+" if kind == "ins" else "swap"}[{snip}]' +
                       ('' if kind == 'ins' and len(a.ins.split('|')) == 1 else f'<{s}>'))
        return ' '.join(out)

    def variant(combo):
        s = t
        for e in sorted(combo, key=lambda e: -edits[e][1]):
            kind, p, x = edits[e]
            if kind == 'ins':
                s = s[:p] + ' ' + x + s[p:]
            else:
                s = s[:p] + x[1] + s[p + len(x[0]):]
        return s

    def valid(combo):
        ps = [edits[e][1] for e in combo]
        return len(set(ps)) == len(ps)

    sys.stdout.reconfigure(line_buffering=True)
    print(f'{a.func}: {len(pos)} cut positions, {len(edits)} edits, src {os.path.relpath(C.src_path, ROOT)}'
          f'{" (promote)" if a.unit and not a.nm_tree else ""}')
    mk = open(os.path.join(ROOT, 'Makefile')).read()
    dm = re.search(r'REPLACE_FUNC_BODY\s*\+?:?=\s*' + re.escape(a.func) + r'=\$\((\w+)\)', mk)
    if a.unit and dm:
        dv = re.search(r'^' + dm.group(1) + r'\s*:=\s*build/(\S+)\.o', mk, re.M)
        print(f'WARNING: {a.func} is spliced from a donor ({dv.group(1) if dv else dm.group(1)});'
              ' search that file with --file instead.')
    base = C.build(t, 'base')
    if base is None and a.unit and not a.nm_tree:
        # the wrap needs declarations that live in other NON_MATCHING blocks
        print('promote build failed; falling back to --nm-tree')
        a.nm_tree = True
        C.v = make_vars('build/non_matching/' + a.unit + '.c.o')
        t = C.text
        base = C.build(t, 'base')
    print('base', base)
    if a.varref:
        dump_cc = os.path.join(DUMP_DIR, 'NCC' if C.kind == 'cpp' else 'cc')
        if not os.path.exists(dump_cc):
            print('varref: dump toolchain missing at', dump_cc)
        else:
            res = []
            for n in (12, 16, 18, 20, 22, 24, 26, 28, 32, 40, 1000):
                res.append(f'{n}:{C.build(t, f"vr{n}", dump_cc, ["-Wo,-varref,%d" % n])}')
            print('varref (DIAGNOSTIC ONLY)', ' '.join(res))
    if a.k <= 0 or base == 0:
        shutil.rmtree(C.tmp, ignore_errors=True)
        return
    results = {(): base}

    def run(combos):
        combos = [c for c in combos if c not in results and valid(c)]
        with ThreadPoolExecutor(a.jobs) as ex:
            for c, r in zip(combos, ex.map(lambda c: C.build(variant(c), 'v' + '_'.join(map(str, c))), combos)):
                results[c] = r
                if r is not None and (min(v for v in results.values() if v is not None) == r) and r < base:
                    print('  improve', r, describe(c))
        return combos

    if not a.greedy:
        ncomb = sum(math.comb(len(edits), k) for k in range(1, a.k + 1))
        if ncomb > a.max:
            print(f'~{ncomb} variants > --max {a.max}: switching to --greedy')
            a.greedy = True
    if a.greedy:
        run([(e,) for e in range(len(edits))])
        beam = sorted([c for c in results if c and results[c] is not None], key=lambda c: results[c])[:4]
        for _ in range(a.k - 1):
            nxt = set()
            for c in beam:
                for e in range(len(edits)):
                    if e not in c:
                        nxt.add(tuple(sorted(c + (e,))))
            run(sorted(nxt))
            beam = sorted([c for c in results if c and results[c] is not None], key=lambda c: results[c])[:4]
    else:
        combos = [c for k in range(1, a.k + 1) for c in itertools.combinations(range(len(edits)), k)]
        print('variants', len(combos))
        run(combos)
    ranked = sorted([(r, c) for c, r in results.items() if r is not None], key=lambda x: (x[0], len(x[1])))
    nok = sum(1 for r in results.values() if r is not None)
    print(f'best ({nok}/{len(results)} variants compiled):')
    for r, c in ranked[:a.top]:
        print(f'  {r:5d}  {describe(c) or "(no cut)"}')
    if a.write and ranked:
        open(a.write, 'w').write(variant(ranked[0][1]))
        print('wrote', a.write)
    shutil.rmtree(C.tmp, ignore_errors=True)


if __name__ == '__main__':
    main()
