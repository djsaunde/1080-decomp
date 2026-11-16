#ifndef GDL_LIGHT_H
#define GDL_LIGHT_H

#include "gdl/node.h"
#include "gdl/parm.h"

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

/* BaseLight +0x10C "Parms" group (0x2C, BaseLight_ctor): the 8-byte GdlParms head (name "Parms", 0),
 * then one "col" colour parm (GdlParmRGBA layout, include/gdl/gadget.h) whose value is the light
 * colour (default {1, 1, 1, 1}; gen_createLights_00001460 sets it per light). */
typedef struct BaseLightParms {
    /* 0x00 */ const char *name;      /* "Parms" (GdlParmList head, gdl/parm.h) */
    /* 0x04 */ GdlParmBase *first;    /* parm list head: colParm */
    /* 0x08 */ GdlParmBase colParm;   /* the "col" parm base (Parms_addParm); type = the colour parm type */
    /* 0x18 */ f32 col[4];            /* BaseLight +0x124: RGBA colour */
    /* 0x28 */ s32 unk28;             /* 0 */
} BaseLightParms;

typedef struct BaseLight {
    /* 0x000 */ Node node;
    /* 0x02C */ s32 unk2C;
    /* 0x030 */ s32 unk30[0x21];      /* MNode-style base: position / matrix (0x30-0xB4) */
    /* 0x0B4 */ f32 intensity;        /* 1.0 */
    /* 0x0B8 */ f32 unkB8[3];         /* 0 */
    /* 0x0C4 */ s32 flags;            /* 0x80 skip; ctor sets 2 | 8 */
    /* 0x0C8 */ f32 atten;            /* HostIO "Atten" slider 0..8000 (BaseLight_hostioParms); 0 */
    /* 0x0CC */ f32 falloff;          /* HostIO "FallOff" slider 0..1000; 0 */
    /* 0x0D0 */ s32 mask;             /* HostIO "MaskID"; 0xFFFF */
    /* 0x0D4 */ s32 unkD4[3];
    /* 0x0E0 */ s32 dirty;            /* gfx_updateLights: 1 when the position moved */
    /* 0x0E4 */ f32 dir[3];           /* world direction; (0, 1, 0) from the ctor */
    /* 0x0F0 */ f32 xdir[3];          /* transformed direction */
    /* 0x0FC */ f32 unkFC;            /* 1.0 */
    /* 0x100 */ f32 unk100[3];        /* 0 */
    /* 0x10C */ BaseLightParms parms; /* colour parm group (0x2C); parms.col at +0x124 */
} BaseLight;

typedef struct LightTarget {
    /* 0x000 */ Node node;
    /* 0x02C */ s32 unk2C[0x2C];       /* QNode body (QNode_ctor builds it; QNode_hostioParms) */
    /* 0x0DC */ f32 pos[3];           /* QNode cvqs.trans: the world position */
    /* 0x0E8 */ s32 unkE8[8];
    /* 0x108 */ f32 unk108[3];        /* RoData constant (+0x1BB0); LightTarget_display stores the
                                       * queryPos gfx_selectLights hands back */
    /* 0x114 */ f32 queryPos[3];      /* LightTarget_display: pos copied here, passed by address */
    /* 0x120 */ s32 mask;             /* 0xFFFF; HostIO "LightMask", doCommand 0x72 */
    /* 0x124 */ f32 scale;            /* 1.0; HostIO "Fade" (0..1) */
    /* 0x128 */ s32 unk128[0x11];     /* [0]: gfx_selectLights arg 5, cleared after each display */
} LightTarget;

typedef char BaseLight_size_check[sizeof(BaseLight) == 0x138 ? 1 : -1];
typedef char BaseLight_flags_check[(int)&((BaseLight *)0)->flags == 0xC4 ? 1 : -1];
typedef char BaseLight_dir_check[(int)&((BaseLight *)0)->dir == 0xE4 ? 1 : -1];
typedef char BaseLight_parms_check[(int)&((BaseLight *)0)->parms == 0x10C ? 1 : -1];
typedef char BaseLight_col_check[(int)&((BaseLight *)0)->parms.col == 0x124 ? 1 : -1];
typedef char BaseLightParms_size_check[sizeof(BaseLightParms) == 0x2C ? 1 : -1];
typedef char LightTarget_size_check[sizeof(LightTarget) == 0x16C ? 1 : -1];
typedef char LightTarget_mask_check[(int)&((LightTarget *)0)->mask == 0x120 ? 1 : -1];
typedef char LightTarget_pos_check[(int)&((LightTarget *)0)->pos == 0xDC ? 1 : -1];

#endif /* GDL_LIGHT_H */
