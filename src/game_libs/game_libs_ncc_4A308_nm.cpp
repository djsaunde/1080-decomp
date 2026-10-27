/* IDO 7.1 NCC (C++) NM-only comparison donor for gl_func_0004A308: the NM C body lifted under NCC (agent-c run 134, cfe 76.89).
 * agent-c run 178 (kit CXXIV de-naming): the vertex-array base `v60` de-named (a macro re-read per use instead of a named
 * local: the named copy cost a home and pushed every temp one register up) and the run-134 `if (arg3) {}` parameter
 * empty test dropped: 77.35 -> 94.24 local, 884 -> 868 bytes (target 872). Residual: frame slot layout (sp74 / sp70 /
 * sp6C / sp48 / sp58 / col at the target's m2c-named offsets; declaration order does not move them) and the
 * entry-pointer colour (v1 vs v0).
 * Still NON_MATCHING: spliced into build/non_matching/ only via REPLACE_FUNC_BODY. */
typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef unsigned short u16;
typedef int s32; typedef unsigned int u32; typedef long long s64; typedef unsigned long long u64;
typedef float f32; typedef double f64;
#undef NULL
#define NULL 0
extern "C" {
extern int D_00000000;
extern void gl_49B3C_regf(char *, char *, char *, float, float, int);
extern void gl_49B3C_regf(char *, char *, char *, float, float, int);
extern int gl_func_00034458(...);
extern int arg3;
extern int arg4;
extern int arg1;
}
extern "C" s32 gl_func_0004A308(char *arg0, int arg1, int arg2, int arg3, char *arg4) {
    char *entry = *(char **)(arg0 + 0x6C) + arg3 * 8;
#define v60 (*(char **)(arg0 + 0x60))
    int t0 = *(unsigned short *)(entry + 4);
    int t2 = *(unsigned short *)(entry + 6);
    char *sp7C = v60 + t0 * 6;
    char *sp78 = v60 + t2 * 6;
    char *pal = *(char **)(arg0 + 0x7C);
    short sp6C, sp6E;
    unsigned char sp74[4], sp70[4], sp48[0x10], sp58[0x10];
    int v0;
    if (pal != 0) {
        sp6C = *(unsigned char *)(pal + t0);
        sp6E = *(unsigned char *)(pal + t2);
    } else {
        sp6E = 0xFF;
        sp6C = 0xFF;
    }
    if (*(char **)(arg0 + 0x50) != 0) {
        gl_func_00034458(&D_00000000, sp74, *(char **)(arg0 + 0x50) + t0 * 4);
        gl_func_00034458(&D_00000000, sp70, *(char **)(arg0 + 0x50) + *(unsigned short *)(*(char **)(arg0 + 0x6C) + arg3 * 8 + 6) * 4);
        gl_func_00034458((char *)&D_00000000 + 0x20030, sp74, sp74[1], sp74[2]);
    } else {
        unsigned char col[4];
        col[0] = (u32)(*(float *)(arg4 + 0) * 255.0f);
        col[1] = (u32)(*(float *)(arg4 + 4) * 255.0f);
        col[2] = (u32)(*(float *)(arg4 + 8) * 255.0f);
        gl_func_00034458(&D_00000000, sp74, col);
        gl_func_00034458(&D_00000000, sp70, col);
    }
    gl_func_00034458(&D_00000000, sp48, sp7C, 0, (s32) sp6C, sp74);
    gl_func_00034458(&D_00000000, sp58, sp78, 0, (s32) sp6E, sp70);
    v0 = 0;
    if (arg1 != 0) {
        v0 = gl_func_00034458(arg2, sp48);
    }
    return v0;
}
