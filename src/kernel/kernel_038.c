#include "common.h"
#include "libultra/os_thread.h"

extern s32 D_80003FE0;          /* __osCleanupThread (thread return address) */
extern OSThread *__osActiveQueue;    /* __osActiveQueue */
extern s32 func_800066B0(void); /* __osDisableInt */
extern void func_800066D0(s32); /* __osRestoreInt */

/* osCreateThread. STRUCT PROTOTYPE (agent-a run 441): the raw
 * `*(T*)((char*)t + 0xNN)` stores became OSThread field stores in the SAME
 * statement order with the SAME access widths -- byte-identical (see
 * docs/READABILITY_PLAN.md "struct path"). */
void osCreateThread(OSThread *t, OSId id, void *entry, s32 arg, void *sp, OSPri p) {
    register s32 saveMask;
    /* u32: ROM emits srl (not sra) for (mask & 0x3F0000) >> 16 — the old
     * s32 decl was ONE WORD off ROM (sra), caught in the 2026-06-10 kernel
     * relayout audit. */
    u32 mask;
    t->id = id;
    t->priority = p;
    t->next = NULL;
    t->queue = NULL;
    t->context.pc = (u32)entry;
    t->context.a0 = (s64)arg;
    t->context.sp = (s64)(s32)sp - 16;
    t->context.ra = (s64)(s32)&D_80003FE0;
    mask = 0x3FFF01;
    t->context.sr = 0xFF03;
    t->context.rcp = (mask & 0x3F0000) >> 16;
    t->context.fpcsr = 0x01000800;
    t->fp = 0;
    t->state = OS_STATE_STOPPED;
    t->flags = 0;
    saveMask = func_800066B0();
    t->tlnext = __osActiveQueue;
    __osActiveQueue = t;
    func_800066D0(saveMask);
}
/* osCreateThread (osCreateThread): ALREADY MATCHED -- this C is the
 * real build path (no wrap) and the kernel ROM is byte-exact with it.
 * The report's 98.75 was an ARTIFACT: the NM-path .o goes through
 * asm-processor, which 16-aligns .text (0x150) and attributes a pad
 * nop to the symbol (0x144 vs true 0x140). Fixed via
 * NON_MATCHING_TRUNCATE_TEXT := 0x140 in the Makefile. All other insn
 * "diffs" were reloc-false. */

