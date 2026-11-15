#include "common.h"

extern s32 func_80009000(void);

/* osSpRawReadIo */
/* 2026-06-10 kernel-relayout: ROM fn starts at __osSpRawReadIo (addiu included); ROM callers jal func_80008BB4 (alt entry) - that name is a link-time absolute in undefined_syms_auto.txt. */
s32 __osSpRawReadIo(s32 devAddr, s32* data) {
    if (func_80009000() != 0) {
        return -1;
    }
    *data = *(volatile s32*)(0xA0000000 | devAddr);
    return 0;
}

/* __rmonGetSRegCount — check SP status for halt/break/signal bits */
s32 __osSpDeviceBusy(s32 status) {
    char pad[4];
    status = *(volatile s32*)0xA4040010;
    if (status & 0x1C) {
        return 1;
    }
    return 0;
}
