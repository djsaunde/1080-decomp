#!/usr/bin/env bash
# The Windows (i686, mingw-w64) toolchain of the PC port (plan section 27), without sudo:
#   - mingw-w64 GCC 10 (posix threads) + binutils + the mingw-w64 v8 headers / CRT: Ubuntu's packages,
#     apt-get download + dpkg -x into tools/mingw-w64/root (the route of ../gfx/fast3d/setup-x11-dev.sh);
#     the toolchain is relocatable, nothing is installed;
#   - SDL2 2.30.9's official mingw development release (static libSDL2.a for i686-w64-mingw32) into
#     tools/sdl2-mingw (window, GL context, WASAPI / DirectSound audio, XInput / DirectInput pads).
# Then: cmake -S port -B <b> -G Ninja -DCMAKE_TOOLCHAIN_FILE=port/win32/mingw-i686.cmake -DPORT_WIN32=ON ...
# Root / CI equivalent: sudo apt install g++-mingw-w64-i686-posix (configure with -DMINGW_ROOT=/usr), then
# SKIP_MINGW=1 port/win32/setup-mingw.sh for SDL2 alone.
set -euo pipefail
REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../../../.." && pwd)"
DEST="${MINGW_W64:-$REPO_ROOT/tools/mingw-w64}"
SDL="${SDL2_MINGW:-$REPO_ROOT/tools/sdl2-mingw}"
SDL_VER=2.30.9
SDL_SHA256=b188d165dc4a0372042bff3d0c18e9bb90772581e92a7b8b47b521b47714f75c
if [[ -n "${SKIP_MINGW:-}" ]]; then
    echo "setup-mingw: SKIP_MINGW: the system's mingw-w64"
elif [[ ! -f "$DEST/.complete" ]]; then
    mkdir -p "$DEST/debs" "$DEST/root"
    (cd "$DEST/debs" && apt-get download binutils-mingw-w64-i686 gcc-mingw-w64-i686-posix g++-mingw-w64-i686-posix \
        gcc-mingw-w64-i686-posix-runtime mingw-w64-i686-dev mingw-w64-common gcc-mingw-w64-base)
    for d in "$DEST"/debs/*.deb; do dpkg -x "$d" "$DEST/root"; done
    test -x "$DEST/root/usr/bin/i686-w64-mingw32-g++-posix"
    touch "$DEST/.complete"
fi
[[ -n "${SKIP_MINGW:-}" ]] || echo "setup-mingw: mingw-w64 in $DEST/root/usr"
if [[ ! -f "$SDL/.complete" ]]; then
    mkdir -p "$SDL/dl" "$SDL/x"
    f="$SDL/dl/SDL2-devel-$SDL_VER-mingw.tar.gz"
    [[ -f "$f" ]] || curl -sSL -o "$f" "https://github.com/libsdl-org/SDL/releases/download/release-$SDL_VER/SDL2-devel-$SDL_VER-mingw.tar.gz"
    echo "$SDL_SHA256  $f" | sha256sum -c -
    tar -xzf "$f" -C "$SDL/x"
    test -f "$SDL/x/SDL2-$SDL_VER/i686-w64-mingw32/lib/libSDL2.a"
    touch "$SDL/.complete"
fi
echo "setup-mingw: SDL2 in $SDL/x/SDL2-$SDL_VER/i686-w64-mingw32"
