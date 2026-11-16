#include "../../include/uso_call.h"  /* NCC donors build without -I include */
/* IDO 7.1 NCC (C++) donor for LensFlareElem_draw_0000B750 (0x134 / 77 words).
 *
 * Camera/basis update: f0 = src->0x00 (+ k * 400 when flag 4), where k is
 * the stack float argument, replaced by 1.0 unless flag 8. Writes
 * a2 + a3 * f0 to the translation row of the GfxLib model-view matrix
 * (modelView[3][0..2]; import_8005C1A8 is gGfxLib, its token named for the
 * +0xA0 site address 0x8005C108 + 0xA0), copies src->0x18 to
 * obj->0xC0 (short), and if src->0x14 (* k when flag 2) exceeds the double
 * at game_uso_D_807FFA78 + 0x158, stores it to obj->0xB0/0xB4, builds a
 * 4-float temporary (src->0x04, 0x08, 0x0C, src->0x10 * k), passes it to
 * 05B750(&gGfxLib_8005C108, &q) and calls obj's third virtual (vptr at +0x28).
 *
 * C++ facts this body depends on (cfe capped at 72.8%):
 *  - k IS the parameter (`arg4 = 1.0f`): the param is re-loaded from its
 *    stack home into its own register on each path (two `lwc1 f16,104(sp)`),
 *    and there is one less local slot.
 *  - The temporary is a Q whose constructor forwards to an inline set();
 *    the nested inline argument temps give the target's frame (88, q at
 *    sp+64) and the FP colouring (x f0, y f2, z f12, w f14). A plain
 *    4-arg constructor gives frame 80 and y/z/w in f12/f14/f16.
 *  - GFXLIB(&import_8005C1A8)->modelView[3][i] keeps the target's
 *    `lui/addiu sym+0xA0` base + 0x30..0x38 stores (agent-g run 570; a
 *    float * / GfxLib * local folds the base to +0xD0 / +0).
 * Spliced into game_uso.c.o via REPLACE_FUNC_BODY
 * (docs/MATCHING_WORKFLOW.md#game-uso-ncc-cplusplus-donor-recipe). Every
 * symbol is extern "C". */
typedef int s32; typedef unsigned short u16; typedef float f32; typedef double f64;
typedef short s16; typedef unsigned char u8; typedef unsigned int u32;
#include "../../include/game/sun.h" /* NCC has no -I include */
#include "../../include/game/game_uso_data.h"
#include "../../include/gdl/gfxlib.h"
#ifdef PORT /* cfront layout: explicit vptr, virtuals as slot calls (port_cfront.h, plan 17.8) */
#include "port_cfront.h"
struct V { char pad[40];
    char *vptr; /* cfront vptr */
    CF_V0(void, m0, 0) CF_V0(void, m1, 1) CF_V0(void, m2, 2)
};
#else
struct V { char pad[40]; virtual void m0(); virtual void m1(); virtual void m2(); };
#endif
struct Q {
    float x, y, z, w;
    void set(float a, float b, float c, float d) { x = a; y = b; z = c; w = d; }
    Q(float a, float b, float c, float d) { set(a, b, c, d); }
};
extern "C" {
extern char gGfxLib_8005C108;
extern char import_8005C1A8;
extern char game_uso_D_807FFA78;
void game_uso_func_05B750(void *, Q *);
}
#define AT(T,b,o) (*(T *)((char *)(b) + (o)))
/* elem: the LensFlareElem; sprite: the flare's Sprite (LensFlare.sprite); sunPos / dir: the sun's world position
 * and the unit direction the elements are laid along; fade: the alignment fade (LensFlare_display) */
extern "C" void LensFlareElem_draw_0000B750(LensFlareElem *elem, Sprite *sprite, float *sunPos, float *dir, float fade) {
    float f0 = elem->pos;
    float v;
    if (elem->flags & LFELEM_FLAG_SLIDE) f0 += fade * 400.0f;
    if (!(elem->flags & LFELEM_FLAG_FADE)) fade = 1.0f;
    GFXLIB(&import_8005C1A8)->modelView[3][0] = sunPos[0] + dir[0] * f0;
    GFXLIB(&import_8005C1A8)->modelView[3][1] = sunPos[1] + dir[1] * f0;
    GFXLIB(&import_8005C1A8)->modelView[3][2] = sunPos[2] + dir[2] * f0;
    sprite->unkC0 = elem->image;
    v = elem->size;
    if (elem->flags & LFELEM_FLAG_SIZE) v *= fade;
    if ((double)v > AT(double, &game_uso_D_807FFA78, kRoD_0p1_158)) {
        sprite->scaleX = v;
        sprite->scaleY = v;
        Q q(elem->color[0], elem->color[1], elem->color[2], elem->color[3] * fade);
        XCALL(game_uso_func_05B750, gfx_setPrimColour)(&gGfxLib_8005C108, &q);
        ((V *)sprite)->m2(); /* the sprite's third virtual: draw */
    }
}
