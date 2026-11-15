/* IDO 5.3 -O1 donor for gl_osLeoDiskInit = libultra osLeoDiskInit
 * (leodiskinit.c verbatim). PROVEN EXACT 64/64 standalone at IDO 5.3 -O1
 * against the concatenated 0x6F044..0x6F140 stream.
 * BOUNDARY FIX: splat's game_libs_func_0006F038 (0x50, "descriptor
 * initializer, no jr ra") was this function's HOISTED HEAD — IDO 5.3
 * schedules the first six LeoDiskHandle field stores (type/baseAddress/
 * latency/pulse/pageSize/relDuration) before the addiu-sp prologue, and
 * splat split the function at the prologue (0x6F088). True entry =
 * 0x6F044; the spliced symbol covers 0x6F044..0x6F140. The three leading
 * pad nops 0x6F038/3C/40 are emitted as SUFFIX_BYTES_FORCE on
 * gl_osEPiStartDma (natural jr-ra epilogue -> FORCE).
 * 5.3 NOTE: IO_WRITE(PI_BSD_DOM2_LAT_REG, latency) constant-folds to
 * li 3 (the latency=3 store is still emitted at +5), while the PWD/PGS/
 * RLS writes reload their bytes (lbu +8/+6/+7) — exactly the target's
 * asymmetry.
 * Blank externs: gl_LeoDiskHandle_leodisk = LeoDiskHandle, gl___osPiTable_pitable
 * = __osPiTable, D_00000000_diskhandle = __osDiskHandle;
 * gl_func_00000000_bzero / _disint / _resint = bzero / __osDisableInt /
 * __osRestoreInt (blank jals).
 * Spliced into game_libs_post1b2c.c.o via REPLACE_FUNC_BODY. */
#include "libultra/os_pi.h" /* OSPiHandle, __OSTranxInfo (was a private copy) */
#include "uso_call.h"

typedef struct {
    u8 pageSize;    /* LeoDiskHandle+6 */
    u8 relDuration; /* LeoDiskHandle+7 */
} PgsRls;

extern OSPiHandle gl_LeoDiskHandle_leodisk;       /* LeoDiskHandle */
extern OSPiHandle *gl___osPiTable_pitable;      /* __osPiTable */
extern OSPiHandle *D_00000000_diskhandle;   /* __osDiskHandle */
extern void gl_func_00000000_bzero(void *, int); /* bzero */
extern u32 gl_func_00000000_disint(void);   /* __osDisableInt */
extern void gl_func_00000000_resint(u32);   /* __osRestoreInt */

#define IO_WRITE(a, v) (*(volatile u32 *)(a) = (v))

/* TU-DEFINED 2-byte pair (pageSize/relDuration) => the li;li;sb;sb head
 * pair batches under ONE lui $at (extern spelling = one lui per store =
 * 65/64 near-miss). Splice imports the reloc as UNDEF;
 * undefined_syms_auto carries gl_LeoDiskHandle_00000006_leopgs = 0x00000006 so
 * %lo(pair)+0/+1 link byte-identically to the blanked LeoDiskHandle+6/+7
 * words. Same lever as osCartRomInit (kernel_008_o1_58C0). */
PgsRls gl_LeoDiskHandle_00000006_leopgs = {0};

OSPiHandle *gl_osLeoDiskInit(void) {
    u32 saveMask;

    gl_LeoDiskHandle_leodisk.type = 2;                 /* DEVICE_TYPE_64DD */
    gl_LeoDiskHandle_leodisk.baseAddress = 0xA5000000; /* PHYS_TO_K1(PI_DOM2_ADDR1) */
    gl_LeoDiskHandle_leodisk.latency = 3;
    gl_LeoDiskHandle_leodisk.pulse = 6;
    gl_LeoDiskHandle_00000006_leopgs.pageSize = 6;
    gl_LeoDiskHandle_00000006_leopgs.relDuration = 2;
    gl_LeoDiskHandle_leodisk.domain = 1;               /* PI_DOMAIN2 */
    IO_WRITE(0xA4600024, gl_LeoDiskHandle_leodisk.latency);     /* PI_BSD_DOM2_LAT */
    IO_WRITE(0xA4600028, gl_LeoDiskHandle_leodisk.pulse);       /* PI_BSD_DOM2_PWD */
    IO_WRITE(0xA460002C, gl_LeoDiskHandle_leodisk.pageSize);    /* PI_BSD_DOM2_PGS */
    IO_WRITE(0xA4600030, gl_LeoDiskHandle_leodisk.relDuration); /* PI_BSD_DOM2_RLS */
    gl_LeoDiskHandle_leodisk.speed = 0;
    gl_func_00000000_bzero(&gl_LeoDiskHandle_leodisk.transferInfo, sizeof(__OSTranxInfo));
    saveMask = gl_func_00000000_disint();
    gl_LeoDiskHandle_leodisk.next = gl___osPiTable_pitable;
    gl___osPiTable_pitable = &gl_LeoDiskHandle_leodisk;
    D_00000000_diskhandle = &gl_LeoDiskHandle_leodisk;
    XCALL(gl_func_00000000_resint, gl___osRestoreInt)(saveMask);
    return &gl_LeoDiskHandle_leodisk;
}
