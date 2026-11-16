#ifndef GAME_ENVOBJ_H
#define GAME_ENVOBJ_H

/* bootup_uso course-environment objects (agent-h run 501): the background wall, EnvSettings, the
 * weather / prop items built from the item templates (course.h CourseItemTmpl), the rider's
 * collider probes and the ShpNode. Only the fields the bootup bodies touch are named; offsets
 * come from the ctors' stores. Needs gdl/node.h and game/course.h first. */

#ifndef __cplusplus
#include "gdl/dnode.h"
#include "gdl/coll.h"
#else
/* The edg (NCC) donors include this header by relative path, without -I include: they get Node only
 * (include game/course.h first for FogItem / SnowItem); ShpNode / Tree and the asserts are C-only. */
#include "../gdl/node.h"
#endif

/* ---- U64BGWall (0x4C, vtbl Data 0x6648; U64BGWall_ctor E68, U64BGWall_display 1A44) ----
 * The tiled background wall. The ctor builds the quad grid into a scratch vertex buffer and
 * records its display list; U64BGWall_display draws it with guPosition(rotX, rotY, rotZ, 1,
 * eye * worldScale) + gSPDisplayList(displayList). game_createBGBox builds the box's sides as
 * U64BGWall_ctor(0, tex, 0, 90, 0) .. (0, tex, -90, 0, 0) (agent-g run 551 / 555). */
typedef struct U64BGWall {
    Node node;                  /* 0x00 */
    s32 rotX;                   /* 0x2C: ctor arg 2, degrees (int -> float for guPosition) */
    s32 rotY;                   /* 0x30: ctor arg 3, degrees */
    s32 rotZ;                   /* 0x34: ctor arg 4, degrees */
    s32 displayList;            /* 0x38: the Gfx list the ctor builds (gSPDisplayList w1) */
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
    struct FallingSnow *fallingSnow; /* 0x40: [0] FallingSnow_create(this) builds it (not in the editor flow
                                 * mode); EnvSettings_setSnow switches it */
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
    char fog[0x18];             /* 0x74: FogParms (gdl/gfxlib.h): the colour Fog_display lerps the scene fog
                                 * (GfxLib +0x294) toward while the camera is inside the fade range */
    s32 inside;                 /* 0x8C */
} FogItem;

#define FOGITEM(p) ((FogItem *)(p))

/* Snower item (instance 0x74, vtbl Data 0x7968; SnowTmpl_create 84A0): parm a float 300,
 * parm b an int (1) whose parm type is the bootup one at EnvSettings_setSnow + 0x70. */
typedef struct SnowItem {
    Node node;                  /* 0x00 */
    s32 link;                   /* 0x2C */
    f32 pos[3];                 /* 0x30 */
    s32 parmsName;              /* 0x3C */
    s32 parmsList;              /* 0x40 */
    CourseFParm rng;            /* 0x44: [300] "rng": depth of the snow zone past pos[2] (scaled z) */
    CourseParm sta;             /* 0x5C: [1] "sta" (hostio "Snow"): Snower_display hands it to
                                 * EnvSettings_setSnow while the camera is inside the zone */
} SnowItem;

#define SNOWITEM(p) ((SnowItem *)(p))

/* A course "cam" item (tag "cam", ItemType registered by item_registerTemplates, create method at
 * bootup Data 0xE4 of its table): a ColourItem head with a camera kind. CamDirector_update polls the
 * one nearest the rider (ItemMgr_queryType) and hands it to the active camera's command slot (VT_SLOT
 * 12): kinds 3..0x12 go to the game.uso CourseCam, the rest to RiderCam_command_00004914, which puts
 * the eye at pos + 100 up. */
typedef struct CourseCamItem {
    Node node;                  /* 0x00 */
    s32 link;                   /* 0x2C: ColourItem link */
    f32 pos[3];                 /* 0x30 */
    s32 kind;                   /* 0x3C: the camera mode / view */
} CourseCamItem;

#define CAMITEM(p) ((CourseCamItem *)(p))

/* The falling-snow effect (0x134; vtables imported from module 60) that FallingSnow_create
 * builds into EnvSettings.fallingSnow (the owner is gEnvSettings, bootup Bss+0x20, which
 * game_createMap stores and SnowTmpl_create / Snower_display pass) and EnvSettings_setSnow
 * (EnvSettings_setSnow) switches on / off. It is a child of the world's "eg" root. */
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
/* Tv1Tmpl's 16-byte member-function callback (the bootup twin of game/sun.h's MemberCallback), built in
 * place by item_registerTemplates as a 3-level cfront cascade: the Callback base stores vptr Data 0x7528
 * (slot 1 = __pure_virtual_called), the member level stores obj and pmf, then the concrete class stores
 * vptr Data 0x7770 (slot 1 = MemberCallback_invoke_00007BF4, which calls obj->*pmf). agent-g run 558. */
typedef struct Tv1CallbackPmf {
    s32 a;                      /* 0x0: delta 0 / index -1 (0x0000FFFF: a non-virtual member) */
    s32 b;                      /* 0x4: the member function */
} Tv1CallbackPmf;
typedef struct Tv1Callback {
    void *vptr;                 /* 0x0 */
    void *obj;                  /* 0x4: the Tv1Tmpl */
    Tv1CallbackPmf pmf;         /* 0x8: Data 0x7520 = {0x0000FFFF, Tv1Tmpl_func_00007D04} (the picture step) */
} Tv1Callback;

typedef struct Tv1Tmpl {
    Node node;                  /* 0x00 */
    char unk2C[0x40 - 0x2C];
    s32 model;                  /* 0x40: the template's model node (each new instance adopts it); its
                                 * +0x84 table holds the two screen nodes */
    char unk44[0x48 - 0x44];
    Tv1Callback stepCallback;   /* 0x48: obj = this, pmf = Tv1Tmpl_func_00007D04 */
    s32 scroll;                 /* 0x58: state 0: -= 10 per step; state 2: 0x18 */
    s32 state;                  /* 0x5C: 0 / 1 / 2 (state 2 picks the next one at random) */
    s32 timer;                  /* 0x60: steps in the state (0x3E / 0x20 / 0x5C) */
    s32 unk64;                  /* 0x64: GfxLib.curWindow's VT_SLOT 11 result (the slot Fader_buildTexture
                                 * reads the frame buffer through on mainWindow) */
    s32 displayCount;           /* 0x68: cleared by update, counted by display */
} Tv1Tmpl;

/* A "tv1" instance (0x88, Tv1Tmpl_create: mem_new(0x88) + SNode_ctor, vptr Tv1_vtbl, radius = RoData
 * 0x5EC; it adopts the template's model). The layout is gdl/mnode.h's SNode, spelled flat because the
 * edg (NCC) donors cannot include mnode.h (Tv1_display, bootup_uso_ncc_76F4.cpp); the C build asserts
 * it against SNode below. Tv1_display is SNode_display's twin: cull on pos / radius * scale, then draw
 * mtx with the camera vector raised by height (agent-g run 555). */
typedef struct Tv1 {
    Node node;                  /* 0x00 */
    char mtx[0x30];             /* 0x2C: SNode "Mtx" */
    f32 pos[3];                 /* 0x5C: the cull query takes pos[0] / pos[1] as int words */
    s32 unk68;                  /* 0x68 */
    f32 height;                 /* 0x6C: added to the camera vector's y while drawing */
    f32 scale;                  /* 0x70 */
    f32 rot[3];                 /* 0x74: rotX / rotY / rotZ (degrees) */
    f32 radius;                 /* 0x80: cull radius (Tv1Tmpl_create: RoData 0x5EC) */
    s32 next;                   /* 0x84: SNode item chain */
} Tv1;

#define TV1(p) ((Tv1 *)(p))

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

#ifndef __cplusplus /* DNode / SNode / CollCylinder (gdl/dnode.h, gdl/coll.h) */
/* ---- ShpNode (0x3D8, `<QNode>` "shp", vtbl Data 0x81E0; ctor 8F18): a game_libs DNode ----
 * ShpNode_func_00008FA8 copies dnode.vqs (trans / scale / quat) into dnode.cvqs before the
 * base update, then counts `life` down. The ctor sets dnode.parms.rad to 50 (DNode default 150). */
typedef struct ShpNode {
    DNode dnode;                /* 0x000: the game_libs DNode base (include/gdl/dnode.h) */
    s32 life;                   /* 0x3D4: 100, decremented by ShpNode_func_00008FA8 */
} ShpNode;                      /* 0x3D8 */

#define SHPNODE(p) ((ShpNode *)(p))

/* ---- Tree (0xC8, vtbl Data 0x87A4 + 0x10; built by TreeTmpl_create for the "tree" item) ----
 * An SNode with a collision cylinder in place (agent-h run 529): TreeTmpl_create sets snode.scale to 10,
 * sends message 0x13, inits coll as a cylinder (owner this, id 0x74, radius 150), sets coll.obj.flags bit 0
 * and adds it to the collision manager. Tree_handleMsg copies snode.pos into coll.obj.pos on messages 7
 * (set position) and 19, so the cylinder follows the tree. */
typedef struct Tree {
    SNode snode;                /* 0x00: pos 0x5C, scale 0x70 */
    CollCylinder coll;          /* 0x88: obj.pos at 0xA0, obj.flags at 0x98, obj.vptr at 0xB8 */
} Tree;                         /* 0xC8 */

#define TREE(p) ((Tree *)(p))
#endif /* __cplusplus */

/* ---- TreeTmpl (0x74; the "tr1".. item types item_registerTemplates builds, bootup_uso_ncc_55A0.cpp) ----
 * An SNodeItemType (gdl/item.h ItemType head, 0x48) for trees (agent-h run 533). Its items are Trees chained
 * through SNode.next from `items`; TreeTmpl_create lazily builds the one PolyShape every tree shares
 * (PolyShape_ctor(0, "tree", shapeData, 0)) and hangs it under each new Tree. TreeTmpl_display sets the
 * prim colour, emits the fog parms, draws every item with DoDisplay set, then restores the GfxLib fog;
 * TreeTmpl_hostioParms publishes colour (gfx_declColourParm) and fog (FogParms_hostioParms). */
typedef struct TreeTmpl {
    Node node;                  /* 0x00 */
    s32 tag;                    /* 0x2C: ItemType tag ("tr1" ..) */
    s32 next;                   /* 0x30: ItemMgr type list */
    struct SNode *items;        /* 0x34: the Trees, chained through SNode.next */
    s32 unk38;                  /* 0x38: 1 (item_registerTemplates) */
    s32 unk3C;
    s32 shape;                  /* 0x40: the shared PolyShape (0 until the first TreeTmpl_create) */
    s32 typeId;                 /* 0x44 */
    s32 shapeData;              /* 0x48: PolyShape_ctor arg 3 (Data, item_registerTemplates) */
    f32 colour[4];              /* 0x4C: gfx_setPrimColour; (0, 0, 0, 1) */
    char fog[0x18];             /* 0x5C: FogParms (FogParms_emit / FogParms_hostioParms) */
} TreeTmpl;                     /* 0x74 */

#define TREETMPL(p) ((TreeTmpl *)(p))

#ifndef __cplusplus /* edg (NCC donors): &((T *)0)->f is not an integral constant expression */
#define ENVOBJ_ASSERT(T, f, off) typedef char envobj_assert_##T##_##off[(int)&((T *)0)->f == (off) ? 1 : -1]
ENVOBJ_ASSERT(U64BGWall, scale, 0x3C);
ENVOBJ_ASSERT(EnvSettings, fallingSnow, 0x40);
ENVOBJ_ASSERT(FogItem, inside, 0x8C);
ENVOBJ_ASSERT(FogItem, rng, 0x5C);
ENVOBJ_ASSERT(SnowItem, sta, 0x5C);
ENVOBJ_ASSERT(FallingSnow, unkBC, 0xBC);
ENVOBJ_ASSERT(Tv1Tmpl, displayCount, 0x68);
ENVOBJ_ASSERT(Tv1Tmpl, stepCallback, 0x48);
typedef char envobj_size_Tv1Callback[sizeof(Tv1Callback) == 0x10 ? 1 : -1];
ENVOBJ_ASSERT(TreeTmpl, shapeData, 0x48);
ENVOBJ_ASSERT(TreeTmpl, fog, 0x5C);
typedef char TreeTmpl_size_check[sizeof(TreeTmpl) == 0x74 ? 1 : -1];
ENVOBJ_ASSERT(SnowBCollider, contactFlags, 0x174);
ENVOBJ_ASSERT(ShpNode, life, 0x3D4);
ENVOBJ_ASSERT(Tree, coll.obj.pos, 0xA0);
typedef char envobj_size_U64BGWall[sizeof(U64BGWall) == 0x4C ? 1 : -1];
typedef char envobj_size_Tv1[sizeof(Tv1) == sizeof(SNode) ? 1 : -1];
ENVOBJ_ASSERT(Tv1, pos, 0x5C);
ENVOBJ_ASSERT(Tv1, radius, 0x80);
typedef char envobj_size_EnvSettings[sizeof(EnvSettings) == 0x44 ? 1 : -1];
typedef char envobj_size_FogItem[sizeof(FogItem) == 0x90 ? 1 : -1];
typedef char envobj_size_SnowItem[sizeof(SnowItem) == 0x74 ? 1 : -1];
typedef char envobj_size_SnowBCollider[sizeof(SnowBCollider) == 0x178 ? 1 : -1];
typedef char envobj_size_ShpNode[sizeof(ShpNode) == 0x3D8 ? 1 : -1];
typedef char envobj_size_Tree[sizeof(Tree) == 0xC8 ? 1 : -1];
#endif /* __cplusplus */

#endif
