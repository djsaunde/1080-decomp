#!/usr/bin/env python3
"""End-to-end race check (plan 20.3): the scripted Crystal Lake run reaches the goal, its replay
replays it, and the ghost it saves races the same line after a reload.

    port/tools/race-check.py -b <port build dir> -o <new scratch dir> [-r ROM]
                             [-s port/host/race_crystal_lake.input]
                             [-g port/host/race_crystal_lake_ghost.input]

Two tenshoe runs, each from a fresh directory (no SRAM):
  race    the script to its `# END` step: entries[0].pct reaches 100 ("Goal!") at the script's
          `# GOAL` step, then (the script's post-goal tail) name entry, TimMenu, View Replay, Update
          Ghost -> Yes.  The View Replay playback (flow 7) must give r0's position bit for bit
          equal to the race's, from the race start through the goal, and the ghost save must write
          SRAM (more writes than the boot's).
  ghost   a new process on that 1080.sram (the ghost script: Time Attack, Crystal Lake, idle pad):
          the ghost rider r1 must match the race's r0 bit for bit through the goal.
One summary line per check; exit status = number of failed checks.  The input script is tied to
the exact race physics: a physics-affecting change legitimately breaks it, and the failure text
says how to regenerate it (port/tools/steer-search.py).
"""
import argparse, os, re, shutil, struct, subprocess, sys

HERE = os.path.dirname(os.path.abspath(__file__))
GAME = os.path.dirname(os.path.dirname(HERE))
MONO = os.path.dirname(os.path.dirname(GAME))
RUN32 = os.environ.get('RUN32', os.path.join(MONO, 'tools/i386-sysroot/run32'))  # `env`: a 64-bit build
def load(path):
    """{step: (flow, raceState, r0 xyz hex, r1 xyz hex, pct hex)}"""
    out = {}
    for ln in open(path, errors='replace'):
        m = re.match(r'F (\d+) \S+ flow=(\d+),\S+ race=(\d+),', ln)
        if not m:
            continue
        r0 = re.search(r' r0=([0-9a-f]+,[0-9a-f]+,[0-9a-f]+),', ln)
        r1 = re.search(r' r1=([0-9a-f]+,[0-9a-f]+,[0-9a-f]+),', ln)
        e0 = re.search(r' e0=([0-9a-f]{8}),', ln)
        pct = struct.unpack('>f', bytes.fromhex(e0.group(1)))[0] if e0 else None
        out[int(m.group(1))] = (int(m.group(2)), int(m.group(3)), r0 and r0.group(1),
                                r1 and r1.group(1), pct)
    return out


def run(build, rom, d, script, frames, sram=None):
    os.makedirs(d, exist_ok=True)
    args = ['timeout', '300', RUN32, os.path.join(build, 'tenshoe'), '--rom', rom, '--input', script,
            '--frames', str(frames), '--trace', os.path.join(d, 'state.log')]
    if sram:
        args += ['--sram', sram]
    with open(os.path.join(d, 'run.log'), 'w') as fh:
        rc = subprocess.run(args, cwd=d, stdout=fh, stderr=subprocess.STDOUT).returncode
    txt = open(os.path.join(d, 'run.log'), errors='replace').read()
    m = re.search(r'(\d+) SRAM writes', txt)
    return rc, load(os.path.join(d, 'state.log')), int(m.group(1)) if m else None


def first(st, pred, after=0):
    return next((k for k in sorted(st) if k > after and pred(st[k])), None)


def compare(a, a0, b, b0, n, ia, ib):
    """first step offset in 0..n where rider field ia of a (from a0) != field ib of b (from b0)"""
    for k in range(n + 1):
        x, y = a.get(a0 + k), b.get(b0 + k)
        if x is None or y is None or x[ia] is None or x[ia] != y[ib]:
            return k
    return None


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('-b', '--build', required=True)
    ap.add_argument('-o', '--outdir', required=True, help='a new scratch directory')
    ap.add_argument('-r', '--rom', default=os.path.join(GAME, 'baserom.z64'))
    ap.add_argument('-s', '--script', default=os.path.join(GAME, 'port/host/race_crystal_lake.input'))
    ap.add_argument('-g', '--ghost-script', default=os.path.join(GAME, 'port/host/race_crystal_lake_ghost.input'))
    a = ap.parse_args()
    text = open(a.script).read()
    want_goal = int(re.search(r'^# GOAL (\d+)', text, re.M).group(1))
    end = int(re.search(r'^# END (\d+)', text, re.M).group(1))
    regen = ('the input script is tied to the exact race physics; if this change is MEANT to change '
             'them, regenerate it:\n    port/tools/steer-search.py -b %s -o %s\n  and commit it '
             '(about 1-2 minutes)' % (a.build, os.path.relpath(a.script, GAME) if os.path.abspath(a.script).startswith(GAME + '/') else a.script))
    fails = []

    def line(name, ok, txt):
        print('  %-6s %-4s %s' % (name, 'PASS' if ok else 'FAIL', txt))
        if not ok:
            fails.append(name)

    rd = os.path.join(a.outdir, 'race')
    rc, st, writes = run(a.build, a.rom, rd, os.path.abspath(a.script), end)
    start = first(st, lambda v: v[0] == 5 and v[1] == 1)
    goal = first(st, lambda v: v[4] is not None and v[4] >= 100.0, start or 0)
    retire = first(st, lambda v: v[0] == 5 and v[1] == 2, start or 0)
    ok = rc == 0 and goal == want_goal
    if goal is None:
        why = 'race ended at step %s short of the goal (retire)' % retire if retire else 'no goal by step %d' % end
    else:
        why = 'goal at step %d (script: %d)' % (goal, want_goal)
    line('goal', ok, 'rc %d, race start %s, %s' % (rc, start, why))

    rep = first(st, lambda v: v[0] == 7 and v[1] == 1, goal or want_goal)
    if goal and start and rep:
        n = goal - start
        bad = compare(st, start, st, rep, n, 2, 2)
        line('replay', bad is None, 'View Replay from %d: r0 position %s' % (
            rep, 'bit-identical to the race for %d steps (start..goal)' % (n + 1) if bad is None
            else 'differs from race step %d on' % (start + bad)))
    else:
        line('replay', False, 'no View Replay playback (flow 7, race state 1) after the goal')

    # the boot writes the SRAM once (header); the ghost save adds more
    sram = os.path.join(rd, '1080.sram')
    gd = os.path.join(a.outdir, 'ghost')
    if writes and writes > 1 and os.path.exists(sram) and goal and start:
        os.makedirs(gd, exist_ok=True)
        shutil.copyfile(sram, os.path.join(gd, '1080.sram'))
        rc2, st2, _ = run(a.build, a.rom, gd, os.path.abspath(a.ghost_script), 1400 + (goal - start) + 60,
                          os.path.join(gd, '1080.sram'))
        g0 = first(st2, lambda v: v[0] == 5 and v[1] == 1)
        n = goal - start
        bad = None if g0 is None else compare(st, start, st2, g0, n, 2, 3)
        line('ghost', rc2 == 0 and g0 is not None and bad is None, '%d SRAM writes; reload rc %d: ghost r1 %s' % (
            writes, rc2, 'no race' if g0 is None else 'bit-identical to the race r0 for %d steps' % (n + 1)
            if bad is None else 'differs from race step %d on' % (start + bad)))
    else:
        line('ghost', False, 'no ghost save (%s SRAM writes) or no goal' % writes)
    if fails:
        print('  ' + regen)
    sys.exit(len(fails))


if __name__ == '__main__':
    main()
