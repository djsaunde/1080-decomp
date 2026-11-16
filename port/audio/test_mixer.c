/*
 * port/audio/test_mixer.c: native unit tests for the CPU audio mixer.
 *   make -C port/audio test
 * The vectors are computed by hand in the comments. The last test decodes a
 * real VADPCM sample from port/assets/extracted (extract.py) when present.
 */
#include "mixer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

/* Fake RDRAM: command-word addresses are offsets into it. */
#define RDRAM_SIZE (1u << 20)
static uint8_t g_rdram[RDRAM_SIZE];

static void *xlate(uint32_t addr, void *user) {
    (void)user;
    return addr < RDRAM_SIZE ? &g_rdram[addr] : NULL;
}

static Mixer g_m;

static Mixer *fresh(void) {
    mixer_init(&g_m, xlate, NULL);
    memset(g_rdram, 0, sizeof(g_rdram));
    return &g_m;
}

#define G(a) mixer_get16(m, (a))
#define S(a, v) mixer_set16(m, (a), (int16_t)(v))

static void test_clear_move(void) {
    Mixer *m = fresh();
    memset(m->dmem, 0xAA, sizeof(m->dmem));
    mix_clearbuff(m, 0x400, 20); /* rounds up to 32 */
    CHECK_EQ(m->dmem[0x41F], 0);
    CHECK_EQ(m->dmem[0x420], 0xAA);
    mix_clearbuff(m, 0x500, 0); /* 0 = nothing */
    CHECK_EQ(m->dmem[0x500], 0xAA);

    S(0x600, 1234);
    S(0x61E, -5);
    mix_dmemmove(m, 0x600, 0x700, 17); /* rounds up to 32 */
    CHECK_EQ(G(0x700), 1234);
    CHECK_EQ(G(0x71E), -5);
    CHECK_EQ(m->dmem[0x720], 0xAA);
}

static void test_addmixer_mix(void) {
    Mixer *m = fresh();
    S(0x400, 30000); S(0x500, 10000);   /* -> 32767 */
    S(0x402, -30000); S(0x502, -10000); /* -> -32768 */
    S(0x404, 100); S(0x504, -50);       /* -> 50 */
    mix_addmixer(m, 0x40, 0x500, 0x400);
    CHECK_EQ(G(0x400), 32767);
    CHECK_EQ(G(0x402), -32768);
    CHECK_EQ(G(0x404), 50);

    m = fresh();
    /* gain 0.5: (2*1000*32767 + 2*1000*16384 + 0x8000) >> 16 = 98334768 >> 16 = 1500 */
    S(0x400, 1000); S(0x500, 1000);
    /* gain -0x8000 (OoT reverb "subtract"): (6553400 - 6553600 + 32768) >> 16 = 0 */
    S(0x440, 100); S(0x540, 100);
    mix_mixer(m, 0x20, 0x4000, 0x500, 0x400);
    mix_mixer(m, 0x20, -0x8000, 0x540, 0x440);
    CHECK_EQ(G(0x400), 1500);
    CHECK_EQ(G(0x440), 0);
}

static void test_interleave_interl(void) {
    Mixer *m = fresh();
    int i;
    for (i = 0; i < 8; i++) {
        S(0x940 + i * 2, i + 1);
        S(0xAE0 + i * 2, -(i + 1));
    }
    mix_interleave(m, 16, 0x3C0, 0x940, 0xAE0);
    CHECK_EQ(G(0x3C0), 1);
    CHECK_EQ(G(0x3C2), -1);
    CHECK_EQ(G(0x3DC), 8);
    CHECK_EQ(G(0x3DE), -8);

    m = fresh();
    for (i = 0; i < 32; i++) {
        S(0x400 + i * 2, i);
    }
    mix_interl(m, 16, 0x400, 0x600);
    CHECK_EQ(G(0x600), 0);
    CHECK_EQ(G(0x602), 2);
    CHECK_EQ(G(0x61E), 30);
}

static void test_hilogain(void) {
    Mixer *m = fresh();
    S(0x400, 1000);  /* * 1.5 = 1500 */
    S(0x402, -1001); /* * 1.5 = -1501.5, floor -> -1502 */
    mix_hilogain(m, 0x18, 32, 0x400);
    CHECK_EQ(G(0x400), 1500);
    CHECK_EQ(G(0x402), -1502);
    S(0x404, 20000); /* * 2.0 -> clamp */
    mix_hilogain(m, 0x20, 8, 0x404);
    CHECK_EQ(G(0x404), 32767);
}

static void test_duplicate(void) {
    Mixer *m = fresh();
    memset(m->dmem + 0x200, 0x11, 128);
    memset(m->dmem + 0x400, 0xEE, 0x200);
    mix_duplicate(m, 2, 0x200, 0x400); /* 2 copies, not 3 */
    CHECK_EQ(m->dmem[0x4FF], 0x11);
    CHECK_EQ(m->dmem[0x500], 0xEE);
    mix_duplicate(m, 0, 0x200, 0x500); /* 0 still writes once */
    CHECK_EQ(m->dmem[0x57F], 0x11);
    CHECK_EQ(m->dmem[0x580], 0xEE);
}

static void test_resample_zoh(void) {
    Mixer *m = fresh();
    int i;
    for (i = 0; i < 8; i++) {
        S(0x400 + i * 2, 100 * (i + 1));
    }
    mix_setbuff(m, 0x400, 0x600, 16);
    mix_resample_zoh(m, 0x4000, 0); /* 0.5: each sample twice */
    CHECK_EQ(G(0x600), 100);
    CHECK_EQ(G(0x602), 100);
    CHECK_EQ(G(0x604), 200);
    CHECK_EQ(G(0x60E), 400);
}

static void test_resample(void) {
    Mixer *m = fresh();
    int16_t *st = (int16_t *)&g_rdram[0x1000];
    int i;
    for (i = 0; i < 16; i++) {
        S(0x400 + i * 2, 1000);
    }
    mix_setbuff(m, 0x400, 0x600, 16);
    mix_resample(m, MIX_F_INIT, 0x8000, 0x1000);
    /* LUT row 0 = {3129, 26285, 3398, -33}; VMULF(1000, c) =
     * (2000c + 0x8000) >> 16: -33 -> -1, 3398 -> 104, 26285 -> 802, 3129 -> 95.
     * Window slides over 4 zeros of history then 1000s. */
    CHECK_EQ(G(0x600), 0);
    CHECK_EQ(G(0x602), -1);
    CHECK_EQ(G(0x604), 103);
    CHECK_EQ(G(0x606), 905);
    CHECK_EQ(G(0x608), 1000);
    CHECK_EQ(G(0x60E), 1000);
    CHECK_EQ(G(0x3F8), 0); /* history written at in - 8 */
    CHECK_EQ(st[0], 1000);
    CHECK_EQ(st[4], 0);

    /* Half speed: odd outputs use fraction 0x8000 -> LUT row 32 =
     * {-337, 16480, 16970, -330} (DMEM 0x1E0 of the data image). VMULF(1000, c)
     * = -10, 503, 518, -10 -> sat(493) + sat(508) = 1001: the pairwise
     * rounding makes a constant 1000 come out as 1001 here. */
    CHECK_EQ(mixer_resample_lut[32][1], 16480);
    mix_setbuff(m, 0x400, 0x600, 16);
    mix_resample(m, 0, 0x4000, 0x1000);
    CHECK_EQ(st[4], 0); /* 8 steps of 0x8000 */
    CHECK_EQ(G(0x60C), 1000); /* even outputs: row 0 */
    CHECK_EQ(G(0x60E), 1001);
}

static void test_env(void) {
    Mixer *m = fresh();
    int i;
    const uint32_t dests = (0x940 >> 4) << 24 | (0xAE0 >> 4) << 16 | (0xC80 >> 4) << 8 | (0xE20 >> 4);
    for (i = 0; i < 32; i++) {
        S(0x3C0 + i * 2, 1000);
    }
    /* wet 0x40 -> 0x4000; volL 0x8000 -> s/2 = 500; volR 0x4000 -> 250;
     * wet sends (500 * 0x4000) >> 16 = 125, (250 * 0x4000) >> 16 = 62. */
    mix_envsetup1(m, 0x40, 0, 0, 0);
    mix_envsetup2(m, 0x8000, 0x4000);
    mix_envmixer(m, (19u << 24) | ((0x3C0 >> 4) << 16) | (16 << 8), dests);
    CHECK_EQ(G(0x940), 500);
    CHECK_EQ(G(0xAE0), 250);
    CHECK_EQ(G(0xC80), 125);
    CHECK_EQ(G(0xE20), 62);
    CHECK_EQ(G(0x95E), 500);
    CHECK_EQ(G(0x960), 0); /* only 16 samples */

    /* swap (bit 4) + left dry negate (bit 1): a = 500 ^ -1 = -501,
     * wet sends: wa = (-501 * 0x4000) >> 16 = -126 (floor), wb = 62; swapped. */
    m = fresh();
    for (i = 0; i < 16; i++) {
        S(0x3C0 + i * 2, 1000);
    }
    mix_envsetup1(m, 0x40, 0, 0, 0);
    mix_envsetup2(m, 0x8000, 0x4000);
    mix_envmixer(m, (19u << 24) | ((0x3C0 >> 4) << 16) | (16 << 8) | 0x10 | 0x2, dests);
    CHECK_EQ(G(0x940), -501);
    CHECK_EQ(G(0xC80), 62);
    CHECK_EQ(G(0xE20), -126);

    /* ramp: rateL 0x1000 per 8 samples: vols 0x8000, 0x9000 | 0xA000, 0xB000
     * -> 500, 562 | 625, 687 */
    m = fresh();
    for (i = 0; i < 32; i++) {
        S(0x3C0 + i * 2, 1000);
    }
    mix_envsetup1(m, 0, 0, 0x1000, 0);
    mix_envsetup2(m, 0x8000, 0);
    mix_envmixer(m, (19u << 24) | ((0x3C0 >> 4) << 16) | (32 << 8), dests);
    CHECK_EQ(G(0x940), 500);
    CHECK_EQ(G(0x950), 562);
    CHECK_EQ(G(0x960), 625);
    CHECK_EQ(G(0x970), 687);
}

static void test_unk3(void) {
    Mixer *m = fresh();
    /* lanes: x = A[k], y = A[8+k] unsigned; B[8+k] = low(y*x), B[k] = clamp_low(2*y*x) */
    S(0x400, 100); S(0x410, 3);           /* 300 / 600 */
    S(0x402, 2); S(0x412, -1);            /* y = 65535: 131070 -> -2; 262140 -> -4 */
    S(0x404, 30000); S(0x414, 30000);     /* 9e8: low -5888; 1.8e9: low -11776 */
    S(0x406, -30000); S(0x416, -5536);    /* y = 60000: 2p = -3.6e9 -> clamp 0 */
    mix_unk3(m, 0x400, 0x400, 32);
    CHECK_EQ(G(0x410), 300);
    CHECK_EQ(G(0x400), 600);
    CHECK_EQ(G(0x412), -2);
    CHECK_EQ(G(0x402), -4);
    CHECK_EQ(G(0x414), -5888);
    CHECK_EQ(G(0x404), -11776);
    CHECK_EQ(G(0x406), 0);
}

static void test_adpcm(void) {
    Mixer *m = fresh();
    int16_t *state = (int16_t *)&g_rdram[0x2000];
    static const uint8_t frame[9] = { 0x00, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0 };
    static const int16_t want[16] = { 1, 2, 3, 4, 5, 6, 7, -8, -7, -6, -5, -4, -3, -2, -1, 0 };
    int i;

    /* zero book, scale 0: output = the signed nibbles */
    memcpy(m->dmem + 0x940, frame, 9);
    mix_setbuff(m, 0x940, 0x580, 32);
    mix_adpcm(m, MIX_F_INIT, 0x2000);
    for (i = 0; i < 16; i++) {
        CHECK_EQ(G(0x580 + i * 2), 0); /* history block */
        CHECK_EQ(G(0x5A0 + i * 2), want[i]);
        CHECK_EQ(state[i], want[i]);
    }

    /* scale 2 -> nibble << 2 */
    m->dmem[0x940] = 0x20;
    mix_adpcm(m, MIX_F_INIT, 0x2000);
    CHECK_EQ(G(0x5A0), 4);
    CHECK_EQ(G(0x5AE), -32);

    /* book: predictor 1 has cb[8] = 2048 (1.0 on sample n-1 at lane 0, and on
     * the previous residual for the other lanes). Residuals all 1:
     * x0 = 1 + prev(0) = 1, x1..7 = 1 + 1 = 2, x8 = 1 + x7 = 3, x9..15 = 2. */
    m->book[16 + 8] = 2048;
    m->dmem[0x940] = 0x01;
    memset(m->dmem + 0x941, 0x11, 8);
    mix_adpcm(m, MIX_F_INIT, 0x2000);
    CHECK_EQ(G(0x5A0), 1);
    CHECK_EQ(G(0x5A2), 2);
    CHECK_EQ(G(0x5AE), 2);
    CHECK_EQ(G(0x5B0), 3);
    CHECK_EQ(G(0x5BE), 2);

    /* continue from the saved state: history block = last frame */
    mix_adpcm(m, 0, 0x2000);
    CHECK_EQ(G(0x580), 1);
    CHECK_EQ(G(0x5A0), 1 + 2); /* x0 = r0 + prev[15] */

    /* 2-bit: 0x1B = crumbs 0, 1, 2, 3 -> 0, 1, -2, -1 */
    m = fresh();
    m->dmem[0x940] = 0x00;
    m->dmem[0x941] = 0x1B;
    mix_setbuff(m, 0x940, 0x580, 32);
    mix_adpcm(m, MIX_F_INIT | MIX_F_2BIT, 0x2000);
    CHECK_EQ(G(0x5A0), 0);
    CHECK_EQ(G(0x5A2), 1);
    CHECK_EQ(G(0x5A4), -2);
    CHECK_EQ(G(0x5A6), -1);

    /* loop flag: history from the SETLOOP address */
    m = fresh();
    ((int16_t *)&g_rdram[0x3000])[15] = 77;
    mix_setloop(m, 0x3000);
    mix_setbuff(m, 0x940, 0x580, 32);
    mix_adpcm(m, MIX_F_LOOP, 0x2000);
    CHECK_EQ(G(0x59E), 77);
}

static void test_s8_filter_dma(void) {
    Mixer *m = fresh();
    int i;
    m->dmem[0x940] = 0x01;
    m->dmem[0x941] = 0xFF;
    mix_setbuff(m, 0x940, 0x580, 32);
    mix_s8dec(m, MIX_F_INIT, 0x2000);
    CHECK_EQ(G(0x5A0), 256);
    CHECK_EQ(G(0x5A2), -256);

    /* filter: c[0] = 0x4000; init averages with zero -> 0x2000:
     * (1000 * 0x2000 + 0x4000) >> 15 = 250; next update 0x3000 -> 375 */
    m = fresh();
    ((int16_t *)&g_rdram[0x100])[0] = 0x4000;
    for (i = 0; i < 16; i++) {
        S(0x3C0 + i * 2, 1000);
    }
    mix_filter(m, 2, 32, 0x100);
    mix_filter(m, MIX_F_INIT, 0x3C0, 0x200);
    CHECK_EQ(G(0x3C0), 250);
    for (i = 0; i < 16; i++) {
        S(0x3C0 + i * 2, 1000);
    }
    mix_filter(m, 0, 0x3C0, 0x200);
    CHECK_EQ(G(0x3C0), 375);

    /* load / save */
    m = fresh();
    for (i = 0; i < 8; i++) {
        ((int16_t *)&g_rdram[0x800])[i] = (int16_t)(i * 3);
    }
    mix_loadbuff(m, 16, 0x400, 0x800);
    CHECK_EQ(G(0x40E), 21);
    mix_savebuff(m, 16, 0x400, 0x900);
    CHECK_EQ(memcmp(&g_rdram[0x800], &g_rdram[0x900], 16), 0);
    mix_loadbuff(m, 16, 0x400, 0xFFFFFFF0u); /* unmapped */
    CHECK_EQ(m->n_bad_addr, 1);
}

/* End-to-end through the word decoder, with the OoT abi.h encodings. */
static void test_abi_decode(void) {
    Mixer *m = fresh();
    int16_t *l = (int16_t *)&g_rdram[0x800];
    int16_t *r = (int16_t *)&g_rdram[0x840];
    int16_t *ai = (int16_t *)&g_rdram[0x900];
    uint32_t list[16];
    int i, n = 0;
    for (i = 0; i < 16; i++) {
        l[i] = (int16_t)(i + 1);
        r[i] = (int16_t)(-i - 1);
    }
    /* aLoadBuffer(0x800, 0x940, 32), aLoadBuffer(0x840, 0xAE0, 32) */
    list[n++] = (20u << 24) | ((32 >> 4) << 16) | 0x940; list[n++] = 0x800;
    list[n++] = (20u << 24) | ((32 >> 4) << 16) | 0xAE0; list[n++] = 0x840;
    /* AudioSynth_HiLoGain(gain 0x20, in 0x940, 0, 32) */
    list[n++] = (14u << 24) | (0x20 << 16) | 32; list[n++] = 0x940u << 16;
    /* aInterleave(0x3C0, 0x940, 0xAE0, 32) */
    list[n++] = (13u << 24) | ((32 >> 4) << 16) | 0x3C0; list[n++] = (0x940u << 16) | 0xAE0;
    /* aSaveBuffer(0x3C0, 0x900, 64) */
    list[n++] = (21u << 24) | ((64 >> 4) << 16) | 0x3C0; list[n++] = 0x900;
    /* op 25 and op 16: counted, no effect */
    list[n++] = (25u << 24); list[n++] = 0;
    list[n++] = (16u << 24); list[n++] = 0;
    audio_abi_run(m, list, n / 2);
    CHECK_EQ(ai[0], 2);
    CHECK_EQ(ai[1], -1);
    CHECK_EQ(ai[30], 32);
    CHECK_EQ(ai[31], -16);
    CHECK_EQ(m->n_cmds, 7);
    CHECK_EQ(m->n_unk19, 1);
    CHECK_EQ(m->n_unknown, 1);
}

/* ---------------------------------------------------- real sample decode */

static uint8_t *slurp(const char *path, long *len) {
    FILE *f = fopen(path, "rb");
    uint8_t *buf;
    if (f == NULL) {
        return NULL;
    }
    fseek(f, 0, SEEK_END);
    *len = ftell(f);
    fseek(f, 0, SEEK_SET);
    buf = (uint8_t *)malloc((size_t)*len);
    if (buf != NULL && fread(buf, 1, (size_t)*len, f) != (size_t)*len) {
        free(buf);
        buf = NULL;
    }
    fclose(f);
    return buf;
}

static uint32_t be32(const uint8_t *p) {
    return (uint32_t)p[0] << 24 | (uint32_t)p[1] << 16 | (uint32_t)p[2] << 8 | p[3];
}

static int16_t be16(const uint8_t *p) {
    return (int16_t)(uint16_t)(p[0] << 8 | p[1]);
}

/* Plain reference VADPCM decoder (independent of mixer.c's loop shape). */
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

static void test_real_sample(const char *root) {
    char path[512];
    long nb = 0, nk = 0, nw = 0;
    uint8_t *boot, *bank, *wave;
    uint32_t font, so, w0, saddr, bookoff, size, bank_off, i, io = 0;
    int32_t order, npred;
    int nframes, f, ninst;
    int16_t *book16 = (int16_t *)&g_rdram[0x1000];
    int16_t *state = (int16_t *)&g_rdram[0x2000];
    int16_t *ref, *got;
    long long sumsq = 0;
    int peak = 0, mism = 0;
    Mixer *m;

    snprintf(path, sizeof(path), "%s/uso/bootup.uso/02_Data.bin", root);
    boot = slurp(path, &nb);
    snprintf(path, sizeof(path), "%s/uso/audio_bank.uso/00_Data.bin", root);
    bank = slurp(path, &nk);
    snprintf(path, sizeof(path), "%s/uso/audio_wave.uso/00_Data.bin", root);
    wave = slurp(path, &nw);
    if (boot == NULL || bank == NULL || wave == NULL) {
        printf("SKIP real sample: no extracted assets under %s\n", root);
        free(boot); free(bank); free(wave);
        return;
    }
    /* gSoundFontTable (bootup Data +0x1D620), entry 0; gSampleBankTable +0x1DFF0 */
    font = be32(boot + 0x1D620 + 0x10);
    ninst = (uint8_t)boot[0x1D620 + 0x10 + 0xC]; /* shortData2 = numInstruments << 8 | numDrums */
    bank_off = be32(boot + 0x1DFF0 + 0x10 + 0x10 * ((uint16_t)be16(boot + 0x1D620 + 0x10 + 0xA) >> 8));
    for (i = 0; i < (uint32_t)ninst && io == 0; i++) {
        io = be32(bank + font + 8 + 4 * i);
    }
    CHECK(io != 0);
    so = be32(bank + font + io + 0x10); /* normalPitchTunedSample.sample */
    w0 = be32(bank + font + so);
    size = w0 & 0xFFFFFF;
    saddr = be32(bank + font + so + 4);
    bookoff = be32(bank + font + so + 0xC);
    order = (int32_t)be32(bank + font + bookoff);
    npred = (int32_t)be32(bank + font + bookoff + 4);
    printf("real sample: font 0 inst %u, codec %u, %u bytes at bank+0x%X, book order %d x %d\n", i - 1, w0 >> 28,
           size, saddr, order, npred);
    CHECK_EQ(w0 >> 28, 0);
    CHECK_EQ(order, 2);
    CHECK(npred >= 1 && npred <= 8);

    m = fresh();
    for (i = 0; i < (uint32_t)(order * npred * 8); i++) {
        book16[i] = be16(bank + font + bookoff + 8 + i * 2); /* to host order */
    }
    nframes = (int)(size / 9);
    if (nframes > 400) {
        nframes = 400;
    }
    memcpy(&g_rdram[0x10000], wave + bank_off + saddr, (size_t)nframes * 9);
    ref = (int16_t *)malloc((size_t)nframes * 16 * 2);
    got = (int16_t *)&g_rdram[0x40000];
    ref_decode(&g_rdram[0x10000], nframes, book16, ref);

    /* Drive it the way AudioSynth_ProcessNote does: LOADADPCM once, then
     * LOADBUFF the compressed frames and ADPCM-decode 4 frames per command. */
    {
        uint32_t list[2 * 4];
        list[0] = (11u << 24) | (uint32_t)(order * npred * 16); list[1] = 0x1000;
        audio_abi_run(m, list, 1);
        for (f = 0; f + 4 <= nframes; f += 4) {
            list[0] = (20u << 24) | ((48 >> 4) << 16) | 0x940; list[1] = 0x10000 + (uint32_t)f * 9;
            list[2] = (8u << 24) | 0x940; list[3] = (0x580u << 16) | 128;
            list[4] = (1u << 24) | ((f == 0 ? MIX_F_INIT : 0) << 16); list[5] = 0x2000;
            list[6] = (21u << 24) | ((128 >> 4) << 16) | 0x5A0; list[7] = 0x40000 + (uint32_t)f * 32;
            audio_abi_run(m, list, 4);
        }
        nframes = f;
    }
    for (i = 0; i < (uint32_t)nframes * 16; i++) {
        int v = got[i];
        if (v != ref[i]) {
            mism++;
        }
        sumsq += (long long)v * v;
        if (v < 0) {
            v = -v;
        }
        if (v > peak) {
            peak = v;
        }
    }
    printf("real sample: %d frames, peak %d, mean square %lld, mismatches vs reference %d\n", nframes, peak,
           nframes ? sumsq / (nframes * 16) : 0, mism);
    CHECK_EQ(mism, 0);
    CHECK(peak > 256);           /* not silent */
    CHECK(sumsq > (long long)nframes * 16 * 100);
    CHECK(peak <= 32767);
    CHECK_EQ(state[15], got[nframes * 16 - 1]);
    free(ref);
    free(boot); free(bank); free(wave);
}

int main(int argc, char **argv) {
    const char *root = argc > 1 ? argv[1] : "../assets/extracted";
    test_clear_move();
    test_addmixer_mix();
    test_interleave_interl();
    test_hilogain();
    test_duplicate();
    test_resample_zoh();
    test_resample();
    test_env();
    test_unk3();
    test_adpcm();
    test_s8_filter_dma();
    test_abi_decode();
    test_real_sample(root);
    printf("%d passed, %d failed\n", g_pass, g_fail);
    return g_fail != 0;
}
