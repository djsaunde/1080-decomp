/* IDO 7.1 NCC (C++) NM-only comparison donor for gl_func_00033094: the NM C body lifted under NCC (agent-c run 134 sweep, cfe 70.95).
 * Still NON_MATCHING: spliced into build/non_matching/ only via REPLACE_FUNC_BODY. */
typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef unsigned short u16;
typedef int s32; typedef unsigned int u32; typedef long long s64; typedef unsigned long long u64;
typedef float f32; typedef double f64;
#undef NULL
#define NULL 0
typedef struct { float x, y, z; } Vec3;
typedef struct { int w0, w1; } GfxPair1EE78;
extern "C" {
extern int gl_func_00000000(...);
extern signed char k;
extern GfxPair1EE78 *p;
extern int x;
extern int y;
extern int gl_ref_000400A4(char *, unsigned char, char *, char *);
extern int gl_func_00000000_f(int, float);
extern void gl_33094_emit(void *, float, float, float, float, float, float, float, float, float);
extern void gl_func_00043BE4(char *o, int event, int zero, float vol, int idx);
extern void gl_33094_emit(void *, float, float, float, float, float, float, float, float, float);
extern char D_33094_k[];
}
extern "C" int gl_func_00033094(char *o) {
    Vec3 a;
    Vec3 b;
    Vec3 t;
    Vec3 *k;
    char *p;

    if (1) { Vec3 *q = &a; q = &b; }
    a.x = *(float *)(o + 0xA0);
    a.y = *(float *)(o + 0xA4);
    a.z = *(float *)(o + 0xA8);
    p = *(char **)(o + 0xF4);
    if (p != 0) {
        /* run 149: held in block locals, then stored (kit LXXXVI): 80.26 -> 84.65 */
        f32 hx = *(float *)(p + 0xA0); f32 hy = *(float *)(p + 0xA4); f32 hz = *(float *)(p + 0xA8);
        b.x = hx; b.y = hy; b.z = hz;
    } else {
        t = *(Vec3 *)(o + 0x104);
        b.x = t.x;
        b.y = t.y;
        b.x = b.x * 10.0f;
        b.x = b.x + a.x; /* agent-c run 181: lever-sweep order hit (independent fields, no calls) */
        b.y = b.y * 10.0f;
        b.z = t.z * 10.0f + a.z;
        b.y = b.y + a.y; /* agent-c run 174: lever-sweep order hit (b.y / b.z independent locals, no calls): 87.00 -> 87.22 */
    }
    /* agent-c run 183 decode fix: D_33094_k is pinned AT 0x128 (the target's `addiu v0,v0,0x128` + lwc1 0/4/8(v0));
     * the old `D_33094_k + 296` read 0x250 (offset counted twice). */
    k = (Vec3 *)D_33094_k;
    a.x = a.x * k->x;
    a.y = a.y * k->y;
    a.z = a.z * k->z;
    b.x = b.x * k->x;
    b.y = b.y * k->y;
    b.z = b.z * k->z;
    gl_33094_emit(o + 0xB4, a.x, a.y, a.z, b.x, b.y, b.z,
                  *(float *)(o + 0x140), *(float *)(o + 0x144), *(float *)(o + 0x148));
    if (*(int *)(o + 0x130) != 0) {
        gl_func_00000000(o + 0xB4, 1, *(int *)(o + 0x138), 0);
        gl_func_00000000(o + 0xB4, 0, *(int *)(o + 0x134), 0);
    }
    return 1;
}
