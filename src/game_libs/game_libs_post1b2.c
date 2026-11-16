#include "common.h"
#include "gdl/gl_data.h"
#include "libultra/os_cont.h"
#include "uso_call.h"
extern int D_00000000;
extern int gl_data_00000000;
extern int AnimPlayer_update();
extern int gl_data_67470_addr;
extern int gl_data_6C9F4_devCfg;
extern char gl_data_BE14_b;
extern char gl_data_BE14_c;
typedef struct { int a, b, c, d; } Quad4;
typedef struct { int a, b, c; } Tri3i;
typedef struct { int a, b; } Pair2;
typedef struct { float x, y, z; } Vec3;


/* gl___osSiCreateAccessQueue: stores 1 to D_00000000, then gl_func(<sym_b>, &D+0x42800, 1)
 * and gl_func(<sym_c>, 0, 0). MATCH: the 0x42800 arg is a SYMBOL reference
 * (&D_00000000 + 0x42800 -> lui 0x4; addiu 0x2800), not a literal (which gives
 * lui; ori). Decode the symbol, don't literal-match
 * (docs/IDO_CODEGEN.md#feedback-return-const-lui-addiu-vs-lui-ori). Real size
 * 0x50 (the .s's 0x58 over-extends 2 insns into successor 0006BE64). Byte-exact. */
extern char gl_data_BE14_b;
extern char gl_data_BE14_c;
void gl___osSiCreateAccessQueue(void) {
    *(int*)&D_00000000 = 1;
    CALL(gl_osCreateMesgQueue)(&gl_data_BE14_b, GL_BSS(0x42800), 1);
    CALL(gl_osSendMesg)(&gl_data_BE14_c, 0, 0);
}
/* game_libs_func_0006BE64 (2-word splat mis-split: lui t6,0; lw t6,0(t6)) was
 * the stolen prologue of gl___osSiGetAccess — forward-merged into it (orphan-fn
 * removed). gl___osSiGetAccess MATCHED. */
void gl___osSiGetAccess(void) {
    int local;
    int pad;
    if (*(int*)&D_00000000 == 0) {
        CALL(gl___osSiCreateAccessQueue)();
    }
    CALL(gl_osRecvMesg)(&D_00000000, &local, 1);
}

void gl___osSiRelAccess(void) {
    CALL(gl_osSendMesg)(&D_00000000, 0, 0);
}
#pragma GLOBAL_ASM("asm/nonmatchings/game_libs/game_libs/gl_func_0006BEA8_pad.s")

/* gl_osCreateThread: 81-insn struct-init constructor (self, a1, a2, a3,
 * arg5, arg6; arg5/arg6 stack-passed). Fields: +0x14=a1, +0x04=arg5,
 * +0x00/+0x08/+0x18=0, +0x11C=a2, +0x38=(s64)a3, +0xF0=(s64)arg6-16,
 * +0x100=(s64)(int)&D, +0x118=0xFF03, +0x128=(0x3FFF01&0x3F0000)>>16,
 * +0x12C=0x01000800, +0x10=(s16)1, +0x12=(s16)0; then v0=fn();
 * self->0xC=D_a; D_b=self; fn2(v0).
 * LANDED 2026-07-09 via REPLACE_FUNC_BODY donor splice: real C lives in
 * the -O1 donor unit game_libs_o1_6BF34.c (81/81 at plain 7.1 -O1 —
 * register-int tmp colors s0; unsigned local for the srl). Body below is
 * a placeholder for the splice (its bytes are replaced by the donor). */
extern int D_00000000;
void gl_osCreateThread(int *a0, int a1, int a2, int a3, int arg5, int arg6) {
    volatile int keep = a1 + a2 + a3 + arg5 + arg6;
    a0[0] = keep;
}
#pragma GLOBAL_ASM("asm/nonmatchings/game_libs/game_libs/gl_func_0006BF34_pad.s")

/* osContStartReadData = osContStartReadData (35 instructions).
 * Exact IDO 7.1 -O1 implementation: game_libs_o1_6C084.c. This -O2
 * placeholder is replaced with the whole compiled body by the Makefile.
 * The former scheduling mismatch was the wrong optimization level and
 * command-store order; __osContLastCmd is set BEFORE __osSiRelAccess. */
int osContStartReadData(void *mq) {
    return 0;
}


/* gl_osContGetReadData = libultra osContGetReadData (contreaddata.c; named __osContGetInitData until
 * run 542): for each of __osMaxControllers 8-byte __OSContReadFormat records in __osContPifRam,
 * data->errno = (rxsize & 0xC0) >> 4 (CHNL_ERR) and, when 0, button / stick_x / stick_y; data
 * advances one 6-byte OSContPad. 42 insns incl. the absorbed 3-word orphan
 * game_libs_func_0006C110 (its lui/lbu count preload is this function's stolen prologue, so the
 * donor's true entry is 0x6C110 and the spliced symbol covers 0x6C110..0x6C1B8).
 * LANDED 2026-07-09 via REPLACE_FUNC_BODY donor splice: the real C is the IDO 5.3 -O1 donor unit
 * game_libs_ido53_6C11C.c (42/42; 5.3 colors the lwl/lwr struct-copy scratch $at, 7.1 uses
 * $t1/$t0). The body below is a placeholder for the splice on both build paths (its bytes are
 * replaced by the donor). */
void gl_osContGetReadData(OSContPad *data, char *cursor, int count) {
    unsigned char rec[8];               /* __OSContReadFormat: dummy, txsize, rxsize, cmd, button, stick_x, stick_y */
    volatile int i;
    volatile char *cur = cursor;
    if (count > 0) {
        i = 0;
        do {
            *(int *)rec = *(int *)cur;
            *(int *)(rec + 4) = *(int *)(cur + 4); /* RAW: data-table: __OSContReadFormat raw bytes in a local buffer (libultra layout) */
            data->errno = (rec[2] & 0xC0) >> 4;
            if (data->errno == 0) {
                data->button = *(unsigned short *)(rec + 4); /* RAW: data-table: __OSContReadFormat raw bytes in a local buffer (libultra layout) */
                data->stick_x = rec[6];
                data->stick_y = rec[7];
            }
            data++;
            cur += 8;
            i++;
        } while (i < 1);
    }
}

/* gl___osPackReadData: 59-insn table-reset + 8-byte record emitter,
 * reset-sibling of gl___osPackRequestData_6BD14 (template {0xFF,1,4,1,0xFFFF,-1,-1},
 * type=4/subtype=1) and emitter-sibling of the landed gl_osContGetReadData
 * decoder. Zeroes D_c[0..14], sets D_c[15]=1, emits the template
 * D_a times via swl/swr struct copies, then a 0xFE terminator.
 * LANDED 2026-07-09 via REPLACE_FUNC_BODY donor splice: real C lives in
 * the IDO 5.3 -O1 donor unit game_libs_ido53_6C1B8.c (59/59 — 5.3 colors
 * the struct-copy scratch $at; FOR-loop shape sinks the cursor store into
 * the bne delay slot, see donor header). Body below is a placeholder for
 * the splice (its bytes are replaced by the donor). */
void gl___osPackReadData(void) {
    volatile int i;
    for (i = 0; i < 15; i++) {}
}


/* game_libs_func_0006C2A4 (0x8 orphan) was the HOISTED HEAD of
 * gl_osDriveRomInit = osDriveRomInit — absorbed into the donor splice
 * below (true entry 0x6C2A4, spliced symbol covers 0x6C2A4..0x6C37C). */


/* gl_osDriveRomInit = libultra osDriveRomInit (driverominit.c verbatim):
 * gate on DriveRomHandle.baseAddress == PHYS_TO_K1(PI_DOM1_ADDR1)
 * (0xA6000000), else init the 64DD drive-ROM PI handle (type=BULK,
 * latency=64, pulse=7, pageSize=7, relDuration=2, domain=PI_DOMAIN1),
 * bzero the transfer info, and link into __osPiTable under disabled
 * interrupts. Needs IDO 5.3 -O1 (hoisted pre-prologue gate load +
 * shared-$at pageSize/relDuration sb pair), so the real C lives in the
 * donor unit game_libs_ido53_6C2A4.c (54/54 exact incl. the absorbed
 * 0x8 head game_libs_func_0006C2A4). Body below is a placeholder for
 * the REPLACE_FUNC_BODY splice (its bytes are replaced by the donor). */
void *gl_osDriveRomInit(void) {
    volatile int i;
    for (i = 0; i < 15; i++) {}
    return 0;
}
#pragma GLOBAL_ASM("asm/nonmatchings/game_libs/game_libs/gl_func_0006C2AC_pad.s")
