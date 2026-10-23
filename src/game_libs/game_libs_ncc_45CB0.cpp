/* IDO 7.1 NCC (C++) donor for gl_func_00045CB0 (0x170 / 92 words): fill one
 * 0x48-byte light record (ambient + light 0 colour/colour-copy bytes from a
 * float RGB) from the context's light pool, then emit gSPNumLights-style
 * 0xBC000002 / 0x80000040 and two G_MOVEMEM light packets whose w1 comes from
 * an address-conversion call.
 * - C++: two inline next() appends on the context (display list +0xC, light
 *   pool +0x2C) and the unused first parameter homed at entry.
 * - Colours / slots are uopt block placement (kit XXXV): `-varref` showed the
 *   target cut later; the chained byte stores through one `q` pointer save the
 *   counted r/g/b and l loads, and two empty `if (1) {}` (after the b
 *   conversion and after the first packet) place the remaining cuts.
 * - Frame 64: `int k1, k2; k1 = 0; k2 = 0;` (dead stores keep two declared
 *   scalar words, kit XXXIII frame rule) so l / p2 / p3 land on sp+60/32/28.
 * Spliced into game_libs_post0b.c.o via REPLACE_FUNC_BODY (agent-c run 126). */
extern "C" unsigned int gl_func_00000000(...);
struct Gfx { unsigned int w0, w1; };
struct DL { Gfx *buf; int n; };
struct Lt { unsigned char c[0x48]; };
struct LB { Lt *buf; int n; };
struct Ctx {
    char pad[0xC]; DL *dl; char pad10[0x2C - 0x10]; LB *lb;
    Gfx *next() { int n = dl->n++; return &dl->buf[n]; }
    Lt *nextl() { int n = lb->n++; return &lb->buf[n]; }
};
extern "C" void gl_func_00045CB0(int a0, Ctx *c, float *col) {
    Lt *l;
    int k1, k2;
    k1 = 0; k2 = 0;
    l = c->nextl();
    int r, g, b;
    r = col[0] * 255.0f;
    g = col[1] * 255.0f;
    b = col[2] * 255.0f; if (1) {}
    { unsigned char *q = l->c; q[0xC] = q[8] = q[4] = q[0] = r;
    q[0xD] = q[9] = q[5] = q[1] = g;
    q[0xE] = q[0xA] = q[6] = q[2] = b; }
    Gfx *p1 = c->next(); p1->w0 = 0xBC000002; p1->w1 = 0x80000040; if (1) {}
    Gfx *p2 = c->next(); p2->w0 = 0x03880010; p2->w1 = gl_func_00000000(&l->c[8]);
    Gfx *p3 = c->next(); p3->w0 = 0x03860010; p3->w1 = gl_func_00000000(l);
}
