#include "common.h"
#include "libultra/rmon.h"

extern s32 rmon_breakpoints;
extern s32 rmon_altBreak_oldInstruction;
extern s32 rmon_altBreak;
extern void func_800031F0(void*, s32);
extern void func_80005350(void*, s32);






/* split from kernel_020.c - 2026-06-10 kernel ROM-order relayout */


/* func_800081D0: the no-prologue CONTINUATION (second half) of the single
 * logical function __rmonClearBreak. The matching bytes for 0x800081D0..0x80008264
 * are emitted by the full C body of rmon_ClearTempBreakpoint in kernel_040.c (verified
 * byte-exact, reloc-correct). kernel_040.c.o now compiles the WHOLE function
 * (TRUNCATE_TEXT=0x194) so the continuation is real compiled C, not asm.
 * The mid-function entry label func_800081D0 (jal'd from kernel_018_o2.c /
 * __rmonStopUserThreads) is provided as a fixed address symbol in symbol_addrs.txt —
 * splat would emit the same glabel. There is no standalone C body here because
 * the entry has no stack frame of its own (it reuses rmon_ClearTempBreakpoint's frame). */

#ifndef FW
#define FW(p, o) (*(s32 *)((char *)(p) + (o)))
#endif
extern s32 rmon_RCPbreakpoints;
extern void func_80006A50(s32, s32);
extern s32 func_80006A98(s32);
extern void func_800073F8(void *, s32, s32);
/* rmon set-breakpoint (__rmonSetBreak-class). Pick the instruction- or
 * data-breakpoint table (rmon_RCPbreakpoints / _0000 by arg0->unk9), scan its
 * 8-byte slots (offsets 0x8..0x80, addr at +0 / saved-insn at +4) for one holding the target
 * address (arg0->unk10) or a free slot; table full -> -0xA. For a fresh slot,
 * save the original word and patch in a BREAK insn (code = (slot>>3)+0x10) via
 * MMIO (func_80006A50/A98) or direct store + cache flush
 * (__osException/func_80005350), then record the address. Finally send a 0x18
 * reply (func_800073F8).
 *
 * STATUS 2026-07-10: EXACT (112/112) at IDO 5.3 -O1 — whole unit flipped to
 * $(IDO53_DIR)/cc in the Makefile (single-function unit, no donor needed).
 * The 2026-06-23 "permuter-immune coloring cap" (target computes each $a1
 * arg op sra/and/sll/ori into a fresh temp + `move $a1,tN` copy-back, 4
 * redundant moves; 7.1 -O1 folds them in place, 4 words short) is a 7.1-ONLY
 * fold — 5.3 -O1 emits the temp+move form from the identical C (same
 * discriminator as __rmonListThreads; see docs/IDO_CODEGEN.md 5.3-vs-7.1
 * residual-class entry). Two follow-up levers closed the tail: (1) the reply
 * struct is declared AFTER the register vars (decl-order slot lever: before
 * them, 5.3 homes it at sp+0x38 instead of the target's sp+0x28); (2) tail
 * store order tag -> zero -> idC -> idx -> saved reproduces the target's
 * ugen temp numbering (t0=tag, t1=idC, t2/t3=idx, t4=saved). The `register`
 * keyword pins arg0->s0 + the 3 table pointers->s1/s2/s3, and the reversed
 * loop-compare operand order (FW(s0,0x10) == *var_s2) fixes the commutative
 * load-order — both carried over from the 2026-06-23 rebuild. */
#define REQ(p) ((KKSetBkptRequest *)(p))
s32 __rmonSetBreak(void *arg0) {
    register void *s0 = arg0;
    register char *var_s1;
    register char *var_s2;
    register char *var_s3;
    KKBkptEvent reply;
    if (REQ(arg0)->header.method == 1) {
        var_s1 = (char *) &rmon_RCPbreakpoints;
        var_s2 = (char *) &rmon_RCPbreakpoints + 8;
        var_s3 = (char *) &rmon_RCPbreakpoints + 0x80;
    } else {
        var_s1 = (char *) &rmon_breakpoints;
        var_s2 = (char *) &rmon_breakpoints + 8;
        var_s3 = (char *) &rmon_breakpoints + 0x80;
    }
    while (var_s2 < var_s3) {
        if (*(s32 *)var_s2 == 0) {
            break;
        }
        if (REQ(s0)->addr == *(s32 *)var_s2) {
            break;
        }
        var_s2 += 8;
    }
    if (var_s2 == var_s3) {
        return -0xA;
    }
    if (*(s32 *)var_s2 == 0) {
        if (REQ(arg0)->header.method == 1) {
            *(s32 *)(var_s2 + 4) = func_80006A98(REQ(s0)->addr);
            func_80006A50(REQ(s0)->addr, ((((var_s2 - var_s1) >> 3) + 0x10) & 0xFFFFF) << 6 | 0xD);
        } else {
            *(s32 *)(var_s2 + 4) = *(s32 *)REQ(s0)->addr;
            *(s32 *)REQ(s0)->addr = ((((var_s2 - var_s1) >> 3) + 0x10) & 0xFFFFF) << 6 | 0xD;
            func_800031F0((void *) REQ(s0)->addr, 4);
            func_80005350((void *) REQ(s0)->addr, 4);
        }
        *(s32 *)var_s2 = REQ(s0)->addr;
    }
    reply.header.code = REQ(s0)->header.code;
    reply.header.error = 0;
    reply.object = REQ(s0)->object;
    reply.bp = (var_s2 - var_s1) >> 3;
    reply.instruction = *(s32 *)(var_s2 + 4);
    func_800073F8(&reply, 0x18, 1);
    return 0;
}
