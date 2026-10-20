#include "common.h"

extern s32 func_80007A98(s32, s32, void*);
extern void func_800073F8(void*, s32, s32);

typedef struct { s32 pad0; u8 type; char pad5; s16 flags; s32 data[17]; } RmonBuf76;
typedef struct { s32 pad0; u8 type; char pad5[4]; u8 domain; char padA[2]; s32 id; } RmonMsg;

/* __rmonGetGRegisters */
s32 func_8000798C(RmonMsg* msg) {
    RmonMsg* p = msg;
    RmonBuf76 buf;
    if (func_80007A98(p->domain, p->id, &buf) != 0) {
        return -2;
    }
    buf.type = p->type;
    buf.flags = 0;
    func_800073F8(&buf, 0x4C, 1);
    return 0;
}

/* func_800079F4 = __rmonStopThread (libultra rmon), IDO 7.1 -O1, EXACT.
 * One function [0x79F4,0x7B3C): it used to be split across units, with
 * the head here (no jr ra, "tail-jumps into func_80007A98's shared
 * return") and the body as func_80007A98 at the head of kernel_018_g
 * (no prologue, starting with this head's last bnez delay-slot nop).
 * The jal'd 0x80007A98 is NOT that body: every kernel jal target sits
 * 0x400 above our symbol numbering (the kernel really runs at 0x80000400),
 * so jal func_80007A98 = __rmonGetThreadStatus = func_80007698. It stays
 * an absolute undefined_syms_auto.txt entry.
 * Callees (all +0x400 names): 785C __rmonStopUserThreads, 8430
 * __rmonRCPrunning, 8454 __rmonIdleRCP, 6A98 __rmonReadWordAt(SP_PC_REG),
 * 898C (1, 1000, SP_IMEM + pc - 4), 8498 __rmonStepRCP, 73F8
 * __rmonSendReply. Shape copied from __rmonGetGRegisters above (msg home
 * + `p` copy, reply buffer 0x4C). */
extern void func_8000785C(s32);
extern s32 func_80008430(void);
extern void func_80008454(void);
extern u32 func_80006A98(u32);
extern u32 func_8000898C(s32, s32, u32);
extern void func_80008498(void);
typedef struct { s32 pad0; u8 type; char pad5; s16 flags; s32 pad8; s32 why; s32 data[15]; } RmonStatusReply;

s32 func_800079F4(RmonMsg *msg) {
    RmonMsg *p = msg;
    RmonStatusReply buf;
    u32 pc;

    switch (msg->domain) {
    case 0:
        func_8000785C(p->id);
        break;
    case 1:
        if (func_80008430()) {
            func_80008454();
            pc = func_80006A98(0x04080000);
            if (pc == 0) {
                break;
            }
            pc -= 4;
            if (!(func_8000898C(1, 1000, pc + 0x04001000) & 3)) {
                func_80008498();
            }
        }
        break;
    default:
        return -4;
    }
    if (func_80007A98(msg->domain, p->id, &buf) != 0) {
        return -2;
    }
    buf.type = p->type;
    buf.flags = 0;
    func_800073F8(&buf, 0x4C, 1);
    if (buf.why == 1) {
        buf.type = 4;
        func_800073F8(&buf, 0x4C, 2);
    }
    return 0;
}
