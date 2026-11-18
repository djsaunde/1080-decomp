#!/usr/bin/env python3
"""PC port, plan 21.12: run the probe build (port/tools/icall/callscan.cc `probe`, port/tools/icall/probe.c) on
every port/host script and some soak seeds; write the distinct (indirect site, target) pairs.

    icall-trace.py -b <probe build> -r <rom> -o <new dir> [--seeds 0-95] [--steps 6000] [-j 16]

Each script runs from a fresh dir to its last step + 900 (at least 3000); the reload scripts get the SRAM
their first part saved (options_reload <- options_save, records_reload <- match_race, the ghost <- the
race), match_race_full also runs on progress_level1.sram, and port/host/repro/*.input run too.  Then
port/tools/soak.py runs the seeds (fresh / saved / unlocked saves, 1-4 pads).  The edges go to
<dir>/edges.txt ("site target" in hex); <dir>/runs.log has one line per run.  The game is deterministic,
so the edge set is the same from run to run of the same tree.
"""
import argparse, concurrent.futures, glob, os, re, shutil, subprocess, sys

HERE = os.path.dirname(os.path.abspath(__file__))
WT = os.path.dirname(os.path.dirname(HERE))
MONO = '/home/danjs/code/decomp'
RUN32 = os.environ.get('RUN32', os.path.join(MONO, 'tools/i386-sysroot/run32'))
H = os.path.join(WT, 'port/host')
ap = argparse.ArgumentParser()
ap.add_argument('-b', '--build', required=True)
ap.add_argument('-r', '--rom', default=os.path.join(WT, 'baserom.z64'))
ap.add_argument('-o', '--out', required=True)
ap.add_argument('--seeds', default='0-95')
ap.add_argument('--steps', default='6000')
ap.add_argument('-j', '--jobs', type=int, default=16)
a = ap.parse_args()
os.makedirs(a.out)
raw = os.path.join(a.out, 'raw')
os.makedirs(raw)


def last_step(p):
    n = 0
    for ln in open(p):
        m = re.match(r'^F\s+(\d+)', ln)
        if m:
            n = max(n, int(m.group(1)))
    return n


def one(name, script, sram=None):
    d = os.path.join(a.out, name)
    os.makedirs(d, exist_ok=True)
    args = [RUN32, os.path.join(a.build, 'tenshoe'), '--rom', a.rom, '--input', script,
            '--frames', str(max(3000, last_step(script) + 900)), '--trace', os.path.join(d, 'state.log')]
    if sram:
        shutil.copy(sram, os.path.join(d, '1080.sram'))
        args += ['--sram', os.path.join(d, '1080.sram')]
    env = dict(os.environ, ICALL_OUT=os.path.join(raw, name))
    with open(os.path.join(d, 'run.log'), 'w') as fh:
        rc = subprocess.run(['timeout', '500'] + args, cwd=d, stdout=fh, stderr=subprocess.STDOUT, env=env).returncode
    return '%s rc %d' % (name, rc)


first = [(os.path.basename(p)[:-6], p, None) for p in sorted(glob.glob(os.path.join(H, '*.input')))
         if not re.search(r'(_reload|_ghost)\.input$', p)]
first.append(('progress1', os.path.join(H, 'match_race_full.input'), os.path.join(H, 'progress_level1.sram')))
first += [('repro_' + os.path.basename(p)[:-6], p, None) for p in sorted(glob.glob(os.path.join(H, 'repro/*.input')))]
second = [('options_reload', os.path.join(H, 'options_reload.input'), os.path.join(a.out, 'options_save/1080.sram')),
          ('records_reload', os.path.join(H, 'records_reload.input'), os.path.join(a.out, 'match_race/1080.sram')),
          ('ghost', os.path.join(H, 'race_crystal_lake_ghost.input'), os.path.join(a.out, 'race_crystal_lake/1080.sram'))]
log = []
with concurrent.futures.ThreadPoolExecutor(a.jobs) as ex:
    log += list(ex.map(lambda t: one(*t), first))
    log += list(ex.map(lambda t: one(*t) if os.path.exists(t[2]) else '%s: no %s' % (t[0], t[2]), second))
if a.seeds:
    env = dict(os.environ, ICALL_OUT=os.path.join(raw, 'soak'))
    r = subprocess.run([sys.executable, os.path.join(HERE, 'soak.py'), '-b', a.build, '-r', a.rom, '-o',
                        os.path.join(a.out, 'soak'), '--seeds', a.seeds, '--steps', a.steps, '-j', str(a.jobs)],
                       env=env, capture_output=True, text=True)
    log.append('soak %s rc %d: %s' % (a.seeds, r.returncode, (r.stdout.splitlines() or [''])[0][:150]))
edges = set()
for f in os.listdir(raw):
    for ln in open(os.path.join(raw, f)):
        if ln.strip():
            edges.add(ln.strip())
open(os.path.join(a.out, 'edges.txt'), 'w').write(''.join(e + '\n' for e in sorted(edges)))
open(os.path.join(a.out, 'runs.log'), 'w').write('\n'.join(log) + '\n')
bad = [l for l in log if ' rc 0' not in l and not l.startswith('soak')]
print('icall-trace: %d runs (%d not rc 0%s), %d edges, %d sites' % (
    len(log), len(bad), (': ' + ', '.join(bad[:4])) if bad else '', len(edges), len({e.split()[0] for e in edges})))
