/* IDO 7.1 NCC (C++) NON_MATCHING donor for snoweffects.uso 0xC90 (moved from agent-c's
 * snoweffects_uso_ncc_c.cpp in agent-h run 699), spliced over the empty NON_MATCHING placeholder in
 * snoweffects_uso.c on the non_matching build only (the matching build keeps INCLUDE_ASM). 0x47D4 went
 * EXACT in run 709 and moved to snoweffects_uso_ncc_47d4.cpp. */
extern "C" int gl_func_00000000(...);

/* ---- snoweffects_uso_func_00000C90 (moved from agent-c's snoweffects_uso_ncc_c.cpp, agent-h run 699) ---- */
#include "../game_libs/ncc_drafts/gbi_subset.h"
struct GCtx;
#define gW(g, a, b) { (g)->words.w0 = (a); (g)->words.w1 = (unsigned int)(b); }
#define gDPWord(pkt, a, b) { Gfx *_g = (Gfx *)(pkt); _g->words.w0 = (a); _g->words.w1 = (unsigned int)(b); }
typedef struct { Gfx *buf; int count; } DList;
/* libgdl display-list context: the Gfx head at +0xC; next() appends one packet. */
struct GCtx {
    char pad0[0xC];
    DList *dl;
    Gfx *next() { return &dl->buf[dl->count++]; } Gfx *nextN() { int n = dl->count++; return &dl->buf[n]; }
    Gfx *nextI() { int n = dl->count; dl->count = n + 1; return &dl->buf[n]; }
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
 * code; they only move as1 line numbers (objdiff 84.92 -> 85.23).
 * agent-h run 717 (85.23 -> 89.59): the prim depth goes through a named `int z` (the target holds the
 * truncated depth in a uopt register, a3, before the & 0x7FFF), and scaled() returns a constructed
 * SnowV3c(x * sx, y * sy, z * sz): the ctor parameters take f0 / f2 / f12 as in the target with no
 * frame words (the named x / y / z locals cost three words between the return temp and r).
 * Then 89.59 -> 90.99, all five callee-saved homes as in the target (self s0, hgt s1, i s2, the return
 * temp address s3, wid ra): the first packet appends through nextN() (a named old count, so the prologue's
 * count is its own LR: a2 / v1 as in the target), the texrect's w0 clamps go through an imax0() inline
 * (its own temps instead of the pooled edg temps), scratch[] shrinks to one word, and the old
 * `if (hgt) {} if (wid) {}` lines are dropped.
 * agent-h run 722 (-> 94.04, body kept at the target's 592 words so 0xA8D0 stays inside TRUNCATE_TEXT):
 * no h2 variable (`halfH * 2.0f` written in both size expressions: the CSE'd product takes f20 directly,
 * k f18, sx / sy the f28 / f30 homes as in the target); the w0 clamp terms written yh-first (the target
 * then evaluates xh first); imax0 on xh / yh / xl; the env colour written alpha-first (the target loads
 * col[0] first); the texrect's third packet appends through nextN(); `if (y0) {}` before the FB packet
 * and before drawn++ (with them the xh CSE temp is computed right after x0, as in the target).
 * agent-h run 731 (-> 94.08): the display-list packet goes through a function-scope Gfx *g declared
 * after c. That is the target's 10th word above m, so m and everything below it down to scratch now sit at
 * the target's offsets (sp-offset mismatches 32 -> 10).
 * agent-h run 737 (-> 94.41): the loop body declares its words up front (y0, z, the five packet pointers
 * EE / FB / E4 / B4 / B3, and x1 = x0 + wid / y1 = y0 + hgt) and the packets go through them (gW / gTR /
 * gImmpV take the pointer, so no `_g` block words); the BB and the last BA tail packets reuse the
 * function-scope g. That puts v / __V / r at the target's sp+220 / 204 / 192 with frame 400 (the whole frame
 * map now matches); the texrect's E4 packet appends through nextN().
 * agent-h run 740 (-> 96.17): the env colour word is the stock gDPSetEnvColor body, _SHIFTL(col[0], 24, 8) |
 * .. | _SHIFTL((int)alpha, 0, 8) in r, g, b, a order: the unsigned (u32) terms keep the target's load order
 * col[0], col[1], alpha, col[2] (no plain-int order of the four terms gives it). Then (-> 96.23) xl clamps
 * through MAX like yl (yl written first), E4 / B4 append through nextI() / nextN(), and the E7 tail packet reuses g
 * (frame kept at 400). */
typedef short s16;
#define G_TEXRECT 0xe4
#define G_RDPHALF_1 0xb4
#define G_RDPHALF_2 0xb3
inline int imax0(int a) { return a > 0 ? a : 0; }

#define gImmpV(g, pkt, c, p0) { g = (pkt); (g)->words.w0 = _SHIFTL((c), 24, 8); (g)->words.w1 = (unsigned int)(p0); }
#define gTR(g0, pkt, g1, pkt2, g2, pkt3, xl, yl, xh, yh, tile, s, t, dsdx, dtdy) \
{ g0 = (pkt); \
    g0->words.w0 = (_SHIFTL(G_TEXRECT, 24, 8) | _SHIFTL(imax0((s16)(yh)), 0, 12) | _SHIFTL(imax0((s16)(xh)), 12, 12)); \
    g0->words.w1 = (_SHIFTL((tile), 24, 3) | _SHIFTL(MAX((s16)(yl),0), 0, 12) | _SHIFTL(MAX((s16)(xl),0), 12, 12)); \
    gImmpV(g1, pkt2, G_RDPHALF_1, (_SHIFTL(((s) - (((s16)(xl) < 0) ? (((s16)(dsdx) < 0) ? (MAX((((s16)(xl)*(s16)(dsdx))>>7),0)) : (MIN((((s16)(xl)*(s16)(dsdx))>>7),0))) : 0)), 16, 16) | \
                 _SHIFTL(((t) - (((yl) < 0) ? (((s16)(dtdy) < 0) ? (MAX((((s16)(yl)*(s16)(dtdy))>>7),0)) : (MIN((((s16)(yl)*(s16)(dtdy))>>7),0))) : 0)), 0, 16))); \
    gImmpV(g2, pkt3, G_RDPHALF_2, (_SHIFTL((dsdx), 16, 16) | _SHIFTL((dtdy), 0, 16))); }

struct SnowV3c { float x, y, z; SnowV3c(float ax, float ay, float az) { x = ax; y = ay; z = az; } };
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
inline SnowV3c scaled(const SnowV3c &a) { return SnowV3c(a.x * snow_gfx_scale_128.sx, a.y * snow_gfx_scale_12C.sy, a.z * snow_gfx_scale_130.sz); }

extern "C" void snoweffects_uso_func_00000C90(SnowSprites *self) {
    GCtx *c = SNOW_CAM->dl;
    Gfx *g;
    int d;
    float inv;
    float k;
    int hgt;
    float sx;
    float sy;
    int wid;
    int x0;
    float m[4][4];
    float halfW, halfH;
    int i;
    SnowPartC *p;
    int scratch[1];

    scratch[0] = 0;
    d = gl_func_00000000(self->tex->dl);
    g = c->next();
    g->words.w0 = 0x06000000;
    g->words.w1 = d;
    gl_func_00000000(SNOW_CAM->view, m);
    halfW = (float)(SNOW_CAM->scrW / 2);
    halfH = (float)(SNOW_CAM->scrH / 2);
    for (i = 0, p = self->items; i < self->count; i++, p++) {
        int y0, z; Gfx *ge, *gf, *gt, *g1, *g2; int x1, y1;
        SnowV3c v = scaled(p->pos);
        float w;
        w = m[0][3] * v.x + m[1][3] * v.y + m[2][3] * v.z + m[3][3];
        if (SNOW_CAM->nearZ < w && w < SNOW_CAM->farZ) {
            inv = 1.0f / w;
            k = p->size * (85.0f / SNOW_CAM->v->fovScale);
            hgt = (int)(k * p->aspect * (halfH * 2.0f * inv) + 1.0f);
            sx = (m[0][0] * v.x + m[1][0] * v.y + m[2][0] * v.z + m[3][0]) * inv * halfW;
            sy = (m[0][1] * v.x + m[1][1] * v.y + m[2][1] * v.z + m[3][1]) * inv * -halfH;
            if (-halfW < sx && sx < halfW && -halfH < sy && sy < halfH) {
                wid = (int)(k * (halfH * 2.0f * (1.0f / w)) + 1.0f);
                x0 = (int)(sx + halfW - (float)(wid / 2) + (float)SNOW_CAM->offX);
                y0 = (int)(sy + halfH - (float)(hgt / 2) + (float)SNOW_CAM->offY);
                z = (int)(((m[0][2] * v.x + m[1][2] * v.y + m[2][2] * v.z + m[3][2]) * inv + 1.0f) * snow_ro_4);
                ge = c->next(); gW(ge, 0xEE000000, (z & 0x7FFF) << 16);
                if (y0) {}
                gf = c->next(); gW(gf, 0xFB000000, _SHIFTL(p->col[0], 24, 8) | _SHIFTL(p->col[1], 16, 8) | _SHIFTL(p->col[2], 8, 8) |
                                                 _SHIFTL((int)p->alpha, 0, 8));
                x1 = x0 + wid;
                y1 = y0 + hgt;
                if (y0) {}
                self->drawn++;
                gTR(gt, c->nextI(), g1, c->nextN(), g2, c->nextN(), x0 << 2, y0 << 2, x1 << 2, y1 << 2, 0, 0, 0,
                    ((self->texW << 10) - 1024) / wid, ((self->texH << 10) - 1024) / hgt);
            }
        }
    }
    g = c->next(); gW(g, 0xE7000000, 0);
    g = c->next(); gW(g, 0xBB000000, 0);
    gDPWord(c->next(), 0xBA001301, 0x80000);
    gDPWord(c->next(), 0xBA000C02, 0x2000);
    gDPWord(c->next(), 0xB9000201, 0);
    g = c->next(); gW(g, 0xBA000602, 0);
}
