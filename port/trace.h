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
 *   I <frame> <pad> <replayFrame> <held:hex8> <rawX> <rawY>
 *   F <frame> have=<hex> flow=<state>,<next>,<mode> race=<state>,<flags:hex>
 *     rand=<hex8> rt=<retraceCount> vi=<viRetraces> el=<elapsed>
 *     cam=<6 x hex8> r<i>=<px,py,pz,qx,qy,qz,qw,speed as hex8>/<anim>
 *     [e0=<pct:hex8>,<speed:hex8>,<finished>,<health>,<score>,<pending>]
 *     [e<i>=<pct:hex8>,<finished>,<health>,<score>]  (i = 1 .. numEntries - 1, at most 3)
 *     [tk=<trickButtons:hex>,<trickInputFrames>,<queuedTrick>,<curTrick>]  (TrickTable indices, -1 none)
 *     [cd=<dm score>,<gates>,<gateX:hex8>,<gateZ:hex8>,<forkX:hex8>,<forkZ:hex8>,<forkFlags>]
 *     (DataManager +0xA0: the Trick Attack / Contest countdown in ms, Time Up below 0; gates =
 *     entries[0]'s "chk" gates still ahead, the next one's x / z; fork = entries[0].gateA, the next
 *     "gtl" / "gtr" slalom gate: cleared on its left when forkFlags & 1, else on its right
 *     (RaceTracker_passGates); 0 when none: include/game/course.h RaceGate)
 *     [trn=<lesson>]  (Training, flow mode 7: TrnDataManager.lesson, the current lesson's TrickTable index)
 *     [scr=<hash:hex8>]  (only with PORT_TRACE_SCREEN=1: the step's gfx tasks' display-list content hash,
 *     pointer-free (dlump_screen_hash), 0 when no task ran; soak.py's screen-level stuck detector)
 *   e0 is the DataManager's RaceEntry 0 (the player, include/gdl/race.h): pct reaches 100.0 at the
 *   finish line ("Goal!") and finished counts the frames there; speed is what the HUD's km/h
 *   gadget prints ((int)speed, RaceTracker_update). Written only when its have bit is set.
 *   score is the flushed points (tricks, gates), pending the running trick's points (both 0 in a
 *   race mode). e1.. are the other entries (Match Race CPU riders, the ghost, 2P VS player 2).
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
#define PORT_TRACE_HAVE_ENTRY 0x80   /* DataManager entries[0]: pct, speed, finished, health */
#define PORT_TRACE_HAVE_TRICK 0x100  /* rider 0's RiderAnim trick input / trick (tk=) */
#define PORT_TRACE_HAVE_LESSON 0x400 /* Training: TrnDataManager.lesson (trn=) */
#define PORT_TRACE_HAVE_GATES 0x200  /* DataManager score / countdown and entries[0]'s next gate (cd=) */

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
    s32 flowMode;   /* GameFlow.flowMode (+0x38): the running flow mode (9 = none) */
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
    f32 entryPct;   /* entries[0].pct (0..100) */
    f32 entrySpeed; /* entries[0].speed: the HUD km/h value */
    s32 entryFinished;
    s32 entryHealth;
    s32 entryScore;   /* entries[0].score (the flushed trick / gate points) */
    s32 entryPending; /* entries[0].pendingPoints (the running trick's points, not yet flushed) */
    s32 numEntries;   /* DataManager.numEntries: e1.. are written for entries 1..numEntries-1 */
    struct {
        f32 pct;
        s32 finished;
        s32 health;
        s32 score;
    } entry[PORT_TRACE_MAX_RIDERS]; /* entries[i] (i >= 1): the CPU / ghost / 2P rider */
    /* rider 0's RiderAnim trick fields (PORT_TRACE_HAVE_TRICK): trickButtons, trickInputFrames,
     * queuedTrick / curTrick as TrickTable indices (-1 = none) */
    u32 trickButtons;
    s32 trickFrames;
    s32 trickQueued;
    s32 trickCur;
    /* PORT_TRACE_HAVE_GATES: DataManager.score (+0xA0; the trk / arc countdown, ms), the number of
     * "chk" gates left on entries[0].gate's list, the next gate's x / z */
    s32 dmScore;
    s32 gatesLeft;
    f32 gateX;
    f32 gateZ;
    f32 forkX;
    f32 forkZ;
    s32 forkFlags;
    s32 lesson;     /* PORT_TRACE_HAVE_LESSON: TrnDataManager.lesson (game/trnproc.h) */
} PortTraceFrame;

/* Open the logs. Any path may be NULL. record: input log written;
 * replay: input log read and injected; state: per-frame state dump. */
int port_trace_open(const char *recordPath, const char *replayPath, const char *statePath);
void port_trace_close(void);
int port_trace_replaying(void);
/* replay: the last frame that has recorded input (main.c stops after it) */
u32 port_trace_replay_last_frame(void);

/* game hooks */
void port_trace_pad(int pad, u32 frameCount, s32 replayFrame, u32 *held, s8 *rawX, s8 *rawY);
void port_trace_frame_end(u32 frameCount);

/* State collector. trace.c has a weak default that reads what is linkable
 * (gInputMgr, the sys_randf state); the PORT game glue overrides it with one
 * that knows the race, rider and camera objects. */
void port_trace_collect(PortTraceFrame *f);

/* PORT_TRACE_FN (cmake -DPORT_TRACE_FN=ON, plan 21.8): the game library is compiled with
 * -finstrument-functions and port/trace.c logs every function entry / exit of one frame
 * (env PORT_TRACE_FN=<frame>, file PORT_TRACE_FN_OUT, default fn.log) with a hash of the object
 * the collector names (rider 0's SnowBoarder; pointer-looking words skipped, so the i386 and the
 * x86-64 logs compare).  port/tools/fn-trace-diff.py maps the addresses to names and reports the
 * first function after which the two runs' objects differ. */
extern void *port_trace_fn_obj;
extern unsigned port_trace_fn_objSize;

#endif /* PORT_TRACE_H */
