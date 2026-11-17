/* Trace logger (port/trace.h). */

#include "port_libc.h"
#include "host.h"
#include "trace.h"
#include "ultra/ultra.h"

#define MAX_PADS 4

typedef struct ReplayInput {
    u32 frame;
    s32 replayFrame;
    u32 held; /* the decoded PadCtl.held word (bits up to 0x40000) */
    s8 rawX;
    s8 rawY;
    u8 pad;
} ReplayInput;

static FILE *sRecord;
static FILE *sState;
static ReplayInput *sReplay;
static u32 sReplayCount;
static u32 sReplayPos;
static u32 sReplayLastFrame;
static u32 sReplayMisses;
static u32 sReplayFrameSkew;

static u32 fbits(f32 f) {
    union {
        f32 f;
        u32 u;
    } u;
    u.f = f;
    return u.u;
}

static int load_replay(const char *path) {
    char *text;
    u32 size, cap = 0, i;
    char *line;

    if (host_file_read_all(path, (void **)&text, &size) != 0) {
        host_log("trace: cannot read replay %s", path);
        return -1;
    }
    for (i = 0; i < size; i++) {
        if (text[i] == '\n') {
            cap++;
        }
    }
    sReplay = (ReplayInput *)calloc(cap + 1, sizeof(ReplayInput));
    sReplayCount = 0;
    line = text;
    while (line < text + size) {
        char *end = line;
        while (end < text + size && *end != '\n') {
            end++;
        }
        if (end < text + size) {
            *end = '\0';
        } else {
            /* last line without a newline: the buffer has no room for a NUL; skip it */
            break;
        }
        if (line[0] == 'I' && line[1] == ' ') {
            unsigned long fr, pad, held;
            long rf, rx, ry;
            char *p = line + 2;
            fr = strtoul(p, &p, 10);
            pad = strtoul(p, &p, 10);
            rf = strtol(p, &p, 10);
            held = strtoul(p, &p, 16);
            rx = strtol(p, &p, 10);
            ry = strtol(p, &p, 10);
            if (pad < MAX_PADS && sReplayCount <= cap) {
                ReplayInput *r = &sReplay[sReplayCount++];
                r->frame = (u32)fr;
                r->pad = (u8)pad;
                r->replayFrame = (s32)rf;
                r->held = (u32)held;
                r->rawX = (s8)rx;
                r->rawY = (s8)ry;
                if ((u32)fr > sReplayLastFrame) {
                    sReplayLastFrame = (u32)fr;
                }
            }
        }
        line = end + 1;
    }
    host_free(text);
    host_log("trace: replay %s: %u input records, last frame %u", path, (unsigned)sReplayCount,
             (unsigned)sReplayLastFrame);
    return 0;
}

int port_trace_open(const char *recordPath, const char *replayPath, const char *statePath) {
    if (recordPath != NULL) {
        sRecord = fopen(recordPath, "w");
        if (sRecord == NULL) {
            host_log("trace: cannot write %s", recordPath);
            return -1;
        }
        fprintf(sRecord, "# 1080 port input log v2: I frame pad replayFrame held:hex8 rawX rawY\n");
    }
    if (statePath != NULL) {
        sState = fopen(statePath, "w");
        if (sState == NULL) {
            host_log("trace: cannot write %s", statePath);
            return -1;
        }
        setvbuf(sState, NULL, _IOLBF, 0); /* a crash keeps every frame's line */
        fprintf(sState, "# 1080 port state trace v1 (port/trace.h)\n");
    }
    if (replayPath != NULL && load_replay(replayPath) != 0) {
        return -1;
    }
    return 0;
}

void port_trace_close(void) {
    if (sReplay != NULL) {
        host_log("trace: replay done: %u missing inputs, %u replayFrame mismatches",
                 (unsigned)sReplayMisses, (unsigned)sReplayFrameSkew);
        free(sReplay);
        sReplay = NULL;
    }
    if (sRecord != NULL) {
        fclose(sRecord);
        sRecord = NULL;
    }
    if (sState != NULL) {
        fclose(sState);
        sState = NULL;
    }
}

int port_trace_replaying(void) {
    return sReplay != NULL;
}

u32 port_trace_replay_last_frame(void) {
    return sReplayLastFrame;
}

/* Records are in log order, so a forward cursor finds each one in O(1); fall
 * back to a scan when the game asks out of order (a pad skipped while paused). */
static ReplayInput *find_replay(u32 frame, int pad) {
    u32 i;

    for (i = sReplayPos; i < sReplayCount; i++) {
        if (sReplay[i].frame == frame && sReplay[i].pad == pad) {
            sReplayPos = i + 1;
            return &sReplay[i];
        }
        if (sReplay[i].frame > frame) {
            break;
        }
    }
    for (i = 0; i < sReplayCount; i++) {
        if (sReplay[i].frame == frame && sReplay[i].pad == pad) {
            return &sReplay[i];
        }
    }
    return NULL;
}

void port_trace_pad(int pad, u32 frameCount, s32 replayFrame, u32 *held, s8 *rawX, s8 *rawY) {
    if (sReplay != NULL) {
        ReplayInput *r = find_replay(frameCount, pad);
        if (r == NULL) {
            sReplayMisses++;
            *held = 0;
            *rawX = 0;
            *rawY = 0;
        } else {
            if (r->replayFrame != replayFrame) {
                sReplayFrameSkew++;
            }
            *held = r->held;
            *rawX = r->rawX;
            *rawY = r->rawY;
        }
    }
    if (sRecord != NULL) {
        fprintf(sRecord, "I %u %d %d %08x %d %d\n", (unsigned)frameCount, pad, (int)replayFrame,
                (unsigned)*held, (int)*rawX, (int)*rawY);
    }
}

/* ---- default collector: whatever is linkable now ---- */
/* The InputMgr / GameFlow object is bootup Bss + 0xF8 (port/include/port_game.h PORT_GAMEFLOW).
 * The game spells it gInputMgr = D_00000000 (a placeholder bound per site), so there is no
 * gInputMgr symbol to link against: read it through the bootup Bss blob (port/uso), weak so
 * the shim tests link without the game. */
extern char port_bootup_bss[] __attribute__((weak));
#define gInputMgr (port_bootup_bss != NULL ? port_bootup_bss + 0xF8 : NULL)
extern int gl_ref_00021E04 __attribute__((weak)); /* sys_randf state, bootup Data 0x21E04 */

__attribute__((weak)) void port_trace_collect(PortTraceFrame *f) {
    if (gInputMgr != NULL) {
        f->flowState = *(s32 *)(gInputMgr + 0x40);
        f->flowNext = *(s32 *)(gInputMgr + 0x34);
        f->flowMode = *(s32 *)(gInputMgr + 0x38);
        f->have |= PORT_TRACE_HAVE_FLOW;
    }
    if (&gl_ref_00021E04 != NULL) {
        f->randState = (u32)gl_ref_00021E04;
        f->have |= PORT_TRACE_HAVE_RAND;
    }
    /* RaceProcess, riders, camera, DataManager and RcpMgr.retraceCount need the
     * game glue's collector (their objects are reached through GameFlow). */
}

void port_trace_frame_end(u32 frameCount) {
    PortTraceFrame f;
    int i;

    if (sState == NULL) {
        return;
    }
    memset(&f, 0, sizeof(f));
    f.frameCount = frameCount;
    f.viRetraces = port_vi_retrace_count();
    port_trace_collect(&f);
    fprintf(sState, "F %u have=%02x flow=%d,%d,%d race=%d,%08x rand=%08x rt=%u vi=%u el=%d",
            (unsigned)f.frameCount, (unsigned)f.have, (int)f.flowState, (int)f.flowNext, (int)f.flowMode,
            (int)f.raceState, (unsigned)f.raceFlags, (unsigned)f.randState,
            (unsigned)f.retraceCount, (unsigned)f.viRetraces, (int)f.elapsed);
    fprintf(sState, " cam=%08x,%08x,%08x,%08x,%08x,%08x", fbits(f.camEye[0]), fbits(f.camEye[1]),
            fbits(f.camEye[2]), fbits(f.camTarget[0]), fbits(f.camTarget[1]),
            fbits(f.camTarget[2]));
    for (i = 0; i < f.numRiders && i < PORT_TRACE_MAX_RIDERS; i++) {
        const PortTraceRider *r = &f.riders[i];
        fprintf(sState, " r%d=%08x,%08x,%08x,%08x,%08x,%08x,%08x,%08x/%d", i, fbits(r->pos[0]),
                fbits(r->pos[1]), fbits(r->pos[2]), fbits(r->quat[0]), fbits(r->quat[1]),
                fbits(r->quat[2]), fbits(r->quat[3]), fbits(r->speed), (int)r->animState);
    }
    if (f.have & PORT_TRACE_HAVE_ENTRY) {
        fprintf(sState, " e0=%08x,%08x,%d,%d", fbits(f.entryPct), fbits(f.entrySpeed),
                (int)f.entryFinished, (int)f.entryHealth);
    }
    fputc('\n', sState);
}
