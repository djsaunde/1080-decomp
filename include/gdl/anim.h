#ifndef GDL_ANIM_H
#define GDL_ANIM_H

#include "gdl/node.h"

/* libgdl skeletal keyframe animation (animate.cpp; game_libs 0x61E58-0x63704, agent-b run 486).
 * Port-author description: docs/1080_GRAPHICS.md section 11.
 *
 * ANIM IDS. An anim id is (group << 16) | index (anim_formatId prints it as " %d:%d").
 * `group` indexes AnimDataMgr.groups, `index` the group's AnimData records
 * (AnimDataMgr_lookup: "Missing AnimGroup %d", "Missing Anim %d:%d", "Empty Anim %d:%d" when
 * numFrames == 0). game.uso's RiderAnim keeps the group in animSet (0x10000 regular stance,
 * 0x50000 switch stance) and ORs the anim number in.
 *
 * KEYS. Each AnimData has numKeys AnimKey {frame, rate}. AnimPlayer_start(p, anim, startKey,
 * endKey, flags, blendFrames) plays from keys[startKey].frame toward keys[endKey].frame,
 * adding keys[endKey].rate to the time each update (backwards when the end frame is lower).
 * So the keys are named sub-ranges / markers of one long take.
 *
 * FRAMES. One frame = numNodes AnimNodeKey records (14 bytes each: s16 translation[3], s16
 * quaternion[4] scaled by 1/32767, RoData 0x2094 = 0x38000100). Frame f of an anim starts at
 * f * numNodes * 14 in the frame data. The frame data is either resident (AnimData.frameData)
 * or streamed from the anim file (fileOffset) into AnimDataMgr's frame cache. */

/* one key: AnimPlayer_getKeyFrame / getKeyTime ("BadKeyFrame %d - %s" / "BadKeyTime %d - %s") */
typedef struct AnimKey {
    /* 0x0 */ s32 frame;
    /* 0x4 */ f32 rate;            /* frames added to AnimPlayer.time per update while playing toward this key */
} AnimKey;

/* one node's pose in one frame (anim_unpackKey) */
typedef struct AnimNodeKey {
    /* 0x0 */ s16 trans[3];        /* unscaled */
    /* 0x6 */ s16 quat[4];         /* x y z w, * (1 / 32767) */
} AnimNodeKey;

/* the unpacked pose (anim_unpackKey writes trans and quat; Animator_ctor inits scale to 1.0 and
 * quat to (0, 0, 0, 1)); anim_blendPose lerps trans and slerps quat (math_quatSlerp) */
typedef struct AnimPose {
    /* 0x00 */ f32 trans[3];
    /* 0x0C */ f32 scale[3];
    /* 0x18 */ f32 quat[4];
} AnimPose;

/* one anim (0x20; AnimData_init 0x61F70, AnimData_load 0x61F8C) */
typedef struct AnimData {
    /* 0x00 */ s16 numNodes;       /* cmd 2; must equal AnimDataMgr.numNodes ("AMgr: bad num Nodes") */
    /* 0x02 */ s16 numFrames;      /* cmd 5; 0 = "Empty Anim" */
    /* 0x04 */ s32 state;          /* 7 once loaded */
    /* 0x08 */ s32 frameData;      /* AnimNodeKey *: resident frames (cmd 3), 0 when streamed */
    /* 0x0C */ s32 numKeys;        /* cmd 1 */
    /* 0x10 */ AnimKey *keys;      /* numKeys entries (cmd 1) */
    /* 0x14 */ s32 fileOffset;     /* streamed frame data: file position of frame 0 (cmd 3) */
    /* 0x18 */ struct AnimDataMgr *mgr;
    /* 0x1C */ s32 keepKeys;       /* nonzero: a reload reads and drops the key block */
} AnimData;

/* one anim group (0xC; AnimGroup_init 0x622C8, AnimGroup_load 0x622D8) */
typedef struct AnimGroup {
    /* 0x0 */ AnimData *anims;      /* count entries */
    /* 0x4 */ s32 count;
    /* 0x8 */ struct AnimDataMgr *mgr;
} AnimGroup;

/* one cached frame (0x14; AnimDataMgr_initSlots 0x62368) */
typedef struct AnimFrameSlot {
    /* 0x00 */ s32 pose;           /* AnimNodeKey *: numNodes records (alloc numNodes * 14) */
    /* 0x04 */ s32 frame;          /* cache key 2 */
    /* 0x08 */ AnimData *anim;      /* cache key 1 */
    /* 0x0C */ s32 inUse;          /* cleared for every slot each update (0x62444) */
    /* 0x10 */ s16 pending;        /* async read handle, -1 when the pose is ready */
    /* 0x12 */ s16 pad12;
} AnimFrameSlot;

typedef struct AnimFrameCache {
    /* 0x0 */ AnimFrameSlot *slots;
    /* 0x4 */ s32 count;
} AnimFrameCache;

/* AnimDataMgr (0x48, "AnimDataMgr", vtable Data 0x21FD8; ctor 0x6264C) */
typedef struct AnimDataMgr {
    /* 0x00 */ Node node;
    /* 0x2C */ s32 version;        /* first int of the anim file */
    /* 0x30 */ AnimGroup *groups;    /* numGroups entries */
    /* 0x34 */ s32 numGroups;
    /* 0x38 */ s32 numNodes;       /* skeleton size; frame stride = numNodes * 14 */
    /* 0x3C */ s32 file;           /* anim file handle (fs_getHandle) */
    /* 0x40 */ AnimFrameCache cache;
} AnimDataMgr;

/* AnimPlayer.flags (AnimPlayer_start's 5th argument; 0 = stopped) */
#define ANIM_PLAY       0x001      /* any nonzero value plays */
#define ANIM_LOOP       0x002      /* at the end key: restart from the start key ... */
#define ANIM_PINGPONG   0x004      /* ... or, with LOOP, swap start / end and play back */
#define ANIM_WEIGHTED   0x080      /* set by AnimPlayer_setBlend: time picked by weight, not played */
#define ANIM_BLENDIN    0x100      /* cross-fade from the previous pose over blendFrames updates */

#define ANIM_NO_KEY     (-1000)    /* eventKey / blendEvent "none" */

/* AnimPlayer (0xB0, "<Node>", vtable Data 0x21F58; ctor 0x62E80, reset 0x62F08). Base of
 * game.uso RiderAnim (include/game/rideranim.h) and genproc's character-select model.
 * Vtable: slot 3 AnimPlayer_update (advance + applyPose), 4 handleMsg, 10 onKey
 * (AnimPlayer_onKey, empty here; called when time passes keys[eventKey].frame, and
 * when a blend-in finishes while blendEvent == -1). */
typedef struct AnimPlayer {
    /* 0x00 */ Node node;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ NodeLink *animators; /* list head, item = Animator *: one per animated node */
    /* 0x34 */ s32 slot0;          /* AnimFrameSlot index of frame0 / frame1 */
    /* 0x38 */ s32 slot1;
    /* 0x3C */ s32 unk3C;
    /* 0x40 */ s32 unk40;
    /* 0x44 */ s32 pose0;          /* AnimNodeKey *: frame0 / frame1 poses (applyPose) */
    /* 0x48 */ s32 pose1;
    /* 0x4C */ s32 frame0;         /* (int)time */
    /* 0x50 */ s32 frame1;         /* frame0 + 1, clamped to numFrames - 1 */
    /* 0x54 */ f32 frac;           /* time - frame0 */
    /* 0x58 */ AnimDataMgr *mgr;
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ s32 unk60;
    /* 0x64 */ s32 eventKey;       /* onKey fires when time passes this key's frame; ANIM_NO_KEY */
    /* 0x68 */ s32 blendEvent;     /* -1: onKey fires when the blend-in ends; ANIM_NO_KEY */
    /* 0x6C */ s32 blendFrames;    /* AnimPlayer_start's 6th argument */
    /* 0x70 */ s32 blendCount;
    /* 0x74 */ s32 anim;           /* anim id, 0 = none */
    /* 0x78 */ f32 time;           /* current frame (fractional) */
    /* 0x7C */ s32 startKey;       /* swapped with endKey by ANIM_PINGPONG */
    /* 0x80 */ s32 endKey;
    /* 0x84 */ s32 reqStartKey;    /* as passed to start (startIfChanged compares anim + these) */
    /* 0x88 */ s32 reqEndKey;
    /* 0x8C */ s32 weightAnimNeg;  /* ANIM_WEIGHTED: weight < 0 plays this anim, keys 0..1, at -weight */
    /* 0x90 */ s32 weightAnimPos;  /* weight >= 0: this anim at weight */
    /* 0x94 */ f32 weight;
    /* 0x98 */ s32 flags;          /* ANIM_* */
    /* 0x9C */ s32 unk9C;
    /* 0xA0 */ s32 updateCount;    /* poses are fetched / applied when it counts down to 0 */
    /* 0xA4 */ s32 updateInterval; /* AnimPlayer_setUpdateInterval; 1 = every update */
    /* 0xA8 */ f32 blendWeight;    /* blendCount / blendFrames */
    /* 0xAC */ s32 blendLeft;      /* blendFrames - blendCount; 0 = no blend */
} AnimPlayer;

typedef char AnimPlayer_size_check[sizeof(AnimPlayer) == 0xB0 ? 1 : -1];
typedef char AnimPlayer_anim_check[(int)&((AnimPlayer *)0)->anim == 0x74 ? 1 : -1];
typedef char AnimPlayer_flags_check[(int)&((AnimPlayer *)0)->flags == 0x98 ? 1 : -1];
typedef char AnimData_size_check[sizeof(AnimData) == 0x20 ? 1 : -1];
typedef char AnimFrameSlot_size_check[sizeof(AnimFrameSlot) == 0x14 ? 1 : -1];
typedef char AnimDataMgr_size_check[sizeof(AnimDataMgr) == 0x48 ? 1 : -1];
typedef char AnimPose_size_check[sizeof(AnimPose) == 0x28 ? 1 : -1];

/* Animator (0x130, "anim"; Animator_ctor 0x628EC (self, target, player, arg3, nodeIndex)):
 * binds one skeleton node to an AnimPlayer (appended to player->animators). anim_blendPose
 * writes dst, copies it to cur only when blendLeft < 2 (cur = the frozen blend-from pose while
 * blending), then calls target's vtable slot 9. */
typedef struct Animator {
    /* 0x000 */ Node node;
    /* 0x02C */ char pad02C[0x48 - 0x2C];
    /* 0x048 */ s32 unk48;         /* ctor arg3 */
    /* 0x04C */ s32 unk4C;         /* blendPose sets 7 */
    /* 0x050 */ s32 unk50;         /* blendPose sets 22 */
    /* 0x054 */ s32 target;        /* the animated node (notified through vtable slot 9) */
    /* 0x058 */ AnimPose rest;     /* identity */
    /* 0x080 */ AnimPose cur;
    /* 0x0A8 */ char pad0A8[0xD0 - 0xA8];
    /* 0x0D0 */ AnimPose dst;      /* identity at ctor */
    /* 0x0F8 */ char pad0F8[0x10C - 0xF8];
    /* 0x10C */ s32 nodeIndex;     /* record index inside a frame (applyPose: nodeIndex * 14) */
    /* 0x110 */ s32 unk110;        /* bit 0 cleared by the ctor */
    /* 0x114 */ char pad114[0x130 - 0x114];
} Animator;

typedef char Animator_size_check[sizeof(Animator) == 0x130 ? 1 : -1];
typedef char Animator_dst_check[(int)&((Animator *)0)->dst == 0xD0 ? 1 : -1];
typedef char Animator_nodeIndex_check[(int)&((Animator *)0)->nodeIndex == 0x10C ? 1 : -1];

#endif
