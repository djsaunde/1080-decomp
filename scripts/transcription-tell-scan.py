#!/usr/bin/env python3
"""transcription-tell-scan.py -- rank NM bodies that look HAND-TRANSCRIBED
from IDO output (docs/MATCHING_WORKFLOW.md #kernel-plain-loops-run112).

IDO cannot reproduce its own output from a body that spells out what uopt
did: it unrolls a hand unroll again, colours an explicit pointer walk
differently, and a call through a fn-pointer cast emits lui/addiu/jalr where
the target has a plain jal. Writing the loop IDO started from usually
matches. This script greps the C of every NM function (report.json fuzzy
< 100) for those tells and ranks by unmatched bytes = size * (100 - pct).

Tells (columns):
  reg    number of `register` locals (pointer walks with invariants)
  cast   calls through a fn-pointer cast  ((T)f)(..) / ((T (*)())f)(..)
  jalr   jalr count in the TARGET function (expected/). cast > jalr means
         at least one cast is wrong (the target calls direct); in USO units a
         cast may be legitimate (relocation-driven jalr) -- check the jal.
  unrl   groups of >= 4 statements that differ only in small constants /
         index offsets (hand 4x unroll, peeled iterations written out)
  step   loop steps of 4 elements (`i += 4`, `p += 4`, `+= 0x10`)
  goto   gotos (m2c control-flow transcription)
  dup    runs of a repeated statement block (2-line window repeated verbatim, peel tell)
score = 2*reg + 3*max(cast-jalr,0) + 3*unrl + 3*step + goto + dup

Usage:
  scripts/transcription-tell-scan.py [--owner g|c|all] [--min-pct 0]
       [--min-score 3] [--unit SUBSTR] [--func NAME] [--top N] [-v]
Run `make non_matching_objects` + scripts/refresh-report.sh first.
"""
import argparse, json, os, re, subprocess, sys
from collections import Counter

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
OBJDUMP = 'mips-linux-gnu-objdump'


def owner_of(unit, fname):
    u = unit.split('/')[-1]
    if u.startswith('bootup_uso') or u.startswith('h2hproc_uso') or \
            u.startswith('timproc_uso_b1') or u == 'game_libs_post':
        return 'c'
    if u == 'game_libs_post0b':
        m = re.search(r'([0-9A-Fa-f]{8})$', fname)
        if m and int(m.group(1), 16) < 0x50000:
            return 'c'
    return 'g'


def strip_comments(s):
    s = re.sub(r'/\*.*?\*/', ' ', s, flags=re.S)
    return re.sub(r'//[^\n]*', '', s)


def find_body(src, fn):
    """C text of fn's definition (prefer one inside #ifdef NON_MATCHING)."""
    best = None
    for m in re.finditer(r'\b%s\s*\(' % re.escape(fn), src):
        # definition: the matching ')' is followed by '{' or K&R decls then '{'
        i = m.end()
        depth = 1
        while i < len(src) and depth:
            if src[i] == '(':
                depth += 1
            elif src[i] == ')':
                depth -= 1
            i += 1
        j = i
        while j < len(src) and src[j] in ' \t\r\n':
            j += 1
        if j >= len(src):
            continue
        if src[j] != '{':
            # K&R parameter declarations
            k = src.find('{', j)
            semi = src.find(';', j)
            if k < 0 or src[j:k].count('(') or not re.match(r'[A-Za-z_]', src[j]) \
                    or semi < 0 or semi > k:
                continue
            j = k
        # line start must not be inside an expression: preceding text on
        # the line should look like a return type
        ls = src.rfind('\n', 0, m.start()) + 1
        pre = src[ls:m.start()]
        if ('=' in pre) or (';' in pre) or pre.strip().startswith(('return', 'if', '/*', '*')):
            continue
        depth, k = 0, j
        while k < len(src):
            if src[k] == '{':
                depth += 1
            elif src[k] == '}':
                depth -= 1
                if depth == 0:
                    break
            k += 1
        body = src[j:k + 1]
        nm = src.rfind('#ifdef NON_MATCHING', 0, ls) > src.rfind('#endif', 0, ls)
        if best is None or nm:
            best = body
            if nm:
                break
    return best


NUM = re.compile(r'\b(0x[0-9a-fA-F]+|\d+)\b')


def tells(body):
    b = strip_comments(body)
    t = {}
    t['reg'] = len(re.findall(r'\bregister\b', b))
    casts = re.findall(r'\(\s*\(\s*[A-Za-z_][\w\s]*\**\s*(?:\(\s*\*\s*\)\s*\([^()]*\))?\s*\)\s*'
                       r'&?\s*[A-Za-z_]\w*\s*\)\s*\(', b)
    t['cast'] = len(casts)
    lines = [l.strip() for l in b.split('\n') if l.strip() and l.strip() not in ('{', '}')]
    norm = [NUM.sub('N', l) for l in lines]
    # unroll groups: >= 4 lines within (3 would flag every x/y/z vector op) a 12-line window that differ in exactly
    # ONE numeric token, whose values form an arithmetic progression (index
    # offsets 0/1/2/3, byte offsets 0/4/8/C ...). Widget-constructor runs that
    # differ in several constants are not counted.
    unrl = 0
    used = set()
    toks = [NUM.findall(l) for l in lines]
    for i in range(len(lines)):
        if i in used or len(norm[i]) < 8 or not toks[i]:
            continue
        grp = [i]
        for j in range(i + 1, min(len(lines), i + 12)):
            if j in used or norm[j] != norm[i]:
                continue
            diff = [k for k, (x, y) in enumerate(zip(toks[i], toks[j])) if x != y]
            if len(diff) == 1 and (len(grp) == 1 or grp_pos == diff[0]):
                grp_pos = diff[0]
                grp.append(j)
        if len(grp) >= 4:
            vals = sorted(int(toks[g][grp_pos], 0) for g in grp)
            d = [b - a for a, b in zip(vals, vals[1:])]
            if len(set(d)) == 1 and d[0] > 0:
                unrl += 1
                used.update(grp)
    t['unrl'] = unrl
    t['step'] = len(re.findall(r'(?:\+=\s*(?:4|0x10|16)\b)|(?:\b\w+\s*=\s*\w+\s*\+\s*(?:4|0x10)\s*\))', b))
    t['goto'] = len(re.findall(r'\bgoto\b', b))
    # duplicated 2-line windows (verbatim) -- peeled iteration
    pairs = Counter(lines[k] + '\n' + lines[k + 1] for k in range(len(lines) - 1)
                    if len(lines[k]) > 10 and len(lines[k + 1]) > 10)
    t['dup'] = sum(1 for v in pairs.values() if v >= 2)
    return t


def target_jalr(obj):
    try:
        out = subprocess.check_output([OBJDUMP, '-d', '-M', 'no-aliases', '--no-show-raw-insn',
                                       '-j', '.text', obj], text=True, stderr=subprocess.DEVNULL)
    except Exception:
        return {}
    res, cur = {}, None
    for ln in out.splitlines():
        m = re.match(r'^[0-9a-f]+ <([^>]+)>:', ln)
        if m:
            cur = m.group(1)
            res.setdefault(cur, 0)
            continue
        if cur and re.match(r'^\s+[0-9a-f]+:\tjalr\b', ln):
            res[cur] += 1
    return res


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--report', default=os.path.join(ROOT, 'report.json'))
    ap.add_argument('--owner', default='all', choices=['g', 'c', 'all'])
    ap.add_argument('--min-pct', type=float, default=0.0)
    ap.add_argument('--min-score', type=float, default=3)
    ap.add_argument('--unit')
    ap.add_argument('--func')
    ap.add_argument('--top', type=int, default=60)
    ap.add_argument('-v', action='store_true')
    a = ap.parse_args()

    rep = json.load(open(a.report))
    cfg = json.load(open(os.path.join(ROOT, 'objdiff.json')))
    units = {u['name']: u for u in cfg['units']}
    rows = []
    for u in rep['units']:
        if a.unit and a.unit not in u['name']:
            continue
        nm = [f for f in u.get('functions', [])
              if a.min_pct <= f.get('fuzzy_match_percent', 0) < 100]
        if a.func:
            nm = [f for f in nm if f['name'] == a.func]
        if not nm or u['name'] not in units:
            continue
        uc = units[u['name']]
        srcp = os.path.join(ROOT, uc['metadata']['source_path'])
        try:
            src = open(srcp, encoding='latin-1').read()
        except OSError:
            continue
        jalr = None
        for f in nm:
            fn = f['name']
            own = owner_of(u['name'], fn)
            if a.owner != 'all' and own != a.owner:
                continue
            body = find_body(src, fn)
            if not body:
                continue
            t = tells(body)
            if t['cast']:
                if jalr is None:
                    jalr = target_jalr(os.path.join(ROOT, uc['target_path']))
                t['jalr'] = jalr.get(fn, 0)
            else:
                t['jalr'] = 0
            badcast = max(t['cast'] - t['jalr'], 0)
            score = 2 * t['reg'] + 3 * badcast + 3 * t['unrl'] + 3 * t['step'] + \
                t['goto'] + t['dup']
            if score < a.min_score:
                continue
            size = int(f['size'])
            pct = f.get('fuzzy_match_percent', 0.0)
            rows.append(dict(func=fn, unit=u['name'].split('/')[-1], size=size, pct=pct,
                             unm=size * (100 - pct) / 100, score=score, **t))
    rows.sort(key=lambda r: -r['unm'])
    print('%-30s %-26s %6s %6s %6s %5s | %3s %4s %4s %4s %4s %4s %3s' % (
        'func', 'unit', 'size', 'pct', 'unmB', 'score', 'reg', 'cast', 'jalr', 'unrl',
        'step', 'goto', 'dup'))
    for r in rows[:a.top]:
        print('%-30s %-26s %6d %6.1f %6d %5d | %3d %4d %4d %4d %4d %4d %3d' % (
            r['func'], r['unit'][:26], r['size'], r['pct'], r['unm'], r['score'], r['reg'],
            r['cast'], r['jalr'], r['unrl'], r['step'], r['goto'], r['dup']))
    print('%d flagged, %d unmatched bytes total' % (len(rows), sum(r['unm'] for r in rows)))


if __name__ == '__main__':
    main()
