/* IDO 5.3 -O1 donor for osInitialize — libultra osInitialize (KMC kernel
 * variant, 284 insns, frame 0x60). PROVEN EXACT 284/284 (2026-07-09
 * standalone probe vs asm/nonmatchings/kernel/osInitialize.s; the parent
 * unit kernel_000.c is IDO 7.1 -O2, which caps at ~53% — the fn is a 5.3 -O1
 * island, same vein as the game_libs contpfs/osCreatePiManager family).
 * Decode corrections vs the old NM body:
 *   - osClockRate is osClockRate, a u64 (OSTime): `osClockRate = osClockRate
 *     * 3 / 4` emits the __ll_mul/__ll_div helper calls func_80002F78/
 *     exceptasm_rdbFront with v0/v1 pair returns spilled to sp20/sp24;
 *   - TU-defined osClockRate/osViClock (osClockRate/osViClock are defined in
 *     initialize.c) = shared-$at single-lui for the u64 store;
 *   - exception-vector installs are literal-address struct copies
 *     (*(V*)0x80000000 etc; %lo(0x80000000)=0 gives the bare-lui form);
 *   - KMC stub copy is a stack-resident `*dst=*src; dst++; src++` walk from
 *     D_80003280 with one `src+=2; dst+=2;` skip step (each increment pair on
 *     its OWN line — same-line grouping flips the as1 addiu order);
 *   - RDRAM 0x800-word copy loop body: `*sp30=*sp34; sp30+=1; sp34+=1;` with
 *     BOTH increments on ONE line + `sp2C-=1;` last (line grouping again);
 *   - two dead pad locals (pad50/pad4C) reproduce the 0x60 frame.
 * Spliced into kernel_000.c.o via REPLACE_FUNC_BODY (real 5.3 -O1 output;
 * donor relocs imported by the splice; D_80003280 added to
 * undefined_syms_auto.txt). */
typedef int s32;
typedef unsigned int u32;
typedef unsigned long long u64;
typedef u64 OSTime;

extern s32 func_800031E0(void);                 /* __osGetSR */
extern void func_800031D0(s32);                 /* __osSetSR */
extern void func_80009C40(s32);                 /* __osSetFpcCsr */
extern s32 func_80004EC0(void *, u32 *);        /* __osSiRawReadIo */
extern s32 func_8000A040(void *, u32);          /* __osSiRawWriteIo */
extern void func_800031F0(void *, s32);         /* osWritebackDCache */
extern void func_80005350(void *, s32);         /* osInvalICache */
extern void func_80009DF0(void);                /* osMapTLBRdb */
extern void func_80002DB0(s32, u32 *);          /* osPiRawReadIo */
extern void func_800030D0(void *, s32);         /* bzero */

typedef struct {
    u32 inst1, inst2, inst3, inst4;
} __osExceptionVector;
extern __osExceptionVector D_80003270;          /* __osExceptionPreamble */
extern __osExceptionVector func_80000000;       /* UT_VEC */

OSTime osClockRate = 62500000;                   /* osClockRate (TU-defined) */
s32 osViClock = 0x02E6D354;                    /* osViClock (TU-defined) */
extern s32 osResetType;                          /* osResetType */
extern s32 osTvType;                          /* osTvType */
extern s32 osAppNMIBuffer;                          /* osAppNMIBuffer */
extern s32 __kmc_pt_mode;
extern s32 D_80003280[];

/* The KMC PARTNER-N64 debugger monitor image at 0xBFF00000 (ultralib os/initialize.c, BUILD_VERSION < K:
 * `mon = 0xBFF00000; if (*mon != 0x4B4D4300) return; ... monadr = *(mon + 1)`): word 0 is the
 * 'KMC\0' signature, word 1 the monitor's run address; osInitialize copies the image there when it
 * is not 0xBFF00000 and calls monadr + 8. */
typedef struct KmcMonitorHeader {
    /* 0x0 */ s32 magic;                        /* 0x4B4D4300 'KMC\0' */
    /* 0x4 */ s32 monadr;
} KmcMonitorHeader;

void osInitialize(void) {
    u32 pifdata;
    u32 clock = 0;
    void (*entry)();
    s32 pad50;
    s32 pad4C;
    s32 *src;
    s32 *dst;
    s32 sp40;
    void *sp3C;
    s32 *sp38;
    s32 *sp34;
    s32 *sp30;
    s32 sp2C;

    func_800031D0(func_800031E0() | 0x20000000);
    func_80009C40(0x01000800);
    while (func_80004EC0((void *)0x1FC007FC, &pifdata)) {
        ;
    }
    while (func_8000A040((void *)0x1FC007FC, pifdata | 8)) {
        ;
    }
    *(__osExceptionVector *)0x80000000 = D_80003270;
    *(__osExceptionVector *)0x80000080 = D_80003270;
    *(__osExceptionVector *)0x80000100 = D_80003270;
    *(__osExceptionVector *)0x80000180 = D_80003270;
    func_800031F0((void *)0x80000000, 0x190);
    func_80005350((void *)0x80000000, 0x190);
    func_80009DF0();
    func_80002DB0(4, &clock);
    clock &= ~0xF;
    if (clock != 0) {
        osClockRate = clock;
    }
    osClockRate = osClockRate * 3 / 4;
    if (osResetType == 0) {
        func_800030D0(&osAppNMIBuffer, 0x40);
    }
    if (osTvType == 0) {
        osViClock = 0x02F5B2D2;
    } else if (osTvType == 2) {
        osViClock = 0x02E6025C;
    } else {
        osViClock = 0x02E6D354;
    }
    if (__kmc_pt_mode == 0) {
        sp38 = (s32 *)0xBFF08004;
        sp3C = (void *)0xBFF00000;
        if (*(s32 *)0xBFF00000 == 0x4B4D4300) {
            src = D_80003280;
            dst = (s32 *)0x80000180;
            *dst = *src;
            dst++; src++;
            *dst = *src;
            dst++; src++;
            *dst = *src;
            dst++; src++;
            src += 2;
            dst += 2;
            *dst = *src;
            dst++; src++;
            *dst = *src;
            dst++; src++;
            *dst = *src;
            dst++; src++;
            func_800031F0((void *)0x80000180, 0x24);
            func_80005350((void *)0x80000180, 0x24);
            __kmc_pt_mode = 1;
            if (!(*sp38 & 0x10)) {
                sp40 = ((KmcMonitorHeader *)sp3C)->monadr;
                if (sp40 != 0xBFF00000) {
                    sp30 = (s32 *)(sp40 | 0x20000000);
                    sp2C = 0x800;
                    sp34 = (s32 *)0xBFF00000;
                    do {
                        *sp30 = *sp34;
                        sp30 += 1; sp34 += 1;
                        sp2C -= 1;
                    } while (sp2C != 0);
                }
                entry = (void (*)())(sp40 + 8);
                entry(0x4B4D4300, 0);
            }
        }
    }
}
