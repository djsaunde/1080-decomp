#!/usr/bin/env python3
"""twin-finder.py -- fuzzy cross-module twin search: non-exact fn -> best EXACT donor.

"Port an exact twin's C before probing" keeps producing exacts (b3 E60 <- b1 0EE8,
arcproc 1D18 <- b3 19CC, the cross-module ctor family).  find-masked-twins.py only
finds IDENTICAL masked signatures; this finds NEAR twins (offsets/one extra field/
different callee count) ranked by size x similarity.

Sources
  * target words : expected/src/<unit>.c.o .text (assembled from the INCLUDE_ASM
                   .s, so USO `.word` lines and symbolic `jal sym` lines both come
                   out as real bytes; the symbolic ones carry an R_MIPS_26 reloc).
  * exactness    : report.json fuzzy_match_percent == 100 AND the name is not the
                   argument of any INCLUDE_ASM(...) in src/ (an NM-wrapped fn can
                   read 100 in objdiff on baked/placeholder jal differences --
                   those are listed as non-exact with tag nm100).
  * donor source : the src/**/*.c file that defines the donor (`name(` at the
                   start of a definition line).

Normalization (per word)
  * opcode + registers always kept.
  * j/jal target, branch offset, lui imm: masked.
  * lo16 of addiu/ori/loads/stores masked when (a) the .o has a reloc there, or
    (b) the base/src register was last written by a lui in this function
    (USO .word code has no relocs -- its symbol refs are lui/addiu pairs on 0).
  * structural score  = difflib ratio over (opcode, regs) with ALL immediates
    masked (offset-adjust-tolerant);  imm score = ratio with only reloc-ish
    immediates masked (how much of the twin is literally identical).

Boundary tell: a near-1.0 twin whose length differs by exactly 1-3 words is
usually a splat boundary error (hoisted-const head words left in the
predecessor's tail, a shared-exit tail stuck on the successor's head) -- fix the
.s boundary first, then the twin is often byte/masked-identical
(arcproc 1BBC/1C74 <- b1 1908/19C0, kernel 3C24 -> 3C30 osDestroyThread).

Usage
  python3 scripts/twin-finder.py [--min-insns 8] [--min-score 0.80] [--top 60]
         [--owner agent-g|agent-c|all] [--clusters-min 0.90] [--tsv out.tsv]
"""
import os, re, sys, json, glob, struct, argparse, difflib
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from twinlib import ROOT, text_funcs, text_bytes, reloc_offsets

LOADSTORE = set(range(0x20, 0x40))          # lb..sdc1 family
IMM_ALU = {0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E}  # addi addiu slti sltiu andi ori xori
BRANCH = {0x01, 0x04, 0x05, 0x06, 0x07, 0x14, 0x15, 0x16, 0x17}


def owner_of(unit, name):
    u = unit.replace('src/', '')
    m = re.search(r'([0-9A-Fa-f]{8})$', name)
    addr = int(m.group(1), 16) if m else None
    if u.startswith(('bootup_uso/', 'h2hproc_uso/', 'timproc_uso_b1/')):
        return 'agent-c'
    if u == 'game_libs/game_libs_post':
        return 'agent-c'
    if u == 'game_libs/game_libs_post0b':
        return 'agent-c' if (addr is not None and addr < 0x50000) else 'agent-g'
    if u.startswith(('boarder', 'map4')):
        return '-'
    return 'agent-g'


def tokens(words, relidx):
    """returns (struct_tokens, imm_tokens)"""
    st, it = [], []
    luireg = set()
    for i, w in enumerate(words):
        op = (w >> 26) & 0x3f
        rs, rt, rd = (w >> 21) & 31, (w >> 16) & 31, (w >> 11) & 31
        if op == 0:
            t = ('R', w & 0x3f, rs, rt, rd, (w >> 6) & 31)
            st.append(t); it.append(t)
            if rd in luireg: luireg.discard(rd)
            continue
        if op in (2, 3):
            t = ('J', op); st.append(t); it.append(t); continue
        if op in (0x10, 0x11, 0x12):
            t = ('C', op, rs, rt, rd, w & 0x7ff); st.append(t); it.append(t); continue
        if op in BRANCH:
            t = ('B', op, rs, rt); st.append(t); it.append(t); continue
        base = ('I', op, rs, rt)
        st.append(base)
        if op == 0x0F:                       # lui
            it.append(base); luireg.add(rt); continue
        symish = i in relidx or rs in luireg
        it.append(base if symish else base + (w & 0xffff,))
        writes_rt = op in IMM_ALU or (0x20 <= op < 0x28) or op == 0x37
        if writes_rt:
            if op == 0x09 and rs in luireg:  # addiu rt, lui-reg, %lo -> address reg
                luireg.add(rt)
            else:
                luireg.discard(rt)
    return st, it


def include_asm_names():
    names = set()
    for p in glob.glob(os.path.join(ROOT, 'src/**/*.c'), recursive=True):
        for m in re.finditer(r'INCLUDE_ASM\([^,]+,\s*(\w+)\s*\)', open(p, errors='replace').read()):
            names.add(m.group(1))
    return names


def def_index():
    idx = {}
    rx = re.compile(r'^[A-Za-z_][\w \t\*]*?\b(\w+)\s*\([^;]*$', re.M)
    for p in glob.glob(os.path.join(ROOT, 'src/**/*.c'), recursive=True):
        txt = open(p, errors='replace').read()
        for m in rx.finditer(txt):
            nm = m.group(1)
            if nm in ('if', 'while', 'for', 'switch', 'return', 'sizeof'): continue
            idx.setdefault(nm, os.path.relpath(p, ROOT))
    return idx


def handwritten_names():
    """fns whose .s says '/* Handwritten function */' or uses cache/COP0 ops"""
    hw = set()
    for p in glob.glob(os.path.join(ROOT, 'asm/**/*.s'), recursive=True):
        try:
            txt = open(p, errors='replace').read(4096 * 4)
        except OSError:
            continue
        if 'Handwritten function' in txt[:200] or re.search(r'\b(cache|mtc0|mfc0|eret|tlbwi|tlbp)\b', txt):
            hw.add(os.path.basename(p)[:-2])
    return hw


def load():
    rep = json.load(open(os.path.join(ROOT, 'report.json')))
    status = {}
    for u in rep['units']:
        for f in u.get('functions', []):
            status[f['name']] = (u['name'], f.get('fuzzy_match_percent', 0.0))
    inc = include_asm_names()
    defs = def_index()
    hw = handwritten_names()
    fns = {}
    for opath in glob.glob(os.path.join(ROOT, 'expected/src/**/*.c.o'), recursive=True):
        unit = 'src/' + os.path.relpath(opath, os.path.join(ROOT, 'expected/src'))[:-4]
        fs = text_funcs(opath)
        if not fs: continue
        data = text_bytes(opath)
        rel = reloc_offsets(opath)
        for nm, (a, sz) in fs.items():
            chunk = data[a:a + sz]
            if len(chunk) < sz or sz < 4: continue
            w = list(struct.unpack('>%dI' % (sz // 4), chunk[:sz // 4 * 4]))
            while len(w) > 1 and w[-1] == 0 and w[-2] != 0x03E00008: w.pop()  # trailing pad
            ri = {(o - a) // 4 for o in rel if a <= o < a + sz}
            st, it = tokens(w, ri)
            u, pct = status.get(nm, (unit, None))
            if pct is None: continue
            exact = pct == 100.0 and nm not in inc
            tag = 'exact' if exact else ('nm100' if pct == 100.0 else
                  ('hand' if nm in hw else
                   ('nm' if nm in defs and nm in inc else 'bare')))
            fns[nm] = dict(unit=u, size=len(w) * 4, pct=pct, exact=exact, tag=tag,
                           st=st, it=it, src=defs.get(nm, '-'), owner=owner_of(u, nm))
    return fns


def intern(seqs):
    tab = {}
    return [[tab.setdefault(t, len(tab)) for t in s] for s in seqs]


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--min-insns', type=int, default=8)
    ap.add_argument('--min-score', type=float, default=0.80)
    ap.add_argument('--len-tol', type=float, default=0.10)
    ap.add_argument('--top', type=int, default=60)
    ap.add_argument('--owner', default='all')
    ap.add_argument('--clusters-min', type=float, default=0.90)
    ap.add_argument('--tsv')
    ap.add_argument('--include-hand', action='store_true',
                    help='keep handwritten (cache/COP0 / "Handwritten function") targets')
    a = ap.parse_args()

    fns = load()
    names = [n for n, f in fns.items() if len(f['st']) >= a.min_insns]
    allst = intern([fns[n]['st'] for n in names] + [fns[n]['it'] for n in names])
    for k, n in enumerate(names):
        fns[n]['S'] = allst[k]; fns[n]['I'] = allst[len(names) + k]
    donors = [n for n in names if fns[n]['exact']]
    targets = [n for n in names if not fns[n]['exact']
               and (a.include_hand or fns[n]['tag'] != 'hand')]
    print(f'# donors(exact)={len(donors)} targets(non-exact)={len(targets)}', file=sys.stderr)

    def score(x, y, minr):
        A, B = fns[x]['S'], fns[y]['S']
        if abs(len(A) - len(B)) > a.len_tol * max(len(A), len(B)): return None
        sm = difflib.SequenceMatcher(None, A, B, autojunk=False)
        if sm.real_quick_ratio() < minr or sm.quick_ratio() < minr: return None
        r = sm.ratio()
        if r < minr: return None
        ri = difflib.SequenceMatcher(None, fns[x]['I'], fns[y]['I'], autojunk=False).ratio()
        return r, ri

    rows = []
    for t in targets:
        best = None
        for d in donors:
            s = score(t, d, max(a.min_score, best[0][0] if best else 0))
            if s and (best is None or s > best[0]): best = (s, d)
        if best:
            (r, ri), d = best
            ft, fd = fns[t], fns[d]
            rows.append(dict(fn=t, unit=ft['unit'], owner=ft['owner'], tag=ft['tag'],
                             pct=ft['pct'], size=ft['size'], score=r, imm=ri, twin=d,
                             tunit=fd['unit'], tsrc=fd['src'], rank=ft['size'] * r))
    rows.sort(key=lambda x: -x['rank'])
    sel = [x for x in rows if a.owner == 'all' or x['owner'] == a.owner]
    print('%-34s %-34s %-7s %-5s %6s %5s %5s %6s  %-34s %s' % (
        'non-exact fn', 'unit', 'owner', 'tag', 'pct', 'size', 'score', 'imm', 'exact twin', 'twin unit / source'))
    for x in sel[:a.top]:
        print('%-34s %-34s %-7s %-5s %6.2f %5d %5.3f %6.3f  %-34s %s  %s' % (
            x['fn'], x['unit'].replace('src/', ''), x['owner'], x['tag'], x['pct'], x['size'],
            x['score'], x['imm'], x['twin'], x['tunit'].replace('src/', ''), x['tsrc']))
    if a.tsv:
        with open(a.tsv, 'w') as fh:
            fh.write('fn\tunit\towner\ttag\tpct\tsize\tscore\timm\ttwin\ttwin_unit\ttwin_src\n')
            for x in rows:
                fh.write('\t'.join(str(x[k]) for k in ('fn', 'unit', 'owner', 'tag', 'pct', 'size',
                         'score', 'imm', 'twin', 'tunit', 'tsrc')) + '\n')

    # --- clusters of NON-exact functions twinned with each other -----------
    parent = {t: t for t in targets}
    def find(x):
        while parent[x] != x:
            parent[x] = parent[parent[x]]; x = parent[x]
        return x
    edges = {}
    for i, x in enumerate(targets):
        for y in targets[i + 1:]:
            s = score(x, y, a.clusters_min)
            if s:
                parent[find(x)] = find(y); edges[(x, y)] = s[0]
    groups = {}
    for t in targets: groups.setdefault(find(t), []).append(t)
    cl = [g for g in groups.values() if len(g) > 1]
    cl.sort(key=lambda g: -sum(fns[n]['size'] for n in g))
    print(f'\n=== {len(cl)} non-exact twin clusters (structural >= {a.clusters_min}) ===')
    for g in cl:
        tot = sum(fns[n]['size'] for n in g)
        print(f'[{len(g)} fns, {tot} bytes]')
        for n in sorted(g, key=lambda n: -fns[n]['pct']):
            f = fns[n]
            print(f"   {n:34s} {f['unit'].replace('src/', ''):34s} {f['owner']:7s} {f['tag']:5s} {f['pct']:6.2f} {f['size']:5d}")


if __name__ == '__main__':
    main()
