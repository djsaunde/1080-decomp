/* IDO 5.3 -O1 donor for gl_osEPiRawWriteIo = libultra osEPiRawWriteIo
 * (epirawwrite.c verbatim, EPI_SYNC/UPDATE_REG macros from piint.h).
 * PROVEN EXACT 100/100 standalone at IDO 5.3 -O1. Absorbs the 2-word
 * orphan game_libs_func_00073694 (lui/lw PI_STATUS_REG hoisted head;
 * true entry = 0x73694, span 0x73694..0x73824). Sibling of the
 * osEPiRawReadIo donor (game_libs_ido53_730CC.c) — only the tail
 * differs (IO_WRITE of data vs IO_READ into *data).
 * Blank extern: D_00000000_pihandles = __osCurrentHandle[].
 * Spliced into game_libs_post2b_d.c.o via REPLACE_FUNC_BODY. */
#include "libultra/os_pi.h" /* OSPiHandle, __OSTranxInfo (was a private copy) */

extern OSPiHandle *D_00000000_pihandles[]; /* __osCurrentHandle */

#define IO_READ(a) (*(volatile u32 *)((a) | 0xA0000000))
#define IO_WRITE(a, v) (*(volatile u32 *)(a) = (v))
#define PI_STATUS_REG 0xA4600010

#define WAIT_ON_IOBUSY(stat)       \
    stat = IO_READ(PI_STATUS_REG); \
    while (stat & 3)               \
        stat = IO_READ(PI_STATUS_REG);

#define UPDATE_REG(reg, var)           \
    if (cHandle->var != pihandle->var) \
        IO_WRITE(reg, pihandle->var);

#define EPI_SYNC(pihandle, stat, domain)                    \
    WAIT_ON_IOBUSY(stat)                                    \
    domain = pihandle->domain;                              \
    if (D_00000000_pihandles[domain] != pihandle) {         \
        OSPiHandle *cHandle = D_00000000_pihandles[domain]; \
        if (domain == 0) {                                  \
            UPDATE_REG(0xA4600014, latency);                \
            UPDATE_REG(0xA460001C, pageSize);               \
            UPDATE_REG(0xA4600020, relDuration);            \
            UPDATE_REG(0xA4600018, pulse);                  \
        } else {                                            \
            UPDATE_REG(0xA4600024, latency);                \
            UPDATE_REG(0xA460002C, pageSize);               \
            UPDATE_REG(0xA4600030, relDuration);            \
            UPDATE_REG(0xA4600028, pulse);                  \
        }                                                   \
        D_00000000_pihandles[domain] = pihandle;            \
    }

s32 gl_osEPiRawWriteIo(OSPiHandle *pihandle, u32 devAddr, u32 data) {
    register u32 stat;
    register u32 domain;

    EPI_SYNC(pihandle, stat, domain);

    IO_WRITE(pihandle->baseAddress | devAddr | 0xA0000000, data);
    return 0;
}
