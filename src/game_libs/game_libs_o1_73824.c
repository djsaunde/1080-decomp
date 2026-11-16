#include "uso_call.h"
/* -O1 donor for gl_osSetTimer — 53-insn 8-word record init + u64-return
 * call + conditional u64-arg call (frame 0x20, saves ra). PROVEN EXACT 53/53
 * at IDO 7.1 -O1 (2026-07-08 standalone probe; 5.3 -O1 emits the identical
 * 53 words). Signature is the load-bearing lever: params 2 and 3 are
 * unsigned long long (OSTime) — the 8-byte alignment makes p skip $a1 (its slot
 * 0x24 is never homed, matching the target's missing sw a1) and land in
 * a2/a3, with q on the stack at 0x30/0x34 (p = countdown, q = interval).
 * Other levers:
 *   (1) u64 pair copies (t->interval = q / t->value = p|q) give the
 *       shared-dst-reload lw/lw/sw/sw quads (lo word first);
 *   (2) `if (p != 0)` on a u64 = the target's bnez-hi / beqz-lo two-branch
 *       chain, and the then-arm reuses the condition's loaded pair;
 *   (3) r = __osInsertTimer(t) homes v0/v1 at 0x18/0x1C; passing r
 *       as a u64 arg reloads them into a0/a1 (lw a1 fills the jal delay).
 * Spliced into game_libs_post2b_d.c.o via REPLACE_FUNC_BODY (real -O1
 * compiler output; blank-extern relocs imported by the splice). */
#include "libultra/os_timer.h"
/* libultra osSetTimer (timer/settimer.c): timer->interval = interval;
 * timer->value = countdown ? countdown : interval; returns 0. */
extern unsigned long long gl_func_00000000_ll(OSTimer *);
extern void gl_func_00000000(unsigned long long);
extern OSTimer *D_00000000_a; /* __osTimerList (list head sentinel pointer) */

int gl_osSetTimer(OSTimer *t, OSTime countdown, OSTime interval, OSMesgQueue *mq, OSMesg msg) {
    OSTime r;

    t->next = 0;
    t->prev = 0;
    t->interval = interval;
    if (countdown != 0) {
        t->value = countdown;
    } else {
        t->value = interval;
    }
    t->mq = mq;
    t->msg = msg;
    r = XCALL(gl_func_00000000_ll, __osInsertTimer)(t);
    if (D_00000000_a->next == t) {
        CALL(__osSetTimerIntr)(r);
    }
    return 0;
}
