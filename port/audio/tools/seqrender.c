/*
 * port/audio/tools/seqrender: render a 1080 sequence to a WAV with the real
 * audio driver, without booting the game (docs/1080_PC_PORT_PLAN.md 15.6 .. 15.8).
 *
 * It links the same objects as tenshoe (libgame.a's driver and snd_ glue, port/uso,
 * port/ultra, port/audio) with its own main:
 *   1. ROM -> port/ultra; port_uso_init + port_sysapi_install (bootup.uso Data / Bss and
 *      gSysApi, as port_game_boot does before boot_main);
 *   2. SnowBoard_ctor's audio part: fs_gdlFileInfo of rom/audio_seq.uso / audio_wave.uso /
 *      audio_bank.uso, then snd_Init(seq, bank, wave) -> AudioLoad_Init (heap init sizes,
 *      port_audio_swap_static, the tables, the spec);
 *   3. GameFlow_create's options (Backup_readOptions, defaults from Backup_resetOptionsData):
 *      snd_SetPlayer1VolumeLevel(8) / snd_SetPlayer0VolumeLevel(8) / snd_SetPlayer2VolumeLevel(8)
 *      (AUDIOCMD 0x41 fadeVolumeScale = the Data 0x1C128 table [8] = 0.8 for players 1 / 0 / 2;
 *      channel volume is squared, so the music plays at 0.64 of a bare --seq) and
 *      snd_SetSoundOutputMode(flags & 3 = 0, stereo); --levels M,S,V picks other menu levels
 *      (0..10), --raw skips the step; then app_runInit's snd_PlayPlayer0Seq0 (sequence 0, the
 *      sound-effect player);
 *   4. the request:
 *        --seq N             snd_PlayPlayer1SeqDefault(N) (player 1, the music player);
 *        --event M[:S][@SEC] snd_seqEventDbg_00000148(M, S, 0) at SEC (snd_playSeq ->
 *                            snd_HandleSeqEvent: the game's music state machine);
 *        --post K[:V][@SEC]  snd_dispatchEvent(K, V) at SEC (snd_postEvent / snd_sendEvent);
 *        --sfx K [--sfx-at SEC]  snd_PostEvent(K);
 *        --title [--title-at SEC]  the boot's music: n64proc's snd_playSeq(1) at 0 (spec 1, the
 *                            N64 logo sound 0xA3) and titproc's snd_playSeq(0) at SEC (spec 0 and
 *                            sequence 2 on player 1: the title music; default 7.4 s, the
 *                            tenshoe boot's time from mode 1 to mode 0);
 *        --spec N            snd_StopAudioCmds(N) (an audio-spec change) and 30 steps before --seq: the boot
 *                            spec's temporary sequence cache is 0x2E00 bytes, seqs 7 / 9 / 19 are bigger
 *                            (their load would run into gAudioCtx; the game plays them in race spec 3);
 *        --course N [--gametype G]  the race start as the game does it (plan 15.8):
 *            post 12:G (the title menu's game type: 0 match race .. 5 training; omitted = the
 *            boot value), event 4:N (timproc's snd_playSeq(4, course) at the race load), then
 *            RaceProcess_handleInput's countdown at 30 Hz: 120 frames, then posts 0x12 + 0x10
 *            ("3"), 0x12 ("2"), 0x12 ("1") and 0x11 ("GO") 20 frames apart.  Event 0x11 is
 *            what starts the course music: snd_dispatchEvent case 17 schedules the music
 *            command word (course + 4) 22 frames later;
 *   5. per 30 Hz step: snd_FrameUpdate (snd_UpdateTimers -> AudioThread_ScheduleProcessCmds)
 *      then two port_audio_frame calls (the RcpMgr 'd' retraces: AudioThread_UpdateImpl +
 *      the CPU mixer), exactly app_runOneFrame's order;
 *   6. every AI buffer (osAiSetNextBuffer -> host_audio_queue) goes to the WAV
 *      (16-bit stereo, 32000 Hz: plan 10.2).
 *
 * It prints the output level (peak, RMS, clipped samples, first / last sound), the mixer counters
 * (PortAudioStats), the music player's starts / stops, its tempo (the header's tempo command,
 * the player's tempo, and the tatums it advanced: per driver tick tempoAcc += tempo, a tatum per
 * tempoInternalToExternal) and an FNV-1a hash of the PCM.  --summary prints one machine-readable
 * "RESULT key=value ..." line (port/audio/tools/regress.py).
 *
 * Usage: seqrender --rom baserom.z64 (--seq N | --event ... | --post ... | --sfx K | --course N)
 *                  [--seconds T] [--out f.wav] [--no-sfx-player] [--tatums N] [--quiet] [--summary]
 * Debug: SEQRENDER_GAPTEST=1 (a LOADBUFF from the unmapped page after the image: badAddr, not a fault),
 *        SEQRENDER_TRACE0=1 (player 0's channels per step, first 3 s), SEQRENDER_SFXTAB=font (that font's
 *        drums, at the end), SEQRENDER_DEBUG=1 (command ring), SEQRENDER_TRACE=1 (player 1 per step, channels / layers /
 *        notes once a second), SEQRENDER_DUMP=file (player 1's first 0x2000 sequence bytes).
 */
#define _GNU_SOURCE /* REG_EIP */
#include "port_libc.h"
#include "host.h"
#include "ultra/ultra.h"
#include "audio/port_audio.h"
#include "include/port_game.h"
#include <signal.h>
#include <ucontext.h>
#include <sys/mman.h>

/* game side (libgame.a), C linkage */
extern int fs_gdlFileInfo(int sysLib, int name, int out);
extern void snd_Init(int seqRom, int fontRom, int sampleRom);
extern void snd_PlayPlayer0Seq0(void);
extern void snd_PlayPlayer1SeqDefault(int seqId);
extern void snd_FrameUpdate(void);
extern void snd_seqEventDbg_00000148(int mode, int sub, int arg);
extern int snd_PostEvent(int ev);
extern void snd_dispatchEvent(int sel, int v);
extern void snd_StopAudioCmds(int spec); /* AudioThread_ResetAudioHeap(spec) + 0xF8000000 (K&R) */
extern void snd_SetPlayer1VolumeLevel(int level);
extern void snd_SetPlayer0VolumeLevel(int level);
extern void snd_SetPlayer2VolumeLevel(int level);
extern void snd_SetSoundOutputMode(int mode);
extern void snd_ApplyPlayer1VolumeLevel(void);
extern void snd_ApplyPlayer0VolumeLevel(void);
extern void snd_ApplyPlayer2VolumeLevel(void);
extern void port_uso_init(void);

/* gAudioCtx = bootup Bss 0x345C0 (port/gen/binds.tsv: AudioLoad_Init's D_00000000). Offsets:
 * include/audio.h (asserted). */
#define ACTX (port_bootup_bss + 0x345C0)
#define ACTX_SEQTABLE 0x201C      /* AudioTable *sequenceTable */
#define ACTX_NUMSEQS 0x202C       /* s16 numSequences */
#define ACTX_TEMPO_I2E 0x2074     /* s16 tempoInternalToExternal */
#define ACTX_TICKS 0x2040         /* s16 ticksPerUpdate (updates per frame) */
#define ACTX_SEQPLAYERS 0x2D00    /* SequencePlayer[4], 0x160 each */
#define ACTX_SEQTEMPPOOL 0x22D0   /* seqCache.temporary.pool (0x21F8 + 0xD4 + 4): start, cur, size */
#define SP_SIZE 0x160
#define SP_SEQID 0x04
#define SP_TEMPO 0x08
#define SP_TEMPOACC 0x0A
#define SP_DELAY 0x10
#define SP_SEQDATA 0x18
#define SP_FADESCALE 0x2C        /* fadeVolumeScale (AUDIOCMD 0x41) */
#define SP_CHANNELS 0x38
#define SP_PC 0x78                /* scriptState.pc */
#define SP_SCRIPTCOUNTER 0xE0     /* +1 per ProcessSequence (one per driver tick) */
#define SP_IO 0x158
#define BOOTUP_TATUMS 0x1CDD2     /* gAudioHeapInitSizes.tatumsPerBeat (bootup Data, s16) */
/* the music state (bootup Data, native u32 under PORT; plan 15.8) */
#define MS_MODE 0x1C0A8           /* last snd_HandleSeqEvent mode */
#define MS_CMD 0x1C0AC            /* music command word: the sequence event 0x11 schedules */
#define MS_SUB 0x1C0B0            /* last mode-4 sub (the course) */
#define MS_GAMETYPE 0x1C0B4       /* snd_dispatchEvent case 12 (title menu) */
#define MS_TIMER 0x1C080          /* snd_UpdateTimers' sequence timer (frames) */
#define MS_TIMERSEQ 0x1C0B8       /* the sequence it starts */

#define OUT_RATE 32000
#define MAX_REQ 24

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

/* FNV-1a 32 over the PCM as little-endian bytes (the WAV's data chunk) */
static u32 pcm_hash(const s16 *pcm, u32 n) {
    u32 h = 2166136261u, i;
    for (i = 0; i < n; i++) {
        u16 v = (u16)pcm[i];
        h = (h ^ (v & 0xFF)) * 16777619u;
        h = (h ^ (v >> 8)) * 16777619u;
    }
    return h;
}

/* A DAC draining 32006 / 60 frames per retrace (osViClock / dacRate, plan 10.2), so the
 * driver's osAiGetLength feedback picks its buffer lengths as on hardware (headless reports 0:
 * every buffer would be the 560-frame maximum and the output 5 % longer than the game time). */

static host_u32 ai_buffered(void) {
    return (host_u32)sAiLevel * 4;
}

static void check_notes(const void *cmds, unsigned int bytes) { /* SEQRENDER_NOTES: enabled notes + the list */
    u8 *notes = *(u8 **)(port_bootup_bss + 0x345C0 + 0x2CFC);
    int k, n = *(int *)(port_bootup_bss + 0x345C0 + 0x2070);
    for (k = 0; k < n && notes != NULL; k++) {
        u8 *nt = notes + 0xD0 * k, *ts, *sm;
        if ((*(u32 *)(nt + 0xB0) & 1) == 0) continue;
        ts = *(u8 **)(nt + 0xB0 + 0x10);
        sm = ts != NULL ? *(u8 **)ts : NULL;
        host_log("note %d: ss %08X ts %p sample %p bits %08X addr %08X loop %p book %p pos %u", k, *(u32 *)(nt + 0xB0),
                 (void *)ts, (void *)sm, sm ? *(u32 *)sm : 0, sm ? *(u32 *)(sm + 4) : 0, sm ? *(void **)(sm + 8) : NULL,
                 sm ? *(void **)(sm + 12) : NULL, *(u32 *)(nt + 0x10 + 4));
    }
    for (k = 0; k < (int)(bytes / 8) && cmds != NULL; k++) {
        const u32 *w = (const u32 *)cmds + 2 * k;
        host_log("cmd %3d: %08X %08X", k, w[0], w[1]);
    }
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

/* a crash names its eip (addr2line -e seqrender) and the step */
static volatile u32 sStep;
static u8 *sWatch; /* SEQRENDER_WATCH: the page of gAudioCtx's head, read-only; writers are logged */
static void on_segv(int sig, siginfo_t *si, void *uc) {
    ucontext_t *u = (ucontext_t *)uc;
    if (sWatch != NULL && (u8 *)si->si_addr >= sWatch && (u8 *)si->si_addr < sWatch + 4096) {
        int off = (int)((u8 *)si->si_addr - (u8 *)(port_bootup_bss + 0x345C0));
        static int logged;
        if (off >= 0 && off < 0x430 && logged < 20) {
            logged++;
            fprintf(stderr, "seqrender: watch: write to ctx%+d at step %u, eip 0x%08X\n", off, sStep,
                    (unsigned)u->uc_mcontext.gregs[REG_EIP]);
            if (logged == 1) { /* return-address candidates on the stack (text of the executable) */
                extern char __executable_start[], etext[];
                u32 *sp = (u32 *)u->uc_mcontext.gregs[REG_ESP];
                int b, nb = 0;
                for (b = 0; b < 256 && nb < 12; b++) {
                    if (sp[b] >= (u32)__executable_start && sp[b] < (u32)etext) {
                        fprintf(stderr, "  bt 0x%08X\n", sp[b]);
                        nb++;
                    }
                }
            }
        }
        mprotect(sWatch, 4096, PROT_READ | PROT_WRITE);
        u->uc_mcontext.gregs[REG_EFL] |= 0x100; /* single-step, then re-arm in on_trap */
        return;
    }
    fprintf(stderr, "seqrender: SIGSEGV at step %u (%.2f s): address %p, eip 0x%08X\n", sStep, sStep / 30.0,
            si->si_addr, (unsigned)u->uc_mcontext.gregs[REG_EIP]);
    signal(sig, SIG_DFL);
}

static void on_trap(int sig, siginfo_t *si, void *uc) {
    ucontext_t *u = (ucontext_t *)uc;
    (void)sig; (void)si;
    u->uc_mcontext.gregs[REG_EFL] &= ~0x100;
    if (sWatch != NULL) {
        mprotect(sWatch, 4096, PROT_READ);
    }
}

static int rom_file(const char *name) {
    int addr = 0;
    fs_gdlFileInfo((int)(port_bootup_bss + 0x3C740) /* gSysLib */, (int)name, (int)&addr);
    return addr;
}

#define SP(i) (ACTX + ACTX_SEQPLAYERS + (i) * SP_SIZE)
#define RD16(p) (*(u16 *)(p))
#define RD32(p) (*(u32 *)(p))
#define MS(off) (*(s32 *)(port_bootup_data + (off)))
#define SP_ON(p) ((RD32(p) >> 31) | (RD32(p) & 1)) /* enabled: bit 31 on MIPS, bit 0 on x86 */

/* The sequence header's first tempo command (0xDD bpm), walking the setup commands seen in
 * 1080's headers; -1 if a delay / jump / unknown command comes first. */
static int header_tempo(const u8 *sd) {
    u32 pc = 0, n;
    for (n = 0; n < 64 && sd != NULL; n++) {
        u8 c = sd[pc++];
        if (c == 0xDD) return sd[pc];
        if (c == 0xD7 || c == 0xD6 || c == 0xD2 || c == 0xD1 || (c >= 0x90 && c <= 0x9F)) pc += 2;
        else if (c == 0xDB || c == 0xD3 || c == 0xD5 || c == 0xD9 || c == 0xDF || c == 0xDE || c == 0xF1 ||
                 c == 0xD0 || c == 0xCC || c == 0xDC) pc += 1;
        else if (c == 0xDA) pc += 3;
        else if (c == 0xD4 || c == 0xF0 || c == 0xC6 || (c & 0xF0) == 0x70 || (c & 0xF0) == 0x80 || c < 0x10) continue;
        else return -1;
    }
    return -1;
}

typedef struct {
    int kind; /* 0 event, 1 post, 2 sfx */
    int a, b;
    u32 step;
} Req;

static void usage(void) {
    fprintf(stderr, "usage: seqrender [--rom PATH] (--seq N | --event M[:S][@SEC] | --post K[:V][@SEC] | --sfx K | "
                    "--course N [--gametype G] | --title [--title-at SEC])... [--spec N]\n"
                    "                 [--seconds T] [--out FILE.wav] [--sfx-at SEC] [--no-sfx-player] [--tatums N]\n"
                    "                 [--levels M,S,V | --raw] [--quiet] [--summary]\n");
}

static void parse_req(Req *r, int kind, const char *v) {
    const char *c = strchr(v, ':'), *t = strchr(v, '@');
    r->kind = kind;
    r->a = (int)strtol(v, NULL, 0);
    r->b = c != NULL ? (int)strtol(c + 1, NULL, 0) : 0;
    r->step = (u32)((t != NULL ? strtod(t + 1, NULL) : 0.0) * 30.0 + 0.5);
}

static void dump_sfxtab(void) { /* SEQRENDER_SFXTAB=font: that font's drums (layer instrument 0x7F) */
    int f = atoi(getenv("SEQRENDER_SFXTAB")), k;
    u8 *font = *(u8 **)(ACTX + 0x2030) + 0x14 * f;
    u8 **drums = *(u8 ***)(font + 0xC);
    host_log("sfxtab: font %d numDrums %u numSfx %u (fontLoadStatus %u)", f, font[1], RD16(font + 4), ACTX[0x2C40 + f]);
    for (k = 0; drums != NULL && k < font[1]; k++) {
        u8 *dr = drums[k], *sm = dr != NULL ? *(u8 **)(dr + 4) : NULL;
        host_log("sfxtab: drum %3d (0x%02X): %p sample %p tuning %.4f size %u", k, k, (void *)dr, (void *)sm,
                 dr != NULL ? *(f32 *)(dr + 8) : 0.0f, sm != NULL ? RD32(sm) & 0xFFFFFF : 0);
    }
}

int main(int argc, char **argv) {
    const char *romPath = "baserom.z64", *outPath = "seqrender.wav";
    Req req[MAX_REQ];
    int nReq = 0;
    int list = 0, spec = -1, seq = -1, sfx = -1, sfxPlayer = 1, quiet = 0, tatums = 0, course = -1, gametype = -1, summary = 0;
    int raw = 0, title = 0, levels[3] = { 8, 8, 8 }; /* Backup_resetOptionsData's levels[0..2] */
    double seconds = 30.0, sfxAt = 1.0, titleAt = 7.4;
    PortUltraConfig uc;
    HostConfig hc;
    void *rom;
    u32 romSize, steps, step, i;
    int seqRom, bankRom, waveRom;
    int trace = getenv("SEQRENDER_TRACE") != NULL, trace0 = getenv("SEQRENDER_TRACE0") != NULL;
    /* player 1 bookkeeping */
    u32 prevOn = 0, prevId = 0xFFFF, starts = 0, lastId = 0xFFFF;
    double startAt = -1.0, stopAt = -1.0;
    u32 prevCounter = 0, prevAcc = 0, tatumsRun = 0, ticksRun = 0, tempoMin = 0xFFFF, tempoMax = 0;
    int tracking = 0, hdrTempo = -1;
    double tempoSum = 0;

    for (i = 1; i < (u32)argc; i++) {
        const char *a = argv[i], *v = i + 1 < (u32)argc ? argv[i + 1] : NULL;
        if (strcmp(a, "--no-sfx-player") == 0) { sfxPlayer = 0; continue; }
        if (strcmp(a, "--quiet") == 0) { quiet = 1; continue; }
        if (strcmp(a, "--summary") == 0) { summary = 1; continue; }
        if (strcmp(a, "--list") == 0) { list = 1; continue; }
        if (strcmp(a, "--raw") == 0) { raw = 1; continue; }
        if (strcmp(a, "--title") == 0) { title = 1; continue; }
        if (v == NULL) { usage(); return 2; }
        i++;
        if (strcmp(a, "--rom") == 0) romPath = v;
        else if (strcmp(a, "--out") == 0) outPath = v;
        else if (strcmp(a, "--seq") == 0) seq = (int)strtol(v, NULL, 0);
        else if (strcmp(a, "--event") == 0 && nReq < MAX_REQ) parse_req(&req[nReq++], 0, v);
        else if (strcmp(a, "--post") == 0 && nReq < MAX_REQ) parse_req(&req[nReq++], 1, v);
        else if (strcmp(a, "--course") == 0) course = (int)strtol(v, NULL, 0);
        else if (strcmp(a, "--gametype") == 0) gametype = (int)strtol(v, NULL, 0);
        else if (strcmp(a, "--spec") == 0) spec = (int)strtol(v, NULL, 0);
        else if (strcmp(a, "--seconds") == 0) seconds = strtod(v, NULL);
        else if (strcmp(a, "--sfx") == 0) sfx = (int)strtol(v, NULL, 0);
        else if (strcmp(a, "--sfx-at") == 0) sfxAt = strtod(v, NULL);
        else if (strcmp(a, "--tatums") == 0) tatums = (int)strtol(v, NULL, 0);
        else if (strcmp(a, "--title-at") == 0) titleAt = strtod(v, NULL);
        else if (strcmp(a, "--levels") == 0) sscanf(v, "%d,%d,%d", &levels[0], &levels[1], &levels[2]);
        else { usage(); return 2; }
    }
    if (sfx >= 0 && nReq < MAX_REQ) {
        req[nReq].kind = 2;
        req[nReq].a = sfx;
        req[nReq].b = 0;
        req[nReq].step = (u32)(sfxAt * 30.0 + 0.5);
        nReq++;
    }
    if (title && nReq + 2 <= MAX_REQ) { /* n64proc, then titproc (see the header) */
        req[nReq].kind = 0; req[nReq].a = 1; req[nReq].b = 0; req[nReq].step = 0;
        nReq++;
        req[nReq].kind = 0; req[nReq].a = 0; req[nReq].b = 0; req[nReq].step = (u32)(titleAt * 30.0 + 0.5);
        nReq++;
    }
    if (course >= 0) { /* the race start (see the header) */
        static const struct { int kind, a, b; u32 step; } race[] = {
            { 0, 4, 0, 0 },                                      /* timproc: snd_playSeq(4, course) */
            { 1, 0x12, 0, 120 + 1 }, { 1, 0x10, 0, 120 + 1 },     /* "3" */
            { 1, 0x12, 0, 120 + 21 },                            /* "2" */
            { 1, 0x12, 0, 120 + 41 },                            /* "1" */
            { 1, 0x11, 0, 120 + 61 },                            /* "GO": schedules the course music */
        };
        if (gametype >= 0 && nReq < MAX_REQ) {
            req[nReq].kind = 1; req[nReq].a = 12; req[nReq].b = gametype; req[nReq].step = 0;
            nReq++;
        }
        for (i = 0; i < sizeof(race) / sizeof(race[0]) && nReq < MAX_REQ; i++) {
            req[nReq].kind = race[i].kind;
            req[nReq].a = race[i].a;
            req[nReq].b = race[i].kind == 0 ? course : race[i].b;
            req[nReq].step = race[i].step;
            nReq++;
        }
    }
    if (seq < 0 && nReq == 0 && !list) {
        usage();
        return 2;
    }

    {
        struct sigaction sa;
        memset(&sa, 0, sizeof(sa));
        sa.sa_sigaction = on_segv;
        sa.sa_flags = SA_SIGINFO;
        sigaction(SIGSEGV, &sa, NULL);
        sa.sa_sigaction = on_trap;
        sigaction(SIGTRAP, &sa, NULL);
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
    if (getenv("SEQRENDER_NOTES") != NULL) {
        port_audio_premix_hook = check_notes;
    }
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
        host_log("seqrender: AudioLoad_Init done: ticksPerUpdate %d, tempoInternalToExternal %d, tatumsPerBeat %d, "
                 "%d sequences",
                 (s16)RD16(ACTX + ACTX_TICKS), (s16)RD16(ACTX + ACTX_TEMPO_I2E),
                 *(s16 *)(port_bootup_data + BOOTUP_TATUMS), (s16)RD16(ACTX + ACTX_NUMSEQS));
    }
    if (list) { /* the sequence table (entries swapped to native by port_audio_swap_static) */
        u8 *tbl = *(u8 **)(ACTX + ACTX_SEQTABLE);
        int k, nseq = (s16)RD16(ACTX + ACTX_NUMSEQS);
        for (k = 0; k < nseq && tbl != NULL; k++) {
            u8 *e = tbl + 0x10 + 0x10 * k;
            u32 ra = RD32(e), sz = RD32(e + 4), j;
            const u8 *d = (const u8 *)rom + ra; /* AudioLoad_InitTable made the entries absolute */
            char hex[3 * 12 + 1];
            if (sz == 0) { /* OoT's table alias: romAddr is the sequence it plays */
                printf("seq %2d = seq %u\n", k, ra);
                continue;
            }
            for (j = 0; j < 12; j++) {
                sprintf(hex + 3 * j, "%02X ", ra + j < romSize ? d[j] : 0);
            }
            printf("seq %2d rom 0x%06X size 0x%05X medium %d cache %d header %s tempo %d\n", k, ra, sz, (s8)e[8],
                   (s8)e[9], hex, header_tempo(d));
        }
        return 0;
    }
    /* let the reset / init settle (the game runs App_ctor .. app_runInit before its first sound) */
    for (step = 0; step < 4; step++) {
        snd_FrameUpdate();
        audio_retrace();
        audio_retrace();
    }
    if (!raw) { /* GameFlow_create: the options' volume levels and sound mode */
        snd_SetPlayer1VolumeLevel(levels[0]);
        snd_SetPlayer0VolumeLevel(levels[1]);
        snd_SetPlayer2VolumeLevel(levels[2]);
        snd_SetSoundOutputMode(0);
    }
    if (sfxPlayer) {
        snd_PlayPlayer0Seq0(); /* app_runInit */
    }
    if (spec >= 0) { /* snd_HandleSeqEvent's spec change (mode 4: 3 / 4 / 6, mode 1: 0), then let it finish */
        snd_StopAudioCmds(spec);
        for (step = 0; step < 30; step++) {
            snd_FrameUpdate();
            audio_retrace();
            audio_retrace();
        }
        if (sfxPlayer) {
            snd_PlayPlayer0Seq0();
        }
        if (!raw) { /* the spec change re-initialises the players (fadeVolumeScale 1.0): the game's
                     * snd_HandleSeqEvent applies the levels again after it (its flag == 1 tail) */
            snd_ApplyPlayer1VolumeLevel();
            snd_ApplyPlayer0VolumeLevel();
            snd_ApplyPlayer2VolumeLevel();
        }
    }
    if (seq >= 0) {
        u8 *tbl = *(u8 **)(ACTX + ACTX_SEQTABLE);
        u32 tmp = RD32(ACTX + ACTX_SEQTEMPPOOL + 8), sz = 0;
        if (tbl != NULL && seq < (s16)RD16(ACTX + ACTX_NUMSEQS)) {
            u8 *e = tbl + 0x10 + 0x10 * seq;
            sz = RD32(e + 4) == 0 ? RD32(tbl + 0x10 + 0x10 * RD32(e) + 4) : RD32(e + 4); /* alias: size 0 */
        }
        if (sz > tmp) { /* the temporary sequence cache has no bounds check: the load runs into gAudioCtx */
            host_log("seqrender: WARNING: seq %d is 0x%X bytes, this spec's temporary sequence cache 0x%X: the load "
                     "overruns the heap into gAudioCtx (as it would on the N64); use --spec 3", seq, sz, tmp);
        }
        snd_PlayPlayer1SeqDefault(seq);
    }
    if (getenv("SEQRENDER_GAPTEST") != NULL) { /* port_audio's valid-address test: the page after the image's
                                                  * end (_end, before the brk heap) must count, not fault */
        extern char _end[];
        u32 gap = (((u32)_end + 0xFFF) & ~0xFFFu) + 0x1000, cmds[4], before = port_audio_stats.badAddr;
        cmds[0] = (20u << 24) | (0x100u << 12) | 0x400; /* LOADBUFF 0x100 bytes to DMEM 0x400 from the gap (K0 form) */
        cmds[1] = gap - 0x80000000u;
        cmds[2] = (20u << 24) | (0x100u << 12) | 0x400; /* the same address raw */
        cmds[3] = gap;
        port_audio_run_list(cmds, sizeof(cmds));
        host_log("seqrender: gap test: _end %p, address 0x%08X: badAddr +%u (expect 2; 0 only if the brk heap "
                 "starts right after the image)", (void *)_end, gap, port_audio_stats.badAddr - before);
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
    if (getenv("SEQRENDER_WATCH") != NULL) {
        sWatch = (u8 *)((unsigned long)(port_bootup_bss + 0x345C0 + 0x18) & ~4095UL);
        mprotect(sWatch, 4096, PROT_READ);
    }
    sFrames = 0; /* the WAV starts at the request */
    sUnderruns = 0;

    steps = (u32)(seconds * 30.0 + 0.5);
    for (step = 0; step < steps; step++) {
        u8 *p1 = SP(1);
        int k;
        sStep = step;
        for (k = 0; k < nReq; k++) {
            Req *r = &req[k];
            if (step != r->step) {
                continue;
            }
            if (r->kind == 0) {
                snd_seqEventDbg_00000148(r->a, r->b, 0);
            } else if (r->kind == 1) {
                snd_dispatchEvent(r->a, r->b);
            } else {
                snd_PostEvent(r->a);
            }
            if (!quiet) {
                host_log("seqrender: %.2f s: %s %d:%d -> mode %d sub %d cmd %d gametype %d; timer %d seq %d",
                         step / 30.0, r->kind == 0 ? "event" : r->kind == 1 ? "post" : "sfx", r->a, r->b,
                         MS(MS_MODE), MS(MS_SUB), MS(MS_CMD), MS(MS_GAMETYPE), MS(MS_TIMER), MS(MS_TIMERSEQ));
            }
        }
        snd_FrameUpdate();
        audio_retrace();
        audio_retrace();

        if (trace0 && step < 90) { /* SEQRENDER_TRACE0: player 0 (sound effects) channels, per step */
            u8 *p0 = SP(0), *sd = *(u8 **)(p0 + SP_SEQDATA);
            for (k = 0; k < 16; k++) {
                u8 *ch = *(u8 **)(p0 + SP_CHANNELS + 4 * k);
                if (ch == NULL || SP_ON(ch) == 0) continue;
                host_log("trace0 %u ch%d: pc +0x%X delay %u value %d depth %u font %u io %d %d %d %d %d %d %d %d "
                         "filter %p layers %p %p", step, k, sd ? RD32(ch + 0x70) - (u32)sd : 0, RD16(ch + 0x1E), (s8)ch[0x89],
                         ch[0x88], ch[7], (s8)ch[0xD4], (s8)ch[0xD5], (s8)ch[0xD6], (s8)ch[0xD7], (s8)ch[0xD8],
                         (s8)ch[0xD9], (s8)ch[0xDA], (s8)ch[0xDB], *(void **)(ch + 0xDC), *(void **)(ch + 0x50), *(void **)(ch + 0x54));
            }
        }

        { /* player 1's starts / stops */
            u32 on = SP_ON(p1), id = p1[SP_SEQID];
            if (on != prevOn || (on && id != prevId)) {
                if (!quiet) {
                    host_log("seqrender: %.2f s: player 1 %s seq %u", (step + 1) / 30.0, on ? "plays" : "stopped", id);
                }
                if (on) {
                    starts++;
                    lastId = id;
                    startAt = (step + 1) / 30.0;
                    stopAt = -1.0;
                    hdrTempo = header_tempo(*(u8 **)(p1 + SP_SEQDATA));
                } else {
                    stopAt = (step + 1) / 30.0;
                }
            }
            prevOn = on;
            prevId = id;
            if (trace && on) {
                u8 *sd = *(u8 **)(p1 + SP_SEQDATA);
                host_log("trace %u: pc +0x%X delay %u io %02X %02X %02X %02X %02X %02X %02X %02X", step,
                         sd ? RD32(p1 + SP_PC) - (u32)sd : 0, RD16(p1 + SP_DELAY), p1[SP_IO], p1[SP_IO + 1],
                         p1[SP_IO + 2], p1[SP_IO + 3], p1[SP_IO + 4], p1[SP_IO + 5], p1[SP_IO + 6], p1[SP_IO + 7]);
                if (step % 30 == 0) {
                    for (k = 0; k < 16; k++) {
                        u8 *ch = *(u8 **)(p1 + SP_CHANNELS + 4 * k), *ly, *nt;
                        if (ch == NULL || SP_ON(ch) == 0) continue;
                        ly = *(u8 **)(ch + 0x50);
                        nt = ly ? *(u8 **)(ly + 0x2C) : NULL;
                        host_log("trace   ch%d font %u instOrWave %d vol %.3f layer0 %p vel %.3f freq %.3f note %p "
                                 "(sample state %08X vol %u/%u rate %u)",
                                 k, ch[7], *(s16 *)(ch + 0x24), *(f32 *)(ch + 0x2C), (void *)ly,
                                 ly ? *(f32 *)(ly + 0x40) : 0.0f, ly ? *(f32 *)(ly + 0x44) : 0.0f, (void *)nt,
                                 nt ? RD32(nt + 0xB0) : 0, nt ? RD16(nt + 0xB8) : 0, nt ? RD16(nt + 0xBA) : 0,
                                 nt ? RD16(nt + 0xBC) : 0);
                    }
                }
            }
        }

        /* tatums: one ProcessSequence per driver tick (scriptCounter + 1) adds tempo to tempoAcc;
         * a tatum each time it passes tempoInternalToExternal */
        if (SP_ON(p1) == 0) {
            tracking = 0;
            continue;
        }
        {
            u32 counter = RD32(p1 + SP_SCRIPTCOUNTER), acc = RD16(p1 + SP_TEMPOACC), tempo = RD16(p1 + SP_TEMPO);
            s32 i2e = (s16)RD16(ACTX + ACTX_TEMPO_I2E);
            if (tracking && counter > prevCounter && i2e > 0) {
                u32 ticks = counter - prevCounter;
                s32 adv = (s32)(prevAcc + ticks * tempo) - (s32)acc;
                tatumsRun += adv > 0 ? (u32)((adv + i2e / 2) / i2e) : 0;
                ticksRun += ticks;
                tempoSum += (double)tempo * ticks;
                if (tempo < tempoMin) tempoMin = tempo;
                if (tempo > tempoMax) tempoMax = tempo;
            }
            prevCounter = counter;
            prevAcc = acc;
            tracking = 1;
        }
    }

    if (getenv("SEQRENDER_DEBUG") != NULL) {
        u8 *c = ACTX;
        host_log("dbg end: cmd write %u read %u finished %u; procQ count %d", c[0x53B8], c[0x53B9], c[0x53BA],
                 *(int *)(c + 0x53E8 + 8));
    }
    if (getenv("SEQRENDER_DUMP") != NULL && *(u8 **)(SP(1) + SP_SEQDATA) != NULL) {
        FILE *f = fopen(getenv("SEQRENDER_DUMP"), "wb");
        if (f != NULL) {
            fwrite(*(u8 **)(SP(1) + SP_SEQDATA), 1, 0x2000, f);
            fclose(f);
        }
    }
    /* ---- report ---- */
    {
        u32 n = sFrames * 2, clip = 0, peak = 0, lastLoud = 0, firstLoud = 0xFFFFFFFF, hash;
        double ss = 0, rms, tps, bpm, meanTempo, tickRate;
        s32 i2e = (s16)RD16(ACTX + ACTX_TEMPO_I2E);
        for (i = 0; i < n; i++) {
            int s = sPcm[i], a = s < 0 ? -s : s;
            if (a >= 32767) clip++;
            if ((u32)a > peak) peak = a;
            ss += (double)s * s;
            if (a > 64) { lastLoud = i / 2; if (firstLoud == 0xFFFFFFFF) firstLoud = i / 2; }
        }
        rms = n ? sqrt(ss / n) : 0.0;
        hash = pcm_hash(sPcm, n);
        /* s16 output: a NaN in the driver shows up as silence or a clip storm */
        meanTempo = ticksRun ? tempoSum / ticksRun : 0.0;
        tickRate = 60.0 * (s16)RD16(ACTX + ACTX_TICKS); /* driver ticks per second of game time */
        tps = ticksRun ? tatumsRun * tickRate / ticksRun : 0.0;
        bpm = tps * 60.0 / 48.0;
        host_log("seqrender: %u AI buffers, %u frames = %.2f s at %u Hz; peak %u, RMS %.1f, clipped %u (%.4f%%), "
                 "first sound %.3f s, last %.3f s; pcm hash %08X",
                 sBuffers, sFrames, (double)sFrames / OUT_RATE, OUT_RATE, peak, rms, clip,
                 n ? 100.0 * clip / n : 0.0, firstLoud == 0xFFFFFFFF ? -1.0 : (double)firstLoud / OUT_RATE,
                 (double)lastLoud / OUT_RATE, hash);
        host_log("seqrender: players: 0 word %08X seq %u, 1 word %08X seq %u; fadeVolumeScale %.3f / %.3f / %.3f; "
                 "AI underruns %u",
                 RD32(SP(0)), SP(0)[SP_SEQID], RD32(SP(1)), SP(1)[SP_SEQID], *(f32 *)(SP(0) + SP_FADESCALE),
                 *(f32 *)(SP(1) + SP_FADESCALE), *(f32 *)(SP(2) + SP_FADESCALE), sUnderruns);
        if (getenv("SEQRENDER_SFXTAB") != NULL) {
            dump_sfxtab();
        }
        { /* the synthesis reverbs (gAudioCtx +0x18, 0x158 each; include/audio.h SynthesisReverb) */
            int k, nr = (s8)ACTX[1];
            for (k = 0; k < nr && k < 3; k++) {
                u8 *r = ACTX + 0x18 + 0x158 * k;
                host_log("seqrender: reverb %d: use %u downsample %u window %u volume %d decay %u leak %u / %u "
                         "resampleFlags %u unk_05 %d", k, r[1], r[4], RD16(r + 6), (s16)RD16(r + 0xA), RD16(r + 0xC),
                         RD16(r + 0x10), RD16(r + 0x12), r[0], (s8)r[5]);
            }
        }
        host_log("seqrender: mixer: %u updates, %u tasks, %u cmds, badAddr %u, unknownOps %u, unk19 %u, "
                 "raw-pointer addresses %u",
                 port_audio_stats.updates, port_audio_stats.tasks, port_audio_stats.cmds,
                 port_audio_stats.badAddr, port_audio_stats.unknownOps, port_audio_stats.unk19,
                 port_audio_stats.rawPointers);
        host_log("seqrender: player 1: seq %d started %u x (last at %.2f s, stopped %.2f s); header tempo %d bpm; "
                 "player tempo min %u max %u mean %.1f (= %.1f bpm), tempoInternalToExternal %d; "
                 "%u ticks (%.0f / s), %u tatums = %.1f tatums/s = %.1f bpm",
                 lastId == 0xFFFF ? -1 : (int)lastId, starts, startAt, stopAt, hdrTempo,
                 tempoMin == 0xFFFF ? 0 : tempoMin, tempoMax, meanTempo, meanTempo / 48.0, i2e, ticksRun, tickRate,
                 tatumsRun, tps, bpm);
        if (summary) {
            printf("RESULT seq=%d starts=%u start=%.2f stop=%.2f frames=%u peak=%u rms=%.1f clip=%u first=%.3f last=%.3f "
                   "hdr_bpm=%d i2e=%d tempo_min=%u tempo_max=%u bpm=%.2f badaddr=%u unknownops=%u unk19=%u underruns=%u "
                   "hash=%08X\n",
                   lastId == 0xFFFF ? -1 : (int)lastId, starts, startAt, stopAt, sFrames, peak, rms, clip,
                   firstLoud == 0xFFFFFFFF ? -1.0 : (double)firstLoud / OUT_RATE, (double)lastLoud / OUT_RATE,
                   hdrTempo, i2e, tempoMin == 0xFFFF ? 0 : tempoMin, tempoMax, bpm, port_audio_stats.badAddr,
                   port_audio_stats.unknownOps, port_audio_stats.unk19, sUnderruns, hash);
        }
    }
    if (outPath[0] != '\0' && strcmp(outPath, "-") != 0 && write_wav(outPath, sPcm, sFrames, OUT_RATE) != 0) {
        host_fatal("seqrender: cannot write %s", outPath);
    }
    if (!quiet) {
        host_log("seqrender: wrote %s", outPath);
    }
    return 0;
}
