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
 * grid do not extend that web. */
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
extern "C" void gui_func_00002DE0(Obj *arg0, s32 x0, s32 y0, f32 sx, f32 sy) {
    volatile s32 q0, q1, q2, q3, q4;
    s32 t;
    f32 fw, fh;
    s32 x, y;
    Spr *spr;
    s32 w, h;
    gSPTexture(CTX->nextp(), 0x8000, 0x8000, 0, 0, 1);
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
    gSPScisTextureRectangle(CTX->next(), CTX->nC(), CTX->nD(), x << 2, y << 2, (x + (s32)fw) << 2, (y + (s32)fh) << 2, 0, 0, 0,
        (s32)(((f32)(w - 1) * 1024.0f) / fw), (s32)(((f32)(h - 1) * 1024.0f) / fh));
}
