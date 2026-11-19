#!/usr/bin/env bash
# Fetch libultraship (LUS) at the pinned commit, apply the 1080 patch series
# (port/third_party/libultraship-patches, plan section 14), and fetch prism-processor
# (the shader template preprocessor LUS's OpenGL backend uses) at the commit LUS
# pins in cmake/dependencies/common.cmake. Plan section 19.6.
#
# Both trees go to tools/ in the monorepo (not committed). The LUS tree keeps its
# LICENSE and src/fast/LICENSE.txt (MIT); see port/third_party/README.md.
# Override the sources with LUS_URL / PRISM_URL (e.g. a local clone) for offline use.
set -euo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
GAME="$(cd "$HERE/../../.." && pwd)"
REPO_ROOT="$(cd "$GAME/../.." && pwd)"
DEST="${LUS_TOOLS:-$REPO_ROOT/tools/lus}"
LUS_PIN=759b13401cd6903daa67a00a753066f791622e78
PRISM_PIN=1de054450e7b3c5f777d2e3dfcb228ad120c329d
LUS_URL="${LUS_URL:-https://github.com/Kenix3/libultraship}"
PRISM_URL="${PRISM_URL:-https://github.com/KiritoDv/prism-processor.git}"
PATCHES=("$GAME"/port/third_party/libultraship-patches/*.patch)
# the stamp names the pin and the patch contents, so a changed series re-applies
SHA1SUM=sha1sum; command -v sha1sum >/dev/null || SHA1SUM="shasum -a 1" # macOS (plan 30) has shasum only
STAMP="$LUS_PIN $PRISM_PIN $(cat "${PATCHES[@]}" | $SHA1SUM | cut -c1-12)"
if [[ -f "$DEST/.complete" && "$(cat "$DEST/.complete")" == "$STAMP" ]]; then
    echo "setup-lus: $DEST present ($STAMP)"
    exit 0
fi
rm -rf "$DEST"
mkdir -p "$DEST"
fetch() { # url pin dir
    git init -q "$3"
    git -C "$3" fetch -q --depth 1 "$1" "$2" || git -C "$3" fetch -q "$1"
    git -C "$3" -c advice.detachedHead=false checkout -q "$2"
}
fetch "$LUS_URL" "$LUS_PIN" "$DEST/libultraship"
fetch "$PRISM_URL" "$PRISM_PIN" "$DEST/prism"
git -C "$DEST/libultraship" apply "${PATCHES[@]}"
test -f "$DEST/libultraship/LICENSE" && test -f "$DEST/libultraship/src/fast/LICENSE.txt"
echo "$STAMP" > "$DEST/.complete"
echo "setup-lus: libultraship $LUS_PIN + ${#PATCHES[@]} patches, prism $PRISM_PIN in $DEST"
