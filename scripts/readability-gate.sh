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
#   5. writes the regenerated report.json into the tree (commit it with the batch).
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

echo ">>> [1/4] full build + ROM verify"
make RUN_CC_CHECK=0 -j"$JOBS" > "$TMP/make.log" 2>&1 || { tail -20 "$TMP/make.log"; fail "make (see $TMP/make.log)"; }
grep -q "ROM OK (byte-identical" "$TMP/make.log" || fail "no 'ROM OK' line in make output"
cmp -s tenshoe.z64 baserom.z64 || fail "tenshoe.z64 != baserom.z64"
echo "    ROM OK (byte-identical)"

echo ">>> [2/4] verify-blocks"
make verify-blocks > "$TMP/blocks.log" 2>&1 || { tail -10 "$TMP/blocks.log"; fail "verify-blocks"; }
nok=$(grep -c '^OK ' "$TMP/blocks.log" || true)
[[ "$nok" == 6 ]] || fail "verify-blocks $nok/6"
echo "    verify-blocks 6/6"

echo ">>> [3/4] non_matching_objects"
make RUN_CC_CHECK=0 -j"$JOBS" non_matching_objects > "$TMP/nm.log" 2>&1 || { tail -20 "$TMP/nm.log"; fail "non_matching_objects"; }
nerr=$(grep -E '(\bError\b|error:|\*\*\*)' "$TMP/nm.log" | grep -vc 'Warning' || true)
[[ "$nerr" == 0 ]] || { grep -E '(\bError\b|error:|\*\*\*)' "$TMP/nm.log" | head; fail "non_matching_objects: $nerr error lines"; }
echo "    non_matching_objects: 0 errors"

echo ">>> [4/4] objdiff report"
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
    return r["measures"], d
mb, b = load(base); mn, n = load(new)
bad = []
if set(b) != set(n):
    bad.append(f"function (unit,addr,size) set changed: -{len(set(b)-set(n))} +{len(set(n)-set(b))}")
    for k in sorted(set(b) ^ set(n))[:10]: bad.append(f"   {k}")
not100 = [(k, v) for k, v in n.items() if v[1] != 100.0]
if not100:
    bad.append(f"{len(not100)} functions not at 100%: {not100[:5]}")
renamed, unexplained = 0, []
for k in set(b) & set(n):
    ob, nb = b[k][0], n[k][0]
    if ob != nb:
        if fold(ob) == nb: renamed += 1
        else: unexplained.append((k, ob, nb))
if unexplained:
    bad.append(f"{len(unexplained)} name changes not in renames.tsv: {unexplained[:5]}")
tf, mf = mn["total_functions"], mn.get("matched_functions", 0)
if tf != mb["total_functions"] or mf != tf:
    bad.append(f"functions {mf}/{tf} (base {mb.get('matched_functions')}/{mb['total_functions']})")
for key in ("total_code", "matched_code", "total_data", "matched_data"):
    if str(mn.get(key)) != str(mb.get(key)):
        bad.append(f"{key} {mb.get(key)} -> {mn.get(key)}")
if bad:
    print("\n".join(bad)); sys.exit(1)
print(f"    report: {mf}/{tf} functions at 100%, code {mn['matched_code']}/{mn['total_code']}, "
      f"exact set identical by address; {renamed} renamed (all in renames.tsv)")
PY

if [[ $WRITE == 1 ]]; then
    cp "$TMP/report.new.json" report.json
    echo "    report.json refreshed"
fi
echo "GATE PASS"
