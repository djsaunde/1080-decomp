/* IDO 7.1 NCC (C++) donor for gui_uso_func_000006B8 (0x208 / 130 words).
 *
 * One G_SETPRIMCOLOR (0xFA000000) append into the widget's gfx context
 * (w->ctx at +0x24) with col[0..2] * 255 packed above the alpha byte. Same
 * recipe as gui_uso_func_00001A5C (docs/IDO_CODEGEN.md kit XXV): the append
 * is an inline next() member on the context, and every channel, red
 * included, is (u32)(c * 255.0f) & 0xFF before its shift. The fourth
 * parameter is unused. Word-exact on the first compile.
 * Spliced via REPLACE_FUNC_BODY into both gui_uso trees. */
struct Gfx { unsigned int w0, w1; };
struct DL { Gfx *buf; int n; };
struct Ctx {
    char pad[0xC];
    DL *dl;
    Gfx *next() { int n = dl->n++; return &dl->buf[n]; }
};
struct W { char pad[0x24]; Ctx *ctx; };
extern "C" void gui_uso_func_000006B8(W *w, int alpha, float *col, int unused) {
    Gfx *p = w->ctx->next();
    p->w0 = 0xFA000000;
    p->w1 = (((unsigned)(col[0] * 255.0f) & 0xFF) << 24) | (((unsigned)(col[1] * 255.0f) & 0xFF) << 16) |
            (((unsigned)(col[2] * 255.0f) & 0xFF) << 8) | (alpha & 0xFF);
}
