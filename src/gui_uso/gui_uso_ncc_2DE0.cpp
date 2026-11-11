/* IDO 7.1 NCC (C++) EXACT donor for gui_func_00002DE0 (303 words = target), spliced over an empty placeholder in
 * both trees (REPLACE_FUNC_BODY). f32-scale sibling of the exact gui_func_0000329C donor: the stock 2.0I gbi.h
 * gSPTexture / gSPScisTextureRectangle macros with inline Ctx appends as their pkt arguments, the three loader
 * calls through an inline Ctx::load() (the pool variable in $s0 as the target).
 * History (docs/IDO_CODEGEN.md): agent-g run 165 (kit CV) real C++ rewrite; agent-c runs 190-333 / agent-g runs
 * 220-310 append spellings, frame pads, spr as one variable for the clamp results (kit CCCXCIV), zero-code I / I2 /
 * I4 pool entries, me() accessor depths, nextp() / folded packet uses (kits CCCLXXXV-CDXXV): 99.109 / 1212.
 * agent-c run 337 (kit CDXXVIII): the last residual was the &D_00000000 lda LR -- the target's t3 is ONE uopt LR
 * cost-split around the yl < 0 arm (reloaded at its entry and both MAX exits, nothing in the yl >= 0 arm). That
 * split happens when the lda is coloured with save = cost (no folded &D / pc uses, no xq-zero or `_g` folds in the
 * t-term, no `if (spr) {}` before the RDPHALF_2 append) and its split piece's neighbour budget reaches the yl >= 0
 * arm; with the s-term MAX through spr (329C's shape) the budget was one neighbour short, and the TEXRECT append
 * as me(CTX)->nD() instead of nextp() (no `n + 1` LR) frees it: 97.013 -> EXACT. */
typedef float f32; typedef int s32; typedef unsigned int u32; typedef short s16;
extern "C" { extern char D_00000000; extern int gui_func_00000000(...); }
#define _SHIFTL(v, s, w) ((unsigned int) (((unsigned int)(v) & ((0x01 << (w)) - 1)) << (s)))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
struct Gwords { unsigned int w0, w1; };
union Gfx { Gwords words; long long force_structure_alignment; };
#define G_TEXTURE 0xbb
#define G_TEXRECT 0xe4
#define G_RDPHALF_1 0xb4
#define G_RDPHALF_2 0xb3
#define gImmp1(pkt, c, p0) { Gfx *_g = (Gfx *)(pkt); _g->words.w0 = _SHIFTL((c), 24, 8); _g->words.w1 = (unsigned int)(p0); }
#define gSPTexture(pkt, s, t, level, tile, on) \
{ Gfx *_g = (Gfx *)(pkt); \
	_g->words.w0 = (_SHIFTL(G_TEXTURE, 24, 8) | _SHIFTL(0, 16, 8) | _SHIFTL(level, 11, 3) | _SHIFTL(tile, 8, 3) | _SHIFTL(on, 0, 8)); \
	_g->words.w1 = (_SHIFTL(s, 16, 16) | _SHIFTL(t, 0, 16)); }
/* The clamp results are assigned INTO spr (int <-> pointer casts; each read follows its own write): the target's t0
 * holds spr through the loader switch and the TEXRECT yh / yl and RDPHALF_1 s-term clamp results (kit CCCXCIV).
 * The folded `0 * (int)_g` in the TEXRECT w1 is a code-free use of the defined packet pointer (agent-c run 310). */
#define gSPScisTextureRectangle(pkt, pkt2, pkt3, xl, yl, xh, yh, tile, s, t, dsdx, dtdy) \
{ Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = (_SHIFTL(G_TEXRECT, 24, 8) | _SHIFTL(I4(MAX((s16)(xh),0)), 12, 12) | _SHIFTL(I2(MS((s16)(yh))), 0, 12)); \
    _g->words.w1 = (_SHIFTL((tile), 24, 3) | _SHIFTL(MAX((s16)(xl),0), 12, 12) | _SHIFTL(MS((s16)(yl)), 0, 12) + 0 * (int)_g); \
    gImmp1(pkt2, G_RDPHALF_1, (_SHIFTL(((s) - (((s16)(xl) < 0) ? (((((s16)(dsdx) < 0) ? ((spr = (Spr *)(int)((MAX((((s16)(xl)*(s16)(dsdx))>>7),0)))), (int)spr) : ((spr = (Spr *)(int)((MIN((((s16)(xl)*(s16)(dsdx))>>7),0)))), (int)spr)), (int)spr)) : 0)), 16, 16) | \
                 _SHIFTL(((t) - (((yl) < 0) ? (((s16)(dtdy) < 0) ? ((((s16)(yl)*(s16)(dtdy))>>7) > 0 ? (((s16)(yl)*(s16)(dtdy))>>7) : (xq = I2(0 + (dtdy) * 0))) : (((((s16)(yl)*(s16)(dtdy))>>7) < 0) ? (((s16)(yl)*(s16)(dtdy))>>7) : I(0 + (dtdy) * 0))) : 0)), 0, 16))); \
    gImmp1(pkt3, G_RDPHALF_2, (_SHIFTL((dsdx), 16, 16) | _SHIFTL((dtdy), 0, 16))); }
/* zero-code accessor (kit CCLXXXIV) */
template <class T> inline T *me(T *p) { return p; }
#define MS(a) (((a) > 0 ? (spr = (Spr *)(int)(a)) : (spr = (Spr *)0)), (int)spr)
/* zero-code inline helpers: each takes an edg pool temp (kits CCCIV, CCCLXXXV) */
inline int I(int v) { return v; }
inline int I2(int v) { if (0) { v = 0; } return v; }
inline int I4(int v) { int r = v; return r; }
struct DL { Gfx *buf; int n; };
struct Ctx { char pad[0xC]; DL *dl; Gfx *nD() { int n = dl->n++; return &dl->buf[n]; }
    void load(int t, s32 w, s32 h) { gui_func_00000000(this, t, w, h, 0, 0, w, h, 0); } };
#define CTX (*(Ctx **)&D_00000000)
struct Spr { char p8[8]; int tex; char pc[0x20 - 0xC]; s16 w; s16 h; int fmt; };
struct Obj { char p[0x10]; Spr *spr; };
extern "C" void gui_func_00002DE0(Obj *arg0, s32 x0, s32 y0, f32 sx, f32 sy) {
    volatile s32 q0, q1, q2, q3; /* frame pads: w / h homes at the target's 0x3C / 0x38 */
    s32 t;
    s32 zq; s32 xq;
    f32 fw, fh;
    s32 x, y;
    Spr *spr;
    s32 w, h;
    gSPTexture(CTX->nD(), 0x8000, 0x8000, 0, 0, 1);
    spr = arg0->spr;
    w = spr->w;
    h = spr->h;
    switch (spr->fmt) {
    case 0x408:
        CTX->load(spr->tex, w, h);
        break;
    case 0x120:
        CTX->load(spr->tex, w, h);
        break;
    case 0x110:
        CTX->load(spr->tex, w, h);
        break;
    }
    x = (f32)x0 + (-((f32)w / 2) * sx);
    fw = w * sx;
    y = (f32)y0 + (-((f32)h / 2) * sy);
    fh = h * sy;
    int pc = (int)&D_00000000; /* the RDPHALF_2 append's ctx through an int copy of &D_00000000 (kit CCCXXXVI) */
    gSPScisTextureRectangle(me(CTX)->nD(), CTX->nD(), me((*(Ctx **)pc))->nD(), x << 2, y << 2, (x + (s32)fw) << 2, (y + (s32)fh) << 2, 0, 0, 0,
        (s32)(((f32)(w - 1) * 1024.0f) / fw), ((s32)(((f32)(h - 1) * 1024.0f) / fh)));
}
