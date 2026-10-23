/* IDO 7.1 NCC (C++) donor for gui_uso_func_0000055C (0x15C / 87 words).
 *
 * Caches the gfx context (*(D + 0x254))->0x158 in w->ctx, then appends six
 * fixed commands (RDPPIPESYNC, two SETOTHERMODE_H, SETOTHERMODE_L,
 * SETCOMBINE, TEXTURE) through the context's inline next() (1A5C recipe,
 * docs/IDO_CODEGEN.md kit XXV).
 *
 * The slot pointers' colours (a2 a3 a2 a3 t1 a2) are set by uopt's basic
 * blocks. Interference is per block: two slot pointers conflict only when
 * they share a block. The target has a boundary after the first packet and
 * one between the dl->n store and the slot address of packets 2 and 3.
 * Here those come from an empty `if (1) {}` after packet 1 and from
 * nextb(), which is next() with the same empty if. A 2048-variant search
 * over next() / nextb() per packet and an empty if after each packet found
 * this as the only exact layout (kit XXXV).
 * Spliced via REPLACE_FUNC_BODY into both gui_uso trees. */
extern "C" { extern char D_00000000; }
struct Gfx { unsigned int w0, w1; };
struct DL { Gfx *buf; int n; };
struct Ctx {
    char pad[0xC];
    DL *dl;
    Gfx *next() { int n = dl->n++; return &dl->buf[n]; }
    Gfx *nextb() { int n = dl->n++; if (1) {} return &dl->buf[n]; }
};
struct G { char pad[0x158]; Ctx *ctx; };
struct W { char pad[0x24]; Ctx *ctx; };
extern "C" void gui_uso_func_0000055C(W *w) {
    Gfx *p1, *p2, *p3, *p4, *p5, *p6;
    w->ctx = (*(G **)(&D_00000000 + 0x254))->ctx;
    p1 = w->ctx->next();
    p1->w0 = 0xE7000000; p1->w1 = 0;
    if (1) {}
    p2 = w->ctx->nextb();
    p2->w0 = 0xBA001402; p2->w1 = 0;
    p3 = w->ctx->nextb();
    p3->w0 = 0xBA001301; p3->w1 = 0;
    p4 = w->ctx->next();
    p4->w0 = 0xB900031D; p4->w1 = 0x00404240;
    p5 = w->ctx->next();
    p5->w0 = 0xFC309661; p5->w1 = 0xFF2FFFFF;
    p6 = w->ctx->next();
    p6->w0 = 0xBB000001; p6->w1 = 0x80008000;
}
