#!/usr/bin/env bash
# PC port gate (docs/1080_PC_PORT_PLAN.md 18.18, run 839): the landing gate (gate-landing-tree.sh)
# checks the MATCHING build only; this checks that the port still regenerates, builds and runs.
#
#   LAND_WT=<worktree> scripts/port-gate.sh [--regen] [--update-baselines]
#
# Needs the matching build in $LAND_WT (make; port-gen-binds reads build/ and the ROM relocs).
# Steps (one summary line each; every step runs, the exit status is the number of failed steps):
#   gen-map    port-sourcemap.py + port-gen-build.py   -> committed port/gen, sourcemap*.tsv unchanged
#   build      fresh i386 build, -DPORT_LINK=ON: 0 compile errors, 0 undefined symbols, no warning
#              of the gated classes (implicit declarations, return type, int/pointer casts,
#              C++ -fpermissive conversions ...) beyond port/tools/port-gate-warnings.tsv
#              (that baseline never accepts a return-type hit: -Wreturn-type / -Wreturn-mismatch in a
#              shipped body must be on the reviewed port/tools/return-type-allow.tsv; the dropped copies
#              port/sourcemap.tsv does not ship are exempt)
#   gen-lib    port-gen-decls / binds / uso (from the built game library) + port-gen-blob-types.py
#              -> committed port/gen unchanged
#   test_ultra, test_fs, gfx (make -C port/gfx test: golden scenes, software GL),
#   audio      (make -C port/audio regress)
#   smoke      tenshoe headless for PORT_GATE_FRAMES (default 2000) frames from a clean dir:
#              rc 0, state.log has every frame, no NaN / Inf in the rider fields (r<N>= pos, quat,
#              speed; the camera's NaN / Inf count is printed, not gated: camTarget goes NaN at
#              frame 1619 on main, run 839).
#              2000: main crashed at ~2625 at the time of writing (agent-g run 835 is on it).
#   saves      test_backup + 4 short boots: the SRAM file is the N64 image (port/include/port_backup.h,
#              plan 20): blank-SRAM save = the ROM defaults' big-endian image, reloads unchanged,
#              a synthetic .sra loads, damaged primaries are restored from the mirrors
#   abi        port/tools/abi-scan.py (DWARF call sites vs definitions) against
#              port/tools/abi-baseline.tsv: no new float / int / double / struct-return mismatch
# --regen             keep the regenerated port/gen + sourcemap (default: restore, report stale)
#                     and rebuild from them before the checks
# --update-baselines  rewrite the warning and ABI baselines from this run (review the diff!)
# Env: LAND_WT (default: this repo), PORT_GATE_TMP (scratch, default $TMPDIR/port-gate-<wt>),
#      PORT_GATE_FRAMES, PORT_GATE_JOBS (8).  ~2 minutes; never commits, never touches the ROM.
set -uo pipefail
MONO=/home/danjs/code/decomp
SELF_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
WT="$(cd "${LAND_WT:-$SELF_DIR/..}" && pwd)" || exit 99
LOG="${PORT_GATE_TMP:-${TMPDIR:-/tmp}/port-gate-$(basename "$WT")}"
FRAMES="${PORT_GATE_FRAMES:-2000}"; JOBS="${PORT_GATE_JOBS:-8}"
REGEN=0; UPDATE=0
for a in "$@"; do case "$a" in
  --regen) REGEN=1 ;; --update-baselines) UPDATE=1 ;;
  *) echo "usage: LAND_WT=<wt> $0 [--regen] [--update-baselines]"; exit 99 ;; esac; done
export PATH="$MONO/tools/build-venv/bin:$MONO/.venv/bin:$MONO/tools/cmake-venv/bin:$PATH"
RUN32="$MONO/tools/i386-sysroot/run32"; TC="$MONO/tools/i386-sysroot/toolchain-i386.cmake"
B="$LOG/build"; ROM="$WT/baserom.z64"
cd "$WT" || exit 99
mkdir -p "$LOG"
echo "== port gate: $WT  HEAD $(git log --oneline -1 | cut -c1-70)  (logs: $LOG)"
[ -f build/tenshoe.elf ] && [ -f undefined_syms_auto.txt ] || { echo "!! no matching build in $WT: run make first"; exit 99; }
[ "$(stat -Lc %s "$ROM" 2>/dev/null)" = 16777216 ] || { echo "!! $ROM is not the 16 MB ROM"; exit 99; }

FAILS=0; T0=0
start() { T0=$(date +%s); }
line() {  # line <step> <PASS|FAIL> <text>
  local dt=$(( $(date +%s) - T0 ))
  printf '%-10s %-4s %4ds  %s\n' "$1" "$2" "$dt" "$3"
  [ "$2" = FAIL ] && FAILS=$((FAILS + 1))
  return 0
}
# the generators write in place: snapshot the committed outputs, restore them unless --regen
GEN_FILES="port/gen port/sourcemap.tsv port/sourcemap-placeholders.tsv"
SNAP="$LOG/snap"; rm -rf "$SNAP"; mkdir -p "$SNAP/port"; cp -a $GEN_FILES "$SNAP/port/"
restore() { rm -rf port/gen && cp -a "$SNAP/port/gen" port/gen && cp -a "$SNAP/port/"sourcemap*.tsv port/; }
[ $REGEN = 0 ] && trap restore EXIT
gendiff() {  # files that differ from the snapshot
  { diff -rq "$SNAP/port/gen" port/gen; for f in sourcemap.tsv sourcemap-placeholders.tsv; do
      cmp -s "$SNAP/port/$f" "port/$f" || echo "port/$f"; done; } 2>&1 | sed -E "s|$SNAP/||; s/^Files ([^ ]*) and .*/\1/" | sort -u
}
gen() {  # gen <log> <cmd...>
  local l="$1"; shift; "$@" >"$LOG/$l.log" 2>&1 || { echo "$l failed (rc $?): $(tail -1 "$LOG/$l.log")"; return 1; }
}

# ---- gen-map: the source map and the build skeleton (no library needed)
start; err=""
gen sourcemap python3 scripts/port-sourcemap.py --quiet || err="$err sourcemap"
gen gen-build python3 scripts/port-gen-build.py || err="$err gen-build"
st=$(gendiff); n=$(printf '%s' "$st" | grep -c .)
if [ -n "$err" ]; then line gen-map FAIL "generator failed:$err (see $LOG)"
elif [ "$n" != 0 ]; then line gen-map FAIL "$n stale file(s): $(echo $st | cut -c1-200)$([ $REGEN = 1 ] && echo ' (regenerated)')"
else line gen-map PASS "port/sourcemap*.tsv, sources.cmake, tu/ up to date"; fi
[ $REGEN = 0 ] && restore

# ---- build: a fresh tree; extra warnings only (no codegen change)
WARN_C="-Wreturn-type -Wimplicit-function-declaration -Wimplicit-int"
build() {
  rm -rf "$B"
  cmake -S port -B "$B" -G Ninja -DCMAKE_TOOLCHAIN_FILE="$TC" -DPORT_LINK=ON -DPORT_ROM="$ROM" \
        "-DCMAKE_C_FLAGS=$WARN_C" "-DCMAKE_CXX_FLAGS=-Wreturn-type" >"$LOG/configure.log" 2>&1 || return 2
  cmake --build "$B" -j"$JOBS" -- -k 0 >"$LOG/build.log" 2>&1
}
start; build; brc=$?; TB=$(( $(date +%s) - T0 ))
if [ $brc = 2 ]; then line build FAIL "cmake configure failed: $(tail -1 "$LOG/configure.log")"; fi

# ---- gen-lib: the generators that read the built game library
start; err=""
LIB="$LOG/libgame-objs.a"; rm -f "$LIB"
find "$B/CMakeFiles/game.dir" -name '*.o' 2>/dev/null | sort | xargs -r ar rcs "$LIB"
if [ -f "$LIB" ]; then
  gen gen-decls python3 scripts/port-gen-decls.py "$LIB" || err="$err decls"
  gen gen-binds python3 scripts/port-gen-binds.py --lib "$LIB" || err="$err binds"
  gen gen-uso python3 scripts/port-gen-uso.py --lib "$LIB" || err="$err uso"
else err=" (no game objects)"; fi
gen gen-blob-types python3 scripts/port-gen-blob-types.py || err="$err blob-types"
st=$(gendiff); n=$(printf '%s' "$st" | grep -c .)
if [ -n "$err" ]; then line gen-lib FAIL "generator failed:$err (see $LOG)"
elif [ "$n" != 0 ]; then line gen-lib FAIL "$n stale file(s) vs committed: $(echo $st | cut -c1-200)$([ $REGEN = 1 ] && echo ' (regenerated)')"
else line gen-lib PASS "decls_cpp.h, binds, aliases.ld, uso_modules.c, blob_types.tsv up to date"; fi
if [ $REGEN = 0 ]; then restore
elif [ "$n" != 0 ]; then echo "           (--regen: rebuilding from the regenerated port/gen)"; start; build; brc=$?; TB=$(( $(date +%s) - T0 )); fi

# ---- build checks (from the build log)
if [ $brc != 2 ]; then
  T0=$(( $(date +%s) - TB ))
  nerr=$(grep -E '(error|Error):' "$LOG/build.log" | grep -vcE 'ld returned|ninja: build stopped|^FAILED')
  grep -o "undefined reference to \`[^']*'" "$LOG/build.log" | sort -u > "$LOG/undefined.txt"; nund=$(wc -l < "$LOG/undefined.txt")
  WB=port/tools/port-gate-warnings.tsv
  wres=$(python3 - "$LOG/build.log" "$WB" "$UPDATE" "$WT/" port/tools/return-type-allow.tsv port/sourcemap.tsv <<'PY'
import collections, re, sys
log, base, update, root = sys.argv[1], sys.argv[2], sys.argv[3] == '1', sys.argv[4]
allowf, smapf = sys.argv[5], sys.argv[6]
# return-type hits (run 842): never baselined.  A function that falls off its end returns MIPS $v0 in
# the ROM but garbage eax on i386 (and g++ may emit no `ret` at all): each hit in a shipped body is
# fixed, or reviewed onto return-type-allow.tsv (file, function, message prefix, verdict, evidence).
RET = ('-Wreturn-type', '-Wreturn-mismatch')
ships = collections.defaultdict(set)
for ln in open(smapf):
    p = ln.rstrip('\n').split('\t')
    if len(p) > 2 and not ln.startswith('#') and p[0] != 'function': ships[p[0]].add(p[2])
allow = []
for ln in open(allowf):
    if ln.startswith('#') or not ln.strip(): continue
    p = ln.rstrip('\n').split('\t'); allow.append(p[:3])
rhits = []; pend = None
# gated classes: an ABI or a missing-declaration / return-value problem on i386
CLS = ('-Wreturn-type', '-Wimplicit-function-declaration', '-Wimplicit-int', '-Wint-to-pointer-cast',
       '-Wpointer-to-int-cast', '-Wincompatible-pointer-types', '-Wint-conversion',
       '-Wbuiltin-declaration-mismatch', '-Wreturn-mismatch', '-fpermissive')
ctx = {}; got = collections.Counter()
for ln in open(log, errors='replace'):
    if pend is not None:  # "return type defaults to int" comes before the "In function" line: name it from the source echo
        m = re.match(r'\s*\d+ \| (?:\w+\s+)*?\**(\w+)\s*\(', ln)
        pend[1] = m.group(1) if m else '-'; rhits.append(tuple(pend)); pend = None; continue
    m = re.match(r"(\S+?): In (?:member )?function [‘'](.*)[’']:", ln)
    if m: ctx[m.group(1)] = m.group(2); continue
    m = re.match(r'(\S+?):\d+:\d+: warning: (.*) \[(-[\w=-]+)\]$', ln.rstrip())
    if not m or m.group(3) not in CLS: continue
    f = m.group(1).replace(root, ''); f = re.sub(r'^.*?/(src|port|include)/', r'\1/', f)
    if m.group(3) in RET:
        if 'defaults to' in m.group(2): pend = [f, None, m.group(2)]
        else: rhits.append((f, ctx.get(m.group(1), '-'), m.group(2)))
        continue
    got['\t'.join((f, ctx.get(m.group(1), '-'), m.group(3), m.group(2)))] += 1
rhits = sorted(set(rhits))
def allowed(h):
    return any(h[0] == a[0] and h[1] == a[1] and h[2].startswith(a[2]) for a in allow)
rdrop = [h for h in rhits if ships.get(h[1]) and h[0] not in ships[h[1]]]
rnew = [h for h in rhits if h not in rdrop and not allowed(h)]
rok = len(rhits) - len(rdrop) - len(rnew)
stale = [a for a in allow if not any(h[0] == a[0] and h[1] == a[1] and h[2].startswith(a[2]) for h in rhits)]
rtxt = 'return-type: %d hits (%d reviewed, %d dropped copies, %d UNREVIEWED%s)' % (
    len(rhits), rok, len(rdrop), len(rnew), ', %d stale allow-list lines' % len(stale) if stale else '')
if update:
    with open(base, 'w') as fh:
        fh.write('# scripts/port-gate.sh --update-baselines: accepted port build warnings of the gated classes\n'
                 '# (file, function, class, message, count).  A new key, or a higher count, fails the gate.\n')
        for k in sorted(got): fh.write('%s\t%d\n' % (k, got[k]))
    with open(log + '.new-warnings', 'w') as fh:
        for h in rnew: fh.write('RETURN\t%s\n' % '\t'.join(h))
    print('%d baseline written (%d keys, %d warnings); %s' % (len(rnew), len(got), sum(got.values()), rtxt)); sys.exit()
acc = collections.Counter()
try:
    for ln in open(base):
        if ln.startswith('#') or not ln.strip(): continue
        k, c = ln.rstrip('\n').rsplit('\t', 1); acc[k] = int(c)
except FileNotFoundError: pass
new = [(k, c - acc[k]) for k, c in sorted(got.items()) if c > acc[k]]
with open(log + '.new-warnings', 'w') as fh:
    for h in rnew: fh.write('RETURN\t%s\n' % '\t'.join(h))
    for k, c in new: fh.write('%d\t%s\n' % (c, k))
    for a in stale: fh.write('STALE-ALLOW\t%s\n' % '\t'.join(a))
print('%d %d gated warnings, %d new; %s' % (len(new) + len(rnew), sum(got.values()), sum(c for _, c in new), rtxt))
PY
)
  nnew=${wres%% *}
  txt="compile errors $nerr, undefined symbols $nund, ${wres#* }"
  if [ "$brc" != 0 ] || [ "$nerr" != 0 ] || [ "$nund" != 0 ] || [ "$nnew" != 0 ]; then
    line build FAIL "$txt (ninja rc $brc; $LOG/build.log)"
    head -5 "$LOG/undefined.txt" | sed 's/^/             /'
    [ "$nnew" != 0 ] && head -8 "$LOG/build.log.new-warnings" | cut -c1-200 | sed 's/^/             NEW /'
    [ "$nerr" != 0 ] && grep -E 'error:' "$LOG/build.log" | grep -v 'ld returned' | head -5 | cut -c1-200 | sed 's/^/             /'
  else line build PASS "$txt"; fi
fi

# ---- host tests
have() { [ -x "$B/$1" ] || { line "$1" FAIL "not built"; return 1; }; }
# test_ultra's argument is an SRAM image it WRITES (32 KB): never pass it the ROM
start; if have test_ultra; then
  rm -f "$LOG/test.sram"; "$RUN32" "$B/test_ultra" "$LOG/test.sram" >"$LOG/test_ultra.log" 2>&1; rc=$?
  [ $rc = 0 ] && line test_ultra PASS "$(tail -1 "$LOG/test_ultra.log")" || line test_ultra FAIL "rc $rc: $(tail -1 "$LOG/test_ultra.log")"; fi
start; if have test_fs; then
  "$RUN32" "$B/test_fs" "$ROM" >"$LOG/test_fs.log" 2>&1; rc=$?
  [ $rc = 0 ] && line test_fs PASS "$(tail -1 "$LOG/test_fs.log")" || line test_fs FAIL "rc $rc: $(grep -m1 FAIL "$LOG/test_fs.log")"; fi
start; rm -rf "$LOG/gfx"; mkdir -p "$LOG/gfx"
make -C port/gfx test OUT="$LOG/gfx" >"$LOG/gfx.log" 2>&1; rc=$?
g=$(grep -E '^golden: [0-9]+ scenes|golden: SKIP' "$LOG/gfx.log" | tail -1)
[ $rc = 0 ] && line gfx PASS "${g:-golden ?}; $(grep -m1 '^test_dlump' "$LOG/gfx.log")" || line gfx FAIL "rc $rc: $(grep -iE 'fail|differ|error' "$LOG/gfx.log" | head -1 | cut -c1-150)"
start; if have seqrender; then
  make -C port/audio regress SEQRENDER="$B/seqrender" RUN32="$RUN32" ROM="$ROM" >"$LOG/audio.log" 2>&1; rc=$?
  nc=$(grep -cE ' (PASS|FAIL)' "$LOG/audio.log")
  if [ $rc = 0 ]; then line audio PASS "$nc cases"
  else line audio FAIL "$(grep -m1 'case(s) failed' "$LOG/audio.log"): $(grep -E ' FAIL' "$LOG/audio.log" | awk '{print $1}' | tr '\n' ' ')"; fi
fi

# ---- smoke: the game itself, headless, from a clean directory
start; if have tenshoe; then
  S="$LOG/smoke"; rm -rf "$S"; mkdir -p "$S"
  ( cd "$S" && timeout 300 "$RUN32" "$B/tenshoe" --rom "$ROM" --sram "$S/1080.sram" --frames "$FRAMES" \
      --trace "$S/state.log" >"$S/out.log" 2>&1 ); rc=$?
  nf=$(grep -c '^F ' "$S/state.log" 2>/dev/null); nf=${nf:-0}
  # NaN / Inf: an f32 bit pattern with an all-ones exponent; r<N>= (pos, quat, speed) gated, cam= counted
  nan=$(python3 - "$S/state.log" <<'PY'
import re, sys
bad = {'r': [0, ''], 'cam': [0, '']}
for ln in open(sys.argv[1], errors='replace'):
    for m in re.finditer(r' (cam|r\d+)=([0-9a-f,]+)', ln):
        k = 'cam' if m.group(1) == 'cam' else 'r'
        for w in m.group(2).split(','):
            if len(w) == 8 and (int(w, 16) >> 23) & 0xFF == 0xFF:
                bad[k][0] += 1; bad[k][1] = bad[k][1] or 'from frame ' + ln.split()[1]
print(bad['r'][0], 'NaN/Inf rider fields %d%s, camera fields %d%s' % (bad['r'][0], bad['r'][1] and ' ' + bad['r'][1],
      bad['cam'][0], bad['cam'][1] and ' ' + bad['cam'][1]))
PY
)
  why=$(grep -m1 -E 'sys_fatal|SIG|DEADLOCK|PORT TRAP' "$S/out.log" | cut -c1-100)
  txt="$FRAMES frames: rc $rc, state.log $nf frames, ${nan#* }"
  if [ $rc = 0 ] && [ "$nf" -ge "$FRAMES" ] && [ "${nan%% *}" = 0 ]; then line smoke PASS "$txt"
  else line smoke FAIL "$txt${why:+; $why}"; fi
fi

# ---- saves (plan 20): the SRAM file is the N64 image (port/include/port_backup.h).  test_backup's unit
# checks; a blank-SRAM boot whose save must equal the ROM defaults' big-endian image (test_backup builds
# it from the extracted bootup Data, hardware checksums); reloading that save and a synthetic .sra
# (non-default records + a ghost) must leave each file unchanged with one SRAM write (the header: no
# chunk reset); damaged primaries must be restored from the mirrors.  Files: $LOG/saves.XXXXXX.
start; if have test_backup && have tenshoe; then
  V=$(mktemp -d "$LOG/saves.XXXXXX"); err=""  # a fresh dir per run (no rm of a variable path)
  boot() {  # boot <sram> <tag>: 30 frames; echoes the SRAM write count
    ( cd "$V" && timeout 120 "$RUN32" "$B/tenshoe" --rom "$ROM" --sram "$1" --frames 30 >"$V/$2.log" 2>&1 )
    grep -oE '[0-9]+ SRAM writes' "$V/$2.log" | cut -d' ' -f1; }
  "$RUN32" "$B/test_backup" >"$V/unit.log" 2>&1 || err="$err unit($(grep -m1 FAIL "$V/unit.log"))"
  w=$(boot "$V/fresh.sram" fresh)
  "$RUN32" "$B/test_backup" "$V/fresh.sram" "$B/extracted/uso/bootup.uso/02_Data.bin" "$V/synth.sra" \
    >"$V/defaults.log" 2>&1 || err="$err defaults($(grep -m1 FAIL "$V/defaults.log" | cut -c1-90))"
  for f in fresh.sram synth.sra; do
    [ -f "$V/$f" ] || continue
    cp "$V/$f" "$V/reload-$f"; w=$(boot "$V/reload-$f" "reload-$f")
    { [ "$w" = 1 ] && cmp -s "$V/$f" "$V/reload-$f"; } || err="$err reload-$f(${w:-?} writes)"
  done
  if [ -f "$V/synth.sra" ]; then  # one byte of the course and the options primaries
    cp "$V/synth.sra" "$V/damaged.sram"
    printf '\167' | dd of="$V/damaged.sram" bs=1 seek=$((0x20)) conv=notrunc 2>/dev/null
    printf '\167' | dd of="$V/damaged.sram" bs=1 seek=$((0x1FB)) conv=notrunc 2>/dev/null
    w=$(boot "$V/damaged.sram" damaged)
    cmp -s "$V/synth.sra" "$V/damaged.sram" || err="$err mirror-restore(${w:-?} writes)"
  fi
  if [ -z "$err" ]; then line saves PASS "$(tail -1 "$V/defaults.log" | cut -d' ' -f2-4); fresh = ROM defaults' N64 image; reload / .sra / mirror restore OK"
  else line saves FAIL "$err"; fi
fi

# ---- abi: DWARF call sites vs definitions in the linked tenshoe
start; if have tenshoe; then
  if [ $UPDATE = 1 ]; then python3 port/tools/abi-scan.py "$B/tenshoe" --baseline port/tools/abi-baseline.tsv --update >"$LOG/abi.log" 2>&1
  else python3 port/tools/abi-scan.py "$B/tenshoe" --baseline port/tools/abi-baseline.tsv >"$LOG/abi.log" 2>&1; fi; rc=$?
  [ $rc = 0 ] && line abi PASS "$(tail -1 "$LOG/abi.log")" || { line abi FAIL "$(tail -1 "$LOG/abi.log")"; grep '^NEW' "$LOG/abi.log" | head -5 | cut -c1-200 | sed 's/^/             /'; }
fi

[ $REGEN = 1 ] && [ -n "$(git status --short -- port/gen port/sourcemap.tsv port/sourcemap-placeholders.tsv)" ] && echo "   --regen: port/gen / sourcemap changed in $WT (commit as tool output)"
[ $UPDATE = 1 ] && echo "   --update-baselines: review git diff port/tools/port-gate-warnings.tsv port/tools/abi-baseline.tsv"
if [ $FAILS = 0 ]; then echo "== port gate: PASS"; else echo "== port gate: FAIL ($FAILS step(s))"; fi
exit $FAILS
