/* IDO 7.1 NCC (C++) donors for snoweffects.uso below 0x6000 (agent-c run 655): 0x270 / 0x28E0
 * (0xC90 moved to agent-h's snoweffects_uso_ncc_nm_h.cpp, agent-h run 699).
 * Kept apart from agent-h's snoweffects_uso_ncc*.cpp and agent-e's _e donor (own Makefile block at the
 * end of the Makefile). NCC donors build without -I include. */
#include "../game_libs/ncc_drafts/gbi_subset.h"
extern "C" int gl_func_00000000(...);
extern "C" char D_00000000[];
struct GCtx;
/* gfx_resetDLContext, void-typed blank jal (frees v0 for the context temp; agent-h run 649) */
extern "C" void gl_func_00000000_snow_reset(GCtx *);

#define gDPWord(pkt, a, b) { Gfx *_g = (Gfx *)(pkt); _g->words.w0 = (a); _g->words.w1 = (unsigned int)(b); }
typedef struct { Gfx *buf; int count; } DList;
/* libgdl display-list context: the Gfx head at +0xC. next() appends one packet; nextN() is the same
 * append through named locals (same code, a different uopt live-unit layout). */
struct GCtx {
    char pad0[0xC];
    DList *dl;
    Gfx *next() { return &dl->buf[dl->count++]; }
    Gfx *nextN() { int n = dl->count++; Gfx *g = &dl->buf[n]; return g; }
    Gfx *nextM() { int n = dl->count++; return &dl->buf[n]; }
    /* named-result appends for 0x270 (agent-c run 683). nextR's g, and the `if (1) {}` cut inside nextB /
     * nextBI, set where uopt's live units end: the target's packet-pointer rotation needs a unit cut inside
     * the LoadTile append (nextBI: the index n crosses the cut as a hidden t0 range, which also puts the
     * E7 / F568 constants in t1 / t2). */
    Gfx *nextR() { Gfx *g = &dl->buf[dl->count++]; return g; }
    Gfx *nextB() { Gfx *g = &dl->buf[dl->count++]; if (1) {} return g; }
    Gfx *nextBI() { int n = dl->count++; if (1) {} return &dl->buf[n]; }
};
/* a texture sprite (name +0xC, 20-packet DL context +0x2C, texture size +0x34 / +0x38) */
struct SnowTex {
    char pad00[0xC];
    char *name;
    char pad10[0x2C - 0x10];
    GCtx *dl;
    int pad30;
    int w;
    int h;
};

/* snoweffects_uso_func_00000270 (EXACT, agent-c run 683): a texture-sprite init (the 0x15D0 family with the
 * render state first): name Data+0x2A8, a 20-packet DL context named Data+0x2BC, then PipeSync, cycle 1,
 * persp off, render mode 0x00404240, combine, prim colour 0, w = h = 32, the 2.0I gDPLoadTextureTile of the
 * 32x32 IA8 texture at Data+0 (mask 5 / 5) as its seven packets, gSPTexture and EndDL.
 * Register match: the packet pointers rotate a1 / a2 / a3 by uopt live unit (about 20 memory ops; interference
 * is per unit). The per-packet append spelling (next / nextR / nextB / nextBI) and the zero-code counted loads
 * (SNOW_PH: a load of a never-read word, masked away) place the unit cuts where the target has them; found by a
 * layout-scored hill climb (packet-register sequence + the E7 / F568 / 32 constant registers), then minimised. */
#define SNOW_PH(p, off) (*(int *)((char *)(p) + (off)) & 0)
extern "C" void snoweffects_uso_func_00000270(SnowTex *t) {
    t->name = D_00000000 + 0x2A8;
    t->dl = (GCtx *)gl_func_00000000(0, D_00000000 + 0x2BC, 20, 0, 0, 0, 0);
    gl_func_00000000_snow_reset(t->dl);
    gDPWord(t->dl->nextR(), 0xE7000000, 0);
    gDPWord(t->dl->nextR(), 0xBA001402, 0);
    gDPWord(t->dl->nextR(), 0xBA001301, 0);
    gDPWord(t->dl->nextR(), 0xB900031D, 0x00404240);
    gDPWord(t->dl->next(), 0xFCFFB3FF, 0xFF64FE7F);
    gDPWord(t->dl->nextR(), 0xFA000000, 0 + SNOW_PH(t, 0x290));
    t->w = 32;
    t->h = t->w;
    gDPWord(t->dl->nextR(), 0xFD680000 | (((t->w - 1) & 0xFFF) + SNOW_PH(t, 0x308) + SNOW_PH(t, 0x30C) +
                                          SNOW_PH(t, 0x310) + SNOW_PH(t, 0x314)), (int)D_00000000);
    gDPWord(t->dl->next(), 0xF5680000 | (((((t->w + 7) >> 3) & 0x1FF) << 9) + SNOW_PH(t, 0x358) + SNOW_PH(t, 0x35C) +
                                         SNOW_PH(t, 0x360) + SNOW_PH(t, 0x364)), 0x07014050);
    gDPWord(t->dl->nextB(), 0xE6000000, 0);
    gDPWord(t->dl->nextBI(), 0xF4000000, 0x07000000 | ((((t->w - 1) << 2) & 0xFFF) << 12) | (((t->h - 1) << 2) & 0xFFF));
    gDPWord(t->dl->nextR(), 0xE7000000, 0);
    gDPWord(t->dl->nextB(), 0xF5680000 | ((((t->w + 7) >> 3) & 0x1FF) << 9), 0x00014050);
    gDPWord(t->dl->next(), 0xF2000000, ((((t->w - 1) << 2) & 0xFFF) << 12) | (((t->h - 1) << 2) & 0xFFF));
    gDPWord(t->dl->next(), 0xBB000001, 0x80008000);
    gDPWord(t->dl->next(), 0xB8000000, 0);
}

extern "C" char snow_gGfxLib;   /* gGfxLib (blank import): the camera pointer at +0x254 */
/* snoweffects_uso_func_000028E0 (NON_MATCHING, objdiff 90.15 %): the snowfall draw. Moves every
 * flake by the camera motion since the last frame (camera +0x70 position * the gGfxLib world scale, minus +0x98,
 * times +0xA4), wraps it inside the camera-space box (x / y +-30, z -100..0) along the box axes
 * (+0x50 / +0x5C / +0x68; a y wrap re-rolls the flake's alpha against +0x4C), then projects the
 * visible flakes (alpha > 0, counted in +0xBC) as gDPSetPrimDepth + texture rectangles of size
 * +0x38 (the 0xC90 projection). Prim colour from +0xC0..+0xCC, first-call fill (+0xB4).
 * Levers: function-scope scalars above the two matrices (m at vfp-116, rot at vfp-180); the motion as ONE
 * expression `p->pos += (cam - last) * follow` (the target's two three-word copy chains), with `last` a reference
 * local (the target hoists &self->lastCam into t4 and spills it at the frame bottom) and operator- returning a
 * constructed SnowV3d (its store / copy schedule); `if (cy) {} if (cz) {}` keeps the camera-space
 * coordinates computed before the first wrap test (uopt otherwise sinks cy / cz into the branch blocks; two
 * ifs instead of three keep c's block count low enough for s2); per-site packet appends (nextR / nextN /
 * nextM / next) from an agent-c run 712 climb: 88.9 -> 90.15 %.
 * Left: halfW loses its f30 home (split / spilled), the frame size (536 vs 520), the temps'
 * frame order (the target: += param, op* param, op- return, op- r, 4 words, op* return, op* r) and float colouring.
 * Needs `snow_ro_10 = 0x10` (RoData+0x10 depth scale); keep the body at <= 793 words, or 0xA8D0 moves past
 * TRUNCATE_TEXT and the gate fails. */
extern "C" float gl_func_00000000_snow_randf(...);   /* sys_randf (typed blank jal: float return) */
struct SnowV3d {
    float x, y, z;
    SnowV3d() {}
    SnowV3d(float a, float b, float c) { x = a; y = b; z = c; }
    SnowV3d &operator+=(const SnowV3d &v) { x += v.x; y += v.y; z += v.z; return *this; }

    SnowV3d &operator=(SnowV3d o) { x = o.x; y = o.y; z = o.z; return *this; }
};
inline SnowV3d operator-(const SnowV3d &a, const SnowV3d &b) { return SnowV3d(a.x - b.x, a.y - b.y, a.z - b.z); }
inline SnowV3d operator*(SnowV3d a, float s) { SnowV3d r; r.x = a.x * s; r.y = a.y * s; r.z = a.z * s; return r; }
struct SnowFlake {
    SnowV3d pos;            /* 0x00 */
    char pad0C[0x38 - 0x0C];
    float size;             /* 0x38 */
    char pad3C[0x50 - 0x3C];
    float alpha;            /* 0x50 */
    int pad54;
};
struct SnowCamObj { char pad[0xA0]; SnowV3d pos; char padAC[0xB4 - 0xAC]; char rot[1]; };
struct SnowCam2 {
    char pad00[0x30];
    char view[0x70 - 0x30];     /* 0x30 */
    SnowCamObj *obj;            /* 0x70 */
    char pad74[0xAC - 0x74];
    float nearZ, farZ;          /* 0xAC, 0xB0 */
    char padB4[0xB8 - 0xB4];
    int scrW, scrH;             /* 0xB8, 0xBC */
    int offX, offY;             /* 0xC0, 0xC4 */
    char padC8[0x158 - 0xC8];
    GCtx *dl;                   /* 0x158 */
};
struct SnowDLHolder2 { char pad[0x60]; void *dl; };
struct SnowFallD {
    char pad00[0x2C];
    SnowDLHolder2 *tex;     /* 0x2C */
    SnowFlake *items;       /* 0x30 */
    int texW, texH;         /* 0x34, 0x38 */
    char pad3C[0x44 - 0x3C];
    int drawn;              /* 0x44 */
    int fadeIn;             /* 0x48 */
    float fadeChance;       /* 0x4C */
    SnowV3d axis[3];        /* 0x50: the box axes (x / y / z wrap) */
    char pad74[0x98 - 0x74];
    SnowV3d lastCam;        /* 0x98 */
    float follow;           /* 0xA4 */
    char padA8[0xB4 - 0xA8];
    int filled;             /* 0xB4 */
    int count;              /* 0xB8 */
    int visible;            /* 0xBC */
    float r, g, b, a;       /* 0xC0 */
};
#define SNOW_CAM2 (*(SnowCam2 **)(&snow_gGfxLib + 0x254))
#define SNOW_SCALE(i) (((float *)(&snow_gGfxLib + 0x128))[i])
extern "C" float snow_ro_10;

extern "C" void snoweffects_uso_func_000028E0(SnowFallD *self) {
    SnowV3d cam;
    GCtx *c;
    float halfW, halfH;
    int i;
    SnowFlake *p;
    int d;
    float x, y, z;
    float w;
    float m[4][4];
    float rot[4][4];
    SnowV3d &last = self->lastCam;

    gl_func_00000000(self);
    cam.x = SNOW_CAM2->obj->pos.x * SNOW_SCALE(0);
    cam.y = SNOW_CAM2->obj->pos.y * SNOW_SCALE(1);
    cam.z = SNOW_CAM2->obj->pos.z * SNOW_SCALE(2);
    if (self->filled == 0) {
        gl_func_00000000(self);
        self->filled = 1;
    }
    c = SNOW_CAM2->dl;
    d = gl_func_00000000(self->tex->dl);
    gDPWord(c->nextR(), 0x06000000, d);
    gl_func_00000000(SNOW_CAM2->view, m);
    gl_func_00000000(SNOW_CAM2->obj->rot, rot);
    halfW = (float)(SNOW_CAM2->scrW / 2);
    halfH = (float)(SNOW_CAM2->scrH / 2);
    gDPWord(c->nextN(), 0xFA000000,
            ((unsigned int)(self->r * 255.0f) << 24) | (((unsigned int)(self->g * 255.0f) & 0xFF) << 16) |
            (((unsigned int)(self->b * 255.0f) & 0xFF) << 8) | ((unsigned int)(self->a * 255.0f) & 0xFF));
    self->visible = 0;
    for (i = 0, p = self->items; i < self->count; i++, p++) {
        float cx, cy, cz;
        float inv; int sz; float sx, sy; int x0, y0;
        float wx = 0.0f, wy = 0.0f, wz = 0.0f;
        p->pos += (cam - last) * self->follow;
        x = p->pos.x; y = p->pos.y; z = p->pos.z;
        cx = rot[0][0] * x + rot[1][0] * y + rot[2][0] * z + rot[3][0];
        cy = rot[0][1] * x + rot[1][1] * y + rot[2][1] * z + rot[3][1];
        cz = rot[0][2] * x + rot[1][2] * y + rot[2][2] * z + rot[3][2];
        if (cy) {} if (cz) {}
        if (cx < -30.0f) {
            wx = 60.0f;
        } else if (cx >= 30.0f) {
            wx = -60.0f;
        }
        if (cy < -30.0f) {
            wy = 60.0f;
        } else if (cy >= 30.0f) {
            wy = -60.0f;
        }
        if (cz < -100.0f) {
            wz = 100.0f;
        } else if (cz >= 0.0f) {
            wz = -100.0f;
        }
        if (wx != 0.0f) {
            p->pos.x = x + self->axis[0].x * wx;
            p->pos.y = y + self->axis[0].y * wx;
            p->pos.z = z + self->axis[0].z * wx;
        }
        if (wy != 0.0f) {
            p->pos.x = p->pos.x + self->axis[1].x * wy;
            p->pos.y = p->pos.y + self->axis[1].y * wy;
            p->pos.z = p->pos.z + self->axis[1].z * wy;
            if (self->fadeChance < gl_func_00000000_snow_randf()) {
                if (self->fadeIn != 0) {
                    p->alpha = 1.0f;
                } else {
                    p->alpha = 0.0f;
                }
            }
        }
        if (wz != 0.0f) {
            p->pos.x = p->pos.x + self->axis[2].x * wz;
            p->pos.y = p->pos.y + self->axis[2].y * wz;
            p->pos.z = p->pos.z + self->axis[2].z * wz;
        }
        if (p->alpha > 0.0f) {
            x = p->pos.x; y = p->pos.y; z = p->pos.z;
            w = m[0][3] * x + m[1][3] * y + m[2][3] * z + m[3][3];
            if (SNOW_CAM2->nearZ < w && w < SNOW_CAM2->farZ) {
                inv = 1.0f / w;
                sz = (int)(p->size * (halfH * 2.0f * inv) + 1.0f);
                sx = (m[0][0] * x + m[1][0] * y + m[2][0] * z + m[3][0]) * inv * halfW;
                sy = (m[0][1] * x + m[1][1] * y + m[2][1] * z + m[3][1]) * inv * -halfH;
                if (-halfW < sx && sx < halfW && -halfH < sy && sy < halfH) {
                    x0 = (int)(sx + halfW - (float)(sz / 2) + (float)SNOW_CAM2->offX);
                    y0 = (int)(sy + halfH - (float)(sz / 2) + (float)SNOW_CAM2->offY);
                    gDPWord(c->next(), 0xEE000000,
                            ((int)(((m[0][2] * x + m[1][2] * y + m[2][2] * z + m[3][2]) * inv + 1.0f) * snow_ro_10) & 0x7FFF) << 16);
                    self->drawn++;
                    gDPWord(c->nextM(), 0xE4000000 | ((((x0 + sz) << 2) & 0xFFF) << 12) | (((y0 + sz) << 2) & 0xFFF),
                            (((x0 << 2) & 0xFFF) << 12) | ((y0 << 2) & 0xFFF));
                    gDPWord(c->next(), 0xB4000000, 0);
                    gDPWord(c->next(), 0xB3000000,
                            ((((self->texW << 10) - 1024) / sz) << 16) | ((((self->texH << 10) - 1024) / sz) & 0xFFFF));
                }
            }
            self->visible++;
        }
    }
    self->lastCam = cam;
    gDPWord(c->next(), 0xE7000000, 0);
    gDPWord(c->next(), 0xB9000201, 0);
    gDPWord(c->next(), 0xBA001301, 0x80000);
    gDPWord(c->next(), 0xBB000000, 0);
}
