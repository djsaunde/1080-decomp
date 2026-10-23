/* IDO 7.1 NCC (C++) NM-only comparison donor for gl_func_00049DBC: the NM C body lifted under NCC (agent-c run 134 sweep, cfe 96.04).
 * Still NON_MATCHING: spliced into build/non_matching/ only via REPLACE_FUNC_BODY. */
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
    s16 sp88;
    s16 sp86;
    s16 sp84;
    s32 sp70[4];
    s32 sp60[4];
    s32 sp50[4];
    u8 sp38[11];
    s32 temp_s1;
    s32 temp_v0_2;
    s32 temp_v1;
    s32 var_a0;
    char *base;

    temp_s1 = arg3 * 8;
    spAC = (void *)gl_func_00000000(arg0, *(u16 *)((*(s32 *)(arg0 + 0x68)) + temp_s1 + 0x2));
    spA8 = (void *)gl_func_00000000(arg0, *(u16 *)((*(s32 *)(arg0 + 0x68)) + temp_s1 + 0x4));
    spA4 = (void *)gl_func_00000000(arg0, *(u16 *)((*(s32 *)(arg0 + 0x68)) + temp_s1 + 0x6));
    var_a0 = *(s32 *)(arg4 + 0x38);
    if ((var_a0 & 0x400000) && !(var_a0 & 0x100000)) {
        spA0 = (void *)gl_func_00000000(arg0, *(u16 *)((*(s32 *)(arg0 + 0x68)) + temp_s1 + 0x2));
        sp9C = (void *)gl_func_00000000(arg0, *(u16 *)((*(s32 *)(arg0 + 0x68)) + temp_s1 + 0x4));
        sp98 = (void *)gl_func_00000000(arg0, *(u16 *)((*(s32 *)(arg0 + 0x68)) + temp_s1 + 0x6));
        var_a0 = *(s32 *)(arg4 + 0x38);
    } else {
        sp98 = NULL;
        sp9C = NULL;
        spA0 = NULL;
    }
    temp_v1 = *(s32 *)(arg0 + 0x7C);
    if (temp_v1 != 0) {
        base = (char *)((*(s32 *)(arg0 + 0x68)) + temp_s1);
        sp84 = *(u8 *)(temp_v1 + *(u16 *)(base + 0x2));
        sp86 = *(u8 *)(temp_v1 + *(u16 *)(base + 0x4));
        sp88 = *(u8 *)(temp_v1 + *(u16 *)(base + 0x6));
    } else {
        sp88 = 0xFF;
        sp86 = 0xFF;
        sp84 = 0xFF;
    }
    if (var_a0 & 0x2000) {
        if (*(s32 *)(arg0 + 0x38) & 0x8000) {
            gl_func_00000000(arg0, *(u16 *)((*(s32 *)(arg0 + 0x68)) + temp_s1 + 0x2), &sp94, arg4);
            gl_func_00000000(arg0, *(u16 *)((*(s32 *)(arg0 + 0x68)) + temp_s1 + 0x4), &sp90);
            gl_func_00000000(arg0, *(u16 *)((*(s32 *)(arg0 + 0x68)) + temp_s1 + 0x6), &sp8C);
        } else {
            if (*(s32 *)(arg0 + 0x54) == 0) {
                gl_func_00000000_49DBCv((char *)&D_00000000 + 0x20014);
            }
            gl_func_00000000_49DBCv((char *)&D_00000000, &sp94, *(s32 *)(arg0 + 0x54) + arg3 * 0xC);
            *(u8 *)&sp8C = *(u8 *)&sp94;
            *(u8 *)&sp90 = *(u8 *)&sp94;
        }
    } else {
        temp_v0_2 = *(s32 *)(arg0 + 0x50);
        if (temp_v0_2 != 0) {
            gl_func_00000000((char *)&D_00000000, &sp94, temp_v0_2 + (*(u16 *)((*(s32 *)(arg0 + 0x68)) + temp_s1 + 0x2)) * 4);
            gl_func_00000000((char *)&D_00000000, &sp90, (*(s32 *)(arg0 + 0x50)) + (*(u16 *)((*(s32 *)(arg0 + 0x68)) + temp_s1 + 0x4)) * 4);
            gl_func_00000000((char *)&D_00000000, &sp8C, (*(s32 *)(arg0 + 0x50)) + (*(u16 *)((*(s32 *)(arg0 + 0x68)) + temp_s1 + 0x6)) * 4);
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
        return (u8 *)gl_func_00000000(arg2, &sp50, (char *)&D_00000000);
    }
    gl_func_00000000(arg2, &sp50, (char *)&D_00000000, (char *)&D_00000000, (*(s32 *)(arg0 + 0x68)) + temp_s1 + 2);
    return NULL;
}
