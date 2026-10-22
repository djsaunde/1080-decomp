/* IDO 7.1 NCC (C++) donor for gui_uso_func_00001EF4 (0x440 / 272 words).
 *
 * Display-list colour setup: G_SETCOMBINE (0xFC309A61 / 0x5536FF7F), then
 * G_SETENVCOLOR (0xFB) from env[0..2] * 255 with the alpha argument, then
 * G_SETPRIMCOLOR (0xFA) from prim[0..2] * 255 with alpha 0xFF. The
 * gui_uso_func_00001A5C recipe (docs/IDO_CODEGEN.md kit XXV): three appends
 * through the context's inline next() member, the context global read as
 * `*(Ctx **)&D_00000000` per append, and every channel masked with & 0xFF
 * before its shift (the gbi.h _SHIFTL form). The old cfe body's "as1
 * scheduler tie + ugen colouring cap" was the missing C++ shape; this is
 * word-exact on the first compile.
 * Spliced via REPLACE_FUNC_BODY into both gui_uso trees. */
extern "C" { extern char D_00000000; }
struct Gfx { unsigned int w0, w1; };
struct DL { Gfx *buf; int n; };
struct Ctx {
    char pad[0xC];
    DL *dl;
    Gfx *next() { int n = dl->n++; return &dl->buf[n]; }
};
#define CTX (*(Ctx **)&D_00000000)
#define C8(x) ((unsigned)((x) * 255.0f) & 0xFF)
extern "C" void gui_uso_func_00001EF4(void *self, int alpha, float *prim, float *env) {
    Gfx *p1 = CTX->next();
    p1->w0 = 0xFC309A61; p1->w1 = 0x5536FF7F;
    Gfx *p2 = CTX->next();
    p2->w0 = 0xFB000000;
    p2->w1 = (C8(env[0]) << 24) | (C8(env[1]) << 16) | (C8(env[2]) << 8) | (alpha & 0xFF);
    Gfx *p3 = CTX->next();
    p3->w0 = 0xFA000000;
    p3->w1 = (C8(prim[0]) << 24) | (C8(prim[1]) << 16) | (C8(prim[2]) << 8) | 0xFF;
}
