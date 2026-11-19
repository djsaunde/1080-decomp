#!/usr/bin/env bash
# port/wasm/macos/make-app.sh <tenshoe> <out dir> [version]: the macOS app (plan 30) -- 1080.app (the wasm2c
# build's native host, ad-hoc signed), README.txt, zipped as 1080-pc-macos.zip in <out dir>.
set -euo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
EXE="$1"; OUT="$2"; VER="${3:-0.0}"
APP="$OUT/1080.app"
rm -rf "$APP"
mkdir -p "$APP/Contents/MacOS" "$APP/Contents/Resources"
cp "$EXE" "$APP/Contents/MacOS/1080"
chmod 755 "$APP/Contents/MacOS/1080"
sed "s/@VERSION@/$VER/g" "$HERE/Info.plist" > "$APP/Contents/Info.plist"
printf 'APPL????' > "$APP/Contents/PkgInfo"
# no Developer ID: an ad-hoc signature (arm64 macOS runs only signed code); Gatekeeper still asks (README)
codesign --force --deep --sign - "$APP"
cp "$HERE/README-macos.txt" "$OUT/README.txt"
(cd "$OUT" && rm -f 1080-pc-macos.zip && ditto -c -k --keepParent 1080.app 1080-pc-macos.zip && zip -q 1080-pc-macos.zip README.txt)
echo "make-app: $OUT/1080-pc-macos.zip"
