/* NM-only compiler donor for game_libs_func_00065EE4, plain IDO 7.1 -O2
 * (agent-g run 390; replaces the -O2 -Olimit 1 fallback donor, 89.73 -> 96.30).
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
 * which uopt only does when no address is used twice in one block: each
 * access goes through one of eight views of the same record, each used at
 * most once per basic block (a view used three times per region is coloured).
 * The `if (1) {}` moves d's constant load below the base store.
 * Left (12 words): in regions 1 / 2 the f8 store and the next base load share
 * view w, uopt colours that address in v1 and as1 hoists the load above the
 * store (region 3's piece stays uncoloured and matches).
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
extern Save65EE4 D_65EE4_g_q, D_65EE4_g_r, D_65EE4_g_s, D_65EE4_g_t, D_65EE4_g_u, D_65EE4_g_v, D_65EE4_g_w;
extern int func_00000000();
#define SAVE65EE4_BLOCK(B, SZ) { \
    int d; \
    D_65EE4_g.base = (B); if (1) {} D_65EE4_g_q.data = d = (B) + 0x10; \
    D_65EE4_g_r.size = (SZ); D_65EE4_g_s.end = (B) + 0x10 + (SZ); \
    func_00000000(&D_65EE4_g_t, (B) + 4, d); \
    { int b2 = D_65EE4_g_u.base; func_00000000(&D_65EE4_g_v, b2, D_65EE4_g.data); } \
    { int b3 = D_65EE4_g_q.base; D_65EE4_g_w.f8 = func_00000000(&D_65EE4_g_r, b3); } \
    D_65EE4_g_t.fC = func_00000000(&D_65EE4_g_s, D_65EE4_g_w.base + 4); \
    D_65EE4_g_u.f34 = 0; \
    func_00000000(&D_65EE4_g_v); }
void game_libs_func_00065EE4(void) {
    SAVE65EE4_BLOCK(0xB1FF0000, 0x2700)
    SAVE65EE4_BLOCK(0xB1FF2710, 0x9C30)
    SAVE65EE4_BLOCK(0xB1FFC350, 0x2700)
}
