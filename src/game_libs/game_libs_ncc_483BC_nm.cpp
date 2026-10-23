/* IDO 7.1 NCC (C++) NM-only comparison donor for gl_func_000483BC: the NM C body lifted under NCC (agent-c run 134 sweep, cfe 88.86).
 * Still NON_MATCHING: spliced into build/non_matching/ only via REPLACE_FUNC_BODY. */
typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef unsigned short u16;
typedef int s32; typedef unsigned int u32; typedef long long s64; typedef unsigned long long u64;
typedef float f32; typedef double f64;
#undef NULL
#define NULL 0
extern "C" {
extern int gl_func_00000000(...);
extern long long gl_func_00035164_inner(int a0);
extern void gl_proto_41148(int, int, int, int, int, double);
extern f32 n[3];
}
extern "C" void gl_func_000483BC(char *a0) {
    char *s0 = a0;
    char *pp;
    char *tbl;
    int n, v, t0;
    unsigned int *slot;
    int base;

    if (*(int *)(a0 + 4) == 0) return;
    pp = *(char **)a0;
    tbl = *(char **)(pp + 0xC);
    n = *(int *)(tbl + 4);
    *(int *)(tbl + 4) = n + 1;
    v = *(int *)(a0 + 4);
    slot = (unsigned int *)(*(char **)(*(char **)(pp + 0xC)) + n * 8);
    slot[0] = 0x04000000 | ((((unsigned)v << 0xA) | (((unsigned)v << 4) - 1)) & 0xFFFF);
    slot[1] = (unsigned int)gl_func_00000000(
        *(int *)(*(int *)(*(char **)a0 + 0x1C)) + (*(int *)(a0 + 0xC) << 4));
    *(int *)(s0 + 0x58C) += *(int *)(s0 + 4);
    t0 = 0;
    a0 = s0;
    if (*(int *)(s0 + 8) > 0) {
        do {
            t0++;
            a0 += 6;
            pp = *(char **)s0;
            tbl = *(char **)(pp + 0xC);
            n = *(int *)(tbl + 4);
            *(int *)(tbl + 4) = n + 1;
            slot = (unsigned int *)(*(char **)(*(char **)(pp + 0xC)) + n * 8);
            slot[0] = 0xBF000000;
            base = *(int *)(s0 + 0xC);
            slot[1] = ((((*(short *)(a0 + 0xE) - base) * 2) & 0xFF) << 0x10)
                    | ((((*(short *)(a0 + 0x10) - base) * 2) & 0xFF) << 8)
                    | (((*(short *)(a0 + 0x12) - base) * 2) & 0xFF);
        } while (t0 < *(int *)(s0 + 8));
    }
    gl_func_00000000(s0);
}
