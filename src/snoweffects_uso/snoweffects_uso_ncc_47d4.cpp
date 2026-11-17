#include "../../include/uso_call.h"  /* NCC donors build without -I include */
/* IDO 7.1 NCC (C++) donor for snoweffects.uso 0x47D4 (the trail emitter update), EXACT (agent-h run 709),
 * REPLACE_FUNC_BODY-spliced over the placeholder in snoweffects_uso.c in both build trees.
 * u4C = +-cam+0x970 (flag 0x80); p / q zeroed; call 0x4374; hits / u80; on = 0x46BC(t); alpha from
 * u160..u168; 0x4714(t, on, prev, alpha); then for each i < stride the point w = lerp(P, Q, i / (stride - 1))
 * (+ the normalized cam+0x318 * u154 and - u158 for i == 1 / 2 in mode 98), written to points[writeOfs + i]
 * and vtx[writeOfs + i].
 * What the bytes needed (runs 694-709, docs/1080_NEW_MODULES.md):
 *  - the x lerp through a `float &` inline that copies the old value (lerpTo), y / z through ONE inline
 *    sharing a single copy (lerp2: w.y / w.z share f0), and a plain x += dv;
 *  - SnowBV3 with no constructor (an inline default ctor is an edg basic block; uopt's callee-saved first-use
 *    cost is 0.25 x the block count), so lim / alpha take f20 / f22;
 *  - no extra named temps: u160 - 5 and the alpha product reuse ab, |u4C| and the u154 scale reuse f (block
 *    locals add frame words, and the variable each piece shares sets its uopt colour);
 *  - the p / q / dv zero stores through the int i (`i = 0; x = i;`): a second zero constant (f18) apart from
 *    the f16 compare zero;
 *  - dv.scale(f) / dv.set(i, i, i) put those inlines' param words ahead of w in the frame (w at sp+72);
 *    operator= returns void (the vel pointer in v0); locals declared in frame order (P 160, Q 148, dv 132). */
extern "C" int gl_func_00000000(...);
struct SnowBV3 {
    float x, y, z;
    void set(float a, float b, float c) { x = a; y = b; z = c; }
    void scale(float s) { x *= s; y *= s; z *= s; }
    void operator=(SnowBV3 o) { x = o.x; y = o.y; z = o.z; }
};
struct SnowVEnt { short d; short pad; void (*fn)(void *); };
struct SnowECam {
    char pad[0x318];
    SnowBV3 v318;
    char pad324[0x348 - 0x324];
    float speed;          /* 348 */
    char pad34C[0x970 - 0x34C];
    float f970;
    char pad974[0x9A2 - 0x974];
    short mode;           /* 9A2 */
    char pad9A4[0xA30 - 0x9A4];
    unsigned char col[4]; /* A30 */
    char padA34[0xA58 - 0xA34];
    int flags;            /* A58 */
};
struct SnowEPoint { SnowBV3 pos; SnowBV3 vel; float w18; float w1C; float w20; float w24; int u28; };
struct SnowESlot { int state; int u4; int u8; };
struct SnowEVtx { short ob[3]; short flag; short tc[2]; unsigned char cn[4]; };
struct SnowEm {
    char pad00[0x28];
    SnowVEnt *vptr;     /* 28 */
    SnowECam *cam;      /* 2C */
    char pad30[0x34 - 0x30];
    float endXInit, endZInit;
    float endX, endZ;     /* 3C 40 */
    char pad44[0x4C - 0x44];
    float lean;
    char pad50[0x5C - 0x50];
    int writeOfs;       /* 5C */
    int read;           /* 60 */
    int write;          /* 64 */
    int size;           /* 68 */
    int stride;         /* 6C */
    int count;          /* 70 */
    int on;             /* 74 */
    int prevOn;           /* 78 */
    int prevState;       /* 7C */
    int noGround;            /* 80 */
    char pad84[0x8C - 0x84];
    SnowEVtx *vtx;      /* 8C */
    SnowEPoint *points; /* 90 */
    SnowESlot *slots;   /* 94 */
    char pad98[0xA8 - 0x98];
    int texW;
    char padAC[0x120 - 0xAC];
    SnowBV3 p;          /* 120 */
    SnowBV3 q;          /* 12C */
    char pad138[0x144 - 0x138];
    int hits;           /* 144 */
    char pad148[0x154 - 0x148];
    float velOfs, dropY;
    char pad15C[0x160 - 0x15C];
    float alphaSpeed, alpha, alphaLean, alphaDecay;
};
extern "C" int gl_func_00000000_snow_46bc(SnowEm *);  /* 0x46BC, typed blank jal */
extern "C" void gl_func_00000000_snow_4714(SnowEm *, int, int, float);
static inline void lerpTo(float &a, float f, float b) { float c = a; a = (b - c) * f + c; }
static inline void lerp2(float &a, float &a2, float f, float b, float b2) { float c = a; a = (b - c) * f + c; c = a2; a2 = (b2 - c) * f + c; }
static inline void addTo(float &a, float b) { float c = a; a = c + b; }
extern "C" void SnowRing_emit_000047D4(SnowEm *t) {
    SnowECam *cam;
    float alpha, lim;
    SnowBV3 P, Q;
    int i;
    SnowBV3 dv;
    float a, f, ab;
    cam = t->cam;
    if (cam->flags & 0x80) {
        t->lean = -cam->f970;
    } else {
        t->lean = cam->f970;
    }
    i = 0; t->p.x = i; t->p.y = i; t->p.z = i;
    t->q.x = i; t->q.y = i; t->q.z = i;
    t->endX = t->endXInit;
    t->endZ = t->endZInit;
    CALL(SnowRing_updateBasis_00004374)(t);  /* 0x4374 */
    if (t->hits <= 0) {
        t->noGround = 1;
    } else {
        t->noGround = 0;
    }
    cam = t->cam;
    if ((cam->mode != 98 && cam->mode != 97) || cam->speed < 5.0f) {
        t->hits = 0;
    }
    t->on = XCALL(gl_func_00000000_snow_46bc, SnowRing_emitTick_000046BC)(t);
    cam = t->cam;
    if (cam->speed < t->alphaSpeed) {
        ab = t->alphaSpeed - 5.0f; a = t->alpha / ab * (cam->speed - 5.0f);
    } else {
        a = t->alpha;
    }
    if (t->lean < 0.0f) {
        f = -t->lean;
    } else {
        f = t->lean;
    }
    ab = f * t->alphaLean; alpha = ab * (255.0f - a) + a;
    if (cam->mode == 98) {
        t->slots[t->write].u4 = 1;
    } else {
        t->slots[t->write].u4 = 0;
    }
    P = t->p;
    Q = t->q;
    XCALL(gl_func_00000000_snow_4714, SnowRing_setSlotState_00004714)(t, t->on, t->prevOn, alpha);
    if (t->slots[t->write].state == 2) {
        alpha = 0.0f;
    }
    if (t->stride == 4 && (cam = t->cam)->mode == 98) {
        lim = t->dropY;
        dv = cam->v318;
        CALL(math_vec3Normalize)(&dv);  /* math_vec3Normalize */
        f = t->velOfs; dv.scale(f);
    } else {
        lim = 0.0f;
        i = 0; dv.set(i, i, i);
    }
    if (t->on != 0) {
        for (i = 0; i < t->stride; i++) {
            SnowBV3 w = P;
            f = (float)i / (float)(t->stride - 1);
            lerpTo(w.x, f, Q.x);
            lerp2(w.y, w.z, f, Q.y, Q.z);
            if (i == 1 || i == 2) {
                w.x += dv.x; w.y += dv.y; w.z += dv.z;
                addTo(w.y, -lim);
            }
            t->points[t->writeOfs + i].pos = w;
            t->points[t->writeOfs + i].w18 = 0.0f;
            t->points[t->writeOfs + i].vel.set(0.0f, 0.0f, 0.0f);
            t->points[t->writeOfs + i].u28 = 0;
            t->points[t->writeOfs + i].w1C = alpha;
            t->points[t->writeOfs + i].w20 = t->points[t->writeOfs + i].w1C / ((float)t->size / t->alphaDecay);
            t->vtx[t->writeOfs + i].tc[0] = (int)(((float)t->texW - 1.0f) * ((float)i / (float)(t->stride - 1))) << 6;
            t->vtx[t->writeOfs + i].tc[1] = ((t->writeOfs & t->stride) >> 1) * 31U << 6;
            t->vtx[t->writeOfs + i].cn[0] = t->cam->col[0];
            t->vtx[t->writeOfs + i].cn[1] = t->cam->col[1];
            t->vtx[t->writeOfs + i].cn[2] = t->cam->col[2];
        }
    }
    t->vptr[16].fn((void *)((int)t->vptr[16].d + (int)t));
    t->prevOn = t->on;
    t->prevState = t->cam->mode;
}
