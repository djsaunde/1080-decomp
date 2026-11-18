#!/usr/bin/env bash
# Fetch the public 1080 USF rip (usf.joshw.info, "1080 Snowboarding (1998-02-28)", ripped by UNKNOWNFILE 2007)
# into the monorepo's tools/usf/rip/1080 (gitignored; USF_TOOLS overrides).  The rip is the game's own code
# and audio data run from save states, so it is downloaded, never committed.  Needs build-lazyusf2.sh's
# venv (py7zr) to unpack the .7z.  The track -> sequence map is in port/audio/tools/usf-compare.
set -euo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$HERE/../../../../../.." && pwd)"
DEST="${USF_TOOLS:-$REPO_ROOT/tools/usf}"
URL='https://usf.joshw.info/0-9/1080%c2%b0%20Snowboarding%20(1998-02-28)(Nintendo%20EAD)(Nintendo)%5bN64%5d.7z'
SHA256=3d2f6d8159b252c2302322b3e02f9b057a24dff64b819e75a5c423b2041c2fb7
OUT="$DEST/rip/1080"
if [[ -f "$OUT/.complete" ]]; then echo "fetch-usf: $OUT present"; exit 0; fi
PY="$DEST/venv/bin/python"
[[ -x "$PY" ]] || { echo "fetch-usf: run build-lazyusf2.sh first (needs $PY with py7zr)"; exit 1; }
mkdir -p "$DEST/rip"
A="$DEST/rip/1080-usf.7z"
if [[ ! -f "$A" ]] || ! echo "$SHA256  $A" | sha256sum -c --quiet 2>/dev/null; then
  curl -fsSL -o "$A.part" "$URL"
  echo "$SHA256  $A.part" | sha256sum -c --quiet || { echo "fetch-usf: checksum mismatch (the rip changed upstream?)"; exit 1; }
  mv "$A.part" "$A"
fi
mkdir -p "$OUT"
# extracted as data (no code from the archive runs); every member is a flat .miniusf / .usflib / .txt
"$PY" -I -c 'import py7zr, sys
with py7zr.SevenZipFile(sys.argv[1]) as z:
    names = [n for n in z.getnames() if "/" not in n and ".." not in n]
    z.extract(path=sys.argv[2], targets=names)' "$A" "$OUT"
ls "$OUT"/*.usflib >/dev/null
n=$(ls "$OUT"/*.miniusf | wc -l)
touch "$OUT/.complete"
echo "fetch-usf: $n tracks in $OUT"
