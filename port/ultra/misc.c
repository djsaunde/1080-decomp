/* port/ultra: boot, globals, caches, addresses, virtual time, timers, printf,
 * and the shim's init / stats. */

#include "ultra_int.h"

PortUltraConfig port_ultra_cfg;
PortUltraStats port_ultra_st;

/* ---- boot globals (N64 low memory, set by the IPL3 / osInitialize) ---- */
s32 osTvType = OS_TV_NTSC; /* the analysed ROM is the NTSC release (plan 9.1) */
s32 osRomType = 0;          /* cartridge */
u32 osRomBase = 0xB0000000;
s32 osResetType = 0;        /* cold reset */
s32 osCicId = 0x17D7;       /* 6103: the cartridge's CIC (App_softReset / SelectWin_startGame check it) */
u32 osMemSize = 0x400000;   /* no Expansion Pak */
u8 osAppNMIBuffer[64];
u64 osClockRate = 62500000;
s32 osViClock = PORT_VI_CLOCK_NTSC;

/* gfx_setVideoMode / the audio init read osTvType at its fixed address 0x80000300
 * under these names in the matching build. */
PORT_ALIAS(osTvType, gl_24378_osTvType);
PORT_ALIAS(osTvType, D_00000000_tvtype);
/* osCicId (0x80000310) and osResetType (0x8000030C, sys_getResetType) under their game tokens */
PORT_ALIAS(osCicId, import_80000310);
PORT_ALIAS(osResetType, D_35360_X);

static u8 sInitialized;

void osInitialize(void) {
    if (sInitialized) {
        return;
    }
    sInitialized = 1;
    /* the FPU mode (FS=1 flush-to-zero, round to nearest) is set on the host
     * thread by main.c (MXCSR FTZ|DAZ), not here: plan 9.5 */
}

/* ---- caches and addresses: no-ops on the host ---- */
void osWritebackDCache(void *p, s32 size) {
    (void)p;
    (void)size;
}

void osWritebackDCacheAll(void) {
}

void osInvalDCache(void *p, s32 size) {
    (void)p;
    (void)size;
}

void osInvalICache(void *p, s32 size) {
    (void)p;
    (void)size;
}

/* Identity: display lists and DMA addresses stay host pointers, which the
 * Fast3D interpreter and this shim's PI dereference directly (32-bit build). */
u32 osVirtualToPhysical(void *p) {
    return (u32)(size_t)p;
}

void *osPhysicalToVirtual(u32 addr) {
    return (void *)(size_t)addr;
}

/* ---- virtual time ----
 * osGetCount = retraces * 781250 (+ osSetTime offset). The main loop does 2
 * retraces per step, so during step N osGetCount is N * 1562500 (plan 9.3:
 * this makes AudioThread_Update's audioRandom reproducible). Nothing advances
 * within a step: there are no spin-waits on the clock in game code (plan 9.2). */
static OSTime sTimeBase;
static OSTimer *sTimers;

OSTime port_time_now(void) {
    return (OSTime)port_vi_retrace_count() * PORT_CYCLES_PER_RETRACE + sTimeBase;
}

u32 osGetCount(void) {
    return (u32)port_time_now();
}

OSTime osGetTime(void) {
    return port_time_now();
}

void osSetTime(OSTime t) {
    sTimeBase = t - (OSTime)port_vi_retrace_count() * PORT_CYCLES_PER_RETRACE;
}

/* Timers fire at retrace granularity (the only time base there is). OSTimer.value
 * holds the absolute expiry time; the list uses the game's own next / prev. */
s32 osSetTimer(OSTimer *t, OSTime countdown, OSTime interval, OSMesgQueue *mq, OSMesg msg) {
    osStopTimer(t);
    t->interval = interval;
    t->value = port_time_now() + (countdown != 0 ? countdown : interval);
    t->mq = mq;
    t->msg = msg;
    t->prev = NULL;
    t->next = sTimers;
    if (sTimers != NULL) {
        sTimers->prev = t;
    }
    sTimers = t;
    return 0;
}

s32 osStopTimer(OSTimer *t) {
    OSTimer *p;

    for (p = sTimers; p != NULL; p = p->next) {
        if (p == t) {
            if (t->prev != NULL) {
                t->prev->next = t->next;
            } else {
                sTimers = t->next;
            }
            if (t->next != NULL) {
                t->next->prev = t->prev;
            }
            t->next = t->prev = NULL;
            return 0;
        }
    }
    return -1;
}

void port_timers_fire(void) {
    OSTime now = port_time_now();
    OSTimer *t = sTimers;

    while (t != NULL) {
        OSTimer *next = t->next;
        if (t->value <= now) {
            if (t->mq != NULL) {
                osSendMesg(t->mq, t->msg, OS_MESG_NOBLOCK);
            }
            if (t->interval != 0) {
                t->value += t->interval;
                if (t->value <= now) {
                    t->value = now + t->interval; /* never fire twice in one retrace */
                }
            } else {
                osStopTimer(t);
            }
        }
        t = next;
    }
}

void port_time_reset(void) {
    sTimeBase = 0;
    sTimers = NULL;
}

/* ---- printf family ----
 * libultra's _Printf (game_libs_ido53_6E224.c) uses IDO's va_arg and cannot be
 * compiled by gcc; the host vsnprintf formats instead. prout(arg, str, len)
 * returns the next arg, NULL = stop (libultra contract). */
int _Printf(void *(*prout)(void *, const char *, u32), void *arg, const char *fmt, va_list ap) {
    char buf[2048];
    int n = vsnprintf(buf, sizeof(buf), fmt, ap);

    if (n < 0) {
        return -1;
    }
    if ((u32)n >= sizeof(buf)) {
        n = sizeof(buf) - 1;
    }
    if (n > 0 && prout(arg, buf, (u32)n) == NULL) {
        return -1;
    }
    return n;
}

void osSyncPrintf(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    host_vlog(fmt, ap);
    va_end(ap);
}

/* game_libs' libc copies with gl_ names */
int gl_sprintf(char *dst, const char *fmt, ...) {
    va_list ap;
    int n;

    va_start(ap, fmt);
    n = vsprintf(dst, fmt, ap);
    va_end(ap);
    return n;
}

void gl_bcopy(const void *src, void *dst, s32 len) {
    memmove(dst, src, (size_t)len);
}

void gl_bzero(void *p, s32 len) {
    memset(p, 0, (size_t)len);
}

PORT_ALIAS(osWritebackDCache, gl_osWritebackDCache);
PORT_ALIAS(osWritebackDCacheAll, gl_osWritebackDCacheAll);
PORT_ALIAS(osInvalDCache, gl_osInvalDCache);
PORT_ALIAS(osInvalICache, gl_osInvalICache);
PORT_ALIAS(osVirtualToPhysical, gl_osVirtualToPhysical);
PORT_ALIAS(osGetCount, gl_osGetCount);
PORT_ALIAS(osGetCount, gl_26214_osGetCount);
PORT_ALIAS(osSetTimer, gl_osSetTimer);
PORT_ALIAS(osInitialize, gl_osInitialize);
PORT_ALIAS(osSyncPrintf, gl_osSyncPrintf);
PORT_ALIAS(osSyncPrintf, gl_rmonPrintf);
/* the kernel-named bzero (boot code) is host libc's bzero: same contract */

/* ---- shim lifecycle ---- */
void port_ultra_log(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    host_vlog(fmt, ap);
    va_end(ap);
}

void port_ultra_init(const PortUltraConfig *cfg) {
    port_ultra_cfg = *cfg;
    if (port_ultra_cfg.maxIdleRetraces == 0) {
        port_ultra_cfg.maxIdleRetraces = 600; /* 10 s of virtual VI time */
    }
    memset(&port_ultra_st, 0, sizeof(port_ultra_st));
    memset(osAppNMIBuffer, 0, sizeof(osAppNMIBuffer));
    sInitialized = 0;
    port_ultra_threads_reset();
    port_ultra_events_reset();
    port_vi_reset();
    port_time_reset();
    port_pi_reset();
    port_si_reset();
}

void port_ultra_shutdown(void) {
    port_sram_flush();
}

const PortUltraStats *port_ultra_stats(void) {
    return &port_ultra_st;
}
