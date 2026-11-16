#include "common.h"

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
extern s32 sFmt_USO_Version_8000A32C;
extern s32 sStr_3_1_8000A340;
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









/* __rmonSendData - verified structural decode (kernel, 0x120, 72
 * insns, memory-dump / word-stream transmit; rmon-family helper).
 * Struct-typing reference: addr/len are a raw memory range; the
 * routine streams ceil(len/4) 32-bit words out via func_80005584
 * (send-one-word). Addresses in [0x04000000, 0x05000000) are the
 * N64 cartridge/PI domain (not CPU-load-safe), so those words are
 * fetched with func_80008FB0 (PI/DMA read into a stack tmp);
 * everything else is a direct lw. A misaligned base forces every
 * word through func_80006AEC (unaligned 4-byte read). func_80005534
 * = end-of-stream flush. (Same rmon memory-inspect family as
 * __rmonGetVRegs / the __rmon* handlers.) Caps <80: alignment
 * branch + cart-range sltu check + 4 callees + decrement loop with
 * spilled counter/cursor at sp+0x30/0x34. INCLUDE_ASM remains build
 * path (no episode; tautology-trap rule).
 * 2026-05-28 opt-level probe (NEGATIVE): the entry double arg-home-spill
 * (sw a1;lw / sw a0;lw) + stack-spilled loop vars look like an -O1/-g
 * marker, but building this TU at -O1 is WORSE (7% raw vs -O2's 15%); -O2
 * is correct. The gap is loop-STRUCTURE (the goto-based aligned/cart/
 * misaligned three-way control flow + stack-resident n/addr), not opt
 * level. Improving it needs a structural C rewrite to mirror the target's
 * exact basic-block layout — not an opt-flag or file-split. */
extern void func_80006AEC(void *dst, void *src, int n);
extern void func_80005584(int word);
extern void func_80008FB0(void *src, void *dst);
extern void func_80005534(void);
/* rmon word-streamer: write len bytes (word at a time) via func_80005584,
 * routing SP-range addresses through func_80008FB0. MATCHED 2026-07-10
 * (92.3% -> 72/72 exact): the "Residual 6 = pure -O1 regalloc/sched ties NO
 * C shape resolves" verdict was a 7.1-only property — 4th member of the
 * IDO 5.3-vs-7.1 temp-rematerialization class (7564/8264/87B4). Exact body
 * lives in kernel_018_d_ido53_70A0.c, compiled IDO 5.3 -O1 and spliced in
 * via REPLACE_FUNC_BODY; key lever = `int *p` + post-increment at the use
 * site (`func(p++)`, `*p++`), which 5.3 materializes through the old value
 * (`move a0,t2` keep-old-p / `lw a0,0(t4)` old-base) while 7.1 folds -4 in
 * place from any C shape. The unaligned-loop delay-slot store-order tie
 * resolved with it (same coloring cascade). This 7.1-compiled body below is
 * a placeholder slot for the splice (structure kept for readability).
 * --- HISTORY ---
 * 2026-06-04 (49.6->84.0%): aligned path as if-BODY; both SP-range bounds.
 * 2026-06-23 (84.0->92.3%): frame -0x38 via decl order p/n/tmp/tmp2 +
 * separate cursor p + two distinct tmps + `p += 4; func(p - 4)` hoist.
 * NEGATIVE at 5.3: explicit `char *oldp = p; p += 4;` homes oldp (+4 insns,
 * frame 0x40) — only the post-increment expression temp stays register-only. */
void __rmonSendData(char *addr, int len) {
    int *p = (int *)addr;
    unsigned int n = ((unsigned int)len + 3) >> 2;
    int tmp;
    int tmp2;
    if (!((unsigned int)addr & 3)) {
        while (n--) {
            if ((unsigned int)p >= 0x04000000U && (unsigned int)p < 0x05000000U) {
                func_80008FB0(p++, &tmp);
                func_80005584(tmp);
            } else {
                func_80005584(*p++);
            }
        }
    } else {
        while (n--) {
            func_80006AEC(&tmp2, p, 4);
            func_80005584(tmp2);
            p++;
        }
    }
    func_80005534();
}

extern void func_800082EC(void);
extern void func_80008E08(void);
extern void func_80008E38(void);
extern void func_80008EA0(void);
extern volatile s32 rmon_somethingToDo;
extern s32 __rmonActive;
extern s8 rmon_cmdinptr;
extern s8 rmon_cmdoutptr;
extern s32 rmon_state;
extern void *rmon_inPointer;
extern s32 rmon_inbuffer;
extern s32 __rmonMQ;
/* rmon main dispatch loop (__rmonIOhandler-class). Init state, then loop
 * forever: recv a flag word from __rmonMQ, OR it into rmon_somethingToDo, and dispatch
 * each set bit (2 -> func_80008E08, 4 -> func_80008E38, 8 -> func_80008EA0,
 * 0x10 -> just clear), clearing the bit as it goes; bit 0x20 is a one-shot
 * (clear and continue), otherwise re-loop. */
void rmonMain(void) {
    register s32 phantom;
    s32 sp20;

    rmon_somethingToDo = 0;
    rmon_cmdoutptr = 0;
    rmon_cmdinptr = 0;
    func_800082EC();
    __rmonActive = 1;
    rmon_state = 0;
    rmon_inPointer = &rmon_inbuffer;
    phantom = 0;
loop_1:
    func_80004FE0(&__rmonMQ, &sp20, 1);
    rmon_somethingToDo |= sp20;
    if (rmon_somethingToDo & 2) {
        rmon_somethingToDo &= ~2;
        func_80008E08();
    }
    if (rmon_somethingToDo & 4) {
        rmon_somethingToDo &= ~4;
        func_80008E38();
    }
    if (rmon_somethingToDo & 8) {
        rmon_somethingToDo &= ~8;
        func_80008EA0();
    }
    if (rmon_somethingToDo & 0x10) {
        rmon_somethingToDo;
        rmon_somethingToDo &= 0xEF;
    }
    if (!(rmon_somethingToDo & 0x20)) {
        goto loop_1;
    }
    rmon_somethingToDo;
    rmon_somethingToDo &= 0xDF;
    goto loop_1;
}
