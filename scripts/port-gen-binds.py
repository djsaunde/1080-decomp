#!/usr/bin/env python3
"""PC port: bind the placeholder calls that have no CALL name, per function, from the ROM
(docs/1080_PC_PORT_PLAN.md section 14).

    scripts/port-gen-binds.py [--undefined port/undefined.txt]    # after a full matching make

A call spelled with a placeholder token (`timproc_uso_b5_alias(0x4F0)`, `func_00000000(self)`,
an address token `game_uso_func_055750(8)` ...) instead of CALL(f) names no callee in the source.
The callee is in the ROM: the module's TextReloc R_MIPS_26 entry at that jal.  For each kept
function (port/sourcemap.tsv) whose shipped object is the ROM code (name-placeholder-calls.py's
byte guard), every jal reloc against a token the port library leaves undefined is resolved; when
ALL of one function's jals through one token reach the same callee, the pair is bound:

    /* port-bind: FUNC TOKEN TARGET */      (in the unit's port/gen/tu header)

and port/tools/cc-select.py renames TOKEN to TARGET inside FUNC's assembly (and its gcc clones
FUNC.part.N / .constprop.N).  The call is then a plain call of the real function.  A token with
several callees in one function needs a CALL name in the source (listed as `ambiguous`).
Targets that are not a plain symbol are listed, not bound:
  mid-entry   `sym+0xK` (a jal into the middle of a build symbol: libultra entry points);
  sentinel    a module's Text+0 function (`*_00000000`);
  unresolved  no ROM reloc at the site.
Writes port/gen/binds.tsv (unit, function, token, target, sites) and port/gen/binds-left.tsv.
"""
import argparse
import collections
import importlib.util
import os
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
os.chdir(ROOT)
spec = importlib.util.spec_from_file_location("npc", str(ROOT / "scripts/name-placeholder-calls.py"))
npc = importlib.util.module_from_spec(spec)
sys.argv, _argv = [sys.argv[0]], sys.argv
spec.loader.exec_module(npc)
sys.argv = _argv


def read_tsv(path):
    return [l.rstrip("\n").split("\t") for l in open(path)
            if l.strip() and not l.startswith(("#", "function\t"))]


def func_at(md, off):
    """name of the function at Text offset off; `name+0xK` inside one; None outside"""
    for v, sz, nm in md.funcs:
        if v == off:
            return nm
    for v, sz, nm in md.funcs:
        if v < off < v + max(sz, 4):
            return "%s+0x%X" % (nm, off - v)
    return None


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--undefined", default="port/undefined.txt")
    a = ap.parse_args()
    undef = {r[0] for r in read_tsv(a.undefined) if len(r) > 1 and r[1] in ("call", "both")}
    excluded = {r[0] for r in read_tsv("port/gen/excluded.tsv")}
    by_src = collections.defaultdict(list)
    for r in read_tsv("port/sourcemap.tsv"):
        if r[0] not in excluded:
            by_src[r[2]].append(r[0])

    pinned = {}
    for line in open("undefined_syms_auto.txt", errors="replace"):
        m = re.match(r"\s*(\w+)\s*=\s*(0x[0-9A-Fa-f]+|\d+)\s*;", line)
        if m:
            pinned[m.group(1)] = int(m.group(2), 0)
    rom, mv = npc.Rom(), npc.MakeVars()
    binds, left = [], []
    stats = collections.Counter()
    for src in sorted(by_src):
        obj = npc.obj_of(src)
        if not os.path.exists(obj):
            stats["no object"] += 1
            continue
        md = rom.module_for(src) or npc.donor_module(src, rom, mv)
        if md is None or not md.funcs:
            stats["no module"] += 1
            continue
        rfuncs, rrel = npc.elf_info(obj)
        rall = npc.reloc_offsets(obj)
        # sites the source already names (CALL macros) call the real name in the port object:
        # take their callees out of the ROM multiset first
        named = collections.defaultdict(collections.Counter)
        if src.endswith(".c"):
            for st in npc.scan(open(src, errors="replace").read())[0]:
                if st.name is not None:
                    named[(st.func, st.tok)][npc.current_name(st.name) or st.name] += 1
        for fn in by_src[src]:
            fo = fn if fn in rfuncs else ("gl_" + fn) if ("gl_" + fn) in rfuncs else None
            if fo is None:
                continue
            base = rom.func_off(md, fo)
            rv, rsz, rb = rfuncs[fo]
            if base is None or not npc.rom_bytes_match(md, base, rb, rv, rall):
                stats["function not proven"] += 1
                continue
            per = collections.defaultdict(collections.Counter)
            for off, sym in rrel.items():
                if rv <= off < rv + rsz and sym in undef:
                    t = rom.jal_target(md, base + off - rv)
                    if t is not None and t.endswith("_00000000") and pinned.get(sym):
                        # a self-relocation: the jal field holds the Text offset (the token's
                        # pinned address), the reloc only adds the module base
                        t = md.name_at(pinned[sym]) if hasattr(md, "name_at") else func_at(md, pinned[sym])
                    per[sym][t] += 1
            for tok in list(per):
                per[tok] -= named.get((fn, tok), collections.Counter())
                if not per[tok]:
                    del per[tok]
            for tok, tg in sorted(per.items()):
                n = sum(tg.values())
                if len(tg) > 1:
                    left.append((src, fn, tok, "ambiguous", " ".join("%s*%d" % kv for kv in tg.items() if kv[0]), n))
                    continue
                target = next(iter(tg))
                if target is None:
                    kind = "unresolved"
                elif "+" in target:
                    kind = "mid-entry"
                elif target.endswith("_00000000"):
                    kind = "sentinel"
                else:
                    binds.append((src, fn, tok, target, n))
                    continue
                left.append((src, fn, tok, kind, target or "", n))
    with open("port/gen/binds.tsv", "w") as fh:
        fh.write("# generated by scripts/port-gen-binds.py: unit\tfunction\ttoken\ttarget\tsites\n")
        for r in binds:
            fh.write("\t".join(map(str, r)) + "\n")
    with open("port/gen/binds-left.tsv", "w") as fh:
        fh.write("# generated by scripts/port-gen-binds.py: unit\tfunction\ttoken\tkind\ttarget(s)\tsites\n")
        for r in left:
            fh.write("\t".join(map(str, r)) + "\n")
    kinds = collections.Counter(r[3] for r in left)
    print("bound %d (function, token) pairs, %d sites, %d tokens; left %d: %s; %s" % (
        len(binds), sum(r[4] for r in binds), len({r[2] for r in binds}), len(left),
        dict(kinds), dict(stats)))
    return 0


if __name__ == "__main__":
    sys.exit(main())
