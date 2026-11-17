/*
 * port/audio/tools/seqrender: render a 1080 sequence to a WAV with the real
 * audio driver, without booting the game (docs/1080_PC_PORT_PLAN.md 15.6 / 15.7).
 *
 * It links the same objects as tenshoe (libgame.a's driver and snd_ glue, port/uso,
 * port/ultra, port/audio) with its own main:
 *   1. ROM -> port/ultra; port_uso_init + port_sysapi_install (bootup.uso Data / Bss and
 *      gSysApi, as port_game_boot does before boot_main);
 *   2. SnowBoard_ctor's audio part: fs_gdlFileInfo of rom/audio_seq.uso / audio_wave.uso /
 *      audio_bank.uso, then snd_Init(seq, bank, wave) -> AudioLoad_Init (heap init sizes,
 *      port_audio_swap_static, the tables, the spec);
 *   3. app_runInit's snd_PlayPlayer0Seq0 (sequence 0, the sound-effect player);
 *   4. the request: --seq N  = snd_PlayPlayer1SeqDefault(N) (player 1, the music player), and / or
 *      --event M[:S][@SEC] (up to 8) = snd_seqEventDbg_00000148(M, S, 0) at SEC (the game's
 *      snd_playSeq -> snd_HandleSeqEvent music state machine: what n64proc / titproc /
 *      timproc call), plus --sfx K = snd_PostEvent(K) at --sfx-at;
 *   5. per 30 Hz step: snd_FrameUpdate (snd_UpdateTimers -> AudioThread_ScheduleProcessCmds)
 *      then two port_audio_frame calls (the RcpMgr 'd' retraces: AudioThread_UpdateImpl +
 *      the CPU mixer), exactly app_runOneFrame's order;
 *   6. every AI buffer (osAiSetNextBuffer -> host_audio_queue) goes to the WAV
 *      (16-bit stereo, 32000 Hz: plan 10.2).
 *
 * It prints the output level (peak, RMS, clipped samples, silent tail), the mixer counters
 * (PortAudioStats) and the music player's tempo bookkeeping: the sequence's own tempo command
 * (SequencePlayer.tempo = bpm x 48, clamped to tempoInternalToExternal) against the tatums the
 * player actually advanced per second of output.
 *
 * Usage: seqrender --rom baserom.z64 (--seq N | --event M[:S][@SEC]...) [--seconds T] [--out f.wav]
 *                  [--sfx K] [--sfx-at SEC] [--no-sfx-player] [--tatums N] [--quiet]
 */
#include "port_libc.h"
#include "host.h"
#include "ultra/ultra.h"
#include "audio/port_audio.h"
#include "include/port_game.h"

/* game side (libgame.a), C linkage */
extern int fs_gdlFileInfo(int sysLib, int name, int out);
extern void snd_Init(int seqRom, int fontRom, int sampleRom);
extern void snd_PlayPlayer0Seq0(void);
extern void snd_PlayPlayer1SeqDefault(int seqId);
extern void snd_FrameUpdate(void);
extern void snd_seqEventDbg_00000148(int mode, int sub, int arg);
extern int snd_PostEvent(int ev);
extern void port_uso_init(void);

/* gAudioCtx = bootup Bss 0x345C0 (port/gen/binds.tsv: AudioLoad_Init's D_00000000). Offsets:
 * include/audio.h (asserted). */
#define ACTX (port_bootup_bss + 0x345C0)
#define ACTX_TEMPO_I2E 0x2074     /* s16 tempoInternalToExternal */
#define ACTX_TICKS 0x2040         /* s16 ticksPerUpdate (updates per frame) */
#define ACTX_SEQPLAYERS 0x2D00    /* SequencePlayer[4], 0x160 each */
#define SP_SIZE 0x160
#define SP_SEQID 0x04
#define SP_TEMPO 0x08
#define SP_TEMPOACC 0x0A
#define SP_TEMPOCHANGE 0x0C
#define SP_DELAY 0x10
#define SP_PC 0x78                /* scriptState.pc */
#define SP_SCRIPTCOUNTER 0xE0
#define BOOTUP_TATUMS 0x1CDD2     /* gAudioHeapInitSizes.tatumsPerBeat (bootup Data, s16) */

#define OUT_RATE 32000

/* ---- hooks main.c provides to the game (copied: seqrender has its own main) ---- */
void port_rcp_stuck(void *rcpMgr, const char *where) {
    (void)rcpMgr;
    host_fatal("seqrender: RCP wait cannot complete (%s)", where);
}

__attribute__((noreturn)) void port_sys_fatal(const char *fmt, va_list ap) {
    fprintf(stderr, "seqrender: sys_fatal: ");
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    host_fatal("seqrender: the game called sys_fatal");
}

/* ---- the AI sink ---- */
static s16 *sPcm;    /* interleaved L R */
static u32 sFrames;  /* stereo frames */
static u32 sCap;
static u32 sBuffers;
static double sAiLevel;  /* frames queued, not yet played (audio_retrace) */
static u32 sUnderruns;

static void sink(const void *samples, host_u32 bytes) {
    u32 n = bytes / 4;
    if (sFrames + n > sCap) {
        sCap = (sFrames + n) * 2 + 65536;
        sPcm = (s16 *)realloc(sPcm, (size_t)sCap * 4);
        if (sPcm == NULL) {
            host_fatal("seqrender: out of memory");
        }
    }
    memcpy(sPcm + (size_t)sFrames * 2, samples, (size_t)n * 4);
    sFrames += n;
    sBuffers++;
    sAiLevel += n;
}

static void put32(u8 *p, u32 v) { p[0] = v; p[1] = v >> 8; p[2] = v >> 16; p[3] = v >> 24; }
static void put16(u8 *p, u32 v) { p[0] = v; p[1] = v >> 8; }

static int write_wav(const char *path, const s16 *pcm, u32 frames, u32 rate) {
    FILE *f = fopen(path, "wb");
    u8 h[44];
    u32 i, bytes = frames * 4;
    if (f == NULL) {
        return -1;
    }
    memcpy(h, "RIFF", 4); put32(h + 4, 36 + bytes); memcpy(h + 8, "WAVEfmt ", 8);
    put32(h + 16, 16); put16(h + 20, 1); put16(h + 22, 2); put32(h + 24, rate);
    put32(h + 28, rate * 4); put16(h + 32, 4); put16(h + 34, 16);
    memcpy(h + 36, "data", 4); put32(h + 40, bytes);
    fwrite(h, 1, 44, f);
    for (i = 0; i < frames * 2; i++) { /* little-endian on any host */
        u8 b[2];
        put16(b, (u16)pcm[i]);
        fwrite(b, 1, 2, f);
    }
    return fclose(f) == 0 ? 0 : -1;
}

/* A DAC draining 32006 / 60 frames per retrace (osViClock / dacRate, plan 10.2), so the
 * driver's osAiGetLength feedback picks its buffer lengths as on hardware (headless reports 0:
 * every buffer would be the 560-frame maximum and the output 5 % longer than the game time). */

static host_u32 ai_buffered(void) {
    return (host_u32)sAiLevel * 4;
}

static void audio_retrace(void) {
    sAiLevel -= 32006.0 / 60.0;
    if (sAiLevel < 0) {
        if (sFrames != 0) {
            sUnderruns++;
        }
        sAiLevel = 0;
    }
    port_audio_frame();
}

static void fpu_setup(void) { /* main.c's: FTZ / DAZ, round to nearest (plan 9.5) */
    unsigned int csr;
    __asm__ volatile("stmxcsr %0" : "=m"(csr));
    csr |= 0x8040u;
    csr &= ~0x6000u;
    csr |= 0x1F80u;
    __asm__ volatile("ldmxcsr %0" : : "m"(csr));
}

static int rom_file(const char *name) {
    int addr = 0;
    fs_gdlFileInfo((int)(port_bootup_bss + 0x3C740) /* gSysLib */, (int)name, (int)&addr);
    return addr;
}

#define SP(i) (ACTX + ACTX_SEQPLAYERS + (i) * SP_SIZE)
#define RD16(p) (*(u16 *)(p))
#define RD32(p) (*(u32 *)(p))

static void usage(void) {
    fprintf(stderr, "usage: seqrender [--rom PATH] (--seq N | --event M[:S][@SEC]...) [--seconds T] [--out FILE.wav]\n"
                    "                 [--sfx K] [--sfx-at SEC] [--no-sfx-player] [--tatums N] [--quiet]\n");
}

int main(int argc, char **argv) {
    const char *romPath = "baserom.z64", *outPath = "seqrender.wav";
    int evMode[8], evSub[8], nEv = 0;
    double evAt[8];
    int seq = -1, sfx = -1, sfxPlayer = 1, quiet = 0, tatums = 0;
    double seconds = 30.0, sfxAt = 1.0;
    PortUltraConfig uc;
    HostConfig hc;
    void *rom;
    u32 romSize, steps, step, i;
    int seqRom, bankRom, waveRom;
    /* tempo bookkeeping (player 1) */
    u32 tatumsRun = 0, tempoSteps = 0, prevDelay = 0, prevPc = 0, firstTempo = 0;
    u32 tempoMin = 0xFFFF, tempoMax = 0;
    double tempoSum = 0;
    int tracking = 0;

    for (i = 1; i < (u32)argc; i++) {
        const char *a = argv[i], *v = i + 1 < (u32)argc ? argv[i + 1] : NULL;
        if (strcmp(a, "--no-sfx-player") == 0) { sfxPlayer = 0; continue; }
        if (strcmp(a, "--quiet") == 0) { quiet = 1; continue; }
        if (v == NULL) { usage(); return 2; }
        i++;
        if (strcmp(a, "--rom") == 0) romPath = v;
        else if (strcmp(a, "--out") == 0) outPath = v;
        else if (strcmp(a, "--seq") == 0) seq = (int)strtol(v, NULL, 0);
        else if (strcmp(a, "--event") == 0 && nEv < 8) {
            const char *c = strchr(v, ':'), *t = strchr(v, '@');
            evMode[nEv] = (int)strtol(v, NULL, 0);
            evSub[nEv] = c != NULL ? (int)strtol(c + 1, NULL, 0) : 0;
            evAt[nEv] = t != NULL ? strtod(t + 1, NULL) : 0.0;
            nEv++;
        } else if (strcmp(a, "--seconds") == 0) seconds = strtod(v, NULL);
        else if (strcmp(a, "--sfx") == 0) sfx = (int)strtol(v, NULL, 0);
        else if (strcmp(a, "--sfx-at") == 0) sfxAt = strtod(v, NULL);
        else if (strcmp(a, "--tatums") == 0) tatums = (int)strtol(v, NULL, 0);
        else { usage(); return 2; }
    }
    if (seq < 0 && nEv == 0 && sfx < 0) {
        usage();
        return 2;
    }

    fpu_setup();
    memset(&hc, 0, sizeof(hc));
    hc.title = "seqrender";
    hc.headless = 1;
    host_init(&hc);
    host_headless_set_audio_sink(sink);
    host_headless_set_audio_clock(ai_buffered);
    if (host_file_read_all(romPath, &rom, &romSize) != 0 || romSize < 0x1000 || ((u8 *)rom)[0] != 0x80) {
        host_fatal("seqrender: cannot read the .z64 ROM %s", romPath);
    }
    memset(&uc, 0, sizeof(uc));
    uc.rom = (const u8 *)rom;
    uc.romSize = romSize;
    uc.deadlockFatal = 1;
    uc.maxIdleRetraces = 600;
    port_ultra_init(&uc);
    osInitialize();

    port_uso_init();
    port_sysapi_install();
    if (tatums != 0) { /* experiment: override gAudioHeapInitSizes.tatumsPerBeat before AudioLoad_Init */
        u32 sw;
        port_audio_swap_static(); /* idempotent; AudioLoad_Init's call is then a no-op */
        sw = (u32)tatums;
        memcpy(port_bootup_data + BOOTUP_TATUMS, &sw, 2);
    }
    /* SnowBoard_ctor: snd_Init(seq, bank, wave) */
    seqRom = rom_file("rom/audio_seq.uso");
    waveRom = rom_file("rom/audio_wave.uso");
    bankRom = rom_file("rom/audio_bank.uso");
    if (!quiet) {
        host_log("seqrender: audio_seq 0x%X audio_bank 0x%X audio_wave 0x%X", seqRom, bankRom, waveRom);
    }
    snd_Init(seqRom, bankRom, waveRom);
    if (!quiet) {
        host_log("seqrender: AudioLoad_Init done: ticksPerUpdate %d, tempoInternalToExternal %d, tatumsPerBeat %d",
                 (s16)RD16(ACTX + ACTX_TICKS), (s16)RD16(ACTX + ACTX_TEMPO_I2E),
                 *(s16 *)(port_bootup_data + BOOTUP_TATUMS));
    }
    /* let the reset / init settle (the game runs App_ctor .. app_runInit before its first sound) */
    for (step = 0; step < 4; step++) {
        snd_FrameUpdate();
        audio_retrace();
        audio_retrace();
    }
    if (sfxPlayer) {
        snd_PlayPlayer0Seq0(); /* app_runInit */
    }
    if (seq >= 0) {
        snd_PlayPlayer1SeqDefault(seq);
    }
    if (getenv("SEQRENDER_DEBUG") != NULL) {
        u8 *c = ACTX;
        u32 k;
        host_log("dbg: cmd write %u read %u finished %u; procQ %08X count %d; mode word %08X",
                 c[0x53B8], c[0x53B9], c[0x53BA], RD32(c + 0x53CC), *(int *)(c + 0x53E8 + 8),
                 RD32(port_bootup_data + 0x1C0A0));
        for (k = 0; k < c[0x53B8] && k < 16; k++) {
            host_log("dbg: cmd[%u] %08X %08X", k, RD32(c + 0x5430 + 8 * k), RD32(c + 0x5430 + 8 * k + 4));
        }
    }
    sFrames = 0; /* the WAV starts at the request */
    sUnderruns = 0;

    steps = (u32)(seconds * 30.0 + 0.5);
    for (step = 0; step < steps; step++) {
        u8 *p1 = SP(1);
        if (sfx >= 0 && step == (u32)(sfxAt * 30.0 + 0.5)) {
            snd_PostEvent(sfx);
        }
        for (i = 0; i < (u32)nEv; i++) {
            if (step == (u32)(evAt[i] * 30.0 + 0.5)) {
                snd_seqEventDbg_00000148(evMode[i], evSub[i], 0);
            }
        }
        snd_FrameUpdate();
        audio_retrace();
        audio_retrace();

        /* tatums advanced by player 1 this step: the delay counts down one per tatum; a script
         * run (pc moved) consumed the remaining delay and reloaded it */
        if ((RD32(p1) >> 31) == 0 && (RD32(p1) & 1) == 0) { /* enabled: bit 31 on MIPS, bit 0 on x86 */
            tracking = 0;
            continue;
        }
        {
            u32 pc = RD32(p1 + SP_PC), delay = RD16(p1 + SP_DELAY), tempo = RD16(p1 + SP_TEMPO);
            if (tracking) {
                if (pc == prevPc) {
                    tatumsRun += prevDelay > delay ? prevDelay - delay : 0;
                } else {
                    tatumsRun += (prevDelay > 0 ? prevDelay : 1) + (delay > 0 ? delay - 1 : 0);
                }
                tempoSteps++;
                tempoSum += tempo;
                if (tempo < tempoMin) tempoMin = tempo;
                if (tempo > tempoMax) tempoMax = tempo;
            } else if (firstTempo == 0) {
                firstTempo = tempo;
            }
            prevPc = pc;
            prevDelay = delay;
            tracking = 1;
        }
    }

    if (getenv("SEQRENDER_DEBUG") != NULL) {
        u8 *c = ACTX;
        host_log("dbg end: cmd write %u read %u finished %u; procQ count %d", c[0x53B8], c[0x53B9], c[0x53BA],
                 *(int *)(c + 0x53E8 + 8));
    }
    /* ---- report ---- */
    {
        u32 n = sFrames * 2, clip = 0, nan = 0, peak = 0, lastLoud = 0, firstLoud = 0xFFFFFFFF;
        double ss = 0;
        for (i = 0; i < n; i++) {
            int s = sPcm[i], a = s < 0 ? -s : s;
            if (a >= 32767) clip++;
            if ((u32)a > peak) peak = a;
            ss += (double)s * s;
            if (a > 64) { lastLoud = i / 2; if (firstLoud == 0xFFFFFFFF) firstLoud = i / 2; }
        }
        (void)nan; /* s16 output: a NaN in the driver shows up as silence or a clip storm */
        host_log("seqrender: %u AI buffers, %u frames = %.2f s at %u Hz; peak %u, RMS %.1f, clipped %u (%.4f%%), "
                 "first sound %.3f s, last %.3f s",
                 sBuffers, sFrames, (double)sFrames / OUT_RATE, OUT_RATE, peak, n ? sqrt(ss / n) : 0.0, clip,
                 n ? 100.0 * clip / n : 0.0, firstLoud == 0xFFFFFFFF ? -1.0 : (double)firstLoud / OUT_RATE,
                 (double)lastLoud / OUT_RATE);
        host_log("seqrender: players: 0 word %08X seq %u, 1 word %08X seq %u; AI underruns %u",
                 RD32(SP(0)), SP(0)[SP_SEQID], RD32(SP(1)), SP(1)[SP_SEQID], sUnderruns);
        host_log("seqrender: mixer: %u updates, %u tasks, %u cmds, badAddr %u, unknownOps %u, unk19 %u, "
                 "raw-pointer addresses %u",
                 port_audio_stats.updates, port_audio_stats.tasks, port_audio_stats.cmds,
                 port_audio_stats.badAddr, port_audio_stats.unknownOps, port_audio_stats.unk19,
                 port_audio_stats.rawPointers);
        {
            u8 *p1 = SP(1);
            s16 i2e = (s16)RD16(ACTX + ACTX_TEMPO_I2E);
            double sec = steps / 30.0;
            double tps = tempoSteps ? tatumsRun / (tempoSteps / 30.0) : 0.0;
            double meanTempo = tempoSteps ? tempoSum / tempoSteps : 0.0;
            /* the driver: one ProcessSequence per update (ticksPerUpdate per 1/60 s frame); a tatum
             * when tempoAcc passes tempoInternalToExternal: tatums/s = 60 x ticks x tempo / i2e */
            double model = i2e ? 60.0 * (s16)RD16(ACTX + ACTX_TICKS) * meanTempo / i2e : 0.0;
            host_log("seqrender: player 1: seqId %u, tempo first %u min %u max %u mean %.1f (= %.1f bpm at 48 "
                     "tatums/beat), tempoInternalToExternal %d, scriptCounter %u",
                     p1[SP_SEQID], firstTempo, tempoMin == 0xFFFF ? 0 : tempoMin, tempoMax, meanTempo,
                     meanTempo / 48.0, i2e, RD32(p1 + SP_SCRIPTCOUNTER));
            host_log("seqrender: player 1: %u tatums in %.2f s tracked = %.1f tatums/s = %.1f bpm "
                     "(driver model %.1f tatums/s = %.1f bpm); %.2f s total",
                     tatumsRun, tempoSteps / 30.0, tps, tps * 60.0 / 48.0, model, model * 60.0 / 48.0, sec);
        }
    }
    if (write_wav(outPath, sPcm, sFrames, OUT_RATE) != 0) {
        host_fatal("seqrender: cannot write %s", outPath);
    }
    if (!quiet) {
        host_log("seqrender: wrote %s", outPath);
    }
    return 0;
}
