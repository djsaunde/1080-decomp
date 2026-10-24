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
extern int *D_8000A420;
extern s32 D_8000A418;
extern s32 siacs_bss_0000;
extern s32 func_800009D8(void*, s32, s32, void*);
extern void func_80003FF0();
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
extern s32 D_80012BE4;
extern s32 D_80012C64;
extern s32 D_80012C68;
extern s32 D_80012D60[];
extern s32 D_80013000;
extern s32 D_8000A2E0;
extern int *D_8000A41C;
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
extern s32 func_80009EA0(void);
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

typedef struct {
    u8 pad_00[0x98];
    u8 in_use;
    u8 pad_99[3];
} UsoSlot;

typedef struct {
    UsoSlot slots[4];
} UsoSlotGroup;

extern u8 D_80013138;
extern UsoSlot D_800130A0;
extern u8 D_800131D4;
extern UsoSlot D_8001313C;
extern u8 D_80013270;
extern UsoSlot D_800131D8;
extern UsoSlotGroup D_80013274;
extern UsoSlotGroup D_800182E4;


/* split from kernel_000.c - 2026-06-10 kernel ROM-order relayout */


/* func_800029B0 + func_80002A10 split out to kernel_045.c (-O1) */

/* func_80002A3C split out to kernel_056.c (-O2 -mips3) */

/* func_80002A78 split out to kernel_056.c (-O2 -mips3) */

/* func_80002AB4 split out to kernel_056.c (-O2 -mips3) */

/* func_80002AE0 split out to kernel_056.c (-O2 -mips3) */

/* func_80002B1C split out to kernel_056.c (-O2 -mips3) */

/* func_80002B78 split out to kernel_056.c (-O2 -mips3) */

/* func_80002BA8 split out to kernel_056.c (-O1 -mips3, d-arithmetic helper) */

/* func_80002C08 split out to kernel_056.c (-O2 -mips3) */

/* func_80002CA4 split out to kernel_056.c (-O2 -mips3) */

/* func_80002CD0 = bzero (libultra handwritten libc/bzero.s). Real source; see docs/MATCHING_WORKFLOW.md#handwritten-assembly-sources. */
GLOBAL_ASM("src/libultra/libc/bzero.s")

/* func_80002DB0 = __osSetSR (+8 leading pad words; handwritten mtc0). Real source; see docs/MATCHING_WORKFLOW.md#handwritten-assembly-sources. */
GLOBAL_ASM("src/libultra/os/setsr.s")

/* func_80002DE0 = __osGetSR (handwritten mfc0). Real source; see docs/MATCHING_WORKFLOW.md#handwritten-assembly-sources. */
GLOBAL_ASM("src/libultra/os/getsr.s")

/* func_80002DF0 = osWritebackDCache (handwritten cache ops). Real source; see docs/MATCHING_WORKFLOW.md#handwritten-assembly-sources. */
GLOBAL_ASM("src/libultra/os/writebackdcache.s")

/* func_80002E70 = __osExceptionPreamble (first half), real source in src/libultra/os/exceptasm_preamble.s. See docs/MATCHING_WORKFLOW.md#handwritten-assembly-sources. */
GLOBAL_ASM("src/libultra/os/exceptasm_preamble.s")

/* func_80002E78 = exception vector stubs + rdb/KMC front end of __osException, real source in src/libultra/os/exceptasm_rdb.s. See docs/MATCHING_WORKFLOW.md#handwritten-assembly-sources. */
GLOBAL_ASM("src/libultra/os/exceptasm_rdb.s")

/* func_80002F78 / func_800030D0 merged into func_80002E78.s as alabel
 * alternative entries (2026-08-22): one contiguous handwritten exception/rdb
 * handler stream -- 2E78 falls through into 2F78 (mfc0 tail, no jr ra), 2F78
 * ends with a branch whose DELAY SLOT is 30D0's first insn, and both branch
 * forward into func_800031F0's labels. jal callers (func_80002530,
 * func_80004E50, func_800058C0) resolve via the global alabel symbols. */

/* func_800031D0: rdb debug-read fragment, real source in src/libultra/os/exceptasm_rdb_2.s. See docs/MATCHING_WORKFLOW.md#handwritten-assembly-sources. */
GLOBAL_ASM("src/libultra/os/exceptasm_rdb_2.s")

/* func_800031E0: rdb debug-read fragment, real source in src/libultra/os/exceptasm_rdb_3.s. See docs/MATCHING_WORKFLOW.md#handwritten-assembly-sources. */
GLOBAL_ASM("src/libultra/os/exceptasm_rdb_3.s")

/* func_800031F0 = __osException core (savecontext, interrupt dispatch,
 * send_mesg, __osEnqueueAndYield, __osEnqueueThread, __osPopThread,
 * __osDispatchThread, __osCleanupThread): handwritten, real source in
 * src/libultra/os/exceptasm.s. See docs/MATCHING_WORKFLOW.md#handwritten-assembly-sources. */
GLOBAL_ASM("src/libultra/os/exceptasm.s")

/* func_80003BF0 = __osDequeueThread (libultra thread.c), compiled C: this
 * unit's -O1 gives the target (prologue scheduled after the two loads, the
 * `register` locals' empty 8-byte frame). Split out of the handwritten
 * func_800031F0 2026-10-24 (agent-g run 158). Its runtime address is
 * 0x80003FF0 (func_80003FF0, called by osDestroyThread below). */
typedef struct DqThread_s { struct DqThread_s *next; } DqThread;

void func_80003BF0(DqThread **queue, DqThread *t) {
    register DqThread *pred;
    register DqThread *succ;

    pred = (DqThread *)queue;
    succ = pred->next;
    while (succ != 0) {
        if (succ == t) {
            pred->next = t->next;
            return;
        }
        pred = succ;
        succ = pred->next;
    }
}

/* func_80003C30 = osDestroyThread (libultra destroythread.c), IDO 7.1 -O1.
 * BOUNDARY FIX 2026-10-20: the old func_80003C24.s was [nop; jr ra; addiu sp,8]
 * + this fn + 3 pad nops. Those 3 head words are the tail of handwritten
 * func_800031F0 (its last bnez delay slot + its .L80003C28 exit), and
 * 0x80003C24 itself is a jal'd pop-8-and-return entry used by 31F0 -- moved
 * there as `alabel func_80003C24`. The 3 trailing nops are all-zero
 * inter-function pad (SUFFIX_BYTES_FORCE in the Makefile).
 * EXACT via twin port (twin-finder.py, 0.977): game_libs' copy gl_func_0006AF44
 * (game_libs_o1_6AF44.c, exact at -O1). __osDisableInt = 66B0,
 * __osDequeueThread = 3FF0, __osDispatchThread = 3E64 (alabel in 3D40.s),
 * __osRestoreInt = 66D0, __osActiveQueue = D_8000A41C,
 * __osRunningThread = D_8000A420. */
extern void func_80003E64(void);

void func_80003C30(int *target) {
    register int prelim;
    register int *curr;
    register int *next;

    prelim = func_800066B0();
    if (target == 0) {
        target = D_8000A420;
    } else {
        if (*(unsigned short *)((char *)target + 0x10) != 1) {
            func_80003FF0(target[2], target);
        }
    }
    if (D_8000A41C == target) {
        D_8000A41C = (int *)D_8000A41C[3];
    } else {
        curr = D_8000A41C;
        while (curr[1] != -1) {
            next = (int *)curr[3];
            if (next == target) {
                curr[3] = target[3];
                break;
            }
            curr = next;
        }
    }
    if (target == D_8000A420) {
        func_80003E64();
    }
    func_800066D0(prelim);
}


/* func_80003D40 = __osLeoInterrupt, func_800043E4 = __osLeoAbnormalResume
 * (libultra io/leointerrupt.c, IDO 7.1 -O1, the unit's flags). NOT a
 * handwritten region: the old "handwritten PI-DMA chain" reading and the
 * 2026-08-22 fragment merges glued one C function (sp -0x40, every local
 * sp-resident at -O1) and its static sibling at 0x43E4 (sp -0x20) into one
 * .s. Port of game_libs' copy gl_func_0006B0FC (game_libs_ido53_6B0FC.c,
 * twin-finder.py 0.936; the +58 words were __osLeoAbnormalResume).
 *   D_80019644 = __osDiskHandle, D_8000A3E0 = __OSGlobalIntMask,
 *   func_800048CC = __osLeoResume, func_800047E4 = abnormal-resume entry
 *   used by this copy, func_800056F0 = osEPiRawStartDma (post-addiu entry).
 * The old alabels func_80003E0C/3E54/3E64/3FF0/4030 and D_80003FE0 point
 * into the middle of this body (jal'd by other kernel code as stale
 * exceptasm addresses); they stay resolvable as absolute
 * undefined_syms_auto.txt entries.
 * Levers vs the game_libs donor: `stat` is volatile here (sp-resident,
 * reloaded at every test; bm_stat stays plain and is reused within a
 * block), and the PI/LEO registers must be raw volatile literals -- extern
 * volatile symbols let IDO share one lui across reads and hoist the
 * PI_STATUS load above the blockInfo store (-8 bytes). The .s was
 * de-symbolized to match (D_A46/D_A50 were splat guesses, same bytes). */
typedef struct {
    u32 errStatus;       /* 0x00 */
    void *dramAddr;      /* 0x04 */
    void *C2Addr;        /* 0x08 */
    u32 sectorSize;      /* 0x0C */
    u32 C1ErrNum;        /* 0x10 */
    u32 C1ErrSector[4];  /* 0x14 */
} __OSBlockInfo;         /* 0x24 */

typedef struct {
    u32 cmdType;         /* 0x00 */
    u16 transferMode;    /* 0x04 */
    u16 blockNum;        /* 0x06 */
    s32 sectorNum;       /* 0x08 */
    u32 track;           /* 0x0C */
    u32 bmCtlShadow;     /* 0x10 */
    u32 seqCtlShadow;    /* 0x14 */
    __OSBlockInfo block[2]; /* 0x18 */
} __OSTranxInfo;

typedef struct LeoPiHandle_s {
    struct LeoPiHandle_s *next; /* 0x00 */
    u8 type;                    /* 0x04 */
    u8 latency;                 /* 0x05 */
    u8 pageSize;                /* 0x06 */
    u8 relDuration;             /* 0x07 */
    u8 pulse;                   /* 0x08 */
    u8 domain;                  /* 0x09 */
    u32 baseAddress;            /* 0x0C */
    u32 speed;                  /* 0x10 */
    __OSTranxInfo transferInfo; /* 0x14 */
} LeoPiHandle;

extern LeoPiHandle *D_80019644;     /* __osDiskHandle */
#define LEO_PI_STATUS (*(volatile u32 *)0xA4600010) /* PI_STATUS_REG */
#define LEO_STATUS    (*(volatile u32 *)0xA5000508)
#define LEO_BM_CTL    (*(volatile u32 *)0xA5000510) /* write CTL / read STATUS */
extern void func_800048CC(void);    /* __osLeoResume */
extern void func_800047E4(void);    /* __osLeoAbnormalResume */
extern s32 func_800056F0(LeoPiHandle *, s32, u32, void *, u32); /* osEPiRawStartDma */

#define LEO_WAIT_ON_IOBUSY(stat) \
    { stat = LEO_PI_STATUS; while (stat & 3) stat = LEO_PI_STATUS; }

s32 func_80003D40(void)
{
    volatile u32 stat;
    volatile u32 pi_stat;
    u32 bm_stat;
    __OSTranxInfo *info;
    __OSBlockInfo *blockInfo;

    stat = 0;
    info = &D_80019644->transferInfo;
    blockInfo = &info->block[info->blockNum];
    pi_stat = LEO_PI_STATUS;
    if (pi_stat & 1) {
        D_8000A3E0 = D_8000A3E0 & ~0x800;
        blockInfo->errStatus = 29;
        func_800048CC();
        return 1;
    }
    LEO_WAIT_ON_IOBUSY(pi_stat);
    stat = LEO_STATUS;
    if (stat & 0x02000000) {
        LEO_WAIT_ON_IOBUSY(pi_stat);
        LEO_BM_CTL = info->bmCtlShadow | 0x01000000;
        blockInfo->errStatus = 0;
        return 0;
    }
    if (info->cmdType == 2)
        return 1;
    if (stat & 0x08000000) {
        LEO_WAIT_ON_IOBUSY(pi_stat);
        stat = LEO_STATUS;
        blockInfo->errStatus = 22;
        func_800048CC();
        LEO_PI_STATUS = 2;
        D_8000A3E0 |= 0x00100401;
        return 1;
    }
    if (info->cmdType == 1) {
        if ((stat & 0x40000000) == 0) {
            if (info->sectorNum + 1 != info->transferMode * 85) {
                blockInfo->errStatus = 24;
                func_800047E4();
                return 1;
            }
            LEO_PI_STATUS = 2;
            D_8000A3E0 |= 0x00100401;
            blockInfo->errStatus = 0;
            func_800048CC();
            return 1;
        }
        blockInfo->dramAddr = (void *)((u32)blockInfo->dramAddr + blockInfo->sectorSize);
        info->sectorNum++;
        func_800056F0(D_80019644, 1, 0x05000400, blockInfo->dramAddr,
                      blockInfo->sectorSize);
        return 1;
    }
    if (info->cmdType == 0) {
        if (info->transferMode == 3) {
            if ((s32)blockInfo->C1ErrNum + 17 < info->sectorNum) {
                blockInfo->errStatus = 0;
                func_800047E4();
                return 1;
            }
            if ((stat & 0x40000000) == 0) {
                blockInfo->errStatus = 23;
                func_800047E4();
                return 1;
            }
        } else {
            blockInfo->dramAddr = (void *)((u32)blockInfo->dramAddr + blockInfo->sectorSize);
        }
        bm_stat = LEO_BM_CTL;
        if ((bm_stat & 0x00200000 && bm_stat & 0x00400000) || bm_stat & 0x02000000) {
            if (blockInfo->C1ErrNum > 3) {
                if (info->transferMode != 3 || info->sectorNum > 0x52) {
                    blockInfo->errStatus = 23;
                    func_800047E4();
                    return 1;
                }
            } else {
                int errNum = blockInfo->C1ErrNum;
                blockInfo->C1ErrSector[errNum] = info->sectorNum + 1;
            }
            blockInfo->C1ErrNum += 1;
        }
        if (stat & 0x10000000) {
            if (info->sectorNum != 87) {
                blockInfo->errStatus = 24;
                func_800047E4();
            }
            if (info->transferMode == 2 && info->blockNum == 0) {
                info->blockNum = 1;
                info->sectorNum = -1;
                info->block[1].dramAddr =
                    (void *)((u32)info->block[1].dramAddr - info->block[1].sectorSize);
                blockInfo->errStatus = 22;
            } else {
                LEO_PI_STATUS = 2;
                D_8000A3E0 |= 0x00100401;
                info->cmdType = 2;
                blockInfo->errStatus = 0;
            }
            func_800056F0(D_80019644, 0, 0x05000000, blockInfo->C2Addr,
                          blockInfo->sectorSize * 4);
            return 1;
        }
        if (info->sectorNum == -1 && info->transferMode == 2 && info->blockNum == 1) {
            __OSBlockInfo *bptr = &info->block[0];
            if (bptr->C1ErrNum == 0) {
                if (((u32 *)bptr->C2Addr)[0] | ((u32 *)bptr->C2Addr)[1] |
                    ((u32 *)bptr->C2Addr)[2] | ((u32 *)bptr->C2Addr)[3]) {
                    bptr->errStatus = 24;
                    func_800047E4();
                    return 1;
                }
            }
            bptr->errStatus = 0;
            func_800048CC();
        }
        info->sectorNum++;
        if (stat & 0x40000000) {
            if (info->sectorNum > 0x54) {
                blockInfo->errStatus = 24;
                func_800047E4();
                return 1;
            }
            func_800056F0(D_80019644, 0, 0x05000400, blockInfo->dramAddr,
                          blockInfo->sectorSize);
            blockInfo->errStatus = 0;
            return 1;
        }
        if (info->sectorNum <= 0x54) {
            blockInfo->errStatus = 24;
            func_800047E4();
            return 1;
        }
        return 1;
    }
    blockInfo->errStatus = 4;
    func_800047E4();
    return 1;
}

void func_800043E4(void)
{
    __OSTranxInfo *info;
    volatile u32 pi_stat;

    info = &D_80019644->transferInfo;
    LEO_WAIT_ON_IOBUSY(pi_stat);
    LEO_BM_CTL = info->bmCtlShadow | 0x10000000;
    LEO_WAIT_ON_IOBUSY(pi_stat);
    LEO_BM_CTL = info->bmCtlShadow;
    func_800048CC();
    LEO_PI_STATUS = 2;
    D_8000A3E0 |= 0x00100401;
}

/* func_800044CC - kernel PI-event message dispatch (inlined send_mesg from
 * libultra exceptasm.s). Sends __osEventStateTab[OS_EVENT_PI].message to the
 * registered queue and wakes any blocked thread.
 * Reference: libreultra src/os/exceptasm.s send_mesg / __osPopThread /
 * __osEnqueueThread; src/os/seteventmesg.c (__osEventStateTab, __OSEventState).
 *   __osEventStateTab : __OSEventState[16] @ 0x80019510 (REAL reloc base;
 *                       prior "0x8001F510 discrepancy" note was WRONG).
 *   __OSEventState    : { OSMesgQueue *messageQueue; OSMesg message; } (8B).
 *   entry [8] = +0x40 = OS_EVENT_PI.
 *   OSMesgQueue fields: 0x0 mtqueue, 0x8 validCount, 0xC first,
 *                       0x10 msgCount, 0x14 msg[] (OSMesg*).
 *   func_80003E54 = __osPopThread, func_80003E0C = __osEnqueueThread,
 *   D_8000A418 = __osRunQueue.
 * The div/mfhi + break 7 (div-by-zero) + break 6 (INT_MIN/-1 overflow) are
 * IDO's signed `%` guards; placement AFTER the store matches the plain C `%`
 * at -O1.
 * EXACT 59/59 (2026-07-09, standalone probe; 5.3 -O1 AND 7.1 -O1 both emit
 * the target bytes -- this TU is already 7.1 -O1, so no donor split needed).
 * The old "pure register-coloring residual" note was three missing levers:
 *   - mq/es as members of a LOCAL STRUCT (struct locals stay sp-resident at
 *     -O1) -> homed at 0x28/0x2C and reloaded per statement;
 *   - `if (1) {}` BB-barrier as the first body statement -> kills the
 *     cross-BB register forwarding of the condition's mq/es temps, forcing
 *     the body-entry reloads (lw t1,0x28 / lw t6,0x2C);
 *   - `last` as a PLAIN fn-scope local declared after v (homed store
 *     sw t7,0x24 + register reuse for the index; a struct member here emits
 *     a second store, a `register` here recolors to s-regs), `t` as
 *     `register` (colors s0, no slot; frame 0x30 with the 0x20 hole). */
extern void func_80003E0C(void *, void *);  /* __osEnqueueThread */
void func_800044CC(void) {
    struct { OSMesgQueue *mq_; OSEventState *es_; } v;
    s32 last;
    register void *t;

    if ((v.mq_ = (OSMesgQueue *)(v.es_ = &__osEventStateTab[8])->queue) != 0 &&
        v.mq_->validCount < v.mq_->msgCount) {
        if (1) {}
        last = (v.mq_->first + v.mq_->validCount) % v.mq_->msgCount;
        v.mq_->msg[last] = (s32 *)v.es_->msg;
        v.mq_->validCount++;
        if (*(void **)v.mq_->mtqueue != 0) {
            t = func_80003E54(v.mq_);
            func_80003E0C(&D_8000A418, t);
        }
    }
}
