#include "common.h"

extern int gl_func_00000000();
extern char D_00000000;
extern int D_8C3C_arg;
extern char D_8C3C_child_desc;
extern char D_8C3C_mid_desc;
extern char D_8C3C_root_desc;

/* Mid section of game_libs.c, split off on 2026-05-07 along with
 * game_libs_o0_8944.c (the 3-fn -O0 cluster at 0x8944..0x8A40).
 * Covers gl_func_00008AE4..gl_func_000093DC (offset 0x8AE4..0x949C).
 * Built at -O2 (default). */

/* gl_func_00008C3C — VERIFIED 92/92 reloc-resolved EXACT (2026-07-03, agent-e W5).
 * Baseline was 88/92 raw = 84 byte-identical + 4 reloc-false (D_0000D138/D_0000D148
 * HI16/LO16: syms valued 0xD138/0xD148 in undefined_syms_auto.txt resolve to the
 * target's lui 0x1 / addiu -0x2EC8 words at link).
 *
 * CAP CRACKED: the long-documented "-O0 temp-pool artifact, NOT C-steerable"
 * call_root home-slot residual (mine sp+0x28 vs target sp+0x34) is pure DECL ORDER.
 * IDO -O0 assigns local frame slots top-down in declaration order, and `register`
 * locals RESERVE slots in that same order (unused). Old form declared call_root
 * inside an inner block AFTER register root/mid/child -> 4th slot = 0x28. Declaring
 * `int *call_root;` FIRST (before the register decls) gives it the top slot 0x34;
 * root/mid/child reserve 0x30/0x2C/0x28. (The old "function scope vs inner-block
 * score-neutral" test kept it after the register decls -- that's why it never moved.)
 *
 * VOID fn -> NOT blocked by the -O0 return-value dead-double-b toolchain gap.
 * PROMOTABLE: replace the #ifdef NON_MATCHING wrap with this body (drop INCLUDE_ASM).
 * No new undefined_syms needed (D_0000D138/D_0000D148/D_8C3C_* already present). */
extern int gl_func_00000000();
extern char D_00000000;
extern char D_8C3C_child_desc, D_8C3C_mid_desc, D_8C3C_root_desc;
extern char D_0000D138, D_0000D148, D_8C3C_v0;
gl_func_00008C3C(a0, a1) int * a0; int a1; {
    int *call_root;       /* declared FIRST -> top local slot 0x34 (-O0 slots are decl-order top-down; register vars reserve the slots below) */
    register int *root;
    register int *mid;
    register int *child;

    gl_func_00000000(&D_00000000, &D_0000D138);
    gl_func_00000000(&D_00000000, *(int*)&D_8C3C_v0);
    gl_func_00000000(&D_00000000, 7, 0, 0);

    root = 0;
    if (root != 0 || (root = (int*)gl_func_00000000(0xD4)) != 0) {
        mid = root;
        if (mid != 0 || (mid = (int*)gl_func_00000000(0x50)) != 0) {
            child = mid;
            if (child != 0 || (child = (int*)gl_func_00000000(0x2C)) != 0) {
                gl_func_00000000(child, &D_0000D148);
                child[0x28 / 4] = (int)&D_8C3C_child_desc;
            }
            mid[0x28 / 4] = (int)&D_8C3C_mid_desc;
        }
        root[0x28 / 4] = (int)&D_8C3C_root_desc;
    }
    call_root = root;
    gl_func_00000000(call_root, a0[0]);
    gl_func_00000000(call_root);
    root = call_root;
    gl_func_00000000(a0, 0, root);
}

/* gl_func_00008DAC: 39-insn 4-call init sequencer. -O0 island (file built
 * OPT_FLAGS := -O0: all 4 args spilled at entry, unfilled jal delays, b-to-epilogue).
 * The 2nd arg is the data symbol at offset 0xD150 referenced DIRECTLY as
 * &D_0000D150 (defined = 0x0000D150 in undefined_syms) so -O0 emits the 2-insn
 * %hi/%lo (lui 0x1; addiu -0x2EB0) the target uses -- NOT &D_00000000+0xD150 (3
 * insns at -O0). rv kept in s0 via `register`. */
extern int gl_func_00000000();
extern char D_0000D150;
void gl_func_00008DAC(int a0, int a1, int a2, int a3) {
    register int rv;
    gl_func_00000000(&D_00000000, &D_0000D150);
    gl_func_00000000(&D_00000000, 8, 0, 0);
    rv = gl_func_00000000(0, a1, a2, a3);
    gl_func_00000000(a0, 1, rv);
}

/* EXACT 2026-10-22 (agent-g run 118) via an IDO 7.1 NCC -O0 donor:
 * src/game_libs/game_libs_ncc_o0_8E48.cpp (the NM body that was here, as C++;
 * no cfe closing-brace b-epilogue). Placeholder spliced over. */
char *gl_func_00008E48(char **arg0, int arg1, s32 arg2) {
    return 0;
}

/* gl_func_00008FFC: -O0 init sequence (fresh decode 2026-05-28, mnemonic disasm).
 * Five callee calls bracketing a global-state mask + a constructor: configures
 * two globals, masks D[0x50] to bit 0x20, builds an object via call3 (returns
 * obj, a3=incoming a2), registers it (call4), links obj into D's list (obj->0x14),
 * sets obj->0x4=1 if obj->0x14 was non-null, then a final call5; returns obj.
 * Data refs use the &D_00000000+offset addend idiom; builds -O0 (game_libs_mid).
 * 82.38% -> 91.08% (2026-05-31). Lever: the target holds base (&D), obj, and a
 * scratch in s0/s1/s2 across the calls — at -O0 that means `register` locals
 * (declared base/obj/tmp in s0/s1/s2 order). The base-CSE residual (target
 * re-materializes &D per global VALUE read; mine shared the s0 base) was busted
 * by routing the +8/+0x4C/+0x17D/+0x50/+0x64 reads through DISTINCT externs
 * (D_00000008.. valued at the offset in undefined_syms_auto.txt) — each read now
 * gets its own lui base, matching the target (+8.7pp). Same distinct-extern
 * lever as the USO &D-CSE busts; works at -O0 too. The bare `&D_00000000` call
 * args + the `base`/`tmp` register pointer correctly stay on the shared s0 base.
 * RESIDUAL (~9%, genuine -O0 caps): obj home-spill — target stores the call3
 * result to obj's home (sw v0,0x34), reloads into s1, RETURNS from the home
 * (lw v0,0x34); `register int *obj` keeps it in s1 (no home), and a plain
 * `int *obj` regresses to 85.5% (loses the s1 deref allocation). Plus -O0 temp
 * reg-renumber. Real wrap, correct logic; stays NM. */
extern int D_00000008, D_0000004C, D_00000050, D_00000064;
extern unsigned char D_0000017D;
/* EXACT 2026-10-22 (agent-g run 118) via an IDO 7.1 NCC -O0 donor:
 * src/game_libs/game_libs_ncc_o0_8FFC.cpp (the NM body that was here, compiled
 * as C++ -- edg emits no closing-brace `b epilogue`, so the "dead-double-b
 * toolchain gap" above is a cfe-only artifact). Placeholder spliced over. */
int gl_func_00008FFC(int a0, int a1, int a2) {
    return 0;
}

/* gl_func_00009100: -O0 init sequence (fresh decode 2026-05-28). Near-twin of
 * gl_func_00008FFC: 3 byte-field copies (D[0x179..0x17B] → D[0x181..0x183]), a
 * call with a duplicated arg (D[0x4C]), a constructor call (a2=4, a3=incoming
 * a2), then the identical obj-register/link tail (call(obj base), set obj->0x4=1
 * if obj->0x14!=0, link obj->0x14=&D, final call(&D,0)); returns obj.
 * base/obj/scratch held in s0/s1/s2 via `register` locals (the -O0 lever from
 * the 8FFC sibling). &D_00000000+offset addend idiom; builds -O0.
 * 79.85 -> 93.54 (2026-05-31): routed every &D byte/word access through DISTINCT
 * externs (D_00000179/17A/17B/181/182/183 unsigned char + D_0000004C/D_00000064
 * int, valued at the offset in undefined_syms_auto.txt) so each re-materializes
 * its own lui base — the build was CSE'ing the 3 byte-copies onto one shared base
 * where the target re-materializes per access. The old comment called this the
 * "-O0 base-CSE nuance hard to steer"; the distinct-extern lever busts it at -O0
 * too (same fix as the 8FFC sibling, +13.69pp). RESIDUAL (~6%): only the obj
 * spill-timing/return-from-home -O0 cap remains (target sw v0,0x34 + reload s1 +
 * return-from-home; register int *obj keeps it in s1) — identical to 8FFC. */
extern unsigned char D_00000179, D_0000017A, D_0000017B, D_00000181, D_00000182, D_00000183;
extern int D_0000004C, D_00000064;
/* gl_func_00009100 [-O0 temp-slot CAP, 93.5%]: same class as gl_func_00008C3C /
 * gl_func_00008FFC. obj is correctly `register` (s1) — objdiff reloc-resolves the
 * distinct-extern D_xxx loads so the lbu-0-vs-377 immediates already score matching;
 * the residual is the obj home-spill (target sw v0,52(sp) / reload, returns from the
 * home slot). TESTED-NEGATIVE 2026-06-23: making obj a plain (spilled) local -> 85.4%
 * (worse); rewriting the D-relative reads as &D_00000000+offset -> 74% (adds a
 * materializing addiu, worse). Original register form stays. */
/* EXACT 2026-10-22 (agent-g run 118) via an IDO 7.1 NCC -O0 donor:
 * src/game_libs/game_libs_ncc_o0_8FFC.cpp (the NM body that was here, compiled
 * as C++ -- edg emits no closing-brace `b epilogue`, so the "dead-double-b
 * toolchain gap" above is a cfe-only artifact). Placeholder spliced over. */
int gl_func_00009100(int a0, int a1, int a2) {
    return 0;
}

/* EXACT 2026-10-22 (agent-g run 118) via an IDO 7.1 NCC -O0 donor:
 * src/game_libs/game_libs_ncc_o0_9204.cpp (8FFC-shape re-decode as C++).
 * Placeholder spliced over. */
int gl_func_00009204(int a0, int a1, int a2) {
    return 0;
}

/* EXACT 2026-10-22 (agent-g run 118) via an IDO 7.1 NCC -O0 donor:
 * src/game_libs/game_libs_ncc_o0_9204.cpp (8FFC-shape re-decode as C++).
 * Placeholder spliced over. */
int gl_func_000092F4(int a0, int a1) {
    return 0;
}

/* gl_func_000093DC: -O0 init/dispatch (scalar-only, no struct locals = no
 * frame-offset ceiling). Real direct-global symbols D_w0 (offset 0) / D_w134
 * (0x134) give the 2-insn at-macro/fold loads; &D+0x10 base = 3-insn; register
 * vars give the s0/s1/s2 usage. fuzzy=100. */
extern int D_w0, D_w134;
/* 2026-05-29: 1-diff cap (lw a0,0 vs lw a0,0x134 on the D_w134 load). D_w134
 * is a direct global at &D+0x134; expected is `lui a0; lw a0,0x134(a0)` (fresh,
 * independent). The placeholder D_w134 (link-resolved) bakes %lo=0 in the .o.
 * The &D+0x134 ADDEND form (which works for isolated loads, e.g. gl_func_00006DC8)
 * REGRESSES here: this function also materializes &D into `base`(s1) for other
 * uses, so IDO CSE-forces a full `lui;addiu;lw 0x134` (extra addiu) instead of the
 * fresh `lui;lw 0x134`. Array-index form same. Tooling-blocked (.o-unverifiable);
 * link-correct. See docs/MATCHING_WORKFLOW.md addend-vs-symbol caveat. */
void gl_func_000093DC(int a0, int a1, int *a2) {
    register int *obj;    /* s0 = a2 */
    register char *base;  /* s1 = &D */
    register char *p;     /* s2 */
    D_w0 = gl_func_00000000(a2, a1);
    obj = a2;
    base = (char *)&D_00000000;
    p = base + 0x10;
    gl_func_00000000(p, obj);
    if (*(int *)((char *)obj + 0x14) != 0) {
        *(int *)((char *)obj + 0x4) = 1;
    }
    p = base;
    *(int *)((char *)obj + 0x14) = (int)p;
    obj = a2;
    gl_func_00000000(D_w0, obj);
    gl_func_00000000(D_w134);
    (void)a0;
}
