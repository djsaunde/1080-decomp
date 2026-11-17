#!/usr/bin/env bash
# Fetch libpulse-simple / libpulse for the live presenter's audio (presenter.cpp, PC port plan 19.12)
# without sudo: apt-get download + dpkg -x into tools/pulse-dev, the route of setup-x11-dev.sh.
# The headers (libpulse-dev) let presenter.cpp compile against the real pa_simple API; the runtime
# libraries (libpulse0 and those of its dependencies a stock WSL2 Ubuntu lacks: libsndfile1 and its
# codecs, libasyncns0) are dlopen'd by the presenter at run time, found through the
# LD_LIBRARY_PATH port/tools/play.sh sets (pulse_libpath below). Without them the presenter runs
# silent. WSLg's PulseAudio server is $PULSE_SERVER (unix:/mnt/wslg/PulseServer).
# Root equivalent: sudo apt install libpulse-dev
set -euo pipefail
REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../../../../.." && pwd)"
DEST="${PULSE_DEV:-$REPO_ROOT/tools/pulse-dev}"
if [[ -f "$DEST/.complete" ]]; then
    echo "setup-pulse-dev: $DEST present"
    exit 0
fi
mkdir -p "$DEST/debs" "$DEST/root"
(cd "$DEST/debs" && apt-get download libpulse-dev libpulse0 libsndfile1 libasyncns0 libflac8 libogg0 libopus0 \
    libvorbis0a libvorbisenc2)
for d in "$DEST"/debs/*.deb; do dpkg -x "$d" "$DEST/root"; done
R="$DEST/root/usr/lib/x86_64-linux-gnu"
test -f "$DEST/root/usr/include/pulse/simple.h"
test -f "$R/libpulse-simple.so.0"
# every runtime dependency must resolve (system libraries or the ones fetched here)
missing=$(LD_LIBRARY_PATH="$R:$R/pulseaudio" ldd "$R/libpulse-simple.so.0" | grep "not found" || true)
if [[ -n "$missing" ]]; then
    echo "setup-pulse-dev: unresolved dependencies:"; echo "$missing"; exit 1
fi
touch "$DEST/.complete"
echo "setup-pulse-dev: headers in $DEST/root/usr/include, libraries in $R (+ pulseaudio/)"
