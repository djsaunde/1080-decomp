/* IDO 7.1 -O2 donor for gl_func_000396FC -- message handler, switch on a1[0]
 * (109..114, 6-entry jumptable) (0x264 / 153 words; 153/153 word-exact
 * standalone apart from the two .rodata reloc fields).
 *
 * WIRED via REPLACE_FUNC_BODY donor splice into game_libs_post0b.c.o (the
 * 29CCC / 52CD4 / 51AD8 recipe, docs/MATCHING_WORKFLOW.md#donor-splice-switch-
 * jumptable-rodata-rename). The donor .rodata holds the jumptable at +0 and
 * the 1/32767 literal at +0x18; the splice renames that reloc to
 * gl_func_000396FC_rodata, pinned to 0x1A64 in undefined_syms_auto.txt
 * (the USO ships the table at +0x1A64 and the literal at +0x1A7C).
 *
 * EXACT 2026-11-13 (agent-h run 371): the last word was the bound check's
 * delay slot (ours: as1 xbb moved the default head `or a0,a3` into it; the
 * target keeps the fall-through `sll t7,t7,2`). An `.alias` directive between
 * case 114's closing `b` and the default label makes the default block
 * unreachable in as1's CFG (kit CDL), so xbb fills the slot from the jump
 * block instead. The two zero-byte struct copies at the end of case 114 make
 * &D_00000000 one uopt LR (`la $2`) that ugen tags `.noalias $2,$sp` and
 * closes after the arm's `b`; they emit no surviving code (as1 deletes the
 * dead `la` / `addu` address temps). Everything else is agent-c's 2026-10-15 decode
 * (docs/IDO_CODEGEN.md#param-split-spill-home-396fc). */
#include "common.h"

extern int D_00000000;
extern int gl_func_00000000();

typedef struct { int m[16]; } Mtx16_396FC;
typedef struct { char c[0]; } Z0_396FC;
typedef struct { f32 v[10]; } Xform_396FC;

void gl_func_000396FC(int *a0, int *a1) {
    int b3[3];
    int b2[3];
    int b1[3];
    int *q;
    short *s;
    Mtx16_396FC *m;
    f32 x;
    f32 y;
    Xform_396FC xf;
    f32 z;
    f32 w;
    int *p;
    int pad1;
    short *s2;
    switch (a1[0]) {
    case 109:
        gl_func_00000000(a1, b1);
        if (1) {}
        gl_func_00000000((char *)a0 + 0x30, b1);
        break;
    case 110:
        gl_func_00000000(a1, b2);
        if (1) {}
        gl_func_00000000((char *)a0 + 0x30, b2);
        break;
    case 111:
        gl_func_00000000(a1, b3);
        if (1) {}
        gl_func_00000000((char *)a0 + 0x30, b3);
        break;
    case 112:
        q = (int *)a1[1]; a1[1] = (int)(q + 1);
        s = (short *)*q;
        *(Mtx16_396FC *)((char *)a0 + 0x30) = *(Mtx16_396FC *)s;
        break;
    case 113:
        p = (int *)((char *)a0 + 0x2C);
        if (1) {
            *p |= 2;
        }
        break;
    case 114:
        q = (int *)a1[1]; a1[1] = (int)(q + 1); s = (short *)*q;
        x = s[3] * (1.0f / 32767.0f);
        y = s[4] * (1.0f / 32767.0f);
        z = s[5] * (1.0f / 32767.0f);
        w = s[6] * (1.0f / 32767.0f);
        s2 = s;
        xf.v[6] = x;
        xf.v[7] = y;
        xf.v[8] = z;
        xf.v[9] = w;
        xf.v[0] = s2[0];
        xf.v[1] = s2[1];
        xf.v[2] = s2[2];
        x = *(f32 *)((char *)&D_00000000 + 0x1A10) * s2[7];
        y = *(f32 *)((char *)&D_00000000 + 0x1A10) * s2[8];
        z = *(f32 *)((char *)&D_00000000 + 0x1A10) * s2[9];
        xf.v[3] = x;
        xf.v[4] = y;
        xf.v[5] = z;
        m = (Mtx16_396FC *)((char *)a0 + 0x30);
        gl_func_00000000(&xf, m);
        *(Mtx16_396FC *)((char *)a0 + 0x70) = *m;
        *(Z0_396FC *)&xf = *(Z0_396FC *)&D_00000000;
        *(Z0_396FC *)b1 = *(Z0_396FC *)&D_00000000;
        break;
    default:
        gl_func_00000000(a0, a1);
        break;
    }
}
