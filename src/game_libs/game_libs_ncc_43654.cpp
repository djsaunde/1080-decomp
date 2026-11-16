#include "../../include/gdl/gl_data.h" /* NCC has no -I include: relative path */
/* IDO 7.1 NCC (C++) donor for PolyShape_buildStateDL_00043654 (0x16C / 91 words): the cfe NM body
 * compiled as C++, plus a dead `if (!sp34) {}` between the two stores of the
 * else-branch display-list append (permuter find, agent-c run 120): it moves the
 * append pointer from v1 to the target's a1. Spliced into game_libs_post0b.c.o via
 * REPLACE_FUNC_BODY. */
typedef int s32; typedef short s16; typedef unsigned int u32; typedef unsigned char u8; typedef float f32;
#include "../../include/gdl/gfxlib.h"   /* gGfx->staticDL */
#include "../../include/gdl/dlctx.h"    /* DLContext / DLBuf */
#include "../../include/gdl/material.h" /* Mat */
#define FW(p, o) (*(int *)((char *)(p) + (o)))
extern "C" {
extern char D_00000000;
int gl_func_00000000(...);
void PolyShape_buildStateDL_00043654(Mat *mat, char *arg1) {
    char *sp34;
    char *var_a3;
    char *temp_v0;
    char *temp_a0;
    char *temp_a0_2;
    char *temp_a1;
    char *temp_a2;
    s32 idx;
    s32 idx2;

    mat->stateDL = gGfx->staticDL;
    var_a3 = (char *) mat->stateDL;
    mat->stateStart = mat->stateDL->gfx.cur->count;
    if (mat->base.unk48 == 0) {
        gl_func_00000000(mat->stateDL, arg1);
        gl_func_00000000(mat->stateDL, arg1);
        gl_func_00000000(mat->stateDL, arg1);
        gl_func_00000000(mat->stateDL, arg1);
        var_a3 = (char *) mat->stateDL;
    }
    if (mat->base.geomFlags & MAT_GEOM_TEXTURED) {
        gl_func_00000000((char *)&gGfxLib, var_a3, mat);
        if (mat->base.geomFlags & MAT_GEOM_TEXGEN) {
            mat->texgenDL = (DLContext *) gl_func_00000000(0, GL_DATA(0x1FD1C), 0xA, 0, 5, 0, 0);
            gl_func_00000000(mat->texgenDL); if (1) {}
            temp_v0 = (char *) mat->texgenDL;
            sp34 = (char *) mat->stateDL;
            temp_v0 = (char *) gl_func_00000000(((DLContext *)temp_v0)->frameStart);
            temp_a0 = (char *) ((DLContext *)sp34)->gfx.cur;
            idx = ((DLBuf *)temp_a0)->count; if (1) {}
            ((DLBuf *)temp_a0)->count = idx + 1;
            temp_a2 = (char *) ((DLContext *)sp34)->gfx.cur->base + (idx * 8); if (1) {}
            DLPKT(temp_a2)->w0 = 0x06000000; /* G_DL to the texgen list */
            DLPKT(temp_a2)->w1 = (int) temp_v0;
        }
        gl_func_00000000(mat->stateDL);
    } else {
        temp_a0_2 = (char *) ((DLContext *)var_a3)->gfx.cur; if (1) {}
        idx2 = ((DLBuf *)temp_a0_2)->count;
        ((DLBuf *)temp_a0_2)->count = idx2 + 1;
        temp_a1 = (char *) ((DLContext *)var_a3)->gfx.cur->base + (idx2 * 8);
        DLPKT(temp_a1)->w0 = 0xBB040000; /* G_TEXTURE off */
        if (!sp34) {}
        DLPKT(temp_a1)->w1 = -1;
    }
    gl_func_00000000(mat->stateDL);
}
}
