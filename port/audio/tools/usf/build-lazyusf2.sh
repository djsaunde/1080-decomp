#!/usr/bin/env bash
# Build the real-game audio reference of port/audio (docs/1080_PC_PORT_PLAN.md 15.11, 15.12) into the
# monorepo's tools/usf (gitignored, like the other tools; USF_TOOLS overrides):
#   src/lazyusf2, src/psflib   github derselbst/lazyusf2 + kode54/psflib at pinned commits, with
#                              lazyusf2-hooks.patch (an audio-task hook in do_SP_Task)
#   zlib/                      zlib headers when the system has none (apt-get download zlib1g-dev, no sudo)
#   bin/usfrender              usfrender.c + port/audio/tools/rsp_lle.c, linked with liblazyusf.a
#   venv/                      python with numpy + py7zr (uv) for fetch-usf.sh and usf-compare
# Then run fetch-usf.sh (the rip itself) and port/audio/tools/usf-compare.
# Idempotent: rebuilds usfrender every run, the rest only when missing.
set -euo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$HERE/../../../../../.." && pwd)"   # the monorepo (projects/<wt>/port/audio/tools/usf)
DEST="${USF_TOOLS:-$REPO_ROOT/tools/usf}"
LAZYUSF2_REV=00d19c9b1e0ae4924d395310844a94d871743c89
PSFLIB_REV=95509e0c6f13d769593bbf51a1b0e0efdc355ba1
JOBS="${JOBS:-8}"
mkdir -p "$DEST/src" "$DEST/bin"

fetch() {  # fetch <dir> <url> <rev>
  if [[ ! -d "$DEST/src/$1/.git" ]]; then git clone -q "$2" "$DEST/src/$1"; fi
  if [[ "$(git -C "$DEST/src/$1" rev-parse HEAD)" != "$3" ]]; then
    git -C "$DEST/src/$1" checkout -q -f "$3"; git -C "$DEST/src/$1" clean -qfdx; fi
}
fetch lazyusf2 https://github.com/derselbst/lazyusf2.git "$LAZYUSF2_REV"
fetch psflib https://github.com/kode54/psflib.git "$PSFLIB_REV"
L="$DEST/src/lazyusf2"; P="$DEST/src/psflib"
if ! git -C "$L" apply --reverse --check "$HERE/lazyusf2-hooks.patch" 2>/dev/null; then
  git -C "$L" apply "$HERE/lazyusf2-hooks.patch"; rm -f "$L/liblazyusf.a" "$L/rsp/rsp_core.o"; fi

ZINC=(); ZLIB=(-lz)
if [[ ! -f /usr/include/zlib.h ]]; then
  if [[ ! -f "$DEST/zlib/usr/include/zlib.h" ]]; then
    mkdir -p "$DEST/zlib/debs"
    (cd "$DEST/zlib/debs" && apt-get download zlib1g-dev)
    for d in "$DEST"/zlib/debs/*.deb; do dpkg -x "$d" "$DEST/zlib"; done
  fi
  ZINC=(-I"$DEST/zlib/usr/include")
  ZLIB=("$(ls "$DEST"/zlib/usr/lib/*/libz.a | head -1)")
fi

if [[ ! -f "$P/libpsflib.a" ]]; then
  (cd "$P" && for c in psflib.c psf2fs.c; do cc -c -O2 -fPIC "${ZINC[@]}" "$c"; done && ar rcs libpsflib.a psflib.o psf2fs.o)
fi
if [[ ! -f "$L/liblazyusf.a" ]]; then
  make -C "$L" -j"$JOBS" CC="cc ${ZINC[*]}" liblazyusf.a >"$DEST/lazyusf2-build.log" 2>&1 || { tail -20 "$DEST/lazyusf2-build.log"; exit 1; }
fi
cc -O2 -g -o "$DEST/bin/usfrender" -I"$L" -I"$P" -I"$HERE/.." "$HERE/usfrender.c" "$HERE/../rsp_lle.c" \
   "$L/liblazyusf.a" "$P/libpsflib.a" "${ZLIB[@]}" -lm

if [[ ! -x "$DEST/venv/bin/python" ]] || ! "$DEST/venv/bin/python" -c 'import numpy, py7zr' 2>/dev/null; then
  command -v uv >/dev/null || { echo "build-lazyusf2: needs uv (or make $DEST/venv with numpy + py7zr by hand)"; exit 1; }
  uv venv -q "$DEST/venv"
  VIRTUAL_ENV="$DEST/venv" uv pip install -q numpy py7zr
fi
echo "build-lazyusf2: $DEST/bin/usfrender, python $DEST/venv/bin/python"
