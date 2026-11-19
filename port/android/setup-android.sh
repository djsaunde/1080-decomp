#!/usr/bin/env bash
# port/android/setup-android.sh (plan section 31): the Android toolchain, no sudo, into tools/android (gitignored):
#   jdk/        Temurin 17 (Gradle 8 / AGP 8 need 17), unless JAVA_HOME already names a JDK 17+
#   sdk/        the command-line tools, platform 34, build-tools 34.0.0, platform-tools (adb), NDK r27c, CMake 3.22.1
#   src/        SDL2's release source (its android-project: SDLActivity's Java, the Gradle wrapper)
#   gradle-home GRADLE_USER_HOME for build-apk.sh (the wrapper's Gradle and AGP's Maven downloads)
# The SDK licences are accepted non-interactively (yes | sdkmanager --licenses): a personal build.
# Idempotent; ANDROID_TOOLS overrides the location. SKIP_JDK=1 uses $JAVA_HOME (CI's setup-java).
set -euo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
GAME="$(cd "$HERE/../.." && pwd)"
REPO_ROOT="$(cd "$GAME/../.." && pwd)"
T="${ANDROID_TOOLS:-$REPO_ROOT/tools/android}"
mkdir -p "$T/dl" "$T/src"

CMDLINE=commandlinetools-linux-11076708_latest.zip
CMDLINE_SHA=2d2d50857e4eb553af5a6dc3ad507a17adf43d115264b1afc116f95c92e5e258
JDK=OpenJDK17U-jdk_x64_linux_hotspot_17.0.13_11.tar.gz
JDK_URL="https://github.com/adoptium/temurin17-binaries/releases/download/jdk-17.0.13%2B11/$JDK"
JDK_SHA=8682892fc02965930b9022c066fa164dd6f458ef4a5dc262016aa28333b30f49
SDL_VER=2.30.9
SDL=SDL2-$SDL_VER.tar.gz
SDL_SHA=24b574f71c87a763f50704bbb630cbe38298d544a1f890f099a4696b1d6beba4
NDK_VER=27.2.12479018 # r27c
PKGS=("platforms;android-34" "build-tools;34.0.0" "platform-tools" "ndk;$NDK_VER" "cmake;3.22.1")

fetch() { # url file sha256
    if [[ ! -f "$T/dl/$2" ]] || ! echo "$3  $T/dl/$2" | sha256sum -c --quiet - 2>/dev/null; then
        echo "setup-android: downloading $2"
        curl -sSfL -o "$T/dl/$2.part" "$1"
        echo "$3  $T/dl/$2.part" | sha256sum -c --quiet -
        mv "$T/dl/$2.part" "$T/dl/$2"
    fi
}

# unzip that keeps the executable bits (no unzip on this machine; python's zipfile drops the modes)
unzip_to() { # zip dir
    python3 -I - "$1" "$2" <<'EOF'
import os, sys, zipfile
z = zipfile.ZipFile(sys.argv[1])
for i in z.infolist():
    p = z.extract(i, sys.argv[2])
    m = (i.external_attr >> 16) & 0o777
    if m:
        os.chmod(p, m)
EOF
}

# JDK
if [[ "${SKIP_JDK:-0}" == 1 ]]; then
    JAVA_HOME="${JAVA_HOME:?SKIP_JDK=1 needs JAVA_HOME}"
else
    if [[ ! -x "$T/jdk/jdk-17.0.13+11/bin/java" ]]; then
        fetch "$JDK_URL" "$JDK" "$JDK_SHA"
        mkdir -p "$T/jdk"
        tar xzf "$T/dl/$JDK" -C "$T/jdk"
    fi
    JAVA_HOME="$T/jdk/jdk-17.0.13+11"
fi
export JAVA_HOME

# SDK command-line tools + packages
SDK="$T/sdk"
if [[ ! -x "$SDK/cmdline-tools/latest/bin/sdkmanager" ]]; then
    fetch "https://dl.google.com/android/repository/$CMDLINE" "$CMDLINE" "$CMDLINE_SHA"
    rm -rf "$SDK/cmdline-tools/tmp"
    mkdir -p "$SDK/cmdline-tools/tmp"
    unzip_to "$T/dl/$CMDLINE" "$SDK/cmdline-tools/tmp"
    mv "$SDK/cmdline-tools/tmp/cmdline-tools" "$SDK/cmdline-tools/latest"
    rmdir "$SDK/cmdline-tools/tmp"
fi
SDKM="$SDK/cmdline-tools/latest/bin/sdkmanager"
if [[ ! -d "$SDK/ndk/$NDK_VER" || ! -d "$SDK/build-tools/34.0.0" || ! -d "$SDK/platforms/android-34" ||
      ! -x "$SDK/platform-tools/adb" || ! -x "$SDK/cmake/3.22.1/bin/cmake" ]]; then
    (yes || true) | "$SDKM" --sdk_root="$SDK" --licenses > "$T/sdkmanager-licenses.log" 2>&1 || true
    "$SDKM" --sdk_root="$SDK" "${PKGS[@]}" > "$T/sdkmanager.log" 2>&1 || { tail -20 "$T/sdkmanager.log"; exit 1; }
fi

# SDL2 source (the android-project template)
if [[ ! -f "$T/src/SDL2-$SDL_VER/android-project/gradlew" ]]; then
    fetch "https://github.com/libsdl-org/SDL/releases/download/release-$SDL_VER/$SDL" "$SDL" "$SDL_SHA"
    tar xzf "$T/dl/$SDL" -C "$T/src"
fi

mkdir -p "$T/gradle-home"
cat > "$T/env.sh" <<EOF
# source this: the Android toolchain of port/android/setup-android.sh
export JAVA_HOME="$JAVA_HOME"
export ANDROID_HOME="$SDK"
export ANDROID_SDK_ROOT="$SDK"
export ANDROID_NDK_HOME="$SDK/ndk/$NDK_VER"
export GRADLE_USER_HOME="$T/gradle-home"
export SDL2_SRC="$T/src/SDL2-$SDL_VER"
export PATH="\$JAVA_HOME/bin:$SDK/platform-tools:$SDK/build-tools/34.0.0:\$PATH"
EOF
echo "setup-android: JDK $JAVA_HOME, SDK $SDK (NDK $NDK_VER), SDL2 $SDL_VER source; environment in $T/env.sh"
