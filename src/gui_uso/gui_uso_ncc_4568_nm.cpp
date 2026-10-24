/* IDO 7.1 NCC (C++) NON_MATCHING comparison donor for gui_func_00004568 (198 words; this body 199).
 * NM object only; the ROM build keeps INCLUDE_ASM. Run 144: the function is the libultra gbi.h
 * gSPTextureRectangle (x, y >= 0) / gSPScisTextureRectangle (else) pair with tile 0, s = t = 0, written
 * with the real macro text (each gImmp1(pkt, ...) re-evaluates c->next(), one packet per word pair)
 * over the context's inline next() append: 85.8 (cfe open-coded body) -> 91.8. RESIDUAL: the first
 * packet's w0 / w1 store order and the temp ring (target stores w0 first, computes xl / yl after it). */
#include "gui_ncc_gbi.h"
typedef short s16;
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define G_TEXRECT 0xe4
#define G_RDPHALF_1 0xb4
#define G_RDPHALF_2 0xb3
#define gImmp1(pkt, c, p0) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = _SHIFTL((c), 24, 8); \
    _g->words.w1 = (unsigned int)(p0); \
}
#define gSPTextureRectangle(pkt, xl, yl, xh, yh, tile, s, t, dsdx, dtdy)\
{									\
    Gfx *_g = (Gfx *)(pkt);						\
									\
    _g->words.w0 = (_SHIFTL(G_TEXRECT, 24, 8) | _SHIFTL(xh, 12, 12) |	\
		    _SHIFTL(yh, 0, 12));    				\
    _g->words.w1 = (_SHIFTL(tile, 24, 3) | _SHIFTL(xl, 12, 12) |	\
		    _SHIFTL(yl, 0, 12));				\
    gImmp1(pkt, G_RDPHALF_1, (_SHIFTL(s, 16, 16) | _SHIFTL(t, 0, 16)));	\
    gImmp1(pkt, G_RDPHALF_2, (_SHIFTL(dsdx, 16, 16) | _SHIFTL(dtdy, 0, 16)));\
}
#define gSPScisTextureRectangle(pkt, xl, yl, xh, yh, tile, s, t, dsdx, dtdy) \
{                                                                            \
    Gfx *_g = (Gfx *)(pkt);                                                  \
                                                                             \
    _g->words.w0 = (_SHIFTL(G_TEXRECT, 24, 8) |                              \
                    _SHIFTL(MAX((s16)(xh),0), 12, 12) |                      \
                    _SHIFTL(MAX((s16)(yh),0), 0, 12));                       \
    _g->words.w1 = (_SHIFTL((tile), 24, 3) |                                 \
                    _SHIFTL(MAX((s16)(xl),0), 12, 12) |                      \
                    _SHIFTL(MAX((s16)(yl),0), 0, 12));                       \
    gImmp1(pkt, G_RDPHALF_1,                                                 \
                (_SHIFTL(((s) -                                              \
                          (((s16)(xl) < 0) ?                                 \
                           (((s16)(dsdx) < 0) ?                              \
                            (MAX((((s16)(xl)*(s16)(dsdx))>>7),0)) :          \
			    (MIN((((s16)(xl)*(s16)(dsdx))>>7),0))) : 0)),    \
			 16, 16) |                                           \
                 _SHIFTL(((t) -                                              \
                          (((yl) < 0) ?                                      \
                           (((s16)(dtdy) < 0) ?                              \
                            (MAX((((s16)(yl)*(s16)(dtdy))>>7),0)) :          \
                            (MIN((((s16)(yl)*(s16)(dtdy))>>7),0))) : 0)),    \
			 0, 16)));                                           \
    gImmp1(pkt, G_RDPHALF_2, (_SHIFTL((dsdx), 16, 16) |                      \
                              _SHIFTL((dtdy), 0, 16)));                      \
}
struct DL { Gfx *buf; int n; };
struct Ctx {
    char pad[0xC];
    DL *dl;
    Gfx *next() { int n = dl->n++; return &dl->buf[n]; }
};
extern "C" void gui_func_00004568(Ctx *c, int x, int y, int w, int h, int dsdx, int dtdy) {
    if (x >= 0 && y >= 0) {
        gSPTextureRectangle(c->next(), x << 2, y << 2, (x + w) << 2, (y + h) << 2, 0, 0, 0, dsdx, dtdy);
    } else {
        gSPScisTextureRectangle(c->next(), x << 2, y << 2, (x + w) << 2, (y + h) << 2, 0, 0, 0, dsdx, dtdy);
    }
}
