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
/* snoweffects_uso_func_000028E0 (NON_MATCHING, objdiff 95.3 %): the snowfall draw. Moves every
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
 * nextM / next) from an agent-c run 712 climb: 88.9 -> 90.15 %. The draw block reuses the loop's cx / cy for
 * the screen x / y and w for 1 / w (no new named locals): frame 520 as the target, 90.25 % (agent-c run 721).
 * agent-c run 729 (90.25 -> 92.52 %, 792 words): two sprite sizes sw / sz from the same expression (the target's
 * `or t1,t2,zero` and two halvings); a `*(volatile float *)&halfH` read in the last y compare (halfH becomes a
 * memory home, as the target's spilled halfH, and the ilod stops copypropagate rewriting the branch uses);
 * the draw block reads z as `p->pos.z` in every term (no z variable there); `(float)snowOne() / w` (the inline
 * int folds late, so 1 / w is not part of the function-wide 1.0f constant LR).
 * Left: the 1.0f constant LR (f30 for the whole function; the target holds it in f26 in the draw block only)
 * pushes halfW to f28; the temps' frame order (the target: += param, op* param, op- return, op- r, 4 words,
 * op* return, op* r).
 * agent-c run 734: wz / wy / wx declared in that order (92.53). Putting 1 / w and both size sites in the snowOne
 * group gives the target's draw-block f26 1.0 (alpha / depth rematerialised), but the freed callee-saved register
 * then goes to another constant (-30, 60 or the alpha / depth 1.0): the target keeps wx in a callee-saved register.
 * agent-c run 741 (92.53 -> 94.9 %, 793 words): operator* computes the three products into named floats before
 * storing them (const-ref param), so they are register LRs as in the target (f2 / f12 / f14); wz / wy / wx are
 * assigned after the += and the x / y / z loads (their LRs are created after the op- / op* temps and lose the
 * priority tie, so the temps take f0 / f2 / f12 and wx lands in a callee-saved register); nested
 * `if (cy) if (cz) {}` (cz finishes after the first compare, no nop); 1.0f literal for 1 / w and the sizes with
 * snowOne on the depth (1.0 in f26 for the draw block only, halfW back in f30); no volatile halfH; a z variable
 * in the depth term only.
 * agent-c run 749 (frame 528 -> 520, the target's): halfH / i declared after rot with the loop variables in
 * function scope (halfH -188, i -208 in M space, as the target spills them); operator* returns
 * `(r.set(..), r)` (single-return body, a takes a pooled word); the 0x06, EE and E4 packets go through the
 * snowW() inline (pooled parameter words instead of the macro's named _g, -2 words).
 * agent-c run 752 (95.04 -> 95.34 %): the SnowV3d ctor body is `set(a, b, c)`, the EE packet goes through
 * snowW() and the E4 packet back through gDPWord (794 -> 793 words), and a code-free `if (1) {}` after
 * `self->drawn++` ends that uopt block: the op* / op- return-copy address LRs now take ra (sp+188, hoisted)
 * and t5 (sp+228, loop only; the post-loop copy recomputes it) as in the target, with no s4. op* r is at
 * -344 (the target's).
 * Left: wx f22 / z f24 swapped (z's pri 8.75 < wx's 10), so the draw-block z needs a copy; the EE packet order;
 * the two `+=` caller temps are swapped (target: op* result -268, op- result -280); i / last in t4 / t3
 * (target t3 / t4); the last spill at 112 (target 116:
 * the target's M is 400 with one more 8-byte unit under it).
 * agent-c run 759 (95.34 -> 95.40 %): `last` is a pointer local lp set in the for-init (was a reference
 * initialised at the top): the hoisted &lastCam LR is created after i's, wins no tie, and i / lp take t3 / t4 as in
 * the target (i spilled to its home 312, lp at 108; target 116).
 * agent-c run 763 (95.44 -> 96.11 %, 793 words, frame 520): the draw block reads x / y / z into the loop's
 * variables and uses z in all four terms (one LR, f22 as in the target). cx / cy are computed in two steps
 * (`cx = row; cy = row; cx = cx * w * halfW; cy = ...`), so uopt keeps the variables in the compares (no
 * expression temp + mov.s). Code-free zf() folds raise priorities: wy (f16, wx f24 as in the target), w, x, cx, cy.
 * self->tex goes through a local (v0); the FA red channel keeps `& 0xFF` (the target's `or t8,t7,zero`).
 * agent-c run 769 (96.11 -> 97.42 %): the `if (cy) if (cz) {}` hack is gone. Code-free folds in the loop-top
 * compares (`(cx < -30.0f) + zf8(cx, 0) + zf1(cy, 0) + zf1(cz, 0)`, `+ zf12(cz, 0)`) keep cy / cz in the cx
 * block (computed in source order cx, cy, cz) and order the colours as the target: cx f0, cz f2, cy f12. The
 * depth row is computed into cz before the EE packet (the target's schedule around the x0 / y0 conversions).
 * `tex` declared last puts m / rot at sp+404 / 340 (agent-e run 767); a zf4(cz) fold in the EE word moves 1 / w
 * to f14 as in the target.
 * EE goes through gDPWord (w0 stored before the depth, the target's order) and B4 through snowW (keeps frame 520).
 * agent-c run 778 (97.96 -> 98.45 %): agent-e's run 775 frame set (tex through lp, B4 / BB through gDPWord, the
 * `p->pos += (t = ..)` temp in a block around the loop with `assign`, op* as `r.set(..); return r;`), and the
 * draw block's screen x / y as one statement each (`cx = row * w * halfW`; the row sums are ugen temps as in the
 * target) with the cx / cy / depth rows reading `p->pos.x` (an ilod: uopt's copypropagate does not rewrite the
 * compare uses of cy, so cy stays one register computed before the first compare; the load CSEs with x's).
 * Folds zf1(halfW) + zf1(cx) in the EE word (98.84 %): halfW / cx / depth colours. The sizes read plain p->size
 * (no zf2(w) fold, 98.90 %): 1 / w f14, cy f12 as the target. Left: x f16 / -halfH f18 swapped, the int
 * t-register rotations, and the temp area / spill 4 bytes low (r 172, spill 112; target 176 / 116).
 * agent-c run 782 (98.94 -> 98.95 %): `lp = &self->lastCam` before the first SNOW_CAM2 load gives the lp item a
 * lower uopt index than the SNOW_CAM2 item, so lp takes the first spill temp (spill 116, as the target). A dead
 * `i = 0` before it keeps i's pick (t3) ahead of lp's (t4); tex now goes through d.
 * agent-c run 787: r at 176 (target). The SnowV3d ctor calls a single-return set3() (comma body returning *this):
 * set3's `this` is then a fresh word of op-'s expansion that is released when it ends, so op*'s `a` reuses it
 * and r lands directly under op*'s return copy. That frees one word at the bottom (M 396), and the BA packet
 * via gDPWord adds it back (its `_g` block local), so the frame is 520 and every sp offset is the target's.
 * Draw-block x is carried in the (dead there) wx, so x's live range no longer reaches the draw block and
 * x / the p->pos.x load can share f18 as in the target; zf1(-halfH) on the third compare gives -halfH f16,
 * and zf1(y) + zf1(z) on the first loop compare put y / z ahead of wx in the colouring (wx f24).
 * Needs `snow_ro_10 = 0x10` (RoData+0x10 depth scale); keep the body at <= 793 words, or 0xA8D0 moves past
 * TRUNCATE_TEXT and the gate fails. */
extern "C" float gl_func_00000000_snow_randf(...);   /* sys_randf (typed blank jal: float return) */
struct SnowV3d {
    float x, y, z;
    SnowV3d() {}
    void set(float a, float b, float c) { x = a; y = b; z = c; }
    SnowV3d &set3(float a, float b, float c) { return (x = a, y = b, z = c, *this); }
    SnowV3d(float a, float b, float c) { set3(a, b, c); }
    SnowV3d &operator+=(const SnowV3d &v) { x += v.x; y += v.y; z += v.z; return *this; }

    SnowV3d &assign(SnowV3d o) { x = o.x; y = o.y; z = o.z; return *this; }
};
inline SnowV3d operator-(const SnowV3d &a, const SnowV3d &b) { return SnowV3d(a.x - b.x, a.y - b.y, a.z - b.z); }
inline SnowV3d operator*(const SnowV3d &a, float s) { SnowV3d r; r.set(a.x * s, a.y * s, a.z * s); return r; }
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
inline int snowOne() { return 1; }
/* zero-code priority folds (agent-e run 756): z is always 0, uopt counts each v use and then folds the product */
static inline int zf1(float v, int z) { return z * (int)(v); }
static inline int zf2(float v, int z) { return z * (int)(v + v); }
static inline int zf4(float v, int z) { return z * (int)(v + v + v + v); }
static inline int zf8(float v, int z) { return z * (int)(v + v + v + v + v + v + v + v); }
static inline int zf12(float v, int z) { return z * (int)(v + v + v + v + v + v + v + v + v + v + v + v); }
inline void snowW(Gfx *g, unsigned int a, unsigned int b) { g->words.w0 = a; g->words.w1 = b; }

extern "C" void snoweffects_uso_func_000028E0(SnowFallD *self) {
    SnowV3d cam;
    GCtx *c;
    float halfW;
    SnowFlake *p;
    int d;
    float x;
    float y;
    float z;
    float w;
    float cx;
    float cy;
    float m[4][4];
    float rot[4][4];
    SnowV3d *lp;
    float halfH;
    float cz;
    int sz;
    int sw;
    int x0;
    int i;
    int y0;
    float wz;
    float wy;
    float wx;

    i = 0;
    lp = &self->lastCam;
    gl_func_00000000(self);
    cam.x = SNOW_CAM2->obj->pos.x * SNOW_SCALE(0);
    cam.y = SNOW_CAM2->obj->pos.y * SNOW_SCALE(1);
    cam.z = SNOW_CAM2->obj->pos.z * SNOW_SCALE(2);
    if (self->filled == 0) {
        gl_func_00000000(self);
        self->filled = 1;
    }
    c = SNOW_CAM2->dl;
    d = (int)self->tex;
    d = gl_func_00000000(((SnowDLHolder2 *)d)->dl);
    snowW(c->nextR(), 0x06000000, d);
    gl_func_00000000(SNOW_CAM2->view, m);
    gl_func_00000000(SNOW_CAM2->obj->rot, rot);
    halfW = (float)(SNOW_CAM2->scrW / 2);
    halfH = (float)(SNOW_CAM2->scrH / 2);
    gDPWord(c->nextN(), 0xFA000000,
            (((unsigned int)(self->r * 255.0f) & 0xFF) << 24) | (((unsigned int)(self->g * 255.0f) & 0xFF) << 16) |
            (((unsigned int)(self->b * 255.0f) & 0xFF) << 8) | ((unsigned int)(self->a * 255.0f) & 0xFF));
    self->visible = 0;
    {
    SnowV3d t;
    for (i = 0, p = self->items; i < self->count; i++, p++) {
        p->pos += (t = (cam - *lp) * self->follow);
        x = p->pos.x; y = p->pos.y; z = p->pos.z;
        wz = 0.0f; wy = 0.0f; wx = 0.0f;
        cx = rot[0][0] * x + rot[1][0] * y + rot[2][0] * z + rot[3][0];
        cy = rot[0][1] * x + rot[1][1] * y + rot[2][1] * z + rot[3][1];
        cz = rot[0][2] * x + rot[1][2] * y + rot[2][2] * z + rot[3][2];
        if ((cx < -30.0f) + zf8(cx, 0) + zf1(cy, 0) + zf1(cz, 0) + zf1(y, 0) + zf1(z, 0)) {
            wx = 60.0f;
        } else if (cx >= 30.0f) {
            wx = -60.0f;
        }
        if (cy < -30.0f) {
            wy = 60.0f;
        } else if (cy >= 30.0f) {
            wy = -60.0f;
        }
        if ((cz < -100.0f) + zf12(cz, 0)) {
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
            p->pos.x = p->pos.x + self->axis[1 + zf2(wy, 0)].x * wy;
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
            wx = p->pos.x; y = p->pos.y; z = p->pos.z;   /* wx is dead here: it carries the draw x */
            w = m[0][3] * wx + m[1][3] * y + m[2][3] * z + m[3][3];
            if (SNOW_CAM2->nearZ < w && w < SNOW_CAM2->farZ) {
                w = 1.0f / w;
                sw = (int)(p->size * (halfH * 2.0f * w) + 1.0f);
                cx = (m[0][0] * p->pos.x + m[1][0] * y + m[2][0] * z + m[3][0]) * w * halfW;
                cy = (m[0][1] * p->pos.x + m[1][1] * y + m[2][1] * z + m[3][1]) * w * -halfH;
                sz = (int)(p->size * (halfH * 2.0f * w) + 1.0f);
                if (-halfW < cx && cx < halfW && (-halfH < cy) + zf1(-halfH, 0) && cy < halfH) {
                    x0 = (int)(cx + halfW - (float)(sw / 2) + (float)(&SNOW_CAM2->offX)[zf2(cy, 0)]);
                    y0 = (int)(cy + halfH - (float)(sz / 2) + (float)(&SNOW_CAM2->offY)[zf4(cx, 0)]);
                    cz = m[0][2] * p->pos.x + m[1][2] * y + m[2][2] * z + m[3][2];
                    gDPWord(c->next(), 0xEE000000,
                            ((int)((cz * w + (float)snowOne()) * snow_ro_10) + zf1(halfW, 0) + zf1(cx, 0) + zf4(cz, 0) & 0x7FFF) << 16);
                    self->drawn++;
                    if (1) {}
                    gDPWord(c->nextM(), 0xE4000000 | ((((x0 + sw) << 2) & 0xFFF) << 12) | (((y0 + sz) << 2) & 0xFFF),
                            (((x0 << 2) & 0xFFF) << 12) | ((y0 << 2) & 0xFFF));
                    gDPWord(c->next(), 0xB4000000, 0);
                    gDPWord(c->next(), 0xB3000000,
                            ((((self->texW << 10) - 1024) / sw) << 16) | ((((self->texH << 10) - 1024) / sz) & 0xFFFF));
                }
            }
            self->visible++;
        }
    }
    }
    self->lastCam.assign(cam);
    gDPWord(c->next(), 0xE7000000, 0);
    gDPWord(c->next(), 0xB9000201, 0);
    gDPWord(c->next(), 0xBA001301, 0x80000);
    gDPWord(c->next(), 0xBB000000, 0);
}
