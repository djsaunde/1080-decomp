#!/usr/bin/env python3
"""void-callee-scan.py -- find NM functions where an UNUSED int call return is
poisoning $v0 (the "void callee" lever, docs/IDO_CODEGEN.md
#feedback-ido-void-callee-kills-dead-v0-def, kit IX 6C00).

Mechanism: a call whose int return is discarded still defines $v0. uopt treats
that dead def as interfering with candidates born after the call in the same
basic block, so a value the target colours $v0 lands one slot up the
caller-saved ring in our build (v0->v1, v1->a0, ... or tN->tN+1). Declaring the
callee (or a distinct alias of the blank callee) `void` removes the def.

For every NM function (report.json fuzzy < 100) this script:
  1. disassembles the target (expected/) and our NM build (build/non_matching/)
     and aligns them (difflib on mnemonics);
  2. for each jal in the BUILD, takes the window up to the next jal / branch
     target region (max --window insns) and collects register operand pairs
     (target_reg, build_reg) on aligned same-mnemonic instructions;
  3. flags the call when
       - the target does NOT read v0 after the jal before redefining it
         (the return really is unused),
       - the build does not read v0 either (our C discards it too),
       - the target writes v0 inside the window (a post-call value got v0),
       - most register diffs in the window are +1 ring shifts
         (build reg == next(target reg));
  4. looks up the callee symbol (build reloc) in the unit's C source and
     reports whether it is declared void (skip) or int/K&R (candidate).

When the hit's call is preceded by OTHER discarded-return calls in the same
straight-line run, try voiding them together: mgrproc_uso_func_00001614 only
went exact with BOTH import_007FE4E8 and 07ACE0 void (either alone: neutral).
The `decl` column greps the unit's C source; `undeclared(K&R int)` means an
implicit declaration (add `extern void NAME();`). For the shared blank callee
gl_func_00000000 / func_00000000 use a distinct alias
(`extern void gl_func_00000000_XXXXv();` + `= 0x00000000;` in
undefined_syms_auto.txt, then rm build/tenshoe.elf).

Ranking: estimated byte gain = 4 * number of instructions in the window whose
only difference is a +1 shift.

Usage:
  scripts/void-callee-scan.py [--report report.json] [--min 50] [--window 24]
                              [--owner g|c|all] [--func NAME] [-v]
Run `make non_matching_objects` + refresh-report first.
"""
import argparse, difflib, json, os, re, subprocess, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
OBJDUMP = 'mips-linux-gnu-objdump'

RING = ['v0', 'v1', 'a0', 'a1', 'a2', 'a3', 't0', 't1', 't2', 't3', 't4',
        't5', 't6', 't7', 't8', 't9']
NEXT = {RING[i]: RING[i + 1] for i in range(len(RING) - 1)}
GPR = set(RING) | {'zero', 'at', 's0', 's1', 's2', 's3', 's4', 's5', 's6', 's7',
                   'k0', 'k1', 'gp', 'sp', 's8', 'fp', 'ra'}

# mnemonics whose operands are ALL sources (no GPR destination)
ALL_SRC = re.compile(r'^(s[bhwd]|swc1|sdc1|swl|swr|sc|b\w*|j|jr|jalr?|mtc1|ctc1|'
                     r'dmtc1|mult|multu|div|divu|dmult|dmultu|ddiv|ddivu|mthi|mtlo|'
                     r'teq|tne|tge|tlt|cache|lwc1|ldc1)$')
# note: lwc1/ldc1 have FP dest, GPR base is a source


def owner_of(unit, fname):
    """'c' for agent-c's units, 'g' otherwise."""
    u = unit.split('/')[-1]
    if u.startswith('bootup_uso') or u.startswith('h2hproc_uso') or \
            u.startswith('timproc_uso_b1') or u == 'game_libs_post':
        return 'c'
    if u == 'game_libs_post0b':
        m = re.search(r'([0-9A-Fa-f]{8})$', fname)
        if m and int(m.group(1), 16) < 0x50000:
            return 'c'
    return 'g'


def disasm(obj):
    """{func: [(mnemonic, [operands], reloc_sym_or_None)]}"""
    try:
        out = subprocess.check_output([OBJDUMP, '-dr', '-M', 'no-aliases',
                                       '--no-show-raw-insn', '-j', '.text', obj],
                                      text=True, stderr=subprocess.DEVNULL)
    except Exception:
        return {}
    funcs, cur = {}, None
    for ln in out.splitlines():
        m = re.match(r'^[0-9a-f]+ <([^>]+)>:', ln)
        if m:
            cur = m.group(1)
            if cur.endswith('.NON_MATCHING'):
                cur = None
            else:
                funcs[cur] = []
            continue
        if cur is None:
            continue
        m = re.match(r'^\s+[0-9a-f]+:\s+R_MIPS_(\w+)\s+(\S+)', ln)
        if m:
            if funcs[cur]:
                mn, ops, _ = funcs[cur][-1]
                if m.group(1) == '26':
                    funcs[cur][-1] = (mn, ops, m.group(2))
            continue
        m = re.match(r'^\s+[0-9a-f]+:\t(\S+)\s*(.*)$', ln)
        if m:
            ops = [o.strip() for o in re.split(r',', m.group(2).split('<')[0]) if o.strip()]
            funcs[cur].append((m.group(1), ops, None))
    return funcs


def regs_in(op):
    return [r for r in re.findall(r'[a-z][a-z0-9]+', op) if r in GPR]


def defs_uses(mn, ops):
    if not ops:
        return set(), set()
    if ALL_SRC.match(mn):
        u = set()
        for o in ops:
            u |= set(regs_in(o))
        if mn in ('jal', 'jalr'):
            return {'ra'}, u
        return set(), u
    if mn in ('mfc1', 'cfc1', 'dmfc1', 'mfhi', 'mflo', 'lui') or mn.startswith('l'):
        d = set(regs_in(ops[0]))
        u = set()
        for o in ops[1:]:
            u |= set(regs_in(o))
        return d, u
    d = set(regs_in(ops[0]))
    u = set()
    for o in ops[1:]:
        u |= set(regs_in(o))
    return d, u


def reads_v0_before_def(seq, start, limit):
    """After index start (the jal; its delay slot start+1 executes BEFORE the call
    returns), does the code read v0 before redefining it? Linear scan, stops at
    control flow (conservative: a branch ends the scan)."""
    i = start + 2
    end = min(len(seq), start + 2 + limit)
    while i < end:
        mn, ops, _ = seq[i]
        d, u = defs_uses(mn, ops)
        if 'v0' in u:
            return True
        if 'v0' in d:
            return False
        if mn.startswith('j') or (mn.startswith('b') and mn not in ('break',)):
            # look at the delay slot too
            if i + 1 < len(seq):
                d2, u2 = defs_uses(seq[i + 1][0], seq[i + 1][1])
                if 'v0' in u2:
                    return True
            return False
        i += 1
    return False


def find_decl(src_text, sym):
    """Return 'void', 'nonvoid', or 'undeclared' for a callee name in a TU."""
    if not sym:
        return 'unknown'
    pat = re.compile(r'^[^\n;{}()]*?\b(\w[\w\s\*]*?)\s*\**\s*\b' + re.escape(sym) +
                     r'\s*\(', re.M)
    kinds = set()
    for m in pat.finditer(src_text):
        line = src_text[src_text.rfind('\n', 0, m.start()) + 1: m.end()]
        if line.lstrip().startswith(('//', '*', '/*', '#')):
            continue
        if re.search(r'\bvoid\s*\**\s*' + re.escape(sym) + r'\s*\(', line) and \
                not re.search(r'\bvoid\s*\*', line.split(sym)[0]):
            kinds.add('void')
        elif re.search(r'\b(extern|int|s32|u32|s16|u16|char|short|f32|float|unsigned|signed|'
                       r'u8|s8|long|struct|\w+_t)\b', line.split(sym)[0]):
            kinds.add('nonvoid')
    if not kinds:
        return 'undeclared(K&R int)'
    if kinds == {'void'}:
        return 'void'
    if 'void' in kinds:
        return 'mixed'
    return 'nonvoid'


def analyse(tseq, bseq, window):
    tm = [x[0] for x in tseq]
    bm = [x[0] for x in bseq]
    sm = difflib.SequenceMatcher(None, tm, bm, autojunk=False)
    b2t = {}
    for a, b, n in sm.get_matching_blocks():
        for k in range(n):
            b2t[b + k] = a + k
    hits = []
    for bi, (mn, ops, rsym) in enumerate(bseq):
        if mn != 'jal':
            continue
        ti = b2t.get(bi)
        if ti is None or tm[ti] != 'jal':
            continue
        # window: until next jal in build (exclusive) or window size
        end = bi + 2
        while end < len(bseq) and end < bi + 2 + window and bseq[end][0] not in ('jal', 'jalr'):
            end += 1
        shifted, other, tv0 = 0, 0, False
        shift_pairs = {}
        for j in range(bi + 2, end):
            tj = b2t.get(j)
            if tj is None:
                continue
            tmn, tops, _ = tseq[tj]
            _, bops, _ = bseq[j]
            td, _tu = defs_uses(tmn, tops)
            if 'v0' in td:
                tv0 = True
            if len(tops) != len(bops):
                other += 1
                continue
            insn_shift, insn_other = False, False
            for to, bo in zip(tops, bops):
                if to == bo:
                    continue
                tr, br = regs_in(to), regs_in(bo)
                if len(tr) == len(br) and tr and re.sub(r'[a-z][a-z0-9]+', 'R', to) == \
                        re.sub(r'[a-z][a-z0-9]+', 'R', bo):
                    for x, y in zip(tr, br):
                        if x == y:
                            continue
                        if NEXT.get(x) == y:
                            insn_shift = True
                            shift_pairs[(x, y)] = shift_pairs.get((x, y), 0) + 1
                        else:
                            insn_other = True
                else:
                    insn_other = True
            if insn_shift and not insn_other:
                shifted += 1
            elif insn_other:
                other += 1
        if shifted == 0:
            continue
        t_reads = reads_v0_before_def(tseq, ti, window)
        b_reads = reads_v0_before_def(bseq, bi, window)
        hits.append(dict(bidx=bi, callee=rsym, shifted=shifted, other=other,
                         tv0=tv0, t_reads_v0=t_reads, b_reads_v0=b_reads,
                         pairs=shift_pairs))
    return hits


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--report', default=os.path.join(ROOT, 'report.json'))
    ap.add_argument('--min', type=float, default=50.0)
    ap.add_argument('--window', type=int, default=24)
    ap.add_argument('--owner', default='all', choices=['g', 'c', 'all'])
    ap.add_argument('--func')
    ap.add_argument('--all-calls', action='store_true',
                    help='also list calls that fail the strict filter')
    ap.add_argument('-v', action='store_true')
    a = ap.parse_args()

    rep = json.load(open(a.report))
    cfg = json.load(open(os.path.join(ROOT, 'objdiff.json')))
    units = {u['name']: u for u in cfg['units']}
    rows = []
    for u in rep['units']:
        nm = [f for f in u.get('functions', [])
              if a.min <= f.get('fuzzy_match_percent', 0) < 100]
        if a.func:
            nm = [f for f in nm if f['name'] == a.func]
        if not nm or u['name'] not in units:
            continue
        uc = units[u['name']]
        tobj = os.path.join(ROOT, uc['target_path'])
        bobj = os.path.join(ROOT, uc['base_path'])
        srcp = os.path.join(ROOT, uc['metadata']['source_path'])
        if not (os.path.exists(tobj) and os.path.exists(bobj)):
            continue
        td, bd = disasm(tobj), disasm(bobj)
        try:
            src = open(srcp, encoding='latin-1').read()
        except OSError:
            src = ''
        for f in nm:
            fn = f['name']
            own = owner_of(u['name'], fn)
            if a.owner != 'all' and own != a.owner:
                continue
            if fn not in td or fn not in bd:
                continue
            for h in analyse(td[fn], bd[fn], a.window):
                strict = (not h['t_reads_v0'] and not h['b_reads_v0'] and h['tv0']
                          and h['shifted'] >= h['other'])
                if not strict and not a.all_calls:
                    continue
                decl = find_decl(src, h['callee'])
                if decl == 'void' and not a.all_calls:
                    continue
                rows.append(dict(func=fn, unit=u['name'], owner=own,
                                 fuzzy=f.get('fuzzy_match_percent', 0),
                                 size=int(f.get('size', 0)), decl=decl,
                                 gain=4 * h['shifted'], strict=strict, **h))
    rows.sort(key=lambda r: (-r['gain'], r['func']))
    print('%-38s %-5s %6s %6s %5s %4s %4s %-26s %-18s %s' % (
        'function', 'own', 'fuzzy', 'size', 'gain', 'shft', 'oth', 'callee', 'decl', 'pairs'))
    for r in rows:
        pairs = ' '.join('%s>%s:%d' % (x, y, n) for (x, y), n in sorted(r['pairs'].items()))
        flag = '' if r['strict'] else ' (loose: t_rd=%d b_rd=%d tv0=%d)' % (
            r['t_reads_v0'], r['b_reads_v0'], r['tv0'])
        print('%-38s %-5s %6.2f %6d %5d %4d %4d %-26s %-18s %s%s' % (
            r['func'], r['owner'], r['fuzzy'], r['size'], r['gain'], r['shifted'],
            r['other'], (r['callee'] or '?')[:26], r['decl'], pairs, flag))
        if a.v:
            print('    unit=%s jal@insn %d' % (r['unit'], r['bidx']))


if __name__ == '__main__':
    main()
