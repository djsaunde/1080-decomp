/* IDO 7.1 NCC (C++) donor for gui_func_00003B80.
 *
 * Image blitter (27A0's sibling for a plain {tex, w, h, mode} image): append
 * gSPTexture through the gfx context's inline next(), pick the TMEM budget and
 * the per-format load function from img->mode (case 0/1/2, no default: the
 * budget is then read uninitialised from its home), then load + draw the whole
 * image or, when w * h exceeds the budget, column strips of gl(budget / h).
 *  - Frame: NCC lays out declared scalars in DECLARATION order down to the
 *    lowest used home, so budget (116) and fn (92) sit five scalars apart.
 *  - The dead `h = 0;` in the strip branch keeps uopt from copy-propagating
 *    hh -> h (kit XXXI #3).
 *  - Each arm puts `budget = K;` on its own source line and `fn = ...; break;`
 *    on the next: as1 schedules per source line, and only this split gives the
 *    target's `lui; addiu; li budget; b; sw fn` (one line: the li drops into
 *    the delay slot). docs/IDO_CODEGEN.md kits XXXI / XXXIII.
 * Spliced via REPLACE_FUNC_BODY into both gui_uso trees. */
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
typedef void (*RFn)(Ctx *, int, int, int, int, int, int, int, int);
extern "C" { extern char D_3B80_f0, D_3B80_f1, D_3B80_f2; }
struct Img { int tex; int w; int h; int mode; int width() { return w; } int height() { return h; } };
extern "C" void gui_func_00003B80(Img *img, int x, int y) {
    int budget;
    int srcx = 0;
    int w, h;
    int hh, tile;
    RFn fn;
    int rem, sy, sx;
    gSPTexture(CTX->next(), 0x8000, 0x8000, 0, 0, 1);
    switch (img->mode) {
    case 0: budget = 4096;
        fn = (RFn)&D_3B80_f0; break;
    case 1: budget = 2048;
        fn = (RFn)&D_3B80_f1; break;
    case 2: budget = 1024;
        fn = (RFn)&D_3B80_f2; break;
    }
    w = img->width();
    h = img->height();
    if (budget < w * h) {
        hh = h;
        tile = gl_func_00000000(budget / h);
        h = 0;
        rem = img->width();
        sy = (hh << 10) / hh;
        while (rem != 0) {
            w = img->width();
            if (tile < rem) {
                sx = (tile << 10) / tile;
                fn(CTX, img->tex, w, img->h, srcx, 0, tile, hh, 0);
                gl_func_00000000(CTX, x, y, tile, hh, sx, sy);
                srcx += tile;
                rem -= tile;
                x += tile;
            } else {
                tile = rem;
                fn(CTX, img->tex, w, img->h, srcx, 0, rem, hh, 0);
                gl_func_00000000(CTX, x, y, rem, hh, (rem << 10) / rem, sy);
                rem -= tile;
            }
        }
    } else {
        fn(CTX, img->tex, w, h, 0, 0, w, h, 0);
        w = img->width();
        h = img->height();
        gl_func_00000000(CTX, x, y, w, h, (w << 10) / w, (h << 10) / h);
    }
}
