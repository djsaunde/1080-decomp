#!/usr/bin/env bash
# Fetch + install the SGI MIPSpro 7.2.1 o32 C++ front end (edgcpfe, EDG 2.29, built Mar 13 1998)
# and the qemu-irix user-mode emulator + IRIX runtime it runs under.
#
# Used by tools/ncc721.sh (edgcpfe -> IDO 7.1 uopt/ugen/as1) for the per-function MIPSpro 7.2.1
# C++ donors (docs/MATCHING_WORKFLOW.md "MIPSpro 7.2.1 donor recipe"; provenance in
# docs/TOOLING_DECOMP.md "Toolchain forensics III"). Nothing here is committed: it installs under
# tools/ido-static-recomp/build/mipspro721/, which is gitignored and is reached through the same
# tools/ido-static-recomp symlink the worktrees and the landing gate's clean checkout already use.
#
# usage: tools/fetch-mipspro721.sh [DEST]
#   DEST           default: <project>/tools/ido-static-recomp/build/mipspro721
#   MIPSPRO_CACHE  optional dir holding already-downloaded archives (same basenames as below:
#                  c++_fe.idb, c++_fe.sw, mipspro7.4.4.tar.xz); every file is sha256-checked anyway.
# Idempotent: exits early when DEST/usr/lib/DCC/edgcpfe already has the pinned sha256.
set -euo pipefail

HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
DEST="${1:-$HERE/ido-static-recomp/build/mipspro721}"

# SGI Varsity Update 1998 August CD (discmaster collection 9077), dist/c++_fe = MIPSpro 7.2.1 C++ front end.
VARSITY="https://discmaster.textfiles.com/file/9077/SGI%20Varsity%20Update%201998%20August.iso/dist"
IDB_URL="$VARSITY/c%2B%2B_fe.idb"
IDB_SHA=e5aa2611352f8db296281a59e9d8b219904e44f27cb129f1a1adcfd34313e4e1
SW_URL="$VARSITY/c%2B%2B_fe.sw"
SW_SHA=6c6dfae3c42a270266b67d4e69c478c754ba10985e017ba09fa53200a6ff7423
EDG_SHA=459cc16e72de63fb17137cd7dc02f4bc211044e40e0956a92e81eeda47e1d9a3
# qemu-irix + IRIX rld / libc / libC: decomp.me's mips_pro_744 archive (decompme/compilers values.yaml).
QEMU_URL="https://github.com/LLONSIT/qemu-irix-helpers/raw/n/qemu/mipspro7.4.4.tar.xz"
QEMU_SHA=df33be0d9605b00b44070e567121fc00283a9b1f97763403f0898d2216028d24
RUNTIME="usr/bin/qemu-irix lib/rld lib/libc.so.1 usr/lib/libc.so.1 usr/lib/libC.so.1 usr/lib/libCsup.so"

sha() { sha256sum "$1" | awk '{print $1}'; }

if [[ -f "$DEST/usr/lib/DCC/edgcpfe" && "$(sha "$DEST/usr/lib/DCC/edgcpfe")" == "$EDG_SHA" \
      && -x "$DEST/usr/bin/qemu-irix" ]]; then
    echo ">>> MIPSpro 7.2.1 edgcpfe already installed at $DEST"
    exit 0
fi

TMP="$(mktemp -d)"
cleanup() {
    find "$TMP" -mindepth 1 -maxdepth 1 -type f -delete 2>/dev/null || true
    rmdir "$TMP" 2>/dev/null || true
}
trap cleanup EXIT

get() {  # get <url> <basename> <sha256>
    local url="$1" name="$2" want="$3" out="$TMP/$2"
    if [[ -n "${MIPSPRO_CACHE:-}" && -f "$MIPSPRO_CACHE/$name" ]]; then
        cp "$MIPSPRO_CACHE/$name" "$out"
    else
        echo ">>> downloading $url"
        curl --fail --location --silent --show-error "$url" --output "$out"
    fi
    if [[ "$(sha "$out")" != "$want" ]]; then
        echo "fetch-mipspro721: checksum mismatch for $name (got $(sha "$out"), want $want)" >&2
        exit 1
    fi
}

get "$IDB_URL" "c++_fe.idb" "$IDB_SHA"
get "$SW_URL" "c++_fe.sw" "$SW_SHA"
get "$QEMU_URL" "mipspro7.4.4.tar.xz" "$QEMU_SHA"

mkdir -p "$DEST/usr/lib/DCC"

# IRIX inst image: the .idb line for edgcpfe gives size / cmpsize; in c++_fe.sw each file is
# stored as <u16 len><path><data>, data = Unix compress (.Z) when cmpsize > 0 (gzip -dc reads it).
python3 -I - "$TMP/c++_fe.idb" "$TMP/c++_fe.sw" "$TMP/edgcpfe" <<'PY'
import re, subprocess, sys
idb, sw, out = sys.argv[1:4]
dst = "usr/lib/DCC/edgcpfe"
line = next(l for l in open(idb, encoding="latin1") if l.split()[:1] == ["f"] and l.split()[4] == dst)
size = int(re.search(r" size\((\d+)\)", line).group(1))
csize = int(re.search(r"cmpsize\((\d+)\)", line).group(1))
data = open(sw, "rb").read()
key = b"\x00" + bytes([len(dst)]) + dst.encode()
i = data.find(key)
assert i >= 0, "edgcpfe not found in c++_fe.sw"
blob = data[i + len(key): i + len(key) + (csize or size)]
if csize:
    blob = subprocess.run(["gzip", "-dc"], input=blob, capture_output=True).stdout
assert len(blob) == size, (len(blob), size)
open(out, "wb").write(blob)
PY
if [[ "$(sha "$TMP/edgcpfe")" != "$EDG_SHA" ]]; then
    echo "fetch-mipspro721: extracted edgcpfe sha256 mismatch" >&2
    exit 1
fi
install -m 0755 "$TMP/edgcpfe" "$DEST/usr/lib/DCC/edgcpfe"

# shellcheck disable=SC2086
tar -xJf "$TMP/mipspro7.4.4.tar.xz" -C "$DEST" $RUNTIME
chmod +x "$DEST/usr/bin/qemu-irix" "$DEST/lib/rld"

echo ">>> MIPSpro 7.2.1 edgcpfe (EDG 2.29, Mar 13 1998) installed at $DEST"
