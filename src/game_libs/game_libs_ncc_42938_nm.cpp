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
};
struct Obj254 { char pad[0xB8]; int b8, bc, c0, c4; char padc8[0x148 - 0xC8]; char *p148; char pad14c[0x158 - 0x14C]; Ctx *ctx; };
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
    v->vtrans[2] = 0x1FF;
    v->vscale[2] = 0x1FF;
    v->vscale[3] = 0;
    v->vtrans[3] = 0;
    v->vtrans[0] = (s32)(x * 4.0f + w * 2.0f) & 0xFFFF;
    v->vtrans[1] = (s32)(y * 4.0f + h * 2.0f) & 0xFFFF;
}

extern "C" void game_libs_func_00042938(void) {
    if (gl_d_42938_fa[0] == 0) {
        gl_d_42938_fa[0] = 1;
        gl_d_42938_pa = gl_d_42938_rec[gl_d_42938_idx].a;
    }
    if (gl_d_42938_fb[0] == 0) {
        gl_d_42938_fb[0] = 1;
        gl_d_42938_pb = gl_d_42938_rec[gl_d_42938_idx].b;
    }
    if (gl_d_42938_fc[0] == 0) {
        gl_d_42938_fc[0] = 1;
        gl_d_42938_pc = gl_d_42938_rec[gl_d_42938_idx].c;
    }
    gDPSetScissor(G.o->ctx->nC(), 0, 0, 0, gl_d_42938_ia, gl_d_42938_ib);
    gImmp1(G.o->ctx->next(), 0xB6, -1);
    gSPSetOtherMode(G.o->ctx->next(), 0xB9, 3, 29, 0x442478);
    func_00000000(G.o->ctx, *(int *)(G.o->p148 + 0xF0));
    gImmp1(G.o->ctx->nD(), 0xB7, 1);
    gSPSetOtherMode(G.o->ctx->next(), 0xBA, 20, 2, 0);
    {
        Ctx *c = G.o->ctx;
        Vp *v = c->vpnext();
        setvp(v, 0, 0, 320, 240);
        gDma1p(c->next(), 0x03, func_00000000(v), sizeof(Vp), 0x80);
    }
    G.m228 = G.m224 = 8;
    G.m220 = (G.m21c = 0x32, 0x50);
    { Obj254 *o = G.o; int x = o->c0; int w = o->b8; int h = o->bc; int y = o->c4; func_00000000(&gl_d_42938_cb, &gl_d_42938_qa, x, y, w + x, y + h); }
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
    Rec *r = &gl_d_42938_rec[gl_d_42938_idx];
    int lo1 = gl_d_42938_sa;
    int va = gl_d_42938_pa;
    r->a = (((((va >> 16) & 0xFFFF) + lo1) % 65535) << 16) | (((va & 0xFFFF) + lo1) % 65535);
    int vb = gl_d_42938_pb;
    int lo2 = gl_d_42938_sb;
    r->b = (((((vb >> 16) & 0xFFFF) + lo2) % 65535) << 16) | (((vb & 0xFFFF) + lo2) % 65535);
    int vc = gl_d_42938_pc;
    r->c = (((((vc >> 16) & 0xFFFF) + lo2) % 65535) << 16) | (((vc & 0xFFFF) + lo2) % 65535);
}
