/* IDO 7.1 NCC (C++) NON_MATCHING comparison donor for gui_func_00002DE0 (303 words; this body 299).
 * NM object only; the ROM build keeps INCLUDE_ASM.
 * agent-g run 165 (docs/IDO_CODEGEN.md kit CV): rewritten from the cfe-shaped PKT macro body (89.83, 298
 * words) as real C++ -- every append is the gfx context's inline Ctx::next() (as the exact 3B80 / 27A0
 * donors), and the stock 2.0I gbi.h gSPTexture / gSPScisTextureRectangle macros get `CTX->next()` as their
 * pkt argument, so the macro's two gImmp1 re-evaluations of pkt ARE the RDPHALF_1 / RDPHALF_2 appends.
 * No pads, no empty tests. 94.54 standalone. LEFT: the target holds ctx, the switch value and the first MAX
 * temp in $s0 (one edg pool variable live across the loader calls) with frame 104; ours uses t-registers,
 * frame 64.
 * agent-c run 190 (kit CLVIII): the rectangle's three appends as separate macro packets (pkt / pkt2 /
 * pkt3), exhaustive 9^4 append-spelling grid with the texture append: texture / rect / RDPHALF_2
 * `&dl->buf[dl->n++]`, RDPHALF_1 `n = dl->n; dl->n += 1` gives length 1212 = target (was 1208) and
 * 94.54 -> 95.43 standalone. The same grid with the CTX->load() inline (s0 as the target) peaks at 95.10
 * at 1212 (&D in t2, w / h homes swapped; a 5040 x 2 decl-order x pad grid does not move it).
 * agent-g run 220: ported to the exact-shape 329C body (inline Ctx::load() for the three loader calls, the
 * rect appends spelled next / nC / nD, texture append `n = dl->n; dl->n = n + 1`, x / y / fw / fh
 * statement order as written, five frame pads with t / fw fh / x y / spr / w h declared in that order:
 * w / h homes at the target's 0x3C / 0x38): 96.65 standalone, every block lines up with the target up to
 * registers but 1196 bytes (target 1212): the target keeps &D_00000000 live in t3 through the RDPHALF_2
 * append (rematerialised lui / addiu t3 at the yl < 0 entry and both MAX-arm exits); ours colours the
 * first two rect appends from t3 and re-forms the address (lui s0 / lw %lo) for the third. Named pointer,
 * per-append extern alias, ctx local, `if (CTX) {}`, empty-block levers and a 9^3 x 4 append-spelling
 * grid do not extend that web.
 * agent-g run 256 (kit CCLXXXIV zero-code accessor, per-site greedy): the RDPHALF_2 append's ctx through
 * `me(me(CTX))`: 96.65 -> 96.77 standalone, length unchanged (the &D web is still split at the third append). */
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
#define gSPScisTextureRectangle(pkt, pkt2, pkt3, xl, yl, xh, yh, tile, s, t, dsdx, dtdy) \
{ Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = (_SHIFTL(G_TEXRECT, 24, 8) | _SHIFTL(I4(MAX((s16)(xh),0)), 12, 12) | _SHIFTL(I2(MS((s16)(yh))), 0, 12)); \
    _g->words.w1 = (_SHIFTL((tile), 24, 3) | _SHIFTL(MAX((s16)(xl),0), 12, 12) | _SHIFTL(MS((s16)(yl)), 0, 12) + 0 * (int)_g); /* agent-c run 310: folded use of the TEXRECT packet pointer (raises its LR over the xh MAX: t1 like the target), 97.442 -> 97.492 */ \
    /* agent-c run 309 had a code-free `if ((s16)(xh)) {}` here (97.096 -> 97.360); agent-c run 333 dropped it: with the \
     * TEXRECT append as nextp() (no phantom n LR holding a0) the shorter (s16)xh LR outranks x's and takes a0 like the target */ \
    /* agent-g run 310: the s-term in exact sibling 329C's shape -- MAX and MIN arms both assign into spr, the middle `?:` \
     * reads (int)spr (every read follows its own write): with the run-310 append receivers below, 97.772 -> 97.805 */ \
    gImmp1(pkt2, G_RDPHALF_1, (_SHIFTL(((s) - (((s16)(xl) < 0) ? (((((s16)(dsdx) < 0) ? ((spr = (Spr *)(int)((MAX((((s16)(xl)*(s16)(dsdx))>>7),0)))), (int)spr) : ((spr = (Spr *)(int)((MIN((((s16)(xl)*(s16)(dsdx))>>7),0)))), (int)spr)), (int)spr)) : 0)), 16, 16) | \
                 _SHIFTL(((t) - (((yl) < 0) ? (((s16)(dtdy) < 0) ? ((xq = I(0 + (dtdy) * 0 + 0 * (int)_g)), ((((s16)(yl)*(s16)(dtdy))>>7) > 0 ? (xq = (((s16)(yl)*(s16)(dtdy))>>7)) : xq)) : (((((s16)(yl)*(s16)(dtdy))>>7) < 0) ? (((s16)(yl)*(s16)(dtdy))>>7) : I(0 + (dtdy) * 0))) : 0 * pc + 0 * (int)_g)), 0, 16))); \
    /* agent-c run 333: two folded uses of the RDPHALF_1 packet pointer _g inside the t-term arms (any two of seven \
     * s-/t-term slots, 21 pairs equal): its LR outranks the &D lda LR, so the packet takes t2 and the lda t3 like the \
     * target (with the nextp() TEXRECT append and no xh test): 98.960 -> 99.109, same length. */ \
    /* agent-c run 331: the inner MAX as `r = 0; if (v > 0) r = v` with xq as the result carrier (the as0 + as1 oracle on \
     * the target shows ugen's zero-then-copy form: `move v1,zero` before the test, one copy out): xq = v1 and the \
     * false exit's copy folds away like the target; the yl >= 0 arm keeps the folded `0 * pc` use and the dtdy argument \
     * its `0 * &D` fold: 98.399 -> 98.960, length 1212 = target. Left: pc is its own LR (a1, the `move a1,t3` copy in \
     * the yl >= 0 arm) where the target re-forms the lda LR in t3; xh's MAX t2 vs a0. */ \
    if (spr) {} \
    gImmp1(pkt3, G_RDPHALF_2, (_SHIFTL((dsdx), 16, 16) | _SHIFTL((dtdy), 0, 16))); }
template <class T> inline T *me(T *p) { return p; }
/* agent-c run 310 (kit CCCXCIV one-variable reading): the target's t0 holds spr through the loader switch AND the TEXRECT
 * yh / yl MAX results AND the RDPHALF_1 s-term MIN result -- one source variable, so those clamps are assigned INTO spr
 * (int <-> pointer casts; each read follows its own write) and spr gets a code-free test before the RDPHALF_2 append
 * (lower priority, later block), plus `if (pc) {}` after the call (random test search): 97.360 -> 97.442, same length.
 * Every yh / yl / s-term role now colours t0 like the target. */
#define MS(a) (((a) > 0 ? (spr = (Spr *)(int)(a)) : (spr = (Spr *)0)), (int)spr)
/* agent-c run 305 (inline helper on a folded copy, kit CCCLXXXV 3B6A0 form; kit CCCLXXXVI): the RDPHALF_1 t-term's MAX / MIN
 * written out with the clamp's 0 RESULT (not the compare's 0) through the zero-code I(0 + dtdy * 0): 96.848 -> 96.914,
 * same length (a whole-operand I() on the compare side gives 1212 bytes = target but slt / beqz compares, lower). */
inline int I(int v) { return v; }
/* agent-c run 309 (kit CCCIV temp rule): the TEXRECT w0 MAX results through two zero-code inlines that each take an edg pool
 * temp -- I4 (a named local) on xh, I2 (an `if (0)` assigned parameter) on yh: the w0 statement's pool entries shift, so the
 * RDPHALF_1 MAX / MIN arm temps and the RDPHALF_2 append's dl / n take the target's v0 / v1 (96.914 -> 97.096, same length) */
inline int I2(int v) { if (0) { v = 0; } return v; }
inline int I4(int v) { int r = v; return r; }
struct DL { Gfx *buf; int n; };
struct Ctx { char pad[0xC]; DL *dl; Gfx *next() { int n; n = dl->n; dl->n++; return &dl->buf[n]; } Gfx *nC() { int n; n = dl->n; dl->n++; return &dl->buf[n]; } Gfx *nD() { int n = dl->n++; return &dl->buf[n]; }
    Gfx *nextp() { int n = dl->n; dl->n = n + 1; return &dl->buf[n]; } void load(int t, s32 w, s32 h) { gui_func_00000000(this, t, w, h, 0, 0, w, h, 0); } };
#define CTX (*(Ctx **)&D_00000000)
struct Spr { char p8[8]; int tex; char pc[0x20 - 0xC]; s16 w; s16 h; int fmt; };
struct Obj { char p[0x10]; Spr *spr; };
extern "C" void gui_func_00002DE0(Obj *arg0, s32 x0, s32 y0, f32 sx, f32 sy) {
    volatile s32 q0, q1, q2, q3;
    s32 t;
    s32 xq; /* agent-c run 311: the RDPHALF_1 t-term MAX arm's zero evaluated BEFORE its test, into xq inside a folded `0 * (xq = I(..))` comparand, and xq as the else value (q4's frame word): the target's early zero + copy, blez kept: 97.492 -> 97.772, 1204 -> 1208 bytes (target 1212) */
    f32 fw, fh;
    s32 x, y;
    Spr *spr;
    s32 w, h;
    gSPTexture(CTX->nD(), 0x8000, 0x8000, 0, 0, 1); /* agent-g run 310: nD() (190-variant random grid over the 329C sibling
                                                     * levers: s-term form x loader / append receivers) */
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
    /* agent-c run 295 (kit CCCXXXVI int <-> pointer local): the RDPHALF_2 append's ctx read through an int copy of
     * &D_00000000 with a folded counted use in the dtdy argument -- the &D web now spans the RDPHALF_1 MAX / MIN arms
     * (rematerialised lui / addiu inside them like the target's t3): 96.766 -> 96.848, 1196 -> 1204 bytes (target 1212).
     * agent-c run 330: the dtdy-argument fold moved into the t-term (see the macro): pc is read only by the macro folds
     * and the RDPHALF_2 receiver now. */
    int pc = (int)&D_00000000;
    /* agent-g run 310: TEXRECT me(CTX)->next(), RDPHALF_1 nD(), RDPHALF_2 at accessor depth 1 (same grid): with the 329C
     * s-term the RDPHALF_1 s-term MIN copy takes spr's t0 and the RDPHALF_2 dl / n take v0 / v1 like the target */
    gSPScisTextureRectangle(me(CTX)->nextp(), CTX->nD(), /* agent-c run 333: TEXRECT nextp() */ me((*(Ctx **)pc))->nD(), x << 2, y << 2, (x + (s32)fw) << 2, (y + (s32)fh) << 2, 0, 0, 0,
        (s32)(((f32)(w - 1) * 1024.0f) / fw), ((s32)(((f32)(h - 1) * 1024.0f) / fh) + 0 * (int)(&D_00000000))); /* agent-c run 331: folded &D use back (98.960 needs it) */
}
