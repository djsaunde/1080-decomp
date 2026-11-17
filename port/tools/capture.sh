#!/usr/bin/env bash
# One-command capture of real frames (PC port plan 19.8): run tenshoe (i386, through run32)
# for N frames with PORT_GFX_DUMP, render every dump with f3dreplay (64-bit, llvmpipe),
# then build a contact sheet and the dlump.py --total command histogram.
#
#   port/tools/capture.sh -b <port build dir> [-o OUT] [-n FRAMES] [-e EVERY] [-f FIRST]
#                         [-r ROM] [-c COLS] [-s SHRINK] [-S RENDER_SCALE] [-x F3DREPLAY]
#                         [-i INPUT] [-F STEPS] [-a "EXTRA TENSHOE ARGS"]
#
#   -b   a configured + built PORT_LINK=ON build (holds tenshoe; plan 13.5)
#   -o   output directory (default ./capture-<date>-<time>); must be new or hold no earlier
#        capture (the script never deletes anything)
#   -n   --frames for tenshoe (default 300)
#   -e   PORT_GFX_DUMP_EVERY (default 1); -f PORT_GFX_DUMP_FIRST (default 0)
#   -r   the ROM (default <game repo>/baserom.z64)
#   -c   contact sheet columns (default 10); -s tile shrink (default 2: 160x120 tiles)
#   -S   f3dreplay --scale (default 1, native 320x240; >1 shows upscaling artifacts)
#   -x   an f3dreplay binary (default: built into OUT/gfx from port/gfx/fast3d)
#   -i   an input script for tenshoe --input (scripted pads; format in port/host/input_script.c)
#   -F   dump only the frames of these steps (PORT_GFX_DUMP_STEPS, e.g. "700,750-752"; replaces
#        -e / -f); tiles are then labelled with the step, not the task number
#   -a   more tenshoe arguments (e.g. "--record in.log"); tenshoe always writes OUT/state.log
#
# Output in OUT: run.log (tenshoe's stdout / stderr and exit code; a crash is reported, the
# frames before it are still processed), state.log (--trace), dump/gfx_NNNNNN.dlmp + dump/steps.txt
# (task step), png/tNNNNNN_sSTEP.png (NNNNNN = the gfx task number since boot, STEP = the
# step / SysLib.frameCount it ran in), sheet.png, histogram.txt, replay.log.
# tenshoe runs in OUT, so its 1080.sram starts fresh each capture.
set -u
HERE="$(cd "$(dirname "$0")" && pwd)"
GAME="$(cd "$HERE/../.." && pwd)"
MONO="$(cd "$GAME/../.." && pwd)"
BUILD= OUT=./capture-$(date +%Y%m%d-%H%M%S) FRAMES=300 EVERY=1 FIRST=0 ROM="$GAME/baserom.z64" COLS=10 SCALE=2 RSCALE=1 F3D=
INPUT= STEPS= EXTRA=
while getopts "b:o:n:e:f:r:c:s:S:x:i:F:a:h" o; do
    case $o in
    b) BUILD=$OPTARG ;; o) OUT=$OPTARG ;; n) FRAMES=$OPTARG ;; e) EVERY=$OPTARG ;;
    f) FIRST=$OPTARG ;; r) ROM=$OPTARG ;; c) COLS=$OPTARG ;; s) SCALE=$OPTARG ;; S) RSCALE=$OPTARG ;; x) F3D=$OPTARG ;;
    i) INPUT=$OPTARG ;; F) STEPS=$OPTARG ;; a) EXTRA=$OPTARG ;;
    *) sed -n '2,28p' "$0"; exit 2 ;;
    esac
done
[ -n "$BUILD" ] && [ -x "$BUILD/tenshoe" ] || { echo "capture: -b <build dir with tenshoe> is required"; exit 2; }
BUILD="$(cd "$BUILD" && pwd)"
ROM="$(cd "$(dirname "$ROM")" && pwd)/$(basename "$ROM")"
[ -f "$ROM" ] || { echo "capture: no ROM at $ROM"; exit 2; }
mkdir -p "$OUT" && OUT="$(cd "$OUT" && pwd)"
INARGS=()
if [ -n "$INPUT" ]; then
    [ -f "$INPUT" ] || { echo "capture: no input script $INPUT"; exit 2; }
    INARGS=(--input "$(cd "$(dirname "$INPUT")" && pwd)/$(basename "$INPUT")")
fi
# nothing is deleted (agents' rm rule, coord run 843): OUT must not hold an earlier capture
for f in dump png 1080.sram state.log; do
    [ -e "$OUT/$f" ] && { echo "capture: $OUT already holds a capture ($f): pass a fresh -o directory"; exit 2; }
done
mkdir -p "$OUT/png"

if [ -z "$F3D" ]; then
    make -s -C "$GAME/port/gfx/fast3d" OUT="$OUT/gfx" all -j8 >"$OUT/f3d-build.log" 2>&1 ||
        { tail -20 "$OUT/f3d-build.log"; echo "capture: f3dreplay build failed (fast3d/setup-lus.sh, replay/setup-gl-dev.sh?)"; exit 1; }
    F3D="$OUT/gfx/f3dreplay"
fi

echo "capture: tenshoe --frames $FRAMES ${INARGS[*]} $EXTRA, dumping ${STEPS:+steps $STEPS}${STEPS:-every $EVERY from $FIRST} -> $OUT/dump"
# shellcheck disable=SC2086  # EXTRA is split on purpose
(cd "$OUT" && PORT_GFX_DUMP="$OUT/dump" PORT_GFX_DUMP_EVERY="$EVERY" PORT_GFX_DUMP_FIRST="$FIRST" \
    PORT_GFX_DUMP_STEPS="$STEPS" "$MONO/tools/i386-sysroot/run32" "$BUILD/tenshoe" --rom "$ROM" \
    --frames "$FRAMES" --trace "$OUT/state.log" "${INARGS[@]}" $EXTRA >run.log 2>&1)
rc=$?
echo "exit $rc" >>"$OUT/run.log"
[ $rc -eq 0 ] || echo "capture: tenshoe exited $rc (see run.log; $(tail -2 "$OUT/run.log" | head -1))"

shopt -s nullglob
dumps=("$OUT"/dump/gfx_*.dlmp)
[ ${#dumps[@]} -gt 0 ] || { echo "capture: no dumps"; exit 1; }
export LIBGL_ALWAYS_SOFTWARE=1
: >"$OUT/replay.log"
bad=0
for d in "${dumps[@]}"; do
    n="$(basename "$d" .dlmp)"; n=${n#gfx_}
    png="$OUT/png/t$n.png"
    st=$(awk -v t="$n" '$1 == t { print $2; exit }' "$OUT/dump/steps.txt" 2>/dev/null)
    [ -n "$STEPS" ] && [ -n "$st" ] && png="$OUT/png/t${n}_s$st.png"  # the sheet labels the last number
    "$F3D" "$d" "$png" --scale "$RSCALE" >>"$OUT/replay.log" 2>&1 || { bad=$((bad + 1)); echo "FAILED $d" >>"$OUT/replay.log"; }
done
echo "capture: ${#dumps[@]} dumps rendered ($bad failed; replay.log)"
grep -v " 0 unresolved G_DL" "$OUT/replay.log" | grep -v "^replay: GL" | head -5

python3 "$HERE/contact_sheet.py" "$OUT/sheet.png" "$OUT"/png/t*.png --cols "$COLS" --scale "$SCALE"
python3 "$GAME/port/gfx/tools/dlump.py" --total "${dumps[@]}" >"$OUT/histogram.txt" 2>&1
echo "capture: histogram -> $OUT/histogram.txt"
exit 0
