/* IDO 7.1 NCC (C++) NM-only comparison donor for gl_func_0004E00C: the NM C body lifted under NCC + one parameter empty test from a fz sweep (agent-c run 134, cfe 75.69; a second parameter empty test from round 2 gives 86.31).
 * Still NON_MATCHING: spliced into build/non_matching/ only via REPLACE_FUNC_BODY.
 * agent-c run 160: the 0x144 mode word as five plain read-modify-write statements through the non-volatile base
 * (`v[1] = (v[1] & 0xF00) | 0x400; v[1] = v[1] | 2; v[1] = v[1] | 1; v[1] = v[1]; v[1] = v[1] | 0x10;`): uopt forwards each
 * stored value into the next read, so the target's five stores of four ring temps come out exactly (the volatile chain
 * reused one register), 86.71 -> 90.75.
  * agent-c run 161: the 0x148 / 0x160 tails are two sub-objects cleared by inline members with ARRAY members:
 * `Blk { int w[6]; }` (w[0], an inline `zero(int k)` loop from 1 to 5, w[5]) and `Blk2 { int e[8]; int c; }` (an
 * 8-trip loop that uopt unrolls x4 like the target): 90.75 -> 93.81, frame homes = target. Residual: the target keeps
 * the unrolled 4-trip loop's IV (`li v1,1; sll; addu`) with ascending stores where ours folds it (descending).
 */
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
struct Blk { int w[6]; void zero(int k) { for (; k < 5; k++) w[k] = 0; } void clear() { w[0] = 0; zero(1); w[5] = 0; } };
struct Blk2 { int e[8]; int c; void clear() { for (int k = 0; k < 8; k++) e[k] = 0; c = 0; } };
extern "C" void gl_func_0004E00C(int *s0) {
    int *v;
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
    v = (int *)((char *)s0 + 0x140); if (s0) {}
    v[1] = (v[1] & 0xF00) | 0x400;
    v[1] = v[1] | 2;
    v[1] = v[1] | 1;
    v[1] = v[1];
    v[1] = v[1] | 0x10;
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
    ((Blk *)((char *)s0 + 0x148))->clear();
    ((Blk2 *)((char *)s0 + 0x160))->clear();
    gl_func_00000000(s0, 0);
}
