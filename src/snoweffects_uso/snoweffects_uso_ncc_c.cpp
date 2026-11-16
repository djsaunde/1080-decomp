/* IDO 7.1 NCC (C++) donors for snoweffects.uso below 0x6000 (agent-c run 655): 0x270 / 0xC90 / 0x28E0.
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

/* snoweffects_uso_func_00000270 (NON_MATCHING, 245 / 260 words): a texture-sprite init (the 0x15D0
 * family with the render state first): name Data+0x2A8, a 20-packet DL context named Data+0x2BC, then
 * PipeSync, cycle 1, persp off, render mode 0x00404240, combine, prim colour 0, w = h = 32, the stock
 * 2.0I gDPLoadTextureTile of the 32x32 IA8 texture at Data+0 (mask 5 / 5), gSPTexture and EndDL.
 * The `if (1) {}` before the prim colour packet and `h = w` keep the 32 out of a hoisted constant
 * register (the target loads it inside that packet; `w = 32; h = 32` hoists it to the entry), which
 * matches the whole first half (the two size stores need separate lines, or they come out h, w).
 * Left: five packet pointers of the texture load take a3 / t0 / a1 where the target has a2 / a3
 * (live-unit layout, as agent-h's 0x15D0 family). */
extern "C" void snoweffects_uso_func_00000270(SnowTex *t) {
    t->name = D_00000000 + 0x2A8;
    t->dl = (GCtx *)gl_func_00000000(0, D_00000000 + 0x2BC, 20, 0, 0, 0, 0);
    gl_func_00000000_snow_reset(t->dl);
    gDPPipeSync(t->dl->nextN());
    gDPWord(t->dl->nextN(), 0xBA001402, 0);
    gDPWord(t->dl->nextN(), 0xBA001301, 0);
    gDPWord(t->dl->nextN(), 0xB900031D, 0x00404240);
    gDPWord(t->dl->nextN(), 0xFCFFB3FF, 0xFF64FE7F);
    if (1) {}
    gDPWord(t->dl->next(), 0xFA000000, 0);
    t->w = 32;
    t->h = t->w;
    gDPSetTextureImage(t->dl->nextN(), G_IM_FMT_IA, G_IM_SIZ_8b, t->w, D_00000000);
    gDPSetTile(t->dl->next(), G_IM_FMT_IA, G_IM_SIZ_8b, (((t->w - 1 + 1) * G_IM_SIZ_8b_TILE_BYTES) + 7) >> 3, 0, G_TX_LOADTILE, 0, 0, 5, 0, 0, 5, 0);
    gDPLoadSync(t->dl->next());
    gDPLoadTile(t->dl->nextN(), G_TX_LOADTILE, 0, 0, (t->w - 1) << G_TEXTURE_IMAGE_FRAC, (t->h - 1) << G_TEXTURE_IMAGE_FRAC);
    gDPPipeSync(t->dl->next());
    gDPSetTile(t->dl->next(), G_IM_FMT_IA, G_IM_SIZ_8b, (((t->w - 1 + 1) * G_IM_SIZ_8b_LINE_BYTES) + 7) >> 3, 0, G_TX_RENDERTILE, 0, 0, 5, 0, 0, 5, 0);
    gDPSetTileSize(t->dl->next(), G_TX_RENDERTILE, 0, 0, (t->w - 1) << G_TEXTURE_IMAGE_FRAC, (t->h - 1) << G_TEXTURE_IMAGE_FRAC);
    gDPWord(t->dl->next(), 0xBB000001, 0x80008000);
    if (1) {}
    gDPWord(t->dl->nextN(), 0xB8000000, 0);
}

/* snoweffects_uso_func_00000C90 (NON_MATCHING draft): the particle sprite draw. Calls the texture's
 * display list (+0x2C, its +0x60 through a blank import), builds the camera view matrix (camera +0x30),
 * then for each particle: world position * the RoData scale (0x128..0x130), projected through the
 * matrix; inside the near / far planes (+0xAC / +0xB0) and the half-screen box it records the prim
 * depth (EE), the env colour (FB, alpha from +0x50), counts the sprite (+0x44) and records the stock
 * gSPScisTextureRectangle (E4 / B4 / B3; s / t steps from the texture size +0x34 / +0x38). Ends with
 * PipeSync, texture off and the render state reset. Left: float register colouring and the frame
 * (384 vs 400: the target keeps more edg temporaries of the scaled-position copy chain). */
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
inline SnowV3c scaled(const SnowV3c &a, float sx, float sy, float sz) { SnowV3c r; r.x = a.x * sx; r.y = a.y * sy; r.z = a.z * sz; return r; }
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
/* RoData floats (each load takes its own lui): the world scale (0x128..0x130) and the depth scale (0x4) */
extern "C" float snow_ro_4, snow_ro_128, snow_ro_12C, snow_ro_130;

extern "C" void snoweffects_uso_func_00000C90(SnowSprites *self) {
    GCtx *c = SNOW_CAM->dl;
    int d;
    float m[4][4];
    float halfW, halfH;
    int i;
    SnowPartC *p;

    d = gl_func_00000000(self->tex->dl);
    gDPWord(c->next(), 0x06000000, d);
    gl_func_00000000(SNOW_CAM->view, m);
    halfW = (float)(SNOW_CAM->scrW / 2);
    halfH = (float)(SNOW_CAM->scrH / 2);
    for (i = 0, p = self->items; i < self->count; i++, p++) {
        SnowV3c v;
        float w;
        v = scaled(p->pos, snow_ro_128, snow_ro_12C, snow_ro_130);
        w = m[0][3] * v.x + m[1][3] * v.y + m[2][3] * v.z + m[3][3];
        if (SNOW_CAM->nearZ < w && w < SNOW_CAM->farZ) {
            float inv = 1.0f / w;
            float k = p->size * (85.0f / SNOW_CAM->v->fovScale);
            float h2 = halfH * 2.0f;
            int hgt = (int)(k * p->aspect * (h2 * inv) + 1.0f);
            float sx = (m[0][0] * v.x + m[1][0] * v.y + m[2][0] * v.z + m[3][0]) * inv * halfW;
            float sy = (m[0][1] * v.x + m[1][1] * v.y + m[2][1] * v.z + m[3][1]) * inv * -halfH;
            if (-halfW < sx && sx < halfW && -halfH < sy && sy < halfH) {
                int wid = (int)(k * (h2 * (1.0f / w)) + 1.0f);
                int x0 = (int)(sx + halfW - (float)(wid / 2) + (float)SNOW_CAM->offX);
                int y0 = (int)(sy + halfH - (float)(hgt / 2) + (float)SNOW_CAM->offY);
                gDPWord(c->next(), 0xEE000000,
                        ((int)(((m[0][2] * v.x + m[1][2] * v.y + m[2][2] * v.z + m[3][2]) * inv + 1.0f) * snow_ro_4) & 0x7FFF) << 16);
                gDPWord(c->next(), 0xFB000000,
                        (p->col[0] << 24) | (p->col[1] << 16) | (p->col[2] << 8) | ((int)p->alpha & 0xFF));
                self->drawn++;
                gSPScisTextureRectangle(c->next(), x0 << 2, y0 << 2, (x0 + wid) << 2, (y0 + hgt) << 2, 0, 0, 0,
                                        ((self->texW << 10) - 1024) / wid, ((self->texH << 10) - 1024) / hgt);
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
