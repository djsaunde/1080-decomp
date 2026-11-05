/* IDO 7.1 NCC (C++) NM-only comparison donor for gl_func_00059E80 (0x444 / 273 words), agent-c run 164.
 * Fixed render-state display list (kit CX family): gSPSegment(0, 0), two gSPDisplayList(sub-DL builder results),
 * SetOtherMode_H texture LOD / alpha dither / colour dither / bilerp / perspective, SetCombine, clear + set
 * geometry mode, cycle type, alpha compare, blend + prim colour, gSPClipRatio(3) as four movewords, then the
 * optional z-buffer gSPSetGeometryMode(1) after the 0xF0 sub-call and the final call. Packets on
 * `Ctx::next() { int n = dl->count++; return &dl->buf[n]; }` (the named-d next() of 42938 is 65 here, the plain
 * `&dl->buf[dl->count++]` 70); the two DisplayList packets through an inline dlist(c, v) (96.55 -> 97.62).
 * Per-site next() spelling grid (kit CXVII): the 6th packet (colour dither) on `int n = dl->count; dl->count = n + 1;`
 * (97.62 -> 97.66). cfe body 93.99 -> 97.66, length = target. Residual: the pooled d / n / packet colours around the two calls
 * (target a1 / a3 while the call result is live) and the early hoist of the 3 / 0xFFFD ClipRatio constants.
 * Still NON_MATCHING. */
extern "C" int gl_func_00034458(...);
extern "C" char D_00000000[];
struct Gfx { unsigned int w0, w1; };
struct DLHead { Gfx *buf; int count; };
struct Ctx { char pad[0xC]; DLHead *dl;
    Gfx *next() { int n = dl->count++; return &dl->buf[n]; }
    Gfx *nextB() { DLHead *d = dl; int n = d->count; d->count = n + 1; return &dl->buf[n]; }
    Gfx *nextC() { return &dl->buf[dl->count++]; }
    Gfx *nextD() { int n = dl->count; dl->count = n + 1; return &dl->buf[n]; } };
struct Sub { char pad[0xF0]; int fF0; };
struct Obj { char pad[0x104]; int f104; char pad108[0x148 - 0x108]; Sub *s148; };
#define PKT(c, a, b) { Gfx *_g = (c)->next(); _g->w0 = (a); _g->w1 = (b); }
#define PKTB(c, a, b) { Gfx *_g = (c)->nextB(); _g->w0 = (a); _g->w1 = (b); }
#define PKTC(c, a, b) { Gfx *_g = (c)->nextC(); _g->w0 = (a); _g->w1 = (b); }
#define PKTD(c, a, b) { Gfx *_g = (c)->nextD(); _g->w0 = (a); _g->w1 = (b); }
/* agent-c run 294: the two DisplayList packets back on an inline dlist() (kit CCCLXIII): its parameter temps are one
 * LR across both packets' blocks, so packet 2's n skips a2 like the target (dl a1 / n a3 in packets 2 and 3); the
 * code-free `d` read of c->dl before next() keeps the in-place body's 0xFFFD / t1 sharing */
inline void dlist(Ctx *c, int v) { DLHead *d = c->dl; Gfx *_g = c->next(); _g->w0 = 0x06000000; _g->w1 = v; }
extern "C" void gl_func_00059E80(Ctx *c, Obj *o) {
    int r1, r2;
    /* agent-c run 293: two packet pointers shared by packet pairs (random pair search over the 19 packets): gs by
     * the 0xBC000404 / 0xBC001C04 ClipRatio words (99.231 -> 99.341), gt by the second 0x06 / the 0xB9 packet
     * (-> 99.396), same length; agent-c run 294: gt dropped (inert once the 0x06 packets went back on dlist()), and
     * gu shared by the 0xBA000C02 / 0xBC001404 packets (exhaustive pair search on the dlist body): 99.505 -> 99.560 */
    Gfx *gu;
    Gfx *gs;
    PKT(c, 0xBC000006, 0);
    r1 = gl_func_00034458(D_00000000);
    /* agent-c run 293 wrote these two packets in place (98.663 -> 99.231: the 0x402 packet pointer takes t1 and
     * 0xFFFD reuses it); agent-c run 294: back on dlist() with its code-free `d` read, which keeps that sharing and
     * gives packets 2 / 3 the target's registers (99.505 kept at this step, then the gu pair below) */
    dlist(c, r1);
    r2 = gl_func_00034458(D_00000000);
    dlist(c, r2);
    PKT(c, 0xBA001001, 0);
    PKT(c, 0xBA000402, 0);
    PKT(c, 0xBA000602, 0); /* agent-c run 208: plain next() (re-scored with functionRelocDiffs=none: 97.86 -> 98.48) */
    { gu = c->next(); gu->w0 = 0xBA000C02; gu->w1 = 0x2000; }
    if (1) {}
    PKT(c, 0xBA001301, 0x80000);
    PKT(c, 0xFCFFFFFF, 0xFFFE793C);
    if (1) {} /* agent-g run 202 (paired bbsplit grid, with the one after 0xB9) */
    PKT(c, 0xB6000000, -1);
    PKT(c, 0xB7000000, 0x22204);
    PKT(c, 0xBA001402, 0);
    PKT(c, 0xB9000002, 0);
    if (1) {}
    PKT(c, 0xF9000000, 0xFF00FA);
    PKT(c, 0xFA000000, 0xFFFF);
    { gs = c->next(); gs->w0 = 0xBC000404; gs->w1 = 3; }
    PKTC(c, 0xBC000C04, 3); /* agent-g run 269: nextC() spelling (single-site spelling sweep): 98.48 -> 98.50 */
    { gu = c->next(); gu->w0 = 0xBC001404; gu->w1 = 0xFFFD; }
    { gs = c->next(); gs->w0 = 0xBC001C04; gs->w1 = 0xFFFD; }
    if (o->f104 & 2) {
        gl_func_00034458(c, o->s148->fF0); if (1) {}
        PKT(c, 0xB7000000, 1);
    }
    gl_func_00034458(c);
}
