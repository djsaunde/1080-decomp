#include "common.h"

/* bootup_uso 0xFC28..0x100F0 region, split off bootup_uso_tail1.c on
 * 2026-05-28 so that func_0000FBCC could move to a -O0 sub-file
 * (bootup_uso_o0_FBCC.c) and land as a byte-exact match. These four
 * functions stay INCLUDE_ASM NM-wraps (documented -O0 alloc-cascade /
 * dead-BB / t-reg caps — see per-function notes). TRUNCATE_TEXT clips
 * this file's .text to FEE8's true end (0x4C8) so o0_100F0.c.o abuts at
 * bootup offset 0x100F0. */

extern int func_00000000();
extern struct { char _p2C[0x2C]; int f2C; char _p38[0x8]; int f38; } func_00000008; /* USO-base reloc; struct-typed for $at-fused member stores */
extern char D_00000000;
extern char D_0000C58C;
extern char D_0000C594;
extern char D_0000C5A4;
extern char D_0000C5AC;
extern char D_0000C5BC;
extern char D_0000C5C4;
extern int D_bootup_scratch0;         /* int-typed base-0 alias of D_00000000; $at-fused scratch store/reload */

/* EXACT via an IDO 7.1 NCC (C++) -O0 donor: src/bootup_uso/bootup_uso_ncc_FC28.cpp,
 * REPLACE_FUNC_BODY-spliced over this placeholder (agent-c run 119). */
void *func_0000FC28(void *a0) {
    return 0;
}

/* EXACT via an IDO 7.1 NCC (C++) -O0 donor: src/bootup_uso/bootup_uso_ncc_FC28.cpp,
 * REPLACE_FUNC_BODY-spliced over this placeholder (agent-c run 119). */
void *func_0000FD4C(void *a0, int a1, int a2) {
    return 0;
}

/* func_0000FEA0: 18-insn indirect-call wrapper:
 *   p = a0->field_28; (*p->field_64)(a0 + (short)p->field_60).
 * MATCHED 2026-07-10 (w50 island sweep, 18/18): the long-documented
 * "double-reload residual cap" (19 vs 18 insns) was the cfe DAG-share
 * lever from func_0000F6C4 — single expression statement with the
 * register-p assignment EMBEDDED IN THE ARG:
 *   (*(void(**)())(p + 0x64))(*(short*)((p = *(char**)(a0+0x28)) + 0x60) + a0);
 * The arg's `+ a0` DAG-shares the assignment chain's a0 home reload
 * (t6) -> `addu a0,t7,t6` with no second lw; fn-ptr load lands in $t9.
 * Required flipping this file to -O0 (all other members stay INCLUDE_ASM
 * so the flag only affects this compiled body). Old exhaustive-variant
 * notes retired: all four 2026-05-08 variants were two-statement forms. */
void func_0000FEA0(char *a0) {
    register char *p;
    (*(void(**)())(p + 0x64))(*(short*)((p = *(char**)(a0 + 0x28)) + 0x60) + a0);
}

/* func_0000FEE8 - verified structural decode (0x208, 130 insns,
 * get-or-create constructor + normalized-colour init). bootup_uso
 * constructor family (cf. func_00005124 / func_0001438C).
 *   void *func_0000FEE8(void *a0) {
 *       o = a0 ? a0 : func_00000000(0x304);
 *       if (!o) return 0;
 *       s = func_00000000(0x2C);
 *       if (!s) return ...;
 *       func_00000000(s, &D_0000C5BC);            // init sub
 *       s->0x28 = &D_a;                            // descriptor
 *       o->0x28 = &D_b;                            // descriptor
 *       o->0xC  = &D_0000C5C4;                     // type/vtable
 *       f32 *c = (f32*)((char*)o + 0x4C);
 *       c[0] = 0x77 / 255.0f;                      // 119/255
 *       c[1] = 0x75 / 255.0f;                      // 117/255
 *       c[2] = 0xE5 / 255.0f;                      // 229/255
 *       c[3] = ... / 255.0f;                       // (RGBA-ish)
 *       ... (further normalized float fields)
 *   }
 * Struct-typing reference: o = 0x304-byte object; o->0x28 (40)
 * descriptor (&D), o->0xC (12) type/vtable ptr (&D_0000C5C4).
 * s = 0x2C-byte sub-object (s->0x28 descriptor, init datum
 * D_0000C5BC). o+0x4C (76) = a float array initialized from u8
 * literals divided by 255.0f (0x437F0000) - a 0..1 normalized
 * colour / parameter vector (the int-byte / 255.0 idiom). Caps
 * <80: get-or-create + alloc-cascade (~3 reloc) + repeated
 * cvt.s.w + div.s by 255.0 chain + &D descriptors. Full body
 * INCLUDE_ASM-preserved (.s = source of truth). INCLUDE_ASM (no
 * episode; tautology-trap rule). */
/* EXACT via an IDO 7.1 NCC (C++) -O0 donor: src/bootup_uso/bootup_uso_ncc_FC28.cpp,
 * REPLACE_FUNC_BODY-spliced over this placeholder (agent-c run 119). */
void *func_0000FEE8(char *a0) {
    return 0;
}
