/* IDO 7.1 NCC (C++) donor for gui_uso_func_000008C0 (0x298 / 166 words).
 *
 * Three appends into the widget's gfx context (w->ctx at +0x24): a
 * SETOTHERMODE pair, a SETCOMBINE pair, then a G_SETPRIMCOLOR whose colour
 * is packed from a memory float col[4] of 1.0f (1A5C / 06B8 recipe, kit XXV).
 *
 * The third append goes through nextb(), next() with an empty `if (1) {}`
 * between the index bump and the slot address. uopt ends a straight-line
 * basic block after 20 variable loads (varreflimit), and that rule puts
 * the only block boundary here before the pooled dl temporary's last use.
 * As a result the third slot pointer interferes with that temporary, and
 * the pointer took a0 / a2. The empty if moves the boundary between the
 * dl->n store and the slot address (as in the target), so the slot pointer
 * shares v1 with the dl temporary. See docs/IDO_CODEGEN.md kit XXXV.
 * Spliced via REPLACE_FUNC_BODY into both gui_uso trees. */
struct Gfx { unsigned int w0, w1; };
struct DL { Gfx *buf; int n; };
struct Ctx {
    char pad[0xC];
    DL *dl;
    Gfx *next() { int n = dl->n++; return &dl->buf[n]; }
    Gfx *nextb() { int n = dl->n++; if (1) {} return &dl->buf[n]; }
};
struct W { char pad[0x24]; Ctx *ctx; };
extern "C" void gui_uso_func_000008C0(W *w) {
    float col[4];
    Gfx *p1, *p2, *p3;
    float *c;
    p1 = w->ctx->next();
    p1->w0 = 0xB900031D; p1->w1 = 0x00404240;
    p2 = w->ctx->next();
    p2->w0 = 0xFC309661; p2->w1 = 0xFF2FFFFF;
    col[0] = 1.0f;
    col[1] = 1.0f;
    col[2] = 1.0f;
    col[3] = 1.0f;
    c = col;
    p3 = w->ctx->nextb();
    p3->w0 = 0xFA000000;
    p3->w1 = (((unsigned)(c[0] * 255.0f) & 0xFF) << 24) | (((unsigned)(c[1] * 255.0f) & 0xFF) << 16) |
             (((unsigned)(c[2] * 255.0f) & 0xFF) << 8) | 0xFF;
}
