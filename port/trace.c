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
static int sRiderX; /* PORT_TRACE_RIDERX: the i<n>= rider input / handicap fields */
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
        sRecord = fopen(recordPath, "wb");
        if (sRecord == NULL) {
            host_log("trace: cannot write %s", recordPath);
            return -1;
        }
        fprintf(sRecord, "# 1080 port input log v2: I frame pad replayFrame held:hex8 rawX rawY\n");
    }
    if (statePath != NULL) {
        sState = fopen(statePath, "wb");
        if (sState == NULL) {
            host_log("trace: cannot write %s", statePath);
            return -1;
        }
        setvbuf(sState, NULL, _IOLBF, 0); /* a crash keeps every frame's line */
        sRiderX = getenv("PORT_TRACE_RIDERX") != NULL && getenv("PORT_TRACE_RIDERX")[0] == '1';
        fprintf(sState, "# 1080 port state trace v1 (port/trace.h)\n");
    }
    if (replayPath != NULL && load_replay(replayPath) != 0) {
        return -1;
    }
    return 0;
}

/* The fork server (port/main.c, run 871): a forked child writes its own state trace from its fork
 * step on.  The parent's FILE is line buffered, so nothing of it is pending here. */
int port_trace_reopen_state(const char *statePath) {
    if (sState != NULL) {
        fclose(sState);
    }
    sState = fopen(statePath, "wb");
    if (sState == NULL) {
        return -1;
    }
    setvbuf(sState, NULL, _IOLBF, 0);
    fprintf(sState, "# 1080 port state trace v1 (port/trace.h)\n");
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

void *port_trace_fn_obj;
unsigned port_trace_fn_objSize;

void port_trace_rider_bin(u32 step, s32 idx, const void *obj, unsigned size) {
    static FILE *sOut;
    static int sInit;
    const u32 *w = (const u32 *)obj;
    u32 i, hdr[2];
    if (!sInit) {
        const char *p = getenv("PORT_TRACE_RIDERBIN");
        sInit = 1;
        sOut = p != NULL ? fopen(p, "wb") : NULL;
    }
    if (sOut == NULL) {
        return;
    }
    hdr[0] = step;
    hdr[1] = (u32)idx;
    fwrite(hdr, 4, 2, sOut);
    for (i = 0; i < size / 4; i++) {
        u32 v = w[i];
        u8 b[4];
        b[0] = (u8)(v >> 24), b[1] = (u8)(v >> 16), b[2] = (u8)(v >> 8), b[3] = (u8)v;
        fwrite(b, 1, 4, sOut);
    }
    fflush(sOut);
}

#ifdef PORT_TRACE_FN
static FILE *sFn;
static int sFnOn;
static unsigned long sFnSp0, sFnObjFn, sFnObjSize;
static const u32 *sFnObj;
static long sFnFrame = -2;
static unsigned long sFnSeq;

#define NOINST __attribute__((no_instrument_function))
/* FNV-1a over the object's words, skipping host-pointer-looking ones (both builds keep the
 * heap and the image below 0x10000000; floats there are < 1e-30) */
static NOINST u32 fn_hash(void) {
    const u32 *w = sFnObj != NULL ? sFnObj : (const u32 *)port_trace_fn_obj;
    u32 h = 2166136261u, i, n = (sFnObj != NULL ? sFnObjSize : port_trace_fn_objSize) / 4;
    if (w == NULL) {
        return 0;
    }
    for (i = 0; i < n; i++) {
        u32 v = w[i];
        if (v >= 0x100000u && v < 0x10000000u) {
            v = 1;
        }
        h = (h ^ v) * 16777619u;
    }
    return h;
}
NOINST __attribute__((used)) void port_fn_enter(unsigned long ret, unsigned long sp, const u32 *regs) {
    int i;
    if (sFnObjFn != 0 && ret - 5 == sFnObjFn) {
        /* PORT_TRACE_FN_OBJ=<function address>:<size>: hash that function's first argument (the
         * latest one before and during the traced frame) instead of rider 0 */
#if defined(__x86_64__)
        sFnObj = (const u32 *)(unsigned long)regs[0];
#else
        sFnObj = (const u32 *)(unsigned long)((const u32 *)sp)[0];
#endif
    }
    if (!sFnOn || sFn == NULL) {
        return;
    }
    if (sFnSp0 == 0) {
        sFnSp0 = sp;
    }
    /* the function: the `call __fentry__` (5 bytes) is its first instruction */
    fprintf(sFn, "E %lx %ld %08x", ret - 5, (long)(sFnSp0 - sp), (unsigned)fn_hash());
    /* the raw arguments: i386 the first 6 stack words; x86-64 rdi rsi rdx rcx r8 r9 (low halves)
     * and xmm0-3 (low lanes) */
#if defined(__x86_64__)
    fprintf(sFn, " |");
    for (i = 0; i < 6; i++) {
        fprintf(sFn, " %x", (unsigned)regs[2 * i]);
    }
    fprintf(sFn, " | %x %x %x %x", (unsigned)regs[32], (unsigned)regs[36], (unsigned)regs[40], (unsigned)regs[44]);
#else
    (void)regs;
    fprintf(sFn, " |");
    for (i = 0; i < 6; i++) {
        fprintf(sFn, " %x", (unsigned)((const u32 *)sp)[i]);
    }
#endif
    fputc('\n', sFn);
    sFnSeq++;
}
/* __fentry__ (-pg -mfentry): the first instruction of every instrumented function, before its
 * prologue.  It saves every caller-saved register (the argument registers are live) around the
 * logger.  There is no exit hook (-finstrument-functions has one, and it breaks the matched void
 * wrappers whose callers read the result their last call left in eax / rax: item_createHel);
 * a function's effect shows in the hash at the next entry. */
#if defined(__x86_64__)
__asm__(".text\n"
        ".globl __fentry__\n__fentry__:\n"
        "  pushq %rax\n  pushq %rbp\n  movq %rsp, %rbp\n  andq $-16, %rsp\n  subq $384, %rsp\n"
        "  movq %rdi, 0(%rsp)\n  movq %rsi, 8(%rsp)\n  movq %rdx, 16(%rsp)\n  movq %rcx, 24(%rsp)\n"
        "  movq %r8, 32(%rsp)\n  movq %r9, 40(%rsp)\n  movq %r10, 48(%rsp)\n  movq %r11, 56(%rsp)\n"
        "  movdqu %xmm0, 128(%rsp)\n  movdqu %xmm1, 144(%rsp)\n  movdqu %xmm2, 160(%rsp)\n  movdqu %xmm3, 176(%rsp)\n  movdqu %xmm4, 192(%rsp)\n  movdqu %xmm5, 208(%rsp)\n  movdqu %xmm6, 224(%rsp)\n  movdqu %xmm7, 240(%rsp)\n  movdqu %xmm8, 256(%rsp)\n  movdqu %xmm9, 272(%rsp)\n  movdqu %xmm10, 288(%rsp)\n  movdqu %xmm11, 304(%rsp)\n  movdqu %xmm12, 320(%rsp)\n  movdqu %xmm13, 336(%rsp)\n  movdqu %xmm14, 352(%rsp)\n  movdqu %xmm15, 368(%rsp)\n"
        "  movq 16(%rbp), %rdi\n  leaq 24(%rbp), %rsi\n  movq %rsp, %rdx\n  call port_fn_enter\n"
        "  movdqu 128(%rsp), %xmm0\n  movdqu 144(%rsp), %xmm1\n  movdqu 160(%rsp), %xmm2\n  movdqu 176(%rsp), %xmm3\n  movdqu 192(%rsp), %xmm4\n  movdqu 208(%rsp), %xmm5\n  movdqu 224(%rsp), %xmm6\n  movdqu 240(%rsp), %xmm7\n  movdqu 256(%rsp), %xmm8\n  movdqu 272(%rsp), %xmm9\n  movdqu 288(%rsp), %xmm10\n  movdqu 304(%rsp), %xmm11\n  movdqu 320(%rsp), %xmm12\n  movdqu 336(%rsp), %xmm13\n  movdqu 352(%rsp), %xmm14\n  movdqu 368(%rsp), %xmm15\n"
        "  movq 0(%rsp), %rdi\n  movq 8(%rsp), %rsi\n  movq 16(%rsp), %rdx\n  movq 24(%rsp), %rcx\n"
        "  movq 32(%rsp), %r8\n  movq 40(%rsp), %r9\n  movq 48(%rsp), %r10\n  movq 56(%rsp), %r11\n"
        "  movq %rbp, %rsp\n  popq %rbp\n  popq %rax\n  ret\n");
#else
__asm__(".text\n"
        ".globl __fentry__\n__fentry__:\n"
        "  pushl %eax\n  pushl %ecx\n  pushl %edx\n  pushl %ebp\n  movl %esp, %ebp\n  andl $-16, %esp\n  subl $160, %esp\n"
        "  movdqu %xmm0, 32(%esp)\n  movdqu %xmm1, 48(%esp)\n  movdqu %xmm2, 64(%esp)\n"
        "  movdqu %xmm3, 80(%esp)\n  movdqu %xmm4, 96(%esp)\n  movdqu %xmm5, 112(%esp)\n"
        "  movdqu %xmm6, 128(%esp)\n  movdqu %xmm7, 144(%esp)\n"
        "  movl 16(%ebp), %ecx\n  movl %ecx, 0(%esp)\n  leal 24(%ebp), %ecx\n  movl %ecx, 4(%esp)\n"
        "  call port_fn_enter\n"
        "  movdqu 32(%esp), %xmm0\n  movdqu 48(%esp), %xmm1\n  movdqu 64(%esp), %xmm2\n"
        "  movdqu 80(%esp), %xmm3\n  movdqu 96(%esp), %xmm4\n  movdqu 112(%esp), %xmm5\n"
        "  movdqu 128(%esp), %xmm6\n  movdqu 144(%esp), %xmm7\n"
        "  movl %ebp, %esp\n  popl %ebp\n  popl %edx\n  popl %ecx\n  popl %eax\n  ret\n");
#endif
/* Dump the object's words (env PORT_TRACE_FN_DUMP=<seq>: after event seq, as hex words). */
static NOINST void fn_frame(u32 frameCount) {
    if (sFnFrame == -2) {
        const char *e = getenv("PORT_TRACE_FN");
        const char *o = getenv("PORT_TRACE_FN_OUT");
        sFnFrame = e != NULL ? strtol(e, NULL, 10) : -1;
        e = getenv("PORT_TRACE_FN_OBJ");
        if (e != NULL) {
            char *end;
            sFnObjFn = strtoul(e, &end, 16);
            sFnObjSize = *end == ':' ? strtoul(end + 1, NULL, 16) : 0x100;
        }
        if (sFnFrame >= 0) {
            sFn = fopen(o != NULL ? o : "fn.log", "wb");
            if (sFn != NULL) {
                setvbuf(sFn, NULL, _IOLBF, 0);   /* a crash inside the frame keeps the log */
            }
        }
    }
    if (sFn == NULL) {
        return;
    }
    if (sFnOn) {
        u32 i, n = (sFnObj != NULL ? sFnObjSize : port_trace_fn_objSize) / 4;
        const u32 *w = sFnObj != NULL ? sFnObj : (const u32 *)port_trace_fn_obj;
        fprintf(sFn, "D");
        for (i = 0; w != NULL && i < n; i++) {
            fprintf(sFn, " %08x", (unsigned)w[i]);
        }
        fprintf(sFn, "\n");
        fclose(sFn);
        sFn = NULL;
        sFnOn = 0;
        return;
    }
    if ((long)frameCount + 1 == sFnFrame) {
        u32 i, n = (sFnObj != NULL ? sFnObjSize : port_trace_fn_objSize) / 4;
        const u32 *w = sFnObj != NULL ? sFnObj : (const u32 *)port_trace_fn_obj;
        fprintf(sFn, "B");
        for (i = 0; w != NULL && i < n; i++) {
            fprintf(sFn, " %08x", (unsigned)w[i]);
        }
        fprintf(sFn, "\n");
        sFnSp0 = 0;
        sFnOn = 1;
    }
}
#endif

/* port/gfx/port_gfx.c with PORT_TRACE_SCREEN=1: the step's display-list content hash (scr=) */
__attribute__((weak)) int port_gfx_screen_hash(u32 *out) {
    (void)out;
    return 0;
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
#ifdef PORT_TRACE_FN
    fn_frame(frameCount);
#endif
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
    if (sRiderX) {   /* PORT_TRACE_RIDERX=1 (run 871): i<n>=flags,maxSpeed,stickX,stickY,held */
        for (i = 0; i < f.numRiders && i < PORT_TRACE_MAX_RIDERS; i++) {
            const PortTraceRider *r = &f.riders[i];
            fprintf(sState, " i%d=%x,%08x,%08x,%08x,%x", i, (unsigned)r->flags, fbits(r->maxSpeed),
                    fbits(r->stickX), fbits(r->stickY), (unsigned)r->held);
        }
    }
    if (f.have & PORT_TRACE_HAVE_ENTRY) {
        fprintf(sState, " e0=%08x,%08x,%d,%d,%d,%d", fbits(f.entryPct), fbits(f.entrySpeed),
                (int)f.entryFinished, (int)f.entryHealth, (int)f.entryScore, (int)f.entryPending);
        for (i = 1; i < f.numEntries && i < PORT_TRACE_MAX_RIDERS; i++) {
            fprintf(sState, " e%d=%08x,%d,%d,%d", i, fbits(f.entry[i].pct), (int)f.entry[i].finished,
                    (int)f.entry[i].health, (int)f.entry[i].score);
        }
    }
    if (f.have & PORT_TRACE_HAVE_TRICK) {
        fprintf(sState, " tk=%x,%d,%d,%d", (unsigned)f.trickButtons, (int)f.trickFrames,
                (int)f.trickQueued, (int)f.trickCur);
    }
    if (f.have & PORT_TRACE_HAVE_GATES) {
        fprintf(sState, " cd=%d,%d,%08x,%08x,%08x,%08x,%d", (int)f.dmScore, (int)f.gatesLeft,
                fbits(f.gateX), fbits(f.gateZ), fbits(f.forkX), fbits(f.forkZ), (int)f.forkFlags);
    }
    if (f.have & PORT_TRACE_HAVE_MGR) {
        fprintf(sState, " ml=%d,%d,%d,%d,%d", (int)f.mgrRetries, (int)f.mgrIdx, (int)f.mgrCount, (int)f.mgrCourse,
                (int)f.mgrRider);
    }
    if (f.have & PORT_TRACE_HAVE_LESSON) {
        fprintf(sState, " trn=%d", (int)f.lesson);
    }
    {
        u32 scr;
        if (port_gfx_screen_hash(&scr)) {
            fprintf(sState, " scr=%08x", (unsigned)scr);
        }
    }
    fputc('\n', sState);
}
