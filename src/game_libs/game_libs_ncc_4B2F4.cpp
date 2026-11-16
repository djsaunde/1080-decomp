/* IDO 7.1 NCC (C++) donor for Shadow_display_0004B2F4 (0x32C / 203 words), agent-c run 162; EXACT agent-g run 265.
 * The cfe body (98.23) compiled under NCC with: no `if (0) f(&arg0)` home keep (NCC homes arg0 anyway), the
 * Mtx and DL slot reads as explicit `n = h->count; h->count = n + 1;` (each post-increment costs an edg temp
 * slot), and the matrix product as `mtx[i][j] = mtx[i][j] + ...` (the `+=` form costs two more temp words):
 * frame 0xC0 = target, every home = target, 203 words = target, 98.37. Residual: flags | 2 in place (target
 * keeps flags in a2 and builds the operand in t5), the 0xE4 load slot, and the last two DL slots' v0 / v1 / a2
 * colours. agent-c run 239: flags | 2 inside the packet expression (`(flags | 2) & 0xFF) << 16`, no store back)
 * with a folded `+ 0 * (int)(flags)` use on the pre-call `fixed` sum (gives flags the weight for a register:
 * without it flags is left in its home) builds the operand in t5 as the target: 99.36 -> 99.41; left: the last
 * two DL slots' v0 / v1 / a0 / a2 colours. Then: both late DL counts read through a block-scoped named
 * `s32 *h` (the count pointer as a uopt variable: slot v1, last block's h a0 as the target), with the last
 * slot / packet pointer MERGED into slot3 / cmd2 (one variable each instead of two: the merge pays back the 8
 * frame bytes the named h homes cost, frame 0xC0 kept): 99.41 -> 99.63. Left: the first h takes v0 (target
 * a2) and the t-ring of the last block is one register off.
 * agent-c run 245: the list call's `arg0->0x40` pointer held in a NAMED variable (ugen then emits it as a uopt
 * value outside the t-ring, so the post-call block's ring = target: t9 / t4 / t5 / t6 / t7), reusing the dead
 * matrix pointer `b` (a fresh name costs a home: frame 0xC8): 99.63 -> 99.88. Left: the first h (v0, target a2)
 * and that pointer (a2, target t0).
 * agent-g run 265 (kit CCCV argscan, register evidence): at the list call the target still holds the FA packet
 * pointer in a1 and the first count pointer h in a2 (h coloured a2, the arg0->0x40 pointer pushed off a2 to t0),
 * so the call takes them as arguments: `F_4B2F4_list(P(b, 0x60), cmd2, h1)` with h1 function-scoped:
 * 99.88 -> 99.95. Left: that pointer (v0, target t0).
 * agent-g run 265 EXACT (99.95 -> 100): the arg0->0x40 pointer held in the outer loop counter i (an int, cast
 * back for the read; i is dead after the loop) with the loop test spelled `i != 4` (same trip count). i's LR
 * spans the matrix loop, where v0 / v1 / a0-a3 / t1 / t2 are all busy, so the shared LR takes t0 like the
 * target; `i != 4` keeps uopt's `bne` exit test (with i referenced after the loop, `i < 4` becomes sltu/bnez).
 * Spliced over a placeholder on both build paths. */
typedef float f32; typedef unsigned int u32; typedef int s32; typedef short s16; typedef unsigned short u16;
#include "../../include/gdl/gfxlib.h" /* GfxLib */
#include "../../include/gdl/window.h" /* ScreenWindow */
#include "../../include/gdl/dlctx.h"  /* DLContext / DLBuf */
extern "C" {
extern char gGfxLib_4B2F4_flags, gGfxLib_4B2F4_state, gGfxLib_4B2F4_matrix, D_4B2F4_fixed;
extern void F_4B2F4_copy(f32 (*)[4], void *);
extern void F_4B2F4_pack(void *, void *);
extern u32 F_4B2F4_phys(void *);
extern u32 F_4B2F4_list(void *, u32 *, s32 *);
}
#define P(p, o) (*(char **)((char *)(p) + (o)))
#define DLCTX(p) ((DLContext *)(p))
extern "C" void Shadow_display_0004B2F4(char *arg0) {
    s32 i, j, k;
    f32 mtx[4][4];
    f32 (*a)[4];
    f32 (*b)[4];
    char *g;
    s32 flags;
    char *fixed;
    char *ctx;
    s32 slot1, slot2, slot3;
    u32 *cmd1;
    s32 slot4;
    u32 *cmd2, *cmd3;
    u32 list;
    s32 *h1;
    if (GFXLIB(&gGfxLib_4B2F4_flags)->flags & 1) {
        g = &gGfxLib_4B2F4_state;
        /* agent-c run 228: b / a / loop on one line (as1 line tie: the b load issues after the a / bound addresses; 98.37 -> 99.36) */
        b = GFXLIB(g)->extraMtx; a = GFXLIB(&gGfxLib_4B2F4_matrix)->modelView; for (i = 0; i != 4; i++) {
            for (j = 0; j < 4; j++) {
                mtx[i][j] = 0.0f;
                for (k = 0; k < 4; k++) {
                    mtx[i][j] = mtx[i][j] + a[i][k] * b[k][j];
                }
            }
        }
        F_4B2F4_copy(mtx, &D_4B2F4_fixed + 0x3C8A0);
        g = &gGfxLib_4B2F4_state;
        ctx = (char *)GFXLIB(g)->curWindow->dl;
        flags = GFXLIB(g)->mtxMode;
        slot1 = DLCTX(ctx)->mtx.cur->count;
        DLCTX(ctx)->mtx.cur->count = slot1 + 1;
        fixed = (char *)DLCTX(ctx)->mtx.cur->base + (slot1 << 6) + 0 * (int)(flags);
        F_4B2F4_pack(&D_4B2F4_fixed + 0x3C8A0, fixed);
        slot2 = DLCTX(ctx)->gfx.cur->count;
        DLCTX(ctx)->gfx.cur->count = slot2 + 1;
        cmd1 = (u32 *)(DLCTX(ctx)->gfx.cur->base + slot2 * 8);
        cmd1[0] = (((u32)(flags | 2) & 0xFF) << 16) | 0x01000000 | 0x40;
        cmd1[1] = F_4B2F4_phys(fixed);
        if (ctx) {} /* run 144: block split (98.15 -> 98.23, 203 words) */
        g = &gGfxLib_4B2F4_state;
        ctx = (char *)GFXLIB(g)->curWindow->dl;
        h1 = (s32 *)DLCTX(ctx)->gfx.cur; slot3 = h1[1]; h1[1] = slot3 + 1;
        cmd2 = (u32 *)(DLCTX(ctx)->gfx.cur->base + slot3 * 8);
        cmd2[0] = 0xFA000100;
        cmd2[1] = (u32)(GFXLIB(g)->unk24C * 255.0f) & 0xFF;
        ctx = (char *)GFXLIB(g)->curWindow->dl;
        i = (s32)P(arg0, 0x40); list = F_4B2F4_list(P(i, 0x60), cmd2, h1);
        { DLBuf *h = DLCTX(ctx)->gfx.cur; slot3 = h->count; h->count = slot3 + 1; }
        cmd2 = (u32 *)(DLCTX(ctx)->gfx.cur->base + slot3 * 8);
        cmd2[0] = 0x06000000;
        cmd2[1] = list;
    }
}
