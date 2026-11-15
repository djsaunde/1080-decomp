#include "common.h"
#include "gdl/gl_data.h"

/* gl_osSendMesg = libultra osSendMesg (sched/sendmesg.c), statically linked
 * into the game_libs USO. Byte-exact masked-twin of matched kernel osSendMesg
 * (kernel_009): block while the queue is full (or fail if non-blocking), append
 * msg to the ring buffer, then if a thread is waiting on mtqueue wake it. IDO
 * 5.3 -O1, spliced into game_libs_post1b.c.o via REPLACE_FUNC_BODY. All OS-API
 * callees collapse to the address-0 placeholder gl_func_00000000; the single
 * global (running-thread ptr, kernel __osRunningThread) maps to D_00000000. */
#include "libultra/os_message.h"
#include "uso_call.h"

extern int gl_func_00000000();
extern int D_00000000; /* __osRunningThread (OSThread *) */

int gl_osSendMesg(OSMesgQueue *mq, OSMesg msg, int flags) {
    register int saveMask = gl_func_00000000();
    register int last;
    while (mq->validCount >= mq->msgCount) {
        if (flags == OS_MESG_BLOCK) {
            ((OSThread *)gl___osRunningThread)->state = 8; /* OS_STATE_WAITING */
            gl_func_00000000((int *)&mq->fullqueue);
        } else {
            CALL(gl___osRestoreInt)(saveMask);
            return -1;
        }
    }
    last = (mq->first + mq->validCount) % mq->msgCount;
    mq->msg[last] = msg;
    mq->validCount++;
    if (mq->mtqueue->next != 0) {
        register void *t = (void *)CALL(gl___osPopThread)(mq);
        CALL(gl_osStartThread)(t);
    }
    CALL(gl___osRestoreInt)(saveMask);
    return 0;
}
