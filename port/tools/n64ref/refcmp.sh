#!/usr/bin/env bash
# Port vs N64 reference over the same steps (PC port plan 22): the native 64-bit game (PORT_NATIVE_PNG)
# and n64ref (the ROM on mupen64plus + angrylion, the framebuffer of each step) run one input script,
# then compare.py scores every step and writes the side-by-side sheet of the worst ones.
#
#   port/tools/n64ref/refcmp.sh -b <native build dir> -o OUT -n STEPS [-i SCRIPT] [-s A-B] [-k K] [-t T]
#
#   -b   a -DPORT_64=ON -DPORT_NATIVE=ON -DPORT_LINK=ON build (holds tenshoe; play.sh builds one)
#   -o   output directory (new): OUT/port, OUT/n64, OUT/score.txt, OUT/worst.png, OUT/spans.txt
#   -n   steps to run (--frames)
#   -i   a port/host/*.input script (both sides read it; steps are SysLib.frameCount)
#   -s   the steps to capture (default 0-N)
#   -k   the sheet's worst-K steps (default 16), at least 8 steps apart
#   -t   the span threshold for spans.txt (mean |diff| of the blurred 160x120 frames; default 10)
# Needs DISPLAY (the native window; WSLg: :0), n64ref built (build.sh), uv (Pillow + numpy for compare.py).
# The ROM is <game>/baserom.z64; only read. Both sides start from a blank SRAM.
set -u
HERE="$(cd "$(dirname "$0")" && pwd)"
GAME="$(cd "$HERE/../../.." && pwd)"
MONO="$(cd "$GAME/../.." && pwd)"
BUILD= OUT= N= INPUT= STEPS= K=16 T=10
while getopts "b:o:n:i:s:k:t:h" o; do
    case $o in
    b) BUILD=$OPTARG ;; o) OUT=$OPTARG ;; n) N=$OPTARG ;; i) INPUT=$OPTARG ;; s) STEPS=$OPTARG ;;
    k) K=$OPTARG ;; t) T=$OPTARG ;; *) sed -n '2,17p' "$0"; exit 2 ;;
    esac
done
[ -n "$BUILD" ] && [ -x "$BUILD/tenshoe" ] && [ -n "$OUT" ] && [ -n "$N" ] || { sed -n '2,17p' "$0"; exit 2; }
[ -e "$OUT/port" ] && { echo "refcmp: $OUT already holds a run"; exit 2; }
STEPS=${STEPS:-0-$N}
ROM="$GAME/baserom.z64"
N64REF="$MONO/tools/n64emu/bin/n64ref"
[ -x "$N64REF" ] || { echo "refcmp: no $N64REF (port/tools/n64ref/build.sh)"; exit 2; }
mkdir -p "$OUT/port" "$OUT/n64"
OUT="$(cd "$OUT" && pwd)"
IN=()
[ -n "$INPUT" ] && IN=(--input "$(cd "$(dirname "$INPUT")" && pwd)/$(basename "$INPUT")")
PL="$MONO/tools/pulse-dev/root/usr/lib/x86_64-linux-gnu"
echo "refcmp: port (native) $N steps ${INPUT:+, $INPUT}"
(cd "$OUT/port" && DISPLAY=${DISPLAY:-:0} LD_LIBRARY_PATH="$PL:$PL/pulseaudio" PORT_NATIVE=1 PORT_NATIVE_MUTE=1 \
    PORT_NATIVE_SCALE=1 PORT_NATIVE_PNG="$OUT/port" PORT_NATIVE_PNG_STEPS="$STEPS" \
    timeout 550 "$BUILD/tenshoe" --rom "$ROM" --frames "$N" "${IN[@]}" >run.log 2>&1)
echo "refcmp: port exit $? ($(tail -1 "$OUT/port/run.log"))"
echo "refcmp: n64ref"
(cd "$OUT/n64" && timeout 590 "$N64REF" --rom "$ROM" --out "$OUT/n64" --steps "$STEPS" --fb --log-steps "${IN[@]}" \
    >run.log 2>&1)
echo "refcmp: n64ref exit $? ($(grep -v '^core' "$OUT/n64/run.log" | tail -1))"
CMP=(uv run -q --with pillow --with numpy python "$HERE/compare.py")
"${CMP[@]}" score "$OUT/port" "$OUT/n64" "$STEPS" >"$OUT/score.txt"
awk -v t="$T" '{d=$4+0; s=$2+0; if (d > t) { if (s != p + 1) { if (n) print a "-" p " max " m; a = s; m = 0; n = 1 } if (d > m) m = d; p = s } }
     END { if (n) print a "-" p " max " m }' "$OUT/score.txt" >"$OUT/spans.txt"
worst=$(sort -k4 -rn "$OUT/score.txt" | awk -v k="$K" '{s=$2+0; ok=1; for (i in got) if (s - i < 8 && i - s < 8) ok=0;
    if (ok) { got[s]=1; out = out (out ? "," : "") s; if (++n == k) exit } } END { print out }')
[ -n "$worst" ] && "${CMP[@]}" sheet "$OUT/worst.png" "$OUT/port" "$OUT/n64" "$worst" --cols 2 \
    --title "port (left) vs N64 (right): worst $K steps of $STEPS${INPUT:+, $(basename "$INPUT")}"
echo "refcmp: $(wc -l <"$OUT/score.txt") steps scored; median diff $(awk '{print $4}' "$OUT/score.txt" | sort -n |
    awk '{a[NR]=$1} END {print a[int((NR+1)/2)]}'); spans over $T: $(wc -l <"$OUT/spans.txt") ($OUT/spans.txt)"
