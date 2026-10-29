/* IDO 7.1 NCC (C++) NON_MATCHING comparison donor for timproc_uso_b5_func_0000C310 (0x400 / 256 words).
 * agent-g run 184 (kit CXXXIII): the colour channels are an inline `Col::set(int r, int g, int b, int a)` that
 * divides each INT parameter by 255.0f -- the int -> float conversion folds after uopt's candidate selection
 * (kit CXIX), which is why the target divides 105 / 155 / 255 / 233 / 247 / 0 by 255 at run time and keeps one
 * shared (f32)255 / 255.0f value across the calls (the cfe body needed `fone = (float)255`, `sp60 = sp60`
 * and a split-name alias); the HUD record is a sprite with the inline `Spr::width()` of the timer-HUD family.
 * The ring / loop half is the cfe body unchanged. Length = target (1024).
 * NM object only. */
typedef float f32; typedef int s32; typedef short s16;
extern "C" {
extern char timproc_uso_b5_D_807FF538, timproc_uso_b5_D_807FFA78, timproc_uso_b5_D_807FFA90, timproc_uso_b5_D_807FFAA8, timproc_uso_b5_D_807FFAA8_b, timproc_uso_b5_D_807FFAD8, timproc_uso_b5_D_807FFAF0;
void timproc_uso_b5_func_00003A4C(int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, char *arg7);
void timproc_uso_b5_func_00003C8C(int arg0, int arg1, int arg2, char *arg3, int arg4, int arg5, int arg6, char *arg7);
void timproc_uso_b5_func_00004068(int a0, int a1, int a2, f32 *a3, int arg5, int arg6);
}
struct Col {
    f32 c[4];
    void set(int r, int g, int b, int a) { c[0] = r / 255.0f; c[1] = g / 255.0f; c[2] = b / 255.0f; c[3] = a / 255.0f; }
};
struct Img { char p[0x20]; s16 w; s16 h; };
struct Spr { char p[0x10]; Img *img; s32 width() { Img *i = img; return i->w + 0 * (int)(i); } /* agent-c run 209: folded counted use of i */ };
extern "C" void timproc_uso_b5_func_0000C310(char *arg0, char *arg1) {
    /* agent-g run 195 (kit CLIII home rules): i first (frame top word), vals / ptrs / hud / cols top-down = the
     * target's 176 / 156 / 152 / 136 homes; the volatile pair moved to the end (dead / volatile homes go last). */
    int i;
    s32 vals[5];
    char *ptrs[5];
    Spr *hud;
    Col cols;
    int m, mv, mv2;
    char *e;
    f32 *fb;
    s32 *pv;
    char **pp;
    volatile s32 sp0, sp1;

    if (*(s32 *)(arg0 + 0x2BC) < 10) {
        if (--*(s32 *)(arg0 + 0x2C0) < 0) {
            *(s32 *)(arg0 + 0x2C0) = 1;
            *(s32 *)(arg0 + 0x2BC) += 2;
        }
    }
    vals[0] = *(s32 *)(arg0 + 0x1C4);
    vals[1] = *(s32 *)(arg0 + 0x1DC);
    vals[2] = *(s32 *)(arg0 + 0x1F4);
    vals[3] = *(s32 *)(arg0 + 0x20C);
    vals[4] = *(s32 *)(arg0 + 0x224);
    ptrs[1] = arg1 + 0x158;
    ptrs[0] = arg1 + 0x134;
    ptrs[2] = arg1 + 0x17C;
    ptrs[3] = arg1 + 0x1A0;
    ptrs[4] = arg1 + 0x1C4;
    fb = (f32 *)(&timproc_uso_b5_D_807FF538 + 0xB8);
    i = 0;
    pv = vals;
    pp = ptrs;
    do {
        m = *(s32 *)(arg0 + 0x2BC);
        if (m < *pv) {
            mv = m;
        } else {
            mv = *pv; if (arg0) {}
        }
        mv2 = mv;
        if (*(s32 *)(arg1 + 0x3BC) - 1 == 0) {
            timproc_uso_b5_func_00003A4C((int)arg0, *(s32 *)(arg0 + 0xA4), *(s32 *)(arg0 + 0xBC) + i * *(s32 *)(arg1 + 0x74), (int)fb, mv, (s32)*pp, (s32)(arg1 + 0x230), arg1);
        } else {
            timproc_uso_b5_func_00003C8C((int)arg0, *(s32 *)(arg0 + 0xA4), *(s32 *)(arg0 + 0xBC) + i * *(s32 *)(arg1 + 0x74), (char *)fb, mv2, (s32)*pp, (s32)(arg1 + 0x230), arg1);
        }
        i++;
        pv++;
        fb += 6;
        pp++;
    } while (i != 5);
    e = *(char **)(arg0 + 0x2B8);
    if (*(s32 *)(e + 0x130) == 0) {
        if (0.0f == *(f32 *)(e + 0x10C)) {
            if (!(*(s32 *)(arg0 + 0x2B4) & 0x20)) {
                cols.set(105, 255, 155, 255);
                timproc_uso_b5_func_00004068((int)arg0, *(s32 *)(arg0 + 0xD4), *(s32 *)(arg0 + 0xEC), cols.c, (int)(&timproc_uso_b5_D_807FFAD8 + 0x658), 0xFF);
                hud = (Spr *)(&timproc_uso_b5_D_807FFA78 + 0x5F8);
            } else {
                cols.set(105, 155, 255, 255);
                timproc_uso_b5_func_00004068((int)arg0, *(s32 *)(arg0 + 0xD4), *(s32 *)(arg0 + 0xEC), cols.c, (int)(&timproc_uso_b5_D_807FFAF0 + 0x670), 0xFF);
                hud = (Spr *)(&timproc_uso_b5_D_807FFA90 + 0x610);
            }
            timproc_uso_b5_func_00004068((int)arg0, *(s32 *)(arg0 + 0xD4), *(s32 *)(arg0 + 0xEC), cols.c, (int)hud, 0xFF);
            if ((++*(s32 *)(arg0 + 0x2C4)) & 8) {
                cols.set(233, 247, 0, 255);
                if (!(*(s32 *)(arg0 + 0x2B4) & 0x20)) {
                    timproc_uso_b5_func_00004068((int)arg0, *(s32 *)(arg0 + 0xD4) - hud->width() / 2 - 2, *(s32 *)(arg0 + 0xEC), cols.c, (int)(&timproc_uso_b5_D_807FFAA8 + 0x628), 0xFF);
                } else {
                    timproc_uso_b5_func_00004068((int)arg0, *(s32 *)(arg0 + 0xD4) + hud->width() / 2 + 2, *(s32 *)(arg0 + 0xEC), cols.c, (int)(&timproc_uso_b5_D_807FFAA8_b + 0x640), 0xFF);
                }
            }
        }
    }
}
