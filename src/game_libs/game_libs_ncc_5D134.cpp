/* IDO 7.1 NCC (C++) donor for game_libs_func_0005D134 (0xBC / 47 words): rotate the
 * quaternion q = (w, x, y, z) about axis i (1-based) by the half-angle pair (c, s) --
 * the (q[i], q[0]) and (q[j], q[k]) Givens pairs. Under cfe it capped at 96.5 (FP
 * operand order + pointer colours); under NCC: `(f32)1 * s` written at both uses (no t
 * local), pk = &q[k] assigned BEFORE the q[i] pair (colours a1 ahead of &q[i] = a2),
 * pj named after it (t0), and a dead `if (q) {}` keeps q in a0 (no home store / copy).
 * Spliced into game_libs_post0b.c.o via REPLACE_FUNC_BODY. */
typedef float f32;
typedef int s32;
extern "C" void game_libs_func_0005D134(f32 *q, f32 c, f32 s, s32 i) {
    s32 j, k;
    f32 a;
    f32 *pk, *pj;
    j = i + 1;
    if (j >= 4) j = 1; if (q) {}
    k = j + 1;
    if (k >= 4) k = 1;
    pk = &q[k];
    a = q[i];
    q[i] = a * c + ((f32)1 * s) * q[0];
    q[0] = q[0] * c - ((f32)1 * s) * a;
    pj = &q[j];
    a = *pj;
    *pj = a * c + s * *pk;
    *pk = *pk * c - s * a;
}
