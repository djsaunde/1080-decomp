/* IDO 7.1 NCC (C++) NM-only comparison donor for gl_func_0001EF20: the NM C body lifted under NCC (agent-c run 134 sweep, cfe 71.35).
 * Still NON_MATCHING: spliced into build/non_matching/ only via REPLACE_FUNC_BODY. */
typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef unsigned short u16;
typedef int s32; typedef unsigned int u32; typedef long long s64; typedef unsigned long long u64;
typedef float f32; typedef double f64;
#undef NULL
#define NULL 0
typedef struct { unsigned int w0, w1; } G1D9E0;
extern "C" {
extern G1D9E0 *gfx;
extern char *gl_1dcb4_f5(char *, s32, s32);
extern char *gl_1dcb4_f4(char *, s16, s16);
extern int gl_ref_00031BC8(...);
extern u16 arg4;
extern int arg5;
extern int gl_ref_00031D00(...);
extern int gl_ref_00031D8C(...);
}
extern "C" char *gl_func_0001EF20(char *arg0, char *arg1, char *arg2, s32 arg3, u16 arg4, s32 arg5) {
    char *dst;
    char *tmp;
    s32 x0;
    s32 x1;
    u16 key;
    u16 rk;
    u8 mode;
    s16 stepA;
    s16 stepB;
    s16 dm;
    s16 t4v;
    s32 m;
    s32 keyCopy;
    s32 raCopy;
    s32 n;
    s32 modeM;
    s32 v1w;
    s32 gfx;

    dst = arg0;
    x0 = (*(u16 *)(arg1 + 0x8) << 4) & 0xFFFF;
    x1 = (*(u16 *)(arg1 + 0xA) << 4) & 0xFFFF;
    key = *(u16 *)(arg2 + 0x10);
    mode = *(u8 *)(arg1 + 0x5);
    rk = *(u16 *)(arg2 + 0x12);
    keyCopy = key;
    if (key != x0) {
        n = arg3 >> 3;
        stepA = (x0 - keyCopy) / n;
    } else {
        stepA = 0;
        n = arg3 >> 3;
    }
    raCopy = rk;
    if (rk != x1) {
        stepB = (x1 - raCopy) / n;
    } else {
        stepB = 0;
    }
    t4v = *(u8 *)(arg2 + 0x4);
    modeM = mode & 0x7F;
    if (t4v != mode) {
        m = t4v & 0x7F;
        dm = ((modeM - m) << 9) / n;
        *(u8 *)(arg2 + 0x4) = mode;
    } else {
        dm = 0;
        m = t4v & 0x7F;
    }
    *(u16 *)(arg2 + 0x10) = keyCopy + (stepA * n);
    *(u16 *)(arg2 + 0x12) = raCopy + (stepB * n);
    if (*(u16 *)(arg1 + 0x0) & 1) {
        tmp = dst;
        dst += 8;
        gl_ref_00031BC8(tmp, 0x5C0, 0x1A0, stepA);
        tmp = dst;
        dst += 8;
        gl_ref_00031D00(tmp, m * 2, dm, stepA, stepB);
        tmp = dst;
        dst += 8;
        gl_ref_00031D8C(tmp, keyCopy, raCopy);
        switch (arg5) {
        case 1:
            gfx = *(s32 *)0x1A2D4;
            break;
        case 2:
            gfx = *(s32 *)0x1A2D8;
            break;
        default:
            gfx = *(s32 *)0x1A2DC;
            break;
        }
    } else {
        *(s32 *)(dst + 0x0) = (((m * 2) & 0xFF) << 0x10) | 0x12000000 | (dm & 0xFFFF);
        *(s32 *)(dst + 0x4) = (stepA << 0x10) | (stepB & 0xFFFF);
        dst += 8;
        *(s32 *)(dst + 0x0) = 0x16000000;
        *(s32 *)(dst + 0x4) = (key << 0x10) | (rk & 0xFFFF);
        dst += 8;
        gfx = *(s32 *)0x1A2DC;
    }
    v1w = *(s32 *)(arg1 + 0x0);
    *(s32 *)(dst + 0x4) = gfx;
    *(s32 *)(dst + 0x0) = ((((s32) arg4 >> 4) & 0xFF) << 0x10) | *(s32 *)0x1A2D0 | ((arg3 & 0xFF) << 8)
        | ((((t4v & 0x80) >> 7) & 1) << 4)
        | ((((u32) (v1w << 6) >> 0x1F) & 1) << 3)
        | ((*(u8 *)(arg1 + 0x0) & 1) << 2)
        | ((((u32) (v1w << 4) >> 0x1F) & 1) << 1)
        | (((u32) (v1w << 5) >> 0x1F) & 1);
    return dst + 8;
}
