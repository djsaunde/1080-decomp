/* IDO 7.1 NCC (C++) donor for PolyShape_shade_00049DBC: three textured-vertex builds from one
 * 8-byte index record (vertex lookups, optional 0x7C alpha table, colour source by
 * flag: per-vertex call / 0x54 table / 0x50 table / the arg4 float RGB * 255).
 * Spliced into game_libs_post0b.c.o via REPLACE_FUNC_BODY.
 * agent-c run 153: NM donor 97.4 -> EXACT. The arg1 path is a TWO-argument call
 * (its `lui a2` is the else-arm's hoisted address in the beqz delay slot); the else
 * call takes two DISTINCT base-0 symbols (a2 / a3 each get their own lui/addiu);
 * no empty `if (arg2) {}` test; and NO named scalar locals (each one takes a frame
 * home below the arrays: dropping temp_s1 / temp_v0_2 / temp_v1 / base and widening
 * the colour scratch to u8[24] puts sp38 at the target's sp+0x38). */
typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef unsigned short u16;
typedef int s32; typedef unsigned int u32; typedef long long s64; typedef unsigned long long u64;
typedef float f32; typedef double f64;
#undef NULL
#define NULL 0
#include "../../include/gdl/gl_data.h"  /* object aliases (NCC donors build without -I include) */
#include "../../include/uso_call.h"  /* NCC donors build without -I include */
#include "../../include/gdl/polyshape.h"
#include "../../include/gdl/material.h"
#include "../../include/gdl/gfxlib.h"
extern "C" {
extern int D_00000000;
extern int gl_func_00000000(...);
extern void gl_func_00000000_3cbb4m(f32);
extern int arg3;
extern int arg4;
extern int arg1;
extern void gl_func_00000000_49DBCv(...);
extern char gMorphPolyShade_49dbc_t2[];
extern char gl_d_49dbc_t3[];
}
extern "C" u8 *PolyShape_shade_00049DBC(BasePolyShape *shape, s32 arg1, s32 arg2, s32 tri, BaseMat *mat) {
    void *spAC;
    void *spA8;
    void *spA4;
    void *spA0;
    void *sp9C;
    void *sp98;
    s32 sp94;
    s32 sp90;
    s32 sp8C;
    s16 sp8A;
    s16 sp88;
    s16 sp86;
    s16 sp84;
#ifdef PORT /* plan 18.21: the three Vtx are read as ONE array (PolyShape_addFace / gfx_findFaceVtxs walk
            * &sp50[0..2]); IDO lays them out contiguously, a host compiler need not (the spike polygons) */
    long long spVtx[3][2];
#define sp50 (spVtx[0])
#define sp60 (spVtx[1])
#define sp70 (spVtx[2])
#else
    long long sp70[2];
    long long sp60[2];
    long long sp50[2];
#endif
    u8 sp38[24];
    s32 var_a0;

    spAC = (void *)CALL(PolyShape_getVertex)(shape, shape->tris[tri].v[0]);
    spA8 = (void *)CALL(PolyShape_getVertex)(shape, shape->tris[tri].v[1]);
    spA4 = (void *)CALL(PolyShape_getVertex)(shape, shape->tris[tri].v[2]);
    var_a0 = mat->geomFlags;
    if ((var_a0 & 0x400000) && !(var_a0 & 0x100000)) {
        spA0 = (void *)CALL(PolyShape_getVertexRef_0005318C)(shape, shape->tris[tri].v[0]);
        sp9C = (void *)CALL(PolyShape_getVertexRef_0005318C)(shape, shape->tris[tri].v[1]);
        sp98 = (void *)CALL(PolyShape_getVertexRef_0005318C)(shape, shape->tris[tri].v[2]);
        var_a0 = mat->geomFlags;
    } else {
        sp98 = NULL;
        sp9C = NULL;
        spA0 = NULL;
    }
    if (shape->alphas != 0) {
        sp84 = shape->alphas[shape->tris[tri].v[0]];
        sp86 = shape->alphas[shape->tris[tri].v[1]];
        sp88 = shape->alphas[shape->tris[tri].v[2]];
    } else {
        sp88 = 0xFF;
        sp8A = 0;
        sp86 = 0xFF;
        sp84 = 0xFF;
    }
    if (var_a0 & 0x2000) {
        if (shape->shapeFlags & 0x8000) {
            CALL(PolyShape_getNormal8)(shape, shape->tris[tri].v[0], &sp94, mat);
            CALL(PolyShape_getNormal8)(shape, shape->tris[tri].v[1], &sp90);
            CALL(PolyShape_getNormal8)(shape, shape->tris[tri].v[2], &sp8C);
        } else {
            if (shape->faceNormals == 0) {
                XCALL(gl_func_00000000_49DBCv, sys_fatal)((char *)&D_00000000 + 0x20014);
            }
            XCALL(gl_func_00000000_49DBCv, gfx_floatToS8x3_00046D4C)(gGfx, &sp94, (s32)shape->faceNormals + tri * 0xC);
            *(u8 *)&sp8C = *(u8 *)&sp94;
            *(u8 *)&sp90 = *(u8 *)&sp94;
        }
    } else {
        if (shape->vtxColours != 0) {
            CALL(gfx_copyBytes3_00046DA0)(gGfx,&sp94, &shape->vtxColours[shape->tris[tri].v[0]]);
            CALL(gfx_copyBytes3_00046DA0)(gGfx,&sp90, &shape->vtxColours[shape->tris[tri].v[1]]);
            CALL(gfx_copyBytes3_00046DA0)(gGfx,&sp8C, &shape->vtxColours[shape->tris[tri].v[2]]);
        } else {
            sp38[0] = (u8)(s32)(u32)(mat->colour[0] * 255.0f);
            sp38[1] = (s8)(s32)(u32)(mat->colour[1] * 255.0f);
            sp38[2] = (s8)(s32)(u32)(mat->colour[2] * 255.0f);
            CALL(gfx_copyBytes3_00046DA0)(gGfx,&sp94, sp38);
            CALL(gfx_copyBytes3_00046DA0)(gGfx,&sp90, sp38);
            CALL(gfx_copyBytes3_00046DA0)(gGfx,&sp8C, sp38);
        }
    }
    CALL(gfx_makeVtx_00048104)(gGfx,&sp50, spAC, spA0, (s32) sp84, &sp94);
    CALL(gfx_makeVtx_00048104)(gGfx,&sp60, spA8, sp9C, (s32) sp86, &sp90);
    CALL(gfx_makeVtx_00048104)(gGfx,&sp70, spA4, sp98, (s32) sp88, &sp8C);
    if (arg1 != 0) {
        return (u8 *)CALL(gfx_findFaceVtxs_0004880C)(arg2, &sp50);
    }
    CALL(PolyShape_addFace_00048510)(arg2, &sp50, gMorphPolyShade_49dbc_t2, gl_d_49dbc_t3, &shape->tris[tri].v[0]);
    return NULL;
}
