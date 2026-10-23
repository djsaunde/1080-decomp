/* IDO 7.1 NCC (C++) NM-only comparison donor for gl_func_0004E424: the NM C body lifted under NCC + a parameter empty test from a fz sweep (agent-c run 134, cfe 88.33 -> 90.98, 53 -> 51 of 49 words).
 * Still NON_MATCHING: spliced into build/non_matching/ only via REPLACE_FUNC_BODY. */
typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef unsigned short u16;
typedef int s32; typedef unsigned int u32; typedef long long s64; typedef unsigned long long u64;
typedef float f32; typedef double f64;
#undef NULL
#define NULL 0
extern "C" {
extern signed char b;
extern void gl_49B3C_regf(char *, char *, char *, float, float, int);
extern char *c;
}
extern "C" float gl_func_0004E424(float x0, float y0, float z0, float x1, float y1, float z1) {
    float dy, dz;
    float a, b, c;
    float zero = 0.0f;

    x1 -= x0; if (x0) {}
    dy = y1 - y0;
    dz = z1 - z0;
    if (x1 < zero) x1 = -x1;
    if (dy < zero) dy = -dy;
    if (dz < zero) dz = -dz;
    if (x1 < dy) {
        a = x1;
        b = dy;
    } else {
        a = dy;
        b = x1;
    }
    if (dz < b) {
        c = b;
        b = dz;
    } else {
        c = dz;
    }
    return c + (a + b) / (float)2;
}
