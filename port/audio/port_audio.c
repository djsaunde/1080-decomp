/*
 * port/audio/port_audio.c: drives the matched audio driver plus the CPU mixer
 * in place of the RcpMgr thread and the RSP (docs/1080_PC_PORT_PLAN.md 15.6).
 *
 * N64: on every retrace RcpMgr_service starts the audio task built on the
 * previous retrace, then calls snd_AudioUpdate -> AudioThread_UpdateImpl,
 * which builds the next one. The task's output is the AI buffer of that
 * update, which AudioThread_UpdateImpl queues two updates later
 * ((curAiBufIndex + 1) % 3), so running the list straight after it is built
 * gives the same samples: the CPU never reads the synthesised data, and every
 * DRAM state the list writes (ADPCM / resample / filter states, reverb rings)
 * is only read by the next list.
 *
 * Addresses: the builders store OS_K0_TO_PHYSICAL(p) = p - 0x80000000 (u32).
 * On the 32-bit port the host pointer is that + 0x80000000 (mod 2^32). A few
 * builders pass a pointer as it is (translate_k0).
 */
#include <stdint.h>
#include "../port_libc.h" /* memcpy */
#include "mixer.h"
#include "audio_swap.h"
#include "port_audio.h"
#include "../ultra/ultra_int.h" /* port_ultra_cfg.rom (the in-memory ROM image) */
#include "../host.h"            /* host_headless_set_audio_sink / _clock (the --audio-out capture) */

PortAudioStats port_audio_stats;

static Mixer sMixer;
static int sMixerReady;

/* src/game_libs/game_libs_o2_26214.c (the donor body the port compiles). */
extern void *AudioThread_UpdateImpl(void);
/* bootup.uso Data, writable copy (port/gen/uso_modules.c). */
extern uint8_t port_bootup_data[];

/* The port binary's image (non-PIE, i386: .text / .data / .bss, where bootup.uso's blobs, the
 * audio heap and every buffer the driver hands the RSP live) up to the current break (anything
 * malloc'd). */
extern char __executable_start[];
extern char _end[];
extern void *sbrk(intptr_t increment);

/* The mapped ranges, not the span __executable_start .. sbrk(0): between the image's end (_end) and
 * the start of the brk heap there is an unmapped gap (randomised, up to 32 MB on i386), and a
 * garbage command-list address that lands there must be counted as badAddr, not dereferenced.
 * Read once from /proc/self/maps: every mapping inside [__executable_start, _end) (text, rodata,
 * data, bss; a hole between segments stays out) and the [heap] start; the heap's top is sbrk(0)
 * at the time of the test.  Without /proc: [__executable_start, _end) and [_end, sbrk(0)), the
 * old span. */
#define MAX_IMAGE_RANGES 16
static uint32_t sImageLo[MAX_IMAGE_RANGES], sImageHi[MAX_IMAGE_RANGES];
static int sNumImage = -1; /* -1: not read yet */
static uint32_t sHeapLo;

static void read_maps(void) {
    uint32_t lo = (uint32_t)(uintptr_t)__executable_start, hi = (uint32_t)(uintptr_t)_end;
    FILE *f = fopen("/proc/self/maps", "r");
    char line[256];
    sNumImage = 0;
    sHeapLo = 0;
    if (f != NULL) {
        while (fgets(line, sizeof(line), f) != NULL) {
            char *e;
            unsigned long a = strtoul(line, &e, 16), b = *e == '-' ? strtoul(e + 1, NULL, 16) : 0;
            size_t n = strlen(line);
            while (n > 0 && (line[n - 1] == '\n' || line[n - 1] == ' ')) {
                n--;
            }
            if (b <= a) {
                continue;
            }
            if (n >= 6 && strncmp(line + n - 6, "[heap]", 6) == 0) {
                /* the LOWEST [heap] line (run 869): without address randomisation (setarch -R, gdb, the
                 * 64-bit build's re-exec in host/main64.c) brk starts right at the page-aligned _end and
                 * the kernel labels the .bss mapping [heap] too, so two [heap] lines follow each other;
                 * keeping the last one left the whole .bss (the audio context, the sound banks) out:
                 * every relocation test failed and the driver loaded no samples (silence) */
                if (sHeapLo == 0 || (uint32_t)a < sHeapLo) {
                    sHeapLo = (uint32_t)a;
                }
            } else if (a < hi && b > lo && sNumImage < MAX_IMAGE_RANGES) {
                sImageLo[sNumImage] = a < lo ? lo : (uint32_t)a;
                sImageHi[sNumImage] = b > hi ? hi : (uint32_t)b;
                sNumImage++;
            }
        }
        fclose(f);
    }
    if (sNumImage == 0) { /* no /proc: the whole image */
        sImageLo[0] = lo;
        sImageHi[0] = hi;
        sNumImage = 1;
    }
    if (sHeapLo == 0) {
        sHeapLo = hi;
    }
}

static int host_mapped(uint32_t p) {
    int i;
    if (sNumImage < 0) {
        read_maps();
    }
    for (i = 0; i < sNumImage; i++) {
        if (p >= sImageLo[i] && p < sImageHi[i]) {
            return 1;
        }
    }
    if (sHeapLo == (uint32_t)(uintptr_t)_end) { /* no [heap] line yet (nothing malloc'd when read): look again */
        uint32_t top = (uint32_t)(uintptr_t)sbrk(0);
        if (top > sHeapLo && p >= sHeapLo && p < top) {
            read_maps();
        }
    }
    return p >= sHeapLo && p < (uint32_t)(uintptr_t)sbrk(0);
}

/* Most builders store OS_K0_TO_PHYSICAL(p) = p - 0x80000000, but some pass the pointer as it is
 * (the FILTER words of AudioSynth_ProcessNote / AudioSynth_FilterReverb, as OoT's aFilter does):
 * the RSP's DMA keeps only the low 24 bits, so on the N64 both spellings reach the same RDRAM.
 * Here: a value that is already a host address is used as it is, else + 0x80000000. */
static void *translate_k0(uint32_t addr, void *user) {
    uint32_t k0;
    (void)user;
    if (addr == 0) {
        return NULL;
    }
    if (host_mapped(addr)) {
        port_audio_stats.rawPointers++;
        return (void *)(uintptr_t)addr;
    }
    k0 = addr + 0x80000000u;
    if (host_mapped(k0)) {
        return (void *)(uintptr_t)k0;
    }
    return NULL; /* counted as badAddr by the mixer */
}

/* The driver's "is this a relocated pointer" tests (OoT's AUDIO_RELOCATED_ADDRESS_START, K0 on the
 * N64): under PORT game_libs_post.c asks this instead (plan 15.8). */
int port_audio_is_ptr(unsigned int p) {
    return host_mapped(p);
}

void *port_audio_translate(unsigned int addr) {
    return translate_k0(addr, NULL);
}

int (*port_audio_run_hook)(const void *cmds, unsigned int bytes);

static void mixer_ready(void) {
    if (!sMixerReady) {
        mixer_init(&sMixer, translate_k0, NULL);
        sMixerReady = 1;
    }
}

void port_audio_run_list(const void *cmds, unsigned int bytes) {
    uint32_t n0;
    mixer_ready();
    if (port_audio_run_hook != NULL && port_audio_run_hook(cmds, bytes)) {
        port_audio_stats.tasks++;
        return;
    }
    n0 = sMixer.n_cmds;
    audio_abi_run(&sMixer, (const uint32_t *)cmds, (int)(bytes / 8));
    port_audio_stats.tasks++;
    port_audio_stats.cmds += sMixer.n_cmds - n0;
    port_audio_stats.badAddr = sMixer.n_bad_addr;
    port_audio_stats.unknownOps = sMixer.n_unknown;
    port_audio_stats.unk19 = sMixer.n_unk19;
}

void (*port_audio_premix_hook)(const void *cmds, unsigned int bytes); /* debug (seqrender): after the driver's
                                                                         * update, before the mixer */

static void capture_retrace(void);

static void outputs_setup(void);

void port_audio_frame(void) {
    OSTask_t *task;
    static int setup;

    if (!setup) {
        outputs_setup(); /* the live presenter's audio ring, if there is one (plan 19.12) */
        setup = 1;
    }
    capture_retrace();
    port_audio_stats.updates++;
    task = (OSTask_t *)AudioThread_UpdateImpl(); /* AudioTask: OSTask_t first */
    if (port_audio_premix_hook != NULL) {
        port_audio_premix_hook(task != NULL ? task->data_ptr : NULL, task != NULL ? task->data_size : 0);
    }
    if (task != NULL && task->data_ptr != NULL && task->data_size != 0) {
        port_audio_run_list(task->data_ptr, task->data_size);
    }
}

int port_audio_rcp_task_is_audio(const void *t) {
    const OSTask_t *task = (const OSTask_t *)t;
    if (task != NULL && task->type == M_AUDTASK) {
        port_audio_stats.droppedSpTasks++;
        return 1;
    }
    return 0;
}

/* ---- gWaveSamples[8] ---- */

/* game_libs VRAM 0 = ROM 0xDE50D8 (tenshoe.yaml: game_libs 0x1CC at 0xDE52A4);
 * AudioThread_Update = game_libs 0x261F4 (build/tenshoe.map). */
#define NOISE_ROM 0xE0B2CCu
/* audioRandom & 0xFFF0, plus what one update can advance it: each noise note
 * loads <= 0x200 samples and moves the pointer 2 x that (OoT's
 * `+= numSamplesToLoad * SAMPLE_SIZE` on an s16 *), 20 notes. */
#define NOISE_BYTES (0x10000 + 20 * 0x800)
static uint8_t sNoise[NOISE_BYTES];
static int sNoiseReady;

unsigned char *port_audio_noise_base(void) {
    if (!sNoiseReady) {
        uint32_t i;
        if (port_ultra_cfg.rom != NULL && port_ultra_cfg.romSize >= NOISE_ROM + NOISE_BYTES) {
            const uint8_t *src = port_ultra_cfg.rom + NOISE_ROM;
            for (i = 0; i < NOISE_BYTES; i += 2) { /* big-endian s16 -> host s16, same values as on the N64 */
                int16_t v = (int16_t)((src[i] << 8) | src[i + 1]);
                memcpy(sNoise + i, &v, 2);
            }
        } else {
            uint32_t x = 0x1080u;
            for (i = 0; i < NOISE_BYTES; i += 2) {
                int16_t v;
                x = x * 1103515245u + 12345u;
                v = (int16_t)(x >> 16);
                memcpy(sNoise + i, &v, 2);
            }
        }
        sNoiseReady = 1;
    }
    return sNoise;
}

/* ---- byte order ---- */

void port_audio_swap_static(void) {
    static int done;
    if (!done) {
        audio_swap_bootup_data(port_bootup_data);
        done = 1;
    }
}

void port_audio_swap_font(void *font, int numInstruments, int numDrums, int numSfx) {
    audio_swap_font((uint8_t *)font, numInstruments, numDrums, numSfx);
}

/* ---- --audio-out: the game's AI output to a WAV (tenshoe, plan 15.9) ---- */

/* Every AI buffer the driver queues (osAiSetNextBuffer -> host_audio_queue -> the headless sink)
 * is appended to a 16-bit stereo 32000 Hz WAV; the header's sizes are written at close.  It also
 * models the DAC (seqrender's model, plan 15.7): it drains 32006 / 60 frames per retrace and
 * reports the rest through osAiGetLength, so the driver picks its 528..544-frame buffers as on
 * hardware and the WAV runs at the game's rate.  PORT_AUDIO_DAC=0 turns the model off
 * (osAiGetLength stays the headless 0: every buffer is 560 frames, the WAV runs 5 % long).  The music
 * state (snd_HandleSeqEvent's mode word, bootup Data 0x1C0A8, and the command word 0x1C0AC) is
 * logged when it changes, with the retrace time. */
static FILE *sCapFile;
static uint32_t sCapFrames, sCapRetraces, sCapClipped, sCapPeak;
static double sCapLevel, sCapSumSq;
static int32_t sCapMode = 0x7FFFFFFF, sCapCmd = 0x7FFFFFFF;

static void cap_put32(uint8_t *p, uint32_t v) {
    p[0] = (uint8_t)v; p[1] = (uint8_t)(v >> 8); p[2] = (uint8_t)(v >> 16); p[3] = (uint8_t)(v >> 24);
}

static void capture_header(uint32_t frames) {
    uint8_t h[44];
    uint32_t bytes = frames * 4;
    memcpy(h, "RIFF", 4); cap_put32(h + 4, 36 + bytes); memcpy(h + 8, "WAVEfmt ", 8);
    cap_put32(h + 16, 16); cap_put32(h + 20, 1 | (2u << 16)); cap_put32(h + 24, 32000);
    cap_put32(h + 28, 32000 * 4); cap_put32(h + 32, 4 | (16u << 16));
    memcpy(h + 36, "data", 4); cap_put32(h + 40, bytes);
    fseek(sCapFile, 0, SEEK_SET);
    fwrite(h, 1, 44, sCapFile);
    fseek(sCapFile, 0, SEEK_END);
}

static void capture_sink(const void *samples, host_u32 bytes) {
    const int16_t *s = (const int16_t *)samples;
    uint32_t i, n = bytes / 4;
    for (i = 0; i < n * 2; i++) {
        int32_t v = s[i];
        uint8_t b[2];
        uint32_t a = (uint32_t)(v < 0 ? -v : v);
        b[0] = (uint8_t)v;
        b[1] = (uint8_t)((uint32_t)v >> 8);
        fwrite(b, 1, 2, sCapFile);
        sCapSumSq += (double)v * v;
        if (a > sCapPeak) sCapPeak = a;
        if (v >= 32767 || v <= -32768) sCapClipped++;
    }
    sCapFrames += n;
    sCapLevel += n;
}

static host_u32 capture_clock(void) {
    return (host_u32)sCapLevel * 4;
}

static void capture_retrace(void) {
    int32_t mode, cmd;
    if (sCapFile == NULL) {
        return;
    }
    sCapRetraces++;
    sCapLevel -= 32006.0 / 60.0;
    if (sCapLevel < 0) {
        sCapLevel = 0;
    }
    memcpy(&mode, port_bootup_data + 0x1C0A8, 4);
    memcpy(&cmd, port_bootup_data + 0x1C0AC, 4);
    if (mode != sCapMode || cmd != sCapCmd) {
        host_log("audio-out: %.2f s (retrace %u, WAV %.2f s): music mode %d, command word %d", sCapRetraces / 60.0,
                 sCapRetraces, sCapFrames / 32000.0, mode, cmd);
        sCapMode = mode;
        sCapCmd = cmd;
    }
}

/* ---- the outputs: --audio-out and the live presenter (plan 19.12) ---- */

/* PORT_LIVE (port/gfx/port_live.c): weak, so seqrender / the tests link without it */
extern int port_live_audio_active(void) __attribute__((weak));
extern void port_live_audio_write(const void *samples, size_t bytes) __attribute__((weak));

static int sLiveAudio;
static double sDacLevel, sDacTime; /* the wall-clock DAC: frames queued at sDacTime */

static int dac_model_on(void) {
    return getenv("PORT_AUDIO_DAC") == NULL || getenv("PORT_AUDIO_DAC")[0] != '0';
}

/* Live, the DAC drains with the wall clock (32000 frames/s, the rate the presenter plays at), not
 * with the retrace count: the driver then sizes its buffers to real time whatever the step pacing
 * does (a late step, a catch-up burst, --authentic), and the stream neither drifts ahead of the
 * presenter nor runs dry. Without PORT_LIVE the retrace model below (capture_clock) is kept, so
 * --audio-out stays deterministic. */
static void dac_advance(void) {
    double now = host_time_seconds();
    if (sDacTime != 0.0) {
        sDacLevel -= (now - sDacTime) * 32000.0;
        if (sDacLevel < 0) {
            sDacLevel = 0; /* the DAC ran dry: it idles, as the hardware does */
        }
    }
    sDacTime = now;
}

static host_u32 live_clock(void) {
    dac_advance();
    return (host_u32)sDacLevel * 4;
}

static void audio_sink(const void *samples, host_u32 bytes) {
    if (sCapFile != NULL) {
        capture_sink(samples, bytes);
    }
    if (sLiveAudio) {
        dac_advance();
        sDacLevel += bytes / 4;
        port_live_audio_write(samples, bytes);
    }
}

static void outputs_setup(void) {
    if (!sLiveAudio && port_live_audio_active != NULL && port_live_audio_active()) {
        sLiveAudio = 1;
        host_log("audio: live output to the presenter (DAC model: %s)", dac_model_on() ? "wall clock" : "off");
    }
    if (sCapFile == NULL && !sLiveAudio) {
        return; /* neither output: leave the host's sink alone (seqrender installs its own) */
    }
    host_headless_set_audio_sink(audio_sink);
    host_headless_set_audio_clock(!dac_model_on() ? NULL : sLiveAudio ? live_clock : capture_clock);
}

int port_audio_capture_open(const char *path) {
    sCapFile = fopen(path, "wb");
    if (sCapFile == NULL) {
        return -1;
    }
    capture_header(0);
    /* the DAC model (default since run 840; its old fault was the 0xB5 byte-order bug, plan 15.10) */
    outputs_setup();
    return 0;
}

void port_audio_capture_close(void) {
    double n;
    if (sCapFile == NULL) {
        return;
    }
    capture_header(sCapFrames);
    fclose(sCapFile);
    sCapFile = NULL;
    sLiveAudio = 0;
    host_headless_set_audio_sink(NULL);
    host_headless_set_audio_clock(NULL);
    n = (double)sCapFrames * 2;
    host_log("audio-out: %u frames = %.2f s over %u retraces (%.2f s); peak %u, RMS %.1f, clipped %u (%.4f%%); "
             "mixer %u tasks, badAddr %u, unknownOps %u, unk19 %u",
             sCapFrames, sCapFrames / 32000.0, sCapRetraces, sCapRetraces / 60.0, sCapPeak,
             n > 0 ? __builtin_sqrt(sCapSumSq / n) : 0.0, sCapClipped, n > 0 ? 100.0 * sCapClipped / n : 0.0,
             port_audio_stats.tasks, port_audio_stats.badAddr, port_audio_stats.unknownOps, port_audio_stats.unk19);
}
