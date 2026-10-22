/* IDO 7.1 NCC (C++) donor for gui_func_000027A0.
 *
 * Tiled sprite blitter: adjust x/y by the sprite size per the alignment flags
 * (4 = right, 1 = centre-x, 2 = centre-y), append gSPTexture through the gfx
 * context's inline next(), pick the per-format load function and TMEM budget
 * from spr->mode (edg lowers the switch to a compare chain in CASE order:
 * 1032, 272, 288, default), then either load + draw the whole sprite or, when
 * w * h exceeds the budget, cut it into column strips of gl(budget / h, spr)
 * texels. The one int scratch `t` holds the width/height reads, the context
 * pointer, the mode and the sprite passed as the strip helper's 2nd argument;
 * that last use gives it a1 (so x, the parameter, takes s8 from the entry).
 * docs/IDO_CODEGEN.md kits XXXI / XXXIII. Spliced via REPLACE_FUNC_BODY into
 * both gui_uso trees. */
#define _SHIFTL(v, s, w) ((unsigned int) (((unsigned int)(v) & ((0x01 << (w)) - 1)) << (s)))
#define G_TEXTURE 0xbb
#define BOWTIE_VAL 0
#define gSPTexture(pkt, s, t, level, tile, on) \
{ \
	Gfx *_g = (Gfx *)(pkt); \
	_g->w0 = (_SHIFTL(G_TEXTURE, 24, 8) | _SHIFTL(BOWTIE_VAL, 16, 8) | \
			_SHIFTL(level, 11, 3) | _SHIFTL(tile, 8, 3) | _SHIFTL(on, 0, 8)); \
	_g->w1 = (_SHIFTL(s, 16, 16) | _SHIFTL(t, 0, 16)); \
}
extern "C" { extern char D_00000000; extern char D_27A0_fn1032, D_27A0_fn272, D_27A0_fn288, D_27A0_fndef;
int gl_func_00000000(...); }
struct Gfx { unsigned int w0, w1; };
struct DL { Gfx *buf; int n; };
struct Ctx {
    char pad[0xC];
    DL *dl;
    Gfx *next() { int n = dl->n++; return &dl->buf[n]; }
};
#define CTX (*(Ctx **)&D_00000000)
struct Spr { char pad[8]; int tex; char pad2[0x14]; short w; short h; int mode; };
struct W { char pad[0x10]; Spr *spr; };
typedef void (*RFn)(Ctx *, int, int, int, int, int, int, int, int);
extern "C" void gui_func_000027A0(W *self, int x, int y, int flags) {
    int t;
    int w, h;
    Spr *p;
    if (flags & 4) { p = self->spr; t = p->w; x -= t; }
    else if (flags & 1) { p = self->spr; t = p->w; x -= t / 2; }
    if (flags & 2) { p = self->spr; t = p->h; y -= t / 2; }
    gSPTexture(((Ctx *)(t = (int)CTX))->next(), 0x8000, 0x8000, 0, 0, 1);
    int srcx = 0;
    int budget;
    RFn fn;
    p = self->spr;
    switch (t = self->spr->mode) {
    case 1032: budget = 4096; fn = (RFn)&D_27A0_fn1032; break;
    case 272: budget = 2048; fn = (RFn)&D_27A0_fn272; break;
    case 288: budget = 1024; fn = (RFn)&D_27A0_fn288; break;
    default: budget = 2048; fn = (RFn)&D_27A0_fndef; break;
    }
    w = self->spr->w;
    h = self->spr->h;
    t = (int)self->spr;
    if (budget < w * h) {
        int tile = gl_func_00000000(budget / h, (Spr*)t);
        int rem = w;
        int sy = (h << 10) / h;
        while (rem != 0) {
            if (tile < rem) {
                int sx = (tile << 10) / tile;
                p = self->spr; fn(CTX, p->tex, w, h, srcx, 0, tile, h, 0);
                gl_func_00000000(CTX, x, y, tile, h, sx, sy);
                x += tile;
                srcx += tile;
                rem -= tile;
            } else {
                tile = rem;
                p = self->spr; fn(CTX, p->tex, w, h, srcx, 0, rem, h, 0);
                gl_func_00000000(CTX, x, y, rem, h, (rem << 10) / rem, sy);
                rem -= tile;
            }
        }
    } else {
        fn(CTX, p->tex, w, h, 0, 0, w, h, 0);
        gl_func_00000000(CTX, x, y, w, h, (w << 10) / w, (h << 10) / h);
    }
}
