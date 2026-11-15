#include "common.h"

/* libultra libc/ll.c 64-bit helpers, named k___<name>: the plain names are
 * the +0x400 runtime call alias pins (docs/USO_FORMAT.md section 1). Compile at -O2 -mips3 so that IDO inlines the
 * d-arithmetic instructions (dsrlv/dsllv/ddivu/dmultu/...) instead of
 * emitting `jal __ull_rshift` back to themselves.
 *
 * Warning "should not be used for ucode 32-bit compiles" is expected. */

u64 k___ull_rshift(u64 a, u64 b) { return a >> b; }  /* __ull_rshift */
u64 k___ull_rem(u64 a, u64 b) { return a % b; }   /* __ull_rem */
u64 k___ull_div(u64 a, u64 b) { return a / b; }   /* __ull_div */
u64 k___ll_lshift(u64 a, u64 b) { return a << b; }  /* __ll_lshift */
u64 k___ll_rem(u64 a, u64 b) { return a % b; }   /* __ll_rem (unsigned left operand) */
s64 k___ll_div(s64 a, s64 b) { return a / b; }   /* __ll_div */
s64 k___ll_mul(s64 a, s64 b) { return a * b; }   /* __ll_mul */
/* 64-bit-by-short divmod. Outputs *quot and *rem of (val / d) and
 * (val % d). At -O1 -mips3 IDO emits 2 sequential ddivu (no CSE),
 * each with break-on-zero check. */
void k___ull_divremi(u64 *quot, u64 *rem, u64 val, u16 d) {
    *quot = val / d;
    *rem = val % d;
}
s64 k___ll_mod(s64 a, s64 b) {                   /* Euclidean mod */
    s64 r = a % b;
    if ((r < 0 && b > 0) || (r > 0 && b < 0)) r += b;
    return r;
}
s64 k___ll_rshift(s64 a, s64 b) { return a >> b; }  /* __ll_rshift */
