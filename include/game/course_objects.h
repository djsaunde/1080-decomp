#ifndef GAME_COURSE_OBJECTS_H
#define GAME_COURSE_OBJECTS_H

/* The course-object USOs: heli.uso, checkpoint.uso, fire.uso, balloon.uso
 * (docs/1080_NEW_MODULES.md; class notes in docs/1080_GAME_CLASSES.md "Course objects").
 * Each is libgdl C++ written as IDO cfe C: an SNode / QNode subclass, its ItemType template
 * (built per course by ItemMgr), cfront `this == 0` allocating ctors.
 * Field names: the HostIO parm tags the *_hostioParms register where there are any (module
 * Data strings), otherwise the role the code shows. C only (the C++ donors keep their own
 * mirrors; no C++ guard). */

#include "gdl/node.h"
#include "gdl/mnode.h"
#include "gdl/item.h"
#include "gdl/coll.h"
#include "gdl/sprite.h"

/* ---- heli.uso ---- */

/* HeliItemType (0x54, HeliItemType_ctor; vtable Data+0xCAF0): loads the helicopter's models once
 * (HeliItemType_create) and builds the Heli. type.unk40 = the body shape too (the node each new
 * item attaches). */
typedef struct HeliItemType {
    /* 0x00 */ ItemType type;
    /* 0x48 */ void *bodyShape;    /* PolyShape "heli" (module Data+0x1C1C model) */
    /* 0x4C */ void *bladeShape;   /* PolyShape "helib" (Data+0xB984), shared by both rotors */
    /* 0x50 */ Sprite *sprite;     /* perspective 1, depthBias -200 */
} HeliItemType;

/* HeliRotor (0x118, HeliRotor_ctor, "<QNode>", vtable Data+0xCA20): HeliRotor_update turns
 * vqs.quat about axis by spinSpeed / RoData+0x54 each frame. */
typedef struct HeliRotor {
    /* 0x000 */ QNode qnode;
    /* 0x104 */ s32 unk104;
    /* 0x108 */ f32 spinSpeed;     /* RoData+0x50 at ctor */
    /* 0x10C */ f32 axis[3];       /* main rotor (0, 1, 0), tail rotor (1, 0, 0) */
} HeliRotor;

/* Heli (0x174, Heli_ctor / Heli_defaultCtor, "<QNode>", vtable Data+0xCA88). The flight state
 * (+0x114..) is set by Heli_resetFlight and driven by Heli_update (the C++ donors spell it as
 * their own struct, heli_uso_ncc.cpp `struct Heli`). */
typedef struct Heli {
    /* 0x000 */ QNode qnode;
    /* 0x104 */ s32 unk104;
    /* 0x108 */ f32 startPos[3];   /* loadData; message 7 stores vqs.trans here, 9 restores it */
    /* 0x114 */ f32 target[3];     /* the point flown towards */
    /* 0x120 */ f32 offset[3];     /* (0, 400, 2000) at reset */
    /* 0x12C */ f32 look[3];
    /* 0x138 */ f32 heading[3];    /* unit heading, slerped towards target */
    /* 0x144 */ f32 roll;
    /* 0x148 */ f32 pitch;
    /* 0x14C */ f32 yaw;
    /* 0x150 */ f32 maxSpeed;      /* 200 */
    /* 0x154 */ f32 speed;
    /* 0x158 */ f32 accel;         /* 5 */
    /* 0x15C */ f32 nearDist;      /* 250 */
    /* 0x160 */ f32 speed2;
    /* 0x164 */ f32 slerp;         /* heading slerp factor (RoData+0x74) */
    /* 0x168 */ s32 marker;        /* the current path marker item */
    /* 0x16C */ s32 mode;          /* 7 at reset; the marker's +0x3C id */
    /* 0x170 */ s32 mode2;         /* overrides mode when set (8 after Heli_placeAtMarker) */
} Heli;

/* the "hem" / "mrk" marker item (0x40): a ColourItem (rgb = position) plus an id */
typedef struct HeliMarkerItem {
    /* 0x00 */ ColourItem item;
    /* 0x3C */ s32 id;             /* HeliMarkerItem_load; Heli_placeAtMarker looks for id 8 */
} HeliMarkerItem;

/* ---- fire.uso ---- */

/* FireTmpl (0x68, FireTmpl_ctor; vtable Data+0x228C): an SNodeItemType whose type.unk40 is the
 * shared Sprite; "Env" / "Prim" colour parms (FireTmpl_declColourParms). */
typedef struct FireTmpl {
    /* 0x00 */ ItemType type;
    /* 0x48 */ f32 envColour[4];   /* "Env" (126, 33, 3, 255) / 255 */
    /* 0x58 */ f32 primColour[4];  /* "Prim" (219, 196, 0, 255) / 255 */
} FireTmpl;

/* Fire (0x94, Fire_ctor; vtable Data+0x231C): an SNode that animates the template's sprite. */
typedef struct Fire {
    /* 0x00 */ SNode snode;
    /* 0x88 */ Sprite *sprite;     /* ctor arg: the template's sprite */
    /* 0x8C */ s16 frame;          /* sprite image index, cycles 0..7 in Fire_display */
    /* 0x8E */ s16 pad8E;
    /* 0x90 */ f32 alpha;          /* "alpha" (0..1) */
} Fire;

/* ---- balloon.uso ---- */

/* Balloon (0x9C, Balloon_ctor; vtable Data+0x4128): an SNode bobbed by off * fsin(phase). */
typedef struct Balloon {
    /* 0x00 */ SNode snode;
    /* 0x88 */ f32 off[3];         /* "Off.x" / "Off.y" / "Off.z" (-500..500): bob amplitude */
    /* 0x94 */ f32 speed;          /* "Speed" (0..1): phase step per frame */
    /* 0x98 */ f32 phase;          /* reset by messages 7 / 19 */
} Balloon;

/* a PolyShape template (balloon BalloonTmpl, checkpoint CheckpointCylTmpl): type.unk40 = the shape
 * built on first use from the model data word */
typedef struct ShapeTmpl {
    /* 0x00 */ ItemType type;
    /* 0x48 */ s32 model;          /* PolyShape_ctor model data */
} ShapeTmpl;

/* ---- checkpoint.uso ---- */

/* CheckpointCyl (0x10C, CheckpointCyl_ctor; vtable Data+0x3E20): a gate of two cylinder posts at
 * (-/+ 117 / worldScale, 0, 0) in the SNode's frame (CheckpointCyl_handleMsg sets their pos). */
typedef struct CheckpointCyl {
    /* 0x00 */ SNode snode;
    /* 0x88 */ s32 time;           /* "Time" (0..100), 20 at ctor */
    /* 0x8C */ CollCylinder postA; /* registered with CollObjMgr_addStatic */
    /* 0xCC */ CollCylinder postB;
} CheckpointCyl;

/* CheckpointBox (0xCC, CheckpointBox_ctor; vtable Data+0x3E80): one cylinder at the SNode pos. */
typedef struct CheckpointBox {
    /* 0x00 */ SNode snode;
    /* 0x88 */ CollCylinder coll;
    /* 0xC8 */ s32 tmplArg;        /* ctor arg: the template's +0x48 word (role not proven) */
} CheckpointBox;

/* CheckpointBoxTmpl: ItemType + the ctor arg word and the model data word */
typedef struct CheckpointBoxTmpl {
    /* 0x00 */ ItemType type;
    /* 0x48 */ s32 boxArg;         /* passed to CheckpointBox_ctor (CheckpointBox.tmplArg) */
    /* 0x4C */ s32 model;          /* PolyShape_ctor model data ("flag") */
} CheckpointBoxTmpl;

typedef char HeliItemType_size_check[sizeof(HeliItemType) == 0x54 ? 1 : -1];
typedef char HeliRotor_size_check[sizeof(HeliRotor) == 0x118 ? 1 : -1];
typedef char Heli_size_check[sizeof(Heli) == 0x174 ? 1 : -1];
typedef char Heli_target_check[(int)&((Heli *)0)->target == 0x114 ? 1 : -1];
typedef char Heli_marker_check[(int)&((Heli *)0)->marker == 0x168 ? 1 : -1];
typedef char HeliMarkerItem_size_check[sizeof(HeliMarkerItem) == 0x40 ? 1 : -1];
typedef char FireTmpl_size_check[sizeof(FireTmpl) == 0x68 ? 1 : -1];
typedef char Fire_size_check[sizeof(Fire) == 0x94 ? 1 : -1];
typedef char Balloon_size_check[sizeof(Balloon) == 0x9C ? 1 : -1];
typedef char ShapeTmpl_size_check[sizeof(ShapeTmpl) == 0x4C ? 1 : -1];
typedef char CheckpointCyl_size_check[sizeof(CheckpointCyl) == 0x10C ? 1 : -1];
typedef char CheckpointCyl_postA_check[(int)&((CheckpointCyl *)0)->postA.obj.pos == 0xA4 ? 1 : -1];
typedef char CheckpointCyl_postB_check[(int)&((CheckpointCyl *)0)->postB.obj.pos == 0xE4 ? 1 : -1];
typedef char CheckpointBox_size_check[sizeof(CheckpointBox) == 0xCC ? 1 : -1];
typedef char CheckpointBox_pos_check[(int)&((CheckpointBox *)0)->coll.obj.pos == 0xA0 ? 1 : -1];
typedef char CheckpointBoxTmpl_size_check[sizeof(CheckpointBoxTmpl) == 0x50 ? 1 : -1];

#define HELIITEMTYPE(p) ((HeliItemType *)(p))
#define HELIROTOR(p) ((HeliRotor *)(p))
#define HELI(p) ((Heli *)(p))
#define HELIMARKER(p) ((HeliMarkerItem *)(p))
#define FIRETMPL(p) ((FireTmpl *)(p))
#define FIRE(p) ((Fire *)(p))
#define BALLOON(p) ((Balloon *)(p))
#define SHAPETMPL(p) ((ShapeTmpl *)(p))
#define CHECKPOINTCYL(p) ((CheckpointCyl *)(p))
#define CHECKPOINTBOX(p) ((CheckpointBox *)(p))
#define CHECKPOINTBOXTMPL(p) ((CheckpointBoxTmpl *)(p))

#endif /* GAME_COURSE_OBJECTS_H */
