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
extern void func_80005400(s32, s32);
extern void func_80009148(s32);
extern void func_80009030(s32, s32);
extern void func_80008498(void);
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


/* __rmonWriteWordTo (unimplemented stub) */
s32 func_80008424(s32 arg0) {
    return -1;
}


/* func_80008430 = __rmonClearBreak (libultra rmon/rmonbrk.c), IDO 7.1
 * -O1, EXACT. One function [0x8430,0x858C); it used to be four .s pieces
 * across two units (8430+alabel 8454 here, 8498/84AC/84D0 at the head of
 * kernel_022, joined by absolute .L label syms). The jal'd 0x80008454 /
 * 0x80008498 / 0x8000857C are other rmon functions 0x400 lower (kernel
 * jal targets sit 0x400 above our symbol numbering) and stay absolute
 * undefined_syms entries. Callees, real names: 6A98 __rmonReadWordAt,
 * 6A50 __rmonWriteWordTo, 31F0 osWritebackDCache, 5350 osInvalICache,
 * 73F8 __rmonSendReply. rmonbrk_bss_0088 = rspBreakpoints,
 * rmonbrk_bss_0000 = breakpoints (16 x {addr, old instruction}). */
typedef struct { u32 *breakAddress; u32 oldInstruction; } BreakInfo;
typedef struct { s32 pad0; u8 code; u8 pad5[4]; u8 method; u8 padA[2]; s32 object; s32 bp; } ClrBkptRequest;
typedef struct { s32 pad0; u8 code; u8 pad5; s16 error; s32 pad8; s32 object; s32 bp; s32 pad14; } BkptReply;
extern BreakInfo rmonbrk_bss_0088[];
extern BreakInfo rmonbrk_bss_0000[];
extern u32 func_80006A98(u32 *);
extern void func_80006A50(u32 *, u32);
extern void func_800031F0(void *, s32);
extern void func_80005350(void *, s32);
extern void func_800073F8(void *, s32, s32);

s32 func_80008430(ClrBkptRequest *req) {
    register ClrBkptRequest *request = req;
    register BreakInfo *whichBreak;
    BkptReply reply;
    u32 inst;

    if (request->bp >= 16) {
        return -2;
    }
    if (req->method == 1) {
        whichBreak = &rmonbrk_bss_0088[request->bp];
        if (!whichBreak->breakAddress) {
            return -2;
        }
        inst = func_80006A98(whichBreak->breakAddress);
        if ((inst & 0xFC00003F) == 0xD) {
            func_80006A50(whichBreak->breakAddress, whichBreak->oldInstruction);
        }
    } else {
        whichBreak = &rmonbrk_bss_0000[request->bp];
        if (!whichBreak->breakAddress) {
            return -2;
        }
        inst = *whichBreak->breakAddress;
        if ((inst & 0xFC00003F) == 0xD) {
            *whichBreak->breakAddress = whichBreak->oldInstruction;
            func_800031F0(whichBreak->breakAddress, 4);
            func_80005350(whichBreak->breakAddress, 4);
        }
    }
    whichBreak->breakAddress = 0;
    reply.code = request->code;
    reply.error = 0;
    reply.object = request->object;
    reply.bp = request->bp;
    func_800073F8(&reply, 0x18, 1);
    return 0;
}
