/* IDO 7.1 NCC (C++) NM-only comparison donor for gl_func_00042778: the NM C body lifted under NCC (agent-c run 134 sweep, cfe 77.04).
 * Still NON_MATCHING: spliced into build/non_matching/ only via REPLACE_FUNC_BODY. */
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
        f32 inv = 1.0 / len;
        x *= inv;
        y *= inv;
        z *= inv;
        a0[0] = w * four + (x * a1[0] + y * a1[4] + z * a1[8]) * w * two;
        w = 32.0f; four = 4.0f; two = 2.0f;
        a0[1] = w * four + (x * a1[1] + y * a1[5] + z * a1[9]) * w * two;
    } else {
        a0[0] = w * two;
        w = 32.0f; two = 2.0f;
        a0[1] = w * two;
    }
}
