#!/usr/bin/env python3
"""rdiff.py -- instruction diff of ONE function between two objects, with
optional raw branch offsets, raw jal targets and undefined_syms pins.

Usage:
  scripts/rdiff.py OURS.o TARGET.o FUNC [flags]
  scripts/rdiff.py --unit src/<seg>/<file> FUNC [flags]
      (= build/non_matching/src/<seg>/<file>.c.o vs expected/src/<seg>/<file>.c.o)

Flags:
  --raw     keep branch targets (as offsets from the function start, `L<hex>`)
            and jal targets; do NOT mask relocated immediates.  This is the
            mode that exposes DECODE errors: a branch that skips one statement
            too few/many, a missing return path, nested-vs-sequential control
            flow, a baked USO jal target the NM calls through a symbol.
  --pins    with --raw: apply undefined_syms_auto.txt pins to relocated
            HI16/LO16/26 fields (both objects), so `gl_ref_X = 0x...` aliases
            compare as the bytes the ROM will hold.  Without --pins a pinned
            symbol shows its in-object addend (usually 0) and looks different.
  --maskbr  with --raw: still mask branch targets (only jal/immediates raw).
  --nosp    normalise sp offsets and the frame size (colour/schedule-only view).
  --noreg   normalise register names (pure schedule/shape view).
  --sym     (masked mode) append the reloc symbol to each relocated insn.
  -q        print only the summary line.

Without --raw the diff is the old reloc-masked view (branch targets -> `L`,
jal -> `jal R`, relocated immediates -> `R`); it is close to objdiff's fuzzy
score and HIDES decode errors (agent-c run 120: six "caps" were wrong C).

Rules:
  * Branch offsets are measured from the FUNCTION'S FIRST LABEL (its symbol),
    never from the last label seen.  bootup_uso expected objects carry
    `alabel D_XXXX` symbols inside functions and print their relocs out of
    place, so label-relative offsets from objdump are wrong there.
  * Summary `diffwords N` counts the larger side of every non-equal opcode run
    (difflib), i.e. roughly the number of target words still wrong.
  * Classify a residue from the --raw --pins output: branch-only rows
    (`b... L<x>`) = control-flow extent; `jal` rows = baked target / pin /
    wrong callee; same mnemonics with permuted registers = colouring; the same
    instruction multiset in a different order = schedule.
"""
import os
import re
import subprocess
import sys
import difflib

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
ARGS = sys.argv[1:]
FLAGS = {a for a in ARGS if a.startswith('-') and a != '--unit'}
RAW = '--raw' in FLAGS
PINS_ON = '--pins' in FLAGS

_pins = None


def pins():
    global _pins
    if _pins is None:
        _pins = {}
        path = os.path.join(ROOT, 'undefined_syms_auto.txt')
        for l in open(path):
            m = re.match(r'\s*(\S+)\s*=\s*(0x[0-9a-fA-F]+|\d+)\s*;', l)
            if m:
                _pins[m.group(1)] = int(m.group(2), 0)
    return _pins


def sext16(v):
    v &= 0xFFFF
    return v - 0x10000 if v >= 0x8000 else v


IMM_TAIL = r',(-?0x[0-9a-f]+|-?\d+)$'
IMM_MEM = r'(-?0x[0-9a-f]+|-?\d+)(\(|$)'


def disasm(obj, fn):
    out = subprocess.run(['mips-linux-gnu-objdump', '-dr', '-M', 'no-aliases',
                          '--disassemble=' + fn, obj],
                         capture_output=True, text=True).stdout
    rows = []
    at = {}
    pending = []
    start = None
    for l in out.splitlines():
        mm = re.match(r'([0-9a-f]+) <', l)
        if mm and start is None:
            start = int(mm.group(1), 16)
        m = re.match(r'\s+([0-9a-f]+):\s+([0-9a-f]{8})\s+(\S+)\s*(.*)', l)
        if m:
            op, args = m.group(3), re.sub(r'<.*>', '', m.group(4)).strip()
            if (op[0] in 'bj') and op not in ('jr', 'jalr'):
                if op == 'jal' and RAW:
                    pass
                elif RAW and '--maskbr' not in FLAGS and op != 'jal' and start is not None:
                    args = re.sub(r'(^|,)([0-9a-f]+)$',
                                  lambda q: q.group(1) + 'L%x' % (int(q.group(2), 16) - start), args)
                else:
                    args = re.sub(r'(^|,)[0-9a-f]+$', r'\1L', args)
            if '--nosp' in FLAGS:
                args = re.sub(r'-?\d+\(sp\)', 'N(sp)', args)
                args = re.sub(r'sp,sp,-?\d+', 'sp,sp,N', args)
                args = re.sub(r',sp,\d+', ',sp,N', args)
            at[int(m.group(1), 16)] = len(rows)
            rows.append([op, args, None, None])
            continue
        # objdump can print a reloc block out of place (bootup/post objects):
        # attach each reloc to the instruction at its offset, not the last one.
        m = re.match(r'\s+([0-9a-f]+):\s+R_MIPS_(\w+)\s+(\S+)', l)
        if m:
            k = at.get(int(m.group(1), 16))
            if k is not None:
                rows[k][2], rows[k][3] = m.group(2), m.group(3)
            else:
                pending.append((int(m.group(1), 16), m.group(2), m.group(3)))
    for off, rt, sym in pending:  # relocs printed before their instruction
        k = at.get(off)
        if k is not None:
            rows[k][2], rows[k][3] = rt, sym
    return rows


def apply_pins(rows):
    P = pins()
    for i, (op, args, rt, sym) in enumerate(rows):
        if not rt or sym not in P:
            continue
        v = P[sym]
        if rt == 'HI16':
            m = re.search(IMM_TAIL, args)
            hi = int(m.group(1), 0) if m else 0
            lo = 0
            for op2, args2, rt2, sym2 in rows[i + 1:]:  # paired LO16 carries the low addend
                if rt2 == 'LO16' and sym2 == sym:
                    m2 = re.search(IMM_MEM, args2)
                    lo = sext16(int(m2.group(1), 0)) if m2 else 0
                    break
            full = v + (hi << 16) + lo
            args = re.sub(IMM_TAIL, ',' + hex(((full + 0x8000) >> 16) & 0xFFFF), args)
        elif rt == 'LO16':
            m = re.search(IMM_MEM, args)
            add = int(m.group(1), 0) if m else 0
            args = re.sub(IMM_MEM, str(sext16(v + add)) + '\\2', args, count=1)
        elif rt == '26':
            args = '%x' % v
        rows[i] = [op, args, None, None]
    return rows


def render(obj, fn):
    rows = disasm(obj, fn)
    if RAW and PINS_ON:
        rows = apply_pins(rows)
    res = []
    for op, args, rt, sym in rows:
        if rt and not RAW:
            if rt == 'LO16':
                args = re.sub(IMM_MEM, 'R\\2', args, count=1)
            elif rt == 'HI16':
                args = re.sub(r',(0x[0-9a-f]+|\d+)$', ',R', args)
            elif rt == '26':
                args = 'R'
            res.append((op + ' ' + args).strip() + ('  %' + sym if '--sym' in FLAGS else ''))
            continue
        if not RAW:
            if op == 'lui':
                args = re.sub(r',0x0$', ',R', args)
            elif op == 'addiu' and re.search(r',(zero|sp),', args) is None:
                args = re.sub(r',0$', ',R', args)
            elif op in ('lw', 'sw', 'lh', 'lhu', 'sh', 'lb', 'lbu', 'sb', 'lwc1', 'swc1'):
                args = re.sub(r',0\((?!sp)', ',R(', args)
        res.append((op + ' ' + args).strip())
    out = []
    for x in res:
        if not RAW:
            x = re.sub(r'^lui (\w+),\S+$', r'lui \1,R', x)
            x = re.sub(r'^addiu (\w+),\1,-?\w+$', r'addiu \1,\1,R', x)
            x = re.sub(r'^jal \S+$', 'jal R', x)
        if '--noreg' in FLAGS:
            x = re.sub(r'\$?\b(s[0-8]|t[0-9]|v[01]|a[0-3]|at|f[0-9]+)\b', 'R', x)
        out.append(x)
    return out


def main():
    pos = [a for a in ARGS if not a.startswith('-')]
    if '--unit' in ARGS:
        if len(pos) != 2:
            sys.exit(__doc__)
        unit = pos[0][:-2] if pos[0].endswith('.c') else pos[0]
        ours = os.path.join(ROOT, 'build/non_matching', unit + '.c.o')
        target = os.path.join(ROOT, 'expected', unit + '.c.o')
        fn = pos[1]
    else:
        if len(pos) != 3:
            sys.exit(__doc__)
        ours, target, fn = pos
    a, b = render(ours, fn), render(target, fn)
    sm = difflib.SequenceMatcher(None, a, b, autojunk=False)
    nd = 0
    for tag, i1, i2, j1, j2 in sm.get_opcodes():
        if tag == 'equal':
            continue
        nd += max(i2 - i1, j2 - j1)
        if '-q' in FLAGS:
            continue
        print(f"--- {tag} ours[{i1}:{i2}] target[{j1}:{j2}]")
        for x in a[i1:i2]:
            print("  < " + x)
        for x in b[j1:j2]:
            print("  > " + x)
    print("ratio %.4f len %d vs %d, diffwords %d" % (sm.ratio(), len(a), len(b), nd))


if __name__ == '__main__':
    main()
