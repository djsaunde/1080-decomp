/* -O1 donor for osCreatePiManager — 115-insn kernel osCreatePiManager (KMC
 * kernel variant of libultra pimgr.c; sibling of the game_libs crack
 * gl_osCreatePiManager, see docs/IDO_CODEGEN "TU-DEFINED global = shared-lui
 * $at store cluster"). PROVEN EXACT 115/115 (2026-07-10 standalone probe vs
 * asm/nonmatchings/kernel/osCreatePiManager.s; identical at IDO 5.3 AND 7.1
 * -O1 — built here with the default 7.1 at -O1).
 * KEY LEVERS:
 *   - TU-DEFINED PiDevMgr: `PiDevMgr __osPiDevMgr = {0};` (a definition, NOT
 *     extern) makes IDO emit the 4+3 store cluster with ONE `lui $at` per
 *     group + batched value materializations (extern form = one lui $at per
 *     store, the old 77% floor). The splice imports the reloc as a global
 *     UNDEF in the host; undefined_syms_auto.txt already carries
 *     __osPiDevMgr = 0x8000A450, and %lo(__osPiDevMgr)+0x4/0x8/0xC/... resolve
 *     byte-identically to the .s's per-field D_8000A454/8/C/60/64/68 relocs.
 *   - Field assignment order f0,f4,f8,fC (f8=cmdQ BEFORE fC=&thread) gives
 *     the target's t3=arg-reload / t4=&pimgr_piEventQueue temp numbering while
 *     as1 still emits the stores in the target's 0/4/C/8 order.
 * KMC-variant differences vs stock pimgr.c: TWO __osDevMgrMain workers
 * (func_80005F10 pairs with stacks pimgr_piThreadStack+0x1000 and
 * pimgr_ramromThreadStack+0x400, second at pri-1, arg block NULL) and dma/edma
 * handlers func_80004A50 / func_800056F0 (the post-addiu alt entry).
 * Spliced into kernel_006.c.o via REPLACE_FUNC_BODY (real -O1 output;
 * donor relocs imported by the splice; C_FILES filter-out). */
typedef int s32;
typedef s32 OSPri;
extern s32 __osPiAccessQueue;
extern s32 __osPiAccessQueueEnabled;
extern void func_800053D0(void *, void *, s32);
extern void func_80005C00(void);
extern void func_800051E0(s32, void *, void *);
extern OSPri func_800064F0(void *);
extern void func_80006510(void *, OSPri);
extern unsigned int func_800066B0(void);
extern void func_800066D0(unsigned int);
extern void func_80005F10(void *, s32, void *, void *, void *, OSPri);
extern void func_8000A110(void *);
typedef struct { s32 f0; void *f4; void *f8; void *fC; void *f10, *f14, *f18; } PiDevMgr;
extern s32 D_80006060, D_80005AEC;
extern void func_80004A50();
extern void func_800056F0();
extern unsigned char pimgr_piThread[], pimgr_piThreadStack[], pimgr_ramromThread[], pimgr_ramromThreadStack[], pimgr_piEventQueue[], pimgr_piEventBuf[];

PiDevMgr __osPiDevMgr = {0};

void osCreatePiManager(OSPri pri, void *cmdQ, void *cmdBuf, s32 cmdMsgCnt) {
    unsigned int savedMask;
    OSPri oldPri;
    OSPri myPri;

    if (!__osPiDevMgr.f0) {
        func_800053D0(cmdQ, cmdBuf, cmdMsgCnt);
        func_800053D0(pimgr_piEventQueue, pimgr_piEventBuf, 1);
        if (!__osPiAccessQueueEnabled) {
            func_80005C00();
        }
        func_800051E0(8, pimgr_piEventQueue, (void *)0x22222222);
        oldPri = -1;
        myPri = func_800064F0(0);
        if (myPri < pri) {
            oldPri = myPri;
            func_80006510(0, pri);
        }
        savedMask = func_800066B0();
        __osPiDevMgr.f0 = 1;
        __osPiDevMgr.f4 = pimgr_piThread;
        __osPiDevMgr.f8 = cmdQ;
        __osPiDevMgr.fC = pimgr_piEventQueue;
        __osPiDevMgr.f10 = &__osPiAccessQueue;
        __osPiDevMgr.f14 = &func_80004A50;
        __osPiDevMgr.f18 = &func_800056F0;
        func_80005F10(pimgr_piThread, 0, &D_80006060, &__osPiDevMgr, pimgr_piThreadStack + 0x1000, pri);
        func_8000A110(pimgr_piThread);
        func_80005F10(pimgr_ramromThread, 0, &D_80005AEC, 0, pimgr_ramromThreadStack + 0x400, pri - 1);
        func_8000A110(pimgr_ramromThread);
        func_800066D0(savedMask);
        if (oldPri != -1) {
            func_80006510(0, oldPri);
        }
    }
}
