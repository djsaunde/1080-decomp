/*
 * port/audio/audio_swap.c: big-endian audio data -> host order (see
 * audio_swap.h and docs/1080_PC_PORT_PLAN.md section 15.6).
 *
 * Layouts are 1080's (include/audio.h, AudioHeap_Init's gl_21498_* views,
 * AudioLoad_Init, AudioLoad_RelocateFont / RelocateSample). Offsets into
 * bootup.uso Data are for the US 1.0 ROM (the extractor refuses any other).
 */
#include <stddef.h>
#include "audio_swap.h"

AudioSwapStats audio_swap_stats;

/* ---- the set of fields already swapped (per call) ---- */

#define FIELD_SET_SIZE (1u << 15)
static uintptr_t sFieldKey[FIELD_SET_SIZE];
static uint32_t sFieldGen[FIELD_SET_SIZE];
static uint32_t sGen;

static void field_set_reset(void) {
    sGen++;
    if (sGen == 0) { /* wrapped: clear */
        size_t i;
        for (i = 0; i < FIELD_SET_SIZE; i++) {
            sFieldGen[i] = 0;
        }
        sGen = 1;
    }
}

/* 1 if p was not yet in the set (and adds it), 0 if it was. */
static int field_claim(const void *p) {
    uintptr_t k = (uintptr_t)p;
    uint32_t h = (uint32_t)((k >> 1) * 2654435761u) & (FIELD_SET_SIZE - 1);
    uint32_t n;
    for (n = 0; n < FIELD_SET_SIZE; n++) {
        if (sFieldGen[h] != sGen) {
            sFieldGen[h] = sGen;
            sFieldKey[h] = k;
            return 1;
        }
        if (sFieldKey[h] == k) {
            return 0;
        }
        h = (h + 1) & (FIELD_SET_SIZE - 1);
    }
    audio_swap_stats.overflow++;
    return 1;
}

/* ---- field swaps ---- */

static uint32_t be32(const uint8_t *b) {
    return ((uint32_t)b[0] << 24) | ((uint32_t)b[1] << 16) | ((uint32_t)b[2] << 8) | b[3];
}

static uint32_t host32(const uint8_t *p) {
    uint32_t v;
    const uint8_t *s = p;
    uint8_t *d = (uint8_t *)&v;
    d[0] = s[0]; d[1] = s[1]; d[2] = s[2]; d[3] = s[3];
    return v;
}

static int16_t host16(const uint8_t *p) {
    int16_t v;
    uint8_t *d = (uint8_t *)&v;
    d[0] = p[0]; d[1] = p[1];
    return v;
}

static void put32(uint8_t *p, uint32_t v) {
    const uint8_t *s = (const uint8_t *)&v;
    p[0] = s[0]; p[1] = s[1]; p[2] = s[2]; p[3] = s[3];
}

static void sw32(uint8_t *p) {
    if (field_claim(p)) {
        put32(p, be32(p));
        audio_swap_stats.fields32++;
    }
}

static void sw16(uint8_t *p) {
    if (field_claim(p)) {
        uint16_t v = (uint16_t)((p[0] << 8) | p[1]);
        const uint8_t *s = (const uint8_t *)&v;
        p[0] = s[0]; p[1] = s[1];
        audio_swap_stats.fields16++;
    }
}

static void sw16_range(uint8_t *p, uint32_t bytes) {
    uint32_t i;
    for (i = 0; i + 1 < bytes; i += 2) {
        sw16(p + i);
    }
}

static void sw32_range(uint8_t *p, uint32_t bytes) {
    uint32_t i;
    for (i = 0; i + 3 < bytes; i += 4) {
        sw32(p + i);
    }
}

uint32_t audio_sample_word0_from_be(const uint8_t *be) {
    uint32_t b0 = be[0];
    uint32_t size = ((uint32_t)be[1] << 16) | ((uint32_t)be[2] << 8) | be[3];
    uint32_t codec = b0 >> 4, medium = (b0 >> 2) & 3, unk26 = (b0 >> 1) & 1, reloc = b0 & 1;
    return codec | (medium << 4) | (unk26 << 6) | (reloc << 7) | (size << 8);
}

/* ---- sound font ---- */

static void swap_envelope(uint8_t *font, uint32_t off) {
    int i;
    if (off == 0) {
        return;
    }
    audio_swap_stats.envelopes++;
    for (i = 0; i < 64; i++) { /* {s16 delay, s16 arg}; delay <= 0 ends it (DISABLE / HANG / GOTO / RESTART) */
        uint8_t *pt = font + off + i * 4;
        sw16(pt);
        sw16(pt + 2);
        if (host16(pt) <= 0) {
            break;
        }
    }
}

static void swap_sample(uint8_t *font, uint32_t off) {
    uint8_t *s = font + off;
    uint32_t w0, loop, book;

    if (!field_claim(s)) {
        return; /* shared sample: done already */
    }
    w0 = audio_sample_word0_from_be(s);
    put32(s, w0);
    audio_swap_stats.samples++;
    audio_swap_stats.codecs[AUDIO_SAMPLE_CODEC(w0)]++;
    if (AUDIO_SAMPLE_CODEC(w0) == 2 || AUDIO_SAMPLE_CODEC(w0) == 5) {
        audio_swap_stats.pcm16Samples++;
    }
    sw32(s + 4);  /* sampleAddr (sample-bank offset) */
    sw32(s + 8);  /* loop */
    sw32(s + 12); /* book */
    if (AUDIO_SAMPLE_SIZE(w0) == 0) {
        return; /* AudioLoad_RelocateSample leaves it alone */
    }
    loop = host32(s + 8);
    book = host32(s + 12);
    if (loop != 0 && field_claim(font + loop + 0xD)) { /* an odd key: no field has it */
        uint8_t *l = font + loop;
        audio_swap_stats.loops++;
        sw32(l + 0x0);                /* start */
        sw32(l + 0x4);                /* end */
        sw32(l + 0x8);                /* count */
        sw32(l + 0xC);
        if (host32(l + 0x8) != 0) {   /* predictorState[16] only exists for a looping sample */
            sw16_range(l + 0x10, 32);
        }
    }
    if (book != 0 && field_claim(font + book + 1)) { /* odd key */
        uint8_t *b = font + book;
        int32_t order, npred;
        audio_swap_stats.books++;
        sw32(b);     /* order */
        sw32(b + 4); /* numPredictors */
        order = (int32_t)host32(b);
        npred = (int32_t)host32(b + 4);
        if (order > 0 && order <= 8 && npred > 0 && npred <= 8) {
            sw16_range(b + 8, (uint32_t)(order * npred * 16));
        }
    }
}

static void swap_tuned_sample(uint8_t *font, uint8_t *ts) {
    uint32_t s;
    sw32(ts);     /* Sample * (font offset) */
    sw32(ts + 4); /* f32 tuning */
    s = host32(ts);
    if (s != 0 && s <= 0x80000000u) {
        swap_sample(font, s);
    }
}

void audio_swap_font(uint8_t *font, int numInstruments, int numDrums, int numSfx) {
    int i;
    uint32_t list;

    field_set_reset();
    if (numInstruments > 126) {
        numInstruments = 126;
    }
    sw32(font);     /* drum list */
    sw32(font + 4); /* sfx list */
    for (i = 0; i < numInstruments; i++) {
        sw32(font + 8 + i * 4);
    }

    list = host32(font);
    if (list != 0 && numDrums != 0) {
        for (i = 0; i < numDrums; i++) {
            uint32_t d;
            sw32(font + list + i * 4);
            d = host32(font + list + i * 4);
            if (d != 0 && field_claim(font + d + 1)) { /* Drum: u8 x3, TunedSample @4, envelope @0xC */
                swap_tuned_sample(font, font + d + 4);
                sw32(font + d + 0xC);
                swap_envelope(font, host32(font + d + 0xC));
            }
        }
    }

    list = host32(font + 4);
    if (list != 0 && numSfx != 0) {
        for (i = 0; i < numSfx; i++) {
            swap_tuned_sample(font, font + list + i * 8);
        }
    }

    for (i = 0; i < numInstruments; i++) {
        uint32_t o = host32(font + 8 + i * 4);
        uint8_t *inst;
        if (o == 0 || !field_claim(font + o + 1)) { /* Instrument: u8 x4, envelope @4, TunedSamples @8/0x10/0x18 */
            continue;
        }
        inst = font + o;
        sw32(inst + 4);
        swap_envelope(font, host32(inst + 4));
        if (inst[1] != 0) { /* normalRangeLo: as AudioLoad_RelocateFont */
            swap_tuned_sample(font, inst + 0x8);
        }
        swap_tuned_sample(font, inst + 0x10);
        if (inst[2] != 0x7F) { /* normalRangeHi */
            swap_tuned_sample(font, inst + 0x18);
        }
    }
}

/* ---- bootup.uso Data: the driver's static tables ----
 *
 * Offsets are bootup Data offsets (port_bootup_data + K). Pointer words are
 * not listed: port/uso writes relocated words in host order already. */

typedef struct {
    uint32_t start, end;
    uint8_t width; /* 2 or 4 */
    const char *what;
} SwapRange;

static const SwapRange sBootupRanges[] = {
    {0x17D50, 0x18D50, 2, "wave tables: 8 x 256 s16 (gWaveSamples[0..7] targets)"},
    {0x18D74, 0x19174, 4, "gBendPitchOneOctaveFrequencies f32[256]"},
    {0x19174, 0x19574, 4, "gBendPitchTwoSemitonesFrequencies f32[256]"},
    {0x19574, 0x19774, 4, "gPitchFrequencies f32[128]"},
    /* 0x19774 / 0x19784: velocity and gate-time u8[16] tables (bytes) */
    {0x19794, 0x197A4, 2, "gDefaultEnvelope {s16 delay, s16 arg}[4]"},
    /* 0x197A4 gZeroNoteSub (zero), 0x197C4 gDefaultNoteSub: byte 0 below */
    {0x197E4, 0x19868, 2, "gHaasEffectDelaySizes u16[64] (+ pad)"},
    {0x19868, 0x198E8, 2, "the zero codebook s16[64] (D_8012FBA8)"},
    {0x198E8, 0x19AE8, 4, "gHeadsetPanVolume f32[128]"},
    {0x19AE8, 0x19CE8, 4, "gStereoPanVolume f32[128]"},
    {0x19CE8, 0x19EE8, 4, "gDefaultPanVolume f32[128]"},
    {0x19EE8, 0x19FE8, 2, "filter table s16[16][8] (before gLowPassFilterData)"},
    {0x19FE8, 0x1A1D8, 2, "gLowPassFilterData s16[31][8]"},
    {0x1A1D8, 0x1A2C8, 2, "gHighPassFilterData s16[15][8]"},
    {0x1A2D0, 0x1A2E0, 4, "sEnvMixerOp + Left / Right / Default Haas dmem dests (u32)"},
    {0x1CDD0, 0x1CDD4, 2, "gAudioHeapInitSizes unk0, tatumsPerBeat s16 (AudioHeap_Init reads +2 as s16)"},
    {0x1CDD4, 0x1CDE0, 4, "gAudioHeapInitSizes heapSize, initPoolSize, permanentPoolSize s32"},
};

/* gAudioSpecs (0x1CC10, 8 x 0x38) and the ReverbSettings each points at
 * (count = spec byte 7). The pointer targets are the ROM's DataRelocs of
 * spec +0x08 (port/gen/uso_modules.c, bootup data relocs 0x1CC18 + 0x38 k). */
#define SPEC_TABLE 0x1CC10
#define SPEC_COUNT 8
#define SPEC_SIZE 0x38
static const uint32_t sSpecReverbs[SPEC_COUNT] = {
    0x1CB00, 0x1CB6C, 0x1CB24, 0x1CB48, 0x1CB48, 0x1CBB4, 0x1CBD8, 0x1CB48,
};

static void swap_spec(uint8_t *data, int k) {
    uint8_t *s = data + SPEC_TABLE + k * SPEC_SIZE;
    int i, n;
    sw32(s + 0x00);           /* samplingFreq */
    /* 0x04..0x07 u8: unk, numNotes, numSequencePlayers, numReverbs; 0x08 pointer */
    sw32(s + 0x0C);
    sw32(s + 0x10);
    sw16(s + 0x14);
    sw32_range(s + 0x18, 0x20); /* 8 x s32: pool / cache sizes */
    n = s[7];
    for (i = 0; i < n; i++) { /* ReverbSettings, 0x12: u8 u8 u16 u16 u16 u16 s8 pad u16 s16 s16 */
        uint8_t *r = data + sSpecReverbs[k] + i * 0x12;
        sw16(r + 0x2);
        sw16(r + 0x4);
        sw16(r + 0x6);
        sw16(r + 0x8);
        sw16(r + 0xC);
        sw16(r + 0xE);
        sw16(r + 0x10);
    }
}

/* AudioTable: header {s16 numEntries, s16, u32 romAddr, pad 8}, entries
 * {u32 romAddr, u32 size, s8 medium, s8 cachePolicy, s16 x3}. */
static int swap_audio_table(uint8_t *t) {
    int i, n;
    sw16(t);
    sw16(t + 2);
    sw32(t + 4);
    n = host16(t);
    for (i = 0; i < n; i++) {
        uint8_t *e = t + 0x10 + i * 0x10;
        sw32(e);
        sw32(e + 4);
        sw16(e + 0xA);
        sw16(e + 0xC);
        sw16(e + 0xE);
    }
    return n;
}

#define SOUNDFONT_TABLE 0x1D620
#define SEQFONT_TABLE 0x1D810
#define SEQUENCE_TABLE 0x1DA40
#define SAMPLEBANK_TABLE 0x1DFF0
#define DEFAULT_NOTESUB 0x197C4

static uint8_t bitrev8(uint8_t b) {
    uint8_t r = 0;
    int i;
    for (i = 0; i < 8; i++) {
        r = (uint8_t)((r << 1) | ((b >> i) & 1));
    }
    return r;
}

void audio_swap_bootup_data(uint8_t *data) {
    size_t i;
    int k, nseq;

    field_set_reset();
    for (i = 0; i < sizeof(sBootupRanges) / sizeof(sBootupRanges[0]); i++) {
        const SwapRange *r = &sBootupRanges[i];
        if (r->width == 2) {
            sw16_range(data + r->start, r->end - r->start);
        } else {
            sw32_range(data + r->start, r->end - r->start);
        }
    }
    for (k = 0; k < SPEC_COUNT; k++) {
        swap_spec(data, k);
    }
    swap_audio_table(data + SOUNDFONT_TABLE);
    nseq = swap_audio_table(data + SEQUENCE_TABLE);
    swap_audio_table(data + SAMPLEBANK_TABLE);
    for (k = 0; k < nseq; k++) { /* gSequenceFontTable: u16 offset per sequence, then u8 lists */
        sw16(data + SEQFONT_TABLE + k * 2);
    }
    /* gDefaultNoteSub byte 0 = bitField0 {enabled, needsInit, finished, unused,
     * stopLoop, hasTwoParts, isSyntheticWave, usesHeadsetPanEffects} as
     * MSB-first bits; the host bitfield order is LSB-first. The rest of the
     * struct is zero on the ROM. */
    if (field_claim(data + DEFAULT_NOTESUB)) {
        data[DEFAULT_NOTESUB] = bitrev8(data[DEFAULT_NOTESUB]);
    }
}
