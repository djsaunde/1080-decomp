#!/usr/bin/env python3
"""Soak / fuzz harness for the headless port (plan 20.7, run 864): random but plausible pad input,
seeded and reproducible, run in parallel; failures are classified, minimised and saved as repros.

    port/tools/soak.py -b <port build> -o <new scratch dir> [--seeds 0-199] [--steps 6000] [-j 8]
                       [--saved-frac 0.3] [--minimise] [--save-repro]      # a soak campaign
    port/tools/soak.py -b <port build> -o <dir> --gate                     # the port gate's soak step
    port/tools/soak.py --script <seed> [--steps N]                         # print a seed's script
    port/tools/soak.py -b <port build> -o <dir> --repro port/host/repro/X.input   # re-run one repro

Input (gen_script): seed -> an input_script.c script.  Title START (+ the language A on a blank
save), then sessions: main-menu navigation (D-pad up / down, A) and selection presses (mostly A, some
D-pad, B back, START), or (40 %) a directed walk: a mode, then D-pad steps on the rider, board and
course screens; a ride (stick segments, hop + grab / spin tricks, hands-off stretches, pauses resolved
with resume / retire / restart / quit and a confirm box); result presses; idle stretches (attract
demo).  15 % of the seeds mash every button instead; 22 % plug in two pads, 5 % four (P2.. press and
ride too).  The seed also picks the save (sram_of): `--saved-frac` boot on an all-unlocked SRAM
(options unlocks 0x00FF, seqStep 5, resealed, on the save race_crystal_lake.input leaves: a record
and a Crystal Lake ghost), 0.6 x that on a plain saved one (match_race.input's: language, a Match
Race record); both are made once per invocation; the rest boot blank.
Coverage lines: racing share, runs per flow mode, the distinct (flow mode, race start position)
pairs (= courses), the music command words heard.

Failure classes (one key per distinct cause; `classify`):
  signal:<sig>:<top frames>   host crash (port/main.c port_crash_signal: backtrace -> addr2line)
  fatal:<message>             sys_fatal / host_fatal / DEADLOCK / PORT TRAP
  hang                        the run timed out (no exit within the wall-clock budget)
  short                       exited 0 before its step count (state.log is missing frames)
  nan-rider / nan-entry       NaN / Inf in r<N>= (pos, quat, speed) or e0= pct / speed
  nan-camera                  NaN / Inf in cam= (reported; soft unless --strict-camera)
  stuck-retrace               the game's retrace count did not advance for 600 steps
  audio-bad / audio-unknown   the audio mixer's badAddr / unknownOps counters (--audio-out)
  frozen (soft)               race running, elapsed advancing, rider 0 position bit-identical 1500 steps

Minimising (--minimise): the script is cut at the failing step, then F lines are removed by ddmin
(parallel complement tests) while the same class key still reproduces.  --save-repro writes each
class's smallest script to port/host/repro/<class>.input with `# SOAK` / `# END` headers; the gate
re-runs every repro (and a few fixed seeds) and expects them clean.
Exit status: the number of distinct hard failure classes.
"""
import argparse, glob, os, random, re, shutil, struct, subprocess, sys
from concurrent.futures import ThreadPoolExecutor

HERE = os.path.dirname(os.path.abspath(__file__))
GAME = os.path.dirname(os.path.dirname(HERE))
MONO = os.path.dirname(os.path.dirname(GAME))
RUN32 = os.environ.get('RUN32', os.path.join(MONO, 'tools/i386-sysroot/run32'))
HOST = os.path.join(GAME, 'port/host')
REPRO = os.path.join(HOST, 'repro')
GATE_SEEDS = list(range(1, 13))
GATE_STEPS = 5000
SOFT = ('frozen', 'nan-camera')

# ---------------------------------------------------------------- input generation


class Script:
    def __init__(self):
        self.ev = []  # (first, last, pad, tokens)

    def add(self, first, length, toks, pad=1):
        self.ev.append((first, first + max(1, length) - 1, pad, toks))

    def text(self, header=()):
        npads = max([e[2] for e in self.ev] + [1])
        out = ['# ' + h for h in header] + ['PADS %d' % npads]
        for f, l, p, toks in sorted(self.ev, key=lambda e: (e[0], e[2])):
            out.append('F %d-%d P%d %s' % (f, l, p, toks))
        return '\n'.join(out) + '\n'


def stick(R, forward=True):
    x = R.choice([0, 0, R.randint(-80, 80), R.randint(-30, 30), R.choice([-80, 80])])
    y = R.choice([0, 80, R.randint(0, 80)] if forward else [R.randint(-80, 80)])
    return 'STICK %d,%d' % (x, y)


def press(s, t, btn, pad=1, hold=3):
    s.add(t, hold, btn, pad)


def gen_menu(R, s, t, pads):
    """main menu choice + the selection screens; returns the next step"""
    for _ in range(R.choice([0, 0, 1, 2, 3, 4, 5, 6])):
        press(s, t, R.choice(['DD', 'DD', 'DU']))
        t += R.randint(40, 90)
    press(s, t, 'A')
    t += R.randint(150, 300)
    for _ in range(R.randint(3, 9)):
        if R.random() < 0.45:  # walk the screen's choices first (level, rider, board, course, round)
            d = R.choice(['DL', 'DR', 'DU', 'DD', 'DR', 'DD'])
            for _ in range(R.randint(1, 6)):
                press(s, t, d if R.random() < 0.8 else R.choice(['DL', 'DR', 'DU', 'DD']))
                t += R.randint(25, 60)
        r = R.random()
        btn = 'A' if r < 0.72 else R.choice(['DL', 'DR', 'DU', 'DD']) if r < 0.9 else 'B' if r < 0.97 else 'START'
        press(s, t, btn)
        if pads == 2 and R.random() < 0.7:
            press(s, t + R.randint(0, 30), 'A' if R.random() < 0.85 else R.choice(['DL', 'DR', 'B']), pad=2)
        t += R.randint(50, 140)
    return t


def gen_directed(R, s, t, pads):
    """the selection screens walked on purpose (time_attack.input's timing): a mode, then rider,
    board and course picked by D-pad steps; returns the next step"""
    d, n = R.choice([('DD', 0), ('DD', 1), ('DD', 2), ('DD', 3), ('DD', 4), ('DU', 2), ('DD', 1), ('DD', 2)])
    for _ in range(n):
        press(s, t, d)
        t += R.randint(55, 70)
    press(s, t, 'A')
    t += R.randint(260, 300)
    for screen in range(R.choice([4, 5, 5, 6])):  # (level,) rider, confirm, board, (confirm,) course
        for _ in range(R.choice([0, 0, 1, 2, 3, 4, 5, 6, 7])):
            press(s, t, R.choice(['DR', 'DR', 'DD', 'DL']))
            t += R.randint(40, 60)
        press(s, t, 'A')
        if pads >= 2:
            for _ in range(R.choice([0, 1, 2])):
                press(s, t - 30, R.choice(['DR', 'DD']), pad=2)
            press(s, t + R.randint(0, 20), 'A', pad=2)
        t += R.randint(100, 150)
    return t


def gen_ride(R, s, t, pads, length):
    end = t + length
    while t < end:
        for pad in range(1, pads + 1):
            if pad > 1 and R.random() < 0.3:
                continue
            seg = R.randint(10, 90)
            s.add(t, seg, stick(R), pad)
            r = R.random()
            if r < 0.25:  # hop + grab / spin: A crouch, release, then a trick button with a direction
                hop = R.randint(8, 25)
                s.add(t, hop, 'A', pad)
                s.add(t + hop + R.randint(0, 8), R.randint(10, 40),
                      R.choice(['B', 'B', 'Z', 'R', 'L', 'CU', 'CD', 'CL', 'CR', 'B Z', 'B R']), pad)
                s.add(t + hop, R.randint(10, 30), stick(R, forward=False), pad)
            elif r < 0.30:
                s.add(t, R.randint(5, 40), R.choice(['Z', 'R', 'B', 'A B']), pad)
        t += R.randint(10, 90)
        r = R.random()
        if r < 0.025:  # pause menu: resume, or walk to retire / restart / quit
            press(s, t, 'START')
            t += R.randint(40, 120)
            if R.random() < 0.4:
                press(s, t, 'START')
            else:
                for _ in range(R.randint(0, 4)):
                    press(s, t, R.choice(['DD', 'DD', 'DU']))
                    t += R.randint(30, 60)
                press(s, t, R.choice(['A', 'A', 'B']))
                if R.random() < 0.5:  # a confirm box
                    t += R.randint(40, 80)
                    if R.random() < 0.5:
                        press(s, t, 'DU' if R.random() < 0.5 else 'DD')
                        t += R.randint(30, 50)
                    press(s, t, 'A')
            t += R.randint(30, 80)
        elif r < 0.04:
            t += R.randint(60, 600)  # hands off mid-race
    return t


def gen_script(seed, steps, sram='fresh'):
    R = random.Random(seed)
    s = Script()
    r = R.random()
    pads = 4 if r < 0.05 else 2 if r < 0.27 else 1  # four pads plugged in: the game uses two
    t = R.randint(470, 530)
    press(s, t, 'START')
    t += R.randint(70, 110)
    if sram == 'fresh':  # the language menu (a saved SRAM skips it)
        press(s, t, 'A')
        t += R.randint(50, 90)
    mash = R.random() < 0.15  # button mashing: every button and direction, short gaps
    while mash and t < steps:
        for pad in range(1, pads + 1):
            s.add(t, R.randint(1, 30), R.choice(['A', 'B', 'Z', 'START', 'L', 'R', 'DU', 'DD', 'DL', 'DR', 'CU', 'CD',
                                                 'CL', 'CR', 'A', 'A', 'DD', 'DU']), pad)
            if R.random() < 0.5:
                s.add(t, R.randint(5, 60), stick(R, forward=False), pad)
        t += R.randint(4, 40)
    while t < steps:
        r = R.random()
        if r < 0.08:
            t += R.randint(100, 1500)  # idle (attract demo, menu timeouts)
            continue
        t = gen_directed(R, s, t, pads) if R.random() < 0.4 else gen_menu(R, s, t, pads)
        t = gen_ride(R, s, t, pads, R.randint(300, 2800))
        for _ in range(R.randint(2, 7)):  # results, name entry, replay menus, next round
            press(s, t, 'A' if R.random() < 0.8 else R.choice(['B', 'DD', 'DU', 'START']))
            if pads == 2 and R.random() < 0.5:
                press(s, t + 5, 'A', pad=2)
            t += R.randint(60, 200)
    s.ev = [e for e in s.ev if e[0] < steps]
    return s, pads

# ---------------------------------------------------------------- running + classifying


def f32bad(w):
    return len(w) == 8 and (int(w, 16) >> 23) & 0xFF == 0xFF


def addr2line(exe, addrs, cache={}):
    out = []
    todo = [a for a in addrs if a not in cache]
    if todo:
        try:
            r = subprocess.run(['addr2line', '-f', '-e', exe] + ['%x' % (a - 1) for a in todo],
                               capture_output=True, text=True, timeout=60).stdout.splitlines()
            for i, a in enumerate(todo):
                cache[a] = (r[2 * i], os.path.basename(r[2 * i + 1]))
        except (OSError, IndexError, subprocess.SubprocessError):
            for a in todo:
                cache[a] = ('?', '?')
    for a in addrs:
        out.append(cache[a])
    return out


SKIP_FN = ('port_crash_signal', 'port_sys_fatal', 'port_trap', '__kernel', 'host_fatal', '??', '_IO_', 'raise',
           'abort', 'backtrace')


def run(a, d, script_text, steps, sram_src=None, keep_trace=True):
    """run tenshoe in a new dir d on script_text; returns a result dict"""
    os.makedirs(d, exist_ok=True)
    sp = os.path.join(d, 'in.input')
    open(sp, 'w').write(script_text)
    sram = os.path.join(d, '1080.sram')
    if sram_src:
        shutil.copyfile(sram_src, sram)
    args = ['timeout', str(a.timeout), RUN32, os.path.join(a.build, 'tenshoe'), '--rom', a.rom, '--input', sp,
            '--frames', str(steps), '--trace', os.path.join(d, 'state.log'), '--sram', sram,
            '--audio-out', '/dev/null']
    with open(os.path.join(d, 'out.log'), 'w') as fh:
        rc = subprocess.run(args, cwd=d, stdout=fh, stderr=subprocess.STDOUT).returncode
    res = analyse(a, d, rc, steps)
    if not keep_trace and not res['hard']:
        shutil.rmtree(d, ignore_errors=True)
    return res


def analyse(a, d, rc, steps):
    exe = os.path.join(a.build, 'tenshoe')
    out = open(os.path.join(d, 'out.log'), errors='replace').read()
    classes = {}  # key -> (step, detail)
    m = re.search(r'PORT: signal (\d+) at step (\d+)', out)
    if m:
        addrs = [int(x, 16) for x in re.findall(r'\[0x([0-9a-f]+)\]', out[m.end():])]
        fns = [f for f in addr2line(exe, addrs) if not any(f[0].startswith(k) for k in SKIP_FN)]
        top = '>'.join(f[0] for f in fns[:2]) or 'unknown'
        classes['signal:%s:%s' % (m.group(1), top)] = (int(m.group(2)), ' < '.join('%s (%s)' % f for f in fns[:8]))
    for pat, k in ((r'PORT: sys_fatal: (.*)', 'fatal'), (r'PORT TRAP: (\S+)', 'trap'),
                   (r'DEADLOCK: ([^:]*)', 'deadlock')):
        m = re.search(pat, out)
        if m:
            msg = re.sub(r'0x[0-9a-f]+|\d+', 'N', m.group(1))[:60].strip()
            classes['%s:%s' % (k, msg)] = (None, m.group(0)[:200])
    if rc == 124:
        classes['hang'] = (None, 'timeout %ds' % a.timeout)
    m = re.search(r'mixer \d+ tasks, badAddr (\d+), unknownOps (\d+)', out)
    if m and int(m.group(1)):
        classes['audio-bad'] = (None, m.group(0))
    if m and int(m.group(2)):
        classes['audio-unknown'] = (None, m.group(0))
    last, rt_prev, rt_since = -1, None, 0
    froz_key, froz_since = None, 0
    modes, racing, starts, was_racing = set(), 0, set(), False
    try:
        fh = open(os.path.join(d, 'state.log'), errors='replace')
    except OSError:
        fh = []
    for ln in fh:
        if not ln.startswith('F '):
            continue
        p = ln.split(' ', 3)
        step = int(p[1])
        last = step
        mm = re.match(r'F \d+ \S+ flow=-?\d+,-?\d+,(-?\d+) race=(-?\d+)', ln)
        if mm:
            modes.add(int(mm.group(1)))
            racing += mm.group(2) == '1'
            if mm.group(2) == '1' and not was_racing:  # the start position names the course
                st = re.search(r' r0=([0-9a-f]+),[0-9a-f]+,([0-9a-f]+),', ln)
                if st:
                    starts.add((int(mm.group(1)), st.group(1)[:3], st.group(2)[:3]))
            was_racing = mm.group(2) == '1'
        for mm in re.finditer(r' (cam|r\d+|e0)=([0-9a-f,]+)', ln):
            ws = mm.group(2).split(',')
            if mm.group(1) == 'e0':
                ws = ws[:2]
            if any(f32bad(w) for w in ws):
                k = 'nan-camera' if mm.group(1) == 'cam' else 'nan-entry' if mm.group(1) == 'e0' else 'nan-rider'
                if k not in classes:
                    classes[k] = (step, '%s=%s' % (mm.group(1), mm.group(2)[:80]))
        mm = re.search(r' rt=(\d+)', ln)
        if mm:
            rt = int(mm.group(1))
            if rt == rt_prev:
                rt_since += 1
                if rt_since == 600 and 'stuck-retrace' not in classes:
                    classes['stuck-retrace'] = (step, 'rt=%d since step %d' % (rt, step - 600))
            else:
                rt_since = 0
            rt_prev = rt
        mm = re.search(r' race=1,\S* .* el=(-?\d+) .* r0=([0-9a-f]+,[0-9a-f]+,[0-9a-f]+),', ln)
        if mm:
            if froz_key and froz_key[1] == mm.group(2) and froz_key[0] != mm.group(1):
                froz_since += 1
                if froz_since == 1500 and 'frozen' not in classes:
                    classes['frozen'] = (step, 'r0 still at %s' % mm.group(2))
            else:
                froz_since = 0 if not froz_key or froz_key[1] != mm.group(2) else froz_since
            froz_key = (mm.group(1), mm.group(2))
        else:
            froz_key, froz_since = None, 0
    if rc == 0 and last < steps - 1 and not classes:
        classes['short'] = (last, 'state.log ends at %d of %d' % (last, steps))
    if rc not in (0, 124) and not classes:
        classes['exit:%d' % rc] = (last, out.strip().splitlines()[-1][:200] if out.strip() else '')
    hard = {k: v for k, v in classes.items() if not k.startswith(SOFT) or (a.strict_camera and k == 'nan-camera')}
    music = {int(x) for x in re.findall(r'music mode \d+, command word (-?\d+)', out)}
    return {'dir': d, 'rc': rc, 'last': last, 'classes': classes, 'hard': hard, 'modes': modes, 'racing': racing,
            'music': music, 'starts': starts}

# ---------------------------------------------------------------- minimising


def fail_step(res, key):
    st = res['classes'][key][0]
    return st if st is not None else res['last']


def minimise(a, base_dir, key, ev, steps, sram, header):
    """ddmin over the F lines; returns (events, steps) still reproducing key"""
    cnt = [0]

    def test(evs, n):
        cnt[0] += 1
        s = Script()
        s.ev = evs
        r = run(a, os.path.join(base_dir, 't%05d' % cnt[0]), s.text(), n, sram, keep_trace=False)
        return key in r['classes'], r

    ok, r = test(ev, steps)
    if not ok:
        return None
    n = min(steps, fail_step(r, key) + 60) if key not in ('hang',) else steps
    ev = [e for e in ev if e[0] <= n]
    ok, r = test(ev, n)
    if not ok:
        n, ev = steps, ev  # keep the full length
    chunks = 2
    while len(ev) >= 1 and chunks <= max(2, len(ev)):
        size = max(1, len(ev) // chunks)
        parts = [ev[i:i + size] for i in range(0, len(ev), size)]
        cands = [[e for j, p in enumerate(parts) if j != i for e in p] for i in range(len(parts))]
        with ThreadPoolExecutor(a.jobs) as ex:
            got = list(ex.map(lambda c: test(c, n), cands))
        hit = next((i for i, g in enumerate(got) if g[0]), None)
        if hit is not None:
            ev = cands[hit]
            r = got[hit][1]
            n2 = min(n, fail_step(r, key) + 60) if key != 'hang' else n
            if n2 < n and test([e for e in ev if e[0] <= n2], n2)[0]:
                n = n2
                ev = [e for e in ev if e[0] <= n]
            chunks = max(chunks - 1, 2)
        elif size == 1:
            break
        else:
            chunks = min(len(ev), chunks * 2)
    return ev, n

# ---------------------------------------------------------------- driver


def sram_of(sd, frac):
    """the seed's save: unlocked (frac), saved (0.6 frac) or fresh"""
    r = random.Random(sd * 7919 + 1).random()
    return 'unlocked' if r < frac else 'saved' if r < frac * 1.6 else 'fresh'


def parse_seeds(t):
    out = []
    for part in t.split(','):
        if '-' in part:
            x, y = part.split('-')
            out += range(int(x), int(y) + 1)
        elif part:
            out.append(int(part))
    return out


def make_saved(a, name='base-save', script='match_race.input'):
    d = os.path.join(a.outdir, name)
    text = open(os.path.join(HOST, script)).read()
    m = re.search(r'^# END (\d+)', text, re.M)
    r = run(a, d, text, int(m.group(1)) if m else 6000)
    p = os.path.join(d, '1080.sram')
    if not os.path.exists(p) or r['hard']:
        print('!! base save run %s failed: %s' % (script, r['hard']))
        return None
    return p


def make_unlocked(a, base):
    """the base save with every course / mode open: options unlocks 0x00FF, seqStep 5 (plan 19.x, run
    851's unlock.sram), chunk sums, mirrors and the header hash resealed (sram-slow-records.py)"""
    import importlib.util
    spec = importlib.util.spec_from_file_location('ssr', os.path.join(HERE, 'sram-slow-records.py'))
    ssr = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(ssr)
    img = bytearray(open(base, 'rb').read())
    img[0x1F8:0x1FC] = struct.pack('>HH', 0x00FF, 5)
    hdr = b''
    for off, sm, mir, mirs, size in ssr.CHUNKS:
        s8 = ssr.checksum(img[off:off + size], ssr.CHUNK_KEY)
        img[sm:sm + 8] = s8
        img[mir:mir + size] = img[off:off + size]
        img[mirs:mirs + 8] = s8
        hdr += s8
    img[0:8] = ssr.checksum(hdr, ssr.HEADER_KEY)
    p = os.path.join(os.path.dirname(base), 'unlocked.sram')
    open(p, 'wb').write(img)
    return p


def read_repro(path):
    text = open(path).read()
    m = re.search(r'^# END (\d+)', text, re.M)
    sv = re.search(r'^# SOAK .*sram=(\w+)', text, re.M)
    return text, int(m.group(1)) if m else 6000, sv.group(1) if sv else 'fresh'


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('-b', '--build')
    ap.add_argument('-o', '--outdir')
    ap.add_argument('-r', '--rom', default=os.path.join(GAME, 'baserom.z64'))
    ap.add_argument('-j', '--jobs', type=int, default=8)
    ap.add_argument('--seeds', default='0-49')
    ap.add_argument('--steps', type=int, default=6000)
    ap.add_argument('--saved-frac', type=float, default=0.3, help='unlocked-save fraction (a saved one: 0.6 x)')
    ap.add_argument('--timeout', type=int, default=120)
    ap.add_argument('--minimise', action='store_true')
    ap.add_argument('--save-repro', action='store_true')
    ap.add_argument('--strict-camera', action='store_true')
    ap.add_argument('--keep', action='store_true', help='keep every run directory (default: failures only)')
    ap.add_argument('--gate', action='store_true', help='fixed seeds + every port/host/repro script')
    ap.add_argument('--repro', nargs='*', help='re-run these repro scripts')
    ap.add_argument('--script', type=int, help='print the script of this seed and exit')
    a = ap.parse_args()
    if a.script is not None:
        sv = sram_of(a.script, a.saved_frac)
        s, _ = gen_script(a.script, a.steps, sv)
        sys.stdout.write(s.text(['soak.py seed %d steps %d sram=%s' % (a.script, a.steps, sv)]))
        return 0
    if not a.build or not a.outdir:
        ap.error('-b and -o are needed')
    os.makedirs(a.outdir, exist_ok=True)
    saved = ghost = None
    if a.saved_frac > 0 or a.gate or a.repro is not None:
        with ThreadPoolExecutor(2) as ex:  # a won Match Race round (record "AAA"); a Crystal Lake ghost
            saved, ghost = ex.map(lambda x: make_saved(a, *x), [('base-save', 'match_race.input'),
                                                                ('base-ghost', 'race_crystal_lake.input')])
    srams = {'fresh': None, 'saved': saved, 'unlocked': make_unlocked(a, ghost or saved) if (ghost or saved) else None}

    jobs = []  # (name, text, steps, sram, seed)
    if a.gate or a.repro is not None:
        paths = a.repro if a.repro else sorted(glob.glob(os.path.join(REPRO, '*.input')))
        for p in paths:
            text, n, sv = read_repro(p)
            jobs.append(('repro-' + os.path.basename(p)[:-6], text, n, sv, None))
    if a.gate:
        for i, sd in enumerate(GATE_SEEDS):
            sv = ('fresh', 'unlocked', 'saved')[i % 3]
            s, _ = gen_script(sd, GATE_STEPS, sv)
            jobs.append(('seed%d' % sd, s.text(), GATE_STEPS, sv, sd))
    elif a.repro is None:
        for sd in parse_seeds(a.seeds):
            sv = sram_of(sd, a.saved_frac)
            s, _ = gen_script(sd, a.steps, sv)
            jobs.append(('seed%d' % sd, s.text(), a.steps, sv, sd))

    def one(j):
        return j, run(a, os.path.join(a.outdir, j[0]), j[1], j[2], srams[j[3]], keep_trace=a.keep)

    with ThreadPoolExecutor(a.jobs) as ex:
        results = list(ex.map(one, jobs))
    byclass = {}
    soft = {}
    for j, r in results:
        for k, v in r['classes'].items():
            (byclass if k in r['hard'] else soft).setdefault(k, []).append((j, r, v))
    print('soak: %d runs x %s steps, %d with a saved SRAM; %d hard failure class(es), %d soft' % (
        len(results), '/'.join(sorted({str(j[2]) for j, _ in results})), sum(1 for j, _ in results if j[3] != 'fresh'),
        len(byclass), len(soft)))
    allm = {}
    for j, r in results:
        for md in r['modes']:
            allm[md] = allm.get(md, 0) + 1
    print('  coverage: %d%% of the steps racing; runs per flow mode %s' % (
        100 * sum(r['racing'] for _, r in results) // max(1, sum(j[2] for j, _ in results)),
        ' '.join('%d:%d' % kv for kv in sorted(allm.items()))))
    allc = {}
    for j, r in results:
        for c in r['music']:
            allc[c] = allc.get(c, 0) + 1
    alls = set().union(*[r['starts'] for _, r in results]) if results else set()
    print('  coverage: %d distinct (flow mode, race start position) pairs: %s' % (
        len(alls), ' '.join(sorted('%d@%s,%s' % x for x in alls))[:400]))
    print('  coverage: runs per music command word (course / menu tracks) %s' % ' '.join('%d:%d' % kv for kv in sorted(allc.items())))
    for tag, d in (('FAIL', byclass), ('soft', soft)):
        for k in sorted(d):
            runs = d[k]
            print('  %s %-50s %3d run(s): %s' % (tag, k, len(runs), ' '.join(j[0] for j, _, _ in runs[:8])))
            j, r, v = runs[0]
            print('       first: %s step %s: %s' % (j[0], v[0], v[1][:300]))
    if a.minimise:
        for k in sorted(byclass):
            j, r, v = min(byclass[k], key=lambda x: (x[2][0] or x[1]['last']))
            if j[4] is None:
                continue
            s, _ = gen_script(j[4], j[2], j[3])
            n0 = len(s.ev)
            got = minimise(a, os.path.join(a.outdir, 'min-' + re.sub(r'\W+', '_', k)[:60]), k, s.ev, j[2],
                           srams[j[3]], None)
            if got is None:
                print('  min %s: did not reproduce' % k)
                continue
            ev, n = got
            s.ev = ev
            name = re.sub(r'\W+', '_', k).strip('_')[:60]
            hdr = ['soak.py repro (run 864): class %s' % k,
                   'SOAK seed=%d steps=%d sram=%s' % (j[4], j[2], j[3]),
                   'minimised from %d to %d F lines; detail: %s' % (n0, len(ev),
                                                                   v[1][:150].replace('\n', ' ')),
                   'END %d' % n]
            text = s.text(hdr)
            dst = os.path.join(REPRO if a.save_repro else a.outdir, name + '.input')
            os.makedirs(os.path.dirname(dst), exist_ok=True)
            open(dst, 'w').write(text)
            print('  min %s: %d F lines, %d steps -> %s' % (k, len(ev), n, dst))
    return len(byclass)


if __name__ == '__main__':
    sys.exit(main())
