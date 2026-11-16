/*
 * port/audio/mixer.c: clean-room CPU implementation of 1080's audio ABI.
 * See mixer.h for the model and sources, and docs/1080_PC_PORT_PLAN.md
 * section 13 for which opcodes are verified against 1080's own aspMain.
 *
 * Notation in the comments: "ucode 0xNNNN" is the IMEM address of the
 * handler in 1080's aspMain (text at ROM 0xE16EDC loads at IMEM 0x1000;
 * the jump table is the data image's DMEM 0x10, indexed by op & 0x7F).
 */
#include "mixer.h"

#include <string.h>

/* The ucode's resample LUT: aspMain data image (ROM 0xDBF730) + 0xE0. */
const int16_t mixer_resample_lut[64][4] = {
    {   3129,  26285,   3398,    -33 },
    {   2873,  26262,   3679,    -40 },
    {   2628,  26217,   3971,    -48 },
    {   2394,  26150,   4276,    -56 },
    {   2173,  26061,   4592,    -65 },
    {   1963,  25950,   4920,    -74 },
    {   1764,  25817,   5260,    -84 },
    {   1576,  25663,   5611,    -95 },
    {   1399,  25487,   5974,   -106 },
    {   1233,  25291,   6347,   -118 },
    {   1077,  25075,   6732,   -130 },
    {    932,  24838,   7127,   -143 },
    {    796,  24583,   7532,   -156 },
    {    671,  24309,   7947,   -170 },
    {    554,  24016,   8371,   -184 },
    {    446,  23706,   8804,   -198 },
    {    347,  23379,   9246,   -212 },
    {    257,  23036,   9696,   -226 },
    {    174,  22678,  10153,   -240 },
    {     99,  22304,  10618,   -254 },
    {     31,  21917,  11088,   -268 },
    {    -30,  21517,  11564,   -280 },
    {    -84,  21104,  12045,   -293 },
    {   -132,  20679,  12531,   -304 },
    {   -173,  20244,  13020,   -314 },
    {   -210,  19799,  13512,   -323 },
    {   -241,  19345,  14006,   -330 },
    {   -267,  18882,  14501,   -336 },
    {   -289,  18413,  14997,   -340 },
    {   -306,  17937,  15493,   -341 },
    {   -320,  17456,  15988,   -340 },
    {   -330,  16970,  16480,   -337 },
    {   -337,  16480,  16970,   -330 },
    {   -340,  15988,  17456,   -320 },
    {   -341,  15493,  17937,   -306 },
    {   -340,  14997,  18413,   -289 },
    {   -336,  14501,  18882,   -267 },
    {   -330,  14006,  19345,   -241 },
    {   -323,  13512,  19799,   -210 },
    {   -314,  13020,  20244,   -173 },
    {   -304,  12531,  20679,   -132 },
    {   -293,  12045,  21104,    -84 },
    {   -280,  11564,  21517,    -30 },
    {   -268,  11088,  21917,     31 },
    {   -254,  10618,  22304,     99 },
    {   -240,  10153,  22678,    174 },
    {   -226,   9696,  23036,    257 },
    {   -212,   9246,  23379,    347 },
    {   -198,   8804,  23706,    446 },
    {   -184,   8371,  24016,    554 },
    {   -170,   7947,  24309,    671 },
    {   -156,   7532,  24583,    796 },
    {   -143,   7127,  24838,    932 },
    {   -130,   6732,  25075,   1077 },
    {   -118,   6347,  25291,   1233 },
    {   -106,   5974,  25487,   1399 },
    {    -95,   5611,  25663,   1576 },
    {    -84,   5260,  25817,   1764 },
    {    -74,   4920,  25950,   1963 },
    {    -65,   4592,  26061,   2173 },
    {    -56,   4276,  26150,   2394 },
    {    -48,   3971,  26217,   2628 },
    {    -40,   3679,  26262,   2873 },
    {    -33,   3398,  26285,   3129 },
};

/* ---------------------------------------------------------------- helpers */

#define DMEM_MASK (MIXER_DMEM_SIZE - 1)

static int16_t clamp16(int64_t v) {
    if (v > 32767) {
        return 32767;
    }
    if (v < -32768) {
        return -32768;
    }
    return (int16_t)v;
}

int16_t mixer_get16(const Mixer *m, uint32_t dmem) {
    int16_t v;
    memcpy(&v, &m->dmem[dmem & (DMEM_MASK & ~1u)], 2);
    return v;
}

void mixer_set16(Mixer *m, uint32_t dmem, int16_t v) {
    memcpy(&m->dmem[dmem & (DMEM_MASK & ~1u)], &v, 2);
}

static uint8_t get8(const Mixer *m, uint32_t dmem) {
    return m->dmem[dmem & DMEM_MASK];
}

static void dmem_copy_out(const Mixer *m, uint32_t dmem, void *dst, uint32_t n) {
    uint8_t *d = (uint8_t *)dst;
    uint32_t i;
    for (i = 0; i < n; i++) {
        d[i] = m->dmem[(dmem + i) & DMEM_MASK];
    }
}

static void dmem_copy_in(Mixer *m, uint32_t dmem, const void *src, uint32_t n) {
    const uint8_t *s = (const uint8_t *)src;
    uint32_t i;
    for (i = 0; i < n; i++) {
        m->dmem[(dmem + i) & DMEM_MASK] = s[i];
    }
}

static void *dram(Mixer *m, uint32_t addr) {
    void *p = m->translate != NULL ? m->translate(addr, m->translate_user) : NULL;
    if (p == NULL) {
        m->n_bad_addr++;
    }
    return p;
}

/* RSP vector-unit primitives, per lane. */

/* VMULF: round((2*a*b + 0x8000) >> 16), signed clamp. */
static int16_t vmulf(int16_t a, int16_t b) {
    return clamp16(((int64_t)a * b * 2 + 0x8000) >> 16);
}

/* VMUDM: (s16 a * u16 b) >> 16 (never out of range). */
static int16_t vmudm(int16_t a, uint16_t b) {
    return (int16_t)(((int64_t)a * b) >> 16);
}

/* VADD with no carry in: signed saturating add. */
static int16_t vadd(int16_t a, int16_t b) {
    return clamp16((int32_t)a + b);
}

/* VMUDN / VMADN result: ACC[15:0] when ACC[47:16] fits in s16, else 0x0000
 * (negative) or 0xFFFF (positive). */
static int16_t clamp_low(int64_t acc) {
    int64_t hi = acc >> 16;
    if (hi < -32768) {
        return 0;
    }
    if (hi > 32767) {
        return (int16_t)0xFFFF;
    }
    return (int16_t)(uint16_t)(acc & 0xFFFF);
}

/* --------------------------------------------------------------- the ops */

void mixer_init(Mixer *m, MixerTranslate translate, void *user) {
    memset(m, 0, sizeof(*m));
    m->translate = translate;
    m->translate_user = user;
}

/* 0, ucode 0x1F38: writes a status / trace record to DRAM. Audio-neutral;
 * OoT / 1080 synthesis never emits it. */
void mix_spnoop(Mixer *m) {
    (void)m;
}

/* Shared by ADPCM and S8DEC: the 16-sample history block written first. */
static void load_history(Mixer *m, uint8_t flags, uint32_t state_addr, int16_t hist[16]) {
    const void *src = NULL;
    if (flags & MIX_F_INIT) {
        memset(hist, 0, 16 * sizeof(int16_t));
        return;
    }
    src = dram(m, (flags & MIX_F_LOOP) ? m->loop_addr : state_addr);
    if (src != NULL) {
        memcpy(hist, src, 16 * sizeof(int16_t));
    } else {
        memset(hist, 0, 16 * sizeof(int16_t));
    }
}

/* 1: VADPCM decode. SETBUFF in = compressed frames, out = samples, count =
 * output bytes (rounded up to 32 = one frame). The 16 previous samples
 * (zero / loop state / saved state) are written at out first, so the
 * decoded audio starts at out + 32. Frame: header (scale << 4 | predictor),
 * then 8 bytes of 4-bit residuals, or 4 bytes of 2-bit ones (flag 4). */
void mix_adpcm(Mixer *m, uint8_t flags, uint32_t state_addr) {
    uint32_t in = m->in;
    uint32_t out = m->out;
    int32_t n = (int32_t)((m->count + 31u) & ~31u);
    int16_t prev[16];
    int16_t res[16];
    int16_t cur[16];
    int two_bit = (flags & MIX_F_2BIT) != 0;
    void *state;
    int i, j, h;

    load_history(m, flags, state_addr, prev);
    for (i = 0; i < 16; i++) {
        mixer_set16(m, out + i * 2, prev[i]);
    }
    out += 32;

    while (n > 0) {
        uint8_t hdr = get8(m, in++);
        int scale = hdr >> 4;
        const int16_t *cb = &m->book[(hdr & 0xF) * 16];

        if (two_bit) {
            int shift = scale < 14 ? 14 - scale : 0;
            for (i = 0; i < 4; i++) {
                uint8_t b = get8(m, in++);
                for (j = 0; j < 4; j++) {
                    int c = (b >> (6 - 2 * j)) & 3;
                    res[i * 4 + j] = (int16_t)((int16_t)(uint16_t)(c << 14) >> shift);
                }
            }
        } else {
            int shift = scale < 12 ? 12 - scale : 0;
            for (i = 0; i < 8; i++) {
                uint8_t b = get8(m, in++);
                res[i * 2] = (int16_t)((int16_t)(uint16_t)((b & 0xF0) << 8) >> shift);
                res[i * 2 + 1] = (int16_t)((int16_t)(uint16_t)((b & 0x0F) << 12) >> shift);
            }
        }

        /* Order-2 predictor, two groups of 8. cb[0..7] weights sample n-2,
         * cb[8..15] weights sample n-1 and (reversed) the group's earlier
         * residuals. Q11, truncating shift, clamp. */
        for (h = 0; h < 2; h++) {
            const int16_t *r = &res[h * 8];
            int16_t s2 = h == 0 ? prev[14] : cur[6];
            int16_t s1 = h == 0 ? prev[15] : cur[7];
            for (i = 0; i < 8; i++) {
                int64_t acc = ((int64_t)r[i] << 11) + (int64_t)cb[i] * s2 + (int64_t)cb[8 + i] * s1;
                for (j = 0; j < i; j++) {
                    acc += (int64_t)cb[8 + i - 1 - j] * r[j];
                }
                cur[h * 8 + i] = clamp16(acc >> 11);
            }
        }
        for (i = 0; i < 16; i++) {
            mixer_set16(m, out + i * 2, cur[i]);
        }
        memcpy(prev, cur, sizeof(prev));
        out += 32;
        n -= 32;
    }

    state = dram(m, state_addr);
    if (state != NULL) {
        memcpy(state, prev, sizeof(prev));
    }
}

/* 2, ucode 0x1148: zero count bytes (0 = nothing; else rounded up to 16). */
void mix_clearbuff(Mixer *m, uint16_t dmem, uint16_t count) {
    int32_t n = count;
    uint32_t i;
    if (count == 0) {
        return;
    }
    while (n > 0) {
        for (i = 0; i < 16; i++) {
            m->dmem[(dmem + i) & DMEM_MASK] = 0;
        }
        dmem += 16;
        n -= 16;
    }
}

/* 3, ucode 0x1EE8 (OoT AudioSynth_UnkCmd3, used for bookOffset 2).
 * a = w1 >> 16, b = w1 & 0xFFFF, count = w0 & 0xFFFF bytes (do-while, 32 per
 * pass). Per 16-sample block, with x = A[k] (signed) and y = A[8 + k]
 * (UNSIGNED):  vmudn  B[8 + k] = clamp_low(y * x)
 *              vmadn  B[k]     = clamp_low(2 * y * x)
 * This is a distortion / ring-mod style effect, not a no-op (SoH treats it
 * as a no-op). Read straight off the disassembly; still wants an emulator
 * capture (section 13). */
void mix_unk3(Mixer *m, uint16_t dmem_a, uint16_t dmem_b, uint16_t count) {
    int32_t n = count;
    uint32_t a = dmem_a, b = dmem_b;
    int16_t x[8];
    uint16_t y[8];
    int k;
    do {
        for (k = 0; k < 8; k++) {
            x[k] = mixer_get16(m, a + k * 2);
            y[k] = (uint16_t)mixer_get16(m, a + 16 + k * 2);
        }
        for (k = 0; k < 8; k++) {
            int64_t p = (int64_t)y[k] * x[k];
            mixer_set16(m, b + 16 + k * 2, clamp_low(p));
            mixer_set16(m, b + k * 2, clamp_low(2 * p));
        }
        a += 32;
        b += 32;
        n -= 32;
    } while (n > 0);
}

/* 4, ucode 0x1DFC: out[i] = sat(out[i] + in[i]). count = (w0 >> 12) & 0xFF0
 * bytes, do-while 64 per pass. The w0 low half (0x7FFF from OoT) is
 * ignored by the ucode. */
void mix_addmixer(Mixer *m, uint16_t count, uint16_t dmem_in, uint16_t dmem_out) {
    int32_t n = count;
    uint32_t i;
    do {
        for (i = 0; i < 32; i++) {
            uint32_t o = dmem_out + i * 2;
            mixer_set16(m, o, vadd(mixer_get16(m, o), mixer_get16(m, dmem_in + i * 2)));
        }
        dmem_in += 64;
        dmem_out += 64;
        n -= 64;
    } while (n > 0);
}

/* 5, ucode 0x14F0: 4-tap polyphase resample. SETBUFF in / out / count
 * (output bytes, do-while 16 per pass). pitch is Q1.15 (0x8000 = 1.0); the
 * position steps by pitch * 2 in Q16, and the LUT row is the top 6 bits of
 * the fraction. Each tap is a VMULF (rounded), the taps are summed in pairs
 * with saturating VADDs: out = sat(sat(t0 + t1) + sat(t2 + t3)).
 * State (32 bytes at state_addr): [0..3] the last 4 input samples, [4] the
 * fraction; the rest is carried through. Flag 1 = init (zero history and
 * fraction). History is written into DMEM just before `in`:
 *   default: 4 samples at in - 8;
 *   flag 2:  s[0], s[2] at in - 4 (2:1 decimated history);
 *   flag 4:  s[0] s[0] s[1] s[1] s[2] s[2] s[3] s[3] at in - 16.
 * Flags 2 / 4 are read off the ucode but unused by 1080, and whether they
 * also change the stepping is unverified (section 13). */
void mix_resample(Mixer *m, uint8_t flags, uint16_t pitch, uint32_t state_addr) {
    int16_t st[16];
    uint32_t pos;
    uint32_t out = m->out;
    int32_t n = m->count;
    uint32_t frac;
    void *state = dram(m, state_addr);
    int i;

    if (!(flags & MIX_F_INIT) && state != NULL) {
        memcpy(st, state, sizeof(st));
    } else {
        memset(st, 0, sizeof(st));
    }

    if (flags & 2) {
        pos = m->in - 4u;
        mixer_set16(m, pos, st[0]);
        mixer_set16(m, pos + 2, st[2]);
    } else if (flags & 4) {
        pos = m->in - 16u;
        for (i = 0; i < 8; i++) {
            mixer_set16(m, pos + i * 2, st[i >> 1]);
        }
    } else {
        pos = m->in - 8u;
        for (i = 0; i < 4; i++) {
            mixer_set16(m, pos + i * 2, st[i]);
        }
    }

    frac = (uint16_t)st[4];
    do {
        for (i = 0; i < 8; i++) {
            const int16_t *c = mixer_resample_lut[frac >> 10];
            int16_t t0 = vmulf(mixer_get16(m, pos), c[0]);
            int16_t t1 = vmulf(mixer_get16(m, pos + 2), c[1]);
            int16_t t2 = vmulf(mixer_get16(m, pos + 4), c[2]);
            int16_t t3 = vmulf(mixer_get16(m, pos + 6), c[3]);
            mixer_set16(m, out, vadd(vadd(t0, t1), vadd(t2, t3)));
            out += 2;
            frac += (uint32_t)pitch << 1;
            pos += (frac >> 16) * 2;
            frac &= 0xFFFF;
        }
        n -= 16;
    } while (n > 0);

    for (i = 0; i < 4; i++) {
        st[i] = mixer_get16(m, pos + i * 2);
    }
    st[4] = (int16_t)frac;
    if (state != NULL) {
        memcpy(state, st, sizeof(st));
    }
}

/* 6, ucode 0x1E6C: zero-order hold. SETBUFF in / out / count (output bytes,
 * do-while 8 per pass). Position = in << 16 | pitch_accu in bytes, step
 * pitch << 2; sample address = (pos >> 16) & ~1. No state is saved. */
void mix_resample_zoh(Mixer *m, uint16_t pitch, uint16_t pitch_accu) {
    uint32_t pos = ((uint32_t)m->in << 16) | pitch_accu;
    uint32_t step = (uint32_t)pitch << 2;
    uint32_t out = m->out;
    int32_t n = m->count;
    int i;
    do {
        for (i = 0; i < 4; i++) {
            mixer_set16(m, out, mixer_get16(m, (pos >> 16) & ~1u));
            out += 2;
            pos += step;
        }
        n -= 8;
    } while (n > 0);
}

/* 7: 8-tap FIR low/high-pass (OoT AudioSynth_LoadFilterSize / Buffer).
 * flags > 1: set the byte count (rounded up to 16) and load 8 coefficients
 *            from addr.
 * flags 0/1: filter count bytes at dmem in place. addr = 32-byte state:
 *            [0..7] the last 8 input samples, [8..15] the coefficients used
 *            last time. The coefficients used are the average of the
 *            previous and the new ones (a one-update crossfade); flag 1
 *            zeroes the state first. y[i] = clamp((sum_j x[i-7+j] *
 *            c[7-j] + 0x4000) >> 15).
 * NOT yet checked against the 1080 ucode (handler 0x1CAC): rounding and
 * the averaging are from the public behaviour description (section 13). */
void mix_filter(Mixer *m, uint8_t flags, uint16_t count_or_dmem, uint32_t addr) {
    if (flags > 1) {
        const void *src = dram(m, addr);
        m->filter_count = (uint16_t)((count_or_dmem + 15u) & ~15u);
        if (src != NULL) {
            memcpy(m->filter_coef, src, sizeof(m->filter_coef));
        }
    } else {
        int16_t st[16];
        int16_t c[8];
        int16_t x[16];
        uint32_t buf = count_or_dmem;
        int32_t n = m->filter_count;
        void *state = dram(m, addr);
        int i, j;

        if (!(flags & MIX_F_INIT) && state != NULL) {
            memcpy(st, state, sizeof(st));
        } else {
            memset(st, 0, sizeof(st));
        }
        for (i = 0; i < 8; i++) {
            c[i] = (int16_t)(((int32_t)st[8 + i] + m->filter_coef[i]) >> 1);
        }
        memcpy(x, st, 8 * sizeof(int16_t));
        do {
            for (i = 0; i < 8; i++) {
                x[8 + i] = mixer_get16(m, buf + i * 2);
            }
            for (i = 0; i < 8; i++) {
                int64_t acc = 0x4000;
                for (j = 0; j < 8; j++) {
                    acc += (int64_t)x[i + 1 + j] * c[7 - j];
                }
                mixer_set16(m, buf + i * 2, clamp16(acc >> 15));
            }
            memcpy(x, x + 8, 8 * sizeof(int16_t));
            buf += 16;
            n -= 16;
        } while (n > 0);
        memcpy(st, x, 8 * sizeof(int16_t));
        memcpy(st + 8, c, 8 * sizeof(int16_t));
        if (state != NULL) {
            memcpy(state, st, sizeof(st));
        }
    }
}

/* 8, ucode 0x1178: the flags byte is ignored. */
void mix_setbuff(Mixer *m, uint16_t in, uint16_t out, uint16_t count) {
    m->in = in;
    m->out = out;
    m->count = count;
}

/* 9, ucode 0x17C8: copy the 128 bytes at src to dst, `copies` times, but at
 * least once (do-while). SoH writes copies + 1; the ucode writes copies. */
void mix_duplicate(Mixer *m, uint8_t copies, uint16_t dmem_src, uint16_t dmem_dst) {
    uint8_t tmp[128];
    int k = copies;
    dmem_copy_out(m, dmem_src, tmp, sizeof(tmp));
    do {
        dmem_copy_in(m, dmem_dst, tmp, sizeof(tmp));
        dmem_dst += 128;
    } while (--k > 0);
}

/* 10, ucode 0x11E4: forward copy in 16-byte chunks (0 = nothing; else
 * rounded up to 16). */
void mix_dmemmove(Mixer *m, uint16_t dmem_in, uint16_t dmem_out, uint16_t count) {
    int32_t n = count;
    uint8_t tmp[16];
    if (count == 0) {
        return;
    }
    while (n > 0) {
        dmem_copy_out(m, dmem_in, tmp, 16);
        dmem_copy_in(m, dmem_out, tmp, 16);
        dmem_in += 16;
        dmem_out += 16;
        n -= 16;
    }
}

/* 11: load `count` bytes of codebook (host-order s16) from addr. */
void mix_loadadpcm(Mixer *m, uint32_t count, uint32_t addr) {
    const void *src = dram(m, addr);
    if (count > sizeof(m->book)) {
        count = sizeof(m->book);
    }
    if (src != NULL) {
        memcpy(m->book, src, count);
    }
}

/* 12, ucode 0x1B30: out = clamp((2*out*0x7FFF + 2*in*gain + 0x8000) >> 16)
 * (VMULF by 0x7FFF then VMACF). count = (w0 >> 12) & 0xFF0 bytes, do-while
 * 32 per pass. No special case for gain -0x8000. */
void mix_mixer(Mixer *m, uint16_t count, int16_t gain, uint16_t dmem_in, uint16_t dmem_out) {
    int32_t n = count;
    uint32_t i;
    do {
        for (i = 0; i < 16; i++) {
            uint32_t o = dmem_out + i * 2;
            int64_t acc = (int64_t)mixer_get16(m, o) * 0x7FFF * 2 + (int64_t)mixer_get16(m, dmem_in + i * 2) * gain * 2 +
                          0x8000;
            mixer_set16(m, o, clamp16(acc >> 16));
        }
        dmem_in += 32;
        dmem_out += 32;
        n -= 32;
    } while (n > 0);
}

/* 13, ucode 0x118C: out = L0 R0 L1 R1 ... count = (w0 >> 12) & 0xFF0 = the
 * number of output s16 (L + R), do-while 8 per pass. */
void mix_interleave(Mixer *m, uint16_t count, uint16_t dmem_out, uint16_t left, uint16_t right) {
    int32_t n = count;
    int16_t l[4], r[4];
    int i;
    do {
        for (i = 0; i < 4; i++) {
            l[i] = mixer_get16(m, left + i * 2);
            r[i] = mixer_get16(m, right + i * 2);
        }
        for (i = 0; i < 4; i++) {
            mixer_set16(m, dmem_out + i * 4, l[i]);
            mixer_set16(m, dmem_out + i * 4 + 2, r[i]);
        }
        left += 8;
        right += 8;
        dmem_out += 16;
        n -= 8;
    } while (n > 0);
}

/* 14, ucode 0x1C58: in place at w1 >> 16 (the w1 low half is ignored):
 * s = clamp((s * gain) >> 4), gain UQ4.4 (VMUDM by the fraction << 12, then
 * VMADH by the integer part). count = w0 & 0xFFFF, do-while 32 per pass. */
void mix_hilogain(Mixer *m, uint8_t gain, uint16_t count, uint16_t dmem) {
    int32_t n = count;
    uint32_t i;
    do {
        for (i = 0; i < 16; i++) {
            uint32_t a = dmem + i * 2;
            mixer_set16(m, a, clamp16(((int32_t)mixer_get16(m, a) * gain) >> 4));
        }
        dmem += 32;
        n -= 32;
    } while (n > 0);
}

/* 15, ucode 0x121C: the ucode keeps w1 & 0xFFFFFF; the port keeps the whole
 * word so translate() can map host pointers. */
void mix_setloop(Mixer *m, uint32_t addr) {
    m->loop_addr = addr;
}

/* 17, ucode 0x182C: out[k] = in[2k] (2:1 decimation). count = output
 * samples, do-while 8 per pass. */
void mix_interl(Mixer *m, uint16_t count, uint16_t dmem_in, uint16_t dmem_out) {
    int32_t n = count;
    int i;
    do {
        for (i = 0; i < 8; i++) {
            mixer_set16(m, dmem_out + i * 2, mixer_get16(m, dmem_in + i * 4));
        }
        dmem_in += 32;
        dmem_out += 16;
        n -= 8;
    } while (n > 0);
}

/* 18, ucode 0x1A1C: clears all of the ucode's volume vector (v1), then sets
 * the wet volume lanes to wet << 8 and wet << 8 + rate_wet. */
void mix_envsetup1(Mixer *m, uint8_t wet, uint16_t rate_wet, uint16_t rate_left, uint16_t rate_right) {
    memset(m->env_vol, 0, sizeof(m->env_vol));
    m->env_wet[0] = (uint16_t)(wet << 8);
    m->env_wet[1] = (uint16_t)(m->env_wet[0] + rate_wet);
    m->env_rate_wet = rate_wet;
    m->env_rate[0] = rate_left;
    m->env_rate[1] = rate_right;
}

/* 22, ucode 0x1A44: the dry volume lanes: vol for samples 0-7 of each
 * 16-sample block and vol + rate for samples 8-15. */
void mix_envsetup2(Mixer *m, uint16_t vol_left, uint16_t vol_right) {
    m->env_vol[0][0] = vol_left;
    m->env_vol[0][1] = (uint16_t)(vol_left + m->env_rate[0]);
    m->env_vol[1][0] = vol_right;
    m->env_vol[1][1] = (uint16_t)(vol_right + m->env_rate[1]);
}

/* 19, ucode 0x1880: envelope mixer. w0: in = ((w0 >> 16) & 0xFF) << 4,
 * count = (w0 >> 8) & 0xFF samples (do-while 16 per pass), bit 4 swaps the
 * wet sends, bit 1 / bit 0 negate (xor -1) the left / right dry signal,
 * bit 3 / bit 2 xor the left / right wet send with -4 / -2. dests (w1) =
 * four DMEM >> 4 bytes: dry L, dry R, wet L, wet R.
 * Per sample: d = (in * vol) >> 16 ^ neg; dry += d (saturating);
 *             w = (d * wet) >> 16 ^ negw; wet += w (saturating).
 * The volume lanes advance by 2 * rate after every 16 samples (wrapping
 * u16 adds), and stay advanced (the ucode updates v1 in place). The ucode
 * also doubles its rate registers in place (add s5,s5,s5), so a second
 * ENVMIXER without a new ENVSETUP1 ramps twice as fast; kept here. */
void mix_envmixer(Mixer *m, uint32_t w0, uint32_t dests) {
    uint32_t in = ((w0 >> 16) & 0xFF) << 4;
    int32_t n = (w0 >> 8) & 0xFF;
    int swap = (w0 & 0x10) != 0;
    int16_t neg_l = (w0 & 2) ? -1 : 0;
    int16_t neg_r = (w0 & 1) ? -1 : 0;
    int16_t neg_wl = (w0 & 8) ? -4 : 0;
    int16_t neg_wr = (w0 & 4) ? -2 : 0;
    uint32_t dry_l = ((dests >> 24) & 0xFF) << 4;
    uint32_t dry_r = ((dests >> 16) & 0xFF) << 4;
    uint32_t wet_l = ((dests >> 8) & 0xFF) << 4;
    uint32_t wet_r = (dests & 0xFF) << 4;
    uint16_t step_l, step_r, step_w;
    int h, k;

    m->env_rate[0] = (uint16_t)(m->env_rate[0] * 2);
    m->env_rate[1] = (uint16_t)(m->env_rate[1] * 2);
    m->env_rate_wet = (uint16_t)(m->env_rate_wet * 2);
    step_l = m->env_rate[0];
    step_r = m->env_rate[1];
    step_w = m->env_rate_wet;

    do {
        for (h = 0; h < 2; h++) {
            for (k = 0; k < 8; k++) {
                uint32_t o = (uint32_t)(h * 8 + k) * 2;
                int16_t s = mixer_get16(m, in + o);
                int16_t a = (int16_t)(vmudm(s, m->env_vol[0][h]) ^ neg_l);
                int16_t b = (int16_t)(vmudm(s, m->env_vol[1][h]) ^ neg_r);
                int16_t wa = (int16_t)(vmudm(a, m->env_wet[h]) ^ neg_wl);
                int16_t wb = (int16_t)(vmudm(b, m->env_wet[h]) ^ neg_wr);
                mixer_set16(m, dry_l + o, vadd(mixer_get16(m, dry_l + o), a));
                mixer_set16(m, dry_r + o, vadd(mixer_get16(m, dry_r + o), b));
                mixer_set16(m, wet_l + o, vadd(mixer_get16(m, wet_l + o), swap ? wb : wa));
                mixer_set16(m, wet_r + o, vadd(mixer_get16(m, wet_r + o), swap ? wa : wb));
            }
        }
        for (h = 0; h < 2; h++) {
            m->env_vol[0][h] = (uint16_t)(m->env_vol[0][h] + step_l);
            m->env_vol[1][h] = (uint16_t)(m->env_vol[1][h] + step_r);
            m->env_wet[h] = (uint16_t)(m->env_wet[h] + step_w);
        }
        in += 32;
        dry_l += 32;
        dry_r += 32;
        wet_l += 32;
        wet_r += 32;
        n -= 16;
    } while (n > 0);
}

/* 20, ucode 0x1A80: DMA count = (w0 >> 12) & 0xFF0 bytes into DMEM. */
void mix_loadbuff(Mixer *m, uint16_t count, uint16_t dmem, uint32_t addr) {
    const void *src;
    if (count == 0) {
        return;
    }
    src = dram(m, addr);
    if (src != NULL) {
        dmem_copy_in(m, dmem, src, count);
    }
}

/* 21, ucode 0x1A90: DMA count bytes out of DMEM. */
void mix_savebuff(Mixer *m, uint16_t count, uint16_t dmem, uint32_t addr) {
    void *dst;
    if (count == 0) {
        return;
    }
    dst = dram(m, addr);
    if (dst != NULL) {
        dmem_copy_out(m, dmem, dst, count);
    }
}

/* 23: signed 8-bit PCM to s16 (byte << 8), with the same 16-sample history
 * block and state handling as ADPCM. count = output bytes, rounded up to
 * 32. Not yet checked against the ucode (handler 0x1B9C). */
void mix_s8dec(Mixer *m, uint8_t flags, uint32_t state_addr) {
    uint32_t in = m->in;
    uint32_t out = m->out;
    int32_t n = (int32_t)((m->count + 31u) & ~31u);
    int16_t hist[16];
    void *state;
    int i;

    load_history(m, flags, state_addr, hist);
    for (i = 0; i < 16; i++) {
        mixer_set16(m, out + i * 2, hist[i]);
    }
    out += 32;
    while (n > 0) {
        for (i = 0; i < 16; i++) {
            hist[i] = (int16_t)(uint16_t)(get8(m, in++) << 8);
            mixer_set16(m, out + i * 2, hist[i]);
        }
        out += 32;
        n -= 32;
    }
    state = dram(m, state_addr);
    if (state != NULL) {
        memcpy(state, hist, sizeof(hist));
    }
}

/* 25 (OoT AudioSynth_UnkCmd19, bookOffset 3). 1080's jump table has only
 * 24 entries (ops 0-23); op 25 reads the word after it (0x0F00), which
 * lands mid-loop in op 3's handler (IMEM 0x1F00) with whatever registers
 * the previous command left. That is undefined, so the port treats it as a
 * no-op and counts it (n_unk19). If an emulator capture ever shows 1080
 * emitting op 25, revisit. */
void mix_unk19(Mixer *m, uint32_t w0, uint32_t w1) {
    (void)w0;
    (void)w1;
    m->n_unk19++;
}
