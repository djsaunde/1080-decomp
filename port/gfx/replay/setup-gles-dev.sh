#!/usr/bin/env bash
# Fetch the OpenGL ES 3 headers and glvnd's libGLESv2.so.2 without sudo (plan 31: f3dreplay-gles renders the
# golden scenes through Fast3D's GLES path on Mesa llvmpipe, the check of the Android renderer on Linux):
# apt-get download + dpkg -x into tools/gles-dev, as setup-gl-dev.sh. Root equivalent: sudo apt install libgles-dev
set -euo pipefail
REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../../../../.." && pwd)"
DEST="${GLES_DEV:-$REPO_ROOT/tools/gles-dev}"
if [[ -f "$DEST/.complete" ]]; then
    echo "setup-gles-dev: $DEST present"
    exit 0
fi
mkdir -p "$DEST/debs" "$DEST/root"
(cd "$DEST/debs" && apt-get download libgles-dev libgles2 libgles1)
for d in "$DEST"/debs/*.deb; do dpkg -x "$d" "$DEST/root"; done
test -f "$DEST/root/usr/include/GLES3/gl3.h"
ls "$DEST"/root/usr/lib/*/libGLESv2.so.2 > /dev/null
touch "$DEST/.complete"
echo "setup-gles-dev: headers in $DEST/root/usr/include, libGLESv2 in $(dirname "$(ls "$DEST"/root/usr/lib/*/libGLESv2.so.2)")"
