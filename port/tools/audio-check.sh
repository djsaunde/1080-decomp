#!/usr/bin/env bash
# Live-audio check for the presenter (PC port plan 19.12 / run 867): play the Match Race script in a
# window, in lockstep (presenter --input), with PulseAudio on, and require that the presenter played
# at least MIN % of the audio it received while its output was open.
#
#   port/tools/audio-check.sh [--native] [-f STEPS] [-m MIN_PERCENT] [-o DIR] [-- MORE PLAY.SH ARGS]
#
#   --native (plan 21.10) checks the native 64-bit game (play.sh --native) instead: it reads the script
#        itself (tenshoe --input), opens its window, then PulseAudio 2.5 s later (live_audio openDelayMs),
#        and prints the same line (PORT_NATIVE_AUDIO_STATS=1; PORT_NATIVE_AUDIO_OPEN_DELAY=0 shows the stall)
#   -f   game steps (default 3300: the menus, then the race from step ~1500, about 60 s of racing)
#   -m   the pass mark, played / received-while-open in percent (default 95)
#   -o   where the logs and the run directory go (default a new mktemp dir; a fresh SRAM, as the
#        script needs: a saved one skips the language menu)
#   AUDIO_CHECK_PRESENTER  more presenter arguments (e.g. "--audio-open-delay 0" shows the WSLg stall)
#
# Needs what play.sh needs: an X display (WSLg), the PulseAudio server ($PULSE_SERVER / WSLg's), and
# the one-time setups play.sh lists; it builds the presenter and tenshoe through play.sh. It cannot
# run in the port gate (no window there), so run it by hand or with `make -C port/gfx/fast3d audio-check`.
# The verdict is the presenter's `audio-stats:` line (--audio-stats). Exit 0 = pass, 1 = below the
# mark, 2 = no verdict (the run failed, or PulseAudio was not reachable: output not "pulse").
# Note (WSLg, run 867): ANY X window mapped while a PulseAudio stream plays stalls WSLg's server for
# that window's lifetime, so do not open other windows during the check.
set -u
HERE="$(cd "$(dirname "$0")" && pwd)"
GAME="$(cd "$HERE/../.." && pwd)"
STEPS=3300 MIN=95 OUTDIR= NATIVE=0
if [ "${1:-}" = --native ]; then NATIVE=1; shift; fi
while getopts "f:m:o:h" o; do
    case $o in
    f) STEPS=$OPTARG ;; m) MIN=$OPTARG ;; o) OUTDIR=$OPTARG ;;
    *) awk 'NR > 1 && !/^#/ { exit } NR > 1' "$0"; exit 2 ;;
    esac
done
shift $((OPTIND - 1))
[ -n "$OUTDIR" ] || OUTDIR="$(mktemp -d /tmp/audio-check.XXXXXX)"
mkdir -p "$OUTDIR/run"
LOG="$OUTDIR/play.log"
if [ $NATIVE = 1 ]; then
    echo "audio-check: native: $STEPS steps of port/host/match_race.input, logs in $OUTDIR"
    PORT_NATIVE_AUDIO_STATS=1 "$HERE/play.sh" --native "$@" -- --input "$GAME/port/host/match_race.input" \
        --sram "$OUTDIR/run/1080.sram" --frames "$STEPS" >"$LOG" 2>&1
else
    echo "audio-check: $STEPS steps of port/host/match_race.input, logs in $OUTDIR"
    "$HERE/play.sh" --presenter "$@" -p "--audio-stats ${AUDIO_CHECK_PRESENTER:-} --input $GAME/port/host/match_race.input" -- \
        --sram "$OUTDIR/run/1080.sram" --frames "$STEPS" >"$LOG" 2>&1
fi
rc=$?
line="$(grep ': audio-stats:' "$LOG" | tail -1)"
if [ -z "$line" ]; then
    tail -20 "$LOG"
    echo "audio-check: no audio-stats line (play.sh exit $rc); see $LOG"
    exit 2
fi
echo "$line"
case "$line" in
*"output pulse,"*) ;;
*) echo "audio-check: the output was not PulseAudio: no verdict"; exit 2 ;;
esac
pct="$(echo "$line" | sed -n 's/.*played\/received-while-open \([0-9.]*\)%.*/\1/p')"
open="$(echo "$line" | sed -n 's/.*received \([0-9.]*\) s, before open \([0-9.]*\) s.*/\1 \2/p')"
if awk -v p="$pct" -v m="$MIN" -v o="$open" 'BEGIN { split(o, a, " "); exit !(p >= m && a[1] - a[2] >= 30) }'; then
    echo "audio-check: PASS ($pct% >= $MIN%)"
    exit 0
fi
echo "audio-check: FAIL ($pct% played, mark $MIN%, or under 30 s received while open); see $LOG"
exit 1
