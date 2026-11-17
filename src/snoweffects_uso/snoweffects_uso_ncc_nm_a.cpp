/* IDO 7.1 NCC (C++) NON_MATCHING donor for snoweffects.uso 0x7FE0 (agent-a runs 661 / 665 / 669 / 677 / 681 / 688),
 * spliced over the placeholder on the non_matching build only (Makefile SNOW_NM_A_FUNCS): 800 / 800 words,
 * objdiff 97.8 %. A donor body has to be exactly the target's length (a size change shifts the later
 * functions of the non_matching object). 0xA10C moved to the exact donor snoweffects_uso_ncc_a.cpp (run 677).
 */
#include "snoweffects_ncc.h"
/* the callees, by name (a matching donor would call them through typed 0-pinned aliases) */
extern "C" {
void snoweffects_uso_func_00004374(...);
int snoweffects_uso_func_000046BC(...);
}

/* ---- 0x7FE0 trail emitter update (decoded whole, first NCC draft: 789 / 800 instructions,
 * frame 320 vs 384, far from the 80 % NM threshold). Structure: speed (+0x4C) = +-cam lean (+0x970,
 * sign from cam flags +0xA58 & 0x80), clear +0x120..+0x134, copy +0x34/+0x38 -> +0x3C/+0x40,
 * 0x4374(this); s = 0 below cam speed 60 (+0x348) or |speed| < lo (+0x1A4), +-1 above hi (+0x1A8),
 * else +-((1 - base) * t + base); sign flips clear +0x144; s -> +0x1A0; state (+0x9A2) != 97 or slow
 * or s == 0 clears +0x144; +0x74 = 0x46BC(this), slots[write].unk04[0] = mode (+0x1EC);
 * 0x4714(this, on, prevOn, w = +0x164); s *= 0.25 when the write slot is live; then for each of the
 * stride (+0x6C) points of the write ring row: pos = +0x138 (mode 2) or lerp(+0x120, +0x12C, i/(n-1)),
 * vel = cam +0x318 * +0x188 plus side / lift terms (cam +0x3B0 * +0x180 * |s|, +0x114 * (i or n-1-i)/n
 * * +0x184 * |s|, negated when s > 0), w[1..3], vtx s/t (texStep +0xA8, *3 unless mode 2, * 64) and
 * the cam colour bytes (+0xA30..; points 1 and 2 faded towards 255 by +0x178, 0 in state 98).
 * Edg temp chains: op* result -> return temp (sp+244) -> by-value operator= param (sp+188). Next:
 * the frame (target named Vec3 locals at 336 / 324 / 312 / 300 / 284 / 272 / 100) and in-loop
 * 64.0f. Run 669: t->speed read once into v before the cam-speed test (target f0 at 0x8064): 85.49 -> 85.74.
 * Left at the |speed| compare: the target's zero for the +0x120 clears is a ugen f0 constant and the
 * ramp's 0.0f is rematerialised per arm into f24 (bc1tl + two mtc1 zero); here one 0.0f LR spans the
 * 0x4374 call in f20.
 * Run 677 (85.7 -> 93.1 %, 811 -> 799 words):
 * - the +0x120..+0x134 clears through an int local (`int z = 0`): their zero is its own f0 web, and the
 *   ramp zero becomes the f24 constant rematerialised per arm (agent-g's run 658 lever).
 * - `speed < 60 || |v| < lo` as one condition: both s = 0 arms share one block (bc1tl into it).
 * - s = +-1 as an if / else, not a ternary (the target assigns f16 in each arm).
 * - d = c * |s| through a TV(const TV &, float) constructor: computed straight into d (-13 words of
 *   op* return-temp / param-temp copy chain).
 * - &t->v114 / &t->vFC as pointer locals before the loop (the target hoists a0+276 / a0+252).
 * - the vtx t value computed before the s store (writeOfs loaded once for both).
 * - the reversed-index test is `0 < s`.
 * Run 681 (93.1 -> 93.5 %, 799 -> 800 words): the lerp is a nested inline, lerp3(p, b, f) calling
 *   lerpf(a, b, t) per component. lerpf's `a` (this->x through the reference) is a pool word; it is
 *   coloured f0 and partly copy-propagated: x uses it twice (the sp+48 reload into f0), y copies p.y into
 *   it (the target's mov.s f0,f10) and reads p.y in place for the subtraction, z is fully propagated.
 *   The fraction passed as an expression is pool word 1 (shared with the |x| / op* scalar words).
 * Run 681 (93.5 -> 97.7 %): the side terms are op* CHAINS, `a = cam->up * f180 * |s|` and
 *   `d(*v114 * k * f184, |s|)`, not named intermediates (up, b, c). The target's operator* words in those
 *   statements are s = word 2, ctor params = words 3 / 1 / 4 (fabs_(s) * f184 reads f0). k is a named
 *   float between a and d (the target's one-word gap there). i / s / v / w declared in that order puts the
 *   s and w spill homes at vfp-8 / vfp-16 as in the target.
 * Run 688 (97.75 -> 97.83 %, frame 368 -> 384 as in the target):
 * - the w[] stores were one index high (target: w at +0x1C, f16C at +0x20, f17C at +0x18): w[0..2].
 * - d's ctor takes the vector BY VALUE and |s| inside: `TV(TV v, float s) { float a = fabs_(s); ... }`.
 *   The arg is then the inline's param word, allocated deep after the *f184 result (target sp+100),
 *   not a third __V temp at the loop-body top, and the word copy precedes the fabs as in the target.
 * - the +0x120.. clears go through `i` (no `int z` block local: one frame word fewer above the __Vs).
 * - two dead floats in lerp3 give the two frame words between the p param temp and fwd * f188's r.
 * Left (frame): up * f180's __V is the lower of the two (target: the higher, sp+336), the a statement
 *   has 2 words fewer ahead of up's r and 1 more after it; the d ctor has its param-word gap 1 / 2
 *   where the target has 2 / 1 (fr: /tmp/claude-1000/agent-a/w688/sl.py, md.py reads -g3 vfp names).
 *   The addiu t3 / t4 vs `or a1` order is uopt's LICM / IV-init order (source line joins and moving the
 *   pointer locals into the loop do not change it).
 * Left: pool word 1 colours f2 where the target has f12 (it wins the f2 race against the TV ctor's second
 *   parameter word; a forcing-uopt probe puts it on f12 for 97.9 %), the frame (368 vs 384: one TV pool
 *   word too many at the loop-body top and one word too many above it) and the hoisted sp addresses. ---- */
extern "C" void SnowRing_setSlotState_00004714(void *, int, int, float);

inline float fabs_(float x) { return x < 0.0f ? -x : x; }
struct TV {
    float x, y, z;
    TV() {}
    TV(float a, float b, float c) { x = a; y = b; z = c; }
    TV(TV v, float s) { float a = fabs_(s); x = v.x * a; y = v.y * a; z = v.z * a; }
    TV operator*(float s) const { TV r(x * s, y * s, z * s); return r; }
    /* by value: `p = t->mid` copies the source into the param temp as words (lw / sw through a3),
     * then member-wise as floats, as in the target (run 665: 79.0 -> 85.5 %) */
    TV &operator=(TV o) { x = o.x; y = o.y; z = o.z; return *this; }
    TV &operator+=(const TV &o) { x += o.x; y += o.y; z += o.z; return *this; }
    TV &operator*=(float s) { x *= s; y *= s; z *= s; return *this; }
};
struct TCam {
    char pad0[0x318];
    /* 0x318 */ TV fwd;
    char pad324[0x348 - 0x324];
    /* 0x348 */ float speed;
    char pad34C[0x3B0 - 0x34C];
    /* 0x3B0 */ TV up;
    char pad3BC[0x970 - 0x3BC];
    /* 0x970 */ float lean;
    char pad974[0x9A2 - 0x974];
    /* 0x9A2 */ short state;
    char pad9A4[0xA30 - 0x9A4];
    /* 0xA30 */ unsigned char rgb[3];
    char padA33[0xA58 - 0xA33];
    /* 0xA58 */ int flags;
};
struct TPoint { TV pos; TV vel; float w[4]; int f28; };
struct TSlot { int state; int unk04[2]; };
struct TVtx { short ob[3]; short flag; short s, t; signed char rgba[4]; };
struct TTrail {
    char pad00[0x2C];
    /* 0x2C */ TCam *cam;
    char pad30[0x34 - 0x30];
    /* 0x34 */ float f34;
    /* 0x38 */ float f38;
    /* 0x3C */ float f3C;
    /* 0x40 */ float f40;
    char pad44[0x4C - 0x44];
    /* 0x4C */ float speed;
    char pad50[0x5C - 0x50];
    /* 0x5C */ int writeOfs;
    /* 0x60 */ int read;
    /* 0x64 */ int write;
    /* 0x68 */ int size;
    /* 0x6C */ int stride;
    /* 0x70 */ int count;
    /* 0x74 */ int on;
    /* 0x78 */ int prevOn;
    /* 0x7C */ int prevState;
    char pad80[0x8C - 0x80];
    /* 0x8C */ TVtx *vtx;
    /* 0x90 */ TPoint *points;
    /* 0x94 */ TSlot *slots;
    char pad98[0xA8 - 0x98];
    /* 0xA8 */ int texStep;
    char padAC[0xFC - 0xAC];
    /* 0xFC */ TV vFC;
    char pad108[0x114 - 0x108];
    /* 0x114 */ TV v114;
    /* 0x120 */ TV a;
    /* 0x12C */ TV b;
    /* 0x138 */ TV mid;
    /* 0x144 */ int hits;
    char pad148[0x164 - 0x148];
    /* 0x164 */ float f164;
    char pad168[0x16C - 0x168];
    /* 0x16C */ float f16C;
    char pad170[0x178 - 0x170];
    /* 0x178 */ float fade;
    /* 0x17C */ float f17C;
    /* 0x180 */ float f180;
    /* 0x184 */ float f184;
    /* 0x188 */ float f188;
    char pad18C[0x19C - 0x18C];
    /* 0x19C */ float f19C;
    /* 0x1A0 */ float s;
    /* 0x1A4 */ float lo;
    /* 0x1A8 */ float hi;
    /* 0x1AC */ float base;
    char pad1B0[0x1EC - 0x1B0];
    /* 0x1EC */ int mode;
};

inline float lerpf(float a, float b, float t) { return (b - a) * t + a; }
inline void lerp3(TV &p, const TV &b, float f) {
    float g = f, h = f; /* unused: the target's two frame words ahead of the fwd * f188 result */
    p.x = lerpf(p.x, b.x, f);
    p.y = lerpf(p.y, b.y, f);
    p.z = lerpf(p.z, b.z, f);
}

extern "C" void snoweffects_uso_func_00007FE0(TTrail *t) {
    int i;
    float s;
    float v;
    float w;
    if (t->cam->flags & 0x80) {
        t->speed = -t->cam->lean;
    } else {
        t->speed = t->cam->lean;
    }
    t->a.x = i = 0; t->a.y = i; t->a.z = i; t->b.x = i; t->b.y = i; t->b.z = i;
    t->f3C = t->f34;
    t->f40 = t->f38;
    snoweffects_uso_func_00004374(t);
    v = t->speed;
    if (t->cam->speed < 60.0f || fabs_(v) < t->lo) {
        s = 0.0f;
    } else if (t->hi < fabs_(v)) {
        if (v < 0.0f) {
            s = -1.0f;
        } else {
            s = 1.0f;
        }
    } else {
        float f = (fabs_(v) - t->lo) / (t->hi - t->lo);
        if (v < 0.0f) {
            s = -((1.0f - t->base) * f + t->base);
        } else {
            s = (1.0f - t->base) * f + t->base;
        }
    }
    if (s < 0.0f && 0.0f < t->s) {
        t->hits = 0;
    }
    if (0.0f < s && t->s < 0.0f) {
        t->hits = 0;
    }
    t->s = s;
    if (t->cam->state != 97 || t->cam->speed < 60.0f || fabs_(s) == 0.0f) {
        t->hits = 0;
    }
    t->on = snoweffects_uso_func_000046BC(t);
    t->slots[t->write].unk04[0] = t->mode;
    w = t->f164;
    SnowRing_setSlotState_00004714(t, t->on, t->prevOn, w);
    if (t->slots[t->write].state == 2) {
        s *= 0.25f;
    }
    if (t->on != 0) {
        float fade = t->fade;
        if (t->cam->state == 98) {
            fade = 0.0f;
        }
        TV *v114 = &t->v114;
        TV *vFC = &t->vFC;
        for (i = 0; i < t->stride; i++) {
            TV p;
            if (t->mode == 2) {
                p = t->mid;
            } else {
                p = t->a;
                lerp3(p, t->b, (float)i / (float)(t->stride - 1));
            }
            t->points[t->writeOfs + i].pos = p;
            t->points[t->writeOfs + i].vel = t->cam->fwd * t->f188;
            t->points[t->writeOfs + i].f28 = 0;
            t->points[t->writeOfs + i].w[1] = w;
            t->points[t->writeOfs + i].w[2] = t->f16C;
            TV a = t->cam->up * t->f180 * fabs_(s);
            float k = (float)(0.0f < s ? t->stride - i - 1 : i) / (float)t->stride;
            TV d(*v114 * k * t->f184, s);
            TV e = *vFC * -1.0f;
            if (0.0f < s) {
                d *= -1.0f;
            }
            t->points[t->writeOfs + i].vel += d;
            if (i == 3) {
                t->points[t->writeOfs + i].w[0] = t->f17C * t->f19C;
            }
            if (i == 2) {
                t->points[t->writeOfs + i].vel += a;
                t->points[t->writeOfs + i].w[0] = t->f17C;
            }
            if (i == 1) {
                t->points[t->writeOfs + i].vel += a;
                t->points[t->writeOfs + i].w[0] = t->f17C;
            }
            if (i == 0) {
                t->points[t->writeOfs + i].w[0] = t->f17C * t->f19C;
            }
            float u = i * t->texStep - 0.5;
            if (t->mode != 2) {
                u *= 3.0f;
            }
            float tv = ((t->writeOfs & t->stride) >> 2) * 31;
            t->vtx[t->writeOfs + i].s = u * 64.0f;
            t->vtx[t->writeOfs + i].t = tv * 64.0f;
            if (i == 1 || i == 2) {
                t->vtx[t->writeOfs + i].rgba[0] = t->cam->rgb[0] + (255.0f - t->cam->rgb[0]) * fade;
                t->vtx[t->writeOfs + i].rgba[1] = t->cam->rgb[1] + (255.0f - t->cam->rgb[1]) * fade;
                t->vtx[t->writeOfs + i].rgba[2] = t->cam->rgb[2] + (255.0f - t->cam->rgb[2]) * fade;
            } else {
                t->vtx[t->writeOfs + i].rgba[0] = t->cam->rgb[0];
                t->vtx[t->writeOfs + i].rgba[1] = t->cam->rgb[1];
                t->vtx[t->writeOfs + i].rgba[2] = t->cam->rgb[2];
            }
        }
    }
    t->prevOn = t->on;
    t->prevState = t->cam->state;
}
