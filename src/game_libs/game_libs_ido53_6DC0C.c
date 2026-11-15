#include "uso_call.h"
/* IDO 5.3 -O1 donor for gl_osCartRomInit = libultra osCartRomInit
 * (cartrominit.c verbatim). BOUNDARY FIX: splat's game_libs_func_0006DBFC
 * (0x10, no jr ra) was 2 alignment nops + this function's HOISTED HEAD —
 * IDO 5.3 schedules the baseAddress gate load (lw CartRomHandle.baseAddress
 * into $t6) before the addiu-sp prologue, and splat split at the prologue
 * (0x6DC0C). True entry = 0x6DC04; the spliced symbol covers
 * 0x6DC04..0x6DCF8 (0xF4); the 2 leading nops stay as a pad sidecar on the
 * preceding gl_osCreatePiManager (osCreatePiManager). Same class as
 * gl_osDriveRomInit osDriveRomInit (game_libs_ido53_6C2A4.c).
 * The old "caller-set $t6" reading was wrong: $t6 is
 * CartRomHandle.baseAddress loaded by the stolen prologue
 * (0xB0000000 = PHYS_TO_K1(PI_DOM1_ADDR2), the cart-ROM base).
 * Blank externs: gl_CartRomHandle_cartrom = __CartRomHandle, gl___osPiTable_pitable
 * = __osPiTable; gl_func_00000000_rdio / _bzero / _disint / _resint =
 * osPiRawReadIo / bzero / __osDisableInt / __osRestoreInt (blank jals).
 * Spliced into game_libs_post1b2c.c.o via REPLACE_FUNC_BODY. */
typedef unsigned int u32;
typedef unsigned char u8;

typedef struct {
    u8 pad[0x60];
} __OSTranxInfo; /* sizeof = 0x60 */

typedef struct OSPiHandle_s {
    struct OSPiHandle_s *next;  /* 0x00 */
    u8 type;                    /* 0x04 */
    u8 latency;                 /* 0x05 */
    u8 pageSize;                /* 0x06 */
    u8 relDuration;             /* 0x07 */
    u8 pulse;                   /* 0x08 */
    u8 domain;                  /* 0x09 */
    u32 baseAddress;            /* 0x0C */
    u32 speed;                  /* 0x10 */
    __OSTranxInfo transferInfo; /* 0x14 */
} OSPiHandle; /* sizeof = 0x74 */

typedef struct {
    u8 pageSize;    /* CartRomHandle+6 */
    u8 relDuration; /* CartRomHandle+7 */
} PgsRls;

extern OSPiHandle gl_CartRomHandle_cartrom;       /* __CartRomHandle */
extern OSPiHandle *gl___osPiTable_pitable;      /* __osPiTable */
extern void gl_func_00000000_rdio(u32, u32 *);   /* osPiRawReadIo */
extern void gl_func_00000000_bzero(void *, int); /* bzero */
extern u32 gl_func_00000000_disint(void);   /* __osDisableInt */
extern void gl_func_00000000_resint(u32);   /* __osRestoreInt */

/* TU-DEFINED 2-byte pair (pageSize/relDuration) => the srl/andi/sb pair
 * batches under ONE lui $at (extern spelling = one lui per store =
 * near-miss). Splice imports the reloc as UNDEF; undefined_syms_auto
 * carries gl_CartRomHandle_00000006_cartpgs = 0x00000006 so %lo(pair)+0/+1 link
 * byte-identically to the blanked CartRomHandle+6/+7 words. Same lever
 * as the 6C2A4 osDriveRomInit / 6F088 osLeoDiskInit donors. */
PgsRls gl_CartRomHandle_00000006_cartpgs = {0};

OSPiHandle *gl_osCartRomInit(void) {
    u32 domain;
    u32 saveMask;

    domain = 0;
    if (gl_CartRomHandle_cartrom.baseAddress == 0xB0000000) { /* PHYS_TO_K1(PI_DOM1_ADDR2) */
        return &gl_CartRomHandle_cartrom;
    }
    gl_CartRomHandle_cartrom.type = 0;                 /* DEVICE_TYPE_CART */
    gl_CartRomHandle_cartrom.baseAddress = 0xB0000000;

    XCALL(gl_func_00000000_rdio, gl_osPiRawReadIo)(0, &domain);           /* read BSD DOM1 config */

    gl_CartRomHandle_cartrom.latency = domain & 0xFF;
    gl_CartRomHandle_cartrom.pulse = (domain >> 8) & 0xFF;
    gl_CartRomHandle_00000006_cartpgs.pageSize = (domain >> 16) & 0xF;
    gl_CartRomHandle_00000006_cartpgs.relDuration = (domain >> 20) & 0xF;
    gl_CartRomHandle_cartrom.domain = 0;               /* PI_DOMAIN1 */
    gl_CartRomHandle_cartrom.speed = 0;

    gl_func_00000000_bzero(&gl_CartRomHandle_cartrom.transferInfo, sizeof(__OSTranxInfo));

    saveMask = gl_func_00000000_disint();
    gl_CartRomHandle_cartrom.next = gl___osPiTable_pitable;
    gl___osPiTable_pitable = &gl_CartRomHandle_cartrom;
    XCALL(gl_func_00000000_resint, gl___osRestoreInt)(saveMask);

    return &gl_CartRomHandle_cartrom;
}
