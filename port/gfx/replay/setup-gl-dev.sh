#!/usr/bin/env bash
# Fetch the 64-bit EGL / GL development headers for dlreplay without sudo
# (PC port plan 19.1): apt-get download + dpkg -x into tools/gl-dev, the same
# route as scripts/setup-i386-sysroot.sh. The runtime libraries (libEGL.so.1,
# libGL.so.1, Mesa's d3d12 / llvmpipe drivers) are already on a stock WSL2
# Ubuntu; only the headers and link symlinks are missing.
# Root equivalent: sudo apt install libegl-dev libgl-dev
set -euo pipefail
REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../../../../.." && pwd)"
DEST="${GL_DEV:-$REPO_ROOT/tools/gl-dev}"
if [[ -f "$DEST/.complete" ]]; then
    echo "setup-gl-dev: $DEST present"
    exit 0
fi
mkdir -p "$DEST/debs" "$DEST/root"
(cd "$DEST/debs" && apt-get download libegl-dev libgl-dev libglx-dev libopengl-dev)
for d in "$DEST"/debs/*.deb; do dpkg -x "$d" "$DEST/root"; done
test -f "$DEST/root/usr/include/EGL/egl.h" && test -f "$DEST/root/usr/include/GL/gl.h"
touch "$DEST/.complete"
echo "setup-gl-dev: headers in $DEST/root/usr/include"
