#!/usr/bin/env bash
# port/android/linux-gles-check.sh (plan 31): the Android host's renderer and window code, run on Linux without a device.
#
#   port/android/linux-gles-check.sh -w <PORT_WASM build dir> -o <new scratch dir> [-f FRAMES] [-l LUS_TOOLS]
#
# Builds SDL2 (static, the offscreen video driver: EGL + Mesa llvmpipe, OpenGL ES 3.2) and two wasm2c hosts of the
# same w2c output: WASM_HOST_SDL_GLES (live_window_sdl.cpp + LUS USE_OPENGLES: the Android code path) and the
# desktop X11 / OpenGL one (needs $DISPLAY). Runs the attract demo in each for FRAMES steps (default 2000) with
# PNGs of the window at five steps: GLES with the extensions llvmpipe has, GLES with every fallback
# (FAST_GLES_NO_EXT=1: what a driver without them gets), desktop GL. Then a run with the gamepad menu chord
# (input.pad_menu: virtual gamepad Back + Start at step 500, D-pad down, A, B) to check that the menu opens and the
# game sees no START. Prints: state.log equality, the depth read-back answers, PNG differences.
set -euo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
GAME="$(cd "$HERE/../.." && pwd)"
REPO="$(cd "$GAME/../.." && pwd)"
BW=""; OUT=""; FRAMES=2000; LUS="${LUS_TOOLS:-$REPO/tools/lus}"
while getopts "w:o:f:l:" o; do
    case $o in w) BW="$OPTARG" ;; o) OUT="$OPTARG" ;; f) FRAMES="$OPTARG" ;; l) LUS="$OPTARG" ;; *) exit 2 ;; esac
done
[[ -f "$BW/w2c/game.h" && -n "$OUT" ]] || { echo "usage: $0 -w <PORT_WASM build dir> -o <scratch dir>" >&2; exit 2; }
ROM="${ROM:-$GAME/baserom.z64}"
SDL2_SRC="${SDL2_SRC:-$(ls -d "$REPO"/tools/android/src/SDL2-* | head -1)}"
mkdir -p "$OUT"
OUT="$(cd "$OUT" && pwd)"
bash "$GAME/port/gfx/replay/setup-gles-dev.sh" > /dev/null

if [[ ! -f "$OUT/sdl/lib/cmake/SDL2/SDL2Config.cmake" ]]; then
    cmake -S "$SDL2_SRC" -B "$OUT/sdl-build" -G Ninja -DCMAKE_BUILD_TYPE=Release -DSDL_SHARED=OFF -DSDL_STATIC=ON \
          -DSDL_TEST=OFF -DSDL_X11=OFF -DSDL_WAYLAND=OFF -DSDL_KMSDRM=OFF -DSDL_OFFSCREEN=ON -DSDL_OPENGL=OFF \
          -DSDL_OPENGLES=ON -DSDL_PULSEAUDIO=OFF -DSDL_ALSA=OFF -DSDL_PIPEWIRE=OFF -DSDL_JACK=OFF -DSDL_SNDIO=OFF \
          -DSDL_DUMMYAUDIO=ON -DSDL_IBUS=OFF -DSDL_HIDAPI=OFF -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
          -DCMAKE_INSTALL_PREFIX="$OUT/sdl" > "$OUT/sdl.log" 2>&1
    cmake --build "$OUT/sdl-build" -j4 >> "$OUT/sdl.log" 2>&1
    cmake --install "$OUT/sdl-build" >> "$OUT/sdl.log" 2>&1
fi
for v in gles desktop; do
    extra=(); [[ $v == gles ]] && extra=(-DWASM_HOST_SDL_GLES=ON -DSDL2_DIR="$OUT/sdl/lib/cmake/SDL2")
    cmake -S "$GAME/port/wasm/host" -B "$OUT/host-$v" -G Ninja -DW2C_DIR="$BW/w2c" -DWASM_HOST_NATIVE=ON \
          -DPORT_LUS="$LUS" "${extra[@]}" > "$OUT/host-$v.log" 2>&1
    cmake --build "$OUT/host-$v" -j4 >> "$OUT/host-$v.log" 2>&1 || { tail -20 "$OUT/host-$v.log"; exit 1; }
done

STEPS=300,450,1000,1500,1900
run() { # name exe env...
    local d="$OUT/run-$1" exe="$2"; shift 2
    rm -rf "$d"; mkdir -p "$d/png"
    (cd "$d" && env "$@" LIBGL_ALWAYS_SOFTWARE=1 PORT_NATIVE=1 PORT_NATIVE_MUTE=1 PORT_NATIVE_PNG="$d/png" \
        PORT_NATIVE_PNG_STEPS="${PNGSTEPS:-$STEPS}" timeout 600 "$exe" --rom "$ROM" --sram "$d/1080.sram" \
        --frames "$FRAMES" --trace "$d/state.log" > "$d/out.log" 2>&1) || { echo "run $1 failed:"; tail -5 "$d/out.log"; }
}
G=(SDL_VIDEO_DRIVER=offscreen SDL_AUDIODRIVER=dummy)
run gles "$OUT/host-gles/tenshoe" "${G[@]}"
run gles-noext "$OUT/host-gles/tenshoe" "${G[@]}" FAST_GLES_NO_EXT=1
run desktop "$OUT/host-desktop/tenshoe" PORT_NATIVE_HIDDEN=1
# the gamepad menu: pad 0 virtual from step 400; Back + Start at 500 (opens), D-pad down at 520 (the next row), B at
# 560 (closes); input.pad_menu on through the settings file
printf '[input]\npad_menu = 1\n' > "$OUT/menu.ini"
PNGSTEPS=499,505,530,565 FRAMES=600 run menu "$OUT/host-gles/tenshoe" "${G[@]}" PORT_SETTINGS="$OUT/menu.ini" \
    PORT_NATIVE_PAD_INJECT="400:0:lt:-32767,400:0:rt:-32767,500:0:back:1,500:0:start:1,502:0:back:0,502:0:start:0,520:0:dpad_down:1,522:0:dpad_down:0,560:0:b:1,562:0:b:0"
true

python3 -I - "$OUT" "$STEPS" <<'PY'
import re, struct, sys, zlib
out, steps = sys.argv[1], sys.argv[2].split(',')
def png(path):
    b = open(path, 'rb').read(); pos, idat = 8, b''
    while pos < len(b):
        n, t = struct.unpack('>I4s', b[pos:pos + 8])
        if t == b'IHDR': w, h, depth, ctype = struct.unpack('>IIBB', b[pos + 8:pos + 18])
        elif t == b'IDAT': idat += b[pos + 8:pos + 8 + n]
        pos += 12 + n
    raw, bpp = zlib.decompress(idat), 4 if ctype == 6 else 3
    st = w * bpp
    return w, h, bpp, b''.join(raw[y * (st + 1) + 1:(y + 1) * (st + 1)] for y in range(h))
def depth(d):
    m = re.search(r'depth answers far (\d+) / near (\d+)', open(f'{out}/run-{d}/out.log').read())
    return m.groups() if m else None
def gl(d):
    m = re.search(r'^fast: (.*)$', open(f'{out}/run-{d}/out.log').read(), re.M)
    return m.group(1) if m else 'desktop GL'
ref = open(f'{out}/run-desktop/state.log', 'rb').read()
for d in ('gles', 'gles-noext'):
    same = open(f'{out}/run-{d}/state.log', 'rb').read() == ref
    print(f'{d}: {gl(d)}')
    print(f'  state.log {"identical to" if same else "DIFFERS from"} desktop GL; depth answers far / near {depth(d)} '
          f'(desktop {depth("desktop")})')
    for s in steps:
        f = f'native_s{int(s):06d}.png'
        w, h, bpp, a = png(f'{out}/run-desktop/png/{f}'); _, _, _, b = png(f'{out}/run-{d}/png/{f}')
        px = [i for i in range(w * h) if a[i * bpp:i * bpp + 3] != b[i * bpp:i * bpp + 3]]
        md = max((abs(a[i * bpp + c] - b[i * bpp + c]) for i in px for c in range(3)), default=0)
        print(f'  step {s}: ' + ('identical' if not px else f'{len(px)} of {w * h} pixels differ, max {md}'))
log = open(f'{out}/run-menu/out.log').read()
st = [l for l in open(f'{out}/run-menu/state.log') if l.startswith('F ')]
print('menu run: ' + ('ok' if 'PORT_NATIVE_PAD_INJECT: unknown' not in log else 'bad inject') +
      f', {len(st)} frames; PNGs before / open / moved / closed in {out}/run-menu/png')
PY
