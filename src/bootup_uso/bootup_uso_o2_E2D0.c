/* IDO 7.1 -O2 donor for func_0000E2D0 -- plane reflect / scale helper (0x20C / 131 words).
 *
 * WIRED via REPLACE_FUNC_BODY donor splice into bootup_uso.c.o (the E270 recipe):
 * the f64 literal 0.1 lives in bootup.uso RoData at +0x9A0 (target reloc
 * func_0000098C + 0x14); the donor-local .rodata reloc is renamed
 * func_0000E2D0_rodata and pinned to 0x9A0.
 *
 * The post-call FP temp ring (1.0 f8 / arg4 f16 / sum f18, then f4 f10 f16 /
 * f6 f8 f18 in the scale block) comes from the two code-free tests after the
 * 9F8 line (agent-h run 408): uopt deletes the tests only after ugen has been
 * handed the shared sub-expression, so ugen loads and combines the v[] terms
 * into ring temps that as1 then deletes as dead. Those zero-code pop / free
 * pairs rotate the FIFO into the target's order. Comparing against lim and
 * dot (not constants) also leaves lim in f2 and -lim in f12. arg4 is NOT kept
 * live, so it is a ring temp as in the target. */
#include "common.h"

typedef struct { float x, y, z; } Vec3;
extern float func_00000000_4948_ff(float);
void func_00000000();

void func_0000E2D0(char *arg0, s32 arg1, Vec3 *arg2, Vec3 *arg3, f32 arg4) {
    Vec3 v[2];
    f32 d;
    f32 lim;
    f32 s;
    int X;
    f32 dot;
    int pad[7];

    v[1].x = arg2->x;
    v[1].y = arg2->y;
    v[1].z = arg2->z;
    d = v[1].z * arg3->z + (v[1].y * arg3->y + v[1].x * arg3->x);
    v[1].x = d * arg3->x - v[1].x;
    v[1].y = d * arg3->y - v[1].y;
    v[1].z = d * arg3->z - v[1].z; if (1) {}
    v[0].x = *(f32 *)(arg0 + 0x3BC);
    v[0].y = *(f32 *)(arg0 + 0x3C0);
    v[0].z = *(f32 *)(arg0 + 0x3C4);
    d = v[0].z * arg3->z + (v[0].y * arg3->y + v[0].x * arg3->x);
    v[0].x = d * arg3->x - v[0].x;
    v[0].y = d * arg3->y - v[0].y;
    v[0].z = d * arg3->z - v[0].z;
    if (0.1 < (f64)func_00000000_4948_ff(v[0].z * v[0].z + (v[0].y * v[0].y + v[0].x * v[0].x))) {
        dot = v[1].z * v[0].z + (v[1].y * v[0].y + v[0].x * v[1].x);
        func_00000000(&v[0]); if (arg3) {}
        lim = *(f32 *)(arg0 + 0x9FC);
        dot *= *(f32 *)(arg0 + 0x9F8);
        if ((((v[1].z + v[1].y) + (v[0].z * v[0].x)) - (v[0].y * v[1].x)) != lim) {} if ((((v[1].z + v[1].y) + (v[0].z * v[0].x)) - (v[0].y * v[1].x)) != dot) {}
        if (dot < -lim) {
            dot = -lim;
        }
        if (lim < dot) {
            dot = lim;
        }
        dot *= 1.0f + arg4;
        v[0].x *= dot;
        v[0].y *= dot;
        v[0].z *= dot;
        func_00000000(arg0, arg1, &v[0]);
    }
}
