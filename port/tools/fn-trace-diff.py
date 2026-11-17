#!/usr/bin/env python3
"""Compare two PORT_TRACE_FN logs (plan 21.8): the i386 and the x86-64 build of one frame.

    fn-trace-diff.py <binA> <fn.logA> <binB> <fn.logB> [-n CONTEXT]

Each log line `E <addr> <stack depth> <hash> | <raw args>` is one function entry (the game library is built
-pg -mfentry, port/trace.c) with the hash of the traced object (rider 0's SnowBoarder) at that
moment; `B` / `D` lines are the object's words at the start / end of the frame.  The addresses are
mapped to names with nm.  Reports the first entry where the call sequence or the hash differs: the
object changed differently in the function(s) that ran since the previous entry, which the report
lists with their callers (the stack depth gives the nesting).  Then the words that differ in D.
"""
import argparse
import subprocess
import sys


def syms(binary):
    out = subprocess.run(["nm", binary], capture_output=True, text=True).stdout
    m = {}
    for ln in out.splitlines():
        p = ln.split()
        if len(p) == 3 and p[1] in "tTwW":
            m.setdefault(int(p[0], 16), p[2])
    return m


def load(binary, log):
    s = syms(binary)
    ev, words = [], {}
    for ln in open(log, errors="replace"):
        p = ln.split()
        if not p:
            continue
        if p[0] == "E" and len(p) >= 4:
            a = int(p[1], 16)
            ev.append((s.get(a, hex(a)), int(p[2]), p[3], " ".join(p[4:])))
        elif p[0] in "BD":
            words[p[0]] = p[1:]
    return ev, words


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("binA"); ap.add_argument("logA"); ap.add_argument("binB"); ap.add_argument("logB")
    ap.add_argument("-n", type=int, default=12)
    ap.add_argument("--args", action="store_true", help="print each entry's raw arguments (i386: the first "
                    "6 stack words; x86-64: rdi..r9 | xmm0-3)")
    a = ap.parse_args()
    ea, wa = load(a.binA, a.logA)
    eb, wb = load(a.binB, a.logB)
    print("events: %d / %d" % (len(ea), len(eb)))
    first = None
    for i, (x, y) in enumerate(zip(ea, eb)):
        if x[0] != y[0] or x[2] != y[2]:
            first = i
            break
    if first is None:
        print("no divergence in %d common events" % min(len(ea), len(eb)))
    else:
        kind = "call sequence" if ea[first][0] != eb[first][0] else "object hash"
        print("first divergence (%s) at event %d" % (kind, first))
        for i in range(max(0, first - a.n), min(first + 3, len(ea), len(eb))):
            x, y = ea[i], eb[i]
            mark = ">>" if i == first else "  "
            print("%s %6d %s%-44s %s | %s%s" % (mark, i, " " * min(x[1] // 32, 30), x[0], x[2],
                  y[2], "" if x[0] == y[0] else "  B: " + y[0]))
            if a.args:
                print("             A args %s\n             B args %s" % (x[3], y[3]))
    for k in "BD":
        if k in wa and k in wb:
            d = [(i, u, v) for i, (u, v) in enumerate(zip(wa[k], wb[k])) if u != v
                 and not (0x100000 <= int(u, 16) < 0x10000000 and 0x100000 <= int(v, 16) < 0x10000000)]
            print("%s words differing (non-pointer): %d %s" % (k, len(d), " ".join(
                "+0x%X:%s/%s" % (4 * i, u, v) for i, u, v in d[:24])))


if __name__ == "__main__":
    sys.exit(main())
