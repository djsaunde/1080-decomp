/* IDO 7.1 NCC (C++) NM-only comparison donor for gl_func_00059E80 (0x444 / 273 words), agent-c run 164.
 * Fixed render-state display list (kit CX family): gSPSegment(0, 0), two gSPDisplayList(sub-DL builder results),
 * SetOtherMode_H texture LOD / alpha dither / colour dither / bilerp / perspective, SetCombine, clear + set
 * geometry mode, cycle type, alpha compare, blend + prim colour, gSPClipRatio(3) as four movewords, then the
 * optional z-buffer gSPSetGeometryMode(1) after the 0xF0 sub-call and the final call. Packets on
 * `Ctx::next() { int n = dl->count++; return &dl->buf[n]; }` (the named-d next() of 42938 is 65 here, the plain
 * `&dl->buf[dl->count++]` 70); the two DisplayList packets through an inline dlist(c, v) (96.55 -> 97.62).
 * cfe body 93.99 -> 97.62, length = target. Residual: the pooled d / n / packet colours around the two calls
 * (target a1 / a3 while the call result is live) and the early hoist of the 3 / 0xFFFD ClipRatio constants.
 * Still NON_MATCHING. */
extern "C" int gl_func_00034458(...);
extern "C" char D_00000000[];
struct Gfx { unsigned int w0, w1; };
struct DLHead { Gfx *buf; int count; };
struct Ctx { char pad[0xC]; DLHead *dl;
    Gfx *next() { int n = dl->count++; return &dl->buf[n]; } };
struct Sub { char pad[0xF0]; int fF0; };
struct Obj { char pad[0x104]; int f104; char pad108[0x148 - 0x108]; Sub *s148; };
#define PKT(c, a, b) { Gfx *_g = (c)->next(); _g->w0 = (a); _g->w1 = (b); }
inline void dlist(Ctx *c, int v) { PKT(c, 0x06000000, v); }
extern "C" void gl_func_00059E80(Ctx *c, Obj *o) {
    int r1, r2;
    PKT(c, 0xBC000006, 0);
    r1 = gl_func_00034458(D_00000000);
    dlist(c, r1);
    r2 = gl_func_00034458(D_00000000);
    dlist(c, r2);
    PKT(c, 0xBA001001, 0);
    PKT(c, 0xBA000402, 0);
    PKT(c, 0xBA000602, 0);
    PKT(c, 0xBA000C02, 0x2000);
    PKT(c, 0xBA001301, 0x80000);
    PKT(c, 0xFCFFFFFF, 0xFFFE793C);
    PKT(c, 0xB6000000, -1);
    PKT(c, 0xB7000000, 0x22204);
    PKT(c, 0xBA001402, 0);
    PKT(c, 0xB9000002, 0);
    PKT(c, 0xF9000000, 0xFF00FA);
    PKT(c, 0xFA000000, 0xFFFF);
    PKT(c, 0xBC000404, 3);
    PKT(c, 0xBC000C04, 3);
    PKT(c, 0xBC001404, 0xFFFD);
    PKT(c, 0xBC001C04, 0xFFFD);
    if (o->f104 & 2) {
        gl_func_00034458(c, o->s148->fF0);
        PKT(c, 0xB7000000, 1);
    }
    gl_func_00034458(c);
}
