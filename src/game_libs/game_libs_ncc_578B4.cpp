/* IDO 7.1 NCC (C++) EXACT donor for gfx_loadTexture (texture loader: gbi LoadTextureTile /
 * LoadBlock per format, CI8 with a TLUT). REPLACE_FUNC_BODY-spliced over the empty placeholder
 * in game_libs_post0b.c.o on both build paths (agent-c run 236); the cfe body kept in
 * game_libs_post0b.c under GL_578B4_OLD_DECODE is the m2c transcription. The 0x34458 calls go
 * through gl_578b4_c34458 (pinned 0: the target's blank USO jal words).
 * agent-c run 154: the cfe body ported to NCC (cfe 98.27 -> NCC 99.93):
 * - edg evaluates a |-chain in SOURCE order, so every packet word is the gbi macro's own
 *   operand order (SetTile w1 flat `tile | cmt | maskt | shiftt | cms | masks | shifts`,
 *   TILESIZE w1 `tile | lrs<<12 | lrt`, LoadTile `uls<<12 | cmd | ult`, `w * h` = 0x20 * 0x22);
 *   this fixed every commutative-operand residual cfe could not reach.
 * - NCC drops never-referenced locals but gives every referenced one a frame home, so the
 *   m2c pad locals are referenced by one dead `= 0` line (frame 0x1F0 = target).
 * - switch 1 is `switch (mat->format)` (edg source-order compare chain, t1 temp).
 * - the two s2_508 single-store packets share one pointer variable (temp_t0_25), which
 *   fixes the s1_508 / s2_508 t0/t1 webs (temp_t1_7 is shared by both arms).
 * - one statement per gbi line matters under NCC: `temp_a0_6 = ...; temp_t0_27 = ...` on
 *   one line keeps the target's 0x10-then-0xC load order.
 * - the s1_508 LOADTILE SetTile w0 reads the width through temp_a0_6 (`(w + h) - w`, like
 *   the other arms): the target then holds 0x07000000 in t1 instead of 0xF5480000.
 * agent-c run 236: the s2_508 FD50 DL pointer and the tmem value `tmem & 0x1FF` are ONE
 *   variable (sp50, s32). The tmem def carries a folded read (`+ 0 * FW(mat, 0x0)`) so uopt
 *   does not copy-propagate it: the variable's live range then reaches the later t0 users and
 *   gets ra. `if (1) { FD50 store; }` puts the def and the store in separate units (adjsave
 *   7/5 = 1.4, below the 1.5 of the t2..t5 shift temps, so it colours after them).
 *   The target's `lw t0,504(sp); andi ra,t0,0x1ff` is tmem copied into its own non-propagated
 *   variable first (temp_ra_8 = tmem + folded read; t0), then masked into sp50 (second folded
 *   read keeps sp50 a variable): EXACT.
 * agent-a run 520: typed through the headers (byte-identical, objdump -d -r): ctx is the
 *   DLContext (gdl/dlctx.h, GfxWords in gdl/texload.h; every append is `n = gfx.cur->count++; GfxWords at gfx.cur->base +
 *   n * 8`), mat the BaseMat (gdl/material.h: texels / width / height / format / texFlags /
 *   shiftS / shiftT / tile), sp1DC the TexImage record (gdl/texload.h) (MAT_TEX_RECORD and CI8), temp_t1_7 the
 *   TexCI (texels / tlut). The two `0 * FW(mat, ...)` reads stay raw: dummy int reads of
 *   BaseMat.colour (f32) that only steer uopt. */
typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef unsigned short u16;
typedef int s32; typedef unsigned int u32; typedef float f32;
#define FW(p, o) (*(int *)((char *)(p) + (o)))
#include "../../include/gdl/dlctx.h"     /* DLContext / DLBuf (NCC donors build without -I include) */
#include "../../include/gdl/material.h"  /* BaseMat, MAT_FMT_* / MAT_TEX_* */
#include "../../include/gdl/texload.h"   /* TexImage / TexCI / GfxWords */
extern "C" {
int gl_578b4_c34458(...);
int game_libs_func_00062F08_578B4(...);
extern char sStr_unkown_tile_219B8[];
extern char gl_d_578b4_s2a[];
extern char sFmt_bad_texfmt_num_219C4[];
}
extern "C"
void gfx_loadTexture(DLContext *ctx, BaseMat *mat, s32 tmem) {
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

    sp1EC = game_libs_func_00062F08_578B4(mat->width);
    sp1E8 = game_libs_func_00062F08_578B4(mat->height);
    sp1E0 = 0;
    sp1E4 = 0;
    temp_v1 = (char *)( (s32)ctx->gfx.cur);
    temp_a1 = DLBUF(temp_v1)->count;
    DLBUF(temp_v1)->count = (s32) (temp_a1 + 1);
    temp_a0 = (char *)( ctx->gfx.cur->base + (temp_a1 * 8));
    GFXW(temp_a0)->w0 = 0xBA000E02; GFXW(temp_a0)->w1 = 0;
    var_a2 = mat->texFlags;
    if (!(var_a2 & MAT_TEX_WRAP_S)) {
        if (var_a2 & MAT_TEX_CLAMP_S) {
            sp1E4 = 2;
        } else if (var_a2 & MAT_TEX_MIRROR_S) {
            sp1E4 = 1;
        }
    }
    if (!(var_a2 & MAT_TEX_WRAP_T)) {
        if (var_a2 & MAT_TEX_CLAMP_T) {
            sp1E0 = 2;
        } else if (var_a2 & MAT_TEX_MIRROR_T) {
            sp1E0 = 1;
        }
    }
    if (var_a2 & MAT_TEX_RECORD) {

        sp1DC = (int *) (s32)mat->texels;
        /* edg-style source-order compare chain (switch 1) */
        switch (mat->format) { case MAT_FMT_RGBA32: goto s1_120; case MAT_FMT_RGBA16: goto s1_110; case MAT_FMT_IA8: goto s1_408; case MAT_FMT_I8: goto s1_308; case MAT_FMT_I4: goto s1_304; case MAT_FMT_CI8: goto s1_508; default: goto s1_def; }
        {
        s1_120:
            g = (s32)ctx->gfx.cur;
            n = DLBUF(g)->count;
            DLBUF(g)->count = n + 1;
            sp1C4 = (char *) (ctx->gfx.cur->base + (n * 8));
            GFXW(sp1C4)->w0 = ((TEXIMG(sp1DC)->width - 1) & 0xFFF) | 0xFD180000; GFXW(sp1C4)->w1 = gl_578b4_c34458(TEXIMG(sp1DC)->pixels, n, sp1DC, ctx);
            g = (s32)ctx->gfx.cur;
            n = DLBUF(g)->count;
            DLBUF(g)->count = n + 1;
            w = TEXIMG(sp1DC)->s;
            temp_t0_4 = (char *)( ctx->gfx.cur->base + (n * 8));
            GFXW(temp_t0_4)->w0 = (((s32) ((((w + mat->width) - w) * 2) + 7) >> 3) & 0x1FF) << 9 | 0xF5180000;
            temp_t2 = (sp1E0 & 3) << 0x12;
            temp_t3 = (sp1E8 & 0xF) << 0xE;
            temp_t4 = (sp1E4 & 3) << 8;
            temp_t5 = (sp1EC & 0xF) * 0x10;
            GFXW(temp_t0_4)->w1 = 0x07000000 | temp_t2 | temp_t3 | temp_t4 | temp_t5;
            g = (s32)ctx->gfx.cur;
            n = DLBUF(g)->count;
            DLBUF(g)->count = n + 1;
            temp_v0_4 = (char *)( ctx->gfx.cur->base + (n * 8));
            GFXW(temp_v0_4)->w0 = 0xE6000000; GFXW(temp_v0_4)->w1 = 0;
            g = (s32)ctx->gfx.cur;
            n = DLBUF(g)->count;
            DLBUF(g)->count = n + 1;
            temp_a0 = (char *)( ctx->gfx.cur->base + (n * 8));
            GFXW(temp_a0)->w0 = ((((TEXIMG(sp1DC)->s * 4) & 0xFFF) << 0xC) | 0xF4000000 | ((TEXIMG(sp1DC)->t * 4) & 0xFFF)); GFXW(temp_a0)->w1 = (0x07000000 | (((((TEXIMG(sp1DC)->s + mat->width) - 1) << 2) & 0xFFF) << 0xC) | ((((TEXIMG(sp1DC)->t + mat->height) - 1) << 2) & 0xFFF));
            g = (s32)ctx->gfx.cur;
            n = DLBUF(g)->count;
            DLBUF(g)->count = n + 1;
            temp_v0_4 = (char *)( ctx->gfx.cur->base + (n * 8));
            GFXW(temp_v0_4)->w0 = 0xE7000000; GFXW(temp_v0_4)->w1 = 0;
            g = (s32)ctx->gfx.cur;
            n = DLBUF(g)->count;
            DLBUF(g)->count = n + 1;
            w = TEXIMG(sp1DC)->s;
            temp_t0_5 = (char *)( ctx->gfx.cur->base + (n * 8));
            GFXW(temp_t0_5)->w0 = (((s32) ((((TEXIMG(sp1DC)->s + mat->width) - TEXIMG(sp1DC)->s) * 2) + 7) >> 3) & 0x1FF) << 9 | 0xF5180000; GFXW(temp_t0_5)->w1 = temp_t2 | temp_t3 | temp_t4 | temp_t5;
            g = (s32)ctx->gfx.cur;
            n = DLBUF(g)->count;
            DLBUF(g)->count = n + 1;
            temp_t0_6 = (char *)( ctx->gfx.cur->base + (n * 8));
            GFXW(temp_t0_6)->w0 = 0xF2000000;
            w = TEXIMG(sp1DC)->s;
            v = TEXIMG(sp1DC)->t;
            GFXW(temp_t0_6)->w1 = (((((((TEXIMG(sp1DC)->s + mat->width) - TEXIMG(sp1DC)->s) - 1) << 2) & 0xFFF) << 0xC) | (((((v + mat->height) - v) - 1) << 2) & 0xFFF));
            return;
        s1_110:                                 /* switch 1 */
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1_9 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1_9 + 1);
            sp1A8 = (char *)( ctx->gfx.cur->base + (temp_a1_9 * 8));
            GFXW(sp1A8)->w0 = (s32) (((TEXIMG(sp1DC)->width - 1) & 0xFFF) | 0xFD100000);
            GFXW(sp1A8)->w1 = gl_578b4_c34458(TEXIMG(sp1DC)->pixels, temp_a1_9, sp1DC, ctx);
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1_9 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1_9 + 1);
            temp_a0_6 = TEXIMG(sp1DC)->s;
            temp_t0_4 = (char *)( ctx->gfx.cur->base + (temp_a1_9 * 8));
            GFXW(temp_t0_4)->w0 = (s32) (((((s32) ((((TEXIMG(sp1DC)->s + mat->width) - TEXIMG(sp1DC)->s) * 2) + 7) >> 3) & 0x1FF) << 9) | 0xF5100000);
            temp_t2 = (sp1E0 & 3) << 0x12;
            temp_t3 = (sp1E8 & 0xF) << 0xE;
            temp_t4 = (sp1E4 & 3) << 8;
            temp_t5 = (sp1EC & 0xF) * 0x10;
            GFXW(temp_t0_4)->w1 = (s32) (0x07000000 | temp_t2 | temp_t3 | temp_t4 | temp_t5);
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1_9 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1_9 + 1);
            temp_v0_4 = (char *)( ctx->gfx.cur->base + (temp_a1_9 * 8));
            GFXW(temp_v0_4)->w0 = 0xE6000000; GFXW(temp_v0_4)->w1 = 0;
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1_9 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1_9 + 1);
            temp_a0 = (char *)( ctx->gfx.cur->base + (temp_a1_9 * 8));
            GFXW(temp_a0)->w0 = (s32) ((((TEXIMG(sp1DC)->s * 4) & 0xFFF) << 0xC) | 0xF4000000 | ((TEXIMG(sp1DC)->t * 4) & 0xFFF)); GFXW(temp_a0)->w1 = (s32) (0x07000000 | (((((TEXIMG(sp1DC)->s + mat->width) - 1) << 2) & 0xFFF) << 0xC) | ((((TEXIMG(sp1DC)->t + mat->height) - 1) << 2) & 0xFFF));
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1_9 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1_9 + 1);
            temp_v0_4 = (char *)( ctx->gfx.cur->base + (temp_a1_9 * 8));
            GFXW(temp_v0_4)->w0 = 0xE7000000; GFXW(temp_v0_4)->w1 = 0;
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1_9 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1_9 + 1);
            temp_a0_6 = TEXIMG(sp1DC)->s;
            temp_t0_5 = (char *)( ctx->gfx.cur->base + (temp_a1_9 * 8));
            GFXW(temp_t0_5)->w0 = (s32) (((((s32) ((((TEXIMG(sp1DC)->s + mat->width) - TEXIMG(sp1DC)->s) * 2) + 7) >> 3) & 0x1FF) << 9) | 0xF5100000); GFXW(temp_t0_5)->w1 = (s32) (temp_t2 | temp_t3 | temp_t4 | temp_t5);
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1_9 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1_9 + 1);
            temp_t0_6 = (char *)( ctx->gfx.cur->base + (temp_a1_9 * 8));
            GFXW(temp_t0_6)->w0 = 0xF2000000;
            temp_a0_6 = TEXIMG(sp1DC)->s;
            temp_v0_6 = TEXIMG(sp1DC)->t;
            GFXW(temp_t0_6)->w1 = (s32) (((((((TEXIMG(sp1DC)->s + mat->width) - TEXIMG(sp1DC)->s) - 1) << 2) & 0xFFF) << 0xC) | (((((temp_v0_6 + mat->height) - temp_v0_6) - 1) << 2) & 0xFFF));
            return;
        s1_408:                                 /* switch 1 */
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1_9 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1_9 + 1);
            sp18C = (char *)( ctx->gfx.cur->base + (temp_a1_9 * 8));
            GFXW(sp18C)->w0 = (s32) (((TEXIMG(sp1DC)->width - 1) & 0xFFF) | 0xFD680000);
            GFXW(sp18C)->w1 = gl_578b4_c34458(TEXIMG(sp1DC)->pixels, temp_a1_9, sp1DC, ctx);
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1_9 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1_9 + 1);
            temp_a0_6 = TEXIMG(sp1DC)->s;
            temp_t0_7 = (char *)( ctx->gfx.cur->base + (temp_a1_9 * 8));
            GFXW(temp_t0_7)->w0 = (s32) (((((s32) (((temp_a0_6 + mat->width) - temp_a0_6) + 7) >> 3) & 0x1FF) << 9) | 0xF5680000);
            temp_t2 = (sp1E0 & 3) << 0x12;
            temp_t3 = (sp1E8 & 0xF) << 0xE;
            temp_t4 = (sp1E4 & 3) << 8;
            temp_t5 = (sp1EC & 0xF) * 0x10;
            GFXW(temp_t0_7)->w1 = (s32) (0x07000000 | temp_t2 | temp_t3 | temp_t4 | temp_t5);
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1_9 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1_9 + 1);
            temp_v0_4 = (char *)( ctx->gfx.cur->base + (temp_a1_9 * 8));
            GFXW(temp_v0_4)->w0 = 0xE6000000; GFXW(temp_v0_4)->w1 = 0;
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1_9 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1_9 + 1);
            temp_a0 = (char *)( ctx->gfx.cur->base + (temp_a1_9 * 8));
            GFXW(temp_a0)->w0 = (s32) ((((TEXIMG(sp1DC)->s * 4) & 0xFFF) << 0xC) | 0xF4000000 | ((TEXIMG(sp1DC)->t * 4) & 0xFFF)); GFXW(temp_a0)->w1 = (s32) (0x07000000 | (((((TEXIMG(sp1DC)->s + mat->width) - 1) << 2) & 0xFFF) << 0xC) | ((((TEXIMG(sp1DC)->t + mat->height) - 1) << 2) & 0xFFF));
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1_9 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1_9 + 1);
            temp_v0_4 = (char *)( ctx->gfx.cur->base + (temp_a1_9 * 8));
            GFXW(temp_v0_4)->w0 = 0xE7000000; GFXW(temp_v0_4)->w1 = 0;
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1_9 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1_9 + 1);
            temp_a0_6 = TEXIMG(sp1DC)->s;
            temp_t0_8 = (char *)( ctx->gfx.cur->base + (temp_a1_9 * 8));
            GFXW(temp_t0_8)->w0 = (s32) (((((s32) (((TEXIMG(sp1DC)->s + mat->width) - TEXIMG(sp1DC)->s) + 7) >> 3) & 0x1FF) << 9) | 0xF5680000); GFXW(temp_t0_8)->w1 = (s32) (temp_t2 | temp_t3 | temp_t4 | temp_t5);
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1_9 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1_9 + 1);
            temp_t0_9 = (char *)( ctx->gfx.cur->base + (temp_a1_9 * 8));
            GFXW(temp_t0_9)->w0 = 0xF2000000;
            temp_a0_6 = TEXIMG(sp1DC)->s;
            temp_v0_6 = TEXIMG(sp1DC)->t;
            GFXW(temp_t0_9)->w1 = (s32) (((((((TEXIMG(sp1DC)->s + mat->width) - TEXIMG(sp1DC)->s) - 1) << 2) & 0xFFF) << 0xC) | (((((temp_v0_6 + mat->height) - temp_v0_6) - 1) << 2) & 0xFFF));
            return;
        s1_308:                                 /* switch 1 */
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1_9 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1_9 + 1);
            sp170 = (char *)( ctx->gfx.cur->base + (temp_a1_9 * 8));
            GFXW(sp170)->w0 = (s32) (((TEXIMG(sp1DC)->width - 1) & 0xFFF) | 0xFD880000);
            GFXW(sp170)->w1 = gl_578b4_c34458(TEXIMG(sp1DC)->pixels, temp_a1_9, sp1DC, ctx);
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1_9 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1_9 + 1);
            temp_a0_6 = TEXIMG(sp1DC)->s;
            temp_t0_10 = (char *)( ctx->gfx.cur->base + (temp_a1_9 * 8));
            GFXW(temp_t0_10)->w0 = (s32) (((((s32) (((TEXIMG(sp1DC)->s + mat->width) - TEXIMG(sp1DC)->s) + 7) >> 3) & 0x1FF) << 9) | 0xF5880000);
            temp_t2 = (sp1E0 & 3) << 0x12;
            temp_t3 = (sp1E8 & 0xF) << 0xE;
            temp_t4 = (sp1E4 & 3) << 8;
            temp_t5 = (sp1EC & 0xF) * 0x10;
            GFXW(temp_t0_10)->w1 = (s32) (0x07000000 | temp_t2 | temp_t3 | temp_t4 | temp_t5);
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1_9 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1_9 + 1);
            temp_v0_4 = (char *)( ctx->gfx.cur->base + (temp_a1_9 * 8));
            GFXW(temp_v0_4)->w0 = 0xE6000000; GFXW(temp_v0_4)->w1 = 0;
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1_9 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1_9 + 1);
            temp_a0 = (char *)( ctx->gfx.cur->base + (temp_a1_9 * 8));
            GFXW(temp_a0)->w0 = (s32) ((((TEXIMG(sp1DC)->s * 4) & 0xFFF) << 0xC) | 0xF4000000 | ((TEXIMG(sp1DC)->t * 4) & 0xFFF)); GFXW(temp_a0)->w1 = (s32) (0x07000000 | (((((TEXIMG(sp1DC)->s + mat->width) - 1) << 2) & 0xFFF) << 0xC) | ((((TEXIMG(sp1DC)->t + mat->height) - 1) << 2) & 0xFFF));
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1_9 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1_9 + 1);
            temp_v0_4 = (char *)( ctx->gfx.cur->base + (temp_a1_9 * 8));
            GFXW(temp_v0_4)->w0 = 0xE7000000; GFXW(temp_v0_4)->w1 = 0;
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1_9 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1_9 + 1);
            temp_a0_6 = TEXIMG(sp1DC)->s;
            temp_t0_11 = (char *)( ctx->gfx.cur->base + (temp_a1_9 * 8));
            GFXW(temp_t0_11)->w0 = (s32) (((((s32) (((TEXIMG(sp1DC)->s + mat->width) - TEXIMG(sp1DC)->s) + 7) >> 3) & 0x1FF) << 9) | 0xF5880000); GFXW(temp_t0_11)->w1 = (s32) (temp_t2 | temp_t3 | temp_t4 | temp_t5);
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1_9 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1_9 + 1);
            temp_t0_12 = (char *)( ctx->gfx.cur->base + (temp_a1_9 * 8));
            GFXW(temp_t0_12)->w0 = 0xF2000000;
            temp_a0_6 = TEXIMG(sp1DC)->s;
            temp_v0_6 = TEXIMG(sp1DC)->t;
            GFXW(temp_t0_12)->w1 = (s32) (((((((TEXIMG(sp1DC)->s + mat->width) - TEXIMG(sp1DC)->s) - 1) << 2) & 0xFFF) << 0xC) | (((((temp_v0_6 + mat->height) - temp_v0_6) - 1) << 2) & 0xFFF));
            return;
        s1_304:                                 /* switch 1 */
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1_9 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1_9 + 1);
            sp154 = (char *)( ctx->gfx.cur->base + (temp_a1_9 * 8));
            GFXW(sp154)->w0 = (s32) (((((s32) TEXIMG(sp1DC)->width >> 1) - 1) & 0xFFF) | 0xFD880000);
            GFXW(sp154)->w1 = gl_578b4_c34458(TEXIMG(sp1DC)->pixels, temp_a1_9, sp1DC, ctx);
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1_9 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1_9 + 1);
            temp_a0_6 = TEXIMG(sp1DC)->s;
            temp_t0_13 = (char *)( ctx->gfx.cur->base + (temp_a1_9 * 8));
            GFXW(temp_t0_13)->w0 = (s32) (((((s32) (((s32) ((TEXIMG(sp1DC)->s + mat->width) - TEXIMG(sp1DC)->s) >> 1) + 7) >> 3) & 0x1FF) << 9) | 0xF5880000);
            temp_t2 = (sp1E0 & 3) << 0x12;
            temp_t3 = (sp1E8 & 0xF) << 0xE;
            temp_t4 = (sp1E4 & 3) << 8;
            temp_t5 = (sp1EC & 0xF) * 0x10;
            GFXW(temp_t0_13)->w1 = (s32) (0x07000000 | temp_t2 | temp_t3 | temp_t4 | temp_t5);
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1_9 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1_9 + 1);
            temp_v0_4 = (char *)( ctx->gfx.cur->base + (temp_a1_9 * 8));
            GFXW(temp_v0_4)->w0 = 0xE6000000; GFXW(temp_v0_4)->w1 = 0;
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1_9 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1_9 + 1);
            temp_a0 = (char *)( ctx->gfx.cur->base + (temp_a1_9 * 8));
            GFXW(temp_a0)->w0 = (s32) ((((TEXIMG(sp1DC)->s * 2) & 0xFFF) << 0xC) | 0xF4000000 | ((TEXIMG(sp1DC)->t * 4) & 0xFFF)); GFXW(temp_a0)->w1 = (s32) (0x07000000 | (((((TEXIMG(sp1DC)->s + mat->width) - 1) << 1) & 0xFFF) << 0xC) | ((((TEXIMG(sp1DC)->t + mat->height) - 1) << 2) & 0xFFF));
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1_9 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1_9 + 1);
            temp_v0_4 = (char *)( ctx->gfx.cur->base + (temp_a1_9 * 8));
            GFXW(temp_v0_4)->w0 = 0xE7000000; GFXW(temp_v0_4)->w1 = 0;
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1_9 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1_9 + 1);
            temp_a0_6 = TEXIMG(sp1DC)->s;
            temp_t0_14 = (char *)( ctx->gfx.cur->base + (temp_a1_9 * 8));
            GFXW(temp_t0_14)->w0 = (s32) (((((s32) (((s32) ((TEXIMG(sp1DC)->s + mat->width) - TEXIMG(sp1DC)->s) >> 1) + 7) >> 3) & 0x1FF) << 9) | 0xF5800000); GFXW(temp_t0_14)->w1 = (s32) (temp_t2 | temp_t3 | temp_t4 | temp_t5);
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1_9 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1_9 + 1);
            temp_t0_15 = (char *)( ctx->gfx.cur->base + (temp_a1_9 * 8));
            GFXW(temp_t0_15)->w0 = 0xF2000000;
            temp_a0_6 = TEXIMG(sp1DC)->s;
            temp_v0_6 = TEXIMG(sp1DC)->t;
            GFXW(temp_t0_15)->w1 = (s32) (((((((TEXIMG(sp1DC)->s + mat->width) - TEXIMG(sp1DC)->s) - 1) << 2) & 0xFFF) << 0xC) | (((((temp_v0_6 + mat->height) - temp_v0_6) - 1) << 2) & 0xFFF));
            return;
        s1_508:                                 /* switch 1 */
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_t1_7 = (int *)( TEXIMG(sp1DC)->pixels);
            temp_a1_9 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1_9 + 1);
            temp_v0_4 = (char *)( ctx->gfx.cur->base + (temp_a1_9 * 8));
            GFXW(temp_v0_4)->w0 = 0xFD100000; GFXW(temp_v0_4)->w1 = (s32) TEXCI(temp_t1_7)->tlut;
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1_9 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1_9 + 1);
            temp_a0 = (char *)( ctx->gfx.cur->base + (temp_a1_9 * 8));
            GFXW(temp_a0)->w0 = 0xE8000000; GFXW(temp_a0)->w1 = 0;
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1_9 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1_9 + 1);
            temp_v0_4 = (char *)( ctx->gfx.cur->base + (temp_a1_9 * 8));
            GFXW(temp_v0_4)->w0 = 0xF5000100; GFXW(temp_v0_4)->w1 = 0x07000000;

            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1_9 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1_9 + 1);
            temp_a0 = (char *)( ctx->gfx.cur->base + (temp_a1_9 * 8));
            GFXW(temp_a0)->w0 = 0xE6000000; GFXW(temp_a0)->w1 = 0;
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1_9 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1_9 + 1);
            temp_t0_16 = (char *)( ctx->gfx.cur->base + (temp_a1_9 * 8));
            GFXW(temp_t0_16)->w0 = 0xF0000000; GFXW(temp_t0_16)->w1 = 0x073FC000;
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1_9 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1_9 + 1);
            temp_v0_4 = (char *)( ctx->gfx.cur->base + (temp_a1_9 * 8));
            GFXW(temp_v0_4)->w0 = 0xE7000000; GFXW(temp_v0_4)->w1 = 0;
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1_9 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1_9 + 1);
            temp_a0 = (char *)( ctx->gfx.cur->base + (temp_a1_9 * 8));
            GFXW(temp_a0)->w0 = 0xBA000E02; GFXW(temp_a0)->w1 = 0x8000;
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1_9 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1_9 + 1);
            sp118 = (char *)( ctx->gfx.cur->base + (temp_a1_9 * 8));
            GFXW(sp118)->w0 = (s32) (((TEXIMG(sp1DC)->width - 1) & 0xFFF) | 0xFD480000); GFXW(sp118)->w1 = gl_578b4_c34458(TEXCI(temp_t1_7)->texels, temp_a1_9, sp1DC, ctx);
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1_9 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1_9 + 1);
            temp_a0_6 = TEXIMG(sp1DC)->s;
            temp_t0_17 = (char *)( ctx->gfx.cur->base + (temp_a1_9 * 8));
            GFXW(temp_t0_17)->w0 = (s32) (((((s32) (((temp_a0_6 + mat->width) - temp_a0_6) + 7) >> 3) & 0x1FF) << 9) | 0xF5480000);
            temp_t2 = (sp1E0 & 3) << 0x12;
            temp_t3 = (sp1E8 & 0xF) << 0xE;
            temp_t4 = (sp1E4 & 3) << 8;
            temp_t5 = (sp1EC & 0xF) * 0x10;
            GFXW(temp_t0_17)->w1 = (s32) (0x07000000 | temp_t2 | temp_t3 | temp_t4 | temp_t5);
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1_9 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1_9 + 1);
            temp_v0_4 = (char *)( ctx->gfx.cur->base + (temp_a1_9 * 8));
            GFXW(temp_v0_4)->w0 = 0xE6000000; GFXW(temp_v0_4)->w1 = 0;
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1_9 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1_9 + 1);
            temp_a0 = (char *)( ctx->gfx.cur->base + (temp_a1_9 * 8));
            GFXW(temp_a0)->w0 = (s32) ((((TEXIMG(sp1DC)->s * 4) & 0xFFF) << 0xC) | 0xF4000000 | ((TEXIMG(sp1DC)->t * 4) & 0xFFF)); GFXW(temp_a0)->w1 = (s32) (0x07000000 | (((((TEXIMG(sp1DC)->s + mat->width) - 1) << 2) & 0xFFF) << 0xC) | ((((TEXIMG(sp1DC)->t + mat->height) - 1) << 2) & 0xFFF));
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1_9 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1_9 + 1);
            temp_v0_4 = (char *)( ctx->gfx.cur->base + (temp_a1_9 * 8));
            GFXW(temp_v0_4)->w0 = 0xE7000000; GFXW(temp_v0_4)->w1 = 0;
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1_9 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1_9 + 1);
            temp_a0_6 = TEXIMG(sp1DC)->s;
            temp_t0_18 = (char *)( ctx->gfx.cur->base + (temp_a1_9 * 8));
            GFXW(temp_t0_18)->w0 = (s32) (((((s32) (((TEXIMG(sp1DC)->s + mat->width) - TEXIMG(sp1DC)->s) + 7) >> 3) & 0x1FF) << 9) | 0xF5480000); GFXW(temp_t0_18)->w1 = (s32) (temp_t2 | temp_t3 | temp_t4 | temp_t5);
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1_9 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1_9 + 1);
            temp_t0_19 = (char *)( ctx->gfx.cur->base + (temp_a1_9 * 8));
            GFXW(temp_t0_19)->w0 = 0xF2000000;
            temp_a0_6 = TEXIMG(sp1DC)->s;
            temp_v0_6 = TEXIMG(sp1DC)->t;
            GFXW(temp_t0_19)->w1 = (s32) (((((((TEXIMG(sp1DC)->s + mat->width) - TEXIMG(sp1DC)->s) - 1) << 2) & 0xFFF) << 0xC) | (((((temp_v0_6 + mat->height) - temp_v0_6) - 1) << 2) & 0xFFF));
            return;
        s1_def:                                    /* switch 1 */
            gl_578b4_c34458((int *)(sStr_unkown_tile_219B8 + 0x219B8), temp_a1, sp1DC, ctx);
            return;
        }
    } else {
        temp_a1 = mat->format;
        /* edg-style source-order compare chain (switch 2) */
        if (temp_a1 == MAT_FMT_RGBA32) goto s2_120; if (temp_a1 == MAT_FMT_RGBA16) goto s2_110; if (temp_a1 == MAT_FMT_IA8) goto s2_408; if (temp_a1 == MAT_FMT_I8) goto s2_308; if (temp_a1 == MAT_FMT_I4) goto s2_304; if (temp_a1 != MAT_FMT_CI8) goto s2_def; goto s2_508;
        {
        s2_120:                                 /* switch 2 */
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1 + 1);
            spFC = (int *)( ctx->gfx.cur->base + (temp_a1 * 8));
            GFXW(spFC)->w0 = 0xFD180000; GFXW(spFC)->w1 = gl_578b4_c34458((s32)mat->texels, temp_a1, spFC, ctx);
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1 + 1);
            temp_ra_2 = tmem & 0x1FF;
            temp_a0 = (char *)( ctx->gfx.cur->base + (temp_a1 * 8));
            GFXW(temp_a0)->w0 = (s32) (temp_ra_2 | 0xF5180000);
            temp_t2 = (sp1E0 & 3) << 0x12;
            temp_t3 = (sp1E8 & 0xF) << 0xE;
            temp_t4 = (sp1E4 & 3) << 8;
            temp_t5 = (sp1EC & 0xF) * 0x10;
            GFXW(temp_a0)->w1 = (s32) (0x07000000 | temp_t2 | temp_t3 | ((mat->shiftT & 0xF) << 0xA) | temp_t4 | temp_t5 | (mat->shiftS & 0xF));
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1 + 1);
            temp_v0_4 = (char *)( ctx->gfx.cur->base + (temp_a1 * 8));
            GFXW(temp_v0_4)->w0 = 0xE6000000; GFXW(temp_v0_4)->w1 = 0;
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1 + 1);
            spF0 = (s32 *)( ctx->gfx.cur->base + (temp_a1 * 8));
            *spF0 = 0xF3000000;
            temp_a0_6 = (mat->width * mat->height) - 1;
            temp_t1 = (temp_a0_6 < 0x7FF) ? temp_a0_6 : 0x7FF;
            temp_v0_6 = (s32) (mat->width * 4) / 8;
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
            GFXW(spF0)->w1 = (s32) (((temp_t1 & 0xFFF) << 0xC) | 0x07000000 | (((s32) (var_t0 + 0x7FF) / temp_a1) & 0xFFF));
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1 + 1);
            temp_v0_4 = (char *)( ctx->gfx.cur->base + (temp_a1 * 8));
            GFXW(temp_v0_4)->w0 = 0xE7000000; GFXW(temp_v0_4)->w1 = 0;
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1 + 1);
            temp_a0 = (char *)( ctx->gfx.cur->base + (temp_a1 * 8));
            GFXW(temp_a0)->w0 = (s32) (0xF5180000 | ((((s32) ((mat->width * 2) + 7) >> 3) & 0x1FF) << 9) | temp_ra_2); GFXW(temp_a0)->w1 = (s32) (((mat->tile & 7) << 0x18) | temp_t2 | temp_t3 | ((mat->shiftT & 0xF) << 0xA) | temp_t4 | temp_t5 | (mat->shiftS & 0xF));
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1 + 1);
            temp_v0_4 = (char *)( ctx->gfx.cur->base + (temp_a1 * 8));
            GFXW(temp_v0_4)->w0 = 0xF2000000; GFXW(temp_v0_4)->w1 = (s32) (((mat->tile & 7) << 0x18) | ((((mat->width - 1) << 2) & 0xFFF) << 0xC) | (((mat->height - 1) << 2) & 0xFFF));
            return;
        s2_110:                                 /* switch 2 */
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1 + 1);
            spE0 = (int *)( ctx->gfx.cur->base + (temp_a1 * 8));
            GFXW(spE0)->w0 = 0xFD100000; GFXW(spE0)->w1 = gl_578b4_c34458((s32)mat->texels, temp_a1, spE0, ctx);
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1 + 1);
            temp_ra_3 = tmem & 0x1FF;
            temp_a0 = (char *)( ctx->gfx.cur->base + (temp_a1 * 8));
            GFXW(temp_a0)->w0 = (s32) (temp_ra_3 | 0xF5100000);
            temp_t2 = (sp1E0 & 3) << 0x12;
            temp_t3 = (sp1E8 & 0xF) << 0xE;
            temp_t4 = (sp1E4 & 3) << 8;
            temp_t5 = (sp1EC & 0xF) * 0x10;
            GFXW(temp_a0)->w1 = (s32) (temp_t2 | 0x07000000 | temp_t3 | temp_t4 | temp_t5);
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1 + 1);
            temp_t0_21 = (char *)( ctx->gfx.cur->base + (temp_a1 * 8));
            GFXW(temp_t0_21)->w0 = 0xE6000000; GFXW(temp_t0_21)->w1 = 0;
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1 + 1);
            spD4 = (s32 *)( ctx->gfx.cur->base + (temp_a1 * 8));
            *spD4 = 0xF3000000;
            temp_a0_6 = (mat->width * mat->height) - 1;
            temp_t1 = (temp_a0_6 < 0x7FF) ? temp_a0_6 : 0x7FF;
            temp_v0_6 = (s32) (mat->width * 2) / 8;
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
            GFXW(spD4)->w1 = (s32) (((temp_t1 & 0xFFF) << 0xC) | 0x07000000 | (((s32) (var_t0 + 0x7FF) / temp_a1) & 0xFFF));
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1 + 1);
            temp_v0_4 = (char *)( ctx->gfx.cur->base + (temp_a1 * 8));
            GFXW(temp_v0_4)->w0 = 0xE7000000; GFXW(temp_v0_4)->w1 = 0;
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1 + 1);
            temp_a0 = (char *)( ctx->gfx.cur->base + (temp_a1 * 8));
            GFXW(temp_a0)->w0 = (s32) (((((s32) ((mat->width * 2) + 7) >> 3) & 0x1FF) << 9) | 0xF5100000 | temp_ra_3); GFXW(temp_a0)->w1 = (s32) (((mat->tile & 7) << 0x18) | temp_t2 | temp_t3 | temp_t4 | temp_t5);
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1 + 1);
            temp_v0_4 = (char *)( ctx->gfx.cur->base + (temp_a1 * 8));
            GFXW(temp_v0_4)->w0 = 0xF2000000; GFXW(temp_v0_4)->w1 = (s32) (((mat->tile & 7) << 0x18) | ((((mat->width - 1) << 2) & 0xFFF) << 0xC) | (((mat->height - 1) << 2) & 0xFFF));
            return;
        s2_408:                                 /* switch 2 */
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1 + 1);
            spC4 = (int *)( ctx->gfx.cur->base + (temp_a1 * 8));
            GFXW(spC4)->w0 = 0xFD700000; GFXW(spC4)->w1 = gl_578b4_c34458((s32)mat->texels, temp_a1, spC4, ctx);
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1 + 1);
            temp_ra_4 = tmem & 0x1FF;
            temp_a0 = (char *)( ctx->gfx.cur->base + (temp_a1 * 8));
            GFXW(temp_a0)->w0 = (s32) (temp_ra_4 | 0xF5700000);
            temp_t2 = (sp1E0 & 3) << 0x12;
            temp_t3 = (sp1E8 & 0xF) << 0xE;
            temp_t4 = (sp1E4 & 3) << 8;
            temp_t5 = (sp1EC & 0xF) * 0x10;
            GFXW(temp_a0)->w1 = (s32) (0x07000000 | temp_t2 | temp_t3 | ((mat->shiftT & 0xF) << 0xA) | temp_t4 | temp_t5 | (mat->shiftS & 0xF));
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1 + 1);
            temp_v0_4 = (char *)( ctx->gfx.cur->base + (temp_a1 * 8));
            GFXW(temp_v0_4)->w0 = 0xE6000000; GFXW(temp_v0_4)->w1 = 0;
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1 + 1);
            spB8 = (s32 *)( ctx->gfx.cur->base + (temp_a1 * 8));
            *spB8 = 0xF3000000;
            temp_a0_6 = ((s32) ((mat->width * mat->height) + 1) >> 1) - 1;
            temp_t1 = (temp_a0_6 < 0x7FF) ? temp_a0_6 : 0x7FF;
            temp_v0_6 = mat->width / 8;
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
            GFXW(spB8)->w1 = (s32) (((temp_t1 & 0xFFF) << 0xC) | 0x07000000 | (((s32) (var_t0 + 0x7FF) / temp_a1) & 0xFFF));
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1 + 1);
            temp_v0_4 = (char *)( ctx->gfx.cur->base + (temp_a1 * 8));
            GFXW(temp_v0_4)->w0 = 0xE7000000; GFXW(temp_v0_4)->w1 = 0;
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1 + 1);
            temp_a0 = (char *)( ctx->gfx.cur->base + (temp_a1 * 8));
            GFXW(temp_a0)->w0 = (s32) (((((s32) (mat->width + 7) >> 3) & 0x1FF) << 9) | 0xF5680000 | temp_ra_4); GFXW(temp_a0)->w1 = (s32) (((mat->tile & 7) << 0x18) | temp_t2 | temp_t3 | ((mat->shiftT & 0xF) << 0xA) | temp_t4 | temp_t5 | (mat->shiftS & 0xF));
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1 + 1);
            temp_v0_4 = (char *)( ctx->gfx.cur->base + (temp_a1 * 8));
            GFXW(temp_v0_4)->w0 = 0xF2000000; GFXW(temp_v0_4)->w1 = (s32) (((mat->tile & 7) << 0x18) | ((((mat->width - 1) << 2) & 0xFFF) << 0xC) | (((mat->height - 1) << 2) & 0xFFF));
            return;
        s2_308:                                 /* switch 2 */
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1 + 1);
            spA8 = (int *)( ctx->gfx.cur->base + (temp_a1 * 8));
            GFXW(spA8)->w0 = 0xFD900000; GFXW(spA8)->w1 = gl_578b4_c34458((s32)mat->texels, temp_a1, spA8, ctx);
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1 + 1);
            temp_ra_5 = tmem & 0x1FF;
            temp_a0 = (char *)( ctx->gfx.cur->base + (temp_a1 * 8));
            GFXW(temp_a0)->w0 = (s32) (temp_ra_5 | 0xF5900000);
            temp_t2 = (sp1E0 & 3) << 0x12;
            temp_t3 = (sp1E8 & 0xF) << 0xE;
            temp_t4 = (sp1E4 & 3) << 8;
            temp_t5 = (sp1EC & 0xF) * 0x10;
            GFXW(temp_a0)->w1 = (s32) (0x07000000 | temp_t2 | temp_t3 | ((mat->shiftT & 0xF) << 0xA) | temp_t4 | temp_t5 | (mat->shiftS & 0xF));
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1 + 1);
            temp_v0_4 = (char *)( ctx->gfx.cur->base + (temp_a1 * 8));
            GFXW(temp_v0_4)->w0 = 0xE6000000; GFXW(temp_v0_4)->w1 = 0;
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1 + 1);
            sp9C = (s32 *)( ctx->gfx.cur->base + (temp_a1 * 8));
            *sp9C = 0xF3000000;
            temp_a0_6 = ((s32) ((mat->width * mat->height) + 1) >> 1) - 1;
            temp_t1 = (temp_a0_6 < 0x7FF) ? temp_a0_6 : 0x7FF;
            temp_v0_6 = mat->width / 8;
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
            GFXW(sp9C)->w1 = (s32) (((temp_t1 & 0xFFF) << 0xC) | 0x07000000 | (((s32) (var_t0 + 0x7FF) / temp_a1) & 0xFFF));
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1 + 1);
            temp_v0_4 = (char *)( ctx->gfx.cur->base + (temp_a1 * 8));
            GFXW(temp_v0_4)->w0 = 0xE7000000; GFXW(temp_v0_4)->w1 = 0;
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1 + 1);
            temp_a0 = (char *)( ctx->gfx.cur->base + (temp_a1 * 8));
            GFXW(temp_a0)->w0 = (s32) (0xF5880000 | ((((s32) (mat->width + 7) >> 3) & 0x1FF) << 9) | temp_ra_5); GFXW(temp_a0)->w1 = (s32) (((mat->tile & 7) << 0x18) | temp_t2 | temp_t3 | ((mat->shiftT & 0xF) << 0xA) | temp_t4 | temp_t5 | (mat->shiftS & 0xF));
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1 + 1);
            temp_v0_4 = (char *)( ctx->gfx.cur->base + (temp_a1 * 8));
            GFXW(temp_v0_4)->w0 = 0xF2000000; GFXW(temp_v0_4)->w1 = (s32) (((mat->tile & 7) << 0x18) | ((((mat->width - 1) << 2) & 0xFFF) << 0xC) | (((mat->height - 1) << 2) & 0xFFF));
            return;
        s2_304:                                 /* switch 2 */
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1 + 1);
            sp8C = (int *)( ctx->gfx.cur->base + (temp_a1 * 8));
            GFXW(sp8C)->w0 = 0xFD900000; GFXW(sp8C)->w1 = gl_578b4_c34458((s32)mat->texels, temp_a1, sp8C, ctx);
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1 + 1);
            temp_ra_6 = tmem & 0x1FF;
            temp_a0 = (char *)( ctx->gfx.cur->base + (temp_a1 * 8));
            GFXW(temp_a0)->w0 = (s32) (temp_ra_6 | 0xF5900000);
            temp_t2 = (sp1E0 & 3) << 0x12;
            temp_t3 = (sp1E8 & 0xF) << 0xE;
            temp_t4 = (sp1E4 & 3) << 8;
            temp_t5 = (sp1EC & 0xF) * 0x10;
            GFXW(temp_a0)->w1 = (s32) (0x07000000 | temp_t2 | temp_t3 | ((mat->shiftT & 0xF) << 0xA) | temp_t4 | temp_t5 | (mat->shiftS & 0xF));
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1 + 1);
            temp_v0_4 = (char *)( ctx->gfx.cur->base + (temp_a1 * 8));
            GFXW(temp_v0_4)->w0 = 0xE6000000; GFXW(temp_v0_4)->w1 = 0;
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1 + 1);
            sp80 = (s32 *)( ctx->gfx.cur->base + (temp_a1 * 8));
            *sp80 = 0xF3000000;
            temp_a0_6 = ((s32) ((mat->width * mat->height) + 3) >> 2) - 1;
            temp_t1 = (temp_a0_6 < 0x7FF) ? temp_a0_6 : 0x7FF;
            temp_v0_6 = mat->width / 16;
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
            GFXW(sp80)->w1 = (s32) (((temp_t1 & 0xFFF) << 0xC) | 0x07000000 | (((s32) (var_t0 + 0x7FF) / temp_a1) & 0xFFF));
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1 + 1);
            temp_v0_4 = (char *)( ctx->gfx.cur->base + (temp_a1 * 8));
            GFXW(temp_v0_4)->w0 = 0xE7000000; GFXW(temp_v0_4)->w1 = 0;
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1 + 1);
            temp_a0 = (char *)( ctx->gfx.cur->base + (temp_a1 * 8));
            GFXW(temp_a0)->w0 = (s32) (((((s32) (((s16) mat->width >> 1) + 7) >> 3) & 0x1FF) << 9) | 0xF5800000 | temp_ra_6); GFXW(temp_a0)->w1 = (s32) (((mat->tile & 7) << 0x18) | temp_t2 | temp_t3 | ((mat->shiftT & 0xF) << 0xA) | temp_t4 | temp_t5 | (mat->shiftS & 0xF));
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1 + 1);
            temp_v0_4 = (char *)( ctx->gfx.cur->base + (temp_a1 * 8));
            GFXW(temp_v0_4)->w0 = 0xF2000000; GFXW(temp_v0_4)->w1 = (s32) (((mat->tile & 7) << 0x18) | ((((mat->width - 1) << 2) & 0xFFF) << 0xC) | (((mat->height - 1) << 2) & 0xFFF));
            return;
        s2_508:                                 /* switch 2 */
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1 = DLBUF(temp_v1)->count;
            temp_t1_7 = (int *)( TEXIMG(sp1DC)->pixels);
            DLBUF(temp_v1)->count = (s32) (temp_a1 + 1);
            temp_v0_4 = (char *)( ctx->gfx.cur->base + (temp_a1 * 8));
            GFXW(temp_v0_4)->w0 = 0xFD100000; GFXW(temp_v0_4)->w1 = (s32) TEXCI(temp_t1_7)->tlut;
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1 + 1);
            temp_a0 = (char *)( ctx->gfx.cur->base + (temp_a1 * 8));
            GFXW(temp_a0)->w0 = 0xE8000000; GFXW(temp_a0)->w1 = 0;
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1 + 1);
            temp_v0_4 = (char *)( ctx->gfx.cur->base + (temp_a1 * 8));
            GFXW(temp_v0_4)->w0 = 0xF5000100; GFXW(temp_v0_4)->w1 = 0x07000000;

            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1 + 1);
            temp_a0 = (char *)( ctx->gfx.cur->base + (temp_a1 * 8));
            GFXW(temp_a0)->w0 = 0xE6000000; GFXW(temp_a0)->w1 = 0;
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1 + 1);
            temp_t0_25 = (char *)( ctx->gfx.cur->base + (temp_a1 * 8));
            GFXW(temp_t0_25)->w0 = 0xF0000000; GFXW(temp_t0_25)->w1 = 0x073FC000;
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1 + 1);
            temp_v0_4 = (char *)( ctx->gfx.cur->base + (temp_a1 * 8));
            GFXW(temp_v0_4)->w0 = 0xE7000000; GFXW(temp_v0_4)->w1 = 0;
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1 + 1);
            temp_t0_25 = (char *)( ctx->gfx.cur->base + (temp_a1 * 8));
            GFXW(temp_t0_25)->w0 = 0xBA000E02; GFXW(temp_t0_25)->w1 = 0x8000;
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1 + 1);
            sp50 = (s32)( ctx->gfx.cur->base + (temp_a1 * 8));
            if (1) { GFXW(sp50)->w0 = 0xFD500000; }
            
            GFXW(sp50)->w1 = gl_578b4_c34458(TEXCI(temp_t1_7)->texels, temp_a1, sp1DC, ctx);
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1 + 1);
            /* RAW: donor-mirror: C++ donor (NCC Cfront); its local mirror structs cannot include the C headers */
            temp_ra_8 = tmem + 0 * FW(mat, 0x0); sp50 = (temp_ra_8 & 0x1FF) + 0 * FW(mat, 0x4);
            temp_a0 = (char *)( ctx->gfx.cur->base + (temp_a1 * 8));
            GFXW(temp_a0)->w0 = (s32) (sp50 | 0xF5500000);
            temp_t2 = (sp1E0 & 3) << 0x12;
            temp_t3 = (sp1E8 & 0xF) << 0xE;
            temp_t4 = (sp1E4 & 3) << 8;
            temp_t5 = (sp1EC & 0xF) * 0x10;
            GFXW(temp_a0)->w1 = (s32) (0x07000000 | temp_t2 | temp_t3 | ((mat->shiftT & 0xF) << 0xA) | temp_t4 | temp_t5 | (mat->shiftS & 0xF));
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1 + 1);
            sp20 = sp50;
            temp_v0_4 = (char *)( ctx->gfx.cur->base + (temp_a1 * 8));
            GFXW(temp_v0_4)->w0 = 0xE6000000; GFXW(temp_v0_4)->w1 = 0;
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1 + 1);
            sp44 = (s32 *)( ctx->gfx.cur->base + (temp_a1 * 8));
            *sp44 = 0xF3000000;
            temp_a0_6 = TEXIMG(sp1DC)->width; temp_t0_27 = ((s32) ((temp_a0_6 * TEXIMG(sp1DC)->height) + 1) >> 1) - 1;
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
            GFXW(sp44)->w1 = (s32) (((temp_t1 & 0xFFF) << 0xC) | 0x07000000 | (((s32) (var_t0 + 0x7FF) / temp_a1) & 0xFFF));
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1 + 1);
            temp_v0_4 = (char *)( ctx->gfx.cur->base + (temp_a1 * 8));
            GFXW(temp_v0_4)->w0 = 0xE7000000; GFXW(temp_v0_4)->w1 = 0;
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1 + 1);
            temp_a0 = (char *)( ctx->gfx.cur->base + (temp_a1 * 8));
            GFXW(temp_a0)->w0 = (s32) (((((s32) (TEXIMG(sp1DC)->width + 7) >> 3) & 0x1FF) << 9) | 0xF5480000 | sp50); GFXW(temp_a0)->w1 = (s32) (((mat->tile & 7) << 0x18) | temp_t2 | temp_t3 | ((mat->shiftT & 0xF) << 0xA) | temp_t4 | temp_t5 | (mat->shiftS & 0xF));
            temp_v1 = (char *)( (s32)ctx->gfx.cur);
            temp_a1 = DLBUF(temp_v1)->count;
            DLBUF(temp_v1)->count = (s32) (temp_a1 + 1);
            temp_v0_4 = (char *)( ctx->gfx.cur->base + (temp_a1 * 8));
            GFXW(temp_v0_4)->w0 = 0xF2000000; GFXW(temp_v0_4)->w1 = (s32) (((mat->tile & 7) << 0x18) | ((((TEXIMG(sp1DC)->width - 1) << 2) & 0xFFF) << 0xC) | (((TEXIMG(sp1DC)->height - 1) << 2) & 0xFFF));
            if (0) { gl_578b4_c34458((s32 *)&sp34); }
            temp_a1 = sp34 = mat->format;
        s2_def:                                    /* switch 2 */
            gl_578b4_c34458((int *)(sFmt_bad_texfmt_num_219C4 + 0x219C4), temp_a1);
            return;
        }
    }
}
