#include "common.h"

/* Kernel entry point + boot main, split out of kernel_000.c (2026-10-20,
 * agent-g run 112). func_80000000.s [0x00,0x60) is the handwritten entry
 * stub (clear BSS, set sp, jump to runtime 0x80000460) plus its pad; the
 * kernel runs at 0x80000400, so that jump lands on func_80000060 here.
 * func_80000060 is plain IDO 7.1 -O2 C. Its callees use the +0x400 jal
 * names: func_80001994 = func_80001594 (USO loader init, given the memory
 * size), func_80000DEC = func_800009EC (load the first module, here the
 * boot USO named by D_8000A59C), func_80001088 = func_80000C88 (call its
 * entry). IDO pads this unit's .text to 0xB0, which is where kernel_000
 * starts. */

/* func_80000000: handwritten boot entry stub, real source in src/handwritten/kernel/entry.s (docs/MATCHING_WORKFLOW.md#handwritten-assembly-sources). */
GLOBAL_ASM("src/handwritten/kernel/entry.s")

typedef struct { s32 pad[4]; u32 unk10; } UsoSegDesc0;
extern UsoSegDesc0 D_8000A260;
extern u32 D_80000318;               /* osMemSize */
extern char D_8000A59C[];
extern void func_80001994(UsoSegDesc0 *, u32);
extern s32 func_80000DEC(char *);
extern s32 func_80001088(void);

void func_80000060(void) {
    D_8000A260.unk10 = D_80000318 + 0x80000000;
    func_80001994(&D_8000A260, D_80000318);
    func_80000DEC(D_8000A59C);
    func_80001088();
}
