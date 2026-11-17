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
extern void *sbrk(intptr_t increment);

static int host_mapped(uint32_t p) {
    return p >= (uint32_t)(uintptr_t)__executable_start && p < (uint32_t)(uintptr_t)sbrk(0);
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

static void mixer_ready(void) {
    if (!sMixerReady) {
        mixer_init(&sMixer, translate_k0, NULL);
        sMixerReady = 1;
    }
}

void port_audio_run_list(const void *cmds, unsigned int bytes) {
    uint32_t n0;
    mixer_ready();
    n0 = sMixer.n_cmds;
    audio_abi_run(&sMixer, (const uint32_t *)cmds, (int)(bytes / 8));
    port_audio_stats.tasks++;
    port_audio_stats.cmds += sMixer.n_cmds - n0;
    port_audio_stats.badAddr = sMixer.n_bad_addr;
    port_audio_stats.unknownOps = sMixer.n_unknown;
    port_audio_stats.unk19 = sMixer.n_unk19;
}

void port_audio_frame(void) {
    OSTask_t *task;

    port_audio_stats.updates++;
    task = (OSTask_t *)AudioThread_UpdateImpl(); /* AudioTask: OSTask_t first */
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
