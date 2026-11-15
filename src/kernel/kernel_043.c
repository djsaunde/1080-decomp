#include "common.h"

extern s32 func_800066F0(void*, s32, s32);

/* __rmonSendData */
void k___rmonSendHeader(char* buf, s32 size, s32 domain) {
    s32 totalSent;
    char* p = buf;

    *(u8*)(buf + 8) = 2;
    *(u8*)(buf + 5) = domain;
    totalSent = 0;
    if (size != 0) {
        do {
            register s32 sent = func_800066F0(p + totalSent, size - totalSent, 8);
            totalSent += sent;
        } while ((u32)totalSent < (u32)size);
    }
}

extern void __rmonSendHeader(s32*, s32, s32);
extern void func_80005534(void);

/* writes the 32-bit value arg1 at *arg0, copies 4 bytes from arg1's
 * stack slot via func_800066F0, then __rmonMaskIdleThreadInts + func_80005534. */
void __rmonSendReply(s32* arg0, s32 arg1, s32 arg2) {
    char* p;
    s32 totalRecv;
    totalRecv = 0;
    *arg0 = arg1;
    p = (char*)&arg1;
    if (totalRecv < 4) {
        do {
            register s32 recv = func_800066F0(p + totalRecv, 4 - totalRecv, 8);
            totalRecv += recv;
        } while (totalRecv < 4);
    }
    __rmonSendHeader(arg0, arg1, arg2);
    func_80005534();
}
