#!/usr/bin/env python3
"""Find (and with --apply remove) never-compiled preprocessor blocks in C/C++ sources.

A block is dead when it is `#if 0`, or `#ifdef M` / `#if defined(M)` for a macro M that is never
#defined (nor -D'd) anywhere in Makefile, scripts/, include/ or src/. NON_MATCHING, __cplusplus,
PERMUTER and M2CTX are live switches and never count as dead. A dead block with an `#else` keeps
the else branch; `#elif` chains are left alone.

  scripts/deadblocks.py FILE.. [--apply]      list (remove) dead blocks; run from the project root

(Copied from the coordinator's /tmp/claude-1000/coord/deadblocks.py by agent-a run 517 and made
importable: scripts/readability-census.py uses dead_blocks().)
"""
import os
import re
import sys

LIVE_SWITCHES = ('NON_MATCHING', '__cplusplus', 'PERMUTER', 'M2CTX')


def defined_macros(root='.'):
    """Every macro #defined or -D'd anywhere in the project's Makefile / scripts / include / src."""
    defined = set()
    paths = [os.path.join(root, 'Makefile')]
    for d in ('scripts', 'include', 'src'):
        paths += [os.path.join(dp, f) for dp, _, fs in os.walk(os.path.join(root, d)) for f in fs]
    for p in paths:
        try:
            t = open(p, errors='ignore').read()
        except OSError:
            continue
        defined |= set(re.findall(r'#\s*define\s+(\w+)', t))
        defined |= set(re.findall(r'-D(\w+)', t))
    return defined


def dead_blocks(lines, defined):
    """-> (kept lines, [(start line 1-based, end line, header, removed line count, keeps else)])."""
    out, found, i = [], [], 0
    while i < len(lines):
        s = lines[i].strip()
        m0 = re.match(r'#\s*if\s+0\b', s)
        m1 = re.match(r'#\s*ifdef\s+(\w+)\s*$', s) or re.match(r'#\s*if\s+defined\s*\(\s*(\w+)\s*\)\s*$', s)
        dead = m0 or (m1 and m1.group(1) not in defined and m1.group(1) not in LIVE_SWITCHES)
        if not dead:
            out.append(lines[i])
            i += 1
            continue
        depth, j, els = 0, i, None
        while j < len(lines):
            t = lines[j].strip()
            if re.match(r'#\s*if', t):
                depth += 1
            elif re.match(r'#\s*endif', t):
                depth -= 1
                if depth == 0:
                    break
            elif depth == 1 and re.match(r'#\s*(else|elif)', t) and els is None:
                els = j
            j += 1
        if els is not None and re.match(r'#\s*elif', lines[els].strip()):
            out.append(lines[i])     # leave elif chains alone
            i += 1
            continue
        keep = lines[els + 1:j] if els is not None else []
        n = (j - i + 1) - len(keep)
        found.append((i + 1, j + 1, s, n, bool(keep)))
        out.extend(keep)
        i = j + 1
    return out, found


def main():
    apply = '--apply' in sys.argv
    files = [a for a in sys.argv[1:] if not a.startswith('--')]
    defined = defined_macros('.')
    tot = 0
    for p in files:
        lines = open(p).read().split('\n')
        out, found = dead_blocks(lines, defined)
        for a, b, hdr, n, keep in found:
            print(f"{p}:{a}-{b} {hdr} ({n} lines){' keep-else' if keep else ''}")
        removed = sum(f[3] for f in found)
        tot += removed
        if apply and removed:
            open(p, 'w').write('\n'.join(out))
    print('total lines removed', tot)


if __name__ == '__main__':
    main()
