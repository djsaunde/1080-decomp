/* IDO 7.1 NCC (C++) NM donor for gl_func_000578B4 (texture loader: gbi LoadTextureTile /
 * LoadBlock per format, CI8 with a TLUT). Spliced into the NON_MATCHING game_libs_post0b.c.o
 * only (REPLACE_FUNC_BODY); the cfe body in game_libs_post0b.c is the m2c transcription.
 * agent-c run 154: the cfe body ported to NCC (cfe 98.27 -> NCC 99.93):
 * - edg evaluates a |-chain in SOURCE order, so every packet word is the gbi macro's own
 *   operand order (SetTile w1 flat `tile | cmt | maskt | shiftt | cms | masks | shifts`,
 *   TILESIZE w1 `tile | lrs<<12 | lrt`, LoadTile `uls<<12 | cmd | ult`, `w * h` = 0x20 * 0x22);
 *   this fixed every commutative-operand residual cfe could not reach.
 * - NCC drops never-referenced locals but gives every referenced one a frame home, so the
 *   m2c pad locals are referenced by one dead `= 0` line (frame 0x1F0 = target).
 * - switch 1 is `switch (FW(arg1, 0x24))` (edg source-order compare chain, t1 temp).
 * - the two s2_508 single-store packets share one pointer variable (temp_t0_25), which
 *   fixes the s1_508 / s2_508 t0/t1 webs (temp_t1_7 is shared by both arms).
 * - one statement per gbi line matters under NCC: `temp_a0_6 = ...; temp_t0_27 = ...` on
 *   one line keeps the target's 0x10-then-0xC load order.
 * - the s1_508 LOADTILE SetTile w0 reads the width through temp_a0_6 (`(w + h) - w`, like
 *   the other arms): the target then holds 0x07000000 in t1 instead of 0xF5480000.
 * agent-c run 236: the s2_508 FD50 DL pointer and the tmem value `arg2 & 0x1FF` are ONE
 *   variable (sp50, s32). The tmem def carries a folded read (`+ 0 * FW(arg1, 0x0)`) so uopt
 *   does not copy-propagate it: the variable's live range then reaches the later t0 users and
 *   gets ra. `if (1) { FD50 store; }` puts the def and the store in separate units (adjsave
 *   7/5 = 1.4, below the 1.5 of the t2..t5 shift temps, so it colours after them).
 * Remaining (2 words): the target loads arg2 into a scratch (`lw t0; andi ra,t0`); ours loads
 *   it straight into ra. */
typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef unsigned short u16;
typedef int s32; typedef unsigned int u32; typedef float f32;
#define FW(p, o) (*(int *)((char *)(p) + (o)))
extern "C" {
int gl_func_00034458(...);
int game_libs_func_00062F08_578B4(...);
extern char gl_d_578b4_s1[];
extern char gl_d_578b4_s2a[];
extern char gl_d_578b4_s2b[];
}
extern "C"
void gl_func_000578B4(char *arg0, char *arg1, s32 arg2) {
    s32 sp1EC;
    s32 sp1E8;
    s32 sp1E4;
    s32 sp1E0;
    int * sp1DC;
    volatile s32 hybrid_pad_0;
    volatile s32 hybrid_pad_1;
    s32 g;
    s32 n;
    s32 p;
    char * sp1C4;
    s32 w;
    s32 v;
    int * temp_a2;
    int * temp_t1_7;
    s32 * temp_t0_20;
    char * temp_t1_3;
    char * sp1A8;
    s32 * temp_t0_22;
    s32 * temp_t0_23;
    s32 * temp_t0_24;
    s32 * temp_t1_9;
    s32 * temp_t8;
    s32 temp_a0_2;
    char * sp18C;
    s32 temp_a0_4;
    s32 temp_a0_5;
    s32 temp_a0_6;
    s32 temp_a1;
    s32 temp_a1_2;
    s32 temp_a1_3;
    char * sp170;
    s32 temp_a1_4;
    s32 temp_a1_5;
    s32 temp_a1_6;
    s32 temp_a1_7;
    s32 temp_a1_8;
    s32 temp_a1_9;
    char * sp154;
    s32 temp_ra_2;
    s32 temp_ra_3;
    s32 temp_ra_4;
    s32 temp_ra_5;
    s32 temp_ra_6;
    s32 temp_ra_8;
    s32 temp_t0_27;
    s32 temp_t1;
    s32 temp_t2;
    s32 temp_t3;
    s32 temp_t4;
    s32 temp_t5;
    s32 temp_v0_3;
    s32 temp_v0_6;
    char * sp118;
    s32 var_a2;
    s32 var_t0;
    s32 pad_100;
    s32 pad_104;
    s32 pad_108;
    s32 pad_10c;
    int * spFC;
    char * temp_a0;
    s32 pad_f4;
    s32 * spF0;
    char * temp_a0_3;
    char * temp_ra;
    char * temp_ra_7;
    int * spE0;
    char * temp_t0;
    char * temp_t0_10;
    s32 * spD4;
    char * temp_t0_11;
    char * temp_t0_12;
    char * temp_t0_13;
    int * spC4;
    char * temp_t0_14;
    char * temp_t0_15;
    s32 * spB8;
    char * temp_t0_16;
    char * temp_t0_17;
    char * temp_t0_18;
    int * spA8;
    char * temp_t0_19;
    char * temp_t0_21;
    s32 * sp9C;
    char * temp_t0_25;
    char * temp_t0_26;
    char * temp_t0_2;
    int * sp8C;
    char * temp_t0_3;
    char * temp_t0_4;
    s32 * sp80;
    char * temp_t0_5;
    char * temp_t0_6;
    char * temp_t0_7;
    char * temp_t0_8;
    char * temp_t0_9;
    char * temp_t1_2;
    char * temp_v0;
    char * temp_v0_2;
    char * temp_v0_4;
    char * temp_v1;
    char * pad_5c;
    s32 sp50;
    char * temp_v1_2;
    char * temp_v1_3;
    s32 * sp44;
    char * temp_v1_4;
    char * temp_v1_5;
    char * temp_v1_6;
    s32 sp34;
    char * temp_v1_7;
    char * temp_v1_8;
    volatile s32 hybrid_pad_2;
    volatile s32 hybrid_pad_3;
    volatile s32 sp20;
    p = 0; temp_a2 = 0; temp_t0_20 = 0; temp_t1_3 = 0; temp_t0_22 = 0; temp_t0_23 = 0; temp_t0_24 = 0; temp_t1_9 = 0; temp_t8 = 0; temp_a0_2 = 0; temp_a0_4 = 0; temp_a0_5 = 0; temp_a1_2 = 0; temp_a1_3 = 0; temp_a1_4 = 0; temp_a1_5 = 0; temp_a1_6 = 0; temp_a1_7 = 0; temp_a1_8 = 0; temp_v0_3 = 0; pad_100 = 0; pad_104 = 0; pad_108 = 0; pad_10c = 0; pad_f4 = 0; temp_a0_3 = 0; temp_ra = 0; temp_ra_7 = 0; temp_t0 = 0; temp_t0_2 = 0; temp_t0_3 = 0; temp_t1_2 = 0; temp_v0 = 0; temp_v0_2 = 0; pad_5c = 0; temp_v1_2 = 0; temp_v1_3 = 0; temp_v1_4 = 0; temp_v1_5 = 0; temp_v1_6 = 0; temp_v1_7 = 0; temp_v1_8 = 0; temp_t0_26 = 0; temp_ra_8 = 0;

    sp1EC = game_libs_func_00062F08_578B4((*(s16 *)((char *)arg1 + 0x20)));
    sp1E8 = game_libs_func_00062F08_578B4((*(s16 *)((char *)arg1 + 0x22)));
    sp1E0 = 0;
    sp1E4 = 0;
    temp_v1 = (char *)( FW(arg0, 0xC));
    temp_a1 = FW(temp_v1, 0x4);
    FW(temp_v1, 0x4) = (s32) (temp_a1 + 1);
    temp_a0 = (char *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1 * 8));
    FW(temp_a0, 0x0) = 0xBA000E02; FW(temp_a0, 0x4) = 0;
    var_a2 = FW(arg1, 0x28);
    if (!(var_a2 & 0x100)) {
        if (var_a2 & 0x200) {
            sp1E4 = 2;
        } else if (var_a2 & 0x400) {
            sp1E4 = 1;
        }
    }
    if (!(var_a2 & 0x1000)) {
        if (var_a2 & 0x2000) {
            sp1E0 = 2;
        } else if (var_a2 & 0x4000) {
            sp1E0 = 1;
        }
    }
    if (var_a2 & 0x10) {

        sp1DC = (int *) FW(arg1, 0x1C);
        /* edg-style source-order compare chain (switch 1) */
        switch (FW(arg1, 0x24)) { case 0x120: goto s1_120; case 0x110: goto s1_110; case 0x408: goto s1_408; case 0x308: goto s1_308; case 0x304: goto s1_304; case 0x508: goto s1_508; default: goto s1_def; }
        {
        s1_120:
            g = FW(arg0, 0xC);
            n = FW(g, 0x4);
            FW(g, 0x4) = n + 1;
            sp1C4 = (char *) (FW(FW(arg0, 0xC), 0x0) + (n * 8));
            FW(sp1C4, 0x0) = ((FW(sp1DC, 0xC) - 1) & 0xFFF) | 0xFD180000; FW(sp1C4, 0x4) = gl_func_00034458(FW(sp1DC, 0x0), n, sp1DC, arg0);
            g = FW(arg0, 0xC);
            n = FW(g, 0x4);
            FW(g, 0x4) = n + 1;
            w = FW(sp1DC, 0x4);
            temp_t0_4 = (char *)( FW(FW(arg0, 0xC), 0x0) + (n * 8));
            FW(temp_t0_4, 0x0) = (((s32) ((((w + (*(s16 *)((char *)arg1 + 0x20))) - w) * 2) + 7) >> 3) & 0x1FF) << 9 | 0xF5180000;
            temp_t2 = (sp1E0 & 3) << 0x12;
            temp_t3 = (sp1E8 & 0xF) << 0xE;
            temp_t4 = (sp1E4 & 3) << 8;
            temp_t5 = (sp1EC & 0xF) * 0x10;
            FW(temp_t0_4, 0x4) = 0x07000000 | temp_t2 | temp_t3 | temp_t4 | temp_t5;
            g = FW(arg0, 0xC);
            n = FW(g, 0x4);
            FW(g, 0x4) = n + 1;
            temp_v0_4 = (char *)( FW(FW(arg0, 0xC), 0x0) + (n * 8));
            FW(temp_v0_4, 0x0) = 0xE6000000; FW(temp_v0_4, 0x4) = 0;
            g = FW(arg0, 0xC);
            n = FW(g, 0x4);
            FW(g, 0x4) = n + 1;
            temp_a0 = (char *)( FW(FW(arg0, 0xC), 0x0) + (n * 8));
            FW(temp_a0, 0x0) = ((((FW(sp1DC, 0x4) * 4) & 0xFFF) << 0xC) | 0xF4000000 | ((FW(sp1DC, 0x8) * 4) & 0xFFF)); FW(temp_a0, 0x4) = (0x07000000 | (((((FW(sp1DC, 0x4) + (*(s16 *)((char *)arg1 + 0x20))) - 1) << 2) & 0xFFF) << 0xC) | ((((FW(sp1DC, 0x8) + (*(s16 *)((char *)arg1 + 0x22))) - 1) << 2) & 0xFFF));
            g = FW(arg0, 0xC);
            n = FW(g, 0x4);
            FW(g, 0x4) = n + 1;
            temp_v0_4 = (char *)( FW(FW(arg0, 0xC), 0x0) + (n * 8));
            FW(temp_v0_4, 0x0) = 0xE7000000; FW(temp_v0_4, 0x4) = 0;
            g = FW(arg0, 0xC);
            n = FW(g, 0x4);
            FW(g, 0x4) = n + 1;
            w = FW(sp1DC, 0x4);
            temp_t0_5 = (char *)( FW(FW(arg0, 0xC), 0x0) + (n * 8));
            FW(temp_t0_5, 0x0) = (((s32) ((((FW(sp1DC, 0x4) + (*(s16 *)((char *)arg1 + 0x20))) - FW(sp1DC, 0x4)) * 2) + 7) >> 3) & 0x1FF) << 9 | 0xF5180000; FW(temp_t0_5, 0x4) = temp_t2 | temp_t3 | temp_t4 | temp_t5;
            g = FW(arg0, 0xC);
            n = FW(g, 0x4);
            FW(g, 0x4) = n + 1;
            temp_t0_6 = (char *)( FW(FW(arg0, 0xC), 0x0) + (n * 8));
            FW(temp_t0_6, 0x0) = 0xF2000000;
            w = FW(sp1DC, 0x4);
            v = FW(sp1DC, 0x8);
            FW(temp_t0_6, 0x4) = (((((((FW(sp1DC, 0x4) + (*(s16 *)((char *)arg1 + 0x20))) - FW(sp1DC, 0x4)) - 1) << 2) & 0xFFF) << 0xC) | (((((v + (*(s16 *)((char *)arg1 + 0x22))) - v) - 1) << 2) & 0xFFF));
            return;
        s1_110:                                 /* switch 1 */
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1_9 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1_9 + 1);
            sp1A8 = (char *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1_9 * 8));
            FW(sp1A8, 0x0) = (s32) (((FW(sp1DC, 0xC) - 1) & 0xFFF) | 0xFD100000);
            FW(sp1A8, 0x4) = gl_func_00034458(FW(sp1DC, 0x0), temp_a1_9, sp1DC, arg0);
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1_9 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1_9 + 1);
            temp_a0_6 = FW(sp1DC, 0x4);
            temp_t0_4 = (char *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1_9 * 8));
            FW(temp_t0_4, 0x0) = (s32) (((((s32) ((((FW(sp1DC, 0x4) + (*(s16 *)((char *)arg1 + 0x20))) - FW(sp1DC, 0x4)) * 2) + 7) >> 3) & 0x1FF) << 9) | 0xF5100000);
            temp_t2 = (sp1E0 & 3) << 0x12;
            temp_t3 = (sp1E8 & 0xF) << 0xE;
            temp_t4 = (sp1E4 & 3) << 8;
            temp_t5 = (sp1EC & 0xF) * 0x10;
            FW(temp_t0_4, 0x4) = (s32) (0x07000000 | temp_t2 | temp_t3 | temp_t4 | temp_t5);
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1_9 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1_9 + 1);
            temp_v0_4 = (char *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1_9 * 8));
            FW(temp_v0_4, 0x0) = 0xE6000000; FW(temp_v0_4, 0x4) = 0;
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1_9 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1_9 + 1);
            temp_a0 = (char *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1_9 * 8));
            FW(temp_a0, 0x0) = (s32) ((((FW(sp1DC, 0x4) * 4) & 0xFFF) << 0xC) | 0xF4000000 | ((FW(sp1DC, 0x8) * 4) & 0xFFF)); FW(temp_a0, 0x4) = (s32) (0x07000000 | (((((FW(sp1DC, 0x4) + (*(s16 *)((char *)arg1 + 0x20))) - 1) << 2) & 0xFFF) << 0xC) | ((((FW(sp1DC, 0x8) + (*(s16 *)((char *)arg1 + 0x22))) - 1) << 2) & 0xFFF));
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1_9 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1_9 + 1);
            temp_v0_4 = (char *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1_9 * 8));
            FW(temp_v0_4, 0x0) = 0xE7000000; FW(temp_v0_4, 0x4) = 0;
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1_9 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1_9 + 1);
            temp_a0_6 = FW(sp1DC, 0x4);
            temp_t0_5 = (char *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1_9 * 8));
            FW(temp_t0_5, 0x0) = (s32) (((((s32) ((((FW(sp1DC, 0x4) + (*(s16 *)((char *)arg1 + 0x20))) - FW(sp1DC, 0x4)) * 2) + 7) >> 3) & 0x1FF) << 9) | 0xF5100000); FW(temp_t0_5, 0x4) = (s32) (temp_t2 | temp_t3 | temp_t4 | temp_t5);
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1_9 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1_9 + 1);
            temp_t0_6 = (char *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1_9 * 8));
            FW(temp_t0_6, 0x0) = 0xF2000000;
            temp_a0_6 = FW(sp1DC, 0x4);
            temp_v0_6 = FW(sp1DC, 0x8);
            FW(temp_t0_6, 0x4) = (s32) (((((((FW(sp1DC, 0x4) + (*(s16 *)((char *)arg1 + 0x20))) - FW(sp1DC, 0x4)) - 1) << 2) & 0xFFF) << 0xC) | (((((temp_v0_6 + (*(s16 *)((char *)arg1 + 0x22))) - temp_v0_6) - 1) << 2) & 0xFFF));
            return;
        s1_408:                                 /* switch 1 */
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1_9 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1_9 + 1);
            sp18C = (char *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1_9 * 8));
            FW(sp18C, 0x0) = (s32) (((FW(sp1DC, 0xC) - 1) & 0xFFF) | 0xFD680000);
            FW(sp18C, 0x4) = gl_func_00034458(FW(sp1DC, 0x0), temp_a1_9, sp1DC, arg0);
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1_9 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1_9 + 1);
            temp_a0_6 = FW(sp1DC, 0x4);
            temp_t0_7 = (char *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1_9 * 8));
            FW(temp_t0_7, 0x0) = (s32) (((((s32) (((temp_a0_6 + (*(s16 *)((char *)arg1 + 0x20))) - temp_a0_6) + 7) >> 3) & 0x1FF) << 9) | 0xF5680000);
            temp_t2 = (sp1E0 & 3) << 0x12;
            temp_t3 = (sp1E8 & 0xF) << 0xE;
            temp_t4 = (sp1E4 & 3) << 8;
            temp_t5 = (sp1EC & 0xF) * 0x10;
            FW(temp_t0_7, 0x4) = (s32) (0x07000000 | temp_t2 | temp_t3 | temp_t4 | temp_t5);
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1_9 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1_9 + 1);
            temp_v0_4 = (char *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1_9 * 8));
            FW(temp_v0_4, 0x0) = 0xE6000000; FW(temp_v0_4, 0x4) = 0;
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1_9 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1_9 + 1);
            temp_a0 = (char *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1_9 * 8));
            FW(temp_a0, 0x0) = (s32) ((((FW(sp1DC, 0x4) * 4) & 0xFFF) << 0xC) | 0xF4000000 | ((FW(sp1DC, 0x8) * 4) & 0xFFF)); FW(temp_a0, 0x4) = (s32) (0x07000000 | (((((FW(sp1DC, 0x4) + (*(s16 *)((char *)arg1 + 0x20))) - 1) << 2) & 0xFFF) << 0xC) | ((((FW(sp1DC, 0x8) + (*(s16 *)((char *)arg1 + 0x22))) - 1) << 2) & 0xFFF));
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1_9 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1_9 + 1);
            temp_v0_4 = (char *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1_9 * 8));
            FW(temp_v0_4, 0x0) = 0xE7000000; FW(temp_v0_4, 0x4) = 0;
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1_9 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1_9 + 1);
            temp_a0_6 = FW(sp1DC, 0x4);
            temp_t0_8 = (char *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1_9 * 8));
            FW(temp_t0_8, 0x0) = (s32) (((((s32) (((FW(sp1DC, 0x4) + (*(s16 *)((char *)arg1 + 0x20))) - FW(sp1DC, 0x4)) + 7) >> 3) & 0x1FF) << 9) | 0xF5680000); FW(temp_t0_8, 0x4) = (s32) (temp_t2 | temp_t3 | temp_t4 | temp_t5);
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1_9 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1_9 + 1);
            temp_t0_9 = (char *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1_9 * 8));
            FW(temp_t0_9, 0x0) = 0xF2000000;
            temp_a0_6 = FW(sp1DC, 0x4);
            temp_v0_6 = FW(sp1DC, 0x8);
            FW(temp_t0_9, 0x4) = (s32) (((((((FW(sp1DC, 0x4) + (*(s16 *)((char *)arg1 + 0x20))) - FW(sp1DC, 0x4)) - 1) << 2) & 0xFFF) << 0xC) | (((((temp_v0_6 + (*(s16 *)((char *)arg1 + 0x22))) - temp_v0_6) - 1) << 2) & 0xFFF));
            return;
        s1_308:                                 /* switch 1 */
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1_9 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1_9 + 1);
            sp170 = (char *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1_9 * 8));
            FW(sp170, 0x0) = (s32) (((FW(sp1DC, 0xC) - 1) & 0xFFF) | 0xFD880000);
            FW(sp170, 0x4) = gl_func_00034458(FW(sp1DC, 0x0), temp_a1_9, sp1DC, arg0);
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1_9 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1_9 + 1);
            temp_a0_6 = FW(sp1DC, 0x4);
            temp_t0_10 = (char *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1_9 * 8));
            FW(temp_t0_10, 0x0) = (s32) (((((s32) (((FW(sp1DC, 0x4) + (*(s16 *)((char *)arg1 + 0x20))) - FW(sp1DC, 0x4)) + 7) >> 3) & 0x1FF) << 9) | 0xF5880000);
            temp_t2 = (sp1E0 & 3) << 0x12;
            temp_t3 = (sp1E8 & 0xF) << 0xE;
            temp_t4 = (sp1E4 & 3) << 8;
            temp_t5 = (sp1EC & 0xF) * 0x10;
            FW(temp_t0_10, 0x4) = (s32) (0x07000000 | temp_t2 | temp_t3 | temp_t4 | temp_t5);
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1_9 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1_9 + 1);
            temp_v0_4 = (char *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1_9 * 8));
            FW(temp_v0_4, 0x0) = 0xE6000000; FW(temp_v0_4, 0x4) = 0;
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1_9 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1_9 + 1);
            temp_a0 = (char *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1_9 * 8));
            FW(temp_a0, 0x0) = (s32) ((((FW(sp1DC, 0x4) * 4) & 0xFFF) << 0xC) | 0xF4000000 | ((FW(sp1DC, 0x8) * 4) & 0xFFF)); FW(temp_a0, 0x4) = (s32) (0x07000000 | (((((FW(sp1DC, 0x4) + (*(s16 *)((char *)arg1 + 0x20))) - 1) << 2) & 0xFFF) << 0xC) | ((((FW(sp1DC, 0x8) + (*(s16 *)((char *)arg1 + 0x22))) - 1) << 2) & 0xFFF));
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1_9 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1_9 + 1);
            temp_v0_4 = (char *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1_9 * 8));
            FW(temp_v0_4, 0x0) = 0xE7000000; FW(temp_v0_4, 0x4) = 0;
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1_9 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1_9 + 1);
            temp_a0_6 = FW(sp1DC, 0x4);
            temp_t0_11 = (char *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1_9 * 8));
            FW(temp_t0_11, 0x0) = (s32) (((((s32) (((FW(sp1DC, 0x4) + (*(s16 *)((char *)arg1 + 0x20))) - FW(sp1DC, 0x4)) + 7) >> 3) & 0x1FF) << 9) | 0xF5880000); FW(temp_t0_11, 0x4) = (s32) (temp_t2 | temp_t3 | temp_t4 | temp_t5);
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1_9 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1_9 + 1);
            temp_t0_12 = (char *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1_9 * 8));
            FW(temp_t0_12, 0x0) = 0xF2000000;
            temp_a0_6 = FW(sp1DC, 0x4);
            temp_v0_6 = FW(sp1DC, 0x8);
            FW(temp_t0_12, 0x4) = (s32) (((((((FW(sp1DC, 0x4) + (*(s16 *)((char *)arg1 + 0x20))) - FW(sp1DC, 0x4)) - 1) << 2) & 0xFFF) << 0xC) | (((((temp_v0_6 + (*(s16 *)((char *)arg1 + 0x22))) - temp_v0_6) - 1) << 2) & 0xFFF));
            return;
        s1_304:                                 /* switch 1 */
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1_9 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1_9 + 1);
            sp154 = (char *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1_9 * 8));
            FW(sp154, 0x0) = (s32) (((((s32) FW(sp1DC, 0xC) >> 1) - 1) & 0xFFF) | 0xFD880000);
            FW(sp154, 0x4) = gl_func_00034458(FW(sp1DC, 0x0), temp_a1_9, sp1DC, arg0);
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1_9 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1_9 + 1);
            temp_a0_6 = FW(sp1DC, 0x4);
            temp_t0_13 = (char *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1_9 * 8));
            FW(temp_t0_13, 0x0) = (s32) (((((s32) (((s32) ((FW(sp1DC, 0x4) + (*(s16 *)((char *)arg1 + 0x20))) - FW(sp1DC, 0x4)) >> 1) + 7) >> 3) & 0x1FF) << 9) | 0xF5880000);
            temp_t2 = (sp1E0 & 3) << 0x12;
            temp_t3 = (sp1E8 & 0xF) << 0xE;
            temp_t4 = (sp1E4 & 3) << 8;
            temp_t5 = (sp1EC & 0xF) * 0x10;
            FW(temp_t0_13, 0x4) = (s32) (0x07000000 | temp_t2 | temp_t3 | temp_t4 | temp_t5);
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1_9 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1_9 + 1);
            temp_v0_4 = (char *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1_9 * 8));
            FW(temp_v0_4, 0x0) = 0xE6000000; FW(temp_v0_4, 0x4) = 0;
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1_9 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1_9 + 1);
            temp_a0 = (char *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1_9 * 8));
            FW(temp_a0, 0x0) = (s32) ((((FW(sp1DC, 0x4) * 2) & 0xFFF) << 0xC) | 0xF4000000 | ((FW(sp1DC, 0x8) * 4) & 0xFFF)); FW(temp_a0, 0x4) = (s32) (0x07000000 | (((((FW(sp1DC, 0x4) + (*(s16 *)((char *)arg1 + 0x20))) - 1) << 1) & 0xFFF) << 0xC) | ((((FW(sp1DC, 0x8) + (*(s16 *)((char *)arg1 + 0x22))) - 1) << 2) & 0xFFF));
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1_9 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1_9 + 1);
            temp_v0_4 = (char *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1_9 * 8));
            FW(temp_v0_4, 0x0) = 0xE7000000; FW(temp_v0_4, 0x4) = 0;
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1_9 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1_9 + 1);
            temp_a0_6 = FW(sp1DC, 0x4);
            temp_t0_14 = (char *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1_9 * 8));
            FW(temp_t0_14, 0x0) = (s32) (((((s32) (((s32) ((FW(sp1DC, 0x4) + (*(s16 *)((char *)arg1 + 0x20))) - FW(sp1DC, 0x4)) >> 1) + 7) >> 3) & 0x1FF) << 9) | 0xF5800000); FW(temp_t0_14, 0x4) = (s32) (temp_t2 | temp_t3 | temp_t4 | temp_t5);
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1_9 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1_9 + 1);
            temp_t0_15 = (char *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1_9 * 8));
            FW(temp_t0_15, 0x0) = 0xF2000000;
            temp_a0_6 = FW(sp1DC, 0x4);
            temp_v0_6 = FW(sp1DC, 0x8);
            FW(temp_t0_15, 0x4) = (s32) (((((((FW(sp1DC, 0x4) + (*(s16 *)((char *)arg1 + 0x20))) - FW(sp1DC, 0x4)) - 1) << 2) & 0xFFF) << 0xC) | (((((temp_v0_6 + (*(s16 *)((char *)arg1 + 0x22))) - temp_v0_6) - 1) << 2) & 0xFFF));
            return;
        s1_508:                                 /* switch 1 */
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_t1_7 = (int *)( FW(sp1DC, 0x0));
            temp_a1_9 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1_9 + 1);
            temp_v0_4 = (char *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1_9 * 8));
            FW(temp_v0_4, 0x0) = 0xFD100000; FW(temp_v0_4, 0x4) = (s32) FW(temp_t1_7, 0xC);
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1_9 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1_9 + 1);
            temp_a0 = (char *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1_9 * 8));
            FW(temp_a0, 0x0) = 0xE8000000; FW(temp_a0, 0x4) = 0;
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1_9 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1_9 + 1);
            temp_v0_4 = (char *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1_9 * 8));
            FW(temp_v0_4, 0x0) = 0xF5000100; FW(temp_v0_4, 0x4) = 0x07000000;

            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1_9 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1_9 + 1);
            temp_a0 = (char *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1_9 * 8));
            FW(temp_a0, 0x0) = 0xE6000000; FW(temp_a0, 0x4) = 0;
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1_9 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1_9 + 1);
            temp_t0_16 = (char *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1_9 * 8));
            FW(temp_t0_16, 0x0) = 0xF0000000; FW(temp_t0_16, 0x4) = 0x073FC000;
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1_9 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1_9 + 1);
            temp_v0_4 = (char *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1_9 * 8));
            FW(temp_v0_4, 0x0) = 0xE7000000; FW(temp_v0_4, 0x4) = 0;
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1_9 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1_9 + 1);
            temp_a0 = (char *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1_9 * 8));
            FW(temp_a0, 0x0) = 0xBA000E02; FW(temp_a0, 0x4) = 0x8000;
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1_9 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1_9 + 1);
            sp118 = (char *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1_9 * 8));
            FW(sp118, 0x0) = (s32) (((FW(sp1DC, 0xC) - 1) & 0xFFF) | 0xFD480000); FW(sp118, 0x4) = gl_func_00034458(FW(temp_t1_7, 0x8), temp_a1_9, sp1DC, arg0);
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1_9 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1_9 + 1);
            temp_a0_6 = FW(sp1DC, 0x4);
            temp_t0_17 = (char *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1_9 * 8));
            FW(temp_t0_17, 0x0) = (s32) (((((s32) (((temp_a0_6 + (*(s16 *)((char *)arg1 + 0x20))) - temp_a0_6) + 7) >> 3) & 0x1FF) << 9) | 0xF5480000);
            temp_t2 = (sp1E0 & 3) << 0x12;
            temp_t3 = (sp1E8 & 0xF) << 0xE;
            temp_t4 = (sp1E4 & 3) << 8;
            temp_t5 = (sp1EC & 0xF) * 0x10;
            FW(temp_t0_17, 0x4) = (s32) (0x07000000 | temp_t2 | temp_t3 | temp_t4 | temp_t5);
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1_9 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1_9 + 1);
            temp_v0_4 = (char *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1_9 * 8));
            FW(temp_v0_4, 0x0) = 0xE6000000; FW(temp_v0_4, 0x4) = 0;
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1_9 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1_9 + 1);
            temp_a0 = (char *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1_9 * 8));
            FW(temp_a0, 0x0) = (s32) ((((FW(sp1DC, 0x4) * 4) & 0xFFF) << 0xC) | 0xF4000000 | ((FW(sp1DC, 0x8) * 4) & 0xFFF)); FW(temp_a0, 0x4) = (s32) (0x07000000 | (((((FW(sp1DC, 0x4) + (*(s16 *)((char *)arg1 + 0x20))) - 1) << 2) & 0xFFF) << 0xC) | ((((FW(sp1DC, 0x8) + (*(s16 *)((char *)arg1 + 0x22))) - 1) << 2) & 0xFFF));
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1_9 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1_9 + 1);
            temp_v0_4 = (char *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1_9 * 8));
            FW(temp_v0_4, 0x0) = 0xE7000000; FW(temp_v0_4, 0x4) = 0;
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1_9 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1_9 + 1);
            temp_a0_6 = FW(sp1DC, 0x4);
            temp_t0_18 = (char *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1_9 * 8));
            FW(temp_t0_18, 0x0) = (s32) (((((s32) (((FW(sp1DC, 0x4) + (*(s16 *)((char *)arg1 + 0x20))) - FW(sp1DC, 0x4)) + 7) >> 3) & 0x1FF) << 9) | 0xF5480000); FW(temp_t0_18, 0x4) = (s32) (temp_t2 | temp_t3 | temp_t4 | temp_t5);
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1_9 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1_9 + 1);
            temp_t0_19 = (char *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1_9 * 8));
            FW(temp_t0_19, 0x0) = 0xF2000000;
            temp_a0_6 = FW(sp1DC, 0x4);
            temp_v0_6 = FW(sp1DC, 0x8);
            FW(temp_t0_19, 0x4) = (s32) (((((((FW(sp1DC, 0x4) + (*(s16 *)((char *)arg1 + 0x20))) - FW(sp1DC, 0x4)) - 1) << 2) & 0xFFF) << 0xC) | (((((temp_v0_6 + (*(s16 *)((char *)arg1 + 0x22))) - temp_v0_6) - 1) << 2) & 0xFFF));
            return;
        s1_def:                                    /* switch 1 */
            gl_func_00034458((int *)(gl_d_578b4_s1 + 0x219B8), temp_a1, sp1DC, arg0);
            return;
        }
    } else {
        temp_a1 = FW(arg1, 0x24);
        /* edg-style source-order compare chain (switch 2) */
        if (temp_a1 == 0x120) goto s2_120; if (temp_a1 == 0x110) goto s2_110; if (temp_a1 == 0x408) goto s2_408; if (temp_a1 == 0x308) goto s2_308; if (temp_a1 == 0x304) goto s2_304; if (temp_a1 != 0x508) goto s2_def; goto s2_508;
        {
        s2_120:                                 /* switch 2 */
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1 + 1);
            spFC = (int *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1 * 8));
            FW(spFC, 0x0) = 0xFD180000; FW(spFC, 0x4) = gl_func_00034458(FW(arg1, 0x1C), temp_a1, spFC, arg0);
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1 + 1);
            temp_ra_2 = arg2 & 0x1FF;
            temp_a0 = (char *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1 * 8));
            FW(temp_a0, 0x0) = (s32) (temp_ra_2 | 0xF5180000);
            temp_t2 = (sp1E0 & 3) << 0x12;
            temp_t3 = (sp1E8 & 0xF) << 0xE;
            temp_t4 = (sp1E4 & 3) << 8;
            temp_t5 = (sp1EC & 0xF) * 0x10;
            FW(temp_a0, 0x4) = (s32) (0x07000000 | temp_t2 | temp_t3 | (((*(s16 *)((char *)arg1 + 0x2E)) & 0xF) << 0xA) | temp_t4 | temp_t5 | ((*(s16 *)((char *)arg1 + 0x2C)) & 0xF));
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1 + 1);
            temp_v0_4 = (char *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1 * 8));
            FW(temp_v0_4, 0x0) = 0xE6000000; FW(temp_v0_4, 0x4) = 0;
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1 + 1);
            spF0 = (s32 *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1 * 8));
            *spF0 = 0xF3000000;
            temp_a0_6 = ((*(s16 *)((char *)arg1 + 0x20)) * (*(s16 *)((char *)arg1 + 0x22))) - 1;
            temp_t1 = (temp_a0_6 < 0x7FF) ? temp_a0_6 : 0x7FF;
            temp_v0_6 = (s32) ((*(s16 *)((char *)arg1 + 0x20)) * 4) / 8;
            if (temp_v0_6 <= 0) {
                var_t0 = 1;
            } else {
                var_t0 = temp_v0_6;
            }
            if (temp_v0_6 <= 0) {
                temp_a1 = 1;
            } else {
                temp_a1 = temp_v0_6;
            }
            FW(spF0, 0x4) = (s32) (((temp_t1 & 0xFFF) << 0xC) | 0x07000000 | (((s32) (var_t0 + 0x7FF) / temp_a1) & 0xFFF));
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1 + 1);
            temp_v0_4 = (char *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1 * 8));
            FW(temp_v0_4, 0x0) = 0xE7000000; FW(temp_v0_4, 0x4) = 0;
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1 + 1);
            temp_a0 = (char *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1 * 8));
            FW(temp_a0, 0x0) = (s32) (0xF5180000 | ((((s32) (((*(s16 *)((char *)arg1 + 0x20)) * 2) + 7) >> 3) & 0x1FF) << 9) | temp_ra_2); FW(temp_a0, 0x4) = (s32) (((FW(arg1, 0x3C) & 7) << 0x18) | temp_t2 | temp_t3 | (((*(s16 *)((char *)arg1 + 0x2E)) & 0xF) << 0xA) | temp_t4 | temp_t5 | ((*(s16 *)((char *)arg1 + 0x2C)) & 0xF));
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1 + 1);
            temp_v0_4 = (char *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1 * 8));
            FW(temp_v0_4, 0x0) = 0xF2000000; FW(temp_v0_4, 0x4) = (s32) (((FW(arg1, 0x3C) & 7) << 0x18) | (((((*(s16 *)((char *)arg1 + 0x20)) - 1) << 2) & 0xFFF) << 0xC) | ((((*(s16 *)((char *)arg1 + 0x22)) - 1) << 2) & 0xFFF));
            return;
        s2_110:                                 /* switch 2 */
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1 + 1);
            spE0 = (int *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1 * 8));
            FW(spE0, 0x0) = 0xFD100000; FW(spE0, 0x4) = gl_func_00034458(FW(arg1, 0x1C), temp_a1, spE0, arg0);
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1 + 1);
            temp_ra_3 = arg2 & 0x1FF;
            temp_a0 = (char *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1 * 8));
            FW(temp_a0, 0x0) = (s32) (temp_ra_3 | 0xF5100000);
            temp_t2 = (sp1E0 & 3) << 0x12;
            temp_t3 = (sp1E8 & 0xF) << 0xE;
            temp_t4 = (sp1E4 & 3) << 8;
            temp_t5 = (sp1EC & 0xF) * 0x10;
            FW(temp_a0, 0x4) = (s32) (temp_t2 | 0x07000000 | temp_t3 | temp_t4 | temp_t5);
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1 + 1);
            temp_t0_21 = (char *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1 * 8));
            FW(temp_t0_21, 0x0) = 0xE6000000; FW(temp_t0_21, 0x4) = 0;
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1 + 1);
            spD4 = (s32 *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1 * 8));
            *spD4 = 0xF3000000;
            temp_a0_6 = ((*(s16 *)((char *)arg1 + 0x20)) * (*(s16 *)((char *)arg1 + 0x22))) - 1;
            temp_t1 = (temp_a0_6 < 0x7FF) ? temp_a0_6 : 0x7FF;
            temp_v0_6 = (s32) ((*(s16 *)((char *)arg1 + 0x20)) * 2) / 8;
            if (temp_v0_6 <= 0) {
                var_t0 = 1;
            } else {
                var_t0 = temp_v0_6;
            }
            if (temp_v0_6 <= 0) {
                temp_a1 = 1;
            } else {
                temp_a1 = temp_v0_6;
            }
            FW(spD4, 0x4) = (s32) (((temp_t1 & 0xFFF) << 0xC) | 0x07000000 | (((s32) (var_t0 + 0x7FF) / temp_a1) & 0xFFF));
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1 + 1);
            temp_v0_4 = (char *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1 * 8));
            FW(temp_v0_4, 0x0) = 0xE7000000; FW(temp_v0_4, 0x4) = 0;
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1 + 1);
            temp_a0 = (char *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1 * 8));
            FW(temp_a0, 0x0) = (s32) (((((s32) (((*(s16 *)((char *)arg1 + 0x20)) * 2) + 7) >> 3) & 0x1FF) << 9) | 0xF5100000 | temp_ra_3); FW(temp_a0, 0x4) = (s32) (((FW(arg1, 0x3C) & 7) << 0x18) | temp_t2 | temp_t3 | temp_t4 | temp_t5);
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1 + 1);
            temp_v0_4 = (char *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1 * 8));
            FW(temp_v0_4, 0x0) = 0xF2000000; FW(temp_v0_4, 0x4) = (s32) (((FW(arg1, 0x3C) & 7) << 0x18) | (((((*(s16 *)((char *)arg1 + 0x20)) - 1) << 2) & 0xFFF) << 0xC) | ((((*(s16 *)((char *)arg1 + 0x22)) - 1) << 2) & 0xFFF));
            return;
        s2_408:                                 /* switch 2 */
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1 + 1);
            spC4 = (int *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1 * 8));
            FW(spC4, 0x0) = 0xFD700000; FW(spC4, 0x4) = gl_func_00034458(FW(arg1, 0x1C), temp_a1, spC4, arg0);
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1 + 1);
            temp_ra_4 = arg2 & 0x1FF;
            temp_a0 = (char *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1 * 8));
            FW(temp_a0, 0x0) = (s32) (temp_ra_4 | 0xF5700000);
            temp_t2 = (sp1E0 & 3) << 0x12;
            temp_t3 = (sp1E8 & 0xF) << 0xE;
            temp_t4 = (sp1E4 & 3) << 8;
            temp_t5 = (sp1EC & 0xF) * 0x10;
            FW(temp_a0, 0x4) = (s32) (0x07000000 | temp_t2 | temp_t3 | (((*(s16 *)((char *)arg1 + 0x2E)) & 0xF) << 0xA) | temp_t4 | temp_t5 | ((*(s16 *)((char *)arg1 + 0x2C)) & 0xF));
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1 + 1);
            temp_v0_4 = (char *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1 * 8));
            FW(temp_v0_4, 0x0) = 0xE6000000; FW(temp_v0_4, 0x4) = 0;
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1 + 1);
            spB8 = (s32 *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1 * 8));
            *spB8 = 0xF3000000;
            temp_a0_6 = ((s32) (((*(s16 *)((char *)arg1 + 0x20)) * (*(s16 *)((char *)arg1 + 0x22))) + 1) >> 1) - 1;
            temp_t1 = (temp_a0_6 < 0x7FF) ? temp_a0_6 : 0x7FF;
            temp_v0_6 = (*(s16 *)((char *)arg1 + 0x20)) / 8;
            if (temp_v0_6 <= 0) {
                var_t0 = 1;
            } else {
                var_t0 = temp_v0_6;
            }
            if (temp_v0_6 <= 0) {
                temp_a1 = 1;
            } else {
                temp_a1 = temp_v0_6;
            }
            FW(spB8, 0x4) = (s32) (((temp_t1 & 0xFFF) << 0xC) | 0x07000000 | (((s32) (var_t0 + 0x7FF) / temp_a1) & 0xFFF));
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1 + 1);
            temp_v0_4 = (char *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1 * 8));
            FW(temp_v0_4, 0x0) = 0xE7000000; FW(temp_v0_4, 0x4) = 0;
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1 + 1);
            temp_a0 = (char *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1 * 8));
            FW(temp_a0, 0x0) = (s32) (((((s32) ((*(s16 *)((char *)arg1 + 0x20)) + 7) >> 3) & 0x1FF) << 9) | 0xF5680000 | temp_ra_4); FW(temp_a0, 0x4) = (s32) (((FW(arg1, 0x3C) & 7) << 0x18) | temp_t2 | temp_t3 | (((*(s16 *)((char *)arg1 + 0x2E)) & 0xF) << 0xA) | temp_t4 | temp_t5 | ((*(s16 *)((char *)arg1 + 0x2C)) & 0xF));
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1 + 1);
            temp_v0_4 = (char *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1 * 8));
            FW(temp_v0_4, 0x0) = 0xF2000000; FW(temp_v0_4, 0x4) = (s32) (((FW(arg1, 0x3C) & 7) << 0x18) | (((((*(s16 *)((char *)arg1 + 0x20)) - 1) << 2) & 0xFFF) << 0xC) | ((((*(s16 *)((char *)arg1 + 0x22)) - 1) << 2) & 0xFFF));
            return;
        s2_308:                                 /* switch 2 */
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1 + 1);
            spA8 = (int *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1 * 8));
            FW(spA8, 0x0) = 0xFD900000; FW(spA8, 0x4) = gl_func_00034458(FW(arg1, 0x1C), temp_a1, spA8, arg0);
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1 + 1);
            temp_ra_5 = arg2 & 0x1FF;
            temp_a0 = (char *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1 * 8));
            FW(temp_a0, 0x0) = (s32) (temp_ra_5 | 0xF5900000);
            temp_t2 = (sp1E0 & 3) << 0x12;
            temp_t3 = (sp1E8 & 0xF) << 0xE;
            temp_t4 = (sp1E4 & 3) << 8;
            temp_t5 = (sp1EC & 0xF) * 0x10;
            FW(temp_a0, 0x4) = (s32) (0x07000000 | temp_t2 | temp_t3 | (((*(s16 *)((char *)arg1 + 0x2E)) & 0xF) << 0xA) | temp_t4 | temp_t5 | ((*(s16 *)((char *)arg1 + 0x2C)) & 0xF));
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1 + 1);
            temp_v0_4 = (char *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1 * 8));
            FW(temp_v0_4, 0x0) = 0xE6000000; FW(temp_v0_4, 0x4) = 0;
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1 + 1);
            sp9C = (s32 *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1 * 8));
            *sp9C = 0xF3000000;
            temp_a0_6 = ((s32) (((*(s16 *)((char *)arg1 + 0x20)) * (*(s16 *)((char *)arg1 + 0x22))) + 1) >> 1) - 1;
            temp_t1 = (temp_a0_6 < 0x7FF) ? temp_a0_6 : 0x7FF;
            temp_v0_6 = (*(s16 *)((char *)arg1 + 0x20)) / 8;
            if (temp_v0_6 <= 0) {
                var_t0 = 1;
            } else {
                var_t0 = temp_v0_6;
            }
            if (temp_v0_6 <= 0) {
                temp_a1 = 1;
            } else {
                temp_a1 = temp_v0_6;
            }
            FW(sp9C, 0x4) = (s32) (((temp_t1 & 0xFFF) << 0xC) | 0x07000000 | (((s32) (var_t0 + 0x7FF) / temp_a1) & 0xFFF));
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1 + 1);
            temp_v0_4 = (char *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1 * 8));
            FW(temp_v0_4, 0x0) = 0xE7000000; FW(temp_v0_4, 0x4) = 0;
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1 + 1);
            temp_a0 = (char *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1 * 8));
            FW(temp_a0, 0x0) = (s32) (0xF5880000 | ((((s32) ((*(s16 *)((char *)arg1 + 0x20)) + 7) >> 3) & 0x1FF) << 9) | temp_ra_5); FW(temp_a0, 0x4) = (s32) (((FW(arg1, 0x3C) & 7) << 0x18) | temp_t2 | temp_t3 | (((*(s16 *)((char *)arg1 + 0x2E)) & 0xF) << 0xA) | temp_t4 | temp_t5 | ((*(s16 *)((char *)arg1 + 0x2C)) & 0xF));
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1 + 1);
            temp_v0_4 = (char *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1 * 8));
            FW(temp_v0_4, 0x0) = 0xF2000000; FW(temp_v0_4, 0x4) = (s32) (((FW(arg1, 0x3C) & 7) << 0x18) | (((((*(s16 *)((char *)arg1 + 0x20)) - 1) << 2) & 0xFFF) << 0xC) | ((((*(s16 *)((char *)arg1 + 0x22)) - 1) << 2) & 0xFFF));
            return;
        s2_304:                                 /* switch 2 */
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1 + 1);
            sp8C = (int *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1 * 8));
            FW(sp8C, 0x0) = 0xFD900000; FW(sp8C, 0x4) = gl_func_00034458(FW(arg1, 0x1C), temp_a1, sp8C, arg0);
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1 + 1);
            temp_ra_6 = arg2 & 0x1FF;
            temp_a0 = (char *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1 * 8));
            FW(temp_a0, 0x0) = (s32) (temp_ra_6 | 0xF5900000);
            temp_t2 = (sp1E0 & 3) << 0x12;
            temp_t3 = (sp1E8 & 0xF) << 0xE;
            temp_t4 = (sp1E4 & 3) << 8;
            temp_t5 = (sp1EC & 0xF) * 0x10;
            FW(temp_a0, 0x4) = (s32) (0x07000000 | temp_t2 | temp_t3 | (((*(s16 *)((char *)arg1 + 0x2E)) & 0xF) << 0xA) | temp_t4 | temp_t5 | ((*(s16 *)((char *)arg1 + 0x2C)) & 0xF));
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1 + 1);
            temp_v0_4 = (char *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1 * 8));
            FW(temp_v0_4, 0x0) = 0xE6000000; FW(temp_v0_4, 0x4) = 0;
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1 + 1);
            sp80 = (s32 *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1 * 8));
            *sp80 = 0xF3000000;
            temp_a0_6 = ((s32) (((*(s16 *)((char *)arg1 + 0x20)) * (*(s16 *)((char *)arg1 + 0x22))) + 3) >> 2) - 1;
            temp_t1 = (temp_a0_6 < 0x7FF) ? temp_a0_6 : 0x7FF;
            temp_v0_6 = (*(s16 *)((char *)arg1 + 0x20)) / 16;
            if (temp_v0_6 <= 0) {
                var_t0 = 1;
            } else {
                var_t0 = temp_v0_6;
            }
            if (temp_v0_6 <= 0) {
                temp_a1 = 1;
            } else {
                temp_a1 = temp_v0_6;
            }
            FW(sp80, 0x4) = (s32) (((temp_t1 & 0xFFF) << 0xC) | 0x07000000 | (((s32) (var_t0 + 0x7FF) / temp_a1) & 0xFFF));
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1 + 1);
            temp_v0_4 = (char *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1 * 8));
            FW(temp_v0_4, 0x0) = 0xE7000000; FW(temp_v0_4, 0x4) = 0;
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1 + 1);
            temp_a0 = (char *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1 * 8));
            FW(temp_a0, 0x0) = (s32) (((((s32) (((s16) (*(s16 *)((char *)arg1 + 0x20)) >> 1) + 7) >> 3) & 0x1FF) << 9) | 0xF5800000 | temp_ra_6); FW(temp_a0, 0x4) = (s32) (((FW(arg1, 0x3C) & 7) << 0x18) | temp_t2 | temp_t3 | (((*(s16 *)((char *)arg1 + 0x2E)) & 0xF) << 0xA) | temp_t4 | temp_t5 | ((*(s16 *)((char *)arg1 + 0x2C)) & 0xF));
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1 + 1);
            temp_v0_4 = (char *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1 * 8));
            FW(temp_v0_4, 0x0) = 0xF2000000; FW(temp_v0_4, 0x4) = (s32) (((FW(arg1, 0x3C) & 7) << 0x18) | (((((*(s16 *)((char *)arg1 + 0x20)) - 1) << 2) & 0xFFF) << 0xC) | ((((*(s16 *)((char *)arg1 + 0x22)) - 1) << 2) & 0xFFF));
            return;
        s2_508:                                 /* switch 2 */
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1 = FW(temp_v1, 0x4);
            temp_t1_7 = (int *)( *(int*)sp1DC);
            FW(temp_v1, 0x4) = (s32) (temp_a1 + 1);
            temp_v0_4 = (char *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1 * 8));
            FW(temp_v0_4, 0x0) = 0xFD100000; FW(temp_v0_4, 0x4) = (s32) FW(temp_t1_7, 0xC);
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1 + 1);
            temp_a0 = (char *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1 * 8));
            FW(temp_a0, 0x0) = 0xE8000000; FW(temp_a0, 0x4) = 0;
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1 + 1);
            temp_v0_4 = (char *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1 * 8));
            FW(temp_v0_4, 0x0) = 0xF5000100; FW(temp_v0_4, 0x4) = 0x07000000;

            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1 + 1);
            temp_a0 = (char *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1 * 8));
            FW(temp_a0, 0x0) = 0xE6000000; FW(temp_a0, 0x4) = 0;
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1 + 1);
            temp_t0_25 = (char *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1 * 8));
            FW(temp_t0_25, 0x0) = 0xF0000000; FW(temp_t0_25, 0x4) = 0x073FC000;
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1 + 1);
            temp_v0_4 = (char *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1 * 8));
            FW(temp_v0_4, 0x0) = 0xE7000000; FW(temp_v0_4, 0x4) = 0;
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1 + 1);
            temp_t0_25 = (char *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1 * 8));
            FW(temp_t0_25, 0x0) = 0xBA000E02; FW(temp_t0_25, 0x4) = 0x8000;
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1 + 1);
            sp50 = (s32)( FW(FW(arg0, 0xC), 0x0) + (temp_a1 * 8));
            if (1) { FW(sp50, 0x0) = 0xFD500000; }
            
            FW(sp50, 0x4) = gl_func_00034458(FW(temp_t1_7, 0x8), temp_a1, sp1DC, arg0);
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1 + 1);
            sp50 = (arg2 & 0x1FF) + 0 * FW(arg1, 0x0);
            temp_a0 = (char *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1 * 8));
            FW(temp_a0, 0x0) = (s32) (sp50 | 0xF5500000);
            temp_t2 = (sp1E0 & 3) << 0x12;
            temp_t3 = (sp1E8 & 0xF) << 0xE;
            temp_t4 = (sp1E4 & 3) << 8;
            temp_t5 = (sp1EC & 0xF) * 0x10;
            FW(temp_a0, 0x4) = (s32) (0x07000000 | temp_t2 | temp_t3 | (((*(s16 *)((char *)arg1 + 0x2E)) & 0xF) << 0xA) | temp_t4 | temp_t5 | ((*(s16 *)((char *)arg1 + 0x2C)) & 0xF));
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1 + 1);
            sp20 = sp50;
            temp_v0_4 = (char *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1 * 8));
            FW(temp_v0_4, 0x0) = 0xE6000000; FW(temp_v0_4, 0x4) = 0;
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1 + 1);
            sp44 = (s32 *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1 * 8));
            *sp44 = 0xF3000000;
            temp_a0_6 = FW(sp1DC, 0xC); temp_t0_27 = ((s32) ((temp_a0_6 * FW(sp1DC, 0x10)) + 1) >> 1) - 1;
            temp_t1 = (temp_t0_27 < 0x7FF) ? temp_t0_27 : 0x7FF;
            temp_v0_6 = temp_a0_6 / 8;
            if (temp_v0_6 <= 0) {
                var_t0 = 1;
            } else {
                var_t0 = temp_v0_6;
            }
            if (temp_v0_6 <= 0) {
                temp_a1 = 1;
            } else {
                temp_a1 = temp_v0_6;
            }
            FW(sp44, 0x4) = (s32) (((temp_t1 & 0xFFF) << 0xC) | 0x07000000 | (((s32) (var_t0 + 0x7FF) / temp_a1) & 0xFFF));
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1 + 1);
            temp_v0_4 = (char *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1 * 8));
            FW(temp_v0_4, 0x0) = 0xE7000000; FW(temp_v0_4, 0x4) = 0;
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1 + 1);
            temp_a0 = (char *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1 * 8));
            FW(temp_a0, 0x0) = (s32) (((((s32) (FW(sp1DC, 0xC) + 7) >> 3) & 0x1FF) << 9) | 0xF5480000 | sp50); FW(temp_a0, 0x4) = (s32) (((FW(arg1, 0x3C) & 7) << 0x18) | temp_t2 | temp_t3 | (((*(s16 *)((char *)arg1 + 0x2E)) & 0xF) << 0xA) | temp_t4 | temp_t5 | ((*(s16 *)((char *)arg1 + 0x2C)) & 0xF));
            temp_v1 = (char *)( FW(arg0, 0xC));
            temp_a1 = FW(temp_v1, 0x4);
            FW(temp_v1, 0x4) = (s32) (temp_a1 + 1);
            temp_v0_4 = (char *)( FW(FW(arg0, 0xC), 0x0) + (temp_a1 * 8));
            FW(temp_v0_4, 0x0) = 0xF2000000; FW(temp_v0_4, 0x4) = (s32) (((FW(arg1, 0x3C) & 7) << 0x18) | ((((FW(sp1DC, 0xC) - 1) << 2) & 0xFFF) << 0xC) | (((FW(sp1DC, 0x10) - 1) << 2) & 0xFFF));
            if (0) { gl_func_00034458((s32 *)&sp34); }
            temp_a1 = sp34 = FW(arg1, 0x24);
        s2_def:                                    /* switch 2 */
            gl_func_00034458((int *)(gl_d_578b4_s2b + 0x219C4), temp_a1);
            return;
        }
    }
}
