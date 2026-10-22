/* IDO 7.1 NCC (C++) donor for gui_uso_func_00001A5C (0x24C / 147 words).
 *
 * Display-list colour emitter: appends a SETCOMBINE (G_CC prim-colour blend) pair
 * (0xFC309661 / 0xFF2FFFFF), then G_SETPRIMCOLOR (0xFA000000) with the RGB of
 * col[0..2] * 255 packed above the alpha byte. gui is C++ (kit XXIII #3):
 *   - the DL append is an inline member on the gfx context (`next()`), so
 *     the context, the list and the index are edg's pooled inline temps
 *     (v0 / v1 / a0 in both appends) while each returned slot pointer is
 *     its own value (a3, t0);
 *   - the context global is read as `*(Ctx **)&D_00000000` per append: its
 *     address is a uopt constant candidate, coloured after the slot
 *     pointers (t1);
 *   - every channel is (u32)(c * 255.0f) & 0xFF before the shift, the red
 *     one included: the redundant mask under `<< 24` is folded only after
 *     the t-ring has spent a register on it (without it every later temp
 *     is one ring slot early);
 *   - the first parameter is an unused `this`-like pointer; edg still
 *     stores it to its home (`sw a0,0(sp)` with no frame).
 * Spliced via REPLACE_FUNC_BODY into both gui_uso trees (docs/IDO_CODEGEN.md
 * kit XXV). */
extern "C" { extern char D_00000000; }
struct Gfx { unsigned int w0, w1; };
struct DL { Gfx *buf; int n; };
struct Ctx {
    char pad[0xC];
    DL *dl;
    Gfx *next() { int n = dl->n++; return &dl->buf[n]; }
};
#define CTX (*(Ctx **)&D_00000000)
extern "C" void gui_uso_func_00001A5C(void *self, int alpha, float *col) {
    Gfx *p1 = CTX->next();
    p1->w0 = 0xFC309661; p1->w1 = 0xFF2FFFFF;
    Gfx *p2 = CTX->next();
    p2->w0 = 0xFA000000;
    p2->w1 = (((unsigned)(col[0] * 255.0f) & 0xFF) << 24) | (((unsigned)(col[1] * 255.0f) & 0xFF) << 16) |
             (((unsigned)(col[2] * 255.0f) & 0xFF) << 8) | (alpha & 0xFF);
}
