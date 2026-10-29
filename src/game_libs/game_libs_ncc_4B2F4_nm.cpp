/* IDO 7.1 NCC (C++) NM-only comparison donor for game_libs_func_0004B2F4 (0x32C / 203 words), agent-c run 162.
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
 * Still NON_MATCHING: build/non_matching/ only. */
typedef float f32; typedef unsigned int u32; typedef int s32;
extern "C" {
extern char D_4B2F4_flags, D_4B2F4_state, D_4B2F4_matrix, D_4B2F4_fixed;
extern void F_4B2F4_copy(f32 (*)[4], void *);
extern void F_4B2F4_pack(void *, void *);
extern u32 F_4B2F4_phys(void *);
extern u32 F_4B2F4_list(void *);
}
#define W(p, o) (*(s32 *)((char *)(p) + (o)))
#define P(p, o) (*(char **)((char *)(p) + (o)))
extern "C" void game_libs_func_0004B2F4(char *arg0) {
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
    if (W(&D_4B2F4_flags, 0x1C4) & 1) {
        g = &D_4B2F4_state;
        /* agent-c run 228: b / a / loop on one line (as1 line tie: the b load issues after the a / bound addresses; 98.37 -> 99.36) */
        b = *(f32 (**)[4])(g + 0xE4); a = (f32 (*)[4])(&D_4B2F4_matrix + 0xA0); for (i = 0; i < 4; i++) {
            for (j = 0; j < 4; j++) {
                mtx[i][j] = 0.0f;
                for (k = 0; k < 4; k++) {
                    mtx[i][j] = mtx[i][j] + a[i][k] * b[k][j];
                }
            }
        }
        F_4B2F4_copy(mtx, &D_4B2F4_fixed + 0x3C8A0);
        g = &D_4B2F4_state;
        ctx = P(P(g, 0x254), 0x158);
        flags = W(g, 0x1F4);
        slot1 = (*(s32 **)(ctx + 0x3C))[1];
        (*(s32 **)(ctx + 0x3C))[1] = slot1 + 1;
        fixed = P(P(ctx, 0x3C), 0) + (slot1 << 6) + 0 * (int)(flags);
        F_4B2F4_pack(&D_4B2F4_fixed + 0x3C8A0, fixed);
        slot2 = (*(s32 **)(ctx + 0xC))[1];
        (*(s32 **)(ctx + 0xC))[1] = slot2 + 1;
        cmd1 = (u32 *)(P(P(ctx, 0xC), 0) + slot2 * 8);
        cmd1[0] = (((u32)(flags | 2) & 0xFF) << 16) | 0x01000000 | 0x40;
        cmd1[1] = F_4B2F4_phys(fixed);
        if (ctx) {} /* run 144: block split (98.15 -> 98.23, 203 words) */
        g = &D_4B2F4_state;
        ctx = P(P(g, 0x254), 0x158);
        { s32 *h = *(s32 **)(ctx + 0xC); slot3 = h[1]; h[1] = slot3 + 1; }
        cmd2 = (u32 *)(P(P(ctx, 0xC), 0) + slot3 * 8);
        cmd2[0] = 0xFA000100;
        cmd2[1] = (u32)(*(f32 *)(g + 0x24C) * 255.0f) & 0xFF;
        ctx = P(P(g, 0x254), 0x158);
        list = F_4B2F4_list(P(P(arg0, 0x40), 0x60));
        { s32 *h = *(s32 **)(ctx + 0xC); slot3 = h[1]; h[1] = slot3 + 1; }
        cmd2 = (u32 *)(P(P(ctx, 0xC), 0) + slot3 * 8);
        cmd2[0] = 0x06000000;
        cmd2[1] = list;
    }
}
