/* 1080 Snowboarding PC port: entry point and fixed-step scheduler
 * (PC port plan 9.6).
 *
 * One host thread runs the simulation at a fixed 30 Hz step, decoupled from
 * presentation:
 *
 *     acc += now - last;
 *     while (acc >= STEP && steps < 4) {     catch-up cap
 *         step();                            input_poll + app_runOneFrame + 2 VI retraces
 *         acc -= STEP; steps++;
 *     }
 *     host_present();
 *
 * Every step runs Display and gfx_end, even when catching up (gfx_end runs
 * MorphUpd / ADrawUpd, and display code grabs the thumbnail every 4th frame);
 * only the host GPU submit of a skipped frame may be dropped (M2). In
 * headless / trace mode the steps run back to back with no sleeping.
 *
 * Game entry points: the PORT restructure of app_run (plan 9.6.2, 17.7), defined
 * in src/game_libs/game_libs_ncc_34EB4.cpp under #ifdef PORT. They are weak
 * here, so main.c also links (and runs a stand-in frame) without libgame.a:
 *   port_game_boot()    port/uso bootup load, boot_main_00066850 .. App_main
 *                       -> app_runInit()
 *   app_runOneFrame()   one iteration of app_run's inner loop
 *   port_game_frame_count()  SysLib.frameCount (trace key)
 *
 * Host hooks the game side calls (weak defaults below, replaced by M2 / M4 code):
 *   port_audio_frame()  once per VI retrace (RcpMgr 'd'); port/audio (agent-c)
 *   port_gfx_task()     an M_GFXTASK from osSpTaskStartGo; Fast3D (M2)
 *   port_audio_task()   an M_AUDTASK from osSpTaskStartGo; port/audio, if it
 *                       runs the audio task through the SP path at all
 *
 * Usage: 1080 [--rom baserom.z64] [--sram save.sram] [--frames N]
 *             [--input script.txt] [--record in.log] [--replay in.log] [--trace state.log]
 *             [--audio-out out.wav] [--authentic] [--realtime] [--fork-at STEP --fork-dir DIR]
 *
 * --fork-at / --fork-dir (run 871, port/tools/steer-search.py's beam search): run to STEP, then
 * serve fork requests instead of replaying every candidate from boot.  DIR holds two FIFOs the
 * client made, cmd and ack; the server opens cmd for reading, then ack for writing, and answers
 * each command line with one line:
 *   run SCRIPT UNTIL OUTDIR     fork a child: chdir OUTDIR (its 1080.sram goes there), swap in
 *                               SCRIPT (same step count), state trace OUTDIR/state.log from here,
 *                               run to step UNTIL and exit.  Reply "pid N".
 *   spawn SCRIPT UNTIL NEWDIR   fork a child that runs SCRIPT to step UNTIL and then serves NEWDIR
 *                               (its FIFOs made by the client).  Reply "pid N".
 *   wait                        wait for every run child.  Reply "done N" (N children).
 *   quit                        exit at once.
 * The port is deterministic, so a child continues exactly as a run from boot with SCRIPT would.
 */

#include "port_libc.h"
#include "host.h"
#include "trace.h"
#include "ultra/ultra.h"
#ifdef PORT_WIN32
#include "win32/win32_host.h"
#endif
#ifdef PORT_WASM
#include <unistd.h> /* chdir (plan 30) */
#endif

/* port/uso/uso_rom.c (PORT_RUNTIME_BLOBS, plan 29): the USO blobs from the ROM read at run time */
extern int port_uso_blobs_load(const unsigned char *rom, unsigned int romSize, const char **why, const char **file)
    __attribute__((weak));

#define CATCH_UP_CAP 4

extern void port_game_boot(void) __attribute__((weak));
extern void app_runOneFrame(void) __attribute__((weak));
extern u32 port_game_frame_count(void) __attribute__((weak));

/* ---- host hooks the game side reaches (weak no-ops until M2 / M4 provide them) ---- */
__attribute__((weak)) void port_audio_frame(void) {
}

extern int port_audio_capture_open(const char *path) __attribute__((weak));
extern void port_audio_capture_close(void) __attribute__((weak));

__attribute__((weak)) void port_gfx_task(OSTask_t *task) {
    (void)task;
}

__attribute__((weak)) void port_audio_task(OSTask_t *task) {
    (void)task;
}

/* the end of a step: port/gfx publishes the step's gfx tasks as one live frame (plan 19.11) */
__attribute__((weak)) void port_gfx_step_end(void) {
}
extern void port_live_close(void) __attribute__((weak));

/* PortUltraConfig.rcpTask: osSpTaskStartGo runs the task here, synchronously. */
static void rcp_task(OSTask_t *task) {
    if (task->type == M_GFXTASK) {
        port_gfx_task(task);
    } else if (task->type == M_AUDTASK) {
        port_audio_task(task);
    }
}

/* A PORT wait on the RCP queue (gfx_end / gfx_waitRcpIdle) that RcpMgr_portDrain cannot
 * advance: nothing is queued, yet a task is still marked busy. */
void port_rcp_stuck(void *rcpMgr, const char *where) {
    port_ultra_log("main: DEADLOCK: %s: RcpMgr %p busy with no message pending", where, rcpMgr);
    host_fatal("main: RCP wait cannot complete");
}

/* sys_fatal (PORT entry, plan 18.9): the N64 prints the message on the console window and
 * halts in sys_fatalHaltLoop, polling the pads forever.  Headless, that is a silent hang, so
 * the port reports the message (the game's %d / %f / %s / %c / %x; its %r is printed as %x)
 * with a backtrace and exits. */
extern int backtrace(void **frames, int n) __attribute__((weak));
extern void backtrace_symbols_fd(void *const *frames, int n, int fd) __attribute__((weak));
__attribute__((noreturn)) void port_sys_fatal(const char *fmt, va_list ap) {
    char f[512];
    size_t i;
    for (i = 0; fmt[i] != '\0' && i + 1 < sizeof(f); i++) {
        f[i] = (fmt[i] == 'r' && i > 0 && fmt[i - 1] == '%') ? 'x' : fmt[i];
    }
    f[i] = '\0';
    fprintf(stderr, "PORT: sys_fatal: ");
    vfprintf(stderr, f, ap);
    fprintf(stderr, "\n");
    if (backtrace != 0 && backtrace_symbols_fd != 0) {
        void *frames[64];
        int n = backtrace(frames, 64);
        backtrace_symbols_fd(frames, n, 2);
    }
    host_fatal("main: the game called sys_fatal (on the N64: the console message and a halt)");
}

/* A host crash (SIGSEGV / SIGBUS / SIGFPE / SIGILL) prints the same backtrace and exits
 * 128 + signal, so capture.sh / scripted-input runs show where the game died (run 846).
 * signal() is weak: port_libc.h has no <signal.h>; the numbers are Linux's. */
extern void (*signal(int sig, void (*fn)(int)))(int) __attribute__((weak));
#ifdef PORT_WIN32
/* plan 29: the game side cannot reference both exit and _exit (port/win32/gen-win32-bridge.py) */
#define port_crash_exit(status) exit(status)
#else
extern void _exit(int status) __attribute__((noreturn));
#define port_crash_exit(status) _exit(status)
#endif
static void port_crash_signal(int sig) {
    fprintf(stderr, "PORT: signal %d at step %u\n", sig, (unsigned)(port_game_frame_count != 0 ? port_game_frame_count() : 0));
    if (backtrace != 0 && backtrace_symbols_fd != 0) {
        void *frames[64];
        int n = backtrace(frames, 64);
        backtrace_symbols_fd(frames, n, 2);
    }
    port_crash_exit(128 + sig);
}

typedef struct Options {
    const char *romPath;
    const char *sramPath;
    const char *recordPath;
    const char *replayPath;
    const char *statePath;
    const char *inputPath; /* --input: scripted pads (port/host/input_script.c) */
    const char *audioPath; /* --audio-out: the game's audio to a WAV (port/audio, agent-c) */
    u32 frames;      /* 0 = run until quit (or the replay ends) */
    int authentic;   /* 29.97 Hz step instead of 30 */
    int realtime;    /* pace to the wall clock even when headless */
} Options;

static u32 sSteps; /* steps run so far */
static u32 sForkAt;          /* --fork-at: serve fork requests at this step (0 = never) */
static const char *sForkDir; /* --fork-dir */
static u32 sFrameLimit;      /* a fork child's UNTIL (overrides --frames) */

/* SSE scalar maths with flush-to-zero and denormals-are-zero, round to
 * nearest: the VR4300's FCSR as osInitialize sets it (FS = 1, RM = 0; plan
 * 9.5). The invalid-operation trap stays masked on the host. */
static void fpu_setup(void) {
#if defined(__i386__) || defined(__x86_64__)
    unsigned int csr;
    __asm__ volatile("stmxcsr %0" : "=m"(csr));
    csr |= 0x8040u;   /* FTZ | DAZ */
    csr &= ~0x6000u;  /* RC = round to nearest */
    csr |= 0x1F80u;   /* all exceptions masked */
    __asm__ volatile("ldmxcsr %0" : : "m"(csr));
#endif
}

/* Poll the host pads into the SI (osContStartReadData snapshots them). */
static void input_poll(void) {
    HostPad hp[HOST_MAX_PADS];
    OSContPad pads[MAXCONTROLLERS];
    u8 connected[MAXCONTROLLERS];
    u8 rumble[MAXCONTROLLERS];
    int i;

    memset(hp, 0, sizeof(hp));
    host_read_pads(hp);
    for (i = 0; i < MAXCONTROLLERS; i++) {
        pads[i].button = hp[i].button;
        pads[i].stick_x = hp[i].stick_x;
        pads[i].stick_y = hp[i].stick_y;
        pads[i].errno = 0;
        connected[i] = hp[i].connected;
        rumble[i] = hp[i].hasRumble;
    }
    port_si_set_pads(pads, connected, rumble);
}

/* Stand-in for app_runOneFrame while the game is not linked: exercises the
 * shim's per-frame path (SI read through the event queue, the pad hook, a
 * frame swap, a PI read) and the trace hooks, so record / replay / state
 * traces can be tested end to end. */
static OSMesgQueue sStubSiQueue;
static OSMesg sStubSiMsg[1];
static u8 sStubFb[2][16];

static void stub_frame(void) {
    static int init;
    OSContPad pads[MAXCONTROLLERS];
    OSMesg m;
    u32 word;
    int i;

    if (!init) {
        u8 bits;
        OSContStatus st[MAXCONTROLLERS];
        init = 1;
        osCreateMesgQueue(&sStubSiQueue, sStubSiMsg, 1);
        osSetEventMesg(OS_EVENT_SI, &sStubSiQueue, (OSMesg)0);
        osContInit(&sStubSiQueue, &bits, st);
        host_log("main: game not linked; running the stand-in frame (pads 0x%x)", bits);
    }
    osContStartReadData(&sStubSiQueue);
    osRecvMesg(&sStubSiQueue, &m, OS_MESG_BLOCK);
    osContGetReadData(pads);
    for (i = 0; i < MAXCONTROLLERS; i++) {
        u32 held = pads[i].button;
        s8 rx = pads[i].stick_x;
        s8 ry = pads[i].stick_y;
        if (pads[i].errno != 0) {
            continue;
        }
        port_trace_pad(i, sSteps, (s32)sSteps, &held, &rx, &ry);
    }
    osPiReadIo(0, &word); /* ROM header word 0 (0x80371240 for a .z64) */
    osViSwapBuffer(sStubFb[sSteps & 1]);
    port_trace_frame_end(sSteps);
}

static void step(void) {
    input_poll();
    if (app_runOneFrame != NULL) {
        app_runOneFrame(); /* calls port_trace_pad / port_trace_frame_end itself */
    } else {
        stub_frame();
    }
    /* two virtual retraces per update (plan 9.1: 30 Hz = every other retrace) */
    port_vi_retrace();
    port_vi_retrace();
    port_gfx_step_end();
    sSteps++;
}

static int done(const Options *o) {
    if (sFrameLimit != 0 ? sSteps >= sFrameLimit : (o->frames != 0 && sSteps >= o->frames)) {
        return 1;
    }
    if (port_trace_replaying()) {
        u32 frame = port_game_frame_count != NULL ? port_game_frame_count() : sSteps;
        if (frame > port_trace_replay_last_frame()) {
            return 1;
        }
    }
    return 0;
}

#if !defined(PORT_LIBC_DECLS_ONLY) && !defined(PORT_WIN32) && !defined(PORT_WASM) /* plan 30: wasm has no fork */
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>

/* The fork server (see the header comment).  Returns in a child that should go on running. */
static void fork_serve(void) {
    char path[1024], line[2048], cmd[16], a1[1024], a3[1024];
    int pids[512], npids = 0, cfd, afd, n, len = 0;
    unsigned long until;

    snprintf(path, sizeof(path), "%s/cmd", sForkDir);
    cfd = open(path, O_RDONLY);
    snprintf(path, sizeof(path), "%s/ack", sForkDir);
    afd = open(path, O_WRONLY);
    if (cfd < 0 || afd < 0) {
        host_fatal("main: fork server: cannot open the FIFOs in %s", sForkDir);
    }
    for (;;) {
        char *nl;
        while ((nl = memchr(line, '\n', len)) == NULL) {
            n = (int)read(cfd, line + len, sizeof(line) - 1 - len);
            if (n <= 0) {
                _exit(0); /* the client went away */
            }
            len += n;
        }
        *nl = '\0';
        a1[0] = a3[0] = '\0';
        until = 0;
        if (sscanf(line, "%15s %1023s %lu %1023s", cmd, a1, &until, a3) < 1) {
            cmd[0] = '\0';
        }
        memmove(line, nl + 1, len - (nl + 1 - line));
        len -= (int)(nl + 1 - line);
        if (strcmp(cmd, "quit") == 0) {
            _exit(0);
        } else if (strcmp(cmd, "wait") == 0) {
            int i, k = npids;
            for (i = 0; i < npids; i++) {
                waitpid(pids[i], NULL, 0);
            }
            npids = 0;
            while (waitpid(-1, NULL, WNOHANG) > 0) { /* spawned servers that quit */
            }
            n = snprintf(path, sizeof(path), "done %d\n", k);
            if (write(afd, path, n) != n) {
                _exit(0);
            }
        } else if ((strcmp(cmd, "run") == 0 || strcmp(cmd, "spawn") == 0) && until > sSteps && a3[0]) {
            int pid = fork();
            if (pid == 0) {
                close(cfd);
                close(afd);
                if (host_input_script_reload(a1) != 0) {
                    _exit(3);
                }
                if (cmd[0] == 'r') {
                    snprintf(path, sizeof(path), "%s/state.log", a3);
                    if (chdir(a3) != 0 || port_trace_reopen_state(path) != 0) {
                        _exit(4);
                    }
                    sFrameLimit = (u32)until;
                    sForkAt = 0;
                } else {
                    sForkAt = (u32)until;
                    sForkDir = strdup(a3);
                }
                return;
            }
            if (cmd[0] == 'r' && pid > 0 && npids < 512) {
                pids[npids++] = pid;
            }
            n = snprintf(path, sizeof(path), "pid %d\n", pid);
            if (write(afd, path, n) != n) {
                _exit(0);
            }
        } else {
            n = snprintf(path, sizeof(path), "error %s\n", cmd);
            if (write(afd, path, n) != n) {
                _exit(0);
            }
        }
    }
}
#else
static void fork_serve(void) {
    host_fatal("main: --fork-at needs the host libc");
}
#endif

static void run(const Options *o, int headless) {
    const double stepSec = o->authentic ? 1001.0 / 30000.0 : 1.0 / 30.0;
    double last = host_time_seconds();
    double acc = 0.0;

    while (!done(o)) {
        if (!host_poll_events()) {
            break;
        }
        if (headless && !o->realtime) {
            while (sForkAt != 0 && sSteps == sForkAt) {
                fork_serve(); /* returns in a child */
            }
            step();
            continue;
        }
        {
            double now = host_time_seconds();
            int steps = 0;
            acc += now - last;
            last = now;
            while (acc >= stepSec && steps < CATCH_UP_CAP && !done(o)) {
                step();
                acc -= stepSec;
                steps++;
            }
            if (acc >= stepSec) {
                acc = 0.0; /* too far behind: drop the backlog (the game slows, as on hardware) */
            }
            host_present();
            if (acc < stepSec) {
                host_sleep_seconds(stepSec - acc);
            }
        }
    }
}

/* a byte-swapped (.v64) or little-endian (.n64) dump in big-endian (.z64) order, in place */
static void rom_to_z64(void *rom, u32 size) {
    u8 *p = (u8 *)rom;
    u32 i;
    if (size >= 4 && p[0] == 0x37 && p[1] == 0x80) {
        for (i = 0; i + 1 < size; i += 2) {
            u8 t = p[i];
            p[i] = p[i + 1];
            p[i + 1] = t;
        }
    } else if (size >= 4 && p[0] == 0x40 && p[1] == 0x12) {
        for (i = 0; i + 3 < size; i += 4) {
            u8 a = p[i], b = p[i + 1];
            p[i] = p[i + 3];
            p[i + 1] = p[i + 2];
            p[i + 2] = b;
            p[i + 3] = a;
        }
    }
}

static void usage(void) {
    host_log("usage: 1080 [--rom PATH] [--sram PATH] [--frames N] [--input SCRIPT] [--record LOG] [--replay LOG]"
             " [--trace LOG] [--audio-out WAV] [--authentic] [--realtime]");
}

static int parse(int argc, char **argv, Options *o) {
    int i;

    memset(o, 0, sizeof(*o));
    o->romPath = "baserom.z64";
    o->sramPath = "1080.sram";
    for (i = 1; i < argc; i++) {
        const char *a = argv[i];
        const char *v = i + 1 < argc ? argv[i + 1] : NULL;
        if (strcmp(a, "--authentic") == 0) {
            o->authentic = 1;
#ifdef PORT_WIN32
        } else if (strcmp(a, "--headless") == 0) {
            /* port_win32_setup: no window */
#endif
        } else if (strcmp(a, "--realtime") == 0) {
            o->realtime = 1;
        } else if (v == NULL) {
            usage();
            return -1;
        } else if (strcmp(a, "--rom") == 0) {
            o->romPath = v, i++;
        } else if (strcmp(a, "--sram") == 0) {
            o->sramPath = v, i++;
        } else if (strcmp(a, "--frames") == 0) {
            o->frames = (u32)strtoul(v, NULL, 10), i++;
        } else if (strcmp(a, "--input") == 0) {
            o->inputPath = v, i++;
        } else if (strcmp(a, "--record") == 0) {
            o->recordPath = v, i++;
        } else if (strcmp(a, "--replay") == 0) {
            o->replayPath = v, i++;
        } else if (strcmp(a, "--trace") == 0) {
            o->statePath = v, i++;
        } else if (strcmp(a, "--fork-at") == 0) {
            sForkAt = (u32)strtoul(v, NULL, 10), i++;
        } else if (strcmp(a, "--fork-dir") == 0) {
            sForkDir = v, i++;
        } else if (strcmp(a, "--audio-out") == 0) {
            o->audioPath = v, i++;
        } else {
            usage();
            return -1;
        }
    }
    return 0;
}

int main(int argc, char **argv) {
    Options o;
    HostConfig hc;
    PortUltraConfig uc;
    void *rom;
    u32 romSize;
    const PortUltraStats *st;
    int headless = 1; /* only the headless host exists so far */

    if (parse(argc, argv, &o) != 0) {
        return 2;
    }
#ifdef PORT_WASM
    {
        /* plan 30: wasi-libc resolves a relative path from its own current directory ("/"); the native host
         * passes the real one, so --rom baserom.z64 / --trace x.log mean what they mean on Linux */
        const char *cwd = getenv("PORT_WASM_CWD");
        if (cwd != NULL && chdir(cwd) != 0) {
            host_fatal("main: cannot chdir to %s", cwd);
        }
    }
#endif
#ifdef PORT_WIN32
    {
        int i, romGiven = 0, sramGiven = 0;
        for (i = 1; i < argc; i++) {
            romGiven |= strcmp(argv[i], "--rom") == 0;
            sramGiven |= strcmp(argv[i], "--sram") == 0;
        }
        port_win32_setup(argc, argv, &o.romPath, romGiven, &o.sramPath, sramGiven, &o.realtime);
    }
#endif
    fpu_setup();
    if (signal != 0) {
        signal(11, port_crash_signal); /* SIGSEGV */
        signal(7, port_crash_signal);  /* SIGBUS */
        signal(8, port_crash_signal);  /* SIGFPE */
        signal(4, port_crash_signal);  /* SIGILL */
    }
    memset(&hc, 0, sizeof(hc));
    hc.title = "1080 Snowboarding";
    hc.headless = headless;
    if (host_init(&hc) != 0) {
        host_fatal("main: host_init failed");
    }
    if (o.inputPath != NULL && host_input_script_open(o.inputPath) != 0) {
        host_fatal("main: bad input script %s", o.inputPath);
    }
    if (host_file_read_all(o.romPath, &rom, &romSize) != 0) {
        host_fatal("main: cannot read the ROM %s", o.romPath);
    }
    rom_to_z64(rom, romSize);
    if (romSize < 0x1000 || ((u8 *)rom)[0] != 0x80 || ((u8 *)rom)[1] != 0x37) {
        host_fatal("main: %s is not an N64 ROM image (.z64 / .n64 / .v64)", o.romPath);
    }
    if (port_uso_blobs_load != NULL) {
        const char *why = NULL, *file = NULL;
        if (port_uso_blobs_load((const unsigned char *)rom, romSize, &why, &file) != 0) {
            host_fatal("main: %s: %s%s%s: this build needs 1080 Snowboarding (US 1.0)", o.romPath, why,
                       file != NULL ? " in " : "", file != NULL ? file : "");
        }
    }

    memset(&uc, 0, sizeof(uc));
    uc.rom = (const u8 *)rom;
    uc.romSize = romSize;
    uc.sramPath = o.sramPath;
    uc.deadlockFatal = 1;
    uc.maxIdleRetraces = 600;
    uc.rcpTask = rcp_task; /* M2: Fast3D for M_GFXTASK; M4: CPU audio for M_AUDTASK */
    port_ultra_init(&uc);
    osInitialize();

    if (port_trace_open(o.recordPath, o.replayPath, o.statePath) != 0) {
        host_fatal("main: cannot open the trace files");
    }
    if (o.replayPath == NULL && o.frames == 0 && headless && app_runOneFrame == NULL) {
        o.frames = 300; /* the stand-in frame would otherwise run forever */
    }

    if (o.audioPath != NULL && (port_audio_capture_open == NULL || port_audio_capture_open(o.audioPath) != 0)) {
        host_fatal("main: cannot write the audio to %s", o.audioPath);
    }
    if (port_game_boot != NULL) {
        port_game_boot();
    }
    run(&o, headless);
    if (port_live_close != NULL) {
        port_live_close();
    }
    if (o.audioPath != NULL) {
        port_audio_capture_close();
    }

    st = port_ultra_stats();
    host_log("main: %u steps, %u retraces (%u idle), %u pumps, %u deadlocks, %u parked threads, "
             "%u PI bytes, %u SRAM writes, %u RCP tasks",
             (unsigned)sSteps, (unsigned)st->retraces, (unsigned)st->idleRetraces,
             (unsigned)st->pumps, (unsigned)st->deadlocks, (unsigned)st->parkedThreads,
             (unsigned)st->piBytes, (unsigned)st->sramWrites, (unsigned)st->spTasks);
    port_trace_close();
    port_ultra_shutdown();
    host_free(rom);
    host_shutdown();
    return 0;
}
