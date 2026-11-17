#ifndef PORT_ULTRA_H
#define PORT_ULTRA_H

/* port/ultra: the host libultra shim (PC port plan, decision 6, section 13).
 *
 * The kernel and the game_libs copies of libultra are not compiled for the
 * port. This shim provides the subset the game calls, with the game's own
 * struct layouts (the include/libultra headers, so the -m32 -malign-double build keeps
 * every offset the game reads directly, e.g. OSMesgQueue.validCount).
 *
 * Model: ONE host thread runs the simulation (decision 5). N64 threads are
 * recorded but not run; the main loop (port/main.c) drives everything. A
 * blocking osRecvMesg / osSendMesg / osYieldThread calls the pump
 * (port_ultra_pump), which runs registered thread services and, if nothing
 * makes progress, advances virtual VI time one retrace at a time. A wait that
 * nothing can satisfy is reported as a deadlock instead of hanging.
 *
 * Time is virtual and deterministic: osGetCount / osGetTime derive from the
 * retrace count (2 retraces per 30 Hz step, 781250 CPU-counter cycles per
 * retrace), never from the host clock (plan 9.3 / 9.6). */

#include <stdarg.h>
#include "common.h"
#include "libultra/os_message.h"
#include "libultra/os_event.h"
#include "libultra/os_pi.h"
#include "libultra/os_vi.h"
#include "libultra/os_cont.h"
#include "libultra/os_timer.h"
#include "libultra/os_task.h"

/* ---- libultra constants the game headers do not define (2.0I PR/os.h) ---- */
#ifndef OS_EVENT_SW1
#define OS_EVENT_SW1 0
#define OS_EVENT_SW2 1
#define OS_EVENT_CART 2
#define OS_EVENT_COUNTER 3
#define OS_EVENT_SP 4
#define OS_EVENT_SI 5
#define OS_EVENT_AI 6
#define OS_EVENT_VI 7
/* OS_EVENT_PI 8 is in libultra/os_event.h */
#define OS_EVENT_DP 9
#define OS_EVENT_CPU_BREAK 10
#define OS_EVENT_SP_BREAK 11
#define OS_EVENT_FAULT 12
#define OS_EVENT_THREADSTATUS 13
#define OS_EVENT_PRENMI 14
#endif
#define OS_NUM_EVENTS 15

#define OS_TV_PAL 0
#define OS_TV_NTSC 1
#define OS_TV_MPAL 2

#define MAXCONTROLLERS 4
#define CONT_TYPE_NORMAL 0x0005
#define CONT_CARD_ON 0x01
#define CONT_NO_RESPONSE_ERROR 0x8

#define PFS_ERR_NOPACK 1
#define PFS_ERR_CONTRFAIL 4

#define OS_CPU_COUNTER 46875000u        /* osGetCount rate (CPU clock / 2) */
#define PORT_CYCLES_PER_RETRACE 781250u /* OS_CPU_COUNTER / 60: 2 per 30 Hz step = 1562500 */
#define PORT_VI_CLOCK_NTSC 48681812     /* osViClock */

#define PORT_SRAM_SIZE 0x8000 /* 1080's save (fs_gdl* kind 2), 256 Kbit */

typedef u32 OSIntMask;
typedef u32 OSHWIntr;

/* ---- libultra API provided by the shim (canonical names) ---- */
/* threads */
void osCreateThread(OSThread *t, OSId id, void (*entry)(void *), void *arg, void *sp, OSPri pri);
void osStartThread(OSThread *t);
void osStopThread(OSThread *t);
void osDestroyThread(OSThread *t);
void osYieldThread(void);
void osSetThreadPri(OSThread *t, OSPri pri);
OSPri osGetThreadPri(OSThread *t);
OSId osGetThreadId(OSThread *t);
OSThread *__osGetCurrFaultedThread(void);
/* messages / events */
void osCreateMesgQueue(OSMesgQueue *mq, OSMesg *msg, s32 count);
s32 osSendMesg(OSMesgQueue *mq, OSMesg msg, s32 flag);
s32 osJamMesg(OSMesgQueue *mq, OSMesg msg, s32 flag);
s32 osRecvMesg(OSMesgQueue *mq, OSMesg *msg, s32 flag);
void osSetEventMesg(OSEvent e, OSMesgQueue *mq, OSMesg msg);
OSIntMask osSetIntMask(OSIntMask mask);
/* VI */
void osCreateViManager(OSPri pri);
void osViSetMode(OSViMode *mode);
void osViSetEvent(OSMesgQueue *mq, OSMesg msg, u32 retraceCount);
void osViSwapBuffer(void *frameBuf);
void osViBlack(u8 active);
void osViSetSpecialFeatures(u32 func);
void osViSetXScale(f32 value);
void osViSetYScale(f32 value);
void *osViGetCurrentFramebuffer(void);
void *osViGetNextFramebuffer(void);
u32 osViGetCurrentLine(void);
u32 osViGetCurrentField(void);
/* PI */
void osCreatePiManager(OSPri pri, OSMesgQueue *cmdQ, OSMesg *cmdBuf, s32 cmdMsgCnt);
OSMesgQueue *osPiGetCmdQueue(void);
s32 osPiStartDma(OSIoMesg *mb, s32 pri, s32 direction, u32 devAddr, void *dramAddr, u32 size, OSMesgQueue *mq);
s32 osEPiStartDma(OSPiHandle *h, OSIoMesg *mb, s32 direction);
s32 osPiRawStartDma(s32 direction, u32 devAddr, void *dramAddr, u32 size);
s32 osEPiRawStartDma(OSPiHandle *h, s32 direction, u32 devAddr, void *dramAddr, u32 size);
s32 osPiReadIo(u32 devAddr, u32 *data);
s32 osPiWriteIo(u32 devAddr, u32 data);
s32 osPiRawReadIo(u32 devAddr, u32 *data);
s32 osEPiReadIo(OSPiHandle *h, u32 devAddr, u32 *data);
s32 osEPiWriteIo(OSPiHandle *h, u32 devAddr, u32 data);
s32 osPiGetStatus(void);
OSPiHandle *osCartRomInit(void);
OSPiHandle *osDriveRomInit(void);
s32 osEPiLinkHandle(OSPiHandle *h);
/* SI: controllers, paks, rumble */
s32 osContInit(OSMesgQueue *mq, u8 *bitpattern, OSContStatus *status);
s32 osContStartReadData(OSMesgQueue *mq);
void osContGetReadData(OSContPad *pads);
s32 osContStartQuery(OSMesgQueue *mq);
void osContGetQuery(OSContStatus *status);
s32 osPfsIsPlug(OSMesgQueue *mq, u8 *pattern);
s32 osPfsInitPak(OSMesgQueue *mq, OSPfs *pfs, s32 channel);
s32 osMotorInit(OSMesgQueue *mq, OSPfs *pfs, s32 channel);
s32 osMotorStart(OSPfs *pfs);
s32 osMotorStop(OSPfs *pfs);
/* AI */
s32 osAiSetFrequency(u32 frequency);
s32 osAiSetNextBuffer(void *buf, u32 size);
u32 osAiGetLength(void);
u32 osAiGetStatus(void);
/* SP / RCP tasks */
void osSpTaskLoad(OSTask_t *task);
void osSpTaskStartGo(OSTask_t *task);
void osSpTaskYield(void);
u32 osSpTaskYielded(OSTask_t *task);
/* caches, addresses */
void osWritebackDCache(void *p, s32 size);
void osWritebackDCacheAll(void);
void osInvalDCache(void *p, s32 size);
void osInvalICache(void *p, s32 size);
u32 osVirtualToPhysical(void *p);
void *osPhysicalToVirtual(u32 addr);
/* time */
u32 osGetCount(void);
OSTime osGetTime(void);
void osSetTime(OSTime t);
s32 osSetTimer(OSTimer *t, OSTime countdown, OSTime interval, OSMesgQueue *mq, OSMesg msg);
s32 osStopTimer(OSTimer *t);
/* boot / misc */
void osInitialize(void);
void osSyncPrintf(const char *fmt, ...);
#ifdef PORT64 /* the game's IDO-layout cursor (plan 21.5) */
int _Printf(void *(*prout)(void *, const char *, u32), void *arg, const char *fmt, const char *cursor);
#else
int _Printf(void *(*prout)(void *, const char *, u32), void *arg, const char *fmt, va_list ap);
#endif

/* boot globals (kernel low memory on the N64: osTvType = 0x80000300 etc.) */
extern s32 osTvType;
extern s32 osRomType;
extern u32 osRomBase;
extern s32 osResetType;
extern s32 osCicId;
extern u32 osMemSize;
extern u8 osAppNMIBuffer[64];
extern u64 osClockRate;
extern s32 osViClock;

/* ---- port-side API (main.c and the PORT glue in game code) ---- */
typedef struct PortUltraConfig {
    const u8 *rom;          /* big-endian .z64 image; PI reads from it */
    u32 romSize;
    const char *sramPath;   /* SRAM save file (PORT_SRAM_SIZE bytes); NULL = in memory only */
    int deadlockFatal;      /* 1: a wait nothing can satisfy aborts; 0: it returns -1 */
    u32 maxIdleRetraces;    /* idle retraces one blocking wait may advance (default 600) */
    void (*rcpTask)(OSTask_t *task); /* osSpTaskStartGo target (Fast3D / CPU audio); NULL = drop */
} PortUltraConfig;

typedef struct PortUltraStats {
    u32 retraces;          /* virtual VI retraces since boot */
    u32 idleRetraces;      /* of which advanced by a blocked wait, not by the main loop */
    u32 pumps;
    u32 deadlocks;
    u32 parkedThreads;     /* threads started with no registered service */
    u32 piBytes;
    u32 sramWrites;
    u32 spTasks;
} PortUltraStats;

void port_ultra_init(const PortUltraConfig *cfg);
void port_ultra_shutdown(void);
const PortUltraStats *port_ultra_stats(void);

/* One virtual VI retrace: posts the osViSetEvent message every N retraces,
 * latches the swapped frame buffer, advances osGetCount, fires timers. The
 * main loop calls it twice per 30 Hz step. */
void port_vi_retrace(void);
u32 port_vi_retrace_count(void);

/* Post a libultra event (osSetEventMesg target), as the interrupt would. */
void port_ultra_post_event(OSEvent e);

/* Thread services. An N64 thread whose loop the port replaces can register a
 * step function for its entry point: when the game starts that thread, the
 * pump calls step(arg) at every blocking point. step returns nonzero if it did
 * any work. Threads with no service are parked (never run) and logged. */
typedef int (*PortThreadStep)(void *arg);
void port_ultra_register_service(void (*entry)(void *), PortThreadStep step, const char *name);

/* Run the services once; returns nonzero if anything made progress. */
int port_ultra_pump(void);

/* Controller state the SI reports (osContStartReadData snapshots it). */
void port_si_set_pads(const OSContPad pads[MAXCONTROLLERS], const u8 connected[MAXCONTROLLERS],
                      const u8 hasRumble[MAXCONTROLLERS]);

/* SRAM image access (save file tooling, tests) */
u8 *port_sram_image(void);
int port_sram_flush(void);

/* shared log helper */
void port_ultra_log(const char *fmt, ...) __attribute__((format(printf, 1, 2)));

#endif /* PORT_ULTRA_H */
