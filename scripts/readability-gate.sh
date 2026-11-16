#!/usr/bin/env bash
# Readability gate: prove a rename / struct-typing batch is PURE (no byte change).
#
# Usage: scripts/readability-gate.sh [--base REF] [--keep-build] [--no-write-report]
#
#   1. clean full build (`make RUN_CC_CHECK=0 -j8`, whose `all: verify` target
#      md5-checks + cmp's tenshoe.z64 against baserom.z64)  -> ROM byte-identical
#   2. make verify-blocks                                   -> 6/6 Yay0 blocks OK
#   3. make non_matching_objects                            -> 0 errors
#   4. objdiff-cli report generate                          -> every function 100%,
#      same function count as REF's report.json, and the same (unit, address,
#      size) set; every NAME that differs must be an OLD->NEW pair recorded in
#      renames.tsv (so the only change is the names you meant to change).
#      Units marked `"complete": false` in objdiff.json (the 2026-11-16 new modules,
#      docs/1080_NEW_MODULES.md) may hold unmatched INCLUDE_ASM functions; a unit
#      absent from REF may be added whole (a complete one with every function at 100%,
#      e.g. code split out of a bin, as game_libs_EBF8 on 2026-11-16), and a unit REF has
#      may gain a 100% function over bytes no REF function covered (eddproc drawGui). Every other count must stay identical,
#      and a function at 100% in REF must stay at 100%.
#   then writes the regenerated report.json into the tree (commit it with the batch).
#   5. source checks: name-placeholder-calls.py check (every CALL(X) names a ROM
#      callee of its function) and uso-data-sites.py check (alias / GL_* claims,
#      plus the plain-base-site ratchet that catches a merge dropping a GL_DATA_STR),
#      and gameflow-import-sites.py check (every INPUT_MGR(tok) token's relocs target
#      GameFlow, bootup Bss 0xF8, in the ROM).
#   6. ROM coverage: scripts/rom-coverage.py --check assigns every baserom byte to one
#      category (code-c / code-asm / data / asset / ucode / padding) from the ROM
#      filesystem + linker map; fails on any unattributed byte, any USO Text without a
#      code segment, or code shapes inside data (reviewed exceptions: rom-coverage.allow).
#
# REF defaults to HEAD (the last committed report.json). Scratch: $GATE_TMP
# (default /tmp/claude-$UID/readability-gate-<worktree dir name>, private per
# worktree so parallel agents never clobber each other's logs/reports).
set -euo pipefail

BASE=HEAD
CLEAN=1
WRITE=1
while [[ $# -gt 0 ]]; do
    case "$1" in
        --base) BASE="$2"; shift 2 ;;
        --keep-build) CLEAN=0; shift ;;
        --no-write-report) WRITE=0; shift ;;
        *) echo "unknown arg $1" >&2; exit 2 ;;
    esac
done

ROOT="$(git rev-parse --show-toplevel)"
cd "$ROOT"
VENV=/home/danjs/code/decomp/tools/build-venv/bin
[[ -d "$VENV" ]] && export PATH="$VENV:$PATH"
JOBS="${JOBS:-8}"
TMP="${GATE_TMP:-/tmp/claude-$(id -u)/readability-gate-$(basename "$ROOT")}"
mkdir -p "$TMP"
echo "    scratch: $TMP"

fail() { echo "GATE FAIL: $*" >&2; exit 1; }

git show "$BASE:report.json" > "$TMP/base_report.json" || fail "no report.json at $BASE"

if [[ $CLEAN == 1 ]]; then rm -rf build tenshoe.z64; fi

echo ">>> [1/6] full build + ROM verify"
make RUN_CC_CHECK=0 -j"$JOBS" > "$TMP/make.log" 2>&1 || { tail -20 "$TMP/make.log"; fail "make (see $TMP/make.log)"; }
grep -q "ROM OK (byte-identical" "$TMP/make.log" || fail "no 'ROM OK' line in make output"
cmp -s tenshoe.z64 baserom.z64 || fail "tenshoe.z64 != baserom.z64"
echo "    ROM OK (byte-identical)"

echo ">>> [2/6] verify-blocks"
make verify-blocks > "$TMP/blocks.log" 2>&1 || { tail -10 "$TMP/blocks.log"; fail "verify-blocks"; }
nok=$(grep -c '^OK ' "$TMP/blocks.log" || true)
[[ "$nok" == 6 ]] || fail "verify-blocks $nok/6"
echo "    verify-blocks 6/6"

echo ">>> [3/6] non_matching_objects"
make RUN_CC_CHECK=0 -j"$JOBS" non_matching_objects > "$TMP/nm.log" 2>&1 || { tail -20 "$TMP/nm.log"; fail "non_matching_objects"; }
nerr=$(grep -E '(\bError\b|error:|\*\*\*)' "$TMP/nm.log" | grep -vc 'Warning' || true)
[[ "$nerr" == 0 ]] || { grep -E '(\bError\b|error:|\*\*\*)' "$TMP/nm.log" | head; fail "non_matching_objects: $nerr error lines"; }
echo "    non_matching_objects: 0 errors"

echo ">>> [4/6] objdiff report"
objdiff-cli report generate -p . -o "$TMP/report.new.json" > "$TMP/objdiff.log" 2>&1 || fail "objdiff report generate"
python3 - "$TMP/base_report.json" "$TMP/report.new.json" renames.tsv <<'PY' || fail "report comparison"
import json, sys
from pathlib import Path
base, new, ledger = sys.argv[1:4]
ren = {}
if Path(ledger).exists():
    for line in Path(ledger).read_text().splitlines():
        line = line.split("#", 1)[0].strip()
        if line:
            o, n = line.split()[:2]
            ren[o] = n
def fold(name):
    seen = set()
    while name in ren and name not in seen:
        seen.add(name); name = ren[name]
    return name
def load(p):
    r = json.load(open(p)); d = {}
    for u in r["units"]:
        for f in u.get("functions", []):
            d[(u["name"], int(f.get("address", 0) or 0), int(f["size"]))] = (f["name"], f.get("fuzzy_match_percent", 0.0))
    return r["measures"], d, {u["name"] for u in r["units"]}
mb, b, bunits = load(base); mn, n, _ = load(new)
# units still being decompiled (objdiff.json "complete": false) may hold unmatched functions
incomplete = {u["name"] for u in json.load(open("objdiff.json"))["units"]
              if not u.get("metadata", {}).get("complete", True)}
bad = []
# a unit absent from REF may be added whole: an incomplete one with any functions, a complete
# one only at 100% (the not100 check below), e.g. a function split out of a bin.  In a unit
# REF has, a NEW function may appear only at 100% and over bytes no REF function covered
# (code that was a symbol-less pad or a bin, e.g. eddproc's drawGui on 2026-11-16).
def fresh(k):
    return n[k][1] == 100.0 and not any(u == k[0] and a < k[1] + k[2] and k[1] < a + s
                                        for (u, a, s) in b)
added = {k for k in set(n) - set(b) if k[0] not in bunits or fresh(k)}
if set(b) != set(n) - added:
    bad.append(f"function (unit,addr,size) set changed: -{len(set(b)-set(n))} +{len(set(n)-set(b)-added)}")
    for k in sorted(set(b) ^ (set(n) - added))[:10]: bad.append(f"   {k}")
not100 = [(k, v) for k, v in n.items() if v[1] != 100.0 and k[0] not in incomplete]
dropped = [(k, v) for k, v in n.items() if v[1] != 100.0 and k in b and b[k][1] == 100.0]
if not100 or dropped:
    bad.append(f"{len(not100) + len(dropped)} functions not at 100%: {(not100 + dropped)[:5]}")
if added:
    print(f"    {len(added)} new functions added in {len({k[0] for k in added})} unit(s)")
renamed, unexplained = 0, []
for k in set(b) & set(n):
    ob, nb = b[k][0], n[k][0]
    if ob != nb:
        if fold(ob) == nb: renamed += 1
        else: unexplained.append((k, ob, nb))
if unexplained:
    bad.append(f"{len(unexplained)} name changes not in renames.tsv: {unexplained[:5]}")
tf, mf = mn["total_functions"], mn.get("matched_functions", 0)
nmatched = sum(1 for v in n.values() if v[1] == 100.0)
if tf != mb["total_functions"] + len(added) or mf != nmatched or mf < mb.get("matched_functions", 0):
    bad.append(f"functions {mf}/{tf} (base {mb.get('matched_functions')}/{mb['total_functions']}, +{len(added)} added)")
want = dict(mb)
want["total_code"] = int(mb["total_code"]) + sum(k[2] for k in added)
for key in ("total_code", "total_data", "matched_data"):
    if str(mn.get(key)) != str(want.get(key)):
        bad.append(f"{key} {want.get(key)} -> {mn.get(key)}")
if int(mn.get("matched_code", 0)) < int(mb.get("matched_code", 0)):
    bad.append(f"matched_code {mb.get('matched_code')} -> {mn.get('matched_code')}")
if bad:
    print("\n".join(bad)); sys.exit(1)
print(f"    report: {mf}/{tf} functions at 100%, code {mn['matched_code']}/{mn['total_code']}, "
      f"exact set identical by address; {renamed} renamed (all in renames.tsv)")
PY

if [[ $WRITE == 1 ]]; then
    cp "$TMP/report.new.json" report.json
    echo "    report.json refreshed"
fi
echo ">>> [5/6] source checks: named placeholder calls, data-site aliases, GameFlow imports"
# both re-exec under the repo .venv (pyelftools, crunch64); ~5 s on the fresh build
python3 scripts/name-placeholder-calls.py check > "$TMP/calls.log" 2>&1 || { grep -v '^check:' "$TMP/calls.log" | head -20; fail "name-placeholder-calls.py check (see $TMP/calls.log)"; }
tail -1 "$TMP/calls.log" | sed 's/^/    /'
python3 scripts/uso-data-sites.py --no-build check > "$TMP/datasites.log" 2>&1 || { grep -E 'MISMATCH|Error|error' "$TMP/datasites.log" | head -20; fail "uso-data-sites.py check (see $TMP/datasites.log)"; }
echo "    uso-data-sites check: 0 mismatches ($(grep -c '' "$TMP/datasites.log") lines)"
python3 scripts/gameflow-import-sites.py check > "$TMP/gameflow.log" 2>&1 || { grep -v '^check:' "$TMP/gameflow.log" | head -20; fail "gameflow-import-sites.py check (see $TMP/gameflow.log)"; }
tail -1 "$TMP/gameflow.log" | sed 's/^/    /'
echo ">>> [6/6] ROM coverage (every byte attributed; docs/ROM_COVERAGE.md)"
python3 scripts/rom-coverage.py --check > "$TMP/romcov.log" 2>&1 || { sed -n '/^Gaps/,$p' "$TMP/romcov.log" | head -30; fail "rom-coverage.py --check (see $TMP/romcov.log)"; }
grep -E 'honest decomp|^Gaps' "$TMP/romcov.log" | sed 's/^ */    /'
echo "GATE PASS"
