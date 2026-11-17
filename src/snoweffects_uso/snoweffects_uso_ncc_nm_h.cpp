/* IDO 7.1 NCC (C++) NON_MATCHING donors for snoweffects.uso 0x47D4 (agent-h run 694) and 0xC90 (moved from
 * agent-c's snoweffects_uso_ncc_c.cpp in run 699, at the end of this file), each spliced over the empty
 * NON_MATCHING placeholder in snoweffects_uso.c on the non_matching build only (the matching build keeps
 * INCLUDE_ASM). The trail emitter update: u4C = +-cam+0x970 (flag 0x80); p / q zeroed; call 0x4374; hits /
 * u80; on = 0x46BC(t); alpha from u160..u168; 0x4714(t, on, prev, alpha); then for each i < stride the point
 * w = lerp(P, Q, i / (stride - 1)) (+ the normalized cam+0x318 * u154 and - u158 for i == 1 / 2 in mode 98),
 * written to points[writeOfs + i] and vtx[writeOfs + i]. The x lerp goes through a `float &` inline that
 * copies the old value (lerpTo), y / z through one inline sharing a single copy (lerp2): w.y / w.z then share
 * one uopt colour (f0, as the target). The x += dv is plain (the target keeps that w.x in a ugen temp)
 * (89.96 -> 94.58 -> 94.91 %). SnowBV3 has no constructor: each inline default constructor is an edg basic
 * block, and uopt's callee-saved first-use cost is 0.25 x the block count, so with it (49 blocks, 12.25) lim was
 * spilled; without it lim takes f20 and alpha f22 as in the target. The named d (u160 - 5) and k (ab * u168)
 * are the target's f0 temps (99.25 %). Left: f outranks the x copy (f in f2, x copy f12; target the reverse),
 * ab / the alpha temp swapped (f2 / f12), and the target's second zero constant (f18) for dv. */
extern "C" int gl_func_00000000(...);
extern "C" char D_00000000[];
struct SnowBV3 {
    float x, y, z;
    void set(float a, float b, float c) { x = a; y = b; z = c; }
    SnowBV3 &operator=(SnowBV3 o) { x = o.x; y = o.y; z = o.z; return *this; }
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
    float u34, u38;
    float len, u40;     /* 3C 40 */
    char pad44[0x4C - 0x44];
    float u4C;
    char pad50[0x5C - 0x50];
    int writeOfs;       /* 5C */
    int read;           /* 60 */
    int write;          /* 64 */
    int size;           /* 68 */
    int stride;         /* 6C */
    int count;          /* 70 */
    int on;             /* 74 */
    int prev;           /* 78 */
    int prevMode;       /* 7C */
    int u80;            /* 80 */
    char pad84[0x8C - 0x84];
    SnowEVtx *vtx;      /* 8C */
    SnowEPoint *points; /* 90 */
    SnowESlot *slots;   /* 94 */
    char pad98[0xA8 - 0x98];
    int uA8;
    char padAC[0x120 - 0xAC];
    SnowBV3 p;          /* 120 */
    SnowBV3 q;          /* 12C */
    char pad138[0x144 - 0x138];
    int hits;           /* 144 */
    char pad148[0x154 - 0x148];
    float u154, u158;
    char pad15C[0x160 - 0x15C];
    float u160, u164, u168, u16C;
};
extern "C" int gl_func_00000000_snow_46bc(SnowEm *);  /* 0x46BC, typed blank jal */
extern "C" void gl_func_00000000_snow_4714(SnowEm *, int, int, float);
static inline void lerpTo(float &a, float f, float b) { float c = a; a = (b - c) * f + c; }
static inline void lerpT1(float &a, float f, float b) { a = (b - a) * f + a; }
static inline void lerp2(float &a, float &a2, float f, float b, float b2) { float c = a; a = (b - c) * f + c; c = a2; a2 = (b2 - c) * f + c; }
static inline void addTo(float &a, float b) { float c = a; a = c + b; }
static inline void subTo(float &a, float b) { float c = a; a = c - b; }
extern "C" void snoweffects_uso_func_000047D4(SnowEm *t) {
    SnowECam *cam;
    float a, alpha, f, ab, drop;
    float lim;
    int i;
    SnowBV3 P, Q, dv, v;
    cam = t->cam;
    if (cam->flags & 0x80) {
        t->u4C = -cam->f970;
    } else {
        t->u4C = cam->f970;
    }
    t->p.x = 0.0f; t->p.y = 0.0f; t->p.z = 0.0f;
    t->q.x = 0.0f; t->q.y = 0.0f; t->q.z = 0.0f;
    t->len = t->u34;
    t->u40 = t->u38;
    gl_func_00000000(t);  /* 0x4374 */
    if (t->hits <= 0) {
        t->u80 = 1;
    } else {
        t->u80 = 0;
    }
    cam = t->cam;
    if ((cam->mode != 98 && cam->mode != 97) || cam->speed < 5.0f) {
        t->hits = 0;
    }
    t->on = gl_func_00000000_snow_46bc(t);
    cam = t->cam;
    if (cam->speed < t->u160) {
        { float d = t->u160 - 5.0f; a = t->u164 / d * (cam->speed - 5.0f); }
    } else {
        a = t->u164;
    }
    if (t->u4C < 0.0f) {
        ab = -t->u4C;
    } else {
        ab = t->u4C;
    }
    { float k = ab * t->u168; alpha = k * (255.0f - a) + a; }
    if (cam->mode == 98) {
        t->slots[t->write].u4 = 1;
    } else {
        t->slots[t->write].u4 = 0;
    }
    P = t->p;
    Q = t->q;
    gl_func_00000000_snow_4714(t, t->on, t->prev, alpha);
    if (t->slots[t->write].state == 2) {
        alpha = 0.0f;
    }
    if (t->stride == 4 && (cam = t->cam)->mode == 98) {
        lim = t->u158;
        dv = cam->v318;
        gl_func_00000000(&dv);  /* math_vec3Normalize */
        { float s = t->u154; dv.x *= s; dv.y *= s; dv.z *= s; }
    } else {
        lim = 0.0f;
        dv.x = 0.0f; dv.y = 0.0f; dv.z = 0.0f;
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
            t->points[t->writeOfs + i].w20 = t->points[t->writeOfs + i].w1C / ((float)t->size / t->u16C);
            t->vtx[t->writeOfs + i].tc[0] = (int)(((float)t->uA8 - 1.0f) * ((float)i / (float)(t->stride - 1))) << 6;
            t->vtx[t->writeOfs + i].tc[1] = ((t->writeOfs & t->stride) >> 1) * 31U << 6;
            t->vtx[t->writeOfs + i].cn[0] = t->cam->col[0];
            t->vtx[t->writeOfs + i].cn[1] = t->cam->col[1];
            t->vtx[t->writeOfs + i].cn[2] = t->cam->col[2];
        }
    }
    t->vptr[16].fn((void *)((int)t->vptr[16].d + (int)t));
    t->prev = t->on;
    t->prevMode = t->cam->mode;
}

/* ---- snoweffects_uso_func_00000C90 (moved from agent-c's snoweffects_uso_ncc_c.cpp, agent-h run 699) ---- */
#include "../game_libs/ncc_drafts/gbi_subset.h"
struct GCtx;
#define gDPWord(pkt, a, b) { Gfx *_g = (Gfx *)(pkt); _g->words.w0 = (a); _g->words.w1 = (unsigned int)(b); }
typedef struct { Gfx *buf; int count; } DList;
/* libgdl display-list context: the Gfx head at +0xC; next() appends one packet. */
struct GCtx {
    char pad0[0xC];
    DList *dl;
    Gfx *next() { return &dl->buf[dl->count++]; }
};
/* snoweffects_uso_func_00000C90 (NON_MATCHING draft): the particle sprite draw. Calls the texture's
 * display list (+0x2C, its +0x60 through a blank import), builds the camera view matrix (camera +0x30),
 * then for each particle: world position * the gGfxLib world scale (+0x128), projected through the
 * matrix; inside the near / far planes (+0xAC / +0xB0) and the half-screen box it records the prim
 * depth (EE), the env colour (FB, alpha from +0x50), counts the sprite (+0x44) and records the stock
 * gSPScisTextureRectangle (E4 / B4 / B3; s / t steps from the texture size +0x34 / +0x38). Ends with
 * PipeSync, texture off and the render state reset. Frame 400 as the target (agent-c run 667 levers: the
 * function-scope scalars above m, the dead scratch block). Left: the target keeps i in s2 and the scaled()
 * return temp address in s3 (ours: ra / s1), and the float colouring after the projection.
 * agent-h run 694 / 699: `if (p) {}` before scratch and `if (hgt) {} if (wid) {}` after the texrect emit no
 * code; they only move as1 line numbers (objdiff 84.92 -> 85.23). */
typedef short s16;
#define G_TEXRECT 0xe4
#define G_RDPHALF_1 0xb4
#define G_RDPHALF_2 0xb3
#define gImmp1(pkt, c, p0) { Gfx *_g = (Gfx *)(pkt); _g->words.w0 = _SHIFTL((c), 24, 8); _g->words.w1 = (unsigned int)(p0); }
#define gSPScisTextureRectangle(pkt, xl, yl, xh, yh, tile, s, t, dsdx, dtdy) \
{ Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = (_SHIFTL(G_TEXRECT, 24, 8) | _SHIFTL(MAX((s16)(xh),0), 12, 12) | _SHIFTL(MAX((s16)(yh),0), 0, 12)); \
    _g->words.w1 = (_SHIFTL((tile), 24, 3) | _SHIFTL(MAX((s16)(xl),0), 12, 12) | _SHIFTL(MAX((s16)(yl),0), 0, 12)); \
    gImmp1(pkt, G_RDPHALF_1, (_SHIFTL(((s) - (((s16)(xl) < 0) ? (((s16)(dsdx) < 0) ? (MAX((((s16)(xl)*(s16)(dsdx))>>7),0)) : (MIN((((s16)(xl)*(s16)(dsdx))>>7),0))) : 0)), 16, 16) | \
                 _SHIFTL(((t) - (((yl) < 0) ? (((s16)(dtdy) < 0) ? (MAX((((s16)(yl)*(s16)(dtdy))>>7),0)) : (MIN((((s16)(yl)*(s16)(dtdy))>>7),0))) : 0)), 0, 16))); \
    gImmp1(pkt, G_RDPHALF_2, (_SHIFTL((dsdx), 16, 16) | _SHIFTL((dtdy), 0, 16))); }

struct SnowV3c { float x, y, z; };
struct SnowPartC {
    SnowV3c pos;            /* 0x00 */
    char pad0C[0x38 - 0x0C];
    float size;             /* 0x38 */
    float aspect;           /* 0x3C */
    char pad40[0x48 - 0x40];
    unsigned char col[4];   /* 0x48 */
    int pad4C;
    float alpha;            /* 0x50 */
    int pad54;
};
struct SnowView { char pad[0x14C]; float fovScale; };
struct SnowCam {
    char pad00[0x30];
    char view[0x70 - 0x30];     /* 0x30: passed to the view-matrix builder */
    SnowView *v;                /* 0x70 */
    char pad74[0xAC - 0x74];
    float nearZ, farZ;          /* 0xAC, 0xB0 */
    char padB4[0xB8 - 0xB4];
    int scrW, scrH;             /* 0xB8, 0xBC */
    int offX, offY;             /* 0xC0, 0xC4 */
    char padC8[0x158 - 0xC8];
    GCtx *dl;                   /* 0x158 */
};
struct SnowDLHolder { char pad[0x60]; void *dl; };
struct SnowSprites {
    char pad00[0x2C];
    SnowDLHolder *tex;      /* 0x2C */
    SnowPartC *items;       /* 0x30 */
    int texW, texH;         /* 0x34, 0x38 */
    int count;              /* 0x3C */
    int max;                /* 0x40 */
    int drawn;              /* 0x44 */
};
extern "C" char snow_gGfxLib;   /* gGfxLib (blank import): the camera pointer at +0x254 */
#define SNOW_CAM (*(SnowCam **)(&snow_gGfxLib + 0x254))
/* the gGfxLib world scale (+0x128..+0x130; 0x28E0 shows it on the same base as the camera pointer)
 * read through one 0-based gGfxLib alias per word, so each load keeps its own lui and its +0x128 / +0x12C /
 * +0x130 immediate as in the target (one shared base is hoisted into a register), and the RoData+0x4 depth
 * scale. scaled() reads the scale inside the inline into named x / y / z: the target holds the three
 * products in f0 / f2 / f12 before storing them (agent-h run 687, objdiff 82.9 -> 84.9). */
struct SnowGfxLibS { char pad0[0x128]; float sx, sy, sz; };
extern "C" SnowGfxLibS snow_gfx_scale_128, snow_gfx_scale_12C, snow_gfx_scale_130;
extern "C" float snow_ro_4;
inline SnowV3c scaled(const SnowV3c &a) { float x = a.x * snow_gfx_scale_128.sx; float y = a.y * snow_gfx_scale_12C.sy; float z = a.z * snow_gfx_scale_130.sz; SnowV3c r; r.x = x; r.y = y; r.z = z; return r; }

extern "C" void snoweffects_uso_func_00000C90(SnowSprites *self) {
    GCtx *c = SNOW_CAM->dl;
    int d;
    float inv;
    float k;
    float h2;
    int hgt;
    float sx;
    float sy;
    int wid;
    int x0;
    float m[4][4];
    float halfW, halfH;
    int i;
    SnowPartC *p;
    if (p) {}
    int scratch[5];

    scratch[0] = 0;
    d = gl_func_00000000(self->tex->dl);
    gDPWord(c->next(), 0x06000000, d);
    gl_func_00000000(SNOW_CAM->view, m);
    halfW = (float)(SNOW_CAM->scrW / 2);
    halfH = (float)(SNOW_CAM->scrH / 2);
    for (i = 0, p = self->items; i < self->count; i++, p++) {
        SnowV3c v;
        float w;
        v = scaled(p->pos);
        w = m[0][3] * v.x + m[1][3] * v.y + m[2][3] * v.z + m[3][3];
        if (SNOW_CAM->nearZ < w && w < SNOW_CAM->farZ) {
            inv = 1.0f / w;
            k = p->size * (85.0f / SNOW_CAM->v->fovScale);
            h2 = halfH * 2.0f;
            hgt = (int)(k * p->aspect * (h2 * inv) + 1.0f);
            sx = (m[0][0] * v.x + m[1][0] * v.y + m[2][0] * v.z + m[3][0]) * inv * halfW;
            sy = (m[0][1] * v.x + m[1][1] * v.y + m[2][1] * v.z + m[3][1]) * inv * -halfH;
            if (-halfW < sx && sx < halfW && -halfH < sy && sy < halfH) {
                wid = (int)(k * (h2 * (1.0f / w)) + 1.0f);
                x0 = (int)(sx + halfW - (float)(wid / 2) + (float)SNOW_CAM->offX);
                int y0 = (int)(sy + halfH - (float)(hgt / 2) + (float)SNOW_CAM->offY);
                gDPWord(c->next(), 0xEE000000,
                        ((int)(((m[0][2] * v.x + m[1][2] * v.y + m[2][2] * v.z + m[3][2]) * inv + 1.0f) * snow_ro_4) & 0x7FFF) << 16);
                gDPWord(c->next(), 0xFB000000,
                        (p->col[0] << 24) | (p->col[1] << 16) | (p->col[2] << 8) | ((int)p->alpha & 0xFF));
                self->drawn++;
                gSPScisTextureRectangle(c->next(), x0 << 2, y0 << 2, (x0 + wid) << 2, (y0 + hgt) << 2, 0, 0, 0,
                                        ((self->texW << 10) - 1024) / wid, ((self->texH << 10) - 1024) / hgt);
                if (hgt) {} if (wid) {}
            }
        }
    }
    gDPWord(c->next(), 0xE7000000, 0);
    gDPWord(c->next(), 0xBB000000, 0);
    gDPWord(c->next(), 0xBA001301, 0x80000);
    gDPWord(c->next(), 0xBA000C02, 0x2000);
    gDPWord(c->next(), 0xB9000201, 0);
    gDPWord(c->next(), 0xBA000602, 0);
}
