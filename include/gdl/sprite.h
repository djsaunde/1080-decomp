#ifndef GDL_SPRITE_H
#define GDL_SPRITE_H

#include "node.h" /* same directory: the NCC donors include this header by relative path */

/* libgdl Sprite (0xD8, Sprite_ctor 0x4B620; docs/1080_GRAPHICS.md).
 *
 * The 0xCC base is a "sprite" node (scale 1.0 at +0xB0 / +0xB4, offsets
 * zeroed). Sprite adds its own "SS StartDL" display list (+0xCC) that the
 * ctor fills once with the 2D state: G_RDPPIPESYNC, G_SETOTHERMODE_H
 * cycle type (0xBA001402, 1-cycle), G_SETOTHERMODE_L z source
 * (0xB9000201 / 4 = G_ZS_PRIM), texture filter bilerp, texture persp off and
 * G_TEXTURE on (0x80008000); "SS EndDL" (+0xD0) restores G_TP_PERSP,
 * G_ZS_PIXEL and G_TEXTURE off. Sprite_display draws through
 * gfx_emitStateAndPrimColour. The ctor also gives every element of the child
 * collection at +0x84 its own "sprblock" list (element +0x64). */

typedef struct Sprite {
    /* 0x00 */ Node node;
    /* 0x2C */ s32 unk2C[0x16];
    /* 0x84 */ void **children;       /* element array (the ctor's Arr: data here, count at +0x90),
                                     * walked by the ctor; element i is the image for unkC0 == i */
    /* 0x88 */ s32 unk88[10];
    /* 0xB0 */ f32 scaleX;            /* 1.0 */
    /* 0xB4 */ f32 scaleY;            /* 1.0 */
    /* 0xB8 */ f32 offsetX;           /* Sprite_display: screen offset added after centring */
    /* 0xBC */ f32 offsetY;
    /* 0xC0 */ s16 unkC0;             /* image index into the +0x84 list (Sprite_display) */
    /* 0xC2 */ s16 padC2;
    /* 0xC4 */ s32 perspective;       /* Sprite_display: != 0 scales the size by the projected depth */
    /* 0xC8 */ f32 depthBias;         /* Sprite_display: added to the G_SETPRIMDEPTH z */
    /* 0xCC */ struct DLContext *startDL; /* "SS StartDL" (0x14 Gfx) */
    /* 0xD0 */ struct DLContext *endDL;   /* "SS EndDL" (0xA Gfx): restores persp, z source, texture off */
    /* 0xD4 */ s32 unkD4;
} Sprite;

#ifndef __cplusplus /* edg (NCC donors): &((T *)0)->f is not an integral constant expression */
typedef char Sprite_size_check[sizeof(Sprite) == 0xD8 ? 1 : -1];
typedef char Sprite_scaleX_check[(int)&((Sprite *)0)->scaleX == 0xB0 ? 1 : -1];
typedef char Sprite_startDL_check[(int)&((Sprite *)0)->startDL == 0xCC ? 1 : -1];
#endif

#endif /* GDL_SPRITE_H */
