/* IDO 7.1 NCC (C++) NON_MATCHING comparison donor for gui_func_00002DE0 (303 words; this body 302).
 * NM object only; the ROM build keeps INCLUDE_ASM.
 * agent-g run 165 (docs/IDO_CODEGEN.md kit CV): rewritten from the cfe-shaped PKT macro body (89.83, 298
 * words) as real C++ -- every append is the gfx context's inline Ctx::next() (as the exact 3B80 / 27A0
 * donors), and the stock 2.0I gbi.h gSPTexture / gSPScisTextureRectangle macros get `CTX->next()` as their
 * pkt argument, so the macro's two gImmp1 re-evaluations of pkt ARE the RDPHALF_1 / RDPHALF_2 appends.
 * No pads, no empty tests. 94.54 standalone. LEFT: the target holds ctx, the switch value and the first MAX
 * temp in $s0 (one edg pool variable live across the loader calls) with frame 104; ours uses t-registers,
 * frame 64. */
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
#define gSPScisTextureRectangle(pkt, xl, yl, xh, yh, tile, s, t, dsdx, dtdy) \
{ Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = (_SHIFTL(G_TEXRECT, 24, 8) | _SHIFTL(MAX((s16)(xh),0), 12, 12) | _SHIFTL(MAX((s16)(yh),0), 0, 12)); \
    _g->words.w1 = (_SHIFTL((tile), 24, 3) | _SHIFTL(MAX((s16)(xl),0), 12, 12) | _SHIFTL(MAX((s16)(yl),0), 0, 12)); \
    gImmp1(pkt, G_RDPHALF_1, (_SHIFTL(((s) - (((s16)(xl) < 0) ? (((s16)(dsdx) < 0) ? (MAX((((s16)(xl)*(s16)(dsdx))>>7),0)) : (MIN((((s16)(xl)*(s16)(dsdx))>>7),0))) : 0)), 16, 16) | \
                 _SHIFTL(((t) - (((yl) < 0) ? (((s16)(dtdy) < 0) ? (MAX((((s16)(yl)*(s16)(dtdy))>>7),0)) : (MIN((((s16)(yl)*(s16)(dtdy))>>7),0))) : 0)), 0, 16))); \
    gImmp1(pkt, G_RDPHALF_2, (_SHIFTL((dsdx), 16, 16) | _SHIFTL((dtdy), 0, 16))); }
struct DL { Gfx *buf; int n; };
struct Ctx { char pad[0xC]; DL *dl; Gfx *next() { int n = dl->n++; return &dl->buf[n]; } };
#define CTX (*(Ctx **)&D_00000000)
struct Spr { char p8[8]; int tex; char pc[0x20 - 0xC]; s16 w; s16 h; int fmt; };
struct Obj { char p[0x10]; Spr *spr; };
extern "C" void gui_func_00002DE0(Obj *arg0, s32 x0, s32 y0, f32 sx, f32 sy) {
    Spr *spr;
    s32 w, h;
    s32 x, y;
    f32 fw, fh;
    gSPTexture(CTX->next(), 0x8000, 0x8000, 0, 0, 1);
    spr = arg0->spr;
    w = spr->w;
    h = spr->h;
    switch (spr->fmt) {
    case 0x408:
        gui_func_00000000(CTX, spr->tex, w, h, 0, 0, w, h, 0);
        break;
    case 0x120:
        gui_func_00000000(CTX, spr->tex, w, h, 0, 0, w, h, 0);
        break;
    case 0x110:
        gui_func_00000000(CTX, spr->tex, w, h, 0, 0, w, h, 0);
        break;
    }
    x = (f32)x0 + (-((f32)w / 2) * sx);
    fw = w * sx;
    y = (f32)y0 + (-((f32)h / 2) * sy);
    fh = h * sy;
    gSPScisTextureRectangle(CTX->next(), x << 2, y << 2, (x + (s32)fw) << 2, (y + (s32)fh) << 2, 0, 0, 0,
        (s32)(((f32)(w - 1) * 1024.0f) / fw), (s32)(((f32)(h - 1) * 1024.0f) / fh));
}
