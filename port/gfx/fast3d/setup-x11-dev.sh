#!/usr/bin/env bash
# Fetch the 64-bit Xlib development headers for the live presenter (presenter.cpp, PC port plan
# 19.11) without sudo: apt-get download + dpkg -x into tools/x11-dev, the route of
# ../replay/setup-gl-dev.sh. The runtime library (libX11.so.6) is already on a stock WSL2
# Ubuntu (WSLg); only the headers are missing. The presenter links libX11.so.6 by path.
# Root equivalent: sudo apt install libx11-dev
set -euo pipefail
REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../../../../.." && pwd)"
DEST="${X11_DEV:-$REPO_ROOT/tools/x11-dev}"
if [[ -f "$DEST/.complete" ]]; then
    echo "setup-x11-dev: $DEST present"
    exit 0
fi
mkdir -p "$DEST/debs" "$DEST/root"
(cd "$DEST/debs" && apt-get download libx11-dev x11proto-dev)
for d in "$DEST"/debs/*.deb; do dpkg -x "$d" "$DEST/root"; done
test -f "$DEST/root/usr/include/X11/Xlib.h" && test -f "$DEST/root/usr/include/X11/X.h"
touch "$DEST/.complete"
echo "setup-x11-dev: headers in $DEST/root/usr/include"
