#include "common.h"

/* Forward declarations */
extern s32 __osPiAccessQueue;
extern s32 D_8000A480;
extern s32 D_80013004;
extern void func_80005DC0(s32*, s32, s32);
extern void func_80005C00(void);
extern void func_80004FE0(s32*, s32*, s32);
extern void func_800053D0(s32*, s32*, s32);
extern s32 D_8000A3E0;
extern s32 func_800066B0(void);
extern void func_800066D0(s32);
extern void func_80003D0C(s32*);
extern void* D_8000A420;
extern s32 D_8000A418;
extern s32 siacs_bss_0000;
extern s32 func_800009D8(void*, s32, s32, void*);
extern void func_80003FF0(s32, void*);
extern void* func_80003E54(void*);
extern void func_8000A110(void*);
extern void func_8000857C(void);
extern void func_8000785C(s32);
extern void func_80008ED0(void);
extern void func_80007760(void);
typedef void (*FuncPtr2)(void*, void*);
extern FuncPtr2 D_80012C44;
extern s32 D_8000A32C;
extern s32 D_8000A340;
extern s32 D_80012BC0;
extern s32 D_8000A2E0;
extern s32 D_8000A41C;
extern s32 func_80002890(s32);
extern void func_8000A0E0(void);
extern void func_80005350(s32, s32);
extern void func_80005400(s32, s32);
extern s32 func_80008430(s32);
extern void func_80009148(s32);
extern void func_80009030(s32, s32);
extern void func_80008498(void);
extern s32 func_80006A98(s32);
extern void func_800091F0(s32);
typedef struct { s32 pad[3]; s32 position; } FileState;

typedef struct {
    void* mtqueue;
    void* fullqueue;
    s32 validCount;
    s32 first;
    s32 msgCount;
    s32** msg;
} OSMesgQueue;

typedef struct Thread {
    s32 field0;
    s32 pri;
    s32 queue;
    s32 pad0C;
    u16 state;
} Thread;

typedef struct { s32 queue; s32 msg; } OSEventState;
extern OSEventState __osEventStateTab[];



/* split from kernel_022.c - 2026-06-10 kernel ROM-order relayout */





#ifndef FW
#define FW(p, o) (*(s32 *)((char *)(p) + (o)))
#endif
extern void *func_800077DC(s32);
extern void func_800073F8();

/* func_80008E98 = __rmonGetGRegisters-style register FETCH (libultra rmon),
 * IDO 7.1 -O1: read the thread context's 64-bit GPRs (low words) into a
 * 0xA4-byte reply, copy the 3 special registers, send it (jal func_800073F8
 * = __rmonSendHeader at our 0x6FF8; jal func_800077DC = __rmonGetTCB at our
 * 0x73DC). Returns -2 on a non-CPU request or a missing thread.
 * EXACT 2026-10-20 (agent-g run 112; was NM 88.9). The "alt entries"
 * func_80008EA0/ED0/FB0 (alabels in the .s, now dropped) are not entries:
 * kernel jal targets sit 0x400 above our numbering. What fixed the frame:
 * - -O1 frame layout is DECLARATION ORDER, top-down, and every `register`
 *   local also takes a ghost slot at its declaration position: msg first
 *   (ghost at the top), then hdr, ctx, src, and `register i` LAST (ghost at
 *   the bottom). That is the target's hdr@0x30 / ctx@0x2C / src@0x28.
 * - the tail stores go through `*(s32 *)((char *)&hdr + K)`: &hdr is
 *   rematerialized per store and ctx reloaded per read (the stores may alias
 *   it);
 * - the -2 for a non-CPU request is an `else { return -2; }` arm and the
 *   send is in the common tail (block order). */
s32 func_80008E98(void *arg0) {
    register s32 *msg = arg0;
    struct {
        s32 w0;
        u8 tag;     /* +4 */
        u8 p5;
        s16 zero;   /* +6 */
        s32 w8;
        s32 id;     /* +C */
        s32 reg[0x22]; /* +0x10 .. +0x98 */
        s32 sp98;
        s32 sp9C;
        s32 spA0;
    } hdr;
    void *ctx;
    s64 *src;
    register s32 i;

    hdr.id = msg[3];
    hdr.tag = ((u8 *)msg)[4];
    hdr.zero = 0;
    if (((u8 *)msg)[9] == 0) {
        ctx = func_800077DC(msg[3]);
        if (ctx == NULL) {
            return -2;
        }
        src = (s64 *)((char *)ctx + 0x20);
        for (i = 1; i < 0x1A; i++) {
            hdr.reg[i] = (s32)*src;
            src++;
        }
        src = (s64 *)((char *)ctx + 0xE8);
        for (i = 0x1C; i < 0x22; i++) {
            hdr.reg[i] = (s32)*src;
            src++;
        }
        *(s32 *)((char *)&hdr + 0x98) = FW(ctx, 0x120);
        *(s32 *)((char *)&hdr + 0x9C) = FW(ctx, 0x11C);
        *(s32 *)((char *)&hdr + 0xA0) = FW(ctx, 0x118);
        *(s32 *)((char *)&hdr + 0x10) = 0;
    } else {
        return -2;
    }
    func_800073F8(&hdr, 0xA4, 1);
    return 0;
}

/* func_80008FF4 = the matching register STORE (inverse of 8E98), IDO 7.1
 * -O1: write the request's GPRs into the thread context as sign-extended
 * 64-bit values, copy the 3 special registers, send a 0x10 ack.
 * EXACT 2026-10-20 (agent-g run 112; was NM 83.5) with the same recipe as
 * 8E98 (decl order msg, hdr, ctx, p, `register i` last; else-return arm;
 * header filled in the tail as id, tag, zero). */
s32 func_80008FF4(void *arg0) {
    register s32 *msg = arg0;
    struct { s32 w0; u8 tag; u8 p5; s16 zero; s32 w8; s32 id; } hdr;
    void *ctx;
    s64 *p;
    register s32 i;

    if (((u8 *)msg)[9] == 0) {
        ctx = func_800077DC(msg[3]);
        if (ctx == NULL) {
            return -2;
        }
        p = (s64 *)((char *)ctx + 0x20);
        for (i = 1; i < 0x1A; i++) {
            *p = (s64)msg[i + 4];
            p++;
        }
        p = (s64 *)((char *)ctx + 0xE8);
        for (i = 0x1C; i < 0x22; i++) {
            *p = (s64)msg[i + 4];
            p++;
        }
        FW(ctx, 0x120) = msg[0x26];
        FW(ctx, 0x11C) = msg[0x27];
        FW(ctx, 0x118) = msg[0x28];
    } else {
        return -2;
    }
    hdr.id = msg[3];
    hdr.tag = ((u8 *)msg)[4];
    hdr.zero = 0;
    func_800073F8(&hdr, 0x10, 1);
    return 0;
}
