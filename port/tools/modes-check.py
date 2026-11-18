#!/usr/bin/env python3
"""The port gate's `modes` step (plan 20.5, run 857): every game mode headless, in a few seconds.

    port/tools/modes-check.py -b <port build dir> -o <new scratch dir> [-r ROM] [-j JOBS] [--gfx-steps STEPS]

Each check runs tenshoe headless from a fresh directory (blank SRAM unless stated) on a committed
input script (port/host/*.input) and reads the --trace state.log (port/trace.h):

  trick     trick_attack.input (Trick Attack): trick points > 0 (e0 score), and at least the
            `# CHECKPOINTS n` countdown extensions the script records (a "chk" gate passed between
            its posts adds its seconds: the trace's cd= countdown jumps up)
  contest   contest.input (Contest): points > 0 and the countdown extended at least once (a cleared
            slalom gate adds 2 s)
  training  training.input (Training): the lesson (trn=) advances past the first one
  vs2p      vs2p.input (2P VS, two pads): the race ends (race state 2) and the next round starts
            (race state 1 again), and both riders' positions equal the real game's (the N64 on n64ref,
            port/tools/n64ref/vs2p-golden.tsv, every 8th step of the round; plan 24)
  match     match_race.input (Match Race): the player reaches the goal; when the script says
            `# WIN`, before the CPU rider (entries[0] finished while entries[1] has not).  With
            `# RECORD m'ss"hh` (run 860: Round 2 paused -> Quit closes the session, name "AAA"):
            that time is saved in SRAM under AAA (course records, mirror equal), and
            records_reload.input in a NEW process on that file leaves it byte-identical
  options   options_save.input from a blank SRAM: the options record (SRAM 0x1F8, mirror 0x7FC8)
            holds the new settings (flags & 3 = 2 Mono, levels 9 / 7 / 8); options_reload.input in
            a NEW process on that file: BGM read back and stepped once more (levels 10 / 7 / 8)

The race scripts are tied to the exact physics (a physics-affecting fix can make a scripted rider
miss a gate, crash or lose): the failure line prints the steer-search command that regenerates
each one (port/tools/steer-search.py; plan 20.3).  Exit status: the number of failed checks.
"""
import argparse, os, re, struct, subprocess, sys
from concurrent.futures import ThreadPoolExecutor

HERE = os.path.dirname(os.path.abspath(__file__))
GAME = os.path.dirname(os.path.dirname(HERE))
MONO = os.path.dirname(os.path.dirname(GAME))
RUN32 = os.environ.get('RUN32', os.path.join(MONO, 'tools/i386-sysroot/run32'))
HOST = os.path.join(GAME, 'port/host')

REGEN = {
    'trick': 'STEER_TRICK_ACTS=fast STEER_POINTS_W=0.2 STEER_POINTS_CAP=150 STEER_JUMP_UNTIL=1700 '
             'port/tools/steer-search.py -b <port build> --mode trick',
    'contest': 'port/tools/steer-search.py -b <port build> --mode contest',
    'match': 'port/tools/steer-search.py -b <port build> --mode match',
    'training': 'edit port/host/training.input (hop + grab windows; header comment)',
    'vs2p': 'edit port/host/vs2p.input (both pads ride straight; header comment); a golden mismatch is a port '
            'bug (plan 24) unless the script changed: then re-record port/tools/n64ref/vs2p-golden.py',
    'options': 'edit port/host/options_save.input / options_reload.input (menu steps; header comments)',
}


def f32(h):
    return struct.unpack('>f', bytes.fromhex(h))[0]


def run(a, name, script, frames, sram=None):
    d = os.path.join(a.outdir, name)
    os.makedirs(d, exist_ok=True)
    sram = sram or os.path.join(d, '1080.sram')
    args = ['timeout', '120', RUN32, os.path.join(a.build, 'tenshoe'), '--rom', a.rom, '--input',
            os.path.join(HOST, script), '--frames', str(frames), '--trace', os.path.join(d, 'state.log'),
            '--sram', sram]
    env = dict(os.environ)
    if name == 'match' and a.gfx_steps:  # the build64 gate compares the Select Level screen's gfx (run 872)
        env.update(PORT_GFX_DUMP=os.path.join(d, 'gfx'), PORT_GFX_DUMP_STEPS=a.gfx_steps)
    with open(os.path.join(d, 'run.log'), 'w') as fh:
        rc = subprocess.run(args, cwd=d, stdout=fh, stderr=subprocess.STDOUT, env=env).returncode
    return rc, os.path.join(d, 'state.log')


def lines(path):
    try:
        return open(path, errors='replace').read().splitlines()
    except OSError:
        return []


def end_of(script, default):
    m = re.search(r'^# END (\d+)', open(os.path.join(HOST, script)).read(), re.M)
    return int(m.group(1)) if m else default


def races(log):
    """[(step, raceState)] for every line with a race= field"""
    out = []
    for ln in lines(log):
        m = re.match(r'F (\d+) \S+ \S+ race=(\d+),', ln)
        if m:
            out.append((int(m.group(1)), int(m.group(2))))
    return out


def points_and_extensions(log):
    """(max e0 score, countdown extensions) over the race (race state 1)"""
    pts, ext, prev = 0, 0, None
    for ln in lines(log):
        m = re.search(r' race=1,.* e0=[0-9a-f]{8},[0-9a-f]{8},-?\d+,-?\d+,(-?\d+),', ln)
        c = re.search(r' cd=(-?\d+),', ln)
        if not m or not c:
            prev = None
            continue
        pts = max(pts, int(m.group(1)))
        cd = int(c.group(1))
        if prev is not None and cd > prev + 500:
            ext += 1
        prev = cd
    return pts, ext


def check_trick(a, name='trick', script='trick_attack.input'):
    text = open(os.path.join(HOST, script)).read()
    m = re.search(r'^# CHECKPOINTS (\d+)', text, re.M)
    want = int(m.group(1)) if m else 1
    goal_want = re.search(r'^# GOAL', text, re.M) is not None
    rc, log = run(a, name, script, end_of(script, 4000))
    pts, ext = points_and_extensions(log)
    goal = None
    for ln in lines(log):
        m = re.search(r' e0=([0-9a-f]{8}),', ln)
        if m and f32(m.group(1)) >= 100.0:
            goal = int(ln.split()[1])
            break
    ok = rc == 0 and pts > 0 and ext >= want and (goal is not None or not goal_want)
    return ok, 'rc %d, points %d, countdown extensions %d (want >= %d)%s' % (
        rc, pts, ext, want, ', goal %s (script: # GOAL)' % goal if goal_want else '')


def check_contest(a):
    rc, log = run(a, 'contest', 'contest.input', end_of('contest.input', 3800))
    pts, ext = points_and_extensions(log)
    return rc == 0 and pts > 0 and ext >= 1, 'rc %d, points %d, countdown extensions %d' % (rc, pts, ext)


def check_training(a):
    rc, log = run(a, 'training', 'training.input', 3000)
    seq = []
    for ln in lines(log):
        m = re.search(r' trn=(-?\d+)', ln)
        if m and (not seq or seq[-1] != int(m.group(1))):
            seq.append(int(m.group(1)))
    return rc == 0 and len(seq) >= 2, 'rc %d, lessons %s' % (rc, seq or 'none (no trn= in the trace)')


VS2P_GOLDEN = os.path.join(HERE, 'n64ref', 'vs2p-golden.tsv')


def vs2p_vs_n64(log):
    """(steps compared, first step whose r0= / r1= position differs from the N64's, or None) against
    port/tools/n64ref/vs2p-golden.tsv (plan 24: the real game on n64ref, every 8th step of the round)"""
    want = {}
    for ln in lines(VS2P_GOLDEN):
        if ln and not ln.startswith('#'):
            f = ln.split()
            want[int(f[0])] = f[1:]
    n = 0
    for ln in lines(log):
        m = re.match(r'F (\d+) ', ln)
        if not m or int(m.group(1)) not in want:
            continue
        st = int(m.group(1))
        got = []
        for i in (0, 1):
            r = re.search(r' r%d=([0-9a-f]{8}),([0-9a-f]{8}),([0-9a-f]{8}),' % i, ln)
            got += list(r.groups()) if r else ['-'] * 3
        if got != want[st]:
            return n, st
        n += 1
    return n, None


def check_vs2p(a):
    rc, log = run(a, 'vs2p', 'vs2p.input', end_of('vs2p.input', 7800))
    r = races(log)
    end = next((s for s, x in r if x == 2), None)
    nxt = next((s for s, x in r if end is not None and s > end and x == 1), None)
    n, bad = vs2p_vs_n64(log)
    total = sum(1 for ln in lines(VS2P_GOLDEN) if ln and not ln.startswith('#'))
    ok = rc == 0 and end is not None and nxt is not None and bad is None and n == total
    return ok, 'rc %d, race end %s, next round %s; N64 rider positions %d/%d steps%s' % (
        rc, end, nxt, n, total, '' if bad is None else ', first differs at step %d' % bad)


def aaa_record(b):
    """(entry, records index, min, sec, hundredths) of the first SRAM time record named "AAA" (the
    name entry's A A A), or None.  gdl/backup.h: records[k] entry j at 0x8 + k * 0x30 + j * 8, the
    name's three letters at +2 (0 = A), time[3] at +5 (steer-search.py writes `# RECORD` the same way)."""
    for k in range(8):
        for j in range(3):
            e = 0x8 + k * 0x30 + j * 8
            if b[e + 2:e + 5] == bytes(3) and any(b[e + 5:e + 8]):
                return (j, k) + tuple(b[e + 5:e + 8])
    return None


def read_sram(path):
    try:
        b = open(path, 'rb').read()
    except OSError:
        return None
    return b if len(b) == 0x8000 else None


def check_match(a):
    text = open(os.path.join(HOST, 'match_race.input')).read()
    win = re.search(r'^# WIN', text, re.M) is not None
    m = re.search(r'^# RECORD (\d+)\'(\d+)"(\d+)', text, re.M)
    want = tuple(int(x) for x in m.groups()) if m else None
    rc, log = run(a, 'match', 'match_race.input', end_of('match_race.input', 6000))
    goal, cpu = None, None
    for ln in lines(log):
        m = re.search(r' e0=([0-9a-f]{8}),[0-9a-f]{8},(-?\d+),', ln)
        e1 = re.search(r' e1=([0-9a-f]{8}),(-?\d+),', ln)
        step = int(ln.split()[1]) if ln.startswith('F ') else 0
        if m and goal is None and f32(m.group(1)) >= 100.0:
            goal = step
        if e1 and cpu is None and int(e1.group(2)) > 0:
            cpu = step
    ok = rc == 0 and goal is not None and (not win or cpu is None or goal < cpu)
    txt = 'rc %d, player goal %s, CPU finish %s%s' % (rc, goal, cpu, ' (script: # WIN)' if win else '')
    if want is None:
        return ok, txt
    # run 860: pause -> Quit closed the session, the record is saved (primary = mirror), and a new
    # process boots on that file without rewriting it (records_reload.input: Time Attack, BEST TIME)
    sram = os.path.join(a.outdir, 'match', '1080.sram')
    b = read_sram(sram)
    rec = aaa_record(b) if b else None
    mirror = b is not None and b[0x8:0x188] == b[0x7DD8:0x7F58]
    reload_sram = os.path.join(a.outdir, 'match', 'reload.sram')
    rc2, same = None, False
    if b:
        open(reload_sram, 'wb').write(b)
        rc2, _ = run(a, 'match-reload', 'records_reload.input', end_of('records_reload.input', 1650), reload_sram)
        same = read_sram(reload_sram) == b
    ok = ok and rec is not None and rec[2:] == want and mirror and rc2 == 0 and same
    got = 'none' if rec is None else '%d\'%02d"%02d (records[%d] entry %d)' % (rec[2], rec[3], rec[4], rec[1], rec[0])
    return ok, txt + '; saved AAA %s (want %d\'%02d"%02d)%s; reload rc %s, file %s' % (
        got, want[0], want[1], want[2], '' if mirror else ' MIRROR DIFFERS', rc2, 'unchanged' if same else 'CHANGED')


def options_rec(path):
    try:
        b = open(path, 'rb').read()
    except OSError:
        return None
    if len(b) < 0x8000:
        return None
    p, q = b[0x1F8:0x1F8 + 0x18], b[0x7FC8:0x7FC8 + 0x18]
    return (struct.unpack('>H', p[4:6])[0] & 3, tuple(p[0x12:0x15]), p == q)


def check_options(a):
    rc1, _ = run(a, 'options', 'options_save.input', end_of('options_save.input', 1700))
    sram = os.path.join(a.outdir, 'options', '1080.sram')
    r1 = options_rec(sram)
    reload_sram = os.path.join(a.outdir, 'options', 'reload.sram')
    if os.path.exists(sram):
        open(reload_sram, 'wb').write(open(sram, 'rb').read())
    rc2, _ = run(a, 'options-reload', 'options_reload.input', end_of('options_reload.input', 1300), reload_sram)
    r2 = options_rec(reload_sram)
    ok = rc1 == 0 and rc2 == 0 and r1 == (2, (9, 7, 8), True) and r2 == (2, (10, 7, 8), True)
    fmt = lambda r: 'none' if r is None else 'mode %d levels %s%s' % (r[0], '/'.join(map(str, r[1])), '' if r[2] else ' MIRROR DIFFERS')
    return ok, 'saved: %s (want mode 2 levels 9/7/8); reloaded + BGM up: %s (want 10/7/8)' % (fmt(r1), fmt(r2))


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('-b', '--build', required=True)
    ap.add_argument('-o', '--outdir', required=True, help='a new scratch directory')
    ap.add_argument('-r', '--rom', default=os.path.join(GAME, 'baserom.z64'))
    ap.add_argument('-j', '--jobs', type=int, default=6)
    ap.add_argument('--gfx-steps', help='PORT_GFX_DUMP these steps of the match run (match/gfx; the build64 gate '
                    'compares them i386 against x86-64: Select Level, plan 21.11)')
    a = ap.parse_args()
    os.makedirs(a.outdir, exist_ok=True)
    checks = [('trick', check_trick), ('contest', check_contest), ('training', check_training),
              ('vs2p', check_vs2p), ('match', check_match), ('options', check_options)]
    with ThreadPoolExecutor(a.jobs) as ex:
        res = list(ex.map(lambda c: (c[0],) + c[1](a), checks))
    fails = 0
    for name, ok, txt in res:
        print('  %-8s %s  %s' % (name, 'ok  ' if ok else 'FAIL', txt))
        if not ok:
            fails += 1
            print('           regenerate: %s (then re-run the gate)' % REGEN[name])
    return fails


if __name__ == '__main__':
    sys.exit(main())
