/* IDO 7.1 NCC (C++) -O0 donor for gl_func_00008E48 (0x1B4 / 109 words): the
 * cfe NM body unchanged; under edg -O0 there is no closing-brace b-epilogue
 * marker (cfe built 111 words). Spliced into game_libs_mid.c.o. */
extern "C" {
typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef unsigned short u16;
typedef int s32; typedef unsigned int u32; typedef float f32; typedef double f64;
typedef long long s64; typedef unsigned long long u64;
#define NULL 0


/* gl_func_00008E48 — VERIFIED 101/111 reloc-resolved (was 10/109 raw baseline).
 * ALL 109 target words reproduced in order except the live `b epilogue` offset
 * (10000003 vs 10000001) + a DEAD `b epilogue; nop` pair (build 111 vs target 109):
 * the documented -O0 RETURN-VALUE dead-double-b TOOLCHAIN-BINARY GAP
 * (docs/IDO_CODEGEN.md). Proven not C-fixable. HONEST CAP -> stays NM at ~98%
 * (up from 92.98%).
 *
 * Levers applied (all verified in-tree):
 * 1. sp3C/sp38/sp34 are PLAIN locals declared FIRST in that order -> slots
 *    0x3C/0x38/0x34 top-down; register base/o/tmp reserve 0x30/0x2C/0x28.
 *    (Old wrap had sp3C as register s1 -> `move s1,v0` instead of the target's
 *    sw v0,0x3C / lw s1,0x3C / return-from-home, and sp38/sp34 landed 0x30/0x2C.)
 * 2. Chain direction swapped: `sp34 = sp38 = (char *)D_0000004C;` -> target's
 *    store order sw t6,0x38 THEN sw t6,0x34 (old `sp38 = sp34 = ...` reversed it).
 * 3. First call arg1 via `*(int *)((char *)&D_8FFC_base + 8)` (base-0 alias,
 *    shared with 8FFC, in undefined_syms_auto.txt) -> 3-insn lui/addiu/lw-off-8
 *    with NO CSE against the &D_00000000 arg0 materialization.
 * 4. Separate `register char *o` (s1) alias of sp3C for the tail derefs;
 *    `tmp = base; FW(o,0x14) = (int)tmp;` two-step keeps the target's
 *    `move s2,s0; sw s2,20(s1)` (direct base store folds to sw s0 — wrong).
 * undefined_syms_auto.txt ADDITION (shared with 8FFC, line 578):
 *   D_8FFC_base = 0x00000000;
 */
#ifndef FW
#define FW(p, o) (*(int *)((char *)(p) + (o)))
#endif
extern int gl_func_00000000(...);  /* USO placeholder callee (jal 0) */
extern char D_00000000;
extern int D_0000004C, D_00000064;
extern unsigned char D_00000181, D_00000182, D_00000183;
extern char D_8FFC_base;   /* base-0 alias of D_00000000 (see gl_func_00008FFC note) */
char *gl_func_00008E48(char **arg0, int arg1, s32 arg2) {
    char *sp3C;            /* plain locals decl'd first: slots 0x3C/0x38/0x34 top-down */
    char *sp38;
    char *sp34;
    register char *base;   /* s0 */
    register char *o;      /* s1 */
    register char *tmp;    /* s2 */

    sp34 = sp38 = (char *)D_0000004C;
    gl_func_00000000(&D_00000000, *(int *)((char *)&D_8FFC_base + 8));
    if (FW(FW((*(int*)arg0), 0x8), 0x8) != 0) {
        D_00000181 = gl_func_00000000(FW((*(int*)arg0), 0x8));
        sp34 = (char *)(int)D_00000181;
        D_00000182 = gl_func_00000000(FW((*(int*)arg0), 0x8));
        D_00000183 = gl_func_00000000(FW((*(int*)arg0), 0x8));
    }
    gl_func_00000000(sp38, sp34);
    if (FW(FW((*(int*)arg0), 0x8), 0x8) != 0) {
        sp3C = (char *)gl_func_00000000(&D_00000000, D_00000064, 3, arg2);
    } else {
        sp3C = (char *)gl_func_00000000(&D_00000000, D_00000064, 2, arg2);
    }
    base = (char *)&D_00000000;
    o = sp3C;
    tmp = base + 0x10;
    gl_func_00000000(tmp, o);
    if (FW(o, 0x14) != 0) {
        FW(o, 0x4) = 1;
    }
    tmp = base;
    FW(o, 0x14) = (int)tmp;
    gl_func_00000000(&D_00000000, 0);
    (void)arg1;
    return sp3C;
}
}
