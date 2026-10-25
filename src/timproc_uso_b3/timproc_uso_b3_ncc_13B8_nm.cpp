/* IDO 7.1 NCC (C++) NON_MATCHING comparison donor for timproc_uso_b3_func_000013B8 (0x2AC / 171 words):
 * timer HUD draw (twin of arcproc_uso_func_000016F4 / timproc_uso_b1_func_00001340, same family as
 * mgrproc 23FC). agent-g run 172 (kit CXVII): the cfe NM body as plain C++ -- the digit loop as
 * `for (i = 0; i < n; i++)` over buf[i] with `x + i * 13` (the 23FC loop), named panel pointers, frame
 * 328 = target from six function-level volatile scalars after the named locals (NCC decl-order homes).
 * 162 of 171 words; left: arg2 / the panel pointers / the buf address take different s-registers
 * (the target keeps both panel pointers in s1 / s2 until strlen and then spills them; here they are
 * memory-homed at once), and the x-offset arithmetic's register juggling (the target uses ra as a temp).
 * NM object only. */
typedef float f32; typedef int s32; typedef unsigned char u8; typedef short s16;
#define FW(p, o) (*(int *)((char *)(p) + (o)))
extern "C" {
extern char D_b3_1240_base;
int timproc_uso_b3_func_000000B0(...);
extern "C" void timproc_uso_b3_func_000013B8(char *arg0, s32 arg1, f32 *arg2) {
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
    s32 y;
    volatile s32 q0, q1, q2, q3, q4, q5;

    one[0] = 1.0f;
    one[1] = 1.0f;
    one[2] = 1.0f;
    one[3] = 1.0f;
    if (FW(FW(arg0, 0x528), 0x14) & 2) {
        timproc_uso_b3_func_000000B0(&D_b3_1240_base, (s32) (255.0f * *(f32 *)(arg0 + 0x72C)), arg0 + 0x2A8, arg0 + 0x2CC);
        tim = arg0 + 0x714;
        timproc_uso_b3_func_000000B0(tim);
        pA = arg0 + 0x6CC;
        w = *(s16 *)((char *)FW(arg0, 0x724) + 0x20) / 12;
        timproc_uso_b3_func_000000B0(pA);
        pB = arg0 + 0x6B4;
        timproc_uso_b3_func_000000B0(pB);
        pC = arg0 + 0x6FC;
        timproc_uso_b3_func_000000B0(pC);
        timproc_uso_b3_func_000000B0(buf, &D_b3_1240_base + 0x3D8, arg2);
        n = timproc_uso_b3_func_000000B0(buf);
        y = arg1 + 0x32;
        xoff = 0xA0 - (*(s16 *)((char *)FW(arg0, 0x70C) + 0x20) + n * 13 + *(s16 *)((char *)FW(arg0, 0x6DC) + 0x20) + *(s16 *)((char *)FW(arg0, 0x6C4) + 0x20) + 0x10) / 2;
        xb = *(s16 *)((char *)FW(arg0, 0x6C4) + 0x20) + xoff + 4;
        xa = *(s16 *)((char *)FW(arg0, 0x6DC) + 0x20) + xb + 4;
        x = *(s16 *)((char *)FW(arg0, 0x70C) + 0x20) + xa + 8;
        timproc_uso_b3_func_000000B0(pC, xa, y, 2);
        for (i = 0; i < n; i++) {
            buf[i] -= 0x30;
            timproc_uso_b3_func_000000B0(tim, x + i * 13, arg1 - *(s16 *)((char *)FW(arg0, 0x724) + 0x22) / 2 + 0x32, buf[i] * w, w);
        }
        timproc_uso_b3_func_000000B0(&D_b3_1240_base, (s32) (255.0f * *(f32 *)(arg0 + 0x72C)), one);
        timproc_uso_b3_func_000000B0(pA, xb, y, 2);
        timproc_uso_b3_func_000000B0(pB, xoff, y, 2);
    }
}
}
