#include "common.h"

extern s32 func_80006A98(s32);
extern void func_80006A50(s32, s32);
extern s32 D_8001FF80;
extern s32 D_8001FF84;
extern s32 D_8001FF88;

/* SetUpForRCPop (rmonregs.c static): save SP DMEM/regs before an RCP register op */
/* 2026-06-10 kernel-relayout: ROM fn starts at rmon_SetUpForRCPop (addiu included); ROM callers jal func_80008D4C (alt entry) - that name is a link-time absolute in undefined_syms_auto.txt. */
void rmon_SetUpForRCPop(s32 extended) {
    D_8001FF80 = func_80006A98(0x04080000);
    D_8001FF84 = func_80006A98(0x04001000);
    *(s32*)&D_8001FF88 = func_80006A98(0x04000000);
    if (extended != 0) {
        *((s32*)&D_8001FF88 + 1) = func_80006A98(0x04000004);
        *((s32*)&D_8001FF88 + 2) = func_80006A98(0x04000008);
        *((s32*)&D_8001FF88 + 3) = func_80006A98(0x0400000C);
    }
}

/* __rmonRestoreRegs — mirror of __rmonSaveRegs (rmon_SetUpForRCPop).
 * NOTE: third restore (0x0400000C) reads from D_8001FF88[2] not [3] —
 * matches a bug in original asm @ 0x80008E60: `lw a1, 0x8(t0)` where t0 is
 * &D_8001FF88. Save pairs SP_DMEM[3] with D_88[3] but restore reuses D_88[2]. */
void rmon_CleanupFromRCPop(s32 extended) {
    func_80006A50(0x04000000, *(s32*)&D_8001FF88);
    if (extended != 0) {
        func_80006A50(0x04000004, *((s32*)&D_8001FF88 + 1));
        func_80006A50(0x04000008, *((s32*)&D_8001FF88 + 2));
        func_80006A50(0x0400000C, *((s32*)&D_8001FF88 + 2));
    }
    func_80006A50(0x04001000, D_8001FF84);
    func_80006A50(0x04080000, D_8001FF80);
}
