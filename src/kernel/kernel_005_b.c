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


/* split from kernel_005.c - 2026-06-10 kernel ROM-order relayout */

typedef struct {
    u32 unk_00;
    u8 type;
    u8 latency;
    u8 pageSize;
    u8 relDuration;
    u8 pulse;
    u8 domain;
    u8 pad_0A[2];
    u32 baseAddress;
} OSPiHandle;

extern OSPiHandle *D_8000A470[];
extern s32 func_80004B30(void *);

/* func_800052F0 = osEPiRawStartDma (libultra io/epirawdma.c), IDO 7.1 -O1.
 * One function [0x52F0,0x5514); IDO pads .text to 16 with the 3 zero words.
 * It used to be split across two units: a 0x60 "entry block" here and the
 * rest as func_80005350 at the head of kernel_006 ("alt-entry shared body,
 * no honest C"). The jal'd 0x80005350/53D0/5400/54C0 addresses are stale
 * mid-body targets that stay as absolute undefined_syms_auto.txt entries
 * (same as __osLeoInterrupt's in kernel_000_b); merged into this unit.
 * EXACT via twin port of game_libs' copy gl_func_0006F8A4
 * (game_libs_ido53_6F8A4.c, 5.3 -O1; 7.1 -O1 emits the same bytes).
 * The old NM's miss was the PI registers: `extern u32 D_A4600010` lets IDO
 * hoist the first PI_STATUS load above the argument homing stores; raw
 * volatile literals keep it after them. __osCurrentHandle = D_8000A470,
 * osVirtualToPhysical = func_80004B30. */
#define EPI_IO_READ(a) (*(volatile u32 *)(a))
#define EPI_IO_WRITE(a, v) (*(volatile u32 *)(a) = (v))

s32 func_800052F0(OSPiHandle *pihandle, s32 direction, u32 devAddr, void *dramAddr, u32 size)
{
    u32 stat;
    u32 domain;

    stat = EPI_IO_READ(0xA4600010);
    while (stat & 3)
        stat = EPI_IO_READ(0xA4600010);

    domain = pihandle->domain;
    if (D_8000A470[domain] != pihandle) {
        OSPiHandle *cHandle = D_8000A470[domain];
        if (domain == 0) {
            if (cHandle->latency != pihandle->latency)
                EPI_IO_WRITE(0xA4600014, pihandle->latency);
            if (cHandle->pageSize != pihandle->pageSize)
                EPI_IO_WRITE(0xA460001C, pihandle->pageSize);
            if (cHandle->relDuration != pihandle->relDuration)
                EPI_IO_WRITE(0xA4600020, pihandle->relDuration);
            if (cHandle->pulse != pihandle->pulse)
                EPI_IO_WRITE(0xA4600018, pihandle->pulse);
        } else {
            if (cHandle->latency != pihandle->latency)
                EPI_IO_WRITE(0xA4600024, pihandle->latency);
            if (cHandle->pageSize != pihandle->pageSize)
                EPI_IO_WRITE(0xA460002C, pihandle->pageSize);
            if (cHandle->relDuration != pihandle->relDuration)
                EPI_IO_WRITE(0xA4600030, pihandle->relDuration);
            if (cHandle->pulse != pihandle->pulse)
                EPI_IO_WRITE(0xA4600028, pihandle->pulse);
        }
        D_8000A470[domain] = pihandle;
    }

    EPI_IO_WRITE(0xA4600000, func_80004B30(dramAddr));
    EPI_IO_WRITE(0xA4600004, (pihandle->baseAddress | devAddr) & 0x1FFFFFFF);
    switch (direction) {
    case 0: /* OS_READ */
        EPI_IO_WRITE(0xA460000C, size - 1);
        break;
    case 1: /* OS_WRITE */
        EPI_IO_WRITE(0xA4600008, size - 1);
        break;
    default:
        return -1;
    }
    return 0;
}
