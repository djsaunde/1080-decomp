#!/usr/bin/env bash
# Build the headless N64 reference emulator (PC port plan 22): mupen64plus-core + the
# angrylion-rdp-plus pixel-accurate software RDP + the cxd4 LLE RSP, driven by n64ref.c
# (frame capture to PNG at given VIs / game steps, scripted pads in the port's .input format).
# No sudo: SDL2 / zlib / libpng headers and the SDL2 runtime come from `apt-get download` debs
# unpacked under $DST/root. Everything lands in <monorepo>/tools/n64emu (gitignored).
#
#   port/tools/n64ref/build.sh            # clone (first run) + build
#   port/tools/n64ref/build.sh frontend   # rebuild n64ref only
set -eu
HERE="$(cd "$(dirname "$0")" && pwd)"
GAME="$(cd "$HERE/../../.." && pwd)"
MONO="$(cd "$GAME/../.." && pwd)"
DST="$MONO/tools/n64emu"
SRC="$DST/src"
ROOT="$DST/root"
LIB="$ROOT/usr/lib/x86_64-linux-gnu"
OUT="$DST/bin"
J=${J:-8}
mkdir -p "$SRC" "$DST/debs" "$OUT"

clone() { [ -d "$SRC/${1#*/}" ] || git clone -q --depth 1 "https://github.com/$1.git" "$SRC/${1#*/}"; }
if [ "${1:-}" != frontend ]; then
    clone mupen64plus/mupen64plus-core
    clone mupen64plus/mupen64plus-rsp-cxd4
    clone ata4/angrylion-rdp-plus
    if [ ! -f "$LIB/libSDL2.so" ]; then
        (cd "$DST/debs" && apt-get download libsdl2-dev libsdl2-2.0-0 zlib1g-dev libpng-dev nasm \
            libasound2 libxss1 libdecor-0-0 libpulse0 libsndfile1 libasyncns0 libflac8 libogg0 \
            libopus0 libvorbis0a libvorbisenc2)
        for d in "$DST"/debs/*.deb; do dpkg-deb -x "$d" "$ROOT"; done
    fi
    INC="-I$ROOT/usr/include -I$ROOT/usr/include/x86_64-linux-gnu"
    # the core: dynarec, no OSD (DebugMemGetPointer, for the step counter, is in every build)
    make -C "$SRC/mupen64plus-core/projects/unix" -j"$J" all PKG_CONFIG=true OSD=0 VULKAN=0 AS="$ROOT/usr/bin/nasm" \
        ZLIB_CFLAGS="$INC" ZLIB_LDLIBS="-l:libz.so.1" LIBPNG_CFLAGS="$INC" LIBPNG_LDLIBS="-l:libpng16.so.16" \
        SDL_CFLAGS="$INC -I$ROOT/usr/include/SDL2 -D_REENTRANT" SDL_LDLIBS="-L$LIB -lSDL2" \
        >"$DST/core-build.log" 2>&1 || { tail -30 "$DST/core-build.log"; exit 1; }
    cp "$SRC/mupen64plus-core/projects/unix/libmupen64plus.so.2.0.0" "$OUT/libmupen64plus.so.2"
    mkdir -p "$OUT/data" && cp "$SRC/mupen64plus-core/data/mupen64plus.ini" "$OUT/data/"
    make -C "$SRC/mupen64plus-rsp-cxd4/projects/unix" -j"$J" all PKG_CONFIG=true \
        APIDIR="$SRC/mupen64plus-core/src/api" >"$DST/rsp-build.log" 2>&1 || { tail -30 "$DST/rsp-build.log"; exit 1; }
    cp "$SRC"/mupen64plus-rsp-cxd4/projects/unix/mupen64plus-rsp-cxd4*.so "$OUT/"
    "$HERE/build-angrylion.sh" "$SRC/angrylion-rdp-plus" "$SRC/mupen64plus-core/src/api" "$OUT" "$J"
fi
# n64ref re-execs itself with LD_LIBRARY_PATH=$RP (SDL2 / pulse from the unpacked debs)
RP="$LIB:$LIB/pulseaudio:$MONO/tools/pulse-dev/root/usr/lib/x86_64-linux-gnu:$MONO/tools/pulse-dev/root/usr/lib/x86_64-linux-gnu/pulseaudio"
gcc -O2 -g -Wall -rdynamic -o "$OUT/n64ref" "$HERE/n64ref.c" -I"$SRC/mupen64plus-core/src/api" -I"$ROOT/usr/include" \
    -DN64REF_LIBPATH="\"$RP\"" -ldl -l:libz.so.1 -lpthread
echo "n64ref: built $OUT/n64ref"
