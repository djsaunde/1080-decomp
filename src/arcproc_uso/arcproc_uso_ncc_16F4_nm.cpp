/* IDO 7.1 NCC (C++) NON_MATCHING comparison donor for arcproc_uso_func_000016F4 (0x2AC / 171 words):
 * timer HUD draw, the opcode-identical twin of timproc_uso_b3_func_000013B8 (offsets +0x50 / +0x38,
 * flag 4): timproc_uso_b3_ncc_13B8_nm.cpp with the field offsets mapped (agent-g run 172, kit CXVII).
 * NM object only. */
typedef float f32; typedef int s32; typedef unsigned char u8; typedef short s16;
#define FW(p, o) (*(int *)((char *)(p) + (o)))
extern "C" {
extern char D_00000000;
int arcproc_uso_func_00001B88(...);
extern "C" void arcproc_uso_func_000016F4(char *arg0, s32 arg1, f32 *arg2) {
    f32 one[4];
    u8 buf[0x80];
    s32 x;
    char *tim;
    char *pA;
    char *pB;
    char *pC;
    s32 w;
    s32 n;
    s32 i;
    s32 xoff;
    s32 xb;
    s32 xa;
    volatile s32 qy;
    volatile s32 q0, q1, q2, q3, q4, q5;

    one[0] = 1.0f;
    one[1] = 1.0f;
    one[2] = 1.0f;
    one[3] = 1.0f;
    if (FW(FW(arg0, 0x528), 0x14) & 4) {
        arcproc_uso_func_00001B88(&D_00000000, (s32) (255.0f * *(f32 *)(arg0 + 0x77C)), arg0 + 0x2A8, arg0 + 0x2CC);
        tim = arg0 + 0x74C;
        arcproc_uso_func_00001B88(tim);
        pA = arg0 + 0x704;
        w = *(s16 *)((char *)FW(arg0, 0x75C) + 0x20) / 12;
        arcproc_uso_func_00001B88(pA);
        pB = arg0 + 0x71C;
        arcproc_uso_func_00001B88(pB);
        pC = arg0 + 0x734;
        arcproc_uso_func_00001B88(pC);
        arcproc_uso_func_00001B88(buf, &D_00000000 + 0x3D8, arg2);
        n = arcproc_uso_func_00001B88(buf);
        xoff = 0xA0 - (*(s16 *)((char *)FW(arg0, 0x744) + 0x20) + n * 13 + *(s16 *)((char *)FW(arg0, 0x714) + 0x20) + *(s16 *)((char *)FW(arg0, 0x72C) + 0x20) + 0x10) / 2;
        xb = *(s16 *)((char *)FW(arg0, 0x72C) + 0x20) + xoff + 4;
        xa = *(s16 *)((char *)FW(arg0, 0x714) + 0x20) + xb + 4;
        x = *(s16 *)((char *)FW(arg0, 0x744) + 0x20) + xa + 8;
        arcproc_uso_func_00001B88(pC, xa, arg1 + 0x32, 2);
        for (i = 0; i < n; i++) {
            buf[i] -= 0x30;
            arcproc_uso_func_00001B88(tim, x + i * 13, arg1 - *(s16 *)((char *)FW(arg0, 0x75C) + 0x22) / 2 + 0x32, buf[i] * w, w);
        }
        arcproc_uso_func_00001B88(&D_00000000, (s32) (255.0f * *(f32 *)(arg0 + 0x77C)), one);
        arcproc_uso_func_00001B88(pA, xb, arg1 + 0x32, 2);
        arcproc_uso_func_00001B88(pB, xoff, arg1 + 0x32, 2);
    }
}
}
