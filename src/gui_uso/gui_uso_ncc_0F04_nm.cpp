/* IDO 7.1 NCC (C++) NON_MATCHING comparison donor for gui_func_00000F04 (313 words; this body 315, the
 * same length as the cfe NM body). NM object only; the ROM build keeps INCLUDE_ASM.
 * agent-g run 165 (docs/IDO_CODEGEN.md kit CV): the string blitter rewritten as C++ in the 329C / 2DE0
 * style -- each append is the font context's inline Ctx::next() (n read, n + 1 stored, &buf[n]) passed as
 * the pkt of the stock 2.0I gbi.h gSPTexture / gSPScisTextureRectangle macros. Two source-order facts from
 * the target: x = x0 + cx * sx is computed BEFORE the glyph-index call (the target converts x0 into a
 * callee-saved FP register there), and the glyph's u / v / w are read into named locals before the load
 * call (u, v, w loaded first, then the call's other arguments). cy / c declared last puts their homes at
 * the target's 212 / 211. No empty tests, no pads. 97.15 standalone (cfe NM 91.58).
 * LEFT: first-append dl / n colours swapped (target dl v0, n v1), second MAX temp v0 (target a0), frame
 * 272 vs 264. */
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
struct Ctx { char pad[0xC]; DL *dl; Gfx *next() { int n = dl->n; dl->n = n + 1; return &dl->buf[n]; } };
struct Glyph { int u; int v; int w; int adv; int pad; };
struct Font { char p0[4]; int tex; char p8[8]; int h10; int h14; int tw; int th; Glyph *glyphs; Ctx *ctx; };
extern "C" int gl_func_00000000(...);
extern "C" void gui_func_00000F04(Font *f, s32 x0, s32 y0, f32 sx, f32 sy, unsigned char *str) {
    f32 cx;
    s32 h;
    unsigned char *p;
    unsigned int i;
    s32 x, y, w;
    f32 fw, fh;
    s32 idx;
    s32 u, v;
    f32 cy;
    unsigned char c;
    h = f->h14;
    gSPTexture(f->ctx->next(), 0x8000, 0x8000, 0, 0, 1);
    cx = -((f32)gl_func_00000000(f, str) / 2.0f) - 2.0f;
    cy = -((f32)f->h10 / 2.0f);
    for (p = str, i = 0; i < (unsigned int)gl_func_00000000(str); p++, i++) {
        if (*p != ' ') {
            x = (f32)x0 + cx * sx;
            fh = h * sy;
            y = (f32)y0 + cy * sy;
            idx = gl_func_00000000(*p);
            c = idx;
            u = f->glyphs[idx].u;
            v = f->glyphs[idx].v;
            w = f->glyphs[idx].w;
            gl_func_00000000(f->ctx, f->tex, f->tw, f->th, u, v, w, h, 0);
            fw = w * sx;
            gSPScisTextureRectangle(f->ctx->next(), x << 2, y << 2, (x + (s32)fw) << 2, (y + (s32)fh) << 2, 0, 0, 0,
                (s32)(((f32)(w - 1) * 1024.0f) / fw), (s32)(((f32)(h - 1) * 1024.0f) / fh));
            cx += (f32)f->glyphs[c].adv;
        } else {
            cx += (f32)f->glyphs[0].adv;
        }
    }
}
