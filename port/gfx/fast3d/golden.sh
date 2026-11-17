#!/usr/bin/env bash
# port/gfx golden-image tests (plan 19.6): render every gold.c scene through f3dreplay on
# Mesa's llvmpipe and compare the PNG hashes with port/gfx/golden/golden.sha1.
#   golden.sh OUT            check (exit 1 on a mismatch; the PNGs stay in OUT/golden/)
#   golden.sh OUT --update   rewrite golden.sha1 from this machine's renders (look at them first)
# Run by `make -C port/gfx test`. The hashes are llvmpipe's (LIBGL_ALWAYS_SOFTWARE=1) for the
# Mesa build named in golden.sha1's header; another Mesa version may round differently, which
# shows up as a mismatch of every textured / blended scene at once.
set -euo pipefail
OUT="$1"
MODE="${2:-check}"
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REF="$HERE/../golden/golden.sha1"
DIR="$OUT/golden-out"
mkdir -p "$DIR"
rm -f "$DIR"/*.dlmp "$DIR"/*.png "$DIR"/*.rb
"$OUT/gold" "$DIR"
export LIBGL_ALWAYS_SOFTWARE=1
renderer=""
for d in $("$OUT/gold" --list); do
    log="$("$OUT/f3dreplay" "$DIR/$d.dlmp" "$DIR/$d.png" --readback "$DIR/$d.rb" 2>&1)" || { echo "$log"; echo "golden: $d: f3dreplay failed"; exit 1; }
    renderer="$(echo "$log" | sed -n 's/^replay: GL //p')"
done
python3 "$HERE/../tools/dlump.py" --check "$DIR"/*.dlmp > /dev/null
# the in-process renderer (port_fast3d.cpp, plan 19.7): every scene, in order, through one interpreter
# in one process (test_inproc), must give f3dreplay's PNG byte for byte
if [[ -x "$OUT/test_inproc" ]]; then
    rm -rf "$DIR/inproc" && mkdir -p "$DIR/inproc"
    scenes=($("$OUT/gold" --list))
    PORT_GFX_PNG="$DIR/inproc" "$OUT/test_inproc" $(printf "$DIR/%s.dlmp " "${scenes[@]}") > "$DIR/inproc/log.txt" 2>&1 ||
        { cat "$DIR/inproc/log.txt"; echo "golden: test_inproc failed"; exit 1; }
    for i in "${!scenes[@]}"; do
        cmp -s "$DIR/${scenes[$i]}.png" "$DIR/inproc/$(printf 'frame_%06d.png' "$i")" ||
            { echo "golden: ${scenes[$i]}: the in-process render differs from f3dreplay's"; exit 1; }
    done
    echo "golden: in process: ${#scenes[@]} scenes identical to f3dreplay"
fi
(cd "$DIR" && sha1sum $("$OUT/gold" --list | sed 's/$/.png/') $("$OUT/gold" --list | sed 's/$/.rb/')) > "$DIR/actual.sha1"
if [[ "$MODE" == "--update" ]]; then
    { echo "# port/gfx golden images: sha1 of f3dreplay's PNG and read-back (.rb) per gold.c scene (fast3d/golden.sh)"
      echo "# renderer: $renderer"
      cat "$DIR/actual.sha1"; } > "$REF"
    echo "golden: wrote $REF ($("$OUT/gold" --list | wc -l) scenes, $renderer)"
    exit 0
fi
if diff <(grep -v '^#' "$REF") "$DIR/actual.sha1" > "$DIR/diff.txt"; then
    echo "golden: $("$OUT/gold" --list | wc -l) scenes match, PNG + read-back ($renderer)"
else
    cat "$DIR/diff.txt"
    echo "golden: MISMATCH (renders in $DIR; reference renderer: $(sed -n 's/^# renderer: //p' "$REF"))"
    exit 1
fi
