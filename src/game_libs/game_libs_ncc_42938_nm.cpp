/* IDO 7.1 NCC (C++) NM-only comparison donor for game_libs_func_00042938 (0x614 / 389 words), agent-c run 160.
 * A C++ frame-setup / text-overlay draw: three guarded one-time initialisations (edg's function-local static
 * shape: flag = 1, then the value from the 0x50-stride record table), stock-gbi packets appended through the
 * global state's context (`G.o->ctx->next()`, gDPSetScissor with its 4.0F float scaling, clear / set geometry
 * mode, the render-mode and cycle-type SetOtherMode words), a viewport taken from the context's Vp pool and set by
 * an inline setvp(x, y, w, h) (the target evaluates 320 * 2 / 240 * 2 / 0 * 4 at run time: inline parameters),
 * gSPViewport via gDma1p, the text-state words at 0x21C..0x228 around six print calls, and the three
 * `% 65535` scroll updates of the current record. next() copies the DL head into a named local (`DLHead *d`) and
 * names the count. The record index is unsigned (the target's multu in the guards, sll / addu in the tail).
 * cfe body 69.98 (1536 bytes) -> 95.07 at 1556 bytes = target. Residual: frame 0x90 vs 0x58 (named d / n per
 * expansion), t-register colours of the guard constants and the DL pool temps.
 * Then 95.07 -> 96.82: the 8 / 8 text-state words as one chained assignment, the 0x50 / 0x32 pair as
 * `m220 = (m21c = 0x32, 0x50)` (the target materialises 0x32 first), and the first print call's rectangle
 * read into named x / y / w / h locals (from one `Obj254 *o` copy) before the call.
 * The two vtrans halfwords carry a folded `& 0xFFFF` (one ugen ring slot each): 96.82 -> 97.15.
 * Still NON_MATCHING: build/non_matching/ only. * agent-c run 164: per-site next() spelling grid (kit CXVII): two packets on nC() (`&dl->buf[dl->count++]`) and
 * nD() (`n = dl->count; dl->count = n + 1`), the rest on the named-d next(); 96.87 -> 96.96 local.
 * agent-c run 193: `G.m228 = G.m224 = 8` (target stores 0x228 first) and the rectangle read x / w / h / y
 * (target loads 0xC4 last-scheduled first): report 97.39 -> 97.43, schedule rows 10 -> 6.
 */
typedef float f32; typedef unsigned int u32; typedef int s32; typedef short s16;
struct Rec { char pad[0x1C]; int a; char p20[0x30 - 0x20]; int b; char p34[0x44 - 0x34]; int c; char p48[0x50 - 0x48]; };
struct Gfx { u32 w0, w1; };
struct DLHead { Gfx *buf; int count; };
struct Vp { s16 vscale[4]; s16 vtrans[4]; };
struct VpHead { Vp *buf; int count; };
template <int K> struct PadK { int a[K]; PadK() {} }; /* agent-c run 288: unreferenced frame pad (a class type with a ctor: edg keeps its home) */
struct Ctx {
    char pad[0xC];
    DLHead *dl;
    char pad10[0x4C - 0x10];
    VpHead *vph;
    Gfx *next() { DLHead *d = dl; int n = d->count; d->count = n + 1; return &dl->buf[n]; }
    Vp *vpnext() { return &vph->buf[vph->count++]; }
    Gfx *nA() { int n = dl->count++; return &dl->buf[n]; }
    Gfx *nB() { DLHead *d = dl; int n = d->count; d->count = n + 1; return &dl->buf[n]; }
    Gfx *nC() { return &dl->buf[dl->count++]; }
    Gfx *nD() { int n = dl->count; dl->count = n + 1; return &dl->buf[n]; }
    /* agent-g run 375: nA()'s named post-increment count plus a code-free `if (n) {}` (a counted use: the count's adjsave
     * rises) for the 0xB6 packet, whose ctx / dl / n / slot now colour a2 / v0 / v1 / a1 like the target: 98.856 -> 98.933;
     * a second test with 0xB7 on next() (no `+ 0`) and the gDma1p slot back on next(): 99.087 -> 99.126.
     * agent-g run 377: that nE() (two tests) retired: after the rect-call folds below, the 0xB6 packet is on nC() and the
     * 0xB9 packet on nF() (one test), 99.164 -> 99.383 -> 99.422 */
    Gfx *nF() { int n = dl->count++; if (n) {} return &dl->buf[n]; }
    /* agent-g run 381: nF() with two code-free tests (two counted uses of the count word) for the 0xB7 packet */
    Gfx *nE() { int n = dl->count++; if (n) {} if (n) {} return &dl->buf[n]; }
    /* agent-g run 379: the viewport slot through a named post-increment count (vA) taken from a zero-code `me()` view of
     * the ctx: c / vph colour a2 / t0 like the target (99.627 -> 99.666 -> 99.692) */
    Vp *vA() { int n = vph->count++; return &vph->buf[n]; }
    Ctx *me() { return this; }
};
extern "C" void gl_func_00000000_void(...);
/* agent-g run 381: the ctx call as a member `cf()` reached through a zero-code `me()` hop: edg puts the inner `this`
 * (G.o) in its SECOND pool word, the 0xB9 count's word, so the count and the call's G.o are one uopt variable and
 * share v1 in one block like the target */
struct Obj254 { char pad[0xB8]; int b8, bc, c0, c4; char padc8[0x148 - 0xC8]; char *p148; char pad14c[0x158 - 0x14C]; Ctx *ctx;
    Obj254 *me() { return this; }
    void cf() { gl_func_00000000_void(ctx, *(int *)(p148 + 0xF0)); } };
struct GState { char pad[0x21C]; int m21c, m220, m224, m228; char pad22c[0x254 - 0x22C]; Obj254 *o; };
extern "C" {
extern u32 gl_d_42938_idx;
extern Rec gl_d_42938_rec[];
extern int gl_d_42938_ia, gl_d_42938_ib;
extern char gl_d_42938_cb, gl_d_42938_z;
extern int gl_d_42938_fa[], gl_d_42938_fb[], gl_d_42938_fc[];
extern GState gl_d_42938_gb;
extern int gl_d_42938_pa, gl_d_42938_pb, gl_d_42938_pc, gl_d_42938_sa, gl_d_42938_sb, gl_d_42938_ck;
extern char gl_d_42938_qa, gl_d_42938_qb, gl_d_42938_qc, gl_d_42938_qd;
void *func_00000000(...);
void gl_func_00000000_void(...); /* agent-g run 377: pinned void alias (=0x0) for the ctx call, kit CCCXLVII */
}
#define G gl_d_42938_gb
#define _SHIFTL(v, s, w) (((u32)(v) & ((1 << (w)) - 1)) << (s))
#define gImmp1(pkt, c, p0) { Gfx *_g = (Gfx *)(pkt); _g->w0 = _SHIFTL((c), 24, 8); _g->w1 = (u32)(p0); }
#define gDPSetScissor(pkt, mode, ulx, uly, lrx, lry) { Gfx *_g = (Gfx *)(pkt); \
    _g->w0 = _SHIFTL(0xED, 24, 8) | _SHIFTL((int)((f32)(ulx) * 4.0F), 12, 12) | _SHIFTL((int)((f32)(uly) * 4.0F), 0, 12); \
    _g->w1 = _SHIFTL(mode, 24, 2) | _SHIFTL((int)((f32)(lrx) * 4.0F), 12, 12) | _SHIFTL((int)((f32)(lry) * 4.0F), 0, 12); }
#define gSPSetOtherMode(pkt, cmd, sft, len, data) { Gfx *_g = (Gfx *)(pkt); \
    _g->w0 = (_SHIFTL(cmd, 24, 8) | _SHIFTL(sft, 8, 8) | _SHIFTL(len, 0, 8)); _g->w1 = (u32)(data); }
#define gDma1p(pkt, c, s, l, p) { Gfx *_g = (Gfx *)(pkt); _g->w0 = (_SHIFTL((c), 24, 8) | _SHIFTL((p), 16, 8) | _SHIFTL((l), 0, 16)); _g->w1 = (u32)(s); }

inline void setvp(Vp *v, f32 x, f32 y, f32 w, f32 h) {
    v->vscale[0] = (s32)(w * 2.0f); if (1) {}
    v->vscale[1] = (s32)(h * 2.0f);
    v->vscale[2] = 0x1FF;
    v->vscale[3] = 0;
    v->vtrans[0] = (s32)(x * 4.0f + w * 2.0f); /* agent-c run 306: the folded mask doubled here and dropped on vtrans[1] (mask-count grid, 81 + 150): one ugen ring slot moved between the two halfwords, 98.118 -> 98.589.
        agent-c run 307 (kit CCCXC 5., FIFO model): no mask at all -- vtrans[0] stays in the mfc1's own t8 and the setvp
        block + gDma1p packet are register-exact; the ring after the call is re-phased by the split constant stores below */
    v->vtrans[1] = (s32)(y * 4.0f + h * 2.0f);
    v->vtrans[2] = 0x1FF; /* agent-c run 306: the eight stores back in field order on the new mask base (store-order random + move search, 200 + 58 + 162): 98.589 -> 98.650 */
    v->vtrans[3] = 0;
}

extern "C" void game_libs_func_00042938(void) {
    /* agent-c run 288: the frame is sized by the home of the one spilled variable (the gSPViewport packet pointer,
     * spilled across the func_00000000(v) call at sp+40): as the gDma1p macro's block-local _g it sat at M-72 (frame
     * 112); a function-scope g under 11 pad words sits at M-48 = the target's frame 88 (98.11311 -> 98.118256) */
    PadK<11> pz;
    Gfx *g;
    /* agent-g run 379: a folded constant address AHEAD of the index in each guard (`0 * (int)&gl_d_42938_z + idx`, zero
     * code): its constant LR piece is numbered between &rec and &idx, so with equal adjsave uopt colours &rec (t0), then
     * it (t1, no code), then &idx / 80 (t2 / t3) -- the target's guard colours; the 0xB9 G.o phantom that held t0 is
     * dropped (99.769 -> 99.884) */
    if (gl_d_42938_fa[0] == 0) {
        gl_d_42938_fa[0] = 1;
        gl_d_42938_pa = gl_d_42938_rec[0 * ((int)&gl_d_42938_z) + gl_d_42938_idx].a;
    }
    if (gl_d_42938_fb[0] == 0) {
        gl_d_42938_fb[0] = 1;
        gl_d_42938_pb = gl_d_42938_rec[0 * ((int)&gl_d_42938_z) + gl_d_42938_idx].b;
    }
    if (gl_d_42938_fc[0] == 0) {
        gl_d_42938_fc[0] = 1;
        gl_d_42938_pc = gl_d_42938_rec[0 * ((int)&gl_d_42938_z) + gl_d_42938_idx].c;
    }
    gDPSetScissor(G.o->ctx->nC(), 0, 0, 0, gl_d_42938_ia, gl_d_42938_ib);
    gImmp1(G.o->ctx->nC(), 0xB6, -1);
    /* agent-g run 381: a code-free block cut before 0xB9, the 0xB9 packet on nA() (no test) in the call's block, the call
     * as `G.o->me()->cf()` and 0xB7 on nE(): the 0xB9 dl / count / slot and the call's G.o take the target's v0 / v1 /
     * a3 / v1 (99.884 -> 99.923) */
    if (1) {}
    gSPSetOtherMode(G.o->ctx->nA(), 0xB9, 3, 29, 0x442478); /* agent-g run 377: on nF() (see nF()); then a folded two-term G.o use (zero code): the guard &idx / 80 take the target's t2 / t3, 99.563 -> 99.627 (agent-g run 379: replaced by the guard folds above, it took t0 from &rec); agent-g run 203: folded `+ 0` packet args re-pool the EDG temps (97.16 -> 97.31 standalone); agent-g run 375: on nC() with no `+ 0` after the nE() change (packet-inline random round), 98.933 -> 99.049 */
    G.o->me()->cf(); /* gl_func_00000000_void(G.o->ctx, *(int *)(G.o->p148 + 0xF0)); agent-g run 377: void alias (the result is never read; no v0 def in the post-call unit): 99.422 -> 99.460 */
    gImmp1(G.o->ctx->nE(), 0xB7, 1); /* agent-g run 377: 0xB7 on nF() and 0xBA on nC() as a pair after the void alias (either alone is worse): 99.460 -> 99.537 */
    gSPSetOtherMode(G.o->ctx->nC(), 0xBA, 20, 2, 0); /* agent-g run 375: `+ 0` dropped (with the gDma1p slot on nA() at that step): 99.049 -> 99.087 */
    {
        Ctx *c = G.o->ctx->me();
        Vp *v = c->me()->me()->vA(); /* agent-g run 379: two zero-code me() hops move vA()'s EDG pool temps to fresh slots (vph / count t0 / v0 like the target) with the gDma1p slot on nA(): 99.692 -> 99.769 */
        setvp(v, 0, 0, 320, 240);
        { g = (Gfx *)(c->nA()); g->w0 = (_SHIFTL((0x03), 24, 8) | _SHIFTL((0x80), 16, 8) | _SHIFTL((sizeof(Vp)), 0, 16)); g->w1 = (u32)(func_00000000(v)); } /* gDma1p(c->next(), 0x03, func_00000000(v), sizeof(Vp), 0x80) through g */
    }
    /* agent-c run 307: the chained `G.m228 = G.m224 = 8` split into two statements (store-order grid with the unmasked
     * vtrans[0]): the after-call ring back in phase, 98.650 -> 98.838; then the 0x32 / 0x50 pair back in its run-157
     * comma form (57 forms x orders): -> 98.856 */
    G.m228 = 8;
    G.m224 = 8;
    G.m220 = (G.m21c = 0x32, 0x50);
    /* agent-g run 377: two folded counted uses of DISTINCT globals on the x / y arguments (zero code; one alone flips to
     * the 1548-byte basin): the call unit gains the occupants that push the gDma1p slot / the 8 constant to the target's
     * t4 / t5 and h to t3 (99.126 -> 99.164 with `0 * (int)G.o->ctx` on x, then the B6 / B9 re-spell, then these two);
     * then `+ 0 * (w + w)` (two more counted uses of w: it now colours before the x / y phantoms, t0 like the target):
     * 99.537 -> 99.563 */
    { Obj254 *o = G.o; int x = o->c0; int w = o->b8; int h = o->bc; int y = o->c4; func_00000000(&gl_d_42938_cb, &gl_d_42938_qa, x + 0 * (gl_d_42938_ia) + 0 * (w + w), y + 0 * (gl_d_42938_sa), w + x, y + h); }
    func_00000000(&G, &gl_d_42938_z, 320);
    G.m224 = 0x10;
    G.m228 = 0x10;
    G.m21c = 0x78;
    G.m220 = 0x78;
    func_00000000(&gl_d_42938_cb, &gl_d_42938_qb, gl_d_42938_sa);
    func_00000000(&G, &gl_d_42938_z, 320);
    func_00000000(&gl_d_42938_cb, &gl_d_42938_qc, gl_d_42938_sb);
    func_00000000(&G, &gl_d_42938_z, 320);
    if (gl_d_42938_ck != 0) {
        func_00000000(&G, &gl_d_42938_qd, 320);
        return;
    }
    /* agent-c run 279: the tail written with NO named locals (the record via a macro, every global read
     * inline): six dead frame homes gone (frame 0x88 -> 0x70, target 0x58) and the 0x2E0 store pair
     * re-ordered; 97.60 -> 98.11 */
#define R (&gl_d_42938_rec[gl_d_42938_idx])
    R->a = (((((gl_d_42938_pa >> 16) & 0xFFFF) + gl_d_42938_sa) % 65535) << 16) | (((gl_d_42938_pa & 0xFFFF) + gl_d_42938_sa) % 65535);
    R->b = (((((gl_d_42938_pb >> 16) & 0xFFFF) + gl_d_42938_sb) % 65535) << 16) | (((gl_d_42938_pb & 0xFFFF) + gl_d_42938_sb) % 65535);
    R->c = (((((gl_d_42938_pc >> 16) & 0xFFFF) + gl_d_42938_sb) % 65535) << 16) | (((gl_d_42938_pc & 0xFFFF) + gl_d_42938_sb) % 65535);
}
