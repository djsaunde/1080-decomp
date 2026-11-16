#include "gdl/gl_data.h"
#include "uso_call.h"
#include "libultra/os_event.h"
/* -O1 donor for gl___osLeoResume — 59-insn ring-buffer append with notify
 * callback (frame 0x30, saves s0). PROVEN EXACT 59/59 at IDO 7.1 -O1
 * (2026-07-08 standalone probe; plain -O1, no -g3). post1b builds -O2 (the
 * old wrap was 38%: -O2 register-promotes head). Levers:
 *   (1) guard fused into ONE expression `(head = load) == 0 || count >= cap`
 *       = the loaded reg (t8) stays live across the beq (home store rides the
 *       delay slot) and feeds the count/cap loads directly — separate
 *       statements reload from the sp home (+1 insn);
 *   (2) NESTED-IF tail (no early return) = uopt keeps the head reload (t6)
 *       live into the call, filling the jal delay with `or a0,t6` (the
 *       early-return form re-loads a0 from the home slot instead);
 *   (3) decl order es/mq/last = sp+0x2C/0x28/0x24 homes;
 *       `register int res;` = $s0 across the second call. */
extern int gl_func_00000000();
extern int D_00000000;

/* libultra leointerrupt.c __osLeoResume: post __osEventStateTab[OS_EVENT_PI]
 * to its queue (inline osSendMesg NOBLOCK, no interrupt bracket). */
void gl___osLeoResume(void) {
    __OSEventState *es;
    OSMesgQueue *mq;
    int last;
    register int res;

    es = &((__OSEventState *)&gl___osEventStateTab)[OS_EVENT_PI];
    if ((mq = ((__OSEventState *)&gl___osEventStateTab)[OS_EVENT_PI].messageQueue) == 0 ||
        mq->validCount >= mq->msgCount) {
        return;
    }
    last = (mq->first + mq->validCount) % mq->msgCount;
    mq->msg[last] = es->message;
    mq->validCount = mq->validCount + 1;
    if (mq->mtqueue->next != 0) {
        res = CALL(gl___osPopThread)(&mq->mtqueue);
        gl_func_00000000((char *)&gl___osRunQueue, res); /* UNNAMED: mid-entry jal gl___osExceptionPreamble+0x730 (= __osEnqueueThread, no symbol of its own) */
    }
}
