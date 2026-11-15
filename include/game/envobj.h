#ifndef GAME_ENVOBJ_H
#define GAME_ENVOBJ_H

/* bootup_uso course-environment objects (agent-h run 501): the background wall, EnvSettings, the
 * weather / prop items built from the item templates (course.h CourseItemTmpl), the rider's
 * collider probes and the ShpNode. Only the fields the bootup bodies touch are named; offsets
 * come from the ctors' stores. Needs gdl/node.h and game/course.h first. */

#include "gdl/dnode.h"

/* ---- U64BGWall (0x4C, vtbl Data 0x6648; U64BGWall_ctor E68) ----
 * The tiled background-wall mesh. The ctor builds the quad grid into a scratch vertex buffer and
 * appends the index list to `mesh`'s +0xC {data, count} handle. */
typedef struct U64BGWall {
    Node node;                  /* 0x00 */
    s32 mesh;                   /* 0x2C: ctor arg 2: the shape whose +0xC index-list handle is filled */
    s32 arg30;                  /* 0x30: ctor arg 3 (appended after the per-quad indices) */
    s32 arg34;                  /* 0x34: ctor arg 4 */
    s32 unk38;
    f32 scale[4];               /* 0x3C: 1.0f x 4 (stored 0x48 first) */
} U64BGWall;

/* ---- EnvSettings (0x44, vtbl Data 0x76A8; EnvSettings_ctor 7150) ----
 * The course environment ("rom/<course>_env.bin", EnvSettings_load). EnvSettings_display copies
 * clipNear / clipFar into the gfx context near / far (+0xAC / +0xB0, int -> float) when flags & 2,
 * and while mode == 0 sets the context +0x78 bit 4 and its +0xDC 16-byte record. */
typedef struct EnvSettings {
    Node node;                  /* 0x00 */
    s32 handlers;               /* 0x2C: the parm-handler list EnvSettings_load searches by tag */
    s32 clipNear;               /* 0x30: [4] */
    s32 clipFar;                /* 0x34: [0xB00]; EnvSettings_load sets 800 for game modes 6 / 11 */
    s32 flags;                  /* 0x38: [1]; bit 2 = override the clip range (set with the 800 far) */
    s32 mode;                   /* 0x3C: [0] */
    s32 unk40;                  /* 0x40: [0] */
} EnvSettings;

#define ENVSET(p) ((EnvSettings *)(p))

/* ---- ColourItem-based course items (fog / snow / tv1; ColourItem = Node + link + position) ---- */
/* Fog item (instance 0x90, vtbl Data 0x7878; FogTmpl_create 8124): "fin" 200, "rng" 300.
 * Fog_display: with p = gfx scale (+0x130) * pos[2], the fog blend runs while the eye z is in
 * [p, p + fin] and stays full up to p + rng; `inside` caches the last result. */
typedef struct FogItem {
    Node node;                  /* 0x00 */
    s32 link;                   /* 0x2C: ColourItem link */
    f32 pos[3];                 /* 0x30 */
    s32 parmsName;              /* 0x3C: "Parms" group (Parms_addParm(parm, group, tag, 1)) */
    s32 parmsList;              /* 0x40 */
    CourseFParm fin;            /* 0x44: [200] fade-in length (value 0x54) */
    CourseFParm rng;            /* 0x5C: [300] full-fog range (value 0x6C) */
    char sub74[0x18];           /* 0x74: the fog colour record handed to the blend call */
    s32 inside;                 /* 0x8C */
} FogItem;

#define FOGITEM(p) ((FogItem *)(p))

/* Snower item (instance 0x74, vtbl Data 0x7968; SnowTmpl_create 84A0): parm a float 300,
 * parm b an int (1) whose parm type is the bootup one at func_00007620 + 0x70. */
typedef struct SnowItem {
    Node node;                  /* 0x00 */
    s32 link;                   /* 0x2C */
    f32 pos[3];                 /* 0x30 */
    s32 parmsName;              /* 0x3C */
    s32 parmsList;              /* 0x40 */
    CourseFParm unk44;          /* 0x44: [300] */
    CourseParm unk5C;           /* 0x5C: [1] */
} SnowItem;

/* The falling-snow effect (0x134; vtables imported from module 60) that FallingSnow_create
 * builds into its owner's +0x40 and func_00007620 switches on / off. */
typedef struct FallingSnow {
    Node node;                  /* 0x00: doFlags bits 4 / 8 = display / update on */
    char unk2C[0x48 - 0x2C];
    s32 enabled;                /* 0x48 */
    f32 unk4C;                  /* 0x4C: 0 when off, a RoData constant when on */
    char unk50[0xBC - 0x50];
    s32 unkBC;                  /* 0xBC: when 0, switching off also clears doFlags 4 / 8 */
} FallingSnow;

/* "tv1" item template (vtbl Data 0x7788; Tv1Tmpl_create 7C74 builds Tv1 instances of 0x88 with
 * +0x80 = a RoData float). Tv1Tmpl_func_00007D04 steps a 3-state picture cycle. */
typedef struct Tv1Tmpl {
    Node node;                  /* 0x00 */
    char unk2C[0x40 - 0x2C];
    s32 model;                  /* 0x40: the template's model node (each new instance adopts it); its
                                 * +0x84 table holds the two screen nodes */
    char unk44[0x58 - 0x44];
    s32 scroll;                 /* 0x58: state 0: -= 10 per step; state 2: 0x18 */
    s32 state;                  /* 0x5C: 0 / 1 / 2 (state 2 picks the next one at random) */
    s32 timer;                  /* 0x60: steps in the state (0x3E / 0x20 / 0x5C) */
    s32 unk64;                  /* 0x64: result of the gfx-lib +0x18 node's vtable +0x60 call */
    s32 displayCount;           /* 0x68: cleared by update, counted by display */
} Tv1Tmpl;

/* ---- SnowBCollider (0x178, vtbl Data 0x8698; ctor 8B44, onContact 8BD4) ----
 * The rider's contact probes (1080_PHYSICS.md). The base (0x150) is built by the shared init. */
typedef struct SnowBCollider {
    char base[0x114];           /* 0x000: collision-probe base (Node at 0) */
    f32 probeVel[3];            /* 0x114 */
    f32 point[3];               /* 0x120: world point */
    char unk12C[0x14C - 0x12C];
    s32 triangle;               /* 0x14C: the touched triangle (surface-code lookup key) */
    f32 unk150;                 /* 0x150: 1.0 */
    f32 unk154;                 /* 0x154: 10.0 */
    char unk158[0x170 - 0x158];
    struct SnowBoarder *rider;  /* 0x170: ctor arg 1 */
    u16 contactFlags;           /* 0x174: ctor arg 2: 0x1F0 bits = body contact, 0x00F = edge contact */
} SnowBCollider;                /* 0x178 */

#define SNOWBCOL(p) ((SnowBCollider *)(p))

/* ---- ShpNode (0x3D8, `<QNode>` "shp", vtbl Data 0x81E0; ctor 8F18): a game_libs DNode ----
 * ShpNode_func_00008FA8 copies dnode.vqs (trans / scale / quat) into dnode.cvqs before the
 * base update, then counts `life` down. The ctor sets dnode.parms.rad to 50 (DNode default 150). */
typedef struct ShpNode {
    DNode dnode;                /* 0x000: the game_libs DNode base (include/gdl/dnode.h) */
    s32 life;                   /* 0x3D4: 100, decremented by ShpNode_func_00008FA8 */
} ShpNode;                      /* 0x3D8 */

#define SHPNODE(p) ((ShpNode *)(p))

#define ENVOBJ_ASSERT(T, f, off) typedef char envobj_assert_##T##_##off[(int)&((T *)0)->f == (off) ? 1 : -1]
ENVOBJ_ASSERT(U64BGWall, scale, 0x3C);
ENVOBJ_ASSERT(EnvSettings, unk40, 0x40);
ENVOBJ_ASSERT(FogItem, inside, 0x8C);
ENVOBJ_ASSERT(FogItem, rng, 0x5C);
ENVOBJ_ASSERT(SnowItem, unk5C, 0x5C);
ENVOBJ_ASSERT(FallingSnow, unkBC, 0xBC);
ENVOBJ_ASSERT(Tv1Tmpl, displayCount, 0x68);
ENVOBJ_ASSERT(SnowBCollider, contactFlags, 0x174);
ENVOBJ_ASSERT(ShpNode, life, 0x3D4);
typedef char envobj_size_U64BGWall[sizeof(U64BGWall) == 0x4C ? 1 : -1];
typedef char envobj_size_EnvSettings[sizeof(EnvSettings) == 0x44 ? 1 : -1];
typedef char envobj_size_FogItem[sizeof(FogItem) == 0x90 ? 1 : -1];
typedef char envobj_size_SnowItem[sizeof(SnowItem) == 0x74 ? 1 : -1];
typedef char envobj_size_SnowBCollider[sizeof(SnowBCollider) == 0x178 ? 1 : -1];
typedef char envobj_size_ShpNode[sizeof(ShpNode) == 0x3D8 ? 1 : -1];

#endif
