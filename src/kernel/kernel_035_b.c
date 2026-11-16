#include "common.h"
#include "libultra/os_thread.h"

extern s32 func_80006A98(s32);
extern void func_80006A50(s32, s32);
extern void func_80007A98(s32, s32, void*);
extern void func_80008D0C(void*);
extern void func_800073F8(void*, s32, s32);
extern u8 __rmonRcpAtBreak;

/* split from kernel_035.c - 2026-06-10 kernel ROM-order relayout */


/* __rmonHitCpuBreak — walk thread list checking for breakpoints */
extern s32 func_80009C30(void);
extern void func_80008D4C(s32, s32);
extern void func_800054C0(void*);

void rmonFindFaultedThreads(void) {
    register s32* t = (s32*)func_80009C30();

    while (((OSThread *)t)->priority != -1) {
        if (((OSThread *)t)->priority > 0 && ((OSThread *)t)->priority < 0x80) {
            if (((OSThread *)t)->flags & 1) {
                s32 instr = *(s32 *)((OSThread *)t)->context.pc;
                if ((instr & (s32)0xFC00003F) == 0xD) {
                    func_80008D4C(((OSThread *)t)->id, instr >> 6);
                } else {
                    func_80008D4C(((OSThread *)t)->id, 0);
                }
            }
            if (((OSThread *)t)->flags & 2) {
                func_800054C0(t);
                func_80008D4C(((OSThread *)t)->id, 0xF);
            }
        }
        t = (s32 *)((OSThread *)t)->tlnext;
    }
}
