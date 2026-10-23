/* IDO 7.1 NCC (C++) donor for gl_func_00042778: normalised a2 + matrix column -> two fixed-point coordinates.
 * Spliced into game_libs_post0b.c.o via REPLACE_FUNC_BODY. */
/* (agent-c run 134) 42778 = Vec3 a2 + matrix column 2 of a1, normalised when
 * longer than 0.1 (double at RoData +0x1B30, read through D_00000000), then the
 * two fixed-point screen coordinates 32*4 + (v . column k) * 32 * 2; (32*2, 32*2)
 * otherwise. NCC NM 77.0 (cfe) -> 99.9 -> EXACT. Load-bearing: len is reused for
 * the inverse (len = 1.0 / len): with a separate inv local x/y/z colour f14/f16/f18
 * (target f2/f14/f16, inverse f18). The sum is a2[k] + a1 column (operand order).
 * The 32/4/2 factors are float locals re-assigned per statement (no folding, no
 * CSE across the statements). The frame (104) needs twelve more words of locals
 * above x/y/z: NCC gives every referenced local a slot even when its only store
 * is dead, so pad[12] with one dead store supplies them. Separately named
 * constants, named copies, named int results and inline helpers all changed the
 * code instead. */
typedef float f32; typedef double f64;
extern "C" { extern int D_00000000; extern float gl_func_00000000_42778(float); }
extern "C" void gl_func_00042778(int *a0, float *a1, float *a2) {
    f32 len;
    f32 w, four, two;
    f32 pad[12];
    f32 x, y, z;
    pad[0] = 0;
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
