/* IDO 7.1 NCC (C++) donor for timproc_uso_b5_func_0000C310 (0x400 / 256 words), EXACT (agent-g run 246),
 * spliced over the empty placeholder in timproc_uso_b5.c (both trees) via REPLACE_FUNC_BODY.
 * agent-g run 184 (kit CXXXIII): the colour channels are an inline `Col::set(int r, int g, int b, int a)` that
 * divides each INT parameter by 255.0f -- the int -> float conversion folds after uopt's candidate selection
 * (kit CXIX), which is why the target divides 105 / 155 / 255 / 233 / 247 / 0 by 255 at run time and keeps one
 * shared (f32)255 / 255.0f value across the calls (the cfe body needed `fone = (float)255`, `sp60 = sp60`
 * and a split-name alias); the HUD record is a sprite with the inline `Spr::width()` of the timer-HUD family.
 * The ring / loop half is the cfe body unchanged. Length = target (1024).
 * agent-g run 246 (docs/IDO_CODEGEN.md kit CCLXXI): the last word pair was the preheader order of the
 * arg1 + 0x230 hoist vs the ptrs IV -- assigning the loop-invariant `q = arg1 + 0x230` INSIDE the loop
 * (uopt hoists the assignment into the preheader between the IV inits, as the target) and passing q. */
typedef float f32; typedef int s32; typedef short s16;
extern "C" {
extern char timproc_uso_b5_D_807FF538, timproc_uso_b5_D_807FFA78, timproc_uso_b5_D_807FFA90, timproc_uso_b5_D_807FFAA8, timproc_uso_b5_D_807FFAD8, timproc_uso_b5_D_807FFAF0;
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
    int mv, mv2;
    char *q; /* run 246: assigned inside the loop (hoisted by uopt between the IV inits) */
    char *e;
    volatile s32 sp0, sp1; /* agent-c run 218 / run 246: q's home replaces the third word (frame 200) */
    /* agent-g run 246: fb / pv / pp as uopt IVs (fb0 + i * 6, vals[i], ptrs[i]: no edg home, one ugen temp word
     * each below the spill, kit CCLV) and m de-named (the per-iteration read written in place: one more temp word,
     * kit CCXVI) -> the 255 / 255.0f spill at sp+96 as the target (99.89 -> 99.91). */
#define fb0 ((f32 *)(&timproc_uso_b5_D_807FF538 + 0xB8))
#define m (*(s32 *)(arg0 + 0x2BC))

    if (*(s32 *)(arg0 + 0x2BC) < 10) {
        *(s32 *)(arg0 + 0x2C0) -= 1; /* run 218: field form (load / adjust in ugen temps t7 / t8) */
        if (*(s32 *)(arg0 + 0x2C0) < 0) {
            *(s32 *)(arg0 + 0x2BC) += 2;
            *(s32 *)(arg0 + 0x2C0) = 1; /* run 218: the constant-1 LR (t0) shared with the loop's == 1 */
        }
    }
    vals[0] = *(s32 *)(arg0 + 0x1C4);
    vals[1] = *(s32 *)(arg0 + 0x1DC);
    vals[2] = *(s32 *)(arg0 + 0x1F4);
    vals[3] = *(s32 *)(arg0 + 0x20C);
    vals[4] = *(s32 *)(arg0 + 0x224);
    ptrs[0] = arg1 + 0x134;
    ptrs[1] = arg1 + 0x158;
    ptrs[2] = arg1 + 0x17C;
    ptrs[3] = arg1 + 0x1A0;
    ptrs[4] = arg1 + 0x1C4;
    for (i = 0; i != 5; i++) {
        if (m) {} /* run 218: m v0 / selection v1 / copy v0 as the target */
        if (m < vals[i]) {
            mv2 = m;
        } else {
            mv2 = vals[i];
        }
        mv = mv2 + 0 * (int)(*arg0); /* run 218: folded counted use keeps the target's `move v0,v1` copy */
        q = arg1 + 0x230;
        if (*(s32 *)(arg1 + 0x3BC) == 1) { /* run 218: `== 1` = the target's held 1 (t0, rematerialised at the loop top) */
            timproc_uso_b5_func_00003A4C((int)arg0, *(s32 *)(arg0 + 0xA4), *(s32 *)(arg0 + 0xBC) + i * *(s32 *)(arg1 + 0x74), (int)(fb0 + i * 6), mv2, (s32)ptrs[i], (s32)q, arg1);
        } else {
            timproc_uso_b5_func_00003C8C((int)arg0, *(s32 *)(arg0 + 0xA4), *(s32 *)(arg0 + 0xBC) + i * *(s32 *)(arg1 + 0x74), (char *)(fb0 + i * 6), mv, (s32)ptrs[i], (s32)q, arg1);
        }
    }
    e = *(char **)(arg0 + 0x2B8);
    if (*(s32 *)(e + 0x130) == 0) {
        if (*(f32 *)(e + 0x10C) == 0.0f) {
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
            *(s32 *)(arg0 + 0x2C4) += 1;
            if (*(s32 *)(arg0 + 0x2C4) & 8) {
                cols.set(233, 247, 0, 255);
                if (!(*(s32 *)(arg0 + 0x2B4) & 0x20)) {
                    timproc_uso_b5_func_00004068((int)arg0, *(s32 *)(arg0 + 0xD4) - hud->width() / 2 - 2, *(s32 *)(arg0 + 0xEC), cols.c, (int)(&timproc_uso_b5_D_807FFAA8 + 0x628), 0xFF);
                } else {
                    timproc_uso_b5_func_00004068((int)arg0, *(s32 *)(arg0 + 0xD4) + hud->width() / 2 + 2, *(s32 *)(arg0 + 0xEC), cols.c, (int)(&timproc_uso_b5_D_807FFAA8 + 0x640), 0xFF);
                }
            }
        }
    }
}
