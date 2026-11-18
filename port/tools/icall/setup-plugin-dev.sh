#!/usr/bin/env bash
# The GCC plugin headers for port/tools/icall/callscan.cc (plan 21.12), WITHOUT sudo: `apt-get download` of
# gcc-<major>-plugin-dev (the version of the host gcc, which the i386 wrapper gcc32 also runs) and libgmp-dev
# (gmp.h), unpacked with `dpkg -x` into $PLUGIN_DEV (default: <monorepo>/tools/gcc-plugin-dev), then the
# plugin built to $PLUGIN_DEV/callscan.so.  Safe to rerun (rebuilds the .so only).  Root route instead:
#     sudo apt install gcc-11-plugin-dev libgmp-dev
#
#   port/tools/icall/setup-plugin-dev.sh [dir/callscan.so]  # prints the .so path (its file name must stay callscan.so)
set -euo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
MONO="${MONO:-/home/danjs/code/decomp}"
DEV="${PLUGIN_DEV:-$MONO/tools/gcc-plugin-dev}"
OUT="${1:-$DEV/callscan.so}"
MAJ="$(gcc -dumpversion | cut -d. -f1)"
INC="$(gcc -print-file-name=plugin)/include"
if [ ! -f "$INC/gcc-plugin.h" ]; then
  INC="$DEV/root/usr/lib/gcc/x86_64-linux-gnu/$MAJ/plugin/include"
  if [ ! -f "$INC/gcc-plugin.h" ]; then
    mkdir -p "$DEV/debs" "$DEV/root"
    ( cd "$DEV/debs" && apt-get download "gcc-$MAJ-plugin-dev" libgmp-dev >&2 )
    for d in "$DEV"/debs/*.deb; do dpkg -x "$d" "$DEV/root"; done
  fi
fi
[ -f "$INC/gcc-plugin.h" ] || { echo "setup-plugin-dev: no gcc-plugin.h for gcc $MAJ" >&2; exit 1; }
GMP="$DEV/root/usr/include/x86_64-linux-gnu"
g++ -shared -fPIC -fno-rtti -O1 -I"$INC" -I"$GMP" "$HERE/callscan.cc" -o "$OUT.tmp"
mv "$OUT.tmp" "$OUT"
echo "$OUT"
