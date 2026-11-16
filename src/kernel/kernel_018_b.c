#include "common.h"
#include "libultra/os_thread.h"
#include "libultra/rmon.h"

/* Forward declarations */
extern s32 __osPiAccessQueue;
extern s32 __osPiAccessQueueEnabled;
extern s32 gUsoErrno;
extern void func_80005DC0(s32*, s32, s32);
extern void func_80005C00(void);
extern void func_80004FE0(s32*, s32*, s32);
extern void func_800053D0(s32*, s32*, s32);
extern s32 __OSGlobalIntMask;
extern s32 func_800066B0(void);
extern void func_800066D0(s32);
extern void func_80003D0C(s32*);
extern void* __osRunningThread;
extern s32 __osRunQueue;
extern s32 piacs_piAccessBuf;
extern s32 func_800009D8(void*, s32, s32, void*);
extern void func_80003FF0(s32, void*);
extern void* func_80003E54(void*);
extern void func_8000A110(void*);
extern void func_8000857C(void);
extern void func_8000785C(s32);
extern void func_80008ED0(void);
extern void func_80007760(void);
typedef void (*FuncPtr2)(void*, void*);
extern FuncPtr2 gUsoPrintfHook;
extern s32 D_8000A32C;
extern s32 D_8000A340;
extern s32 gUsoCtx;
extern s32 gFsDeviceTable;
extern s32 __osActiveQueue;
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



/* func_800066B0+func_800066D0 INCLUDE_ASM moved to kernel_017.c on
 * 2026-05-05 to enable the 3-way merge with the parent fragment
 * __rmonReadWordAt. Per
 * feedback_cross_file_fragment_unblock_via_move_then_merge.md: moving
 * just the INCLUDE_ASM (no C body change) is layout-neutral when both
 * .o files reside in adjacent linker-script slots and the destination
 * .o gets exactly the bytes the source loses. Adjacent .ld slots
 * verified: kernel_017.c.o then kernel_018.c.o.
 * 800066D0 caller-entry symbol preserved via undefined_syms_auto.txt
 * (still resolves at 0x800066D0 inside the merged 800066B0 body). */

/* split from kernel_018.c - 2026-06-10 kernel ROM-order relayout */


/* func_800066F0 split out to kernel_048.c (-O1) */

/* __rmonCopyWords split out to kernel_049.c (-O1) */

/* rmon_strcpy split out to kernel_047.c (-O1) */

/* __rmonReadMem - verified structural decode (kernel, 0x1FC, 127
 * insns, rmon memory-read command handler). rmon family (cf.
 * __rmonGetVRegs / __rmonListThreads; __rmonUtilityBuffer /
 * __rmonMaskIdleThreadInts).
 *   s32 __rmonReadMem(RmonMsg *msg) {
 *       UB *ub = &__rmonUtilityBuffer;
 *       ub->0x4 = msg->0x4;                       // domain
 *       ub->0xC = msg->0xC;                       // type/id echo
 *       ub->0x6 = 0;
 *       if (msg->0x10 == -1)       return -5;     // bad address
 *       if (msg->0x14 >= 0x401)    return -8;     // len > 1024
 *       if (msg->0x9 == 1)                        // RSP IMEM win
 *           ok = msg->0x10 >= 0x04001000 &&
 *                msg->0x10 + msg->0x14 < 0x04002000;
 *       else                                       // RSP DMEM win
 *           ok = msg->0x10 >= 0x04000000 &&
 *                msg->0x10 + msg->0x14 < 0x04001000;
 *       if (ok) {
 *           phys = osVirtualToPhysical(msg->0x10);// func_80004B30
 *           func_800074A0(phys, ...);             // PI read
 *           func_800066F0(ub->payload, ..., n);   // copy out
 *       }
 *       __rmonMaskIdleThreadInts(ub, len, flag);          // ship reply
 *       return 0;
 *   }
 * Struct-typing reference: RmonMsg msg - msg->0x4 (4) u8 domain,
 * msg->0x9 (9) u8 mode (1 = RSP IMEM 0x04001000..0x04002000 else
 * DMEM 0x04000000..0x04001000), msg->0xC (12) type/id, msg->0x10
 * (16) target address (-1 = invalid), msg->0x14 (20) byte length
 * (must be < 0x401 / 1024). __rmonUtilityBuffer: ->0x4 domain,
 * ->0x6 (6) u16 cleared, ->0xC status/echo. func_80004B30 =
 * osVirtualToPhysical, func_800074A0 = PI/cart IO read,
 * func_800066F0 = bounded copy, __rmonMaskIdleThreadInts = the shared rmon
 * emit. The RSP DMEM/IMEM range gate prevents reads outside the
 * SP memory window. Caps <80: rmon buffer build + multi-range
 * sltu validation + osVirtualToPhysical/PI-read + branch-likely +
 * 4 callees. MATCHED byte-exact (agent-b 2026-06-23): -O1, register
 * flags var_s0/var_s1 -> s0/s1 (loop accumulator reuses s0), decl
 * order pins the stack-slot layout (frame 0x40), and the SP-range
 * `len + addr` summands plus `sp34 = len+0x10` ordered before `p =
 * &sp34` fix the addu commutativity + token-loop scheduling. */
extern void __rmonSendHeader(void *, s32, s32);
extern s32 func_80004B30(u32);
extern s32 func_800066F0(void *, s32, s32);
extern void func_800074A0(u32, u32);
extern char __rmonUtilityBuffer;
/* rmon read-memory request handler. Stamp the reply header (__rmonUtilityBuffer:
 * tag=req->unk4, unkC=req->unkC, unk6=0). Reject a -1 address (-5) or a length
 * over 0x400 (-8). For an instruction-space read (req->unk9==1) require the range
 * to lie wholly within SP IMEM (0x04001000..0x04002000) or SP DMEM
 * (0x04000000..0x04001000) else -5; otherwise validate the RDRAM address via
 * func_80004B30. Then set the reply length (len+0x10), poll 4 bytes of the read
 * token (func_800066F0), send the header, and kick the transfer
 * (func_800074A0(addr, len)). */
s32 __rmonReadMem(KKReadRequest *arg0) {
    char *p;
    s32 sp38;
    s32 sp34;
    KKReadRequest *sp30 = arg0;
    KKBufferEvent *sp2C = (KKBufferEvent *)&__rmonUtilityBuffer;
    u32 addr;
    register s32 var_s0;
    register s32 var_s1;

    sp2C->header.code = arg0->header.code;
    sp2C->object = sp30->object;
    sp2C->header.error = TV_ERROR_NO_ERROR;
    if (sp30->addr == 0xFFFFFFFFU) {
        return TV_ERROR_INVALID_ADDRESS;
    }
    if (sp30->nbytes >= 0x401U) {
        return TV_ERROR_INVALID_CAPABILITY;
    }
    if (arg0->header.method == RMON_RSP) {
        /* (*sp30).f spelling in both range tests: sp30->f reorders the addu / sltu temps */
        if (((*sp30).addr < 0x04001000U) || (((*sp30).nbytes + (*sp30).addr) >= 0x04002000U)) {
            var_s0 = 0;
        } else {
            var_s0 = 1;
        }
        if (var_s0 == 0) {
            if (((*sp30).addr < 0x04000000U) || (((*sp30).nbytes + (*sp30).addr) >= 0x04001000U)) {
                var_s1 = 0;
            } else {
                var_s1 = 1;
            }
            if (var_s1 == 0) {
                return TV_ERROR_INVALID_ADDRESS;
            }
        }
    } else {
        if (func_80004B30(sp30->addr) == -1) {
            return TV_ERROR_INVALID_ADDRESS;
        }
    }
    addr = sp30->addr;
    sp2C->header.length = sp30->nbytes + 0x10;
    sp34 = sp30->nbytes + 0x10;
    p = (char *) &sp34;
    sp38 = 0;
    do {
        var_s0 = func_800066F0(p + sp38, 4 - sp38, 8);
        sp38 += var_s0;
    } while (sp38 < 4);
    __rmonSendHeader(sp2C, 0x10, KK_TYPE_REPLY);
    func_800074A0(addr, sp30->nbytes);
    return 0;
}

/* rmon memory-write command handler (write sibling of __rmonReadMem). Validate
 * the RDRAM address (func_80004B30) / length (<=0x400). For an SP-window address
 * (0x04000000..0x05000000): unaligned single-byte write -> read-modify-write
 * via func_80006A98/func_80006A50 (the byte-merge is an if/else-if/else CHAIN
 * on the misalignment 1/2/else); aligned -> word-loop via func_80006A50. Else
 * (general RAM) bulk-write via func_80006AEC. Then send the 0x10 reply header.
 *
 * MATCHED byte-exact 147/147 (agent-f 2026-07-10): IDO 5.3 -O1 donor
 * (kernel_018_b_ido53_698C.c, REPLACE_FUNC_BODY splice) — 5th
 * 5.3-vs-7.1 temp-rematerialization crack. The old "coloring/frame-layout
 * cap" note fell to four levers: else-if chain structure, first byte-read
 * through the RAW param (keeps the deferred `or s0,t6` commit), register-var
 * DEAD HOME SLOTS in decl order (`register arg0` first = home 0x44,
 * `register var_s1` last = home 0x20 -> frame 0x48 with struct at 0x34),
 * and order ties (sp28-before-sp24 init, len-first range sum, tail
 * tag->zero->id). See the donor file header for the full mechanism. */
extern void func_80006A50(s32, s32);
extern void func_80006AEC(s32, void *, s32);
extern void func_800073F8(void *, s32, s32);

s32 __rmonWriteMem(KKWriteRequest *sp48) {
    register KKWriteRequest *arg0;
    KKObjectEvent sp34;
    s32 sp30;
    s32 sp2C;
    u32 sp28;
    s32 *sp24;
    register s32 var_s1;

    arg0 = sp48;
    if ((sp48->writeHeader.header.method == RMON_CPU) && (func_80004B30(arg0->writeHeader.addr) == -1)) {
        return TV_ERROR_INVALID_ADDRESS;
    }
    if (arg0->writeHeader.nbytes >= 0x401U) {
        return TV_ERROR_INVALID_CAPABILITY;
    }
    /* (*arg0).f in the range test: the -> spelling reorders its addu / sltu temps */
    if (((*arg0).writeHeader.addr < 0x04000000U) || (((*arg0).writeHeader.nbytes + (*arg0).writeHeader.addr) >= 0x05000000U)) {
        var_s1 = 0;
    } else {
        var_s1 = 1;
    }
    if (var_s1 != 0) {
        sp30 = arg0->writeHeader.addr & 3;
        if (sp30 != 0) {
            if (arg0->writeHeader.nbytes != 1) {
                return TV_ERROR_INVALID_ADDRESS;
            }
            sp2C = func_80006A98(arg0->writeHeader.addr & ~3);
            if (sp30 == 1) {
                sp2C = (arg0->buffer[0] << 0x10) | (sp2C & 0xFF00FFFF);
            } else if (sp30 == 2) {
                sp2C = (arg0->buffer[0] << 8) | (sp2C & 0xFFFF00FF);
            } else {
                sp2C = arg0->buffer[0] | (sp2C & ~0xFF);
            }
            func_80006A50(arg0->writeHeader.addr & ~3, sp2C);
            goto block_26;
        }
        sp28 = arg0->writeHeader.nbytes >> 2;
        sp24 = (s32 *)arg0->buffer;
        if (arg0->writeHeader.nbytes & 3) {
            return TV_ERROR_INVALID_ADDRESS;
        }
        var_s1 = sp28;
        sp28 -= 1;
        if (var_s1 != 0) {
            do {
                func_80006A50(arg0->writeHeader.addr, *sp24++);
                arg0->writeHeader.addr = arg0->writeHeader.addr + 4;
                var_s1 = sp28;
                sp28 -= 1;
            } while (var_s1 != 0);
        }
        goto block_26;
    }
    func_80006AEC(arg0->writeHeader.addr, arg0->buffer, arg0->writeHeader.nbytes);
block_26:
    sp34.header.code = arg0->writeHeader.header.code;
    sp34.header.error = TV_ERROR_NO_ERROR;
    sp34.object = arg0->writeHeader.object;
    func_800073F8(&sp34, sizeof(sp34), KK_TYPE_REPLY);
    return 0;
}
