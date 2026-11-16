/* port/ultra: VI (video interface) on virtual time.
 *
 * There is no real retrace. port_vi_retrace() is one virtual retrace: the main
 * loop calls it twice per 30 Hz step (plan 9.6.3), and a blocked wait calls it
 * when nothing else can make progress. Each retrace latches the swapped frame
 * buffer, posts the osViSetEvent message every `retraceCount` retraces, and
 * advances osGetCount. osViSwapBuffer hands the frame to the host
 * (host_frame_ready: Fast3D present in M2). */

#include "ultra_int.h"

static OSViMode *sMode;
static void *sCurFb;
static void *sNextFb;
static OSMesgQueue *sEvtQueue;
static OSMesg sEvtMsg;
static u32 sEvtPeriod;
static u32 sEvtCounter;
static u32 sRetraces;
static u32 sFeatures;
static f32 sXScale = 1.0f;
static f32 sYScale = 1.0f;
static u8 sBlack;
static u8 sManagerCreated;

void osCreateViManager(OSPri pri) {
    (void)pri;
    sManagerCreated = 1;
}

void osViSetMode(OSViMode *mode) {
    u32 width;

    sMode = mode;
    if (mode == NULL) {
        return;
    }
    /* the game picks a 320-wide lan1 mode (gfx_setVideoMode 0x202 / osViModeTable + 0xA0) */
    width = mode->comRegs.width;
    host_video_mode(width, width > 320 ? 480 : 240, width > 320);
}

void osViSetEvent(OSMesgQueue *mq, OSMesg msg, u32 retraceCount) {
    sEvtQueue = mq;
    sEvtMsg = msg;
    sEvtPeriod = retraceCount != 0 ? retraceCount : 1;
    sEvtCounter = 0;
}

void osViSwapBuffer(void *frameBuf) {
    sNextFb = frameBuf;
    host_frame_ready(frameBuf);
}

void osViBlack(u8 active) {
    sBlack = active;
    host_video_black(active);
}

void osViSetSpecialFeatures(u32 func) {
    sFeatures = func;
}

void osViSetXScale(f32 value) {
    sXScale = value;
}

void osViSetYScale(f32 value) {
    sYScale = value;
}

void *osViGetCurrentFramebuffer(void) {
    return sCurFb;
}

void *osViGetNextFramebuffer(void) {
    return sNextFb;
}

/* Only the debug perf meter reads the beam position. */
u32 osViGetCurrentLine(void) {
    return 0;
}

u32 osViGetCurrentField(void) {
    return sRetraces & 1;
}

void port_vi_retrace(void) {
    sRetraces++;
    port_ultra_st.retraces = sRetraces;
    sCurFb = sNextFb;
    if (sEvtQueue != NULL && ++sEvtCounter >= sEvtPeriod) {
        sEvtCounter = 0;
        osSendMesg(sEvtQueue, sEvtMsg, OS_MESG_NOBLOCK);
    }
    port_timers_fire();
}

u32 port_vi_retrace_count(void) {
    return sRetraces;
}

void port_vi_reset(void) {
    sMode = NULL;
    sCurFb = sNextFb = NULL;
    sEvtQueue = NULL;
    sEvtMsg = NULL;
    sEvtPeriod = 1;
    sEvtCounter = 0;
    sRetraces = 0;
    sFeatures = 0;
    sXScale = sYScale = 1.0f;
    sBlack = 0;
    sManagerCreated = 0;
}

PORT_ALIAS(osViSetXScale, gl_osViSetXScale);
PORT_ALIAS(osViSetYScale, gl_osViSetYScale);
