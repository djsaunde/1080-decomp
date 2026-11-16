/*
 * port/audio/test_audio_swap.c: checks audio_swap.c against the real ROM data
 * (port/assets/extracted from extract.py; skipped when absent). 64-bit host,
 * no game code. `make -C port/audio test`.
 *
 * - bootup Data tables: spec 0, the tables' entry counts / ranges, the f32
 *   tables (monotonic pitch ratios, pan volumes in [0, 1]), the default
 *   envelope, the filter tables;
 * - every sound font: swapped as AudioLoad_RelocateFont would see it, then
 *   walked in host order (samples inside their sample bank, codebooks,
 *   loops, envelopes terminated, tunings sane), codec census;
 * - the swapped sample header read through a bitfield view declared like
 *   include/audio.h's Sample (the host bitfield order assumption);
 * - one real sample decoded by the mixer straight from the swapped codebook
 *   (no further conversion), against a reference decoder fed the ROM bytes.
 */
#include "mixer.h"
#include "audio_swap.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

static int g_fail, g_pass;
#define CHECK(cond)                                                          \
    do {                                                                     \
        if (cond) {                                                          \
            g_pass++;                                                        \
        } else {                                                             \
            g_fail++;                                                        \
            printf("FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond);           \
        }                                                                    \
    } while (0)
#define CHECK_EQ(a, b)                                                       \
    do {                                                                     \
        long _a = (long)(a), _b = (long)(b);                                 \
        if (_a == _b) {                                                      \
            g_pass++;                                                        \
        } else {                                                             \
            g_fail++;                                                        \
            printf("FAIL %s:%d: %s == %ld, want %ld\n", __FILE__, __LINE__, #a, _a, _b); \
        }                                                                    \
    } while (0)

static uint8_t *slurp(const char *path, long *len) {
    FILE *f = fopen(path, "rb");
    uint8_t *p;
    if (f == NULL) {
        return NULL;
    }
    fseek(f, 0, SEEK_END);
    *len = ftell(f);
    fseek(f, 0, SEEK_SET);
    p = (uint8_t *)malloc((size_t)*len + 0x100);
    if (p == NULL || fread(p, 1, (size_t)*len, f) != (size_t)*len) {
        fclose(f);
        free(p);
        return NULL;
    }
    fclose(f);
    return p;
}

static int16_t be16(const uint8_t *p) {
    return (int16_t)((p[0] << 8) | p[1]);
}
static uint32_t h32(const uint8_t *p) {
    uint32_t v;
    memcpy(&v, p, 4);
    return v;
}
static int16_t h16(const uint8_t *p) {
    int16_t v;
    memcpy(&v, p, 2);
    return v;
}
static float hf(const uint8_t *p) {
    float v;
    memcpy(&v, p, 4);
    return v;
}

/* include/audio.h's Sample word 0, as declared there. */
typedef struct {
    uint32_t codec : 4;
    uint32_t medium : 2;
    uint32_t unk_bit26 : 1;
    uint32_t isRelocated : 1;
    uint32_t size : 24;
} SampleW0;

/* ---- bootup Data ---- */

static uint8_t *g_boot, *g_bank, *g_wave, *g_seq;
static long g_nboot, g_nbank, g_nwave, g_nseq;

static void test_bootup(uint8_t *d) {
    int i, n, nseq;
    uint8_t *s0 = d + 0x1CC10;

    audio_swap_bootup_data(d);

    CHECK_EQ(h32(s0), 32000);           /* spec 0 samplingFreq */
    CHECK_EQ(s0[5], 20);                /* numNotes */
    CHECK_EQ(s0[6], 3);                 /* numSequencePlayers */
    CHECK_EQ(s0[7], 2);                 /* numReverbs */
    CHECK_EQ(h32(s0 + 0x0C), 0x500);
    CHECK_EQ(h32(d + 0x1CC10 + 4 * 0x38), 24000); /* spec 4: the 24 kHz one */
    for (i = 1; i < 4; i++) { /* heapSize, initPoolSize, permanentPoolSize (word 0 is not read) */
        CHECK(h32(d + 0x1CDD0 + 4 * i) != 0 && h32(d + 0x1CDD0 + 4 * i) < 0x400000); /* heap init sizes */
    }
    /* spec 0's reverbs (0x1CB00, 2 x 0x12): downsample 1, window b1 << 6, h4 0x7FFF / h2 0x3FFF */
    CHECK_EQ(d[0x1CB00], 1);
    CHECK_EQ(d[0x1CB01], 0x0A);
    CHECK_EQ(h16(d + 0x1CB04), 0x7FFF);
    CHECK_EQ(d[0x1CB12], 1);
    CHECK_EQ(h16(d + 0x1CB14), 0x3FFF);
    CHECK_EQ(h16(d + 0x1CB18), 0x1000);

    /* pitch tables: increasing ratios, 1.0 inside */
    {
        int up = 1, one = 0;
        for (i = 1; i < 117; i++) { /* entries 117-127 wrap to the octave below (as OoT's) */
            if (!(hf(d + 0x19574 + 4 * i) > hf(d + 0x19574 + 4 * (i - 1)))) {
                up = 0;
            }
        }
        for (i = 0; i < 256; i++) {
            if (hf(d + 0x18D74 + 4 * i) == 1.0f) {
                one++;
            }
        }
        CHECK(up);
        CHECK(one >= 1);
        CHECK(hf(d + 0x19574) > 0.0f && hf(d + 0x19574 + 4 * 127) < 1024.0f);
    }
    for (i = 0; i < 3 * 128; i++) {
        float v = hf(d + 0x198E8 + 4 * i);
        if (!(v >= 0.0f && v <= 1.0f)) {
            CHECK(v >= 0.0f && v <= 1.0f);
            break;
        }
    }
    CHECK_EQ(h16(d + 0x19794), 1);      /* gDefaultEnvelope */
    CHECK_EQ(h16(d + 0x19796), 32000);
    CHECK_EQ(h16(d + 0x19798), 1000);
    CHECK_EQ(h16(d + 0x1979C), -1);     /* ADSR_HANG */
    CHECK_EQ(h16(d + 0x197E4), 0x3C);   /* gHaasEffectDelaySizes[0] */
    CHECK_EQ(h16(d + 0x197E6), 0x3A);
    CHECK_EQ(h16(d + 0x19FE8 + 6), 0x7FFF); /* gLowPassFilterData[0] = {0, 0, 0, 0x7FFF, ...} */
    CHECK_EQ(h16(d + 0x19EE8), 0x1000);
    CHECK_EQ(h32(d + 0x1A2D0), 0x13000000u); /* sEnvMixerOp = A_ENVMIXER << 24 */
    CHECK_EQ(d[0x197C4], 0x03);         /* gDefaultNoteSub: enabled | needsInit, LSB-first */

    /* the load tables */
    n = h16(d + 0x1D620);
    printf("tables: %d fonts, %d sequences, %d sample banks\n", n, h16(d + 0x1DA40), h16(d + 0x1DFF0));
    CHECK(n > 0 && n < 64);
    for (i = 0; i < n; i++) {
        uint8_t *e = d + 0x1D620 + 0x10 + 0x10 * i;
        CHECK(h32(e) + h32(e + 4) <= (uint32_t)g_nbank);
    }
    nseq = h16(d + 0x1DA40);
    CHECK(nseq > 0 && nseq < 256);
    for (i = 0; i < nseq; i++) {
        uint8_t *e = d + 0x1DA40 + 0x10 + 0x10 * i;
        CHECK(h32(e) + h32(e + 4) <= (uint32_t)g_nseq);
        CHECK((uint16_t)h16(d + 0x1D810 + 2 * i) < 0x230); /* gSequenceFontTable offsets */
    }
    n = h16(d + 0x1DFF0);
    CHECK(n > 0 && n < 16);
    for (i = 0; i < n; i++) {
        uint8_t *e = d + 0x1DFF0 + 0x10 + 0x10 * i;
        CHECK(h32(e) + h32(e + 4) <= (uint32_t)g_nwave);
    }
}

/* ---- fonts ---- */

static int g_bad_samples, g_bad_env, g_bad_tuning;

static void check_tuned(uint8_t *f, uint32_t fsize, uint8_t *ts, uint32_t bankStart, uint32_t bankSize) {
    uint32_t so = h32(ts), w0, addr, loop, book;
    float tuning = hf(ts + 4);
    SampleW0 v;
    if (so == 0) {
        return;
    }
    if (so >= fsize) {
        g_bad_samples++;
        return;
    }
    if (!(tuning > 0.0f && tuning < 16.0f)) {
        g_bad_tuning++;
    }
    w0 = h32(f + so);
    memcpy(&v, f + so, 4);
    if (v.codec != AUDIO_SAMPLE_CODEC(w0) || v.medium != AUDIO_SAMPLE_MEDIUM(w0) || v.size != AUDIO_SAMPLE_SIZE(w0) ||
        v.isRelocated != AUDIO_SAMPLE_RELOCATED(w0) || v.unk_bit26 != AUDIO_SAMPLE_UNK26(w0)) {
        g_bad_samples++;
    }
    if (AUDIO_SAMPLE_SIZE(w0) == 0) {
        return;
    }
    addr = h32(f + so + 4);
    loop = h32(f + so + 8);
    book = h32(f + so + 12);
    if (AUDIO_SAMPLE_MEDIUM(w0) == 0 && addr + AUDIO_SAMPLE_SIZE(w0) > bankSize) {
        g_bad_samples++;
    }
    (void)bankStart;
    if (loop == 0 || loop + 0x10 > fsize || h32(f + loop) > h32(f + loop + 4)) {
        g_bad_samples++;
    }
    if (AUDIO_SAMPLE_CODEC(w0) == 0 || AUDIO_SAMPLE_CODEC(w0) == 3) {
        int32_t order = (int32_t)h32(f + book), npred = (int32_t)h32(f + book + 4);
        if (book == 0 || order != 2 || npred < 1 || npred > 8) {
            g_bad_samples++;
        }
    }
}

static void check_env(uint8_t *f, uint32_t fsize, uint32_t e) {
    int i;
    if (e == 0 || e >= fsize) {
        g_bad_env++;
        return;
    }
    for (i = 0; i < 64; i++) {
        if (h16(f + e + 4 * i) <= 0) {
            return;
        }
    }
    g_bad_env++;
}

static void test_fonts(const uint8_t *d) {
    int nf = h16(d + 0x1D620), i, k;
    long fields = 0;
    for (i = 0; i < nf; i++) {
        const uint8_t *e = d + 0x1D620 + 0x10 + 0x10 * i;
        uint32_t rom = h32(e), size = h32(e + 4);
        uint16_t sd1 = (uint16_t)h16(e + 0xA), sd2 = (uint16_t)h16(e + 0xC), sd3 = (uint16_t)h16(e + 0xE);
        int ninst = sd2 >> 8, ndrum = sd2 & 0xFF, nsfx = sd3;
        const uint8_t *be = d + 0x1DFF0 + 0x10 + 0x10 * (sd1 >> 8);
        uint32_t bankStart = h32(be), bankSize = h32(be + 4);
        uint8_t *f = (uint8_t *)calloc(1, size + 0x40);
        uint32_t list;

        memcpy(f, g_bank + rom, size);
        audio_swap_font(f, ninst, ndrum, nsfx);
        list = h32(f);
        if (list != 0) {
            for (k = 0; k < ndrum; k++) {
                uint32_t dr = h32(f + list + 4 * k);
                if (dr != 0) {
                    check_tuned(f, size, f + dr + 4, bankStart, bankSize);
                    check_env(f, size, h32(f + dr + 0xC));
                }
            }
        }
        list = h32(f + 4);
        if (list != 0) {
            for (k = 0; k < nsfx; k++) {
                check_tuned(f, size, f + list + 8 * k, bankStart, bankSize);
            }
        }
        for (k = 0; k < (ninst > 126 ? 126 : ninst); k++) {
            uint32_t o = h32(f + 8 + 4 * k);
            if (o == 0) {
                continue;
            }
            check_env(f, size, h32(f + o + 4));
            if (f[o + 1] != 0) {
                check_tuned(f, size, f + o + 8, bankStart, bankSize);
            }
            check_tuned(f, size, f + o + 0x10, bankStart, bankSize);
            if (f[o + 2] != 0x7F) {
                check_tuned(f, size, f + o + 0x18, bankStart, bankSize);
            }
        }
        free(f);
    }
    fields = audio_swap_stats.fields16 + audio_swap_stats.fields32;
    printf("fonts: %d swapped (%ld fields), %u samples, %u books, %u loops, %u envelopes; codecs:", nf, fields,
           audio_swap_stats.samples, audio_swap_stats.books, audio_swap_stats.loops, audio_swap_stats.envelopes);
    for (k = 0; k < 16; k++) {
        if (audio_swap_stats.codecs[k]) {
            printf(" %d x%u", k, audio_swap_stats.codecs[k]);
        }
    }
    printf("\n");
    CHECK_EQ(g_bad_samples, 0);
    CHECK_EQ(g_bad_env, 0);
    CHECK_EQ(g_bad_tuning, 0);
    CHECK_EQ(audio_swap_stats.pcm16Samples, 0); /* no s16 PCM sample data to swap */
    CHECK_EQ(audio_swap_stats.overflow, 0);
    CHECK(audio_swap_stats.samples > 10);
}

/* ---- one sample through the mixer from the swapped book ---- */

static uint8_t g_rdram[1u << 20];
static void *xlate(uint32_t addr, void *user) {
    (void)user;
    return addr < sizeof(g_rdram) ? &g_rdram[addr] : NULL;
}

/* test_mixer.c's reference VADPCM decoder (host-order book). */
static void ref_decode(const uint8_t *src, int nframes, const int16_t *book, int16_t *out) {
    int16_t h1 = 0, h2 = 0; /* n-1, n-2 */
    int f, i, j;
    for (f = 0; f < nframes; f++) {
        const uint8_t *fr = src + f * 9;
        int scale = fr[0] >> 4;
        const int16_t *c = book + (fr[0] & 0xF) * 16;
        int r[16];
        for (i = 0; i < 16; i++) {
            int nib = (i & 1) ? (fr[1 + i / 2] & 0xF) : (fr[1 + i / 2] >> 4);
            if (nib >= 8) {
                nib -= 16;
            }
            r[i] = scale <= 12 ? nib * (1 << scale) : nib * 4096;
        }
        for (i = 0; i < 16; i += 8) {
            for (j = 0; j < 8; j++) {
                long long acc = (long long)r[i + j] * 2048 + (long long)c[j] * h2 + (long long)c[8 + j] * h1;
                int k;
                long long v;
                for (k = 0; k < j; k++) {
                    acc += (long long)c[8 + j - 1 - k] * r[i + k];
                }
                v = acc >> 11;
                out[f * 16 + i + j] = (int16_t)(v > 32767 ? 32767 : v < -32768 ? -32768 : v);
            }
            h2 = out[f * 16 + i + 6];
            h1 = out[f * 16 + i + 7];
        }
    }
}

static void test_decode_from_swapped_book(const uint8_t *d) {
    const uint8_t *e = d + 0x1D620 + 0x10; /* font 0 */
    uint32_t rom = h32(e), size = h32(e + 4), bank_off, io = 0, so, w0, saddr, book, n;
    uint16_t sd1 = (uint16_t)h16(e + 0xA), sd2 = (uint16_t)h16(e + 0xC);
    uint8_t *f = (uint8_t *)calloc(1, size + 0x40);
    int i, nframes, mism = 0, peak = 0;
    int16_t *ref, *got = (int16_t *)&g_rdram[0x40000];
    Mixer m;
    uint32_t list[8];

    memcpy(f, g_bank + rom, size);
    audio_swap_font(f, sd2 >> 8, sd2 & 0xFF, (uint16_t)h16(e + 0xE));
    bank_off = h32(d + 0x1DFF0 + 0x10 + 0x10 * (sd1 >> 8));
    for (i = 0; i < (sd2 >> 8) && io == 0; i++) {
        io = h32(f + 8 + 4 * i);
    }
    so = h32(f + io + 0x10);
    w0 = h32(f + so);
    saddr = h32(f + so + 4);
    book = h32(f + so + 0xC);
    n = h32(f + book) * h32(f + book + 4) * 16; /* book bytes */
    CHECK_EQ(AUDIO_SAMPLE_CODEC(w0), 0);
    memcpy(&g_rdram[0x1000], f + book + 8, n);  /* host order already: no conversion here */
    nframes = (int)(AUDIO_SAMPLE_SIZE(w0) / 9);
    if (nframes > 400) nframes = 400;
    nframes &= ~3;
    memcpy(&g_rdram[0x10000], g_wave + bank_off + saddr, (size_t)nframes * 9);
    ref = (int16_t *)malloc((size_t)nframes * 32);
    {
        int16_t bookRef[0x80]; /* from the ROM bytes, independently of audio_swap */
        for (i = 0; i < (int)(n / 2) && i < 0x80; i++) {
            bookRef[i] = be16(g_bank + rom + book + 8 + 2 * i);
        }
        ref_decode(&g_rdram[0x10000], nframes, bookRef, ref);
    }

    memset(&m, 0, sizeof(m));
    mixer_init(&m, xlate, NULL);
    list[0] = (11u << 24) | n; list[1] = 0x1000;
    audio_abi_run(&m, list, 1);
    for (i = 0; i < nframes; i += 4) {
        list[0] = (20u << 24) | ((48 >> 4) << 16) | 0x940; list[1] = 0x10000 + (uint32_t)i * 9;
        list[2] = (8u << 24) | 0x940; list[3] = (0x580u << 16) | 128;
        list[4] = (1u << 24) | ((i == 0 ? MIX_F_INIT : 0) << 16); list[5] = 0x2000;
        list[6] = (21u << 24) | ((128 >> 4) << 16) | 0x5A0; list[7] = 0x40000 + (uint32_t)i * 32;
        audio_abi_run(&m, list, 4);
    }
    for (i = 0; i < nframes * 16; i++) {
        int v = got[i] < 0 ? -got[i] : got[i];
        mism += got[i] != ref[i];
        if (v > peak) peak = v;
    }
    printf("swapped-book decode: %d frames, peak %d, mismatches %d\n", nframes, peak, mism);
    CHECK_EQ(mism, 0);
    CHECK(peak > 256);
    free(ref);
    free(f);
}

int main(int argc, char **argv) {
    const char *root = argc > 1 ? argv[1] : "../assets/extracted";
    char path[512];
    uint8_t *d;

    snprintf(path, sizeof(path), "%s/uso/bootup.uso/02_Data.bin", root);
    g_boot = slurp(path, &g_nboot);
    snprintf(path, sizeof(path), "%s/uso/audio_bank.uso/00_Data.bin", root);
    g_bank = slurp(path, &g_nbank);
    snprintf(path, sizeof(path), "%s/uso/audio_wave.uso/00_Data.bin", root);
    g_wave = slurp(path, &g_nwave);
    snprintf(path, sizeof(path), "%s/uso/audio_seq.uso/00_Data.bin", root);
    g_seq = slurp(path, &g_nseq);
    if (!g_boot || !g_bank || !g_wave || !g_seq) {
        printf("SKIP test_audio_swap: no extracted assets under %s\n", root);
        return 0;
    }
    d = (uint8_t *)malloc((size_t)g_nboot);
    memcpy(d, g_boot, (size_t)g_nboot);
    test_bootup(d);
    test_fonts(d);
    test_decode_from_swapped_book(d);
    printf("test_audio_swap: %d passed, %d failed\n", g_pass, g_fail);
    return g_fail != 0;
}
