/* IDO 7.1 NCC (C++) EXACT donor for gl_func_00059E80 (0x444 / 273 words): lift agent-c run 164, exact agent-c run 296.
 * Fixed render-state display list (kit CX family): gSPSegment(0, 0), two gSPDisplayList(sub-DL builder results),
 * SetOtherMode_H texture LOD / alpha dither / colour dither / bilerp / perspective, SetCombine, clear + set
 * geometry mode, cycle type, alpha compare, blend + prim colour, gSPClipRatio(3) as four movewords, then the
 * optional z-buffer gSPSetGeometryMode(1) after the 0xF0 sub-call and the final call. Packets on
 * `Ctx::next() { int n = dl->count++; return &dl->buf[n]; }`; the two DisplayList packets through an inline
 * dlist(c, v). REPLACE_FUNC_BODY-spliced over the placeholder in game_libs_post0b.c on both build paths.
 * The gl_func_00034458 calls are the target's blank USO jal words (0x0C000000): they go through the 0-pinned
 * gl_func_00034458_59E80 alias (undefined_syms_auto.txt); D_00000000 is the existing 0 pin (blank lui / addiu). */
extern "C" int gl_func_00034458_59E80(...);
extern "C" char D_00000000[];
struct Gfx { unsigned int w0, w1; };
struct DLHead { Gfx *buf; int count; };
/* agent-c run 296: the 0x1402 packet appends through at(d) with the DL head passed in: its post-increment's old count
 * lands in the shared next() pool temp (the target's v1, the 0xB9 packet's dl), d keeps its own LR (v0) */
struct Ctx { char pad[0xC]; DLHead *dl;
    Gfx *next() { int n = dl->count++; return &dl->buf[n]; }
    Gfx *nextC() { return &dl->buf[dl->count++]; }
    Gfx *at(DLHead *d) { int n = d->count++; return &dl->buf[n]; } };
struct Sub { char pad[0xF0]; int fF0; };
struct Obj { char pad[0x104]; int f104; char pad108[0x148 - 0x108]; Sub *s148; };
#define PKT(c, a, b) { Gfx *_g = (c)->next(); _g->w0 = (a); _g->w1 = (b); }
/* agent-c run 294: the two DisplayList packets on an inline dlist() (kit CCCLXIII): its parameter temps are one
 * LR across both packets' blocks, so packet 2's n skips a2 like the target (dl a1 / n a3 in packets 2 and 3); the
 * code-free `d` read of c->dl before next() keeps the 0xFFFD / t1 sharing */
inline void dlist(Ctx *c, int v) { DLHead *d = c->dl; Gfx *_g = c->next(); _g->w0 = 0x06000000; _g->w1 = v; }
extern "C" void gl_func_00059E80(Ctx *c, Obj *o) {
    int r1, r2;
    /* agent-c runs 293-295 (kit CCCLXV): uopt colours each packet pointer as a block-granular LR, so packets the
     * target gives the SAME register share one pointer variable: gv = the v0 packets (0x602 / 0x1301 / 0xF9 /
     * 0xC04 / last 0xB7), gu = the a0 packets (0xC02 / 0xB7 / 0xFA / 0x1404), gs = the a1 packets (0xB6 / 0x1402 /
     * 0x404) plus 0x1C04 */
    Gfx *gu;
    Gfx *gs;
    Gfx *gv;
    PKT(c, 0xBC000006, 0);
    r1 = gl_func_00034458_59E80(D_00000000);
    dlist(c, r1);
    r2 = gl_func_00034458_59E80(D_00000000);
    dlist(c, r2);
    PKT(c, 0xBA001001, 0);
    PKT(c, 0xBA000402, 0);
    { gv = c->next(); gv->w0 = 0xBA000602; gv->w1 = 0; }
    { gu = c->next(); gu->w0 = 0xBA000C02; gu->w1 = 0x2000; }
    { gv = c->next(); gv->w0 = 0xBA001301; gv->w1 = 0x80000; }
    PKT(c, 0xFCFFFFFF, 0xFFFE793C);
    { gs = c->next(); gs->w0 = 0xB6000000; gs->w1 = -1; }
    { gu = c->next(); gu->w0 = 0xB7000000; gu->w1 = 0x22204; }
    /* agent-c run 296: with at() here the four earlier `if (1) {}` BB splits are all removed -- one block from the
     * 0xC02 packet to the ClipRatio words, so the 0xB9 pointer sees gu / gs / d live and takes the target's a3 */
    { DLHead *d = c->dl; gs = c->at(d); gs->w0 = 0xBA001402; gs->w1 = 0; }
    PKT(c, 0xB9000002, 0);
    { gv = c->next(); gv->w0 = 0xF9000000; gv->w1 = 0xFF00FA; }
    { gu = c->next(); gu->w0 = 0xFA000000; gu->w1 = 0xFFFF; }
    { gs = c->next(); gs->w0 = 0xBC000404; gs->w1 = 3; }
    { gv = c->next(); gv->w0 = 0xBC000C04; gv->w1 = 3; }
    { gu = c->next(); gu->w0 = 0xBC001404; gu->w1 = 0xFFFD; }
    /* agent-c run 296: nextC() with a folded counted use of o->f104 -- its return temp is live into the o-test
     * block (the target's a3) and leaves v0 to the o reload */
    { gs = c->nextC() + 0 * o->f104; gs->w0 = 0xBC001C04; gs->w1 = 0xFFFD; }
    if (o->f104 & 2) {
        gl_func_00034458_59E80(c, o->s148->fF0); if (1) {}
        { gv = c->next(); gv->w0 = 0xB7000000; gv->w1 = 1; }
    }
    gl_func_00034458_59E80(c);
}
