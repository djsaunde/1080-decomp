/* IDO 7.1 NCC (C++) donor for gui_uso_func_00002334 (0x280 / 160 words).
 *
 * Three appends into the global gfx context: G_SETCOMBINE (0xFC11EA23 / 0xFFD7FFFF),
 * G_SETENVCOLOR with the a1 alpha byte, then G_SETPRIMCOLOR from col[0..2]
 * * 255 above the a3 alpha byte (the 1EF4 recipe, docs/IDO_CODEGEN.md kit
 * XXV). gui_uso_func_000017DC is the same function with another combiner.
 *
 * uopt ends a straight-line basic block after 20 variable loads
 * (varreflimit, a statement boundary). The target needs that boundary to
 * come AFTER the third slot pointer is defined, so that p1, p2, p3 and the
 * pooled dl temporary share one block and p3 colours t2. With next() written
 * `int n = dl->n++; return &dl->buf[n];` and stores through p->w0 / p->w1,
 * the boundary fell three counted loads too early: p3 took v1 and every later
 * t-register moved by one. Here next() is `return &dl->buf[dl->n++];` (no
 * copy of n), and the first two packets go through an inline Gfx::set().
 * Reads of set()'s `this` forward to p1 / p2 and do not count toward the
 * limit. Same code, three fewer counted loads. See kit XXXV.
 * Spliced via REPLACE_FUNC_BODY into both gui_uso trees. */
extern "C" { extern char D_00000000; }
struct Gfx {
    unsigned int w0, w1;
    void set(unsigned a, unsigned b) { w0 = a; w1 = b; }
};
struct DL { Gfx *buf; int n; };
struct Ctx {
    char pad[0xC];
    DL *dl;
    Gfx *next() { return &dl->buf[dl->n++]; }
};
#define CTX (*(Ctx **)&D_00000000)
#define C8(x) ((unsigned)((x) * 255.0f) & 0xFF)
extern "C" void gui_uso_func_00002334(void *self, int a1, float *col, int alpha) {
    Gfx *p1 = CTX->next();
    p1->set(0xFC11EA23, 0xFFD7FFFF);
    Gfx *p2 = CTX->next();
    p2->set(0xFB000000, a1 & 0xFF);
    Gfx *p3 = CTX->next();
    p3->w0 = 0xFA000000;
    p3->w1 = (C8(col[0]) << 24) | (C8(col[1]) << 16) | (C8(col[2]) << 8) | (alpha & 0xFF);
}
