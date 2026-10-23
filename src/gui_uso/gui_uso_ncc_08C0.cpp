/* IDO 7.1 NCC (C++) donor for gui_uso_func_000008C0 (0x298 / 166 words).
 *
 * Three appends into the widget's gfx context (w->ctx at +0x24): a
 * SETOTHERMODE pair, a SETCOMBINE pair, then a G_SETPRIMCOLOR whose colour
 * is packed from a memory float col[4] of 1.0f (1A5C / 06B8 recipe, kit XXV).
 *
 * uopt ends a straight-line basic block after 20 variable loads
 * (varreflimit, at a statement boundary). For the target colours that
 * boundary must fall between the third append's dl->n store and its slot
 * address. Then the third slot pointer does not interfere with the pooled
 * dl temporary and shares v1 with it. With `int n = dl->n++; return
 * &dl->buf[n];` and p->w0 / p->w1 stores, the boundary came two statements
 * early and p3 took a0. This version is the 17DC recipe: next() without the
 * n copy, and an inline Gfx::set() for the first two packets (reads of
 * set()'s `this` forward to p1 / p2 and are not counted). See
 * docs/IDO_CODEGEN.md kit XXXV.
 * Spliced via REPLACE_FUNC_BODY into both gui_uso trees. */
struct Gfx { unsigned int w0, w1; void set(unsigned a, unsigned b) { w0 = a; w1 = b; } };
struct DL { Gfx *buf; int n; };
struct Ctx {
    char pad[0xC];
    DL *dl;
    Gfx *next() { return &dl->buf[dl->n++]; }
};
struct W { char pad[0x24]; Ctx *ctx; };
extern "C" void gui_uso_func_000008C0(W *w) {
    float col[4];
    Gfx *p1, *p2, *p3;
    float *c;
    p1 = w->ctx->next();
    p1->set(0xB900031D, 0x00404240);
    p2 = w->ctx->next();
    p2->set(0xFC309661, 0xFF2FFFFF);
    col[0] = 1.0f;
    col[1] = 1.0f;
    col[2] = 1.0f;
    col[3] = 1.0f;
    c = col;
    p3 = w->ctx->next();
    p3->w0 = 0xFA000000;
    p3->w1 = (((unsigned)(c[0] * 255.0f) & 0xFF) << 24) | (((unsigned)(c[1] * 255.0f) & 0xFF) << 16) |
             (((unsigned)(c[2] * 255.0f) & 0xFF) << 8) | 0xFF;
}
