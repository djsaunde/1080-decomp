/* IDO 5.3 -O1 donor for __rmonListThreads — rmon thread-list command handler
 * (__rmonListThreads family; 77 insns, frame 0x30). PROVEN EXACT 77/77
 * (2026-07-10 standalone probe vs asm/nonmatchings/kernel/__rmonListThreads.s;
 * only HI16/LO16(__rmonUtilityBuffer) + R_MIPS_26 reloc placeholders differ,
 * resolved at link). The parent unit kernel_018_f.c is IDO 7.1 -O1, which
 * caps 1 insn short: 7.1 folds the tail's `count*4` shift in place
 * (`sll a1,a1,2`) while the target (and 5.3) materialize it through a temp
 * (`sll t0,a1,2; or a1,t0,zero`) — the documented "clean C at -O1 never
 * re-materializes through the temp" residual is a 7.1-only property. Same
 * 5.3 -O1 island vein as osInitialize/osCartRomInit/osCreatePiManager.
 * All other levers (frame layout, msg-vs-m reload split, direct-global
 * store in the -1 arm, pad slot before ub) carried over unchanged from the
 * 2026-06-23 kernel_018_f.c rebuild. Spliced into kernel_018_f.c.o via
 * REPLACE_FUNC_BODY (real 5.3 -O1 output; donor relocs imported by the
 * splice). */
#include "common.h"
#include "libultra/os_thread.h"
#include "libultra/rmon.h"

extern void func_800073F8();        /* __rmonSendReply (ultralib rmontask.c) */
extern char __rmonUtilityBuffer;
extern OSThread *func_80009C30(void); /* __osGetActiveQueue */

/* typed on rmon.h / os_thread.h like the kernel_018_f.c copy (agent-a run 531) */
s32 __rmonListThreads(KKObjectRequest *msg) {
    int pad;
    KKObjsEvent *ub = (KKObjsEvent *)&__rmonUtilityBuffer;
    register KKObjectRequest *m = msg;
    register OSThread *t;
    if (m->object == -1) {
        ((KKObjsEvent *)&__rmonUtilityBuffer)->object = RMON_PID_CPU;
    } else {
        ub->object = m->object;
    }
    if (msg->header.method == RMON_RSP) {
        ub->objs.number = 1;
        ub->objs.objects[0] = RMON_TID_RSP;
    } else {
        t = func_80009C30();
        ub->objs.number = 0;
        if (t->priority != -1) {
            do {
                if (t->id != 0) {
                    ub->objs.objects[ub->objs.number] = t->id;
                    ub->objs.number += 1;
                }
                t = t->tlnext;
            } while (t->priority != -1);
        }
    }
    ub->header.code = m->header.code;
    ub->header.error = TV_ERROR_NO_ERROR;
    func_800073F8(ub, ub->objs.number * 4 + 0x14, 1);
    return 0;
}
