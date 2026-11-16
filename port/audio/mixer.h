/*
 * port/audio/mixer.h: clean-room CPU implementation of the N64 audio ABI that
 * 1080 Snowboarding's aspMain ucode executes (OoT command numbering).
 *
 * Written from scratch for the 1080 PC port (docs/1080_PC_PORT_PLAN.md, M1).
 * Sources: 1080's own AudioSynth_* command builders, the OoT decomp's abi.h,
 * and a disassembly of 1080's own aspMain (ROM 0xE16EDC, IMEM 0x1000; data
 * image ROM 0xDBF730, jump table at DMEM 0x10). No code from SoH /
 * sm64-port mixer.c is used.
 *
 * Model:
 * - DMEM is a 4 KB byte array. 16-bit samples in it are stored in HOST byte
 *   order (memcpy in / out), so the game's s16 buffers (AI buffers, reverb
 *   rings, ADPCM / resample / filter states, wave tables) move through
 *   LOADBUFF / SAVEBUFF unchanged. Byte streams (ADPCM frames, S8 samples)
 *   are read byte by byte, so they must be in ROM (big-endian) byte order.
 * - Codebooks (LOADADPCM) and filter coefficients are s16 arrays in host
 *   order; whoever loads a font from the ROM must swap them.
 * - "DRAM" addresses in the command words go through mixer->translate.
 */
#ifndef PORT_AUDIO_MIXER_H
#define PORT_AUDIO_MIXER_H

#include <stddef.h>
#include <stdint.h>

#define MIXER_DMEM_SIZE 0x1000

/* OoT / 1080 opcode numbers (references/oot include/ultra64/abi.h). */
enum {
    MIX_A_SPNOOP = 0,
    MIX_A_ADPCM = 1,
    MIX_A_CLEARBUFF = 2,
    MIX_A_UNK3 = 3,
    MIX_A_ADDMIXER = 4,
    MIX_A_RESAMPLE = 5,
    MIX_A_RESAMPLE_ZOH = 6,
    MIX_A_FILTER = 7,
    MIX_A_SETBUFF = 8,
    MIX_A_DUPLICATE = 9,
    MIX_A_DMEMMOVE = 10,
    MIX_A_LOADADPCM = 11,
    MIX_A_MIXER = 12,
    MIX_A_INTERLEAVE = 13,
    MIX_A_HILOGAIN = 14,
    MIX_A_SETLOOP = 15,
    MIX_A_OP16 = 16,
    MIX_A_INTERL = 17,
    MIX_A_ENVSETUP1 = 18,
    MIX_A_ENVMIXER = 19,
    MIX_A_LOADBUFF = 20,
    MIX_A_SAVEBUFF = 21,
    MIX_A_ENVSETUP2 = 22,
    MIX_A_S8DEC = 23,
    MIX_A_UNK19 = 25
};

#define MIX_F_INIT 0x01
#define MIX_F_LOOP 0x02
#define MIX_F_2BIT 0x04 /* ADPCM: CODEC_SMALL_ADPCM (2 bits per sample) */

/* Maps a command-word address (what the game wrote: K0 pointer minus
 * 0x80000000 on the original) to host memory. */
typedef void *(*MixerTranslate)(uint32_t addr, void *user);

typedef struct Mixer {
    uint8_t dmem[MIXER_DMEM_SIZE];

    /* SETBUFF */
    uint16_t in, out, count;

    /* LOADADPCM: up to 8 predictors x 16 coefficients */
    int16_t book[0x80];

    /* SETLOOP */
    uint32_t loop_addr;

    /* FILTER (flags 2): byte count and the 8 coefficients */
    uint16_t filter_count;
    int16_t filter_coef[8];

    /* ENVSETUP1 / ENVSETUP2 (the ucode's v1 lanes and rate registers) */
    uint16_t env_vol[2][2];   /* [L/R][first 8 / second 8 samples] */
    uint16_t env_wet[2];      /* [first 8 / second 8] */
    uint16_t env_rate[2];     /* L, R */
    uint16_t env_rate_wet;

    MixerTranslate translate;
    void *translate_user;

    /* diagnostics */
    uint32_t n_cmds;
    uint32_t n_unknown;   /* opcodes with no handler (16, 24, 26+) */
    uint32_t n_unk19;     /* opcode 25: see mixer.c */
    uint32_t n_bad_addr;  /* translate returned NULL */
} Mixer;

void mixer_init(Mixer *m, MixerTranslate translate, void *user);

/* One command (two words, as the game wrote them). */
void mixer_exec(Mixer *m, uint32_t w0, uint32_t w1);

/* abi_decode.c: run a whole Acmd list (ncmds commands, 2 words each). */
void audio_abi_run(Mixer *m, const uint32_t *words, int ncmds);

/* Per-opcode entry points (also used by the unit tests). Argument names
 * follow the command fields. Addresses are command-word addresses. */
void mix_spnoop(Mixer *m);
void mix_adpcm(Mixer *m, uint8_t flags, uint32_t state_addr);
void mix_clearbuff(Mixer *m, uint16_t dmem, uint16_t count);
void mix_unk3(Mixer *m, uint16_t dmem_a, uint16_t dmem_b, uint16_t count);
void mix_addmixer(Mixer *m, uint16_t count, uint16_t dmem_in, uint16_t dmem_out);
void mix_resample(Mixer *m, uint8_t flags, uint16_t pitch, uint32_t state_addr);
void mix_resample_zoh(Mixer *m, uint16_t pitch, uint16_t pitch_accu);
void mix_filter(Mixer *m, uint8_t flags, uint16_t count_or_dmem, uint32_t addr);
void mix_setbuff(Mixer *m, uint16_t in, uint16_t out, uint16_t count);
void mix_duplicate(Mixer *m, uint8_t copies, uint16_t dmem_src, uint16_t dmem_dst);
void mix_dmemmove(Mixer *m, uint16_t dmem_in, uint16_t dmem_out, uint16_t count);
void mix_loadadpcm(Mixer *m, uint32_t count, uint32_t addr);
void mix_mixer(Mixer *m, uint16_t count, int16_t gain, uint16_t dmem_in, uint16_t dmem_out);
void mix_interleave(Mixer *m, uint16_t count, uint16_t dmem_out, uint16_t left, uint16_t right);
void mix_hilogain(Mixer *m, uint8_t gain, uint16_t count, uint16_t dmem);
void mix_setloop(Mixer *m, uint32_t addr);
void mix_interl(Mixer *m, uint16_t count, uint16_t dmem_in, uint16_t dmem_out);
void mix_envsetup1(Mixer *m, uint8_t wet, uint16_t rate_wet, uint16_t rate_left, uint16_t rate_right);
void mix_envmixer(Mixer *m, uint32_t w0, uint32_t dests);
void mix_loadbuff(Mixer *m, uint16_t count, uint16_t dmem, uint32_t addr);
void mix_savebuff(Mixer *m, uint16_t count, uint16_t dmem, uint32_t addr);
void mix_envsetup2(Mixer *m, uint16_t vol_left, uint16_t vol_right);
void mix_s8dec(Mixer *m, uint8_t flags, uint32_t state_addr);
void mix_unk19(Mixer *m, uint32_t w0, uint32_t w1);

/* DMEM access helpers (host-order s16). */
int16_t mixer_get16(const Mixer *m, uint32_t dmem);
void mixer_set16(Mixer *m, uint32_t dmem, int16_t v);

/* The resample LUT from 1080's aspMain data image (DMEM 0xE0, 64 x 4). */
extern const int16_t mixer_resample_lut[64][4];

#endif
