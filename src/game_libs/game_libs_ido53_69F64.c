#include "common.h"
#include "gdl/gl_data.h"

/* gl_osRecvMesg = libultra osRecvMesg (os/recvmesg.c verbatim), statically
 * linked into the game_libs USO right after osSendMesg (gl_osSendMesg) and
 * osViGetCurrentLine (osViGetCurrentLine). IDO 5.3 -O1 (the 69E04 recipe),
 * spliced into game_libs_post1b.c.o via REPLACE_FUNC_BODY. Block while the
 * queue is empty (or fail if non-blocking), pop msg[first], advance first
 * modulo msgCount, and if a thread waits on fullqueue, wake it. All OS-API
 * callees (__osDisableInt / __osRestoreInt / __osEnqueueAndYield /
 * __osPopThread / osStartThread) collapse to the address-0 placeholder
 * gl_func_00000000; __osRunningThread maps to D_00000000. */
#include "libultra/os_message.h"

extern int gl_func_00000000();
extern int D_00000000; /* __osRunningThread (OSThread *) */

int gl_osRecvMesg(OSMesgQueue *mq, OSMesg *msg, int flags) {
    register int saveMask = gl_func_00000000();

    while (mq->validCount == 0) {
        if (flags == OS_MESG_NOBLOCK) {
            gl_func_00000000(saveMask);
            return -1;
        }
        ((OSThread *)gl___osRunningThread)->state = 8; /* OS_STATE_WAITING */
        gl_func_00000000((int *)&mq->mtqueue);
    }
    if (msg != 0) {
        *msg = mq->msg[mq->first];
    }
    mq->first = (mq->first + 1) % mq->msgCount;
    mq->validCount--;
    if (mq->fullqueue->next != 0) {
        gl_func_00000000(gl_func_00000000((int *)&mq->fullqueue));
    }
    gl_func_00000000(saveMask);
    return 0;
}
