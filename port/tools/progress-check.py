#!/usr/bin/env python3
"""Match Race progression / unlock check (run 871).

    port/tools/progress-check.py -b <port build> -o <new dir> [-s port/host/match_race_full.input] [-j 8]
    port/tools/progress-check.py -b <port build> -o <new dir> --sram SAVE --expect-levels N

Full mode (-s, slow: the whole progression from a blank save).  The script is
port/tools/steer-search.py --mode rounds --levels 3 output: `# ROUND k GOAL g WIN|LOSE` and
`# LEVEL n START s` notes.  For every level the script finishes, one run from a fresh directory stops
just before the next level's Select Level presses (`# LEVEL n START` - 300; the last level: `# END`),
and its 1080.sram is checked:
  * the options chunk (gdl/backup.h BackupOptions at 0x1F8): primary == mirror (0x7FC8), both chunk
    checksums and the header hash valid (the hardware sealing, sram-slow-records.py);
  * seqStep == 1 + levels cleared (settings_stepSequence: Normal -> 2 = Hard open, Hard -> 3 = Expert
    open, Expert -> 4); the unlock bits are printed (settings_setBit: Time Attack course k = bit k);
  * the reload: a new process on that save (no language menu) opens Match Race, presses DR once per
    level the save should have open and A through the rider / board screens; the trace's ml=
    (MgrCourseList retries, idx, count, course, rider; port/trace.c) must show the expected level's
    course count (Normal 4, Hard 5, Expert 6), and the save file must stay byte-identical (a clean
    load is one SRAM write, the header, with no chunk reset).
  * the trace shows every round: ml= idx advances once per WIN and retries drops once per LOSE.

--sram mode (the gate-sized check): only the reload of one committed save, `--expect-levels N` = the
number of levels it has cleared (N + 1 levels selectable, at most 3).
Exit status = number of failed checks.
"""
import argparse, os, re, shutil, struct, subprocess, sys
from concurrent.futures import ThreadPoolExecutor

HERE = os.path.dirname(os.path.abspath(__file__))
GAME = os.path.dirname(os.path.dirname(HERE))
MONO = os.path.dirname(os.path.dirname(GAME))
RUN32 = os.environ.get('RUN32', os.path.join(MONO, 'tools/i386-sysroot/run32'))
COUNTS = {1: 4, 2: 5, 3: 6}   # mgr_initMatchRace_00000700: courses per level (this cart: 4 on Normal)
CHUNKS = [(0x008, 0x188, 0x7DD8, 0x7F58, 0x180), (0x190, 0x1F0, 0x7F60, 0x7FC0, 0x60),
          (0x1F8, 0x210, 0x7FC8, 0x7FE0, 0x18), (0x218, 0x228, 0x7FE8, 0x7FF8, 0x10)]
CHUNK_KEY, HEADER_KEY = 0xDEADBBAD, 0xD1265205
ML = re.compile(r'^F (\d+) .* ml=(-?\d+),(-?\d+),(-?\d+),(-?\d+),(-?\d+)')


def checksum(src, key):
    s = sum(struct.unpack('>%dI' % (len(src) // 4), bytes(src))) & 0xFFFFFFFF
    c0 = (0xF251F205 - s) & 0xFFFFFFFF
    return struct.pack('>II', c0, (key - (c0 - s)) & 0xFFFFFFFF)


def options(path):
    """(unlocks, seqStep, problems[]) of a 1080.sram"""
    b = open(path, 'rb').read()
    bad = []
    if len(b) != 0x8000:
        return None, None, ['not a 32 KB image']
    hdr = b''
    for off, sm, mir, mirs, size in CHUNKS:
        c = checksum(b[off:off + size], CHUNK_KEY)
        if b[sm:sm + 8] != c:
            bad.append('chunk 0x%X checksum' % off)
        if b[mir:mir + size] != b[off:off + size] or b[mirs:mirs + 8] != b[sm:sm + 8]:
            bad.append('chunk 0x%X mirror' % off)
        hdr += b[sm:sm + 8]
    if b[0:8] != checksum(hdr, HEADER_KEY):
        bad.append('header hash')
    unlocks, seq = struct.unpack('>HH', b[0x1F8:0x1FC])
    return unlocks, seq, bad


def run(build, rom, d, script, frames, sram=None, env=None):
    os.makedirs(d, exist_ok=True)
    if sram:
        shutil.copy(sram, os.path.join(d, '1080.sram'))
    with open(os.path.join(d, 's.input'), 'w') as fh:
        fh.write(script)
    with open(os.path.join(d, 'run.log'), 'w') as fh:
        rc = subprocess.run(['timeout', '600', RUN32, os.path.join(build, 'tenshoe'), '--rom', rom, '--input',
                             's.input', '--frames', str(frames), '--trace', 'state.log'], cwd=d, stdout=fh,
                            stderr=subprocess.STDOUT, env=env).returncode
    return rc


def reload_script(dr):
    """a saved SRAM: no language menu.  Match Race (the first main-menu item) -> Select Level -> DR x dr
    -> A (level) -> rider, confirm -> board, confirm -> round 1 loads"""
    s = ('# progress-check.py: Select Level on a saved SRAM, DR x %d\nPADS 1\n'
         'F 480:3 START          # title -> main menu\nF 600:3 A              # Match Race -> Select Level\n' % dr)
    t = 860
    for k in range(dr):
        s += 'F %d:3 DR\n' % t
        t += 60
    for c in ('level', 'rider', 'rider confirm', 'board', 'board confirm'):
        t += 100
        s += 'F %d:3 A              # %s\n' % (t, c)
    return s, t + 400


def check_reload(a, d, sram, cleared):
    """the save's levels: DR x min(cleared, 2) must land on level min(cleared, 2) + 1; one DR more than
    is open must stay put (a locked card).  Returns [(name, ok, text)]."""
    out = []
    want = min(cleared, 2)
    tries = [(want, want + 1)] + ([(want + 1, want + 1)] if want < 2 else [])
    for dr, level in tries:
        dd = os.path.join(d, 'reload_dr%d' % dr)
        text, frames = reload_script(dr)
        rc = run(a.build, a.rom, dd, text, frames, sram)
        after = open(os.path.join(dd, '1080.sram'), 'rb').read() if os.path.exists(os.path.join(dd, '1080.sram')) else b''
        counts = {int(m.group(4)) for m in map(ML.match, open(os.path.join(dd, 'state.log'), errors='replace')) if m}
        ok = rc == 0 and counts == {COUNTS[level]} and after == open(sram, 'rb').read()
        out.append(('reload DR x%d' % dr, ok, 'rc %d, Match Race course count %s (want %d = level %d), save %s'
                    % (rc, sorted(counts), COUNTS[level], level,
                       'unchanged' if after == open(sram, 'rb').read() else 'CHANGED')))
    return out


def rounds_of(path, start, end):
    """[(step, retries, idx, count)] where ml= changes, between two steps"""
    out, prev = [], None
    for ln in open(path, errors='replace'):
        m = ML.match(ln)
        if m and start <= int(m.group(1)) <= end:
            cur = tuple(int(m.group(k)) for k in (2, 3, 4))
            if cur != prev:
                out.append((int(m.group(1)),) + cur)
                prev = cur
    return out


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('-b', '--build', required=True)
    ap.add_argument('-o', '--out', required=True, help='a new scratch dir')
    ap.add_argument('-r', '--rom', default=os.path.join(GAME, 'baserom.z64'))
    ap.add_argument('-s', '--script', default=os.path.join(GAME, 'port/host/match_race_full.input'))
    ap.add_argument('--sram', help='reload check only: a committed save')
    ap.add_argument('--expect-levels', type=int, default=None, help='--sram: the levels that save has cleared')
    a = ap.parse_args()
    if os.path.exists(a.out) and os.listdir(a.out):
        sys.exit('progress-check: %s is not empty (start from a fresh dir)' % a.out)
    os.makedirs(a.out, exist_ok=True)
    results = []
    if a.sram:
        u, q, bad = options(a.sram)
        cleared = a.expect_levels if a.expect_levels is not None else (q or 1) - 1
        results.append(('save', not bad and q == cleared + 1,
                        'unlocks 0x%04X seqStep %s (want %d)%s' % (u or 0, q, cleared + 1,
                                                                  ', ' + ', '.join(bad) if bad else '')))
        results += check_reload(a, a.out, a.sram, cleared)
    else:
        text = open(a.script).read()
        levels = [(int(n), int(s)) for n, s in re.findall(r'^# LEVEL (\d+) START (\d+)', text, re.M)]
        goals = [int(g) for g in re.findall(r'^# ROUND \d+ GOAL (\d+)', text, re.M)]
        end = int(re.search(r'^# END (\d+)', text, re.M).group(1)) if re.search(r'^# END', text, re.M) else max(goals) + 3000
        stops = [(n - 1, s - 300) for n, s in levels]
        if re.search(r'^# END', text, re.M) and re.search(r'^# LEVELS CLEARED (\d+)', text, re.M):
            stops.append((int(re.search(r'^# LEVELS CLEARED (\d+)', text, re.M).group(1)), end))

        def one(st):
            lv, frames = st
            d = os.path.join(a.out, 'level%d' % lv)
            rc = run(a.build, a.rom, d, text, frames)
            return lv, d, rc
        with ThreadPoolExecutor(max(1, len(stops))) as ex:
            done = list(ex.map(one, stops))
        for lv, d, rc in done:
            sram = os.path.join(d, '1080.sram')
            u, q, bad = options(sram) if os.path.exists(sram) else (None, None, ['no 1080.sram'])
            results.append(('level %d save' % lv, rc == 0 and not bad and q == lv + 1,
                            'rc %d, unlocks 0x%04X, seqStep %s (want %d)%s' % (rc, u or 0, q, lv + 1,
                                                                            ', ' + ', '.join(bad) if bad else '')))
            if not bad:
                results += [('level %d %s' % (lv, n), ok, t) for n, ok, t in check_reload(a, d, sram, lv)]
        if done:
            log = os.path.join(max(done)[1], 'state.log')
            ch = rounds_of(log, 0, 1 << 30)
            print('ml= changes (step, retries, idx, count):', ' '.join('%d:%d/%d/%d' % c for c in ch))
    fails = 0
    for name, ok, txt in results:
        print('%-22s %s  %s' % (name, 'PASS' if ok else 'FAIL', txt))
        fails += not ok
    sys.exit(fails)


if __name__ == '__main__':
    main()
