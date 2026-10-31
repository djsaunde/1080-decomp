/* IDO 7.1 NCC (C++) NON_MATCHING comparison donor for gui_func_0000329C (286 words; this body 286 = target).
 * NM object only; the ROM build keeps INCLUDE_ASM.
 * agent-g run 165 (docs/IDO_CODEGEN.md kit CV): rewritten from the cfe-shaped PKT macro body (88.70) as
 * real C++ -- every append is the gfx context's inline Ctx::next() (n read, n + 1 stored, &buf[n]), and the
 * stock 2.0I gbi.h gSPTexture / gSPScisTextureRectangle macros get `CTX->next()` as their pkt argument, so
 * the macro's two gImmp1 re-evaluations of pkt ARE the RDPHALF_1 / RDPHALF_2 appends. No pads, no empty
 * tests. 97.33 standalone. Then: the three loader calls as an inline Ctx::load() (its `this` puts the pool
 * variable -- next()'s this, the switch value, the MAX temps -- into the call blocks, so uopt gives it $s0 as
 * in the target), x / y / fw / fh declared before w / h (w / h homes = target) and a named `t = spr->fmt`
 * (the target's sixth home before w; also takes t1 so spr lands in t2): 98.62, length 1144 = target.
 * run 166 (kit CVII): the switch operand re-read through arg0 (`arg0->spr->fmt`, CSE'd with spr) keeps t
 * (t1, now a phantom) out of the compares, which read the pool variable s0 as in the target: 98.69.
 * LEFT: the second MAX pool temp (zdbug:5 bit 102, home M-56) is v1 (target t2, so its zero init
 * schedules at the join); it is coloured with only v0 / s0 forbidden.
 * run 172 (kit CXVII): the gSPTexture append through a post-increment next (`n = dl->n++`) while the
 * texture rectangle keeps the explicit one: 49 -> 43 words (the 1A44 increment-spelling lever).
 * run 189 (kit CLVII): the rectangle's three appends spelled separately (pkt / pkt2 / pkt3 in the
 * macro): rect + RDPHALF_1 as `n = dl->n; dl->n++`, RDPHALF_2 as `n = dl->n++`, texture as `n + 1`.
 * The second MAX temp's zero now schedules at the join as in the target (a0, target t2). Same as 3714. */
typedef float f32; typedef int s32; typedef unsigned int u32; typedef short s16;
extern "C" { extern char D_00000000; extern int gui_func_00000000(...); }
#define _SHIFTL(v, s, w) ((unsigned int) (((unsigned int)(v) & ((0x01 << (w)) - 1)) << (s)))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
template <class T> inline T *me(T *p) { return p; } /* agent-c run 252: zero-code accessor (kit CCLXXXIV) */
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
#define gSPScisTextureRectangle(pkt, pkt2, pkt3, xl, yl, xh, yh, tile, s, t, dsdx, dtdy) \
{ Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = (_SHIFTL(G_TEXRECT, 24, 8) | _SHIFTL(MAX((s16)(xh),0), 12, 12) | _SHIFTL(MAX((s16)(yh),0), 0, 12)); \
    _g->words.w1 = (_SHIFTL((tile), 24, 3) | _SHIFTL(MAX((s16)(xl),0), 12, 12) | _SHIFTL(MAX((s16)(yl),0), 0, 12)); \
    gImmp1(pkt2, G_RDPHALF_1, (_SHIFTL(((s) - (((s16)(xl) < 0) ? (((s16)(dsdx) < 0) ? (MAX((((s16)(xl)*(s16)(dsdx))>>7),0)) : (MIN((((s16)(xl)*(s16)(dsdx))>>7),0))) : 0)), 16, 16) | \
                 _SHIFTL(((t) - (((yl) < 0) ? (((s16)(dtdy) < 0) ? (MAX((((s16)(yl)*(s16)(dtdy))>>7),0)) : (MIN((((s16)(yl)*(s16)(dtdy))>>7),0))) : 0)), 0, 16))); \
    gImmp1(pkt3, G_RDPHALF_2, (_SHIFTL((dsdx), 16, 16) | _SHIFTL((dtdy), 0, 16))); }
struct DL { Gfx *buf; int n; };
struct Ctx { char pad[0xC]; DL *dl; Gfx *next() { int n; n = dl->n; dl->n++; return &dl->buf[n]; } Gfx *nC() { int n; n = dl->n; dl->n++; return &dl->buf[n]; } Gfx *nD() { int n = dl->n++; return &dl->buf[n]; }
    Gfx *nextp() { int n = dl->n; dl->n = n + 1; return &dl->buf[n]; } void load(int t, s32 w, s32 h) { gui_func_00000000(this, t, w, h, 0, 0, w, h, 0); } };
#define CTX (*(Ctx **)&D_00000000)
struct Spr { char p8[8]; int tex; char pc[0x20 - 0xC]; s16 w; s16 h; int fmt; };
struct Obj { char p[0x10]; Spr *spr; };
extern "C" void gui_func_0000329C(Obj *arg0, s32 x0, s32 y0, s32 x1, s32 y1) {
    Spr *spr;
    s32 x, y;
    f32 fw, fh;
    s32 t;
    s32 w, h;
    gSPTexture(me(me(CTX))->nD(), 0x8000, 0x8000, 0, 0, 1); /* agent-c run 252: per-pkt accessor greedy (kit CCLXXXIV): 99.06 -> 99.37 */
    spr = arg0->spr;
    w = spr->w;
    h = spr->h;
    t = spr->fmt;
    switch (arg0->spr->fmt) {
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
    x = (f32)x0;
    fw = x1 - x0;
    y = (f32)y0;
    fh = y1 - y0;
    gSPScisTextureRectangle(CTX->next(), CTX->nC(), me(CTX)->nD() + 0, x << 2, y << 2, (x + (s32)fw) << 2, (y + (s32)fh) << 2, 0, 0, 0,
        (s32)(((f32)(w - 1) * 1024.0f) / fw), (s32)(((f32)(h - 1) * 1024.0f) / fh));
}
