#!/usr/bin/env bash
# angrylion-rdp-plus as a headless mupen64plus video plugin (no GL): its core + the
# mupen64plus plugin glue + headless_vdac.c instead of output/vdac.c and screen.c.
#   build-angrylion.sh <angrylion src> <mupen64plus api dir> <out dir> [jobs]
set -eu
HERE="$(cd "$(dirname "$0")" && pwd)"
A=$1 API=$2 OUT=$3
B="$OUT/../build-angrylion"; mkdir -p "$B"
sed -e 's/@GIT_BRANCH@/master/' -e 's/@GIT_TAG@/n64ref/' -e 's/@GIT_COMMIT_HASH@/n64ref/' \
    -e 's/@GIT_COMMIT_DATE@//' "$A/src/core/version.h.in" >"$A/src/core/version.h"
CF="-O2 -fPIC -I$A/src -I$A/src/plugin/mupen64plus -DM64P_PLUGIN_API"
objs=()
for f in "$A/src/core/n64video.c" "$A/src/plugin/mupen64plus/gfx_m64p.c" "$A/src/plugin/mupen64plus/msg.c" \
         "$HERE/headless_vdac.c"; do
    o="$B/$(basename "${f%.c}").o"; gcc -std=gnu11 $CF -c "$f" -o "$o"; objs+=("$o")
done
g++ -std=c++14 $CF -c "$A/src/core/parallel.cpp" -o "$B/parallel.o"; objs+=("$B/parallel.o")
g++ -shared -o "$OUT/mupen64plus-video-angrylion-plus.so" "${objs[@]}" -lpthread
