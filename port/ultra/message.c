/* port/ultra: message queues, events, interrupt mask.
 *
 * Queues keep libultra's ring-buffer layout and arithmetic exactly
 * (first / validCount / msgCount in the game's OSMesgQueue), because game code
 * reads validCount directly (MQ_IS_EMPTY / MQ_IS_FULL style tests). The thread
 * lists (mtqueue / fullqueue) stay NULL: nothing is ever blocked on the host;
 * a blocking call waits through port_ultra_wait() instead. */

#include "ultra_int.h"

static __OSEventState sEventTab[OS_NUM_EVENTS];
static OSIntMask sIntMask = 0x003FFF01; /* OS_IM_ALL */
u32 port_ultra_activity;

void osCreateMesgQueue(OSMesgQueue *mq, OSMesg *msg, s32 count) {
    mq->mtqueue = NULL;
    mq->fullqueue = NULL;
    mq->validCount = 0;
    mq->first = 0;
    mq->msgCount = count;
    mq->msg = msg;
}

static int mq_not_full(void *ctx) {
    OSMesgQueue *mq = (OSMesgQueue *)ctx;
    return mq->validCount < mq->msgCount;
}

static int mq_not_empty(void *ctx) {
    return ((OSMesgQueue *)ctx)->validCount != 0;
}

s32 osSendMesg(OSMesgQueue *mq, OSMesg msg, s32 flag) {
    s32 last;

    if (mq->validCount >= mq->msgCount) {
        if (flag == OS_MESG_NOBLOCK) {
            return -1;
        }
        if (port_ultra_wait(mq_not_full, mq, "osSendMesg on a full queue", mq) != 0) {
            return -1;
        }
    }
    last = (mq->first + mq->validCount) % mq->msgCount;
    mq->msg[last] = msg;
    mq->validCount++;
    port_ultra_activity++;
    return 0;
}

s32 osJamMesg(OSMesgQueue *mq, OSMesg msg, s32 flag) {
    if (mq->validCount >= mq->msgCount) {
        if (flag == OS_MESG_NOBLOCK) {
            return -1;
        }
        if (port_ultra_wait(mq_not_full, mq, "osJamMesg on a full queue", mq) != 0) {
            return -1;
        }
    }
    mq->first = (mq->first + mq->msgCount - 1) % mq->msgCount;
    mq->msg[mq->first] = msg;
    mq->validCount++;
    port_ultra_activity++;
    return 0;
}

s32 osRecvMesg(OSMesgQueue *mq, OSMesg *msg, s32 flag) {
    if (mq->validCount == 0) {
        if (flag == OS_MESG_NOBLOCK) {
            return -1;
        }
        if (port_ultra_wait(mq_not_empty, mq, "osRecvMesg on an empty queue", mq) != 0) {
            return -1;
        }
    }
    if (msg != NULL) {
        *msg = mq->msg[mq->first];
    }
    mq->first = (mq->first + 1) % mq->msgCount;
    mq->validCount--;
    port_ultra_activity++;
    return 0;
}

void osSetEventMesg(OSEvent e, OSMesgQueue *mq, OSMesg msg) {
    if (e >= OS_NUM_EVENTS) {
        port_ultra_log("ultra: osSetEventMesg: bad event %u", (unsigned)e);
        return;
    }
    sEventTab[e].messageQueue = mq;
    sEventTab[e].message = msg;
}

/* What the interrupt handler does: post without blocking, drop when full. */
void port_ultra_post_event(OSEvent e) {
    if (e < OS_NUM_EVENTS && sEventTab[e].messageQueue != NULL) {
        osSendMesg(sEventTab[e].messageQueue, sEventTab[e].message, OS_MESG_NOBLOCK);
    }
}

/* Interrupts never preempt the host simulation thread; the mask is only stored. */
OSIntMask osSetIntMask(OSIntMask mask) {
    OSIntMask old = sIntMask;
    sIntMask = mask;
    return old;
}

void port_ultra_events_reset(void) {
    memset(sEventTab, 0, sizeof(sEventTab));
    sIntMask = 0x003FFF01;
    port_ultra_activity = 0;
}

PORT_ALIAS(osCreateMesgQueue, gl_osCreateMesgQueue);
PORT_ALIAS(osSendMesg, gl_osSendMesg);
PORT_ALIAS(osJamMesg, gl_osJamMesg);
PORT_ALIAS(osRecvMesg, gl_osRecvMesg);
PORT_ALIAS(osSetEventMesg, gl_osSetEventMesg);
PORT_ALIAS(osSetIntMask, gl_osSetIntMask);
