#!/usr/bin/env python3
"""M4 audio regression sweep (docs/1080_PC_PORT_PLAN.md 15.8).

Renders every sequence-table entry with seqrender (--seq N on the music player), the race start of
every course (--course N: the game's own event script), and a set of sound effects (--sfx K), each
twice, and checks:
  - the render is not silent (peak > 0), unless the golden list marks the case "silent";
  - no mixer errors: badAddr, unknownOps, unk19 (op 25) and AI underruns all 0;
  - tempo: the player's tempo is the header's tempo command (x 48 tatums / beat, plus the driver's
    0.3 % rounding) and the tatums it advanced per second match it (within 1 %);
  - determinism: both runs give the same PCM hash;
  - the PCM hash equals the golden one (port/audio/tools/regress-golden.tsv), so any driver /
    mixer change that alters the output shows up.

  regress.py --seqrender <build>/seqrender [--run32 <sysroot>/run32] [--rom baserom.z64] [--update]

--update rewrites the golden list from this run (after checking a change is intended); it marks a
silent case "silent".  Silent today: seq 0 / 1 (the sound-effect sequences: their channels wait for
IO from snd_SetPlayer0Channel0IO) and the events that are controls, not sounds, on their own
(0x0E / 0x0F / 0x14 / 0x15 / 0x16: player-2 IO cue changes; 0x10 / 0x11: race-start state, 0x11
schedules the course music only after event 4; 0x18 / 0x1C: not traced yet).
Exit status 0 when every case passes.
"""
import argparse
import os
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
GOLDEN = os.path.join(HERE, "regress-golden.tsv")

SEQ_SECONDS = 40.0
COURSE_SECONDS = 30.0
SFX_SECONDS = 4.0
NUM_SEQS = 26  # 0..25: the table's sequences and its three aliases (11 = 8, 24 = 21, 25 = 22)
COURSES = range(0, 8)
# event ids snd_HandleSeqEvent / snd_HandleSeqEnd / the race code post (snd_PostEvent)
SFX = [0x0E, 0x0F, 0x10, 0x11, 0x12, 0x14, 0x15, 0x16, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
       0x33, 0xA3, 0xBC]


# seq 9 (course 5's music) crashes the mixer when it starts in the boot audio spec (garbage reverb
# ring addresses, plan 15.8 open item); the game only plays it after timproc's spec-3 change, so it
# is rendered the game's way here (course5 renders it too).
SEQ_ARGS = {9: ["--event", "4:5", "--post", "0x11@0.1"]}


def cases():
    for n in range(NUM_SEQS):
        yield "seq%02d" % n, SEQ_ARGS.get(n, ["--seq", str(n)]) + ["--seconds", str(SEQ_SECONDS)]
    for c in COURSES:
        yield "course%d" % c, ["--course", str(c), "--gametype", "0", "--seconds", str(COURSE_SECONDS)]
    for k in SFX:
        yield "sfx%02X" % k, ["--sfx", str(k), "--sfx-at", "0.2", "--seconds", str(SFX_SECONDS)]


def run(args, name, extra):
    cmd = ([args.run32] if args.run32 else []) + [args.seqrender, "--rom", args.rom, "--quiet", "--summary",
                                                   "--out", "-"] + extra
    p = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, timeout=300)
    for line in p.stdout.decode(errors="replace").splitlines():
        if line.startswith("RESULT "):
            return dict(kv.split("=", 1) for kv in line[7:].split())
    raise SystemExit("%s: seqrender failed (exit %d):\n%s" % (name, p.returncode, p.stderr.decode()[-2000:]))


def load_golden():
    g = {}
    if os.path.exists(GOLDEN):
        for line in open(GOLDEN):
            if line.startswith("#") or not line.strip():
                continue
            f = line.rstrip("\n").split("\t")
            g[f[0]] = {"hash": f[1], "flags": f[2] if len(f) > 2 else ""}
    return g


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--seqrender", required=True)
    ap.add_argument("--run32", default=os.environ.get("RUN32", ""))
    ap.add_argument("--rom", default="baserom.z64")
    ap.add_argument("--update", action="store_true")
    ap.add_argument("--only", help="comma-separated case names")
    args = ap.parse_args()
    golden = load_golden()
    only = set(args.only.split(",")) if args.only else None

    rows, fails, new = [], 0, {}
    print("%-9s %4s %6s %7s %6s %6s %5s %4s %7s %7s %8s %-4s %-4s  %s" % (
        "case", "seq", "start", "last", "peak", "rms", "clip", "hdr", "bpm", "tempo", "hash", "det", "gold", "verdict"))
    for name, extra in cases():
        if only and name not in only:
            continue
        a = run(args, name, extra)
        b = run(args, name, extra)
        flags = golden.get(name, {}).get("flags", "")
        problems = []
        peak = int(a["peak"])
        if peak == 0 and "silent" not in flags:
            problems.append("silent")
        if peak != 0 and "silent" in flags:
            problems.append("not silent (golden says silent)")
        for k in ("badaddr", "unknownops", "unk19", "underruns"):
            if int(a[k]) != 0:
                problems.append("%s %s" % (k, a[k]))
        hdr = int(a["hdr_bpm"])
        bpm = float(a["bpm"])
        tmin, tmax = int(a["tempo_min"]), int(a["tempo_max"])
        tempo_ok = "-"
        if hdr > 0 and int(a["starts"]) > 0 and name.startswith(("seq", "course")):
            if tmin != tmax:
                tempo_ok = "var"  # the sequence changes its own tempo: no single expectation
            elif tmax != min(hdr * 48, int(a["i2e"])) or abs(bpm - min(hdr, int(a["i2e"]) / 48.0)) > 0.01 * hdr:
                tempo_ok = "BAD"
                problems.append("tempo %d (%.2f bpm) vs header %d bpm" % (tmax, bpm, hdr))
            else:
                tempo_ok = "ok"
        det = "ok" if a["hash"] == b["hash"] else "BAD"
        if det != "ok":
            problems.append("non-deterministic %s / %s" % (a["hash"], b["hash"]))
        gold = golden.get(name, {}).get("hash")
        if gold is None:
            gstat = "new"
        elif gold == a["hash"]:
            gstat = "ok"
        else:
            gstat = "DIFF"
            if not args.update:
                problems.append("hash %s != golden %s" % (a["hash"], gold))
        new[name] = (a["hash"], "silent" if peak == 0 else "")
        verdict = "PASS" if not problems else "FAIL: " + "; ".join(problems)
        if problems:
            fails += 1
        print("%-9s %4s %6s %7s %6d %6.0f %5s %4s %7.2f %7s %8s %-4s %-4s  %s" % (
            name, a["seq"], a["start"], a["last"], peak, float(a["rms"]), a["clip"], hdr if hdr > 0 else "-",
            bpm, tempo_ok, a["hash"], det, gstat, verdict))
        sys.stdout.flush()
    if args.update:
        merged = dict((k, (v["hash"], v["flags"])) for k, v in golden.items())
        merged.update(new)
        with open(GOLDEN, "w") as f:
            f.write("# port/audio/tools/regress.py golden PCM hashes (FNV-1a 32 of the WAV data; seqrender --summary).\n"
                    "# case<TAB>hash<TAB>flags (silent = expected silent).  Regenerate with regress.py --update.\n")
            for name, _ in cases():
                if name in merged:
                    f.write("%s\t%s\t%s\n" % (name, merged[name][0], merged[name][1]))
        print("golden list written: %s" % GOLDEN)
    print("%d case(s) failed" % fails if fails else "all cases passed")
    return 1 if fails else 0


if __name__ == "__main__":
    sys.exit(main())
