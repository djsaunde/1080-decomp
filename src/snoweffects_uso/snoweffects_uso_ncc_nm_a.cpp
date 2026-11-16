/* IDO 7.1 NCC (C++) draft for snoweffects.uso 0x7FE0 (agent-a runs 661 / 665 / 669), NOT spliced: 811 / 800
 * words (85.7 %), and a size change shifts the later functions of the non_matching object (0xA8D0 then drops
 * to 41.7 %), so a donor body has to be exactly the target's length. 0xA10C moved to the exact donor
 * snoweffects_uso_ncc_a.cpp (run 677). Makefile SNOW_NM_A_FUNCS is empty until 0x7FE0 is 800 words.
 */
#include "snoweffects_ncc.h"
/* the callees, by name (a matching donor would call them through typed 0-pinned aliases); the
 * discarded-result ones are void (an int return leaves a dead v0 def) */
extern "C" {
int ItemMgr_findType_0003EA98(...);
void PolyShape_getNormal8(...);
int PolyShape_getVertex(...);
void snoweffects_uso_func_00004374(...);
int snoweffects_uso_func_000046BC(...);
void snoweffects_uso_func_00009768(...);
void snoweffects_uso_func_00009884(...);
void snoweffects_uso_func_00009BE8(...);
void snoweffects_uso_func_00009C48(...);
void snoweffects_uso_func_00009FE8(...);
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
 * 0x4374 call in f20. ---- */
extern "C" void SnowRing_setSlotState_00004714(void *, int, int, float);

struct TV {
    float x, y, z;
    TV() {}
    TV(float a, float b, float c) { x = a; y = b; z = c; }
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

extern "C" void snoweffects_uso_func_00007FE0(TTrail *t) {
    float s;
    float w;
    int i;
    if (t->cam->flags & 0x80) {
        t->speed = -t->cam->lean;
    } else {
        t->speed = t->cam->lean;
    }
    t->a.x = 0.0f;
    t->a.y = 0.0f;
    t->a.z = 0.0f;
    t->b.x = 0.0f;
    t->b.y = 0.0f;
    t->b.z = 0.0f;
    t->f3C = t->f34;
    t->f40 = t->f38;
    snoweffects_uso_func_00004374(t);
    float v = t->speed;
    if (t->cam->speed < 60.0f) {
        s = 0.0f;
    } else if ((v < 0.0f ? -v : v) < t->lo) {
        s = 0.0f;
    } else if (t->hi < (v < 0.0f ? -v : v)) {
        s = v < 0.0f ? -1.0f : 1.0f;
    } else {
        float f = ((v < 0.0f ? -v : v) - t->lo) / (t->hi - t->lo);
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
    if (t->cam->state != 97 || t->cam->speed < 60.0f || (s < 0.0f ? -s : s) == 0.0f) {
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
        for (i = 0; i < t->stride; i++) {
            TV p;
            if (t->mode == 2) {
                p = t->mid;
            } else {
                p = t->a;
                float f = (float)i / (float)(t->stride - 1);
                p.x = (t->b.x - p.x) * f + p.x;
                p.y = (t->b.y - p.y) * f + p.y;
                p.z = (t->b.z - p.z) * f + p.z;
            }
            t->points[t->writeOfs + i].pos = p;
            t->points[t->writeOfs + i].vel = t->cam->fwd * t->f188;
            t->points[t->writeOfs + i].f28 = 0;
            t->points[t->writeOfs + i].w[2] = w;
            t->points[t->writeOfs + i].w[3] = t->f16C;
            TV up = t->cam->up * t->f180;
            TV a = up * (s < 0.0f ? -s : s);
            TV b = t->v114 * ((float)(s < 0.0f ? t->stride - i - 1 : i) / (float)t->stride);
            TV c = b * t->f184;
            TV d = c * (s < 0.0f ? -s : s);
            TV e = t->vFC * -1.0f;
            if (0.0f < s) {
                d *= -1.0f;
            }
            t->points[t->writeOfs + i].vel += d;
            if (i == 3) {
                t->points[t->writeOfs + i].w[1] = t->f17C * t->f19C;
            }
            if (i == 2) {
                t->points[t->writeOfs + i].vel += a;
                t->points[t->writeOfs + i].w[1] = t->f17C;
            }
            if (i == 1) {
                t->points[t->writeOfs + i].vel += a;
                t->points[t->writeOfs + i].w[1] = t->f17C;
            }
            if (i == 0) {
                t->points[t->writeOfs + i].w[1] = t->f17C * t->f19C;
            }
            float u = i * t->texStep - 0.5;
            if (t->mode != 2) {
                u *= 3.0f;
            }
            t->vtx[t->writeOfs + i].s = u * 64.0f;
            t->vtx[t->writeOfs + i].t = (float)(((t->writeOfs & t->stride) >> 2) * 31) * 64.0f;
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
