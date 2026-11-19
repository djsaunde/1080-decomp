#!/usr/bin/env bash
# port/android/build-apk.sh (plan 31): the Android APK of the wasm2c build.
#
#   port/android/build-apk.sh [-o OUT] [--abis arm64-v8a[,x86_64]] [--jobs N] [--ci-key] [--version NAME]
#
#   1. the game as wasm32 + its wasm2c translation (port/ with PORT_WASM; the same C the macOS build compiles);
#   2. Gradle (SDL2's wrapper) on port/android/project: libSDL2.so + libmain.so for each ABI with the NDK's clang
#      (port/android/jni), SDLActivity + TenshoeActivity, packaged and signed (release);
#   3. OUT/1080-snowboarding.apk, checked (apksigner verify, the ABIs inside).
#
# Signing: the release key tools/android/tenshoe-release.jks (made on first use; its passwords in
# tools/android/tenshoe-release.properties; both gitignored: keep them safe, an update must be signed with the same
# key). --ci-key: a throwaway key in OUT instead (CI). Environment: ANDROID_TOOLS (tools/android, from
# setup-android.sh), WASI_SDK / WASM2C / WASM2C_RT (default tools/wasm), LUS_TOOLS (default tools/lus; it must hold
# the whole patch series: port/gfx/fast3d/setup-lus.sh). No ROM is needed or packaged.
set -euo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
GAME="$(cd "$HERE/../.." && pwd)"
REPO="$(cd "$GAME/../.." && pwd)"
OUT="$GAME/build/android"
ABIS=arm64-v8a
JOBS=4
CI_KEY=0
VERSION="0.$(date -u +%Y%m%d)"
while [[ $# -gt 0 ]]; do
    case "$1" in
        -o) OUT="$2"; shift 2 ;;
        --abis) ABIS="$2"; shift 2 ;;
        --jobs) JOBS="$2"; shift 2 ;;
        --ci-key) CI_KEY=1; shift ;;
        --version) VERSION="$2"; shift 2 ;;
        *) echo "build-apk: unknown option $1" >&2; exit 2 ;;
    esac
done
mkdir -p "$OUT"
OUT="$(cd "$OUT" && pwd)"
T="${ANDROID_TOOLS:-$REPO/tools/android}"
[[ -f "$T/env.sh" ]] || ANDROID_TOOLS="$T" bash "$HERE/setup-android.sh"
# shellcheck disable=SC1091
source "$T/env.sh"
W="$REPO/tools/wasm"
WASI_SDK="${WASI_SDK:-$(ls -d "$W"/wasi-sdk-*-linux 2>/dev/null | head -1)}"
WASM2C="${WASM2C:-$W/wabt-build/wasm2c}"
WASM2C_RT="${WASM2C_RT:-$(ls -d "$W"/src/wabt-*/wasm2c 2>/dev/null | head -1)}"
LUS_TOOLS="${LUS_TOOLS:-$REPO/tools/lus}"
for x in "$WASI_SDK/bin/clang" "$WASM2C" "$WASM2C_RT/wasm-rt-impl.c" "$LUS_TOOLS/.complete"; do
    [[ -e "$x" ]] || { echo "build-apk: missing $x (tools/wasm: plan 30.2; LUS: port/gfx/fast3d/setup-lus.sh)" >&2; exit 1; }
done
grep -q 'GlesDetectExtensions' "$LUS_TOOLS/libultraship/src/fast/backends/gfx_opengl.cpp" ||
    { echo "build-apk: $LUS_TOOLS lacks LUS patches 20 / 21 (OpenGL ES): LUS_TOOLS=<dir> port/gfx/fast3d/setup-lus.sh" >&2; exit 1; }

# 1. the game, wasm32 -> wasm2c
echo "build-apk: [1/3] the game as wasm32 + wasm2c ($OUT/wasm)"
cmake -S "$GAME/port" -B "$OUT/wasm" -G Ninja -DPORT_WASM=ON -DCMAKE_TOOLCHAIN_FILE="$WASI_SDK/share/cmake/wasi-sdk.cmake" \
      -DWASI_SDK_PREFIX="$WASI_SDK" -DWASM2C="$WASM2C" > "$OUT/wasm-cfg.log" 2>&1 || { tail -20 "$OUT/wasm-cfg.log"; exit 1; }
cmake --build "$OUT/wasm" -j "$JOBS" > "$OUT/wasm-build.log" 2>&1 || { grep -m20 -E 'error|FAILED' "$OUT/wasm-build.log"; exit 1; }

# 2. the signing key
if [[ "$CI_KEY" == 1 ]]; then
    KS="$OUT/ci-key.jks"; KP="$OUT/ci-key.properties"
else
    KS="$T/tenshoe-release.jks"; KP="$T/tenshoe-release.properties"
fi
if [[ ! -f "$KS" ]]; then
    PASS="$(python3 -c 'import secrets; print(secrets.token_urlsafe(24))')"
    keytool -genkeypair -keystore "$KS" -storetype PKCS12 -alias tenshoe -keyalg RSA -keysize 4096 -validity 10000 \
            -storepass "$PASS" -keypass "$PASS" -dname "CN=1080 Snowboarding PC port, O=djsaunde" > /dev/null 2>&1
    (umask 077 && printf 'storePassword=%s\nkeyPassword=%s\nkeyAlias=tenshoe\nstoreFile=%s\n' "$PASS" "$PASS" "$KS" > "$KP")
    chmod 600 "$KS" "$KP"
    echo "build-apk: new signing key $KS (passwords in $KP)"
fi
STOREPASS="$(sed -n 's/^storePassword=//p' "$KP")"
KEYPASS="$(sed -n 's/^keyPassword=//p' "$KP")"
ALIAS="$(sed -n 's/^keyAlias=//p' "$KP")"

# 3. Gradle: native libraries, Java, the APK
echo "build-apk: [2/3] Gradle ($ABIS, NDK $(basename "$ANDROID_NDK_HOME"), $JOBS jobs)"
VCODE="$(date -u +%y%j%H)"
"$SDL2_SRC/android-project/gradlew" -p "$GAME/port/android/project" --no-daemon --console=plain \
    --project-cache-dir "$OUT/gradle-project-cache" assembleRelease \
    -PtenshoeOut="$OUT" -PtenshoeW2c="$OUT/wasm/w2c" -PtenshoeWasm2cRt="$WASM2C_RT" -PtenshoeSdl2="$SDL2_SRC" \
    -PtenshoeLus="$LUS_TOOLS" -PtenshoeAbis="$ABIS" -PtenshoeJobs="$JOBS" -PtenshoeKeystore="$KS" \
    -PtenshoeStorePass="$STOREPASS" -PtenshoeKeyAlias="$ALIAS" -PtenshoeKeyPass="$KEYPASS" \
    -PtenshoeVersionName="$VERSION" -PtenshoeVersionCode="$VCODE" > "$OUT/gradle.log" 2>&1 ||
    { grep -m30 -E 'error|FAILED|What went wrong' -A3 "$OUT/gradle.log"; tail -30 "$OUT/gradle.log"; exit 1; }
APK="$(ls "$OUT"/gradle/app/outputs/apk/release/*.apk | head -1)"
cp "$APK" "$OUT/1080-snowboarding.apk"

echo "build-apk: [3/3] checks"
apksigner verify --print-certs "$OUT/1080-snowboarding.apk" | grep -E 'Signer #1 certificate (DN|SHA-256)'
python3 -I - "$OUT/1080-snowboarding.apk" <<'PY'
import sys, zipfile
z = zipfile.ZipFile(sys.argv[1])
libs = sorted(n for n in z.namelist() if n.startswith('lib/'))
for n in libs:
    print(f'  {n} {z.getinfo(n).file_size} bytes')
for abi in sorted({n.split('/')[1] for n in libs}):
    need = {f'lib/{abi}/libmain.so', f'lib/{abi}/libSDL2.so', f'lib/{abi}/libc++_shared.so'}
    missing = need - set(libs)
    if missing:
        sys.exit(f'build-apk: missing {sorted(missing)}')
PY
ls -la "$OUT/1080-snowboarding.apk"
echo "build-apk: $OUT/1080-snowboarding.apk ($VERSION, $ABIS)"
