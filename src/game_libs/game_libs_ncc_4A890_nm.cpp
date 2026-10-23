/* IDO 7.1 NCC (C++) NM-only comparison donor for gl_func_0004A890: the NM C body lifted under NCC + one parameter empty test from a fz sweep (agent-c run 134, cfe 60.66; a second parameter empty test from round 2 gives 68.45).
 * Still NON_MATCHING: spliced into build/non_matching/ only via REPLACE_FUNC_BODY. */
typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef unsigned short u16;
typedef int s32; typedef unsigned int u32; typedef long long s64; typedef unsigned long long u64;
typedef float f32; typedef double f64;
#undef NULL
#define NULL 0
typedef struct { int w; } Word3E968;
extern "C" {
extern int D_00000000;
extern Word3E968 *p;
extern void gl_49B3C_regf(char *, char *, char *, float, float, int);
extern int gl_func_00034458(...);
extern int  idx;
extern f32 n[3];
extern int **list;
extern int arg1;
extern int  end;
}
extern "C" s16 *gl_func_0004A890(char *arg0, s32 arg1, s16 *arg2) {
    s16 list[300];
    s16 cnt;
    s16 *result;
    s16 *p;
    s16 *end;
    s32 n;
    s32 idx;
    s32 off;
    s32 sz;
    s16 t0;
    p = arg2;
    t0 = 0;
    if (arg1 > 0) {
        end = arg2 + arg1;
        do {
            list[t0] = *p;
            t0 = t0 + 1;
            n = *(s16 *)(arg0 + 0xD6) - *(s16 *)(arg0 + 0xD4);
            if (n > 0) {
                idx = 0;
                off = 0;
                do {
                    if (*p == *(s16 *)(*(int *)(arg0 + 0xD0) + off)) {
                        list[t0] = idx;
                        t0 = t0 + 1;
                        n = *(s16 *)(arg0 + 0xD6) - *(s16 *)(arg0 + 0xD4);
                    }
                    idx += 1;
                    off += 2;
                } while (idx < n);
            }
            p += 1;
            list[t0] = -1;
            t0 = t0 + 1;
        } while (p != end); if (arg1) {}
    }
    list[t0] = -1;
    t0 = t0 + 1;
    if (t0 >= 299) {
        cnt = t0;
        gl_func_00034458((char *)((char *)&D_00000000 + 0x20058));
        t0 = cnt;
    }
    sz = t0 * 2;
    result = (s16 *)gl_func_00034458(sz);
    gl_func_00034458(result, list, sz); if (arg0) {}
    return result;
}
