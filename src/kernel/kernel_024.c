#include "common.h"

/* osEPiRawWriteIo = libultra osEPiRawWriteIo (epirawwrite.c).
 * 2026-07-10 CRACKED EXACT 100/100: the former osEPiRawWriteIo +
 * func_80009B60 INCLUDE_ASM fragments were ONE function (the 9B60 label
 * was a splat fragment boundary; the symbol stays exported via
 * undefined_syms_auto.txt).
 * Match levers: PI registers as baked-constant derefs (IO_REG =
 * libreultra IO_WRITE/PHYS_TO_K1 form) giving fresh-temp store bases
 * instead of $at-fused extern-scalar stores; register stat/domain;
 * plain cHandle homed at sp+4 with statement-granular reloads.
 * Donor kernel_024_o1_9AB0.c bytes replace the body below in the .o via
 * REPLACE_FUNC_BODY (see Makefile); this -O2 host body is the semantic
 * reference.
 *
 * (Historical, former func_80009B60 note: size 0xE0, was 0xD0 — merged in
 * former func_80009C30 fragment, the 4-insn fall-through tail at
 * 0x80009C30 `or t2,t5,at; sw a2,0(t2); jr ra; or v0,0,0`. The address
 * 0x80009C30 is re-exported via undefined_syms_auto.txt for callers like
 * func_80008AD0/__rmonHitCpuBreak that jal directly into the tail.) */

#define IO_REG(a) (*(volatile u32 *)(a))

typedef struct OSPiHandle_s {
    struct OSPiHandle_s *next; /* 0x00 */
    u8 type;        /* 0x04 */
    u8 latency;     /* 0x05 */
    u8 pageSize;    /* 0x06 */
    u8 relDuration; /* 0x07 */
    u8 pulse;       /* 0x08 */
    u8 domain;      /* 0x09 */
    u8 pad[2];
    u32 baseAddress; /* 0x0C */
} OSPiHandle;

extern OSPiHandle *D_8000A470[]; /* __osCurrentHandle */

s32 osEPiRawWriteIo(OSPiHandle *pihandle, u32 devAddr, u32 data) {
    register u32 stat;
    register u32 domain;

    stat = IO_REG(0xA4600010);
    while (stat & 3) {
        stat = IO_REG(0xA4600010);
    }

    domain = pihandle->domain;
    if (D_8000A470[domain] != pihandle) {
        OSPiHandle *cHandle = D_8000A470[domain];
        if (domain == 0) {
            if (cHandle->latency != pihandle->latency) {
                IO_REG(0xA4600014) = pihandle->latency;
            }
            if (cHandle->pageSize != pihandle->pageSize) {
                IO_REG(0xA460001C) = pihandle->pageSize;
            }
            if (cHandle->relDuration != pihandle->relDuration) {
                IO_REG(0xA4600020) = pihandle->relDuration;
            }
            if (cHandle->pulse != pihandle->pulse) {
                IO_REG(0xA4600018) = pihandle->pulse;
            }
        } else {
            if (cHandle->latency != pihandle->latency) {
                IO_REG(0xA4600024) = pihandle->latency;
            }
            if (cHandle->pageSize != pihandle->pageSize) {
                IO_REG(0xA460002C) = pihandle->pageSize;
            }
            if (cHandle->relDuration != pihandle->relDuration) {
                IO_REG(0xA4600030) = pihandle->relDuration;
            }
            if (cHandle->pulse != pihandle->pulse) {
                IO_REG(0xA4600028) = pihandle->pulse;
            }
        }
        D_8000A470[domain] = pihandle;
    }

    *(volatile u32 *)(0xA0000000 | (pihandle->baseAddress | devAddr)) = data;
    return 0;
}
/* (donor bytes replace the body above in the .o; see Makefile) */

/* __osSiRawWriteIo split out to kernel_046.c (-O1) */
