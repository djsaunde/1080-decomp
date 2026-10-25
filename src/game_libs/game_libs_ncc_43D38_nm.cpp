/* IDO 7.1 NCC (C++) NM-only comparison donor for gl_func_00043D38 (0x174 / 93 words), agent-c run 160.
 * Rewritten as real C++ (was the run-134 C lift under NCC, 84.83): `void Elem::clip()` (renamed to gl_func_00043D38 with
 * objcopy) patches the element's row display list (the 493AC Elem: ctx6C, rows7C, n80): rows [lo, hi) of the global
 * state become gDPNoOp, the head slot n80 becomes gSPBranchList to the first kept row (or a NoOp), and the row after the
 * last gets gSPEndDisplayList. Stock gbi macro shapes (gDma1p G_DL / G_DL_NOPUSH = 0x06010000, w1 = the
 * virtual-to-physical call). The rows go through an inline `Ctx::at(int i)` whose parameter is kept (`if (0) { i = 0; }`:
 * the pooled index temp, the target's a2) with `this` = the pooled context temp (a1); the branch target row index is a
 * named local read before the head packet; `lo` straight from GS (no named state pointer: frame 0x38 = target).
 * 84.83 -> 97.51, length 364 -> 372 = target. Residual: the context / index pool temps' colours swapped (a2 / a1).
 * Still NON_MATCHING: build/non_matching/ only. */
typedef short s16; typedef unsigned int u32; typedef int s32;
extern "C" {
extern char D_00000000[];
u32 gl_func_00034458(...);
}
struct Gfx { u32 w0, w1; };
struct DLHead { Gfx *buf; int count; };
struct Ctx { char pad[0xC]; DLHead *dl; Gfx *at(int i) { if (0) { i = 0; } return &dl->buf[i]; } };
struct GState { char pad[0x10C]; int lo; int hi; };
#define GS (*(GState **)(D_00000000 + 0x254))
#define _SHIFTL(v, s, w) (((u32)(v) & ((1 << (w)) - 1)) << (s))
#define gDPNoOp(pkt) { Gfx *_g = (Gfx *)(pkt); _g->w0 = _SHIFTL(0xC0, 24, 8); _g->w1 = 0; }
#define gDma1p(pkt, c, s, l, p) { Gfx *_g = (Gfx *)(pkt); _g->w0 = (_SHIFTL((c), 24, 8) | _SHIFTL((p), 16, 8) | _SHIFTL((l), 0, 16)); _g->w1 = (u32)(s); }
#define gSPBranchList(pkt, dl) gDma1p(pkt, 0x06, dl, 0, 1)
#define gSPEndDisplayList(pkt) { Gfx *_g = (Gfx *)(pkt); _g->w0 = _SHIFTL(0xB8, 24, 8); _g->w1 = 0; }
struct Elem {
    char pad[0x6C]; Ctx *ctx6C; char pad70[0x7C - 0x70]; s16 *rows7C; s16 n80;
    void clip();
};
void Elem::clip() {
    if (rows7C != 0) {
        int lo = GS->lo;
        if (lo > 0) {
            lo -= 1;
        }
        for (int k = lo; k < GS->hi; k++) {
            gDPNoOp(ctx6C->at(rows7C[k]));
        }
        if (lo != 0) {
            int i = rows7C[lo];
            gSPBranchList(ctx6C->at(n80), gl_func_00034458(&ctx6C->dl->buf[i]));
        } else {
            gDPNoOp(&ctx6C->dl->buf[n80]);
        }
        int e = rows7C[GS->hi];
        if (e != -1) {
            gSPEndDisplayList(ctx6C->at(e));
        }
    }
}
