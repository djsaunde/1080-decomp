#!/usr/bin/env bash
# MIPSpro 7.2.1 C++ front end (o32 edgcpfe, EDG 2.29, Mar 13 1998) followed by the IDO 7.1 back end
# (uopt / ugen / as1 static recomps) -- the same phases, flags and back end as the IDO 7.1 `NCC -c -G 0
# -non_shared -Wab,-r4300_mul -O2 -mips2 -32` driver line (`NCC -show`), with only the front end swapped.
#
# usage: tools/ncc721.sh <in.cpp> <out.o> [-O2|-O1|-O0]
#
# For per-function donors only (docs/MATCHING_WORKFLOW.md "MIPSpro 7.2.1 donor recipe"). The front end
# is evidenced for the 4-label jump-table family (docs/TOOLING_DECOMP.md "Toolchain forensics III");
# it is NOT the front end of the existing IDO 7.1 NCC donors (87 / 214 change under it), so never
# move an existing donor to it.
# Install the front end with tools/fetch-mipspro721.sh. Overrides: MIPSPRO721_ROOT, IDO71_DIR.
set -euo pipefail
IN="$1"; OUT="$2"; OPT="${3:--O2}"
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="${MIPSPRO721_ROOT:-$HERE/ido-static-recomp/build/mipspro721}"
BE="${IDO71_DIR:-$HERE/ido-static-recomp/build/7.1/out}"
QEMU="$ROOT/usr/bin/qemu-irix"
FE="$ROOT/usr/lib/DCC/edgcpfe"
if [[ ! -x "$QEMU" || ! -f "$FE" ]]; then
    echo "ncc721.sh: MIPSpro 7.2.1 edgcpfe not installed at $ROOT -- run tools/fetch-mipspro721.sh" >&2
    exit 1
fi
OUTDIR="$(cd "$(dirname "$OUT")" && pwd)"
IN_ABS="$(cd "$(dirname "$IN")" && pwd)/$(basename "$IN")"
# intermediates live next to the output (unique per object, so -j is safe)
W="$OUTDIR/$(basename "$OUT").mp721"
rm -f "$W.B" "$W.O" "$W.G" "$W.S" "$W.os" "$W.gt" "$W.err" "$W.ii"
"$QEMU" -silent -L "$ROOT" "$FE" \
    -D_MIPS_FPSET=16 -D_MIPS_ISA=2 -D_ABIO32=1 -D_MIPS_SIM=_ABIO32 -D_MIPS_SZINT=32 -D_MIPS_SZLONG=32 \
    -D_MIPS_SZPTR=32 -YE -D__EXTENSIONS__ -D_LANGUAGE_C_PLUS_PLUS=1 -D__cplusplus=1 -D__ANSI_CPP__=1 \
    -D__INLINE_INTRINSICS -Dsgi -D__sgi -Dunix -Dmips -Dhost_mips -D__unix -D__host_mips -D_SVR4_SOURCE \
    -D_MODERN_C -D_SGI_SOURCE -D__DSO__ -DSYSTYPE_SVR4 -D_SYSTYPE_SVR4 -D_LONGLONG -D__mips=2 -D_MIPSEB \
    -DMIPSEB -D__STDC__=1 -I"$(dirname "$IN_ABS")" \
    -O -b -Zz -ZO -ZS"$W.S" -Qn -u -YN"$(basename "$OUT")" -YO"$W.ii" -YG0 -Zg0 "$IN_ABS" \
    > "$W.B" 2> "$W.err" || { cat "$W.err" >&2; exit 1; }
if [[ "$OPT" == "-O2" ]]; then
    "$BE/uopt" -G 0 -mips2 -EB -g0 -O2 "$W.B" "$W.O" -t "$W.S" "$W.os" > /dev/null
else
    cp "$W.B" "$W.O"
fi
"$BE/ugen" -G 0 -mips2 -EB -g0 "$OPT" "$W.O" -o "$W.G" -t "$W.S" -temp "$W.gt" > /dev/null
"$BE/as1" -t5_ll_sc_bug -elf -G 0 -p0 -r4300_mul -r4300_mul -mips2 -EB -g0 "$OPT" "$W.G" -o "$OUT" -t "$W.S" > /dev/null
rm -f "$W.B" "$W.O" "$W.G" "$W.S" "$W.os" "$W.gt" "$W.err" "$W.ii"
