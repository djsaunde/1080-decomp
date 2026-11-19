#!/usr/bin/env bash
# port/gfx/fast3d/golden-gles.sh OUT (plan 31): the golden scenes through Fast3D's OpenGL ES 3 path (f3dreplay-gles:
# LUS built USE_OPENGLES in an EGL GLES 3.0 context on Mesa llvmpipe, the Android renderer) against the desktop GL
# renders of the same dumps (golden.sh's OUT/golden-out/*.png / *.rb, made by `make -C port/gfx test OUT=OUT`).
#   FAST_GLES_NO_EXT=1 (or a list: depth_clamp,mirror_clamp,read_depth) checks the fallbacks: vertex-shader far clamp,
#   mirrored repeat, depth read through a depth texture (llvmpipe blits no depth
#   from its window surface, so that one reads the far plane here).
# Per scene: identical, or the number of differing pixels / the largest channel difference, and whether the
# read-back (colour + depth grid, .rb) is identical. Not a gate: llvmpipe's GLES and GL paths may round differently.
# On ES Fast3D draws into its game framebuffer (LUS patch 21), which rasterises a few pixels 1 level apart from drawing
# into the window (rows flipped); with OUT/f3dreplay-fb (make -C port/gfx/fast3d OUT=OUT OUT/f3dreplay-fb: desktop GL
# drawing into the framebuffer the same way) the GLES renders are also compared with that like-for-like reference.
set -euo pipefail
OUT="$1"
DIR="$OUT/golden-out"
G="$OUT/golden-gles${FAST_GLES_NO_EXT:+-noext-${FAST_GLES_NO_EXT//,/-}}"
mkdir -p "$G"
export LIBGL_ALWAYS_SOFTWARE=1
for d in $("$OUT/gold" --list); do
    log="$("$OUT/f3dreplay-gles" "$DIR/$d.dlmp" "$G/$d.png" --readback "$G/$d.rb" 2>&1)" ||
        { echo "$log"; echo "golden-gles: $d: f3dreplay-gles failed"; exit 1; }
done
echo "$log" | grep -m1 '^fast: OpenGL ES' || true
refs=("$DIR")
if [[ -x "$OUT/f3dreplay-fb" ]]; then
    F="$OUT/golden-fb"
    mkdir -p "$F"
    for d in $("$OUT/gold" --list); do
        "$OUT/f3dreplay-fb" "$DIR/$d.dlmp" "$F/$d.png" --readback "$F/$d.rb" > /dev/null 2>&1 ||
            { echo "golden-gles: $d: f3dreplay-fb failed"; exit 1; }
    done
    refs+=("$F")
fi
for R in "${refs[@]}"; do
[[ "$R" == "$DIR" ]] && echo "against desktop GL drawing into the window (the goldens):" ||
    echo "against desktop GL drawing into the game framebuffer (f3dreplay-fb):"
python3 -I - "$R" "$G" $("$OUT/gold" --list) <<'PY'
import struct, sys, zlib
def png(path):
    b = open(path, 'rb').read()
    pos, idat, w = 8, b'', 0
    while pos < len(b):
        n, t = struct.unpack('>I4s', b[pos:pos + 8])
        if t == b'IHDR':
            w, h, depth, ctype = struct.unpack('>IIBB', b[pos + 8:pos + 18])
        elif t == b'IDAT':
            idat += b[pos + 8:pos + 8 + n]
        pos += 12 + n
    raw, bpp = zlib.decompress(idat), 4 if ctype == 6 else 3
    stride, rows, prev = w * bpp, [], bytearray(w * bpp)
    for y in range(h):
        f, line = raw[y * (stride + 1)], bytearray(raw[y * (stride + 1) + 1:(y + 1) * (stride + 1)])
        assert f == 0, 'filtered PNG rows are not expected (egl_png writes filter 0)'
        rows.append(bytes(line))
    return w, h, bpp, b''.join(rows)
ref, new, scenes = sys.argv[1], sys.argv[2], sys.argv[3:]
same = 0
for s in scenes:
    w, h, bpp, a = png(f'{ref}/{s}.png')
    _, _, _, b = png(f'{new}/{s}.png')
    px = [i for i in range(w * h) if a[i * bpp:i * bpp + bpp] != b[i * bpp:i * bpp + bpp]]
    md = max((abs(a[i * bpp + c] - b[i * bpp + c]) for i in px for c in range(bpp)), default=0)
    rb = open(f'{ref}/{s}.rb', 'rb').read() == open(f'{new}/{s}.rb', 'rb').read()
    same += not px and rb
    print(f'  {s:12s} ' + ('identical' if not px else f'{len(px)} of {w * h} pixels differ ({100.0 * len(px) / (w * h):.3f} %), max {md}') +
          f'; read-back {"identical" if rb else "differs"}')
print(f'golden-gles: {same} of {len(scenes)} scenes identical (PNG + read-back)')
PY
done
