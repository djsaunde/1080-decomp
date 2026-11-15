#ifndef GDL_LIGHT_H
#define GDL_LIGHT_H

#include "gdl/node.h"

/* libgdl lights (game_libs post0b 0x51448 / 0x546E8-0x54C6C; docs/1080_GRAPHICS.md).
 *
 * BaseLight (0x138, BaseLight_ctor 0x546E8) is built on a 0xB4-byte MNode-style
 * base (matrix node with a position sub-object at +0x30) and adds a light
 * colour parm group ("Parms" / "col", a Quad colour whose default is
 * {1, 1, 1, 1}) at +0x10C. The GfxLib light list (gfx +0x18C) links these;
 * gfx_updateLights / gfx_selectLights / gfx_setLightDirs walk it:
 *   - +0xC4 flags: bit 0x80 = skip (ambient / disabled), 2 and 8 set by the ctor,
 *     2 toggled by gfx_selectLights for lights in the target's mask;
 *   - +0xE4 world direction, +0xF0 transformed direction (gfx_setLightDirs
 *     picks one, scaled by 120 into the Light_t dir bytes);
 *   - +0xD0 light mask (0xFFFF), matched against a LightTarget's mask.
 *
 * LightTarget (0x16C, shape_cmdLightTarget 0x51448, name "lighttarget") is the node
 * a shape uses to pick lights: LightTarget_display calls gfx_selectLights with
 * its position, mask (+0x120) and scale (+0x124). */

typedef struct BaseLight {
    /* 0x000 */ Node node;
    /* 0x02C */ s32 unk2C;
    /* 0x030 */ s32 unk30[0x21];      /* MNode-style base: position / matrix (0x30-0xB4) */
    /* 0x0B4 */ f32 intensity;        /* 1.0 */
    /* 0x0B8 */ f32 unkB8[3];         /* 0 */
    /* 0x0C4 */ s32 flags;            /* 0x80 skip; ctor sets 2 | 8 */
    /* 0x0C8 */ f32 unkC8;
    /* 0x0CC */ f32 unkCC;
    /* 0x0D0 */ s32 mask;             /* 0xFFFF */
    /* 0x0D4 */ s32 unkD4[3];
    /* 0x0E0 */ s32 dirty;            /* gfx_updateLights: 1 when the position moved */
    /* 0x0E4 */ f32 dir[3];           /* world direction */
    /* 0x0F0 */ f32 xdir[3];          /* transformed direction */
    /* 0x0FC */ f32 unkFC;            /* 1.0 */
    /* 0x100 */ f32 unk100[3];        /* 0 */
    /* 0x10C */ s32 parms[0xB];       /* colour parm group (0x2C) */
} BaseLight;

typedef struct LightTarget {
    /* 0x000 */ Node node;
    /* 0x02C */ s32 unk2C[0x37];
    /* 0x108 */ f32 unk108[3];        /* RoData constant (+0x1BB0) */
    /* 0x114 */ s32 unk114[3];
    /* 0x120 */ s32 mask;             /* 0xFFFF */
    /* 0x124 */ f32 scale;            /* 1.0 */
    /* 0x128 */ s32 unk128[0x11];
} LightTarget;

typedef char BaseLight_size_check[sizeof(BaseLight) == 0x138 ? 1 : -1];
typedef char BaseLight_flags_check[(int)&((BaseLight *)0)->flags == 0xC4 ? 1 : -1];
typedef char BaseLight_dir_check[(int)&((BaseLight *)0)->dir == 0xE4 ? 1 : -1];
typedef char BaseLight_parms_check[(int)&((BaseLight *)0)->parms == 0x10C ? 1 : -1];
typedef char LightTarget_size_check[sizeof(LightTarget) == 0x16C ? 1 : -1];
typedef char LightTarget_mask_check[(int)&((LightTarget *)0)->mask == 0x120 ? 1 : -1];

#endif /* GDL_LIGHT_H */
