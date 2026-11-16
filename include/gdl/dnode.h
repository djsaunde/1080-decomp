#ifndef GDL_DNODE_H
#define GDL_DNODE_H

#include "gdl/mnode.h"
#include "gdl/gadget.h"

/*
 * DNode: the libgdl dynamics node (game_libs post1b 0x63884-0x65E64; agent-b run 511).
 *
 * Evidence: DNode_ctor (0x64588) allocs 0x3D4 when this == 0, runs the QNode base ctor,
 * stores DNode_vtbl (Data 0x22208) and builds "DNode::Parms" (Data 0x223C0) in place at
 * +0x108 with 15 parms whose tags are the words at Data 0x223D0..0x22408 (imp spd spl spw
 * gvt suc vsc usc tsc cog unc dsc csc rad cts). DNode_update runs QNode_update,
 * DNode_solveContacts, DNode_applyAccums_00065448, DNode_integrate_00064DEC and
 * DNode_resetAccums_00065148. bootup's SnowBoarder (include/game/snowboarder.h, agent-g)
 * and ShpNode (include/game/envobj.h) derive from it; their names for the shared fields
 * were agreed in run 470 and are used here.
 *
 * Parm uses proven by the bodies: imp / spd / vsc values seed surfImp / surfSpd / surfVsc
 * (DNode_init_00065060); gvt.value is added to vel[1] every frame (integrate: gravity,
 * default -0.72); tsc.value scales both accumulators (applyAccums); cog.value is the centre
 * of gravity in body space (resetAccums -> worldCog); suc.value is read by
 * DNode_getSuc. Defaults (ctor; RoData pool kRo_DNode_ctor): imp 0, spd 0.2,
 * spl 100, spw 0, gvt -0.72, suc 0, vsc 0.216, usc 0.2, tsc 1, cog (0, 0, 0) in +-1000,
 * unc 0, dsc 1, csc 420, rad 150, cts (1, 1, 0) in +-10.
 */

/* a Vec3 parm (0x20; type vtable Data 0x221F0, the float parm type is Data 0x221D8) */
typedef struct GdlParmV3 {
    /* 0x00 */ char head[0xC];      /* filled by the parm base ctor (Parms_addParm) */
    /* 0x0C */ s32 type;
    /* 0x10 */ f32 value[3];
    /* 0x1C */ s32 unk1C;           /* 0 */
} GdlParmV3;

/* "DNode::Parms" (0x180) */
typedef struct DNodeParms {
    /* 0x000 */ s32 name;           /* "DNode::Parms" */
    /* 0x004 */ s32 head;           /* parm list head */
    /* 0x008 */ GdlParmF imp;       /* surface impulse scale (-> surfImp) */
    /* 0x020 */ GdlParmF spd;       /* surface speed scale (-> surfSpd) */
    /* 0x038 */ GdlParmF spl;
    /* 0x050 */ GdlParmF spw;
    /* 0x068 */ GdlParmF gvt;       /* gravity: added to vel[1] each frame */
    /* 0x080 */ GdlParmF suc;
    /* 0x098 */ GdlParmF vsc;       /* (-> surfVsc) */
    /* 0x0B0 */ GdlParmF usc;
    /* 0x0C8 */ GdlParmF tsc;       /* accumulator time scale */
    /* 0x0E0 */ GdlParmV3 cog;      /* centre of gravity, body space */
    /* 0x100 */ GdlParmF unc;
    /* 0x118 */ GdlParmF dsc;
    /* 0x130 */ GdlParmF csc;
    /* 0x148 */ GdlParmF rad;
    /* 0x160 */ GdlParmV3 cts;
} DNodeParms;

/* the 0x34-byte force accumulator (DNodeAccum_reset_00064170 / _addForce_000641DC /
 * _apply_00064388). addForce(point, force) adds |force| to weight, point * |force| to
 * pointSum, force to force and cross(point, force) / |point| to torque; apply turns the
 * target quat about the normalized torque and adds force to the target vector. */
typedef struct DNodeAccum {
    /* 0x00 */ f32 weight;
    /* 0x04 */ f32 pointSum[3];     /* application points weighted by |force| */
    /* 0x10 */ f32 force[3];
    /* 0x1C */ f32 torque[3];
    /* 0x28 */ f32 origin[3];       /* reference point (cvqs.trans), set by reset */
} DNodeAccum;

typedef struct DNode {
    /* QNode base (same layout as QNode in gdl/mnode.h, one level flatter) */
    /* 0x000 */ MNode mnode;
    /* 0x0B4 */ AnimPose vqs;       /* trans = position, quat = orientation (x y z w) */
    /* 0x0DC */ AnimPose cvqs;      /* vqs at the end of the last integrate (lastPos / lastOrient) */
    /* 0x104 */ s32 unk104;
    /* 0x108 */ DNodeParms parms;
    /* 0x288 */ f32 surfImp;        /* parms.imp, or the per-surface value (SnowBoarder_procColl) */
    /* 0x28C */ f32 surfSpd;
    /* 0x290 */ f32 surfVsc;
    /* 0x294 */ DNodeAccum impulseAcc; /* applied to angVel / vel (DNode_addImpulseAt_00065250) */
    /* 0x2C8 */ DNodeAccum moveAcc;    /* applied to vqs.quat / vqs.trans (DNode_addMoveAt_000652D8) */
    /* 0x2FC */ f32 angVel[4];      /* per-frame rotation quat; integrate: vqs.quat = angVel * vqs.quat */
    /* 0x30C */ f32 velDir[3];      /* vel / speed (0 when stopped) */
    /* 0x318 */ f32 vel[3];
    /* 0x324 */ f32 worldCog[3];    /* parms.cog through cmtx (resetAccums) */
    /* 0x330 */ f32 velDelta[3];    /* vel - lastVel of the last integrate */
    /* 0x33C */ f32 lastVel[3];
    /* 0x348 */ f32 speed;          /* |vel| */
    /* 0x34C */ f32 unk34C[3];      /* zeroed by DNode_init */
    /* 0x358 */ char pad358[0x360 - 0x358];
    /* 0x360 */ f32 friction;       /* Friction_applyContact scales its impulse by it */
    /* 0x364 */ char pad364[0x38C - 0x364];
    /* 0x38C */ s32 unk38C;         /* DNode_init 0xFFFF; SnowBoarder_ctor 1 */
    /* 0x390 */ f32 unk390[3];      /* DNode_init (1, 1, 1) */
    /* 0x39C */ s32 attached;       /* list of attached Frictions ({data, next} cells): DNode_updateAttached
                                     * sets each one's contactPoint to its cvqs.trans plus vel and keeps
                                     * the frame delta (contactDelta) and previous value (prevContact) */
    /* 0x3A0 */ s32 unk3A0;         /* 0 at ctor */
    /* 0x3A4 */ f32 unk3A4[3];      /* SnowBoarder: startPos */
    /* 0x3B0 */ f32 axes[3][3];     /* rotation matrix of cvqs.quat (DNode_quatToMtx_00065B40);
                                     * SnowBoarder calls the rows up / side / dir */
} DNode;

typedef char DNodeParms_size_check[sizeof(DNodeParms) == 0x180 ? 1 : -1];
typedef char DNodeAccum_size_check[sizeof(DNodeAccum) == 0x34 ? 1 : -1];
typedef char GdlParmV3_size_check[sizeof(GdlParmV3) == 0x20 ? 1 : -1];
typedef char DNode_size_check[sizeof(DNode) == 0x3D4 ? 1 : -1];
typedef char DNode_parms_check[(int)&((DNode *)0)->parms == 0x108 ? 1 : -1];
typedef char DNode_impulse_check[(int)&((DNode *)0)->impulseAcc == 0x294 ? 1 : -1];
typedef char DNode_vel_check[(int)&((DNode *)0)->vel == 0x318 ? 1 : -1];
typedef char DNode_speed_check[(int)&((DNode *)0)->speed == 0x348 ? 1 : -1];
typedef char DNode_friction_check[(int)&((DNode *)0)->friction == 0x360 ? 1 : -1];
typedef char DNode_attached_check[(int)&((DNode *)0)->attached == 0x39C ? 1 : -1];
typedef char DNode_axes_check[(int)&((DNode *)0)->axes == 0x3B0 ? 1 : -1];
typedef char DNode_tsc_check[(int)&((DNode *)0)->parms.tsc.value == 0x1E0 ? 1 : -1];
typedef char DNode_cog_check[(int)&((DNode *)0)->parms.cog.value == 0x1F8 ? 1 : -1];

#define DNODE(p) ((DNode *)(p))

/* PhysQNode (0x124, "<QNode>", vtable PhysQNode_vtbl; PhysQNode_ctor_00063884(this, owner,
 * &probeA, &probeB)): two body-space probe points of a DNode. Its vfn11
 * (PhysQNode_pushProbes_00063964) rotates each probe by cvqs.quat (math_quatRotateVec3), adds
 * cvqs.trans, and for each probe below y = 0 pushes the owner up by the depth through
 * DNode_addImpulseAt_00065250 at the (averaged) contact point. */
typedef struct PhysQNode {
    /* 0x000 */ MNode mnode;
    /* 0x0B4 */ AnimPose vqs;
    /* 0x0DC */ AnimPose cvqs;
    /* 0x104 */ s32 unk104;
    /* 0x108 */ s32 owner;          /* DNode * (ctor arg 1) */
    /* 0x10C */ f32 probeA[3];      /* ctor arg 2 */
    /* 0x118 */ f32 probeB[3];      /* ctor arg 3 */
} PhysQNode;

/* Friction (0x170, "<QNode>", vtable Friction_vtbl; Friction_ctor_00063DC4(this, owner, x,
 * y, z): QNode_translate by (x, y, z), offsetLen = |(x, y, z)|). HostIO parms "Friction"
 * (coef, 0..2) and "Friction Limit" (limit, 0..10). Friction_applyContact(this, t,
 * normal, k): while t < 10 it reflects unk114 about the unit normal and, when that is
 * longer than a RoData epsilon, pushes the owner at contactPoint with it scaled by
 * clamp(((coef * owner->friction + k) * K)^2). SnowBCollider_ctor builds one and
 * SnowBCollider_onContact calls vfn11. Friction_reset_00064124 (message 9) clears it. */
typedef struct Friction {
    /* 0x000 */ MNode mnode;
    /* 0x0B4 */ AnimPose vqs;
    /* 0x0DC */ AnimPose cvqs;
    /* 0x104 */ s32 unk104;
    /* 0x108 */ f32 prevContact[3]; /* 0 at reset; contactPoint of the last DNode_updateAttached */
    /* 0x114 */ f32 contactDelta[3]; /* contactPoint - prevContact (DNode_updateAttached): the vector
                                     * vfn11 reflects about the normal */
    /* 0x120 */ f32 contactPoint[3]; /* world point passed to DNode_addImpulseAt_00065250: cvqs.trans
                                     * + owner vel, set by DNode_updateAttached */
    /* 0x12C */ f32 unk12C[3];      /* 0 at reset */
    /* 0x138 */ char pad138[0x144 - 0x138];
    /* 0x144 */ f32 unk144;         /* 0 at reset */
    /* 0x148 */ f32 unk148;         /* 1 at reset */
    /* 0x14C */ s32 unk14C;         /* 0 at reset */
    /* 0x150 */ f32 coef;           /* "Friction" (1 at reset) */
    /* 0x154 */ f32 limit;          /* "Friction Limit" (1 at reset) */
    /* 0x158 */ char pad158[0x160 - 0x158];
    /* 0x160 */ s32 unk160;         /* 0 at reset */
    /* 0x164 */ f32 offsetLen;      /* |ctor offset| */
    /* 0x168 */ s32 unk168;         /* 256 at reset */
    /* 0x16C */ s32 owner;          /* DNode * (ctor arg 1) */
} Friction;

typedef char PhysQNode_size_check[sizeof(PhysQNode) == 0x124 ? 1 : -1];
typedef char Friction_size_check[sizeof(Friction) == 0x170 ? 1 : -1];
typedef char Friction_coef_check[(int)&((Friction *)0)->coef == 0x150 ? 1 : -1];

#define PHYSQNODE(p) ((PhysQNode *)(p))
#define FRICTION(p) ((Friction *)(p))

#endif /* GDL_DNODE_H */
