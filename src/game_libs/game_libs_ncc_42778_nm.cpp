/* IDO 7.1 NCC (C++) NM-only comparison donor for gl_func_00042778 (112 words =
 * target). NCC 99.9 vs cfe 77.0 (agent-c run 134). Load-bearing: the
 * normalisation reuses len (len = 1.0 / len) instead of an inv local -- with
 * inv, x/y/z/inv colour f14/f16/f18/f2 (target f2/f14/f16/f18); the sum is
 * a2[k] + a1 column (operand order); the 32/4/2 factors are float locals
 * re-assigned per statement (no folding, no cross-statement CSE).
 * Still NON_MATCHING (4 words): the frame is 56 (target 104): the target has
 * 12 more words of locals above x/y/z; separately named constant locals,
 * named row/column copies, named int results, named f64 limit and inline
 * helpers all change the code. Spliced into build/non_matching/ only. */
typedef float f32; typedef double f64;
extern "C" { extern int D_00000000; extern float gl_func_00000000_42778(float); }
extern "C" void gl_func_00042778(int *a0, float *a1, float *a2) {
    f32 len;
    f32 w, four, two;
    f32 x, y, z;
    w = 32.0f;
    four = 4.0f;
    two = 2.0f;
    x = a2[0] + a1[2];
    y = a2[1] + a1[6];
    z = a2[2] + a1[10];
    len = gl_func_00000000_42778(x * x + y * y + z * z);
    if (len > *(f64 *)((char *)&D_00000000 + 0x1B30)) {
        len = 1.0 / len;
        x *= len;
        y *= len;
        z *= len;
        a0[0] = w * four + (x * a1[0] + y * a1[4] + z * a1[8]) * w * two;
        w = 32.0f; four = 4.0f; two = 2.0f;
        a0[1] = w * four + (x * a1[1] + y * a1[5] + z * a1[9]) * w * two;
    } else {
        a0[0] = w * two;
        w = 32.0f; two = 2.0f;
        a0[1] = w * two;
    }
}
