/* IDO 5.3 -O1 -mips2 whole-TU donor for the libultra VI-manager pair:
 *   osCreateViManager = osCreateViManager (donor .text [0x000,0x188);
 *                      true entry USO 0x74EF4: splat's 74EFC label sits
 *                      2 words in; the .s carries the pre-prologue
 *                      active-flag load, which IS the fn's first two
 *                      words = lui/lw of __osViDevMgr.active)
 *   gl_viMgrMain = viMgrMain (donor .text [0x188,0x354) + as1 pad
 *                      to 0x360; target size 0x1CC)
 * (libreultra src/io/vimgr.c verbatim.) PROVEN EXACT 2026-08-22:
 * create 98/98 + main 115/115 masked words vs the raw target .s, first
 * compile once the two structural keys below are in place. NOT the -O3
 * class (probed: -O3/-O2 fill jal delays and s-reg-allocate; target is
 * -O1 shaped) -- the whole-TU keys are:
 *   1. WHOLE-TU compile: viMgrMain is static; the TU .bss layout
 *      (viThread 0x1B0, viThreadStack 0x1000, viEventQueue 0x18,
 *      viEventBuf 0x14+pad, viRetraceMsg 0x18, viCounterMsg 0x18,
 *      viMgrMain's static retrace u16) maps 1:1 onto USO
 *      0x44080/0x44230/0x45230/0x45248/0x45260/0x45278/0x45290.
 *   2. __osViDevMgr DEFINED in the TU (= {0}; USO .data offset 0 --
 *      the "blank-looking" lui/lo 0 fields in the target are its REAL
 *      baked address, the game_libs USO data segment is 0-based, cf.
 *      xldtob's .rodata base 0x2540). A local-.data definition makes
 *      IDO share $at across the 7 devmgr field-store burst; an extern
 *      declaration re-luis $at per store (+5 words -- that WAS the
 *      whole 65%-era residual, and the create-size change is also what
 *      displaces viMgrMain's dead epilogue: as1 32-byte-aligns the
 *      unreachable epilogue block, giving the target's 6-zero gap at
 *      the right offset for free).
 * viMgrMain's forward-declared-static symbol resolves *UND* in the
 * donor symtab, so the donor rule's POST_COMPILE add-elf-func-symbol.py
 * injects gl_viMgrMain=0x188:0x1cc for REPLACE_FUNC_BODY to key on.
 * Reloc story (every masked word fits one of these):
 *   - donor .bss section relocs -> gl_func_*_bss pins = 0x00044080;
 *   - the viMgrMain fn-ptr %hi/%lo against donor .text (addend 0x188)
 *     -> osCreateViManager_text pin = 0x00089560 (bakes 0x896E8, the
 *     USO's original-link address of viMgrMain, same story as _Ldtob's
 *     jal 0x87F70);
 *   - gl_viDevMgr symbol relocs -> pin = 0 (USO .data offset 0);
 *   - imported kernel data (__osViIntrCount/__osCurrentTime/
 *     __osBaseCounter) and all jals: blank USO relocs -> pins = 0.
 * Spliced into game_libs_post2b_f.c.o via REPLACE_FUNC_BODY. */
#include "libultra/os_pi.h"    /* OSMesgQueue, OSIoMesgHdr, OSIoMesg, OSDevMgr, OSThread (were private copies) */
#include "libultra/os_timer.h" /* OSTime */

typedef struct {
    u16 state;       /* 0x00 */
    u16 retraceCount;/* 0x02 */
    void *framep;    /* 0x04 */
    void *modep;     /* 0x08 */
    u32 control;     /* 0x0C */
    void *msgq;      /* 0x10 (OSMesgQueue *) */
    OSMesg msg;      /* 0x14 */
} __OSViContext;

#define OS_VIM_STACKSIZE 0x1000
#define OS_EVENT_COUNTER 3
#define OS_EVENT_VI 7
#define TRUE 1

/* __osViDevMgr: DEFINED in-TU (structural key #2 above); the splice
 * imports the symbol reloc, pinned = 0 in undefined_syms_auto.txt */
OSDevMgr gl_viDevMgr = {0};
/* imported kernel data (blank USO relocs, resolve to 0) */
extern u32 gl_viIntrCount;           /* __osViIntrCount */
extern OSTime gl_CurrentTime;        /* __osCurrentTime */
extern u32 gl_BaseCounter;           /* __osBaseCounter */

/* imported functions (blank USO jals, resolve to 0) */
extern void gl_func_00000000_tsvcinit(void);                          /* __osTimerServicesInit */
extern void gl_func_00000000_crmq(OSMesgQueue *, OSMesg *, s32);      /* osCreateMesgQueue */
extern void gl_func_00000000_sevm(s32, OSMesgQueue *, OSIoMesg *);    /* osSetEventMesg */
extern OSPri gl_func_00000000_getpri(OSThread *);                     /* osGetThreadPri */
extern void gl_func_00000000_setpri(OSThread *, OSPri);               /* osSetThreadPri */
extern u32 gl_func_00000000_dint(void);                               /* __osDisableInt */
extern void gl_func_00000000_rint(u32);                               /* __osRestoreInt */
extern void gl_func_00000000_crth(OSThread *, s32, void (*)(void *), void *, void *, OSPri); /* osCreateThread */
extern void gl_func_00000000_viinit(void);                            /* __osViInit */
extern void gl_func_00000000_stth(OSThread *);                        /* osStartThread */
extern s32 gl_func_00000000_recv(OSMesgQueue *, OSMesg *, s32);       /* osRecvMesg */
extern s32 gl_func_00000000_send(OSMesgQueue *, OSMesg, s32);         /* osSendMesg */
extern __OSViContext *gl_func_00000000_vigetc(void);                  /* __osViGetCurrentContext */
extern void gl_func_00000000_viswap(void);                            /* __osViSwapContext */
extern u32 gl_func_00000000_getcount(void);                           /* osGetCount */
extern void gl_func_00000000_tint(void);                              /* __osTimerInterrupt */

static OSThread viThread;
static unsigned char viThreadStack[OS_VIM_STACKSIZE];
static OSMesgQueue viEventQueue;
static OSMesg viEventBuf[5];
static OSIoMesg viRetraceMsg;
static OSIoMesg viCounterMsg;

static void viMgrMain(void *arg);

void osCreateViManager(OSPri pri)
{
    u32 savedMask;
    OSPri oldPri;
    OSPri myPri;
    if (gl_viDevMgr.active == 0)
    {
        gl_func_00000000_tsvcinit();
        gl_func_00000000_crmq(&viEventQueue, viEventBuf, 5);
        viRetraceMsg.hdr.type = OS_MESG_TYPE_VRETRACE;
        viRetraceMsg.hdr.pri = OS_MESG_PRI_NORMAL;
        viRetraceMsg.hdr.retQueue = NULL;
        viCounterMsg.hdr.type = OS_MESG_TYPE_COUNTER;
        viCounterMsg.hdr.pri = OS_MESG_PRI_NORMAL;
        viCounterMsg.hdr.retQueue = NULL;
        gl_func_00000000_sevm(OS_EVENT_VI, &viEventQueue, &viRetraceMsg);
        gl_func_00000000_sevm(OS_EVENT_COUNTER, &viEventQueue, &viCounterMsg);
        oldPri = -1;
        myPri = gl_func_00000000_getpri(NULL);
        if (myPri < pri)
        {
            oldPri = myPri;
            gl_func_00000000_setpri(NULL, pri);
        }
        savedMask = gl_func_00000000_dint();
        gl_viDevMgr.active = 1;
        gl_viDevMgr.thread = &viThread;
        gl_viDevMgr.cmdQueue = &viEventQueue;
        gl_viDevMgr.evtQueue = &viEventQueue;
        gl_viDevMgr.acsQueue = NULL;
        gl_viDevMgr.dma = NULL;
        gl_viDevMgr.edma = NULL;
        gl_func_00000000_crth(&viThread, 0, viMgrMain, &gl_viDevMgr, &viThreadStack[OS_VIM_STACKSIZE], pri);
        gl_func_00000000_viinit();
        gl_func_00000000_stth(&viThread);
        gl_func_00000000_rint(savedMask);
        if (oldPri != -1)
        {
            gl_func_00000000_setpri(0, oldPri);
        }
    }
}
static void viMgrMain(void *arg)
{
    __OSViContext *vc;
    OSDevMgr *dm;
    OSIoMesg *mb;
    static u16 retrace;
    s32 first;
    u32 count;

    mb = NULL;
    first = 0;
    vc = gl_func_00000000_vigetc();
    retrace = vc->retraceCount;
    if (retrace == 0)
        retrace = 1;
    dm = (OSDevMgr *)arg;
    while (TRUE)
    {
        gl_func_00000000_recv(dm->evtQueue, (OSMesg)&mb, OS_MESG_BLOCK);
        switch (mb->hdr.type)
        {
        case OS_MESG_TYPE_VRETRACE:
            gl_func_00000000_viswap();
            retrace--;
            if (retrace == 0)
            {
                vc = gl_func_00000000_vigetc();
                if (vc->msgq != NULL)
                    gl_func_00000000_send(vc->msgq, vc->msg, OS_MESG_NOBLOCK);
                retrace = vc->retraceCount;
            }
            gl_viIntrCount++;
            if (first)
            {

                count = gl_func_00000000_getcount();
                gl_CurrentTime = count;
                first = 0;
            }
            count = gl_BaseCounter;
            gl_BaseCounter = gl_func_00000000_getcount();
            count = gl_BaseCounter - count;
            gl_CurrentTime = gl_CurrentTime + count;
            break;
        case OS_MESG_TYPE_COUNTER:
            gl_func_00000000_tint();
            break;
        }
    }
}
