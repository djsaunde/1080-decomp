/* NM-only compiler donor for game_libs_func_00065EE4, plain IDO 7.1 -O2
 * (agent-g run 390; replaces the -O2 -Olimit 1 fallback donor, 89.73 -> 98.15).
 * The default parent object keeps its original INCLUDE_ASM; this donor is used
 * only by the non_matching comparison build. No exact episode.
 * Keep this C body synchronized with the parent post1b NM body.
 *
 * Why -O2 and not the size-limit fallback: under -Olimit 1 uopt does nothing
 * and ugen maps every compiler / register temporary of a non-leaf function to
 * a callee-saved register, so neither the target's a2-born data constant
 * (`lui a2; ori a2,a2,0x10; sw a2,36(at)`, = a uopt-coloured LOCAL) nor its
 * a1-first argument order for calls 2 / 3 (= a hoisted temporary coloured a1)
 * is reachable there. Under uopt both are natural: `d` colours a2 and the
 * block-scoped b2 / b3 colour a1. The target re-materialises the record
 * address at every access (`lui at` per store, `lui a0; addiu a0` per call),
 * which uopt only does when no record view is used twice in one basic block
 * (a view's live range across the calls is split and its single-use pieces
 * are never worth a register). The `if (1) {}` moves d's constant load below
 * the base store. The f8 store and the following base load must share a view
 * (with two symbols as1 hoists the load above the store); uopt carves that
 * two-use block out of the view's live range and colours it unless it lies in
 * the uncoloured remainder of an earlier split. The per-region view choice
 * below (a sampled assignment) leaves regions 2 and 3 in such remainders.
 * Left (6 words): region 1's pair (address coloured v1, load hoisted).
 * See docs/IDO_CODEGEN.md kit CDLXXVI.
 */
typedef struct {
    char pad0[8];
    int f8, fC;          /* 0x08, 0x0C: words read back from base / base+4 */
    char pad1[0x14];
    int data;            /* 0x24: base + 0x10 */
    int base;            /* 0x28 */
    int end;             /* 0x2C: base + 0x10 + size */
    int size;            /* 0x30 */
    int f34;             /* 0x34: cleared before the successor call */
} Save65EE4;
extern Save65EE4 D_65EE4_g;   /* sym1520 (BSS 0x3F030), = 0 in undefined_syms_auto.txt */
/* per-access views of the same sym1520 record, all pinned = 0 like D_65EE4_g */
extern Save65EE4 D_65EE4_g_q, D_65EE4_g_r, D_65EE4_g_s, D_65EE4_g_t, D_65EE4_g_u,
                 D_65EE4_g_v, D_65EE4_g_w, D_65EE4_g_x, D_65EE4_g_y;
extern int func_00000000();
/* one region; the view arguments name the record view of each access, in source order */
#define SAVE65EE4_BLOCK(B, SZ, VB, VD, VS, VE, C1, L2, C2, D2, L3, F8, C3, FC, C4, Z, C5) { \
    int d; \
    VB.base = (B); if (1) {} VD.data = d = (B) + 0x10; \
    VS.size = (SZ); VE.end = (B) + 0x10 + (SZ); \
    func_00000000(&C1, (B) + 4, d); \
    { int b2 = L2.base; func_00000000(&C2, b2, D2.data); } \
    { int b3 = L3.base; F8.f8 = func_00000000(&C3, b3); } \
    FC.fC = func_00000000(&C4, F8.base + 4); \
    Z.f34 = 0; \
    func_00000000(&C5); }
#define V65EE4(c) D_65EE4_g_##c
#define G65EE4 D_65EE4_g
void game_libs_func_00065EE4(void) {
    SAVE65EE4_BLOCK(0xB1FF0000, 0x2700, V65EE4(v), G65EE4, V65EE4(q), V65EE4(t), V65EE4(u), V65EE4(t), V65EE4(q), V65EE4(s), V65EE4(t), V65EE4(y), V65EE4(q), V65EE4(q), V65EE4(x), V65EE4(y), V65EE4(s))
    SAVE65EE4_BLOCK(0xB1FF2710, 0x9C30, V65EE4(r), V65EE4(x), V65EE4(v), G65EE4, V65EE4(t), V65EE4(u), V65EE4(w), V65EE4(t), V65EE4(s), V65EE4(y), V65EE4(y), V65EE4(s), V65EE4(w), V65EE4(v), V65EE4(w))
    SAVE65EE4_BLOCK(0xB1FFC350, 0x2700, V65EE4(u), V65EE4(x), V65EE4(s), V65EE4(v), V65EE4(w), V65EE4(q), V65EE4(t), V65EE4(v), V65EE4(s), V65EE4(x), G65EE4, V65EE4(x), V65EE4(v), V65EE4(w), V65EE4(q))
}
#undef V65EE4
#undef G65EE4
