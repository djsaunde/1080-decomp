/* IDO 7.1 NCC (C++) donor for gui_uso_func_000016A4 (0xF0 / 60 words).
 *
 * Caches the gfx context (*(D + 0x254))->0x158 in the global
 * D_gui16a4_cache, then appends four fixed commands (RDPPIPESYNC, two
 * SETOTHERMODE_H, SETOTHERMODE_L) through the context's inline next(),
 * re-reading the global per packet. 055C's sibling. The slot-pointer colours
 * (a1 a2 a3 a1) come from uopt's basic blocks: an empty `if (1) {}` after
 * packets 1 and 3, and nextb() (next() with an empty if between the index
 * bump and the slot address) for packet 2. Found by a 256-variant layout
 * search (docs/IDO_CODEGEN.md kit XXXV). The unused int parameter is still
 * homed (sw a0,0(sp)), as edg does for the gui emitters.
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
extern "C" { extern Ctx *D_gui16a4_cache; }
extern "C" void gui_uso_func_000016A4(int a0) {
    Gfx *p1, *p2, *p3, *p4;
    D_gui16a4_cache = (*(G **)(&D_00000000 + 0x254))->ctx;
    p1 = D_gui16a4_cache->next();
    p1->w0 = 0xE7000000; p1->w1 = 0;
    if (1) {}
    p2 = D_gui16a4_cache->nextb();
    p2->w0 = 0xBA001402; p2->w1 = 0;
    p3 = D_gui16a4_cache->next();
    p3->w0 = 0xBA001301; p3->w1 = 0;
    if (1) {}
    p4 = D_gui16a4_cache->next();
    p4->w0 = 0xB900031D; p4->w1 = 0x00404240;
}
