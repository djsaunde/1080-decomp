#!/usr/bin/env bash
# Play the PC port live in a window (PC port plan 19.11): the 32-bit game (tenshoe, PORT_LIVE)
# and the 64-bit presenter (port/gfx/fast3d/presenter.cpp) tied by one shared-memory name.
#
#   port/tools/play.sh [-b BUILD] [-r ROM] [-s SCALE] [-S] [-m] [-l MS] [-n NAME] [-p "PRESENTER ARGS"] [-- TENSHOE ARGS]
#   port/tools/play.sh --native [-b BUILD] [-r ROM] [-s SCALE] [-S] [-m] [-l MS] [-V] [-- TENSHOE ARGS]
#
# --native (plan 21.9): the 64-bit game alone (-DPORT_64=ON -DPORT_NATIVE=ON, run with PORT_NATIVE=1): it
#        renders with Fast3D in process, opens its own window, plays its own audio and reads the keyboard /
#        gamepad itself; no presenter, no shared memory, no i386 sysroot. -b is then a PORT_NATIVE build tree
#        (default <monorepo>/tools/play/<worktree>/build-native); -V turns vsync on (default off: the game's
#        30 Hz step paces the frames); -n / -p do not apply. More settings through the environment
#        (port/gfx/fast3d/native_host.cpp: PORT_NATIVE_PNG, PORT_NATIVE_INJECT, PORT_NATIVE_JS, ...).
#
#   -b   a built PORT_LINK=ON port tree holding tenshoe (default: configured and built on first
#        use in <monorepo>/tools/play/<worktree>/build, about 3 minutes)
#   -r   the ROM (default <game repo>/baserom.z64)
#   -s   window scale over 320x240 (default 3: 960x720)
#   -S   software GL (LIBGL_ALWAYS_SOFTWARE=1, llvmpipe) instead of the GPU: the fallback when the GPU
#        path fails (no EGL config, "D3D12: Removing Device", a black or frozen window); slower
#   -m   mute (no sound; the game's audio is still produced and drained)
#   -l   audio buffer in ms (default 80; raise it if the sound crackles)
#   -n   the shared-memory name (default 1080live-<pid>)
#   -p   more presenter arguments (--png DIR --png-every N, --inject STEP:KEY:d|u,..., --frames N, --stay,
#        --audio-dump FILE, --audio-stats, --input SCRIPT: pads from a port/host/*.input script, in lockstep)
#   --   more tenshoe arguments (--frames N, --input SCRIPT, --record LOG, --trace LOG, --audio-out WAV, ...)
#
# The save (1080.sram) is kept in <monorepo>/tools/play/<worktree>/run between sessions.
# Keys: arrows / WASD stick (Shift = half), X or Space = A, Z = B, C = Z, Enter = START, Q = L,
# E = R, I / J / K / L = C buttons, T / F / G / H = D-pad, F11 = fullscreen (the window also resizes;
# the frame is letterboxed at 4:3), Esc = quit. A Linux joystick
# (/dev/input/js0, else the first evdev gamepad /dev/input/event*) is used when present. Needs an
# X display (WSLg: DISPLAY=:0). Sound goes to PulseAudio ($PULSE_SERVER; WSLg's
# unix:/mnt/wslg/PulseServer); without a server the game plays silent.
# One-time setup (no sudo): scripts/setup-i386-sysroot.sh (monorepo), port/gfx/fast3d/setup-lus.sh,
# port/gfx/replay/setup-gl-dev.sh, port/gfx/fast3d/setup-x11-dev.sh, port/gfx/fast3d/setup-pulse-dev.sh;
# this script runs the last four itself when their tools/ directories are missing (the audio one
# may fail: the game then runs silent). Sound opens 2.5 s after the window (WSLg: a window mapped while
# a stream plays stalls its PulseAudio; so do other windows opened during play). Check: audio-check.sh.
set -u
HERE="$(cd "$(dirname "$0")" && pwd)"
GAME="$(cd "$HERE/../.." && pwd)"
MONO="$(cd "$GAME/../.." && pwd)"
WORK="$MONO/tools/play/$(basename "$GAME")"
BUILD= ROM="$GAME/baserom.z64" SCALE=3 SOFT=0 NAME="1080live-$$" PARGS= MUTE= LAT=80 NATIVE=0 VSYNC=0
if [ "${1:-}" = --native ]; then NATIVE=1; shift; fi
while getopts "b:r:s:Sml:n:p:Vh" o; do
    case $o in
    b) BUILD=$OPTARG ;; r) ROM=$OPTARG ;; s) SCALE=$OPTARG ;; S) SOFT=1 ;; n) NAME=$OPTARG ;; p) PARGS=$OPTARG ;;
    m) MUTE=--mute ;; l) LAT=$OPTARG ;; V) VSYNC=1 ;;
    *) sed -n '2,39p' "$0"; exit 2 ;;
    esac
done
shift $((OPTIND - 1))
[ -f "$ROM" ] || { echo "play: no ROM at $ROM"; exit 2; }
ROM="$(cd "$(dirname "$ROM")" && pwd)/$(basename "$ROM")"
[ -n "${DISPLAY:-}" ] || { echo "play: DISPLAY is not set: an X server is needed (WSL2: WSLg provides one; export DISPLAY=:0)"; exit 2; }
mkdir -p "$WORK/run"

if [ $NATIVE = 1 ]; then
    for s in "$MONO/tools/lus:$GAME/port/gfx/fast3d/setup-lus.sh" "$MONO/tools/gl-dev:$GAME/port/gfx/replay/setup-gl-dev.sh" \
             "$MONO/tools/x11-dev:$GAME/port/gfx/fast3d/setup-x11-dev.sh"; do
        [ -f "${s%%:*}/.complete" ] || "${s#*:}" || { echo "play: ${s#*:} failed"; exit 1; }
    done
    PULSE="$MONO/tools/pulse-dev"
    [ -f "$PULSE/.complete" ] || "$GAME/port/gfx/fast3d/setup-pulse-dev.sh" >"$WORK/setup-pulse.log" 2>&1 ||
        echo "play: setup-pulse-dev.sh failed (see $WORK/setup-pulse.log): no sound"
    if [ -z "$BUILD" ]; then
        BUILD="$WORK/build-native"
        export PATH="$MONO/tools/cmake-venv/bin:$PATH"
        if [ ! -f "$BUILD/build.ninja" ]; then
            echo "play: configuring the native 64-bit port in $BUILD"
            cmake -S "$GAME/port" -B "$BUILD" -G Ninja -DPORT_64=ON -DPORT_NATIVE=ON -DPORT_LINK=ON -DPORT_ROM="$ROM" \
                  >"$WORK/configure-native.log" 2>&1 || { tail -5 "$WORK/configure-native.log"; exit 1; }
        fi
        echo "play: building the native tenshoe (first time: a few minutes)"
        cmake --build "$BUILD" --target tenshoe -j8 >"$WORK/build-native.log" 2>&1 ||
            { tail -20 "$WORK/build-native.log"; echo "play: build failed"; exit 1; }
    fi
    [ -x "$BUILD/tenshoe" ] || { echo "play: no tenshoe in $BUILD"; exit 2; }
    [ $SOFT = 1 ] && export LIBGL_ALWAYS_SOFTWARE=1
    PLIB="$PULSE/root/usr/lib/x86_64-linux-gnu"
    echo "play: native 64-bit game (PORT_NATIVE=1)"
    cd "$WORK/run" && LD_LIBRARY_PATH="$PLIB:$PLIB/pulseaudio${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}" \
        PORT_NATIVE=1 PORT_NATIVE_SCALE="$SCALE" PORT_NATIVE_LATENCY="$LAT" PORT_NATIVE_MUTE="${MUTE:+1}" \
        PORT_NATIVE_VSYNC="$VSYNC" "$BUILD/tenshoe" --rom "$ROM" --sram "$WORK/run/1080.sram" --realtime "$@"
    rc=$?
    echo "play: game exited $rc"
    exit $rc
fi

RUN32="$MONO/tools/i386-sysroot/run32"
[ -x "$RUN32" ] || { echo "play: no i386 sysroot: run $MONO/scripts/setup-i386-sysroot.sh"; exit 2; }

# the presenter (64-bit)
for s in "$MONO/tools/lus:$GAME/port/gfx/fast3d/setup-lus.sh" "$MONO/tools/gl-dev:$GAME/port/gfx/replay/setup-gl-dev.sh" \
         "$MONO/tools/x11-dev:$GAME/port/gfx/fast3d/setup-x11-dev.sh"; do
    [ -f "${s%%:*}/.complete" ] || "${s#*:}" || { echo "play: ${s#*:} failed"; exit 1; }
done
PULSE="$MONO/tools/pulse-dev"
[ -f "$PULSE/.complete" ] || "$GAME/port/gfx/fast3d/setup-pulse-dev.sh" >"$WORK/setup-pulse.log" 2>&1 ||
    echo "play: setup-pulse-dev.sh failed (see $WORK/setup-pulse.log): no sound"
make -s -C "$GAME/port/gfx/fast3d" OUT="$WORK/gfx" "$WORK/gfx/presenter" -j8 >"$WORK/presenter-build.log" 2>&1 ||
    { tail -20 "$WORK/presenter-build.log"; echo "play: presenter build failed"; exit 1; }

# the game (32-bit)
if [ -z "$BUILD" ]; then
    BUILD="$WORK/build"
    export PATH="$MONO/tools/cmake-venv/bin:$PATH"
    if [ ! -f "$BUILD/build.ninja" ]; then
        echo "play: configuring the port in $BUILD"
        cmake -S "$GAME/port" -B "$BUILD" -G Ninja -DCMAKE_TOOLCHAIN_FILE="$MONO/tools/i386-sysroot/toolchain-i386.cmake" \
              -DPORT_LINK=ON -DPORT_ROM="$ROM" >"$WORK/configure.log" 2>&1 || { tail -5 "$WORK/configure.log"; exit 1; }
    fi
    echo "play: building tenshoe (first time: a few minutes)"
    cmake --build "$BUILD" --target tenshoe -j8 >"$WORK/build.log" 2>&1 || { tail -20 "$WORK/build.log"; echo "play: build failed"; exit 1; }
fi
[ -x "$BUILD/tenshoe" ] || { echo "play: no tenshoe in $BUILD"; exit 2; }

[ $SOFT = 1 ] && export LIBGL_ALWAYS_SOFTWARE=1
# libpulse-simple and its dependencies from tools/pulse-dev (dlopen'd by the presenter only)
PLIB="$PULSE/root/usr/lib/x86_64-linux-gnu"
# shellcheck disable=SC2086  # PARGS / MUTE are split on purpose
LD_LIBRARY_PATH="$PLIB:$PLIB/pulseaudio${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}" \
    "$WORK/gfx/presenter" --shm "$NAME" --scale "$SCALE" --audio-latency "$LAT" $MUTE $PARGS &
PRES=$!
trap 'kill $PRES 2>/dev/null' INT TERM
# the presenter makes /dev/shm/$NAME only once its window is open: if it dies first (no display,
# no GL), say so instead of starting a game that waits 10 s for it
for _ in $(seq 100); do
    [ -e "/dev/shm/$NAME" ] && break
    kill -0 $PRES 2>/dev/null || { wait $PRES; echo "play: the presenter could not start (exit $?; its message is above)"; exit 1; }
    sleep 0.1
done
echo "play: presenter pid $PRES; starting the game (PORT_LIVE=$NAME)"
(cd "$WORK/run" && PORT_LIVE="$NAME" "$RUN32" "$BUILD/tenshoe" --rom "$ROM" --sram "$WORK/run/1080.sram" --realtime "$@")
rc=$?
# the presenter exits when it sees the game end; a crashed game cannot say so
for _ in 1 2 3 4 5 6; do kill -0 $PRES 2>/dev/null || break; sleep 0.5; done
kill $PRES 2>/dev/null
wait $PRES
echo "play: game exited $rc"
exit $rc
