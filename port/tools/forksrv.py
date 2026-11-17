#!/usr/bin/env python3
"""Client for tenshoe's fork server (port/main.c --fork-at / --fork-dir; run 871).

A Server is a tenshoe process parked at one step.  `run` forks a child that swaps in a new input
script (same step count), writes its state trace from the fork step on and exits at UNTIL; `spawn`
forks a child that becomes a new Server at a later step.  The port is deterministic, so a child
continues exactly as a run from boot with the same script would, at the cost of the steps after
the fork only (port/tools/steer-search.py's beam search: --mode rounds).

    srv = Server.boot(tenshoe, rom, script_path, step, workdir, runner=[run32])
    outs = srv.run_many([(script_path, until, outdir), ...])     # parallel children; waits
    child = srv.spawn(script_path, until, newdir)                # a Server at `until`
    srv.quit()
"""
import os, subprocess, threading


class Server:
    def __init__(self, d, step, proc=None):
        self.d, self.step, self.proc = d, step, proc
        self.lock = threading.Lock()
        self.cmd = open(os.path.join(d, 'cmd'), 'w', buffering=1)   # blocks until the server opens it
        self.ack = open(os.path.join(d, 'ack'), 'r')

    @staticmethod
    def fifos(d):
        os.makedirs(d, exist_ok=True)
        for f in ('cmd', 'ack'):
            p = os.path.join(d, f)
            if os.path.exists(p):
                os.remove(p)
            os.mkfifo(p)

    @classmethod
    def boot(cls, tenshoe, rom, script, step, d, runner=(), env=None, log=None):
        """a new tenshoe process running `script` from boot, parked at `step`"""
        cls.fifos(d)
        out = open(log or os.path.join(d, 'server.log'), 'w')
        proc = subprocess.Popen(list(runner) + [tenshoe, '--rom', rom, '--input', script, '--fork-at', str(step),
                                                '--fork-dir', d], cwd=d, env=env, stdout=out,
                                stderr=subprocess.STDOUT)
        return cls(d, step, proc)

    def _ask(self, line):
        self.cmd.write(line + '\n')
        r = self.ack.readline()
        if not r:
            raise RuntimeError('fork server %s went away' % self.d)
        return r.split()

    def run_many(self, jobs):
        """jobs: [(script path, until, out dir)]; forks them all, then waits for every one"""
        with self.lock:
            for script, until, out in jobs:
                os.makedirs(out, exist_ok=True)
                for f in ('state.log', '1080.sram'):
                    if os.path.exists(os.path.join(out, f)):
                        os.remove(os.path.join(out, f))
                r = self._ask('run %s %d %s' % (os.path.abspath(script), until, os.path.abspath(out)))
                if r[0] != 'pid' or int(r[1]) <= 0:
                    raise RuntimeError('fork server: %s' % ' '.join(r))
            self._ask('wait')

    def spawn(self, script, until, d):
        """a new Server: the child runs `script` on to step `until` and parks there"""
        self.fifos(d)
        with self.lock:
            r = self._ask('spawn %s %d %s' % (os.path.abspath(script), until, os.path.abspath(d)))
        if r[0] != 'pid' or int(r[1]) <= 0:
            raise RuntimeError('fork server: %s' % ' '.join(r))
        return Server(d, until)

    def quit(self):
        try:
            self.cmd.write('quit\n')
            self.cmd.close()
        except OSError:
            pass
        try:
            self.ack.close()
        except OSError:
            pass
        if self.proc is not None:
            self.proc.wait()
