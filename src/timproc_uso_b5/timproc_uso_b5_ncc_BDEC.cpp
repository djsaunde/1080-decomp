/* IDO 7.1 NCC (C++) donor for timproc_uso_b5_func_0000BDEC (0x258 / 150 words): the two-arrow
 * UI draw (centre icon at &D+0x6E8, left arrow x-30 at &D+0x700, right arrow x+30 at &D+0x718,
 * colour {c, c, c, a} with c = 192/255 or 64/255 by the 0x20000 dim flag, a = 255/255, the
 * arrows gated on arg2 & 8 when undimmed). Spliced into timproc_uso_b5.c.o (both trees) via
 * REPLACE_FUNC_BODY. agent-g run 167 (docs/IDO_CODEGEN.md kit CIX):
 * - edg keeps the written operand order: `denom * field` and `s2 + field` as in the target
 *   (cfe canonicalised them, the old "FP-renumber tie" cap);
 * - the alpha numerator is `(f32)full()` with an inline returning int 255: edg does not fold
 *   the call, uopt folds the cvt after its CSE pass, so the target's fresh 255.0 is loaded
 *   instead of reusing denom's f20 (`255.0f`, `(float)255` and int / short variables all CSE
 *   or keep a runtime cvt);
 * - `col[3] = a` repeated at the top of each arm of the second test (the target's shared
 *   reload / store join), the colour buffer declared first as 5 words (frame 104).
 * History: runs 2026-05-29 .. 163 (cfe body 76.0 -> 95.82). */
typedef float f32; typedef int s32;
extern "C" {
extern char D_00000000;
int gl_func_00000000(...);
}
inline int full() { return 255; } /* the alpha numerator: an int that uopt folds after CSE */
extern "C" void timproc_uso_b5_func_0000BDEC(char *arg0, char *arg1, int arg2) {
    float col[5]; int s2; float c; float denom = 255.0f; float a;
    s2 = *(int *)(*(char **)(*(char **)(arg1 + 0x414) + 0xC) + 0xC4);
    gl_func_00000000(arg0, *(int *)(arg1 + 0x8C), s2 + *(int *)(arg1 + 0xA4), (char *)&D_00000000 + 0x6E8,
                     (int)(denom * *(float *)(arg1 + 0x4A0)));
    if (*(int *)(arg0 + 0x2B4) & 0x20000) {
        c = 192.0f / denom;
        col[0] = c;
        a = (f32)full() / denom;
        col[1] = c;
        col[2] = c;
        col[3] = a;
        if (arg2 & 8) {
            gl_func_00000000(arg0, *(int *)(arg1 + 0x8C) - 0x1E, s2 + *(int *)(arg1 + 0xA4) + 0x14,
                             col, (char *)&D_00000000 + 0x700, (int)(denom * *(float *)(arg1 + 0x4A0)));
        }
    } else {
        c = 64.0f / denom;
        a = (f32)full() / denom;
        col[0] = c;
        col[1] = c;
        col[2] = c;
        col[3] = a;
        gl_func_00000000(arg0, *(int *)(arg1 + 0x8C) - 0x1E, s2 + *(int *)(arg1 + 0xA4) + 0x14,
                         col, (char *)&D_00000000 + 0x700, (int)(denom * *(float *)(arg1 + 0x4A0)));
    }
    if (!(*(int *)(arg0 + 0x2B4) & 0x20000)) {
        col[3] = a;
        c = 192.0f / denom;
        col[0] = c;
        col[1] = c;
        col[2] = c;
        if (arg2 & 8) {
            gl_func_00000000(arg0, *(int *)(arg1 + 0x8C) + 0x1E, s2 + *(int *)(arg1 + 0xA4) + 0x14,
                             col, (char *)&D_00000000 + 0x718, (int)(denom * *(float *)(arg1 + 0x4A0)));
        }
    } else {
        col[3] = a;
        c = 64.0f / denom;
        col[0] = c;
        col[1] = c;
        col[2] = c;
        gl_func_00000000(arg0, *(int *)(arg1 + 0x8C) + 0x1E, s2 + *(int *)(arg1 + 0xA4) + 0x14,
                         col, (char *)&D_00000000 + 0x718, (int)(denom * *(float *)(arg1 + 0x4A0)));
    }
}
