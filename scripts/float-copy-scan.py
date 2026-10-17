#!/usr/bin/env python3
"""Scan NM functions for the float-copied-as-int-word signature.

Signature: the BUILT (build/non_matching) object has extra integer `lw`/`sw`
while the TARGET (expected/) has matching extra `lwc1`/`swc1`.  That means the
C source moves a float through an int-typed lvalue (s32 field, int array, raw
`*(s32 *)` copy) where the original moved it through an f32 lvalue.  Retyping
the field / struct member / pointer as f32 flips each lw/sw pair to
lwc1/swc1 and is typically a multi-point gain (timproc_b5 7C1C +8.2, 8FC8 +3.9).

The reverse signature (build has extra lwc1/swc1, target extra lw/sw) is also
reported with a negative `dir` -- the source types a field f32 that the
original copied as an int word.

Usage (from a project worktree, after `make non_matching_objects` and a fresh
report.json):

    python3 scripts/float-copy-scan.py [--units SUBSTR,...] [--all] [--min N]

  --units  only scan objdiff units whose name contains one of the substrings
  --all    list every NM fn with any lw/sw/lwc1/swc1 delta, not just the signature
  --min    minimum matched-pair count to print (default 1)

Columns: offm  = offset-matched pairs: non-$sp field offsets accessed as int in
                one object and as float in the other (strongest signal;
                rows sort by it).  0 with pairs>0 is usually a spill/regalloc
                coincidence, not a float copy.
         pairs = min(extra int ops, missing float ops) (the fixable count),
         dlw/dsw/dlwc1/dswc1 = build - target, size = target bytes, fuzzy %.
"""
import argparse
import collections
import json
import re
import subprocess
import sys

OPS = ("lw", "sw", "lwc1", "swc1")


def disasm_counts(obj):
    """Return {func: Counter} for every function symbol in obj's .text.

    Keys are the bare op (lw/sw/lwc1/swc1) plus ("i"|"f", offset) for each
    non-$sp memory access, so int-vs-float copies of the SAME field offset can
    be paired (stack spills are excluded from the offset pairing)."""
    try:
        out = subprocess.run(
            ["mips-linux-gnu-objdump", "-d", "-M", "no-aliases", "-j", ".text", obj],
            capture_output=True, text=True, check=True,
        ).stdout
    except (subprocess.CalledProcessError, FileNotFoundError):
        return {}
    res = {}
    cur = None
    head = re.compile(r"^[0-9a-f]+ <([^>]+)>:$")
    ins = re.compile(r"^\s+[0-9a-f]+:\s+[0-9a-f]{8}\s+(\S+)\s+[^,]+,(-?\d+)\((\w+)\)")
    for line in out.splitlines():
        m = head.match(line)
        if m:
            name = m.group(1)
            cur = None if name.startswith(".") else res.setdefault(name, collections.Counter())
            continue
        if cur is None:
            continue
        m = ins.match(line)
        if m and m.group(1) in OPS:
            op = m.group(1)
            cur[op] += 1
            if m.group(3) != "sp":
                cur[("f" if op.endswith("c1") else "i", int(m.group(2)))] += 1
    return res


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--units", default="")
    ap.add_argument("--all", action="store_true")
    ap.add_argument("--min", type=int, default=1)
    a = ap.parse_args()
    subs = [s for s in a.units.split(",") if s]

    cfg = {u["name"]: u for u in json.load(open("objdiff.json"))["units"]}
    rep = json.load(open("report.json"))
    rows = []
    for u in rep["units"]:
        un = u["name"]
        if subs and not any(s in un for s in subs):
            continue
        c = cfg.get(un)
        if not c or "base_path" not in c or "target_path" not in c:
            continue
        nm = [f for f in u.get("functions", []) if f.get("fuzzy_match_percent", 0) < 100]
        if not nm:
            continue
        b = disasm_counts(c["base_path"])
        t = disasm_counts(c["target_path"])
        for f in nm:
            fn = f["name"]
            if fn not in b or fn not in t:
                continue
            d = {op: b[fn][op] - t[fn][op] for op in OPS}
            extra_int = d["lw"] + d["sw"]
            miss_flt = -(d["lwc1"] + d["swc1"])
            if extra_int > 0 and miss_flt > 0:
                pairs, dirn = min(extra_int, miss_flt), 1
            elif extra_int < 0 and miss_flt < 0:
                pairs, dirn = min(-extra_int, -miss_flt), -1
            else:
                pairs, dirn = 0, 0
            # offset-matched pairs: field offsets the build touches as int but
            # the target touches as float (or the reverse for dir -1)
            offs = {k[1] for k in list(b[fn]) + list(t[fn]) if isinstance(k, tuple)}
            om = 0
            for o in offs:
                bi, bf = b[fn][("i", o)], b[fn][("f", o)]
                ti, tf = t[fn][("i", o)], t[fn][("f", o)]
                om += max(0, min(bi - ti, tf - bf)) if dirn >= 0 else max(0, min(ti - bi, bf - tf))
            if pairs < a.min and not (a.all and any(d.values())):
                continue
            rows.append((pairs, int(f.get("size", 0)), dirn, d, om,
                         f.get("fuzzy_match_percent", 0.0), un, fn))
    rows.sort(key=lambda r: (-r[4], -r[0], -r[1]))
    print(f"{'offm':>4} {'pairs':>5} {'dir':>3} {'dlw':>4} {'dsw':>4} {'dlwc1':>5} {'dswc1':>5} "
          f"{'size':>6} {'fuzzy':>6}  unit / function")
    for pairs, size, dirn, d, om, fz, un, fn in rows:
        print(f"{om:4d} {pairs:5d} {dirn:3d} {d['lw']:4d} {d['sw']:4d} {d['lwc1']:5d} {d['swc1']:5d} "
              f"{size:6d} {fz:6.1f}  {un.replace('src/', '')} {fn}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
