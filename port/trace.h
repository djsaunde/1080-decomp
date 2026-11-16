#ifndef PORT_TRACE_H
#define PORT_TRACE_H

/* Trace logger: per-frame input record / injection and a per-frame state dump
 * (PC port plan 9.6.4). The same format is produced by the emulator-side
 * logger, so a port run and an emulator run of the same input can be diffed
 * line by line.
 *
 * Game-side hook points (added under #ifdef PORT in the game code, M1
 * integration; until then main.c calls them from its stand-in frame):
 *   - port_trace_pad():       in pad_update, right after
 *                             pad_decodeButtonMap_00034CC8, once per pad per
 *                             update. Logs (record) or overwrites (replay) the
 *                             full held word and both raw stick bytes.
 *   - port_trace_frame_end(): at the end of app_runOneFrame (after
 *                             SysLib.frameCount++ and the one-shot callback).
 *
 * File format (text, one record per line, '#' comments):
 *   I <frame> <pad> <replayFrame> <held:hex4> <rawX> <rawY>
 *   F <frame> have=<hex> flow=<state>,<next> race=<state>,<flags:hex>
 *     rand=<hex8> rt=<retraceCount> vi=<viRetraces> el=<elapsed>
 *     cam=<6 x hex8> r<i>=<px,py,pz,qx,qy,qz,qw,speed as hex8>/<anim>
 * Floats are written as their IEEE bit patterns (hex8): diffs are exact.
 * Fields whose `have` bit is clear were not collected and are written as 0. */

#include "common.h"

#define PORT_TRACE_HAVE_FLOW 0x01    /* GameFlow state (InputMgr +0x40), next mode (+0x34) */
#define PORT_TRACE_HAVE_RACE 0x02    /* RaceProcess raceState / flags */
#define PORT_TRACE_HAVE_RIDERS 0x04  /* rider dnode pos / quat, speed, RiderAnim state */
#define PORT_TRACE_HAVE_CAMERA 0x08  /* camera eye / target */
#define PORT_TRACE_HAVE_ELAPSED 0x10 /* DataManager elapsed (ms) */
#define PORT_TRACE_HAVE_RAND 0x20    /* sys_randf state (bootup Data 0x21E04) */
#define PORT_TRACE_HAVE_RETRACE 0x40 /* RcpMgr retraceCount (virtual) */

#define PORT_TRACE_MAX_RIDERS 4

typedef struct PortTraceRider {
    f32 pos[3];
    f32 quat[4];
    f32 speed;
    s32 animState;
} PortTraceRider;

typedef struct PortTraceFrame {
    u32 frameCount; /* SysLib.frameCount (main.c's step count until the game is linked) */
    u32 have;       /* PORT_TRACE_HAVE_* */
    s32 flowState;
    s32 flowNext;
    s32 raceState;
    u32 raceFlags;
    s32 numRiders;
    PortTraceRider riders[PORT_TRACE_MAX_RIDERS];
    f32 camEye[3];
    f32 camTarget[3];
    s32 elapsed;
    u32 randState;
    u32 retraceCount;
    u32 viRetraces; /* the shim's virtual VI count, always filled */
} PortTraceFrame;

/* Open the logs. Any path may be NULL. record: input log written;
 * replay: input log read and injected; state: per-frame state dump. */
int port_trace_open(const char *recordPath, const char *replayPath, const char *statePath);
void port_trace_close(void);
int port_trace_replaying(void);
/* replay: the last frame that has recorded input (main.c stops after it) */
u32 port_trace_replay_last_frame(void);

/* game hooks */
void port_trace_pad(int pad, u32 frameCount, s32 replayFrame, u16 *held, s8 *rawX, s8 *rawY);
void port_trace_frame_end(u32 frameCount);

/* State collector. trace.c has a weak default that reads what is linkable
 * (gInputMgr, the sys_randf state); the PORT game glue overrides it with one
 * that knows the race, rider and camera objects. */
void port_trace_collect(PortTraceFrame *f);

#endif /* PORT_TRACE_H */
