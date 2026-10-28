/* IDO 7.1 NCC (C++) NM-only comparison donor for gl_func_0004A308: the NM C body lifted under NCC (agent-c run 134, cfe 76.89).
 * agent-c run 178 (kit CXXIV de-naming): the vertex-array base `v60` de-named (a macro re-read per use instead of a named
 * local: the named copy cost a home and pushed every temp one register up) and the run-134 `if (arg3) {}` parameter
 * empty test dropped: 77.35 -> 94.24 local, 884 -> 868 bytes (target 872). Residual: frame slot layout (sp74 / sp70 /
 * sp6C / sp48 / sp58 / col at the target's m2c-named offsets; declaration order does not move them) and the
 * entry-pointer colour (v1 vs v0).
 * Still NON_MATCHING: spliced into build/non_matching/ only via REPLACE_FUNC_BODY.
 * agent-c run 179: t2 (the entry+6 index) de-named too, a macro re-read at both uses: 94.47 -> 95.66.
 * agent-g run 190: declarations split from their assignments in the target's top-down slot order (sp7C, sp78, sp74,
 * sp70, sp6E/sp6C, entry, sp58, sp48), the tail as an early `return` of a named block-local result (no v0 variable:
 * 868 -> 872 bytes = target), and a decode fix -- the 0x20030 debug print passes the byte sp74[0], not the array:
 * 95.43 -> 99.60 (objdiff). Left: col / the spill word 4 / 8 bytes low and the entry / v60 v0-v1 swap.
 * agent-g run 198 (kit CLXIII): a folded `+ 0 * (int)v60` on the entry statement puts the 0x60 load's
 * candidate ahead of entry in uopt's bit order: v60 v0 / entry v1 as in the target. Left: the arg3*8 spill
 * at 56 (target 60). Without the block-local `r` the body is word-exact but the frame is 120 (target 128):
 * the target keeps 8 bytes of frame below the spill word that nothing in this body reserves.
 */
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
    /* agent-g run 190: declarations split from the assignments, in the target's top-down slot order */
    char *sp7C;
    char *sp78;
    unsigned char sp74[4];
    unsigned char sp70[4];
    short sp6E, sp6C;
    char *entry;
    unsigned char sp58[0x10];
    unsigned char sp48[0x10];
    int t0;
#define pal (*(char **)(arg0 + 0x7C)) /* agent-g run 191: de-named (its home pushed col 4 bytes low) */
    entry = *(char **)(arg0 + 0x6C) + arg3 * 8 + 0 * (int)(*(char **)(arg0 + 0x60)); /* run 198: folded, no code (kit CLXIII) */
#define v60 (*(char **)(arg0 + 0x60))
    t0 = *(unsigned short *)(entry + 4);
#define t2 (*(unsigned short *)(entry + 6))
    sp7C = v60 + t0 * 6;
    sp78 = v60 + t2 * 6;
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
        gl_func_00034458((char *)&D_00000000 + 0x20030, sp74[0], sp74[1], sp74[2]); /* run 190 decode fix: the target passes the byte (lbu a1) */
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
    if (arg1 != 0) { /* agent-g run 190: early return, no result variable (868 -> 872 B = target) */
        int r = gl_func_00034458(arg2, sp48); return r;
    }
    return 0;
}
