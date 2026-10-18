#!/usr/bin/env python3
"""Scan non-exact functions for the BY-VALUE STRUCT parameter / return tells.

IDO homes a struct passed by value at entry (sw aN,home(sp)) and reads its
members back from the home; a struct returned by value is copied word by word
into the hidden a0 buffer in the epilogue and returned in v0.  Old notes often
call the resulting residuals "register caps".  See docs/IDO_CODEGEN.md
#post0b-kit-v-run106.

Tells, counted in the TARGET (expected/) disassembly of each function:

  sub   lh/lhu/lb/lbu from sp+N with N >= frame size (a param home), or an
        lwc1 from a param home: a member of a by-value struct (or a char/short
        passed on the stack).  Offsets that are not word aligned (home+2/+3)
        are the strongest form and are counted again in `odd`.
  out   sw aN,K(sp) with K < 16 (the function's OWN outgoing-arg area) before
        a jal: passing a by-value struct on to the callee.
  ret   lw v0,F(sp) (a0's home) in the epilogue plus a lw/sw word copy through
        a pointer loaded from the same home: `S f(...)` with a hidden return
        buffer.
  homes number of a0..a3 homed at entry (weak; all homed while one is never
        read as an int = that one is a struct).
  hl    every load (lw/lwc1/lh/lb...) from a param home, frame > 0 (weak:
        members re-read from the home at each use; the build keeping the
        param in a register instead is the classic by-value residual).

The same counts are taken from the BUILT (build/non_matching) object; `miss`
is how many strong tells (sub + out + ret) the target has that the build does
not reproduce, `weak` the same for homes + hl.  Rows rank by
size * (100 - fuzzy), filtered to miss + weak > 0 unless --all is given
(--strong keeps only miss > 0).

Usage (from a project worktree, after `make non_matching_objects` and a fresh
report.json):

    python3 scripts/byvalue-struct-scan.py [--units GLOB,...] [--max-addr post0b=0x50000]
                                           [--all] [--min-fuzzy N]
"""
import argparse
import fnmatch
import json
import re
import subprocess
import sys

HEAD = re.compile(r"^[0-9a-f]+ <([^>]+)>:$")
INS = re.compile(r"^\s+[0-9a-f]+:\s+[0-9a-f]{8}\s+(\S+)\s*(.*)$")
MEM = re.compile(r"^(\w+),(-?\d+)\((\w+)\)$")
SUBW = ("lh", "lhu", "lb", "lbu", "lwc1")


def disasm(obj):
    try:
        out = subprocess.run(
            ["mips-linux-gnu-objdump", "-d", "-M", "no-aliases", "-j", ".text", obj],
            capture_output=True, text=True, check=True).stdout
    except (subprocess.CalledProcessError, FileNotFoundError):
        return {}
    res, cur = {}, None
    for line in out.splitlines():
        m = HEAD.match(line)
        if m:
            name = m.group(1)
            cur = None if name.startswith(".") else res.setdefault(name, [])
            continue
        if cur is None:
            continue
        m = INS.match(line)
        if m:
            cur.append((m.group(1), m.group(2).replace(" ", "")))
    return res


def tells(insns):
    frame = 0
    for op, args in insns[:3]:
        m = re.match(r"^sp,sp,(-\d+)$", args)
        if op == "addiu" and m:
            frame = -int(m.group(1))
            break
    t = {"sub": 0, "odd": 0, "out": 0, "ret": 0, "homes": 0, "hl": 0, "frame": frame}
    n = len(insns)
    a0home = frame
    homeptr = set()  # regs holding the pointer loaded from a0's home
    for i, (op, args) in enumerate(insns):
        m = MEM.match(args)
        if not m:
            continue
        reg, off, base = m.group(1), int(m.group(2)), m.group(3)
        if base == "sp":
            if op in SUBW and off >= frame:
                t["sub"] += 1
                if op != "lwc1" and off % 4:
                    t["odd"] += 1
            if op == "sw" and reg in ("a0", "a1", "a2", "a3") and off >= frame and i < 14:
                t["homes"] += 1
            if op in SUBW + ("lw",) and off >= frame and frame > 0:
                t["hl"] += 1
            if op == "sw" and off < 16 and frame > 0 and reg in ("a0", "a1", "a2", "a3"):
                # must precede a jal within a few instructions
                if any(o == "jal" or o == "jalr" for o, _ in insns[i + 1:i + 8]):
                    t["out"] += 1
            if op == "lw" and off == a0home and frame > 0:
                if reg == "v0" and i >= n - 6:
                    t["ret"] += 1
                else:
                    homeptr.add(reg)
        elif op == "sw" and base in homeptr and i >= n - 24:
            t["ret"] += 1 if t["ret"] == 0 else 0
    if t["ret"] and not any(o == "lw" and a.startswith("v0,") and a.endswith("%d(sp)" % a0home)
                            for o, a in insns[-6:]):
        t["ret"] = 0
    return t


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--units", default="game_libs_post,game_libs_post0b,bootup_uso*,h2hproc_uso*,timproc_uso_b1*",
                    help="comma list of fnmatch globs on the unit basename")
    ap.add_argument("--max-addr", default="game_libs_post0b=0x50000",
                    help="unit=ADDR,... only fns whose name address is below ADDR")
    ap.add_argument("--all", action="store_true")
    ap.add_argument("--strong", action="store_true")
    ap.add_argument("--min-fuzzy", type=float, default=0.0)
    a = ap.parse_args()
    subs = [s for s in a.units.split(",") if s]
    maxa = {}
    for kv in a.max_addr.split(","):
        if "=" in kv:
            k, v = kv.split("=")
            maxa[k] = int(v, 0)

    cfg = {u["name"]: u for u in json.load(open("objdiff.json"))["units"]}
    rep = json.load(open("report.json"))
    rows = []
    for u in rep["units"]:
        un = u["name"]
        base = un.rsplit("/", 1)[-1]
        if subs and not any(fnmatch.fnmatch(base, s) for s in subs):
            continue
        c = cfg.get(un)
        if not c or "base_path" not in c or "target_path" not in c:
            continue
        nm = [f for f in u.get("functions", []) if f.get("fuzzy_match_percent", 0) < 100
              and f.get("fuzzy_match_percent", 0) >= a.min_fuzzy]
        if base in maxa:
            def addr(f):
                m = re.search(r"([0-9A-Fa-f]{8})$", f["name"])
                return int(m.group(1), 16) if m else 0
            nm = [f for f in nm if addr(f) < maxa[base]]
        if not nm:
            continue
        T = disasm(c["target_path"])
        B = disasm(c["base_path"])
        for f in nm:
            fn = f["name"]
            if fn not in T:
                continue
            tt = tells(T[fn])
            bt = tells(B.get(fn, []))
            miss = max(0, tt["sub"] - bt["sub"]) + max(0, tt["out"] - bt["out"]) + max(0, tt["ret"] - bt["ret"])
            weak = max(0, tt["homes"] - bt["homes"]) + max(0, tt["hl"] - bt["hl"])
            if not a.all and (miss == 0 if a.strong else miss + weak == 0):
                continue
            size = int(f.get("size", 0))
            fz = f.get("fuzzy_match_percent", 0.0)
            rows.append((size * (100 - fz), size, fz, tt, bt, miss, weak, un, fn))
    rows.sort(key=lambda r: -r[0])
    print(f"{'rank':>8} {'size':>5} {'fuzzy':>6} {'frm':>4} {'sub':>3} {'odd':>3} {'out':>3} {'ret':>3} "
          f"{'hom':>3} {'hl':>3} {'miss':>4} {'weak':>4}  build(sub/out/ret/hom/hl)  unit / function")
    for rk, size, fz, tt, bt, miss, weak, un, fn in rows:
        print(f"{rk:8.0f} {size:5d} {fz:6.1f} {tt['frame']:4d} {tt['sub']:3d} {tt['odd']:3d} {tt['out']:3d} "
              f"{tt['ret']:3d} {tt['homes']:3d} {tt['hl']:3d} {miss:4d} {weak:4d}  "
              f"{bt['sub']:3d}/{bt['out']}/{bt['ret']}/{bt['homes']}/{bt['hl']:<10d} "
              f"{un.replace('src/', '')} {fn}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
