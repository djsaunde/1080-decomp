/* IDO 5.3 -O1 donor for gl_osDriveRomInit = libultra osDriveRomInit
 * (driverominit.c verbatim). PROVEN EXACT 54/54 standalone at IDO 5.3 -O1
 * against the concatenated 0x6C2A4..0x6C37C stream.
 * BOUNDARY FIX: splat's game_libs_func_0006C2A4 (0x8, "lui t6/lw t6, no
 * jr ra") was this function's HOISTED HEAD — IDO 5.3 schedules the
 * baseAddress gate load (lw DriveRomHandle.baseAddress) before the
 * addiu-sp prologue, and splat split the function at the prologue
 * (0x6C2AC). True entry = 0x6C2A4; the spliced symbol covers
 * 0x6C2A4..0x6C37C (0xD8). Same class as gl_osLeoDiskInit osLeoDiskInit.
 * The old "caller-set $t6 == 0xA6000000 gate cap" reading was wrong: $t6
 * is DriveRomHandle.baseAddress loaded by the stolen prologue
 * (0xA6000000 = PHYS_TO_K1(PI_DOM1_ADDR1), the 64DD drive-ROM base).
 * Blank externs: gl_DriveRomHandle_drvrom = DriveRomHandle, gl___osPiTable_pitable
 * = __osPiTable; gl_func_00000000_bzero / _disint / _resint = bzero /
 * __osDisableInt / __osRestoreInt (blank jals).
 * Spliced into game_libs_post1b2.c.o via REPLACE_FUNC_BODY. */
#include "libultra/os_pi.h" /* OSPiHandle, __OSTranxInfo (was a private copy) */

typedef struct {
    u8 pageSize;    /* DriveRomHandle+6 */
    u8 relDuration; /* DriveRomHandle+7 */
} PgsRls;

extern OSPiHandle gl_DriveRomHandle_drvrom;        /* DriveRomHandle */
extern OSPiHandle *gl___osPiTable_pitable;      /* __osPiTable */
extern void gl_func_00000000_bzero(void *, int); /* bzero */
extern u32 gl_func_00000000_disint(void);   /* __osDisableInt */
extern void gl_func_00000000_resint(u32);   /* __osRestoreInt */

/* TU-DEFINED 2-byte pair (pageSize/relDuration) => the li;li;sb;sb pair
 * batches under ONE lui $at (extern spelling = one lui per store =
 * near-miss). Splice imports the reloc as UNDEF; undefined_syms_auto
 * carries D_00000006_drvpgs = 0x00000006 so %lo(pair)+0/+1 link
 * byte-identically to the blanked DriveRomHandle+6/+7 words. Same lever
 * as the 6F088 osLeoDiskInit donor. */
PgsRls D_00000006_drvpgs = {0};

OSPiHandle *gl_osDriveRomInit(void) {
    u32 saveMask;

    if (gl_DriveRomHandle_drvrom.baseAddress == 0xA6000000) { /* PHYS_TO_K1(PI_DOM1_ADDR1) */
        return &gl_DriveRomHandle_drvrom;
    }
    gl_DriveRomHandle_drvrom.type = 1;                  /* DEVICE_TYPE_BULK */
    gl_DriveRomHandle_drvrom.baseAddress = 0xA6000000;
    gl_DriveRomHandle_drvrom.latency = 64;
    gl_DriveRomHandle_drvrom.pulse = 7;
    D_00000006_drvpgs.pageSize = 7;
    D_00000006_drvpgs.relDuration = 2;
    gl_DriveRomHandle_drvrom.domain = 0;                /* PI_DOMAIN1 */
    gl_DriveRomHandle_drvrom.speed = 0;

    gl_func_00000000_bzero(&gl_DriveRomHandle_drvrom.transferInfo, sizeof(__OSTranxInfo));

    saveMask = gl_func_00000000_disint();
    gl_DriveRomHandle_drvrom.next = gl___osPiTable_pitable;
    gl___osPiTable_pitable = &gl_DriveRomHandle_drvrom;
    gl_func_00000000_resint(saveMask);

    return &gl_DriveRomHandle_drvrom;
}
