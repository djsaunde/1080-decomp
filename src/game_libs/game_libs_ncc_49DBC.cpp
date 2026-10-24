/* IDO 7.1 NCC (C++) donor for gl_func_00049DBC: three textured-vertex builds from one
 * 8-byte index record (vertex lookups, optional 0x7C alpha table, colour source by
 * flag: per-vertex call / 0x54 table / 0x50 table / the arg4 float RGB * 255).
 * Spliced into game_libs_post0b.c.o via REPLACE_FUNC_BODY.
 * agent-c run 153: NM donor 97.4 -> EXACT. The arg1 path is a TWO-argument call
 * (its `lui a2` is the else-arm's hoisted address in the beqz delay slot); the else
 * call takes two DISTINCT base-0 symbols (a2 / a3 each get their own lui/addiu);
 * no empty `if (arg2) {}` test; and NO named scalar locals (each one takes a frame
 * home below the arrays: dropping temp_s1 / temp_v0_2 / temp_v1 / base and widening
 * the colour scratch to u8[24] puts sp38 at the target's sp+0x38). */
typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef unsigned short u16;
typedef int s32; typedef unsigned int u32; typedef long long s64; typedef unsigned long long u64;
typedef float f32; typedef double f64;
#undef NULL
#define NULL 0
extern "C" {
extern int D_00000000;
extern int gl_func_00000000(...);
extern void gl_func_00000000_3cbb4m(f32);
extern int arg3;
extern int arg4;
extern int arg1;
extern void gl_func_00000000_49DBCv(...);
extern char gl_d_49dbc_t2[];
extern char gl_d_49dbc_t3[];
}
extern "C" u8 *gl_func_00049DBC(char *arg0, s32 arg1, s32 arg2, s32 arg3, char *arg4) {
    void *spAC;
    void *spA8;
    void *spA4;
    void *spA0;
    void *sp9C;
    void *sp98;
    s32 sp94;
    s32 sp90;
    s32 sp8C;
    s16 sp8A;
    s16 sp88;
    s16 sp86;
    s16 sp84;
    long long sp70[2];
    long long sp60[2];
    long long sp50[2];
    u8 sp38[24];
    s32 var_a0;

    spAC = (void *)gl_func_00000000(arg0, *(u16 *)((*(s32 *)(arg0 + 0x68)) + (arg3 * 8) + 0x2));
    spA8 = (void *)gl_func_00000000(arg0, *(u16 *)((*(s32 *)(arg0 + 0x68)) + (arg3 * 8) + 0x4));
    spA4 = (void *)gl_func_00000000(arg0, *(u16 *)((*(s32 *)(arg0 + 0x68)) + (arg3 * 8) + 0x6));
    var_a0 = *(s32 *)(arg4 + 0x38);
    if ((var_a0 & 0x400000) && !(var_a0 & 0x100000)) {
        spA0 = (void *)gl_func_00000000(arg0, *(u16 *)((*(s32 *)(arg0 + 0x68)) + (arg3 * 8) + 0x2));
        sp9C = (void *)gl_func_00000000(arg0, *(u16 *)((*(s32 *)(arg0 + 0x68)) + (arg3 * 8) + 0x4));
        sp98 = (void *)gl_func_00000000(arg0, *(u16 *)((*(s32 *)(arg0 + 0x68)) + (arg3 * 8) + 0x6));
        var_a0 = *(s32 *)(arg4 + 0x38);
    } else {
        sp98 = NULL;
        sp9C = NULL;
        spA0 = NULL;
    }
    if (*(s32 *)(arg0 + 0x7C) != 0) {
        sp84 = *(u8 *)(*(s32 *)(arg0 + 0x7C) + *(u16 *)((char *)((*(s32 *)(arg0 + 0x68)) + (arg3 * 8)) + 0x2));
        sp86 = *(u8 *)(*(s32 *)(arg0 + 0x7C) + *(u16 *)((char *)((*(s32 *)(arg0 + 0x68)) + (arg3 * 8)) + 0x4));
        sp88 = *(u8 *)(*(s32 *)(arg0 + 0x7C) + *(u16 *)((char *)((*(s32 *)(arg0 + 0x68)) + (arg3 * 8)) + 0x6));
    } else {
        sp88 = 0xFF;
        sp8A = 0;
        sp86 = 0xFF;
        sp84 = 0xFF;
    }
    if (var_a0 & 0x2000) {
        if (*(s32 *)(arg0 + 0x38) & 0x8000) {
            gl_func_00000000(arg0, *(u16 *)((*(s32 *)(arg0 + 0x68)) + (arg3 * 8) + 0x2), &sp94, arg4);
            gl_func_00000000(arg0, *(u16 *)((*(s32 *)(arg0 + 0x68)) + (arg3 * 8) + 0x4), &sp90);
            gl_func_00000000(arg0, *(u16 *)((*(s32 *)(arg0 + 0x68)) + (arg3 * 8) + 0x6), &sp8C);
        } else {
            if (*(s32 *)(arg0 + 0x54) == 0) {
                gl_func_00000000_49DBCv((char *)&D_00000000 + 0x20014);
            }
            gl_func_00000000_49DBCv((char *)&D_00000000, &sp94, *(s32 *)(arg0 + 0x54) + arg3 * 0xC);
            *(u8 *)&sp8C = *(u8 *)&sp94;
            *(u8 *)&sp90 = *(u8 *)&sp94;
        }
    } else {
        if (*(s32 *)(arg0 + 0x50) != 0) {
            gl_func_00000000((char *)&D_00000000, &sp94, *(s32 *)(arg0 + 0x50) + (*(u16 *)((*(s32 *)(arg0 + 0x68)) + (arg3 * 8) + 0x2)) * 4);
            gl_func_00000000((char *)&D_00000000, &sp90, (*(s32 *)(arg0 + 0x50)) + (*(u16 *)((*(s32 *)(arg0 + 0x68)) + (arg3 * 8) + 0x4)) * 4);
            gl_func_00000000((char *)&D_00000000, &sp8C, (*(s32 *)(arg0 + 0x50)) + (*(u16 *)((*(s32 *)(arg0 + 0x68)) + (arg3 * 8) + 0x6)) * 4);
        } else {
            sp38[0] = (u8)(s32)(u32)(*(f32 *)(arg4 + 0x0) * 255.0f);
            sp38[1] = (s8)(s32)(u32)(*(f32 *)(arg4 + 0x4) * 255.0f);
            sp38[2] = (s8)(s32)(u32)(*(f32 *)(arg4 + 0x8) * 255.0f);
            gl_func_00000000((char *)&D_00000000, &sp94, sp38);
            gl_func_00000000((char *)&D_00000000, &sp90, sp38);
            gl_func_00000000((char *)&D_00000000, &sp8C, sp38);
        }
    }
    gl_func_00000000((char *)&D_00000000, &sp50, spAC, spA0, (s32) sp84, &sp94);
    gl_func_00000000((char *)&D_00000000, &sp60, spA8, sp9C, (s32) sp86, &sp90);
    gl_func_00000000((char *)&D_00000000, &sp70, spA4, sp98, (s32) sp88, &sp8C);
    if (arg1 != 0) {
        return (u8 *)gl_func_00000000(arg2, &sp50);
    }
    gl_func_00000000(arg2, &sp50, gl_d_49dbc_t2, gl_d_49dbc_t3, (*(s32 *)(arg0 + 0x68)) + (arg3 * 8) + 2);
    return NULL;
}
