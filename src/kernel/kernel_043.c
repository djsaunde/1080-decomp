#include "common.h"
#include "libultra/rmon.h"

extern s32 func_800066F0(void*, s32, s32);

/* __rmonSendHeader (rmonmain.c). Named k___rmonSendHeader: the plain name
 * is the +0x400 runtime call alias (undefined_syms_auto.txt). */
void k___rmonSendHeader(KKHeader* block, s32 size, s32 type) {
    s32 totalSent;
    char* p = (char*)block;

    block->rev = KK_REV;
    block->type = type;
    totalSent = 0;
    if (size != 0) {
        do {
            register s32 sent = func_800066F0(p + totalSent, size - totalSent, 8);
            totalSent += sent;
        } while ((u32)totalSent < (u32)size);
    }
}

extern void __rmonSendHeader(KKHeader*, s32, s32);
extern void func_80005534(void);

/* writes the packet length arg1 into block->length, copies 4 bytes from arg1's
 * stack slot via func_800066F0, then __rmonMaskIdleThreadInts + func_80005534. */
void __rmonSendReply(KKHeader* block, s32 arg1, s32 arg2) {
    char* p;
    s32 totalRecv;
    totalRecv = 0;
    block->length = arg1;
    p = (char*)&arg1;
    if (totalRecv < 4) {
        do {
            register s32 recv = func_800066F0(p + totalRecv, 4 - totalRecv, 8);
            totalRecv += recv;
        } while (totalRecv < 4);
    }
    __rmonSendHeader(block, arg1, arg2);
    func_80005534();
}
