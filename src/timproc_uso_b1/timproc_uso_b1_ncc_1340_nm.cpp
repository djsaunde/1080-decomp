/* IDO 7.1 NCC (C++) NON_MATCHING comparison donor for timproc_uso_b1_func_00001340 (0x3B8 / 238 words):
 * timer HUD draw with the mm:ss.cc split (the 23FC / timproc_b3 13B8 / arcproc 16F4 family). agent-g
 * run 172 (kit CXVII): the cfe NM body as plain C++ without the saved-copy / dead-kill / volatile
 * glyph-pointer emulation; frame 336 = target from six volatile scalars after the named locals.
 * 228 of 238 words; same residual class as 13B8 (panel pointers memory-homed at once, s-register
 * rotation). NM object only. */
typedef float f32; typedef int s32; typedef short s16;
#define FW(p, o) (*(int *)((char *)(p) + (o)))
extern "C" {
extern char D_00000000;
int timproc_uso_b1_func_000000B0(...);
extern "C" void timproc_uso_b1_func_00001340(char *arg0, s32 arg1, s32 arg2) {
    f32 one[4];
    char buf[128];
    char *p714;
    char *w6CC;
    char *w6B4;
    char *w6FC;
    s32 min, sec;
    s32 glyph;
    s32 x, pos4, posA1, pos6;
    s32 xmid;
    s32 i;
    volatile s32 qc;
    s32 count;
    volatile s32 q0, q1, q2, q3, q4, q5;

    one[0] = 1.0f;
    one[1] = 1.0f;
    one[2] = 1.0f;
    one[3] = 1.0f;
    if (FW(FW(arg0, 0x528), 0x14) & 1) {
        min = arg2 / 60000;
        arg2 -= min * 60000;
        sec = arg2 / 1000;
        arg2 -= sec * 1000;
        arg2 = arg2 / 10;
        timproc_uso_b1_func_000000B0(&D_00000000, (s32)(255.0f * *(f32 *)(arg0 + 0x72C)), arg0 + 0x2A8, arg0 + 0x2CC);
        p714 = arg0 + 0x714;
        timproc_uso_b1_func_000000B0(p714);
        glyph = *(s16 *)(*(char **)(arg0 + 0x724) + 0x20) / 12;
        w6CC = arg0 + 0x6CC;
        timproc_uso_b1_func_000000B0(w6CC);
        w6B4 = arg0 + 0x6B4;
        timproc_uso_b1_func_000000B0(w6B4);
        w6FC = arg0 + 0x6FC;
        timproc_uso_b1_func_000000B0(w6FC);
        timproc_uso_b1_func_000000B0(buf, &D_00000000 + 0x3F4, min, sec, arg2);
        count = timproc_uso_b1_func_000000B0(buf);
        x = 160 - (*(s16 *)(*(char **)(arg0 + 0x70C) + 0x20) + count * 13 + *(s16 *)(*(char **)(arg0 + 0x6DC) + 0x20) + *(s16 *)(*(char **)(arg0 + 0x6C4) + 0x20) + 16) / 2;
        pos4 = *(s16 *)(*(char **)(arg0 + 0x6C4) + 0x20) + x + 4;
        posA1 = *(s16 *)(*(char **)(arg0 + 0x6DC) + 0x20) + pos4 + 4; if (1) {} /* run 182 (lever-sweep bbsplit) */
        pos6 = *(s16 *)(*(char **)(arg0 + 0x70C) + 0x20) + posA1 + 8;
        xmid = arg1 + 0x32;
        timproc_uso_b1_func_000000B0(w6FC, posA1, xmid, 2);
        for (i = 0; i < count; i++) {
            if (buf[i] == 0x5B) {
                buf[i] = 0xA;
            } else if (buf[i] == 0x5D) {
                buf[i] = 0xB;
            } else {
                buf[i] = buf[i] - 0x30;
            }
            timproc_uso_b1_func_000000B0(p714, pos6 + i * 13, (arg1 - *(s16 *)(*(char **)(arg0 + 0x724) + 0x22) / 2) + 0x32, buf[i] * glyph, glyph);
        }
        timproc_uso_b1_func_000000B0(w6CC, (s32)(255.0f * *(f32 *)(arg0 + 0x72C)), one, 0xFF);
        timproc_uso_b1_func_000000B0(w6CC, pos4, xmid, 2);
        timproc_uso_b1_func_000000B0(w6B4, (s32)(255.0f * *(f32 *)(arg0 + 0x72C)), one, 0xFF);
        timproc_uso_b1_func_000000B0(w6B4, x, xmid, 2);
    }
}
}
