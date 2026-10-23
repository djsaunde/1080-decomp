/* IDO 7.1 NCC (C++) NM-only comparison donor for gl_func_0004E00C: the NM C body lifted under NCC + one parameter empty test from a fz sweep (agent-c run 134, cfe 75.69; a second parameter empty test from round 2 gives 86.31).
 * Still NON_MATCHING: spliced into build/non_matching/ only via REPLACE_FUNC_BODY. */
typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef unsigned short u16;
typedef int s32; typedef unsigned int u32; typedef long long s64; typedef unsigned long long u64;
typedef float f32; typedef double f64;
#undef NULL
#define NULL 0
struct StackB *p;
typedef struct { int w; } Word3E968;
extern "C" {
extern int gl_func_00000000(...);

extern void gl_49B3C_regf(char *, char *, char *, float, float, int);
extern void gl_49B3C_regf(char *, char *, char *, float, float, int);
extern float q[4];
}
extern "C" void gl_func_0004E00C(int *s0) {
    volatile int *v;
    int x;
    int *q;
    int *p;
    int i;
    s0[0x1C8 / 4] = 0;
    s0[0] = 0;
    gl_func_00000000((char *)s0 + 0xA0);
    gl_func_00000000((char *)s0 + 0x60);
    gl_func_00000000((char *)s0 + 0x20);
    s0[0xE4 / 4] = 0;
    v = (volatile int *)((char *)s0 + 0x140); if (s0) {}
    x = (v[1] & 0xF00) | 0x400;
    v[1] = x;
    x |= 2;
    v[1] = x;
    x |= 1;
    v[1] = x;
    v[1] = x;
    v[1] = x | 0x10;
    *(float *)((char *)s0 + 0x130) = 1.0f;
    *(float *)((char *)s0 + 0x12C) = 1.0f;
    *(float *)((char *)s0 + 0x128) = 1.0f;
    gl_func_00000000((char *)s0 + 0xE8);
    gl_func_00000000((char *)s0 + 0xE8, (char *)s0 + 0x128);
    s0[0x1CC / 4] = 0;
    s0[0x18C / 4] = 0;
    *(float *)((char *)s0 + 0x1D0) = 0.0f;
    *(float *)((char *)s0 + 0x1D4) = 0.0f;
    *(float *)((char *)s0 + 0x1D8) = 0.0f;
    *(float *)((char *)s0 + 0x1DC) = 0.5f;
    *(int *)((char *)s0 + 0x148) = 0;
    q = (int *)((char *)s0 + 0x148 + 4); if (s0) {}
    q[0] = 0;
    q[1] = 0;
    q[2] = 0;
    q[3] = 0;
    *(int *)((char *)s0 + 0x15C) = 0;
    p = (int *)((char *)s0 + 0x160);
    for (i = 0; i != 8; i += 4) {
        p[0] = 0;
        p[1] = 0;
        p[2] = 0;
        p[3] = 0;
        p += 4;
    }
    *(int *)((char *)s0 + 0x180) = 0;
    gl_func_00000000(s0, 0);
}
