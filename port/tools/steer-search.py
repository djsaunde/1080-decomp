#!/usr/bin/env python3
"""Steering search: find a stick script that rides Crystal Lake (Time Attack) to the goal.

    port/tools/steer-search.py -b <port build dir> [-o port/host/race_crystal_lake.input]
                               [-w 60] [-j 5] [--budget 900] [--keep-prefix STEP]
    port/tools/steer-search.py -b <port build dir> --mode trick [-o port/host/trick_attack.input]
    port/tools/steer-search.py -b <port build dir> --mode match [-o port/host/match_race.input]

The race replays bit for bit from a given input script (the port is deterministic), so the search
runs tenshoe headless from boot again for every candidate (about 2 s for 6000 steps) and keeps no
emulator state.  It is a greedy search over fixed windows with backtracking:

  * every window of W steps (default 60) after the race start holds one stick state, x in
    {-80, -40, 0, 40, 80}, y = 80 (lean forward);
  * each candidate is held for 2 W steps (a one-window lookahead) and scored from the last
    state.log line: entries[0].pct (course progress, 0..100) + 0.1 x health; a "Goal!"
    (pct 100 / race state 2 with finished > 0) wins outright, a retire (health <= 0, race state
    2 short of the goal) is out;
  * the best candidate is committed and the next window searched; when every candidate of a
    window retires, the search backs up one window and takes that window's next-best candidate.

The physics decides which stick sequence works, so ANY physics-affecting change (collision,
rider parameters, a float / byte-order fix in the race code) can legitimately make an old script
retire or miss the goal: then re-run this tool and commit the new script (scripts/port-gate.sh's
race step prints the command).  With -o pointing at an existing script, windows before
--keep-prefix (default: none) are reused as is.

The output script is the menu head (boot -> title -> Time Attack -> Akari -> board -> Crystal
Lake), the STICK windows, then a post-goal tail placed relative to the goal step: name entry
(A) -> TimMenu -> View Replay, and after the replay -> Update Ghost -> Yes (an SRAM ghost
save).  `# GOAL <step>` and `# END <step>` comments record the goal step and the step count
the gate should run (port/tools/race-check.py reads them).

--mode trick (run 853): Trick Attack (flow mode 6, trkproc) on its default course.  Every window
also has a "jump + grab" variant: A held for the first 20 steps (crouch, released = hop / jump),
B (grab) from step 15 to 50 with the stick centred in y (stick forward at the landing crashes the
rider), so a trick is the grab (+ stick direction) of include/game/tricktable.h.  The score adds
entries[0].score + pendingPoints / 20 (the trace's e0 fields 5 / 6), and the run ends with the
countdown ("Time Up", race state 2): the search stops at the first window whose best candidate
ends the race.  No post-race tail: `# END` is the race end + 300 (TrkMenu is up by then).

--mode contest (run 853): Contest (flow mode 5, arcproc), searched like --mode trick (gates and tricks
score; the countdown ends the run).

--mode match (run 853): Match Race (flow mode 3, mgrproc) against the CPU rider (SnowAI, the trace's
e1 / r1), level Normal, the first course (Crystal Lake).  The same window search as Time Attack
(the player's pct + health; a retire is out); the tail after the player's goal only presses A
through the result screens (`# GOAL`, `# END`).  Run 857: 30-step windows, 11 stick states with y
centred (x 0, +-17, +-24, +-32, +-48, +-80: the CPU rider steers with |stickX| mostly under 0.15 and
stickY 0; STEER_MATCH_ACTS=wide adds lean / crouch), the score adds STEER_SPEED_W (0.05) per km/h,
takes STEER_CRASH_W (50) off a window that loses health and STEER_LINE_W (0.001) per unit of x off
the CPU rider's line (beyond 500).  The script gets `# WIN` when the player finishes first.

Run 857 (--mode trick / contest): the score is STEER_PCT_W (8) x pct + points / 20 + health / 10 +
2 per second left on the countdown (the trace's cd=: DataManager +0xA0), minus 1 per 400 units of x
off the next gate's line within 20000 units of it (a "chk" checkpoint must be crossed between its
posts, RaceTracker_passCheckpoints / CheckpointCyl_isBetweenPosts; a slalom gate on its side,
RaceTracker_passGates).  In Trick Attack a candidate that crosses a checkpoint's line outside the
posts is out (backtracking as in --mode time), and the script records `# CHECKPOINTS n`, the
countdown extensions, for port/tools/modes-check.py.
"""
import argparse, os, re, shutil, struct, subprocess, sys, tempfile, time
from concurrent.futures import ThreadPoolExecutor

HERE = os.path.dirname(os.path.abspath(__file__))
GAME = os.path.dirname(os.path.dirname(HERE))
MONO = os.path.dirname(os.path.dirname(GAME))
RUN32 = os.environ.get('RUN32', os.path.join(MONO, 'tools/i386-sysroot/run32'))   # env: a scratch checkout

HEAD = """\
# Crystal Lake Time Attack, ridden to the goal (port/tools/steer-search.py output; plan 20.3).
# Format: port/host/input_script.c.  Steps are SysLib.frameCount (state.log's F number).
# Tied to the exact race physics: when a physics-affecting fix makes the rider retire or miss the
# goal, regenerate it:  port/tools/steer-search.py -b <port build> -o port/host/race_crystal_lake.input
PADS 1
F 480:3 START          # title "Press START" -> language select
F 560:3 A              # English
F 620:3 DD             # main menu: Match Race -> Time Attack
F 680:3 A              # Time Attack -> Select Rider
F 950:3 A              # rider (Akari Hayami)
F 1050:3 A             # rider confirm -> Select Board
F 1150:3 A             # board -> Select Course
F 1360:3 A             # Crystal Lake (the default course) -> race load; race state 1 at ~1460
"""
RACE_FIRST = 1460      # first steered step (the race starts at ~1462 on main, run 848)
# The post-goal tail is built from probe runs (the screens after the goal depend on the time: a
# top-3 time opens name entry first, flow 12).  Menu waits, in steps, measured run 850:
NAME_FIRST = 743       # goal -> the name-entry cursor takes input (flow 12 from goal + 481)
NAME_STEP = 60         # A x 5: letters "AAA", then END
BEST_WAIT = 160        # "Best Times" (flow 12, race state 0 after the name entry) -> A -> TimMenu
MENU_WAIT = 90         # TimMenu (flow 10) -> first press
GHOST_END = 460        # END = replay end + this
ACTS = [(0, 80), (-40, 80), (40, 80), (-80, 80), (80, 80)]

TRICK_HEAD = """\
# Trick Attack, ridden with jumps and grabs (port/tools/steer-search.py --mode trick; run 853).
# Format: port/host/input_script.c.  Steps are SysLib.frameCount (state.log's F number).
# Tied to the exact race physics (regenerate: port/tools/steer-search.py -b <build> --mode trick).
# Each "jump + grab" window: A (crouch) 20 steps then released = hop, B (grab) held in the air;
# the trick is the grab + stick direction (include/game/tricktable.h, e.g. B + up = NOSE GRAB).
# Run 857: the search steers through the "chk" checkpoints (each one passed between its posts adds
# its seconds to the countdown; the trace's cd=); a window that misses one is backed out.
PADS 1
F 480:3 START          # title "Press START" -> language select
F 560:3 A              # English
F 620:3 DD             # main menu: Match Race -> Time Attack
F 680:3 DD             # -> Trick Attack
F 740:3 A              # Trick Attack (flow mode 6, TRICKMODE) -> Select Rider
F 1000:3 A             # rider
F 1100:3 A             # rider confirm -> Select Board
F 1200:3 A             # board -> Select Course
F 1350:3 A             # the default course -> race load; race state 1 at ~1424
"""
TRICK_FIRST = 1424

MATCH_HEAD = """\
# Match Race vs the CPU rider (port/tools/steer-search.py --mode match; runs 853, 857), level Normal,
# course 1 (Crystal Lake).  Format: port/host/input_script.c; steps are SysLib.frameCount.
# Run 857: 30-step windows of fine steering, stick y centred: the player wins ("You Win! Advance
# to next round"), the A presses go on to Round 2 (Crystal Peak).
# Tied to the exact race physics (regenerate: port/tools/steer-search.py -b <build> --mode match).
# Start from a fresh directory (a saved SRAM skips the language menu and shifts every step).
PADS 1
F 480:3 START          # title
F 560:3 A              # English
F 640:3 A              # Match Race (flow mode 3, MULTIGAMERACE)
F 900:3 A              # Select Level: Normal
F 1000:3 A             # Select Rider
F 1100:3 A             # rider confirm
F 1200:3 A             # Select Board
F 1300:3 A             # board confirm -> race load (flow 2, 4); race state 1 at ~1501
"""
MATCH_FIRST = 1500

CONTEST_HEAD = """\
# Contest (flow mode 5, ARCADEMODE: gates + tricks for points against the countdown), ridden by
# port/tools/steer-search.py --mode contest (run 853); the first course.  Format:
# port/host/input_script.c; steps are SysLib.frameCount.  Tied to the exact race physics.
PADS 1
F 480:3 START          # title
F 560:3 A              # English
F 620:3 DD             # Match Race -> Time Attack
F 680:3 DD             # -> Trick Attack
F 740:3 DD             # -> Contest
F 800:3 A              # Contest (flow 0,5,5)
F 1000:3 A             # rider
F 1100:3 A             # rider confirm
F 1200:3 A             # board
F 1300:3 A             # board confirm -> race load (flow 3, 5); race state 1 at ~1493
"""
CONTEST_FIRST = 1493
MATCH_TAIL_PRESSES = 8      # A every 120 steps after the goal: the result / standings screens
# (stick x, stick y, jump + grab)
# --mode match (run 857): fine steering with the stick y centred, as the CPU rider (SnowAI) rides
# (its stickX mostly within +-0.15, stickY 0: a 60-step window of full lean or crouch loses to it);
# raw 17 is just past the pad's 0.2 dead zone.  STEER_MATCH_ACTS=wide adds y 80 / -80.  (x, y, button)
MATCH_ACTS = [(x, 0, '') for x in (0, -17, 17, -24, 24, -32, 32, -48, 48, -80, 80)]
if os.environ.get('STEER_MATCH_ACTS') == 'wide':
    MATCH_ACTS += [(x, y, '') for y in (80, -80) for x in (0, -20, 20, -40, 40, -80, 80)]
SPEED_W = float(os.environ.get('STEER_SPEED_W', 0.05))   # --mode match: score per km/h at the lookahead end
TRICK_ACTS = [(x, 80, 0) for x in (0, -40, 40, -80, 80)] + [(x, 0, 1) for x in (0, -40, 40, -80, 80)]

LINE = re.compile(r'^F (\d+) .*?race=(\d+),.*? e0=([0-9a-f]{8}),([0-9a-f]{8}),(-?\d+),(-?\d+)(?:,(-?\d+),(-?\d+))?')


def f32(h):
    return struct.unpack('>f', bytes.fromhex(h))[0]


CD = re.compile(r' cd=(-?\d+),(\d+),([0-9a-f]{8}),([0-9a-f]{8}),([0-9a-f]{8}),([0-9a-f]{8}),(-?\d+)')
R0 = re.compile(r' r0=([0-9a-f]{8}),([0-9a-f]{8}),([0-9a-f]{8}),')
E1 = re.compile(r' e1=([0-9a-f]{8}),(-?\d+),')


def last_state(path):
    """(step, raceState, pct, finished, health, points, cd, r0, e1, speed) of the last line with an e0=
    field, and the goal step (first line with pct >= 100), or None.  cd: (countdown ms, chk gates
    left, chk x, chk z, slalom x, slalom z, slalom flags) or None; r0: rider 0 (x, z) or None;
    e1: (pct, finished) of entries[1] (the Match Race CPU rider) or None"""
    last, goal = None, None
    try:
        fh = open(path, errors='replace')
    except OSError:
        return None, None
    for ln in fh:
        m = LINE.match(ln)
        if m:
            c, r, e = CD.search(ln), R0.search(ln), E1.search(ln)
            last = (int(m.group(1)), int(m.group(2)), f32(m.group(3)), int(m.group(5)), int(m.group(6)),
                    int(m.group(7) or 0) + int(m.group(8) or 0),
                    c and (int(c.group(1)), int(c.group(2)), f32(c.group(3)), f32(c.group(4)),
                           f32(c.group(5)), f32(c.group(6)), int(c.group(7))),
                    r and (f32(r.group(1)), f32(r.group(3))),
                    e and (f32(e.group(1)), int(e.group(2))), f32(m.group(4)))
            if goal is None and last[2] >= 100.0:
                goal = last[0]
    return last, goal


PCT_W = float(os.environ.get('STEER_PCT_W', 8))   # --mode trick / contest: course progress weight
GATE_NEAR = 20000.0     # course units (world z) ahead of the rider: steer for the next gate
GATE_W = 1.0 / 400      # score per unit of x on the wrong side of a gate


def gate_score(st):
    """--mode trick / contest: 2 per second on the countdown (a "chk" checkpoint in Trick Attack, a
    cleared slalom gate in Contest extends it), minus GATE_W per unit of x the rider is off the next
    gate's line when it is within GATE_NEAR (between the chk posts, about +-880 around x; on the
    slalom gate's side: x - 200 and below for flags & 1, x + 200 and above otherwise)"""
    cd, r0 = st[6], st[7]
    if cd is None:
        return 0.0
    v = 2.0 * max(cd[0], 0) / 1000.0
    if r0 is not None:
        x, z = r0
        if cd[1] > 0 and 0 <= cd[3] - z < GATE_NEAR:
            v -= GATE_W * max(0.0, abs(x - cd[2]) - 600)
        if cd[5] != 0 and 0 <= cd[5] - z < GATE_NEAR:
            v -= GATE_W * (max(0.0, x - (cd[4] - 200)) if cd[6] & 1 else max(0.0, (cd[4] + 200) - x))
    return v


def seg_lines(s):
    """one window: (first, last, x, y[, jump + grab[, button held]])"""
    t, e, x, y = s[:4]
    if len(s) > 5 and s[5]:
        return 'F %d-%d STICK %d,%d\nF %d-%d %s\n' % (t, e, x, y, t, e, s[5])
    if len(s) > 4 and s[4]:
        return ('F %d-%d STICK %d,%d\nF %d-%d A\nF %d-%d B\n'
                % (t, e, x, y, t, min(t + 19, e), min(t + 15, e), min(t + 50, e)))
    return 'F %d-%d STICK %d,%d\n' % (t, e, x, y)


def script(segs, tail='', head=None):
    return (head or HEAD) + ''.join(seg_lines(s) for s in segs) + tail


class Runner:
    def __init__(self, build, rom, work):
        self.tenshoe = os.path.join(build, 'tenshoe')
        self.rom, self.work, self.n = rom, work, 0
        self.env = {k: v for k, v in os.environ.items() if not k.startswith('PORT_RDBG')}

    def run(self, text, frames, slot):
        d = os.path.join(self.work, 'c%d' % slot)
        os.makedirs(d, exist_ok=True)
        for f in ('1080.sram', 'state.log'):
            if os.path.exists(os.path.join(d, f)):
                os.remove(os.path.join(d, f))
        with open(os.path.join(d, 's.input'), 'w') as fh:
            fh.write(text)
        subprocess.run(['timeout', '300', RUN32, self.tenshoe, '--rom', self.rom, '--input', 's.input',
                        '--frames', str(frames), '--trace', 'state.log'], cwd=d, env=self.env,
                       stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        return last_state(os.path.join(d, 'state.log'))


CRASH_W = float(os.environ.get('STEER_CRASH_W', 50))   # --mode match: a crash (health lost) in the window
LINE_W = float(os.environ.get('STEER_LINE_W', 0.001))  # --mode match: per unit of x off the CPU rider's line
CPU_PATH = []           # --mode match: the CPU rider's (z, x) along the course, from a full-length run


def cpu_path(path):
    """[(z, x)] of rider 1 (r1=, the Match Race CPU) in a state.log, z increasing"""
    out = []
    for ln in open(path, errors='replace'):
        m = re.search(r' r1=([0-9a-f]{8}),[0-9a-f]{8},([0-9a-f]{8}),', ln)
        if m and ' race=1,' in ln:
            z, x = f32(m.group(2)), f32(m.group(1))
            if not out or z > out[-1][0]:
                out.append((z, x))
    return out


def line_off(x, z):
    """|x - the CPU's x where it was at this z| (0 without a path or past its ends)"""
    import bisect
    i = bisect.bisect_left(CPU_PATH, (z,))
    if i <= 0 or i >= len(CPU_PATH):
        return 0.0
    (z0, x0), (z1, x1) = CPU_PATH[i - 1], CPU_PATH[i]
    return abs(x - (x0 + (x1 - x0) * (z - z0) / max(z1 - z0, 1e-6)))


def score(st, goal, match=False, hp0=None):
    if st is None:
        return None
    step, race, pct, fin, hp = st[:5]
    if goal is not None:
        return 1e9 - goal                      # earliest goal wins
    if hp <= 0 or race == 2:
        return None                            # retired (or the race ended short of the goal)
    if not match:
        return pct + 0.1 * min(hp, 100)
    off = line_off(*st[7]) if st[7] else 0.0
    return (pct + 0.1 * min(hp, 100) + SPEED_W * st[9] - (CRASH_W if hp0 is not None and hp < hp0 else 0.0)
            - LINE_W * max(0.0, off - 500))


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('-b', '--build', required=True, help='PORT_LINK=ON build dir (holds tenshoe)')
    ap.add_argument('-o', '--out', default=os.path.join(GAME, 'port/host/race_crystal_lake.input'))
    ap.add_argument('-r', '--rom', default=os.path.join(GAME, 'baserom.z64'))
    ap.add_argument('-w', '--window', type=int, default=None, help='steps per window (default 60; --mode match 30)')
    ap.add_argument('-j', '--jobs', type=int, default=None, help='parallel runs (default: one per stick state)')
    ap.add_argument('--budget', type=float, default=1200, help='wall seconds (default 1200)')
    ap.add_argument('--last', type=int, default=9000, help='give up past this step (default 9000)')
    ap.add_argument('--keep-prefix', type=int, default=0,
                    help='reuse the STICK windows of the existing --out before this step')
    ap.add_argument('--mode', choices=('time', 'trick', 'match', 'contest'), default='time',
                    help='time: Crystal Lake Time Attack to the goal (default); trick: Trick Attack; match: Match Race; contest: Contest (as trick)')
    ap.add_argument('--tail-only', metavar='SCRIPT',
                    help='no search: take the STICK windows of SCRIPT, rebuild the post-goal tail')
    a = ap.parse_args()
    global HEAD, RACE_FIRST
    if a.mode == 'match':
        if a.out == os.path.join(GAME, 'port/host/race_crystal_lake.input'):
            a.out = os.path.join(GAME, 'port/host/match_race.input')
        HEAD, RACE_FIRST = MATCH_HEAD, MATCH_FIRST
    if a.mode in ('trick', 'contest'):
        if a.out == os.path.join(GAME, 'port/host/race_crystal_lake.input'):
            a.out = os.path.join(GAME, 'port/host/%s.input' % ('trick_attack' if a.mode == 'trick' else 'contest'))
        return trick_main(a, *((TRICK_HEAD, TRICK_FIRST) if a.mode == 'trick' else (CONTEST_HEAD, CONTEST_FIRST)))
    match = a.mode == 'match'
    acts = MATCH_ACTS if match else [(x, y, '') for x, y in ACTS]
    W = a.window or (30 if match else 60)
    work = tempfile.mkdtemp(prefix='steer-search-')   # under $TMPDIR
    R = Runner(a.build, a.rom, work)

    segs, t = [], RACE_FIRST
    if a.keep_prefix and os.path.exists(a.out):
        for m in re.finditer(r'^F (\d+)-(\d+) STICK (-?\d+),(-?\d+)', open(a.out).read(), re.M):
            s = tuple(map(int, m.groups()))
            if s[1] < a.keep_prefix:
                segs.append(s); t = s[1] + 1
    if a.tail_only:
        segs = [tuple(map(int, m.groups())) for m in
                re.finditer(r'^F (\d+)-(\d+) STICK (-?\d+),(-?\d+)', open(a.tail_only).read(), re.M)]
        _, goal = R.run(script(segs), segs[-1][1] + 60, 0)
        if goal is None:
            sys.exit('steer-search: %s does not reach the goal on this build: run the search' % a.tail_only)
        segs[-1] = (segs[-1][0], goal, segs[-1][2], segs[-1][3])
        text, end = build_tail(R, segs, goal)
        open(a.out, 'w').write(text)
        print('steer-search: goal at step %d, END %d -> %s' % (goal, end, a.out))
        shutil.rmtree(work, ignore_errors=True)
        return
    stack = []          # per committed window: the remaining ranked alternatives
    t0, goal, nruns, hp0 = time.time(), None, 0, None
    if match and LINE_W > 0:    # the CPU rider's line: one run of the head with the stick centred
        R.run(script([(t, a.last, 0, 0)]), a.last, 0)
        CPU_PATH[:] = cpu_path(os.path.join(work, 'c0', 'state.log'))
        print('CPU line: %d points' % len(CPU_PATH), flush=True)
    with ThreadPoolExecutor(a.jobs or len(acts)) as ex:
        while time.time() - t0 < a.budget and t < a.last:
            jobs = [(script(segs + [(t, t + 2 * W - 1, x, y, 0, b)]), t + 2 * W, k) for k, (x, y, b) in enumerate(acts)]
            res = list(ex.map(lambda j: R.run(*j), jobs)); nruns += len(jobs)
            ranked = sorted(((score(st, g, match, hp0), act, st, g) for act, (st, g) in zip(acts, res)
                             if score(st, g, match, hp0) is not None), key=lambda r: -r[0])
            while not ranked:   # every candidate retires: back up one window, take its next best
                if not stack:
                    sys.exit('steer-search: no candidate survives the first window at step %d' % t)
                ranked = stack.pop(); segs.pop(); t -= W
                print('  backtrack to step %d (%d alternatives left)' % (t, len(ranked)), flush=True)
            sc, act, st, g = ranked[0]
            segs.append((t, t + W - 1, act[0], act[1], 0, act[2])); stack.append(ranked[1:])
            hp0 = st[4]         # --mode match: a later window that loses health counts as a crash
            print('t=%d stick=%d,%d%s -> step %d race %d pct %.2f health %d speed %.0f%s%s' % (
                t, act[0], act[1], ' ' + act[2] if act[2] else '', st[0], st[1], st[2], st[4], st[9],
                ' cpu %.2f%s' % (st[8][0], ' (finished)' if st[8][1] else '') if st[8] else '',
                '  GOAL at %d' % g if g else ''), flush=True)
            if g is not None and g < t + W:
                goal = g; break
            t += W
    if goal is None:
        sys.exit('steer-search: no goal within the budget (reached step %d; %d runs; scratch %s)' % (t, nruns, work))
    segs[-1] = (segs[-1][0], goal) + tuple(segs[-1][2:])   # trim the last window to the goal
    if a.mode == 'match':
        R.run(script(segs), goal + 300, 0)
        cpu = cpu_finish(os.path.join(work, 'c0', 'state.log'))
        tail = '# GOAL %d   (entries[0].pct reaches 100)\n' % goal
        if cpu is None or goal < cpu:   # port/tools/modes-check.py then requires the win
            tail += '# WIN   (the player finishes first: the CPU rider %s)\n' % (
                'finishes at %d' % cpu if cpu else 'has not finished')
        for i in range(MATCH_TAIL_PRESSES):
            tail += press(goal + 300 + 120 * i, 'A', 'result screens')
        end = goal + 300 + 120 * MATCH_TAIL_PRESSES + 200
        text = script(segs, tail + '# END %d\n' % end)
    else:
        text, end = build_tail(R, segs, goal)
    with open(a.out, 'w') as fh:
        fh.write(text)
    print('steer-search: goal at step %d (race time %d steps), END %d; %d runs, %.0f s -> %s' % (
        goal, goal - RACE_FIRST, end, nruns, time.time() - t0, a.out))
    shutil.rmtree(work, ignore_errors=True)


def chk_misses(path, after):
    """Trick Attack: the "chk" checkpoints passed after step `after` without a countdown extension
    (the trace's cd= gates count drops while the countdown does not jump up: the rider crossed the
    gate's line outside its posts, RaceTracker_passCheckpoints / CheckpointCyl_isBetweenPosts)"""
    misses, prev = 0, None
    for ln in open(path, errors='replace'):
        c = CD.search(ln)
        if not c:
            continue
        cur = (int(ln.split()[1]), int(c.group(1)), int(c.group(2)))
        if prev and cur[0] > after and cur[2] < prev[2] and cur[1] <= prev[1]:
            misses += 1
        prev = cur
    return misses


def cpu_finish(path):
    """the first step entries[1] (the Match Race CPU rider) has finished > 0, or None"""
    for ln in open(path, errors='replace'):
        e = E1.search(ln)
        if e and int(e.group(2)) > 0:
            return int(ln.split()[1])
    return None


def trick_main(a, head, first):
    """--mode trick / contest: windows over TRICK_ACTS, score = PCT_W x pct + points / 20 (+ health)
    + gate_score (the countdown, the next gate's line), until the race ends (race state 2: Time Up
    or the goal).  Trick Attack: a candidate that misses a "chk" checkpoint is out; when every
    candidate of a window is out, back up one window and take its next best (as --mode time)."""
    W = a.window or 60
    work = tempfile.mkdtemp(prefix='steer-search-')
    R = Runner(a.build, a.rom, work)
    trick = head is TRICK_HEAD
    def sc(st):
        if st is None or st[4] <= 0:
            return None
        return PCT_W * st[2] + st[5] / 20.0 + 0.1 * min(st[4], 100) + gate_score(st)
    def run(kc):
        st, g = R.run(script(kc[1], head=head), t + 2 * W, kc[0])
        if trick and st is not None and chk_misses(os.path.join(work, 'c%d' % kc[0], 'state.log'), t) > 0:
            return None, g
        return st, g
    segs, stack, t, t0, nruns, end = [], [], first, time.time(), 0, None
    with ThreadPoolExecutor(min((a.jobs or 5) * 2, len(TRICK_ACTS))) as ex:
        while time.time() - t0 < a.budget and t < a.last:
            cands = [segs + [(t, t + 2 * W - 1, x, y, j)] for (x, y, j) in TRICK_ACTS]
            res = list(ex.map(run, enumerate(cands)))
            nruns += len(cands)
            ranked = sorted(((sc(st), k, st) for k, (st, g) in enumerate(res) if sc(st) is not None),
                            key=lambda r: -r[0])
            while not ranked:
                if not stack:
                    sys.exit('steer-search: every candidate failed at step %d (scratch %s)' % (t, work))
                ranked = stack.pop(); segs.pop(); t -= W
                print('  backtrack to step %d (%d alternatives left)' % (t, len(ranked)), flush=True)
            _, k, st = ranked[0]
            x, y, j = TRICK_ACTS[k]
            segs.append((t, t + W - 1, x, y, j)); stack.append(ranked[1:])
            print('t=%d stick=%d,%d%s -> step %d race %d pct %.2f points %.0f countdown %s' % (
                t, x, y, ' jump+grab' if j else '', st[0], st[1], st[2], st[5],
                st[6] and '%d (%d chk left)' % (st[6][0], st[6][1])), flush=True)
            if st[1] == 2:      # the race ended within the lookahead (Time Up or the goal): its step
                st2, _ = R.run(script(segs, head=head), t + 2 * W + 600, 0)
                end = next((s for s, f, r, _ in flows(os.path.join(work, 'c0', 'state.log'))
                            if s > t and r == 2), None)
                break
            t += W
    if end is None:
        sys.exit('steer-search: the race did not end within the budget (step %d; scratch %s)' % (t, work))
    ext, prev = 0, None     # countdown extensions (the trace's cd= jumps up: a passed "chk" / slalom gate)
    for ln in open(os.path.join(work, 'c0', 'state.log'), errors='replace'):
        c = CD.search(ln) if ' race=1,' in ln else None
        cd = int(c.group(1)) if c else None
        if cd is not None and prev is not None and cd > prev + 500:
            ext += 1
        prev = cd
    tail = '# RACE END %d   (race state 2: Time Up or the goal)\n' % end
    if trick:
        tail += '# CHECKPOINTS %d   (countdown extensions; port/tools/modes-check.py wants at least this many)\n' % ext
    text = script(segs, tail + '# END %d\n' % (end + 300), head=head)
    with open(a.out, 'w') as fh:
        fh.write(text)
    print('steer-search: trick run ends at step %d, %d countdown extensions; %d runs, %.0f s -> %s' % (
        end, ext, nruns, time.time() - t0, a.out))
    shutil.rmtree(work, ignore_errors=True)


def flows(path):
    """[(step, flow, raceState, raceFlags)] of a state.log"""
    out = []
    for ln in open(path, errors='replace'):
        m = re.match(r'F (\d+) \S+ flow=(\d+),\S+ race=(\d+),([0-9a-f]+)', ln)
        if m:
            out.append((int(m.group(1)), int(m.group(2)), int(m.group(3)), m.group(4)))
    return out


def press(step, b, c):
    return 'F %-13s # %s\n' % ('%d:3 %s' % (step, b), c)


def build_tail(R, segs, goal):
    """probe runs: name entry (if the time is a top-3 record) -> TimMenu -> View Replay -> Update
    Ghost -> Yes.  Returns (script text, END step)."""
    log = os.path.join(R.work, 'c0', 'state.log')
    def probe(tail, frames):
        R.run(script(segs, tail), frames, 0)
        return [x for x in flows(log) if x[0] > goal]
    tail = '# GOAL %d   (entries[0].pct reaches 100: "Goal!")\n' % goal
    fl = probe(tail, goal + 900)
    if any(f == 12 for _, f, _, _ in fl):          # a top-3 time: name entry "AAA", END
        for i, c in enumerate(('A', 'A', 'A', 'END', 'END (again: the first press can land before the cursor takes input)')):
            tail += press(goal + NAME_FIRST + i * NAME_STEP, 'A', 'name entry: ' + c)
        fl = probe(tail, goal + 1500)
        n = goal + NAME_FIRST + 4 * NAME_STEP
        best = next((s for s, f, r, fx in fl if s > n and f == 12 and r == 0 and fx == '00000000'), None)
        if best is None:
            sys.exit('steer-search: no Best Times screen after the name entry (scratch %s)' % R.work)
        tail += press(best + BEST_WAIT, 'A', 'Best Times -> TimMenu')
    else:                                           # no record: the results screen
        tail += press(goal + NAME_FIRST, 'A', 'results -> TimMenu')
    fl = probe(tail, goal + 1800)
    menu = next((s for s, f, _, _ in fl if f == 10), None)
    if menu is None:
        sys.exit('steer-search: TimMenu (flow 10) not reached after the goal (scratch %s)' % R.work)
    tail += press(menu + MENU_WAIT, 'DD', 'TimMenu: Retry -> View Replay')
    tail += press(menu + MENU_WAIT + 60, 'A', 'View Replay')
    fl = probe(tail, menu + (goal - RACE_FIRST) + 900)
    rend = replay_end(fl)
    if rend is None:
        sys.exit('steer-search: the View Replay playback did not start / end (scratch %s)' % R.work)
    tail += '# REPLAY END %d\n' % rend
    tail += press(rend + 100, 'DD', 'TimMenu (back from the replay, cursor on Retry) -> View Replay')
    tail += press(rend + 160, 'DD', '-> Update Ghost')
    tail += press(rend + 220, 'A', 'Update Ghost -> "Save ghost data?" (cursor on No)')
    tail += press(rend + 300, 'DL', 'No -> Yes')
    tail += press(rend + 360, 'A', 'save the ghost (SRAM)')
    tail += '# END %d\n' % (rend + GHOST_END)
    return script(segs, tail), rend + GHOST_END


def replay_end(fl):
    """the step the View Replay playback (flow 7, race state 1) ends: the flow leaves 7"""
    inrep = False
    for s, f, r, _ in fl:
        if f == 7 and r == 1:
            inrep = True
        elif inrep and f != 7:
            return s
    return None


if __name__ == '__main__':
    main()
