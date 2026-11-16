/*
 * port/audio/audio_swap.h: byte order for the audio data the C driver and
 * the CPU mixer read (docs/1080_PC_PORT_PLAN.md section 15.6).
 *
 * The cartridge is big-endian. Byte streams (sequences, VADPCM frames, S8
 * samples) are read byte by byte and stay as they are. Every multi-byte field
 * the driver or the mixer reads is converted to host order once, where the
 * data enters RAM:
 * - audio_swap_font(): a sound font, from AudioLoad_RelocateFont (every font
 *   load, sync or async, goes through it before anything reads the font);
 * - audio_swap_bootup_data(): the driver's static tables in bootup.uso Data,
 *   from AudioLoad_Init (once).
 *
 * Sample header word 0 (codec:4 medium:2 unk_bit26:1 isRelocated:1 size:24,
 * declared MSB first for MIPS) is re-packed into the host compiler's
 * bitfield order for the SAME declaration (gcc / clang on a little-endian
 * host: codec bits 0-3, medium 4-5, unk_bit26 6, isRelocated 7, size 8-31),
 * so every bitfield view of it in the game source works unchanged. Code that
 * reads that word with shifts and masks needs a PORT spelling (the one site
 * is AudioLoad_SlowLoadSample). AUDIO_SAMPLE_* below are those accessors.
 *
 * Both functions are idempotent per field: a font's structures are shared
 * (several instruments point at one sample, book or envelope), so every
 * swapped field is recorded and never swapped twice.
 */
#ifndef PORT_AUDIO_SWAP_H
#define PORT_AUDIO_SWAP_H

#include <stdint.h>

/* Host-order Sample word 0 (after audio_swap_font). */
#define AUDIO_SAMPLE_CODEC(w) ((uint32_t)(w) & 0xF)
#define AUDIO_SAMPLE_MEDIUM(w) (((uint32_t)(w) >> 4) & 3)
#define AUDIO_SAMPLE_UNK26(w) (((uint32_t)(w) >> 6) & 1)
#define AUDIO_SAMPLE_RELOCATED(w) (((uint32_t)(w) >> 7) & 1)
#define AUDIO_SAMPLE_SIZE(w) ((uint32_t)(w) >> 8)

typedef struct AudioSwapStats {
    uint32_t fields16, fields32, samples, books, loops, envelopes;
    uint32_t codecs[16]; /* samples per codec (0 ADPCM, 1 S8, 2 S16 in memory, 3 small ADPCM, 4 reverb, 5 S16) */
    uint32_t pcm16Samples; /* codec 2 / 5: their sample data would need an s16 swap too (none on the US ROM) */
    uint32_t overflow;     /* the field set filled up (fields swapped without the duplicate check) */
} AudioSwapStats;

extern AudioSwapStats audio_swap_stats;

/* font: the font's data, as loaded from the ROM (offsets not yet relocated).
 * Counts as in gAudioCtx.soundFontList[fontId] (numInstruments clamped to
 * 126 as AudioLoad_RelocateFont does). */
void audio_swap_font(uint8_t *font, int numInstruments, int numDrums, int numSfx);

/* data: bootup.uso's Data section (port_bootup_data), after relocation.
 * Swaps the audio driver's tables listed in audio_swap.c; once per load. */
void audio_swap_bootup_data(uint8_t *data);

/* Re-pack one big-endian Sample header word into host bitfield order. */
uint32_t audio_sample_word0_from_be(const uint8_t *be);

#endif
