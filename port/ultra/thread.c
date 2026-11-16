/* port/ultra: threads, services and the pump.
 *
 * The game creates five N64 threads (boot/idle, main, RcpMgr, pad, fault;
 * plan section 13.2). None of them runs as a host thread. osCreateThread /
 * osStartThread record the thread in the game's own OSThread (so code that
 * reads priority / id / state still sees sensible values) and in a side table.
 * A started thread whose entry has a registered service gets its step
 * function called by the pump; any other started thread is parked.
 *
 * The pump runs at every blocking point: osRecvMesg / osSendMesg with
 * OS_MESG_BLOCK on an empty / full queue, and osYieldThread. If no service
 * makes progress, the wait advances virtual VI time by one retrace (that is
 * what a blocked N64 thread waits for: the next interrupt) and retries. After
 * maxIdleRetraces retraces with no progress the wait is a deadlock. */

#include "ultra_int.h"

#define MAX_THREADS 32
#define MAX_SERVICES 16
#define MAX_BUSY_PUMPS 100000 /* progress pumps one wait may run before counting as idle */

typedef struct ThreadRec {
    OSThread *t;
    void (*entry)(void *);
    void *arg;
    PortThreadStep step;
    const char *name;
    u8 started;
} ThreadRec;

typedef struct ServiceRec {
    void (*entry)(void *);
    PortThreadStep step;
    const char *name;
} ServiceRec;

static ThreadRec sThreads[MAX_THREADS];
static ServiceRec sServices[MAX_SERVICES];
static u32 sNumServices;
static OSThread sHostThread; /* the simulation thread, i.e. "the current thread" */
static OSThread sThreadTail;  /* __osThreadTail: priority -1 ends the active list */

/* libultra's active-thread list (every created thread, linked by tlnext, ending
 * at the priority -1 tail). dbg_printThreads walks it under the name
 * gl___osActiveQueue_34458_head. */
OSThread *__osActiveQueue = &sThreadTail;

static void active_unlink(OSThread *t) {
    OSThread **pp = &__osActiveQueue;

    while (*pp != &sThreadTail && *pp != NULL) {
        if (*pp == t) {
            *pp = t->tlnext;
            t->tlnext = NULL;
            return;
        }
        pp = &(*pp)->tlnext;
    }
}

static void active_link(OSThread *t) {
    t->tlnext = __osActiveQueue;
    __osActiveQueue = t;
}
static int sPumpDepth;
static u32 sYieldIdle;
static u32 sYieldActivity;

static ThreadRec *find_rec(OSThread *t, int create) {
    ThreadRec *free_rec = NULL;
    int i;

    for (i = 0; i < MAX_THREADS; i++) {
        if (sThreads[i].t == t) {
            return &sThreads[i];
        }
        if (sThreads[i].t == NULL && free_rec == NULL) {
            free_rec = &sThreads[i];
        }
    }
    if (create && free_rec != NULL) {
        memset(free_rec, 0, sizeof(*free_rec));
        free_rec->t = t;
        return free_rec;
    }
    if (create) {
        host_fatal("ultra: more than %d threads", MAX_THREADS);
    }
    return NULL;
}

static const ServiceRec *find_service(void (*entry)(void *)) {
    u32 i;

    for (i = 0; i < sNumServices; i++) {
        if (sServices[i].entry == entry) {
            return &sServices[i];
        }
    }
    return NULL;
}

void port_ultra_register_service(void (*entry)(void *), PortThreadStep step, const char *name) {
    int i;

    if (sNumServices >= MAX_SERVICES) {
        host_fatal("ultra: too many thread services");
    }
    sServices[sNumServices].entry = entry;
    sServices[sNumServices].step = step;
    sServices[sNumServices].name = name;
    sNumServices++;
    /* a thread that is already running picks the service up */
    for (i = 0; i < MAX_THREADS; i++) {
        if (sThreads[i].t != NULL && sThreads[i].entry == entry) {
            sThreads[i].step = step;
            sThreads[i].name = name;
        }
    }
}

void osCreateThread(OSThread *t, OSId id, void (*entry)(void *), void *arg, void *sp, OSPri pri) {
    ThreadRec *r = find_rec(t, 1);
    const ServiceRec *s = find_service(entry);

    active_unlink(t);
    memset(t, 0, sizeof(*t));
    active_link(t);
    t->priority = pri;
    t->state = OS_STATE_STOPPED;
    t->id = id;
    t->context.pc = (u32)(size_t)entry;
    t->context.a0 = (u32)(size_t)arg;
    t->context.sp = (u32)(size_t)sp;
    t->context.fpcsr = 0x01000800; /* FS | EV, round to nearest (plan 9.5) */
    r->entry = entry;
    r->arg = arg;
    r->started = 0;
    r->step = s != NULL ? s->step : NULL;
    r->name = s != NULL ? s->name : NULL;
}

void osStartThread(OSThread *t) {
    ThreadRec *r = find_rec(t, 0);

    if (r == NULL) {
        port_ultra_log("ultra: osStartThread on a thread that was never created (%p)", (void *)t);
        return;
    }
    t->state = OS_STATE_RUNNABLE;
    if (r->started) {
        return;
    }
    r->started = 1;
    if (r->step == NULL) {
        port_ultra_st.parkedThreads++;
        port_ultra_log("ultra: thread id %d pri %d entry %p parked (no service; the main loop drives it)",
                       (int)t->id, (int)t->priority, (void *)(size_t)r->entry);
    }
}

void osStopThread(OSThread *t) {
    ThreadRec *r;

    if (t == NULL || t == &sHostThread) {
        port_ultra_log("ultra: osStopThread on the simulation thread ignored");
        return;
    }
    r = find_rec(t, 0);
    t->state = OS_STATE_STOPPED;
    if (r != NULL) {
        r->started = 0;
    }
}

void osDestroyThread(OSThread *t) {
    ThreadRec *r;

    if (t == NULL || t == &sHostThread) {
        port_ultra_log("ultra: osDestroyThread on the simulation thread ignored");
        return;
    }
    r = find_rec(t, 0);
    t->state = OS_STATE_STOPPED;
    active_unlink(t);
    if (r != NULL) {
        memset(r, 0, sizeof(*r));
    }
}

void osSetThreadPri(OSThread *t, OSPri pri) {
    (t != NULL ? t : &sHostThread)->priority = pri;
}

OSPri osGetThreadPri(OSThread *t) {
    return (t != NULL ? t : &sHostThread)->priority;
}

OSId osGetThreadId(OSThread *t) {
    return (t != NULL ? t : &sHostThread)->id;
}

/* No thread ever faults on the host (dbg_faultThread asks). */
OSThread *__osGetCurrFaultedThread(void) {
    return NULL;
}

/* Run each started service once, highest priority first. */
int port_ultra_pump(void) {
    int progress = 0;
    int done[MAX_THREADS];
    int n, i;

    if (sPumpDepth > 0) {
        return 0; /* a service blocked inside its own step: only VI time can help */
    }
    sPumpDepth++;
    port_ultra_st.pumps++;
    memset(done, 0, sizeof(done));
    for (n = 0; n < MAX_THREADS; n++) {
        int best = -1;
        for (i = 0; i < MAX_THREADS; i++) {
            ThreadRec *r = &sThreads[i];
            if (done[i] || r->t == NULL || !r->started || r->step == NULL) {
                continue;
            }
            if (best < 0 || r->t->priority > sThreads[best].t->priority) {
                best = i;
            }
        }
        if (best < 0) {
            break;
        }
        done[best] = 1;
        sThreads[best].t->state = OS_STATE_RUNNING;
        if (sThreads[best].step(sThreads[best].arg)) {
            progress = 1;
        }
        if (sThreads[best].t != NULL) {
            sThreads[best].t->state = OS_STATE_RUNNABLE;
        }
    }
    sPumpDepth--;
    return progress;
}

int port_ultra_wait(int (*ready)(void *ctx), void *ctx, const char *what, const void *obj) {
    u32 idle = 0;
    u32 busy = 0;

    while (!ready(ctx)) {
        if (busy < MAX_BUSY_PUMPS && port_ultra_pump()) {
            busy++;
            continue;
        }
        if (idle >= port_ultra_cfg.maxIdleRetraces) {
            port_ultra_st.deadlocks++;
            port_ultra_log("ultra: DEADLOCK: %s %p not satisfied after %u idle retraces "
                           "(no service or event can make progress)",
                           what, obj, (unsigned)idle);
            if (port_ultra_cfg.deadlockFatal) {
                host_fatal("ultra: deadlock is fatal (PortUltraConfig.deadlockFatal)");
            }
            return -1;
        }
        port_ultra_st.idleRetraces++;
        idle++;
        port_vi_retrace();
    }
    return 0;
}

/* osYieldThread: give the services a turn. A yield loop that spins on a flag
 * nothing will set (gfx_waitRcpIdle before its PORT restructure, for example)
 * shows up as yields with no activity at all; those advance VI time and end in
 * a deadlock report instead of hanging. */
void osYieldThread(void) {
    if (port_ultra_pump() || port_ultra_activity != sYieldActivity) {
        sYieldActivity = port_ultra_activity;
        sYieldIdle = 0;
        return;
    }
    if (++sYieldIdle > port_ultra_cfg.maxIdleRetraces) {
        port_ultra_st.deadlocks++;
        port_ultra_log("ultra: DEADLOCK: osYieldThread spun %u times with no activity",
                       (unsigned)sYieldIdle);
        if (port_ultra_cfg.deadlockFatal) {
            host_fatal("ultra: deadlock is fatal (PortUltraConfig.deadlockFatal)");
        }
        sYieldIdle = 0;
        return;
    }
    port_ultra_st.idleRetraces++;
    port_vi_retrace();
}

void port_ultra_threads_reset(void) {
    memset(sThreads, 0, sizeof(sThreads));
    memset(&sHostThread, 0, sizeof(sHostThread));
    memset(&sThreadTail, 0, sizeof(sThreadTail));
    sThreadTail.priority = -1;
    __osActiveQueue = &sThreadTail;
    active_link(&sHostThread);
    sHostThread.priority = 10; /* nominal: nothing in the game asks for the main thread's priority */
    sHostThread.id = 3;
    sHostThread.state = OS_STATE_RUNNING;
    sPumpDepth = 0;
    sYieldIdle = 0;
}

PORT_ALIAS(osCreateThread, gl_osCreateThread);
PORT_ALIAS(osStartThread, gl_osStartThread);
PORT_ALIAS(osStopThread, gl_osStopThread);
PORT_ALIAS(osDestroyThread, gl_osDestroyThread);
PORT_ALIAS(osYieldThread, gl_osYieldThread);
PORT_ALIAS(osSetThreadPri, gl_osSetThreadPri);
PORT_ALIAS(osGetThreadPri, gl_osGetThreadPri);
PORT_ALIAS(__osGetCurrFaultedThread, gl___osGetCurrFaultedThread);
PORT_ALIAS(__osActiveQueue, gl___osActiveQueue_34458_head);
