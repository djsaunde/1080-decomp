#include "common.h"

/* 2026-06-10 kernel-relayout: kernel_051 (__rmonSendFault), kernel_050
 * (__rmonIOflush), kernel_052 (__rmonIOputw) and kernel_055
 * (__rmonIOhandler) merged into ONE unit in ROM order [0x50C0..0x52C0).
 * They are one original TU: the functions are back-to-back with NO
 * inter-fn padding (separate .o files would 16-align each), and
 * __rmonIOhandler's unreachable epilogue is 32-byte-aligned by IDO from
 * the TU-relative offset — it only lands pad-free when the function
 * sits at TU offset 0x130, i.e. after the other three. */

extern s32 rmon_IOmq;
extern s32 rmon_IOmsgs;
extern u8 rmonRdbReadBuf;
extern void* __osRdb_DbgRead_Buf;
extern void func_800053D0(s32*, s32*, s32);
extern void func_800051E0(s32, s32*, s32);
extern void func_80004FE0(s32*, s32*, s32);
extern void func_800069BC(u8*);
extern s32 func_800066F0(void*, s32, s32);

/* Receive 0x1B0 bytes into arg0 buffer via repeated func_800066F0 calls.
 * The unused float local (`dummy = 0.0f`) is preserved from the original
 * — IDO emits the mtc1+swc1 even though the value is never read. */
void __rmonSendFault(void* arg0) {
    f32 dummy;
    void* p;
    s32 totalRecv;
    dummy = 0.0f;
    p = arg0;
    totalRecv = 0;
    do {
        register s32 recv = func_800066F0((char*)p + totalRecv, 0x1B0 - totalRecv, 2);
        totalRecv += recv;
    } while ((u32)totalRecv < 0x1B0);
}

/* Polls func_800066F0 until it returns a positive value. Used to wait
 * for a single byte of host I/O. */
void __rmonIOflush(void) {
    s32 totalRecv;
    s32 buf;
    totalRecv = 0;
    do {
        register s32 recv = func_800066F0(&buf, 1, 10);
        totalRecv += recv;
    } while (totalRecv <= 0);
}

/* Receive 4 bytes from host I/O into the caller's arg0 stack slot. */
void __rmonIOputw(s32 arg0) {
    s32 totalRecv;
    char* p;
    totalRecv = 0;
    p = (char*)&arg0;
    do {
        register s32 recv = func_800066F0(p + totalRecv, 4 - totalRecv, 8);
        totalRecv += recv;
    } while (totalRecv < 4);
}

/* __rmonIOhandler (rmonsio.c): rmon I/O thread, runs __rmonExecute per request. */
void __rmonIOhandler(void) {
    s32 total;
    s32 buf;

    func_800053D0(&rmon_IOmq, &rmon_IOmsgs, 1);
    func_800051E0(0x14, &rmon_IOmq, 0);
    __osRdb_DbgRead_Buf = &rmonRdbReadBuf;
loop:
    func_80004FE0(&rmon_IOmq, 0, 1);
    func_800069BC(&rmonRdbReadBuf);
    __osRdb_DbgRead_Buf = &rmonRdbReadBuf;
    total = 0;
    /* do-while: the top-test while form emitted 4 dead nops before the
     * unreachable epilogue (+0x10 vs ROM) — caught in the 2026-06-10
     * kernel relayout audit. Entry is guaranteed (total == 0). */
    do {
        register s32 n = func_800066F0(&buf, 1, 0xB);
        total += n;
    } while (total <= 0);
    goto loop;
}
