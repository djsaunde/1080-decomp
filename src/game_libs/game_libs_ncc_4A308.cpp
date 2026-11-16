/* IDO 7.1 NCC (C++) donor for PolyShape_vslot12_0004A308 -- EXACT (agent-c run 219), REPLACE_FUNC_BODY-spliced over the empty
 * placeholder in src/game_libs/game_libs_post0b.c in both build trees.
 * History (NM donor game_libs_ncc_4A308_nm.cpp): the NM C body lifted under NCC (agent-c run 134, cfe 76.89);
 * run 178 (kit CXXIV de-naming) `v60` and run 179 `t2` as macros re-read per use; agent-g run 190: declarations
 * split from their assignments in the target's top-down slot order, the tail as an early `return`, the 0x20030
 * debug print passes the byte sp74[0]; agent-g run 198 (kit CLXIII): a folded `+ 0 * (int)v60` on the entry
 * expression puts the 0x60 load's candidate ahead of entry (v60 v0 / entry v1).
 * Run 219 (kit CCXVI): `entry` is an EXPRESSION (macro), not a named local. As a CSE temp that lives across the
 * pal if / else and the calls it takes a ugen temp-area word below the spill, which is what the target's frame
 * has (128, arg3*8 spill at sp+60); `entryd = 0` keeps the named home the target also has at frame-24. With the
 * named entry the body was word-exact but the frame was 120 (or 128 with the spill one word low, sp+56).
 */
typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef unsigned short u16;
typedef int s32; typedef unsigned int u32; typedef long long s64; typedef unsigned long long u64;
typedef float f32; typedef double f64;
#include "../../include/gdl/gl_data.h"  /* object aliases (NCC donors build without -I include) */
#include "../../include/uso_call.h"  /* NCC donors build without -I include */
#include "../../include/gdl/polyshape.h"
/* arg0 is a PolyShape (gdl/polyshape.h): verts / vtxColours / alphas. unk6C is a table of 8-byte
 * records whose u16 pair at +4 / +6 is a vertex-index pair (the two ends this slot builds
 * vertices for); the first word is not read here. */
struct PolySeg { s32 unk0; u16 v[2]; };
#define P BASEPOLY(arg0)
extern "C" {
extern int D_00000000;
extern int gl_func_00000000(...);
}
extern "C" s32 PolyShape_vslot12_0004A308(char *arg0, int arg1, int arg2, int arg3, char *arg4) {
    char *sp7C;
    char *sp78;
    unsigned char sp74[4];
    unsigned char sp70[4];
    short sp6E, sp6C;
    char *entryd = 0; /* run 219: the home of the named entry (frame-24), kept */
    unsigned char sp58[0x10];
    unsigned char sp48[0x10];
    int t0;
#define pal (P->alphas)
#define entry ((char *)P->unk6C + arg3 * 8 + 0 * (int)P->verts) /* run 219: expression, not a local */
#define v60 ((char *)P->verts)
    t0 = ((PolySeg *)entry)->v[0];
#define t2 (((PolySeg *)entry)->v[1])
    sp7C = v60 + t0 * 6;
    sp78 = v60 + t2 * 6;
    if (pal != 0) {
        sp6C = pal[t0];
        sp6E = pal[t2];
    } else {
        sp6E = 0xFF;
        sp6C = 0xFF;
    }
    if (P->vtxColours != 0) {
        CALL(gfx_copyBytes3_00046DA0)(&gGfxLib,sp74, P->vtxColours + t0);
        CALL(gfx_copyBytes3_00046DA0)(&gGfxLib,sp70, P->vtxColours + ((PolySeg *)P->unk6C)[arg3].v[1]);
        CALL(dbg_printf)((char *)&D_00000000 + 0x20030, sp74[0], sp74[1], sp74[2]);
    } else {
        unsigned char col[4];
        /* RAW: donor-mirror: C++ donor (NCC Cfront); its local mirror structs cannot include the C headers */
        col[0] = (u32)(*(float *)(arg4 + 0) * 255.0f);
        col[1] = (u32)(*(float *)(arg4 + 4) * 255.0f); /* RAW: donor-mirror (as above) */
        col[2] = (u32)(*(float *)(arg4 + 8) * 255.0f); /* RAW: donor-mirror (as above) */
        CALL(gfx_copyBytes3_00046DA0)(&gGfxLib,sp74, col);
        CALL(gfx_copyBytes3_00046DA0)(&gGfxLib,sp70, col);
    }
    CALL(gfx_makeVtx_00048104)(&gGfxLib,sp48, sp7C, 0, (s32) sp6C, sp74);
    CALL(gfx_makeVtx_00048104)(&gGfxLib,sp58, sp78, 0, (s32) sp6E, sp70);
    if (arg1 != 0) {
        return CALL(gfx_findFaceVtxs_0004880C)(arg2, sp48);
    }
    return 0;
}
