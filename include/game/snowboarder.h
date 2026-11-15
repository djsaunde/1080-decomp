#ifndef GAME_SNOWBOARDER_H
#define GAME_SNOWBOARDER_H

/*
 * SnowBoarder: the rider object of bootup.uso (C++ class, agent-g run 447).
 *
 * Evidence: SnowBoarder_ctor (bootup 0x90CC) does `this == 0 -> new(0xA8C)`,
 * calls the base Node ctor with the name "SnowB" and stores the vtable
 * (bootup Data 0x83D0) at +0x28. Its vtable slots are SnowBoarder_display
 * (2), SnowBoarder_update (3, the "SnowB" perf section with the SnowBai /
 * SnowBDyn / SnowBAnm / SnowBChild sub-sections), SnowBoarder_handleMsg (4)
 * and SnowBoarder_readParms (8). The physics step SnowBoarder_updateDyn and
 * the collision pass SnowBoarder_procColl take the same `this`.
 *
 * Only the fields below are typed. Their offsets are proven by the matched
 * bodies of SnowBoarder_func_0000DDCC / _0000DF04 / _0000E014 / _0000E124 and
 * (physics: pos / vel / speed / dir / maxSpeed / rival, run 452) _0000B520;
 * everything else is padding. Names stay unkNNN where the meaning is unknown.
 */

/* a 12-byte record returned by the table lookup in the DDCC/DF04/E014 family */
typedef struct SnowBRec3 {
    int a, b, c;
} SnowBRec3;

/* a float 3-vector as SnowBoarder stores it (the TUs' own Vec3 typedefs have the same layout) */
typedef struct SnowBVec3 {
    f32 x, y, z;
} SnowBVec3;

/* a quaternion (x, y, z, w) as DNode stores it (identity w = 1) */
typedef struct SnowBQuat {
    f32 x, y, z, w;
} SnowBQuat;

/* the 0x34-byte force accumulator of the game_libs DNode (DNodeAccum_reset_00064170 /
 * DNodeAccum_addForce_000641DC / DNodeAccum_apply_00064388). addForce(point, force) adds |force| to weight,
 * point * |force| to pointSum, force to force and cross(point, force) / |point| to torque; apply turns the
 * target quat about the normalized torque by |torque| / k * scale and adds force to the target vector. */
typedef struct SnowBAccum {
    f32 weight;             /* 0x00 */
    SnowBVec3 pointSum;     /* 0x04: application points weighted by |force| */
    SnowBVec3 force;        /* 0x10 */
    SnowBVec3 torque;       /* 0x1C */
    SnowBVec3 origin;       /* 0x28: reference point (DNode lastPos), set by reset */
} SnowBAccum;

/* one 0x18-byte float parm (game_libs parm base 0x35B1C + float subclass; same layout as SnowAIParm) */
typedef struct SnowBParm {
    char pad00[0xC];        /* 0x00: owner list link and the 4-char tag (game_libs Parms_addParm) */
    void *vptr;             /* 0x0C: float-parm vtable (bootup Data 0x8018) */
    f32 value;              /* 0x10: ctor default, overwritten by the parms file */
    s32 unk14;              /* 0x14 */
} SnowBParm;

/* one 0x20-byte Vec3 parm of DNode::Parms (vtable bootup Data 0x221F0) */
typedef struct SnowBVec3Parm {
    char pad00[0xC];
    void *vptr;             /* 0x0C */
    SnowBVec3 value;        /* 0x10 */
    s32 unk1C;              /* 0x1C */
} SnowBVec3Parm;

/* "DNode::Parms" (0x180), the game_libs DNode base's parms (DNode_ctor builds it at +0x108; tags at bootup
 * Data 0x223D0). Proven uses: gvt.value is added to vel.y every frame (DNode_integrate_00064DEC, gravity);
 * tsc.value scales both accumulators (DNode_applyAccums_00065448); cog.value is the centre of gravity in
 * body space (DNode_resetAccums_00065148 -> worldCog); imp / spd / vsc are copied to surfImp / surfSpd /
 * surfVsc by DNode_init_00065060 and per surface by SnowBoarder_procColl. */
typedef struct SnowBDNodeParms {
    const char *name;       /* 0x00: "DNode::Parms" */
    s32 unk04;              /* 0x04: parm list head */
    SnowBParm imp, spd, spl, spw, gvt, suc, vsc, usc, tsc; /* 0x08..0xC8 (rider 0x110..0x1D0; gvt value 0x180, tsc value 0x1E0) */
    SnowBVec3Parm cog;      /* 0xE0 (rider 0x1E8; value 0x1F8) */
    SnowBParm unc, dsc, csc, rad; /* 0x100..0x148 */
    SnowBVec3Parm cts;      /* 0x160 (default (1, 1, 0)) */
} SnowBDNodeParms;

/* "SnowBoarder::Parms" (0x2C0), embedded at SnowBoarder +0x540. Members are named by their
 * parms-file tag, in SnowBoarder_ctor construction order (bootup 0x9680..). Meanings are only
 * given where a reader proves them. */
typedef struct SnowBoarderParms {
    const char *name;       /* 0x00: "SnowBoarder::Parms" */
    s32 unk04;              /* 0x04: parm list head */
    SnowBParm hpf;          /* 0x08 (+0x558): on the ground with groundFlags & 1, vel.x += vel.x * hpf * |dir.x| */
    SnowBParm snf, dsf, icf; /* 0x20, 0x38, 0x50: impactScale on surface 'a' / 'b' / other (procColl) */
    SnowBParm tfc, edf, bdf; /* 0x68, 0x80, 0x98: edgeGrip *= edf, friction *= bdf (updateDyn) */
    SnowBParm ure;          /* 0xB0 (+0x600): self-righting gain: rotate about dir / side by localUp.y / .x * ure */
    SnowBParm mro, mpi;     /* 0xC8, 0xE0: dead zones: self-right only while |localUp.y| > mro, |localUp.x| > mpi */
    SnowBParm mdr;          /* 0xF8 (+0x648): counter-steer test: dot(dir, velDir) < mdr cancels the yaw input */
    SnowBParm axr, ayr, azr; /* 0x110..0x140: air rotation rates about side (stick y), up (stick x), dir (stick x) */
    SnowBParm tlt;          /* 0x158 */
    SnowBParm wfr;          /* 0x170 (SnowBoarder +0x6B0): XZ drag; SnowBoarder_limitSpeed scales vel.x / vel.z by 1 - value */
    SnowBParm sfr, dts;     /* 0x188, 0x1A0 (+0x6E0: dts, scaled by SnowAI_scaleRiderParms) */
    SnowBParm nts;          /* 0x1B8 (+0x6F8): steering rate; SnowAI_finishRun divides its turn output by it */
    SnowBParm trt;          /* 0x1D0 (+0x720): edgeSteer decay: edgeSteer -= edgeSteer * trt * turnRate */
    SnowBParm twt, min;     /* 0x1E8, 0x200 */
    SnowBParm max;          /* 0x218 (+0x758): max speed; maxSpeed eases toward max.value (SnowBoarder_limitSpeed);
                             * SnowAI_update sets it per rival character from ms1..ms8 */
    SnowBParm jmp, djp, ufs, bdm, cdm; /* 0x230..0x290 */
    SnowBParm tch;          /* 0x2A8 (+0x7F8): angVel decay factor while groundFlags & 1 is clear (0.724 otherwise) */
} SnowBoarderParms;

/* The 0x154-byte child at SnowBoarder +0x3EC (vtbl bootup Data 0x8108, perf label "SnowBChild"; its vtbl slot 8 is
 * SnowBChild_readParms_0000E720). Its embedded "Parms" (+0x2C = rider +0x418) holds the ground-handling parms that
 * SnowBoarder_updateDyn selects every frame. Tags in construction order; the letters read as n = normal set,
 * d = the alternate set chosen while useDParms is set, i / d (ied, ifr / ded, dfr) = the surface 'c' / 'b' scales. */
typedef struct SnowBChildParms {
    const char *name;       /* 0x00: "Parms" */
    s32 unk04;              /* 0x04: parm list head */
    SnowBParm ied, ifr;     /* 0x08, 0x20 (rider value 0x430, 0x448): surface 'c' scales of edgeGrip / friction */
    SnowBParm ded, dfr;     /* 0x38, 0x50 (0x460, 0x478): surface 'b' scales of edgeGrip / friction */
    SnowBParm efr;          /* 0x68 (0x490): copied to edgeFriction every frame */
    SnowBParm nbf, nel;     /* 0x80, 0x98 (0x4A8, 0x4C0): base friction / edge grip, normal set */
    SnowBParm dbf, del;     /* 0xB0, 0xC8 (0x4D8, 0x4F0): base friction / edge grip, alternate set */
    SnowBParm res;          /* 0xE0 (0x508): turnRate = res * nts (or dts) */
    SnowBParm flx, sta;     /* 0xF8, 0x110 */
} SnowBChildParms;

typedef struct SnowBChild {
    char pad00[0x2C];
    SnowBChildParms parms;  /* 0x2C */
} SnowBChild;

struct SnowAI;
struct RiderAnim;

/* The input object a rider reads (SnowBoarder +0x800): a pad, a replay player or the SnowAI writes it, so the
 * physics never knows who steers (the same object family as InputMgr.pads[], include/game/input.h: held at 0x10).
 * stickX / stickY are -1..1 (updateOrient: steer = stickX * 0.15; updateDyn: crouch = stickY < -0.75). */
typedef struct RiderInput {
    f32 stickX;             /* 0x00 */
    f32 stickY;             /* 0x04 */
    signed char rawX;       /* 0x08: raw stick x (CamDirector_update passes it to the manual-mode hook) */
    signed char rawY;       /* 0x09 */
    char pad0A[0x10 - 0x0A];
    s32 held;               /* 0x10: held buttons: 4 / 8 nudge (limitSpeed), 0x200 air spin, 0x400 alternate parms;
                             * CamDirector: 8 / 4 fov up / down, 0x400 / 0x1000 to the manual hook */
    s32 unk14;
    s32 pressed;            /* 0x18: buttons pressed this frame: CamDirector 0x40 toggles manual mode, CourseCam
                             * 0x40 cycles chaseMode. pad_update also sets stick-direction bits in held (and so
                             * in pressed): 0x2000 up, 0x4000 right, 0x8000 down, 0x10000 left (|stick| > 0.5) */
    char pad1C[0x7C - 0x1C];
    s16 stickDir;           /* 0x7C: stick direction code, 0 = centred, else 1..8 around the circle: pad_update
                             * (game_libs 0x675A4) looks the four direction bits (held >> 13 & 0xF) up in a byte
                             * table. RiderAnim_readStickSpin_0000EAF4 counts its steps to detect stick spins */
} RiderInput;

typedef struct SnowBoarder {
    /* 0x000..0x3D4: the game_libs DNode base (DNode_ctor, DNode_update) */
    char pad000[0x0B4];
    SnowBVec3 pos;          /* 0x0B4: world position (the node position: the ctor also sets a child node's
                             * +0xB4 to (0, 100, 0)). SnowBoarder_limitSpeed compares rival->pos.z - pos.z;
                             * SnowAI (game.uso) seeds its target from rider->pos. DNode_integrate adds vel */
    char pad0C0[0x0CC - 0x0C0];
    SnowBQuat orient;       /* 0x0CC: orientation; DNode_integrate does orient = angVel * orient,
                             * SnowBoarder_updateOrient turns it with math_quatRotateAxisAngle */
    SnowBVec3 lastPos;      /* 0x0DC: pos at the end of the last DNode_integrate (accumulator origin, the
                             * ground-query origin in procColl) */
    char pad0E8[0x0F4 - 0x0E8];
    SnowBQuat lastOrient;   /* 0x0F4: orient at the end of the last integrate */
    char pad104[0x108 - 0x104];
    SnowBDNodeParms dparms; /* 0x108: "DNode::Parms" */
    f32 surfImp;            /* 0x288: dparms.imp, or the surface value set by procColl */
    f32 surfSpd;            /* 0x28C: dparms.spd, or the surface value */
    f32 surfVsc;            /* 0x290: dparms.vsc, or the surface value */
    SnowBAccum impulseAcc;  /* 0x294: applied to angVel / vel (DNode_addImpulseAt_00065250) */
    SnowBAccum moveAcc;     /* 0x2C8: applied to orient / pos directly (DNode_addMoveAt_000652D8) */
    SnowBQuat angVel;       /* 0x2FC: per-frame rotation quat; updateOrient resets it to identity in the air and
                             * decays it (xyz * k, w eased to 1) every frame */
    SnowBVec3 velDir;       /* 0x30C: vel / speed (0 when stopped) */
    SnowBVec3 vel;          /* 0x318: velocity. B520 adds dir * nudge, then rescales it to maxSpeed
                             * (normalize + scale) when speed > maxSpeed */
    SnowBVec3 worldCog;     /* 0x324: dparms.cog in world space (DNode_resetAccums) */
    SnowBVec3 velDelta;     /* 0x330: vel - lastVel of the last integrate */
    SnowBVec3 lastVel;      /* 0x33C: vel after the last integrate; procColl's impact test dots it with the contact normal */
    f32 speed;              /* 0x348: |vel|, compared against maxSpeed (B520) and 30 / 10 thresholds */
    char pad34C[0x360 - 0x34C];
    f32 friction;           /* 0x360: ground friction, rebuilt each frame by SnowBoarder_updateDyn: nbf / dbf, + |spin.x| * k
                             * while crouching, * ifr / dfr on surface 'c' / 'b', + k off the ground, * parms.bdf,
                             * + slowdown / 100 */
    char pad364[0x38C - 0x364];
    s32 unk38C;             /* 0x38C: set to 1 by SnowBoarder_ctor */
    char pad390[0x3A4 - 0x390];
    SnowBVec3 startPos;     /* 0x3A4: spawn point: ctor args (x, z), y = ground height at (x, z) + a RoData offset */
    SnowBVec3 up;           /* 0x3B0: body up axis (row of the orientation matrix); up.y < 0.8 = steep */
    SnowBVec3 side;         /* 0x3BC: body side axis */
    SnowBVec3 dir;          /* 0x3C8: unit forward direction (B520 nudge axis; dotted with vel in updateDyn) */
    char pad3D4[0x3EC - 0x3D4];
    SnowBChild child;       /* 0x3EC: the "SnowBChild" sub-object and its ground-handling Parms */
    SnowBoarderParms parms; /* 0x540: "SnowBoarder::Parms" (the ctor builds it in place) */
    RiderInput *input;      /* 0x800: input object (SnowBoarder_ctor arg 4: the pad / replay / AI input built
                             * by input_addRacePad); its +0x10 word carries the direction bits 4 / 8 (B520) */
    struct SnowAI *ai;      /* 0x804: the AI controller (game.uso SnowAI_ctor(0, this, ...), the ctor's last child);
                             * SnowBoarder_handleMsg forwards win / lose / timeout to SnowAI_onWin / onLose / onTimeout */
    char sound[0xC];        /* 0x808: embedded 0xC-byte sound object, constructed in place with (ctor arg 1,
                             * ctor arg 2); updateOrient calls snd_soundObjSetScaled_00000B2C on it, procColl
                             * switches it with groundFlags & 0x400, update feeds it contactSurface / landImpact */
    char *unk814;           /* 0x814: ctor: word 0 of the bu_90cc_fz source table */
    char *unk818;           /* 0x818: ctor: word 2 of that table; update stores landImpact at +0x94 when unk8B8 & 4 */
    char *unk81C;           /* 0x81C: ctor: word 1; update stores landImpact at +0x94 when unk8B8 & 1 */
    char *bodyNodes[7];     /* 0x820..0x838: rider model nodes read by the ctor from the shape-file node registry
                             * (gNodeGlobals slots). SnowBoarder_update jitters the +0x64 float of [1] [2] [4] by
                             * the shake amount, [5] [3] by half of it and [6] by minus half (agent-h run 496) */
    char pad83C[0x840 - 0x83C];
    struct RiderAnim *anim; /* 0x840: the RiderAnim (game.uso C48C) built by the ctor; the SnowAI run-out steps and
                             * SnowBoarder_handleMsg's ending ("en1" / "en2") call its state methods */
    char *unk844;           /* 0x844: child object A; its +0x30 float is negated on a mirror flip */
    s32 controller;         /* 0x848: ctor arg 1 (also passed to the 0x808 member). 0 = pad rider, 2 = CPU rider
                             * (game_setupVsCpu; SnowAI_update and SnowAI_onLose test == 2); 1 = other (not yet proven) */
    char pad84C[0x850 - 0x84C];
    char *light;            /* 0x850: a light object; procColl passes unk91C to it (light_setF124_000550A0) */
    char pad854[0x864 - 0x854];
    char *unk864;           /* 0x864: child object B (may be NULL); transform at +0x30 */
    char pad868[0x874 - 0x868];
    char *camSource;        /* 0x874: a node of the rider model: slot 22 of the shape-file node registry
                             * (bootup Bss 0x3C058, Sym 550, 40 slots; shape cmd 0x67 registers the current
                             * node in a slot, cmd 0x66 parents to one: Node_doCommand), read by the ctor
                             * right after the model is built, like 0x820..0x864 (agent-g run 488).
                             * cam_create passes it as the SnowChaseActorCam source, where a mode-3
                             * (fixed) eye parks */
    char pad878[0x890 - 0x878];
    SnowBVec3 prevPos;      /* 0x890: pos saved at the start of SnowBoarder_procColl */
    SnowBVec3 prevVel;      /* 0x89C: vel saved at the start of SnowBoarder_procColl */
    char pad8A8[0x8B8 - 0x8A8];
    s32 unk8B8;             /* 0x8B8: bit mask set by the ctor; bits 1 / 4 pick which of unk81C / unk818 gets landImpact */
    char pad8BC[0x8C0 - 0x8BC];
    s32 unk8C0;             /* 0x8C0: 1 << character (one-hot mask, set by the ctor) */
    u32 character;          /* 0x8C4: rider character 1..8: ctor arg 2 = game_createPlayers' index + 1; SnowAI_update
                             * picks ms1..ms8 by it; 6 and 7 get special ctor handling and DDCC skips them */
    char pad8C8[0x8DC - 0x8C8];
    char *camera;           /* 0x8DC: the race camera built from the mode's 0x1C-byte descriptor row; every
                             * game_setup* stores it (vs-ghost / demo-race share one between both riders).
                             * SnowBoarder_update passes it to the 0x808 sound update when non-zero */
    f32 tint[4];            /* 0x8E0: r, g, b, a as 0..1 (ctor: colour byte / 255, alpha 255 / 164 / 117 / 70) */
    char pad8F0[0x8F4 - 0x8F0];
    SnowBRec3 *unk8F4[3];   /* 0x8F4: three record slots refilled from the table by DDCC/DF04/E014 */
    s16 unk900;             /* 0x900: table index used when (flags & SNOWB_FLAG_MIRROR) == 0 */
    s16 unk902;             /* 0x902: table index used when mirrored */
    char pad904[0x908 - 0x904];
    struct SnowBoarder *rival; /* 0x908: the other rider; game_setupVsCpu links the two riders through it,
                                * B520's catch-up compares positions with it */
    char pad90C[0x910 - 0x90C];
    f32 impactScale;        /* 0x910: parms snf / dsf / icf by groundSurface; scales the bump* response */
    char pad914[0x91C - 0x914];
    f32 unk91C;             /* 0x91C: eased toward a RoData value while groundFlags & 0x400, else toward 1.0,
                             * and passed to the 0x850 object (not in game mode 2) */
    SnowBVec3 penetration;  /* 0x920: contactNormal * contactDepth while contactDepth < 0, else 0 */
    SnowBVec3 impactVel;    /* 0x92C: contactNormal * -min(dot(lastVel, contactNormal), 0) */
    s32 contactCount;       /* 0x938: collider contacts this frame: SnowBoarder_procColl clears it, SnowBCollider_onContact
                             * increments it; SnowBoarder_limitSpeed applies the wfr air drag only while it is 0 */
    SnowBVec3 contactNormal; /* 0x93C: summed contact directions, divided by contactCount in procColl */
    SnowBVec3 lastContactNormal; /* 0x948: the averaged contactNormal of the last frame with contacts */
    char pad954[0x95C - 0x954];
    s32 noGroundFrames;     /* 0x95C: frames the ground query found nothing; after 151 procColl respawns the
                             * rider 200 above the nearest course point (DNode_resetXform) */
    s32 slowdown;           /* 0x960: percent: updateDyn zeroes edgeGrip, adds slowdown / 100 to friction and scales vel.xz
                             * by 1 - sfr; 100 with speed < 30 stops the rider (RiderAnim sets 100; st / sl parms drive it) */
    char pad964[0x968 - 0x964];
    f32 contactDepth;       /* 0x968: summed contact amounts, averaged like contactNormal */
    char pad96C[0x970 - 0x96C];
    f32 edge;               /* 0x970: board edge angle, edgeSteer + edgeSlope clamped to [-1, 1] (negated when
                             * mirrored); RiderAnim_selectAnim tests |edge|, crouching adds |edge| * k to friction,
                             * shake = 20 + 30 * |edge| on the ground */
    f32 edgeSteer;          /* 0x974: steering part: decays by trt * turnRate, gains stick x * 0.15 * turnRate */
    f32 edgeSlope;          /* 0x978: slope part: eased (0.4) toward -side.y (0 while crouching); negated on a mirror
                             * flip. RiderAnim_clearRiderEdge_00011868 zeroes all three */
    char pad97C[0x990 - 0x97C];
    s32 crouch;             /* 0x990: input stick y < -0.75 (updateDyn); adds |spin.x| * k to friction */
    char pad994[0x9A0 - 0x994];
    s16 groundSurface;      /* 0x9A0: surface code of the ground below (procColl ground query) */
    s16 surface;            /* 0x9A2: groundSurface while close enough to the ground, else 0. updateDyn: 'a' (0x61) plain,
                             * 'b' / 'c' scale by ded, dfr / ied, ifr, 0 (airborne) zeroes the edge terms */
    s16 contactSurface;     /* 0x9A4: surface code of the last object contact */
    char pad9A6[0x9A8 - 0x9A6];
    s32 groundFlags;        /* 0x9A8: ground attribute bits; 0x400 switches the sub808 member on */
    char pad9AC[0x9CC - 0x9AC];
    s32 useDParms;          /* 0x9CC: input bit 0x400 held: updateDyn uses the d* parms (dbf, del, dts) instead of n* */
    f32 groundDist;         /* 0x9D0: distance to the ground below (a RoData default when nothing is found) */
    s32 airFrames;          /* 0x9D4: frames since the last collider contact (procColl) */
    SnowBVec3 groundNormal; /* 0x9D8: ground normal under the rider while onGround, else (0, 1, 0) */
    f32 groundPlaneD;       /* 0x9E4: plane distance of the ground hit */
    char pad9E8[0x9F8 - 0x9E8];
    f32 edgeFriction;       /* 0x9F8: child parms efr; 0 off the ground */
    f32 edgeGrip;           /* 0x9FC: nel / del, * ied / ded on 'c' / 'b', * parms.edf; 0 off the ground or while slowed */
    f32 turnRate;           /* 0xA00: res * nts (or dts with useDParms) */
    SnowBVec3 localUp;      /* 0xA04: the target up vector in body axes (dot with dir, -side, up): world up while
                             * groundFlags & 1 or !groundGentle, else groundNormal. .x / .y drive the self-righting */
    u16 hitFlags;           /* 0xA10: SnowBoarder_handleMsg msg 20 (bumped by an object) sets 0x1F0;
                             * RiderAnim_selectAnim picks a stumble anim while (hitFlags & 0x1F0) */
    char padA12[0xA14 - 0xA12];
    s32 health;             /* 0xA14: damage meter; RiderAnim_applyDamage_0000EDD4 subtracts dm1..dm4;
                             * game_setupVsCpu gives the CPU rider 10000000 */
    s32 unkA18;             /* 0xA18: non-zero enables the stick trick rotations (axr / ayr / azr) in the air */
    f32 bumpSide;           /* 0xA1C: impact response along side: procColl adds dot(side, impactVel) * impactScale,
                             * msg 20 adds the bump; selectAnim compares |bumpSide| with RiderAnim parms to
                             * choose a stumble side; cleared each update */
    f32 bumpUp;             /* 0xA20: dot(up, impactVel) * impactScale */
    f32 bumpFwd;            /* 0xA24: minus dot(dir, impactVel) */
    s32 unkA28;             /* 0xA28: toggled by pad button 0x20 in SnowBoarder_update (pause latch) */
    char padA2C[0xA30 - 0xA2C];
    u8 groundColor[4];      /* 0xA30: colour bytes of the ground triangle (procColl; display reads r, g, b) */
    char padA34[0xA38 - 0xA34];
    f32 fwdSpeed;           /* 0xA38: dot(dir, vel) (SnowBoarder_update); cleared on a mirror flip */
    char padA3C[0xA4C - 0xA3C];
    s32 onGround;           /* 0xA4C: ground hit within reach of lastPos, or collider contacts */
    s32 groundGentle;       /* 0xA50: onGround and groundNormal.y above a RoData threshold */
    s32 raceResult;         /* 0xA54: SnowBoarder_handleMsg: 1 win (msg 1000), 2 lose (1001), 3 timeout (1003),
                             * 4 (1002), 5 (1004), 6 ending (1005..1007) */
    u32 flags;              /* 0xA58: SNOWB_FLAG_* */
    s32 shake;              /* 0xA5C: percent: random jitter of six body nodes (update); 20 + 30 * |edge| on the ground,
                             * +20 on a counter-steer bump, 100 / 70 on surface 'd' / 'c' (procColl) */
    char padA60[0xA6C - 0xA60];
    s16 spinHold;           /* 0xA6C: frames pad button 0x200 has been held in the air with groundFlags & 1 */
    char padA6E[0xA70 - 0xA6E];
    f32 airSpin;            /* 0xA70: yaw rate in the air: after 11 frames of spinHold, += stick x * 0.015, clamped +-0.2 */
    f32 unkA74;             /* 0xA74: cleared on the ground */
    f32 maxSpeed;           /* 0xA78: speed cap; eased toward baseMaxSpeed (+-catch-up) by B520 */
    char padA7C[0xA80 - 0xA7C];
    s32 msgCooldown;        /* 0xA80: decremented each procColl; SnowBoarder_handleMsg ignores some messages while set */
    s32 landImpact;         /* 0xA84: (|vel.y| - 100) / k, >= 0, passed to the sound object (flags 0x40000) */
    char padA88[0xA8C - 0xA88];
} SnowBoarder;

/* Field access through an untyped `char *` rider pointer whose type is a lever (agent-h run 496).
 * Lowers to the same base + offset access as the raw `*(T *)(p + K)` form. */
#define SNOWB(p) ((SnowBoarder *)(p))

#define SNOWB_FLAG_08     0x08  /* SnowBoarder_mirrorFlip_0000E124: selects the notify-call form */
#define SNOWB_FLAG_20     0x20  /* SnowBoarder_mirrorFlip_0000E124: gates the 180-degree re-orient */
#define SNOWB_FLAG_MIRROR 0x80  /* left/right stance flip, toggled by SnowBoarder_mirrorFlip_0000E124 */
#define SNOWB_FLAG_NUDGE  0x100 /* SnowBoarder_limitSpeed: input bits 4 / 8 push the rider along dir */
#define SNOWB_FLAG_CATCHUP 0x20000 /* SnowBoarder_limitSpeed: maxSpeed gets +-k when the rival is > k' ahead / behind in z */

#ifndef __cplusplus /* edg (the NCC donors include this header too) rejects the &((T *)0)->f constant form */
/* compile-time offset checks (array size -1 => error) */
#define SNOWB_ASSERT_OFFSET(field, off) \
    typedef char snowb_assert_##off[(int)&((SnowBoarder *)0)->field == (off) ? 1 : -1]
SNOWB_ASSERT_OFFSET(pos, 0x0B4);
SNOWB_ASSERT_OFFSET(vel, 0x318);
SNOWB_ASSERT_OFFSET(speed, 0x348);
SNOWB_ASSERT_OFFSET(unk38C, 0x38C);
SNOWB_ASSERT_OFFSET(startPos, 0x3A4);
SNOWB_ASSERT_OFFSET(dir, 0x3C8);
SNOWB_ASSERT_OFFSET(parms, 0x540);
SNOWB_ASSERT_OFFSET(parms.wfr, 0x6B0);
SNOWB_ASSERT_OFFSET(parms.nts, 0x6F8);
SNOWB_ASSERT_OFFSET(parms.max, 0x758);
SNOWB_ASSERT_OFFSET(input, 0x800);
SNOWB_ASSERT_OFFSET(ai, 0x804);
SNOWB_ASSERT_OFFSET(bodyNodes[6], 0x838);
SNOWB_ASSERT_OFFSET(anim, 0x840);
SNOWB_ASSERT_OFFSET(unk8B8, 0x8B8);
SNOWB_ASSERT_OFFSET(camera, 0x8DC);
SNOWB_ASSERT_OFFSET(sound, 0x808);
SNOWB_ASSERT_OFFSET(unk844, 0x844);
SNOWB_ASSERT_OFFSET(controller, 0x848);
SNOWB_ASSERT_OFFSET(light, 0x850);
SNOWB_ASSERT_OFFSET(unk864, 0x864);
SNOWB_ASSERT_OFFSET(camSource, 0x874);
SNOWB_ASSERT_OFFSET(unk8C0, 0x8C0);
SNOWB_ASSERT_OFFSET(character, 0x8C4);
SNOWB_ASSERT_OFFSET(tint, 0x8E0);
SNOWB_ASSERT_OFFSET(unk8F4, 0x8F4);
SNOWB_ASSERT_OFFSET(unk900, 0x900);
SNOWB_ASSERT_OFFSET(unk902, 0x902);
SNOWB_ASSERT_OFFSET(rival, 0x908);
SNOWB_ASSERT_OFFSET(friction, 0x360);
SNOWB_ASSERT_OFFSET(child, 0x3EC);
SNOWB_ASSERT_OFFSET(child.parms.efr, 0x480);
SNOWB_ASSERT_OFFSET(child.parms.nbf, 0x498);
SNOWB_ASSERT_OFFSET(child.parms.res, 0x4F8);
SNOWB_ASSERT_OFFSET(prevVel, 0x89C);
SNOWB_ASSERT_OFFSET(contactCount, 0x938);
SNOWB_ASSERT_OFFSET(contactNormal, 0x93C);
SNOWB_ASSERT_OFFSET(slowdown, 0x960);
SNOWB_ASSERT_OFFSET(contactDepth, 0x968);
SNOWB_ASSERT_OFFSET(crouch, 0x990);
SNOWB_ASSERT_OFFSET(groundSurface, 0x9A0);
SNOWB_ASSERT_OFFSET(surface, 0x9A2);
SNOWB_ASSERT_OFFSET(groundFlags, 0x9A8);
SNOWB_ASSERT_OFFSET(useDParms, 0x9CC);
SNOWB_ASSERT_OFFSET(groundDist, 0x9D0);
SNOWB_ASSERT_OFFSET(edgeFriction, 0x9F8);
SNOWB_ASSERT_OFFSET(edgeGrip, 0x9FC);
SNOWB_ASSERT_OFFSET(turnRate, 0xA00);
SNOWB_ASSERT_OFFSET(edge, 0x970);
SNOWB_ASSERT_OFFSET(edgeSlope, 0x978);
SNOWB_ASSERT_OFFSET(hitFlags, 0xA10);
SNOWB_ASSERT_OFFSET(health, 0xA14);
SNOWB_ASSERT_OFFSET(bumpSide, 0xA1C);
SNOWB_ASSERT_OFFSET(bumpFwd, 0xA24);
SNOWB_ASSERT_OFFSET(raceResult, 0xA54);
SNOWB_ASSERT_OFFSET(fwdSpeed, 0xA38);
SNOWB_ASSERT_OFFSET(flags, 0xA58);
SNOWB_ASSERT_OFFSET(maxSpeed, 0xA78);
SNOWB_ASSERT_OFFSET(orient, 0x0CC);
SNOWB_ASSERT_OFFSET(lastPos, 0x0DC);
SNOWB_ASSERT_OFFSET(lastOrient, 0x0F4);
SNOWB_ASSERT_OFFSET(dparms, 0x108);
SNOWB_ASSERT_OFFSET(dparms.gvt, 0x170);
SNOWB_ASSERT_OFFSET(dparms.tsc, 0x1D0);
SNOWB_ASSERT_OFFSET(dparms.cog, 0x1E8);
SNOWB_ASSERT_OFFSET(surfImp, 0x288);
SNOWB_ASSERT_OFFSET(impulseAcc, 0x294);
SNOWB_ASSERT_OFFSET(moveAcc, 0x2C8);
SNOWB_ASSERT_OFFSET(angVel, 0x2FC);
SNOWB_ASSERT_OFFSET(velDir, 0x30C);
SNOWB_ASSERT_OFFSET(worldCog, 0x324);
SNOWB_ASSERT_OFFSET(lastVel, 0x33C);
SNOWB_ASSERT_OFFSET(up, 0x3B0);
SNOWB_ASSERT_OFFSET(side, 0x3BC);
SNOWB_ASSERT_OFFSET(parms.hpf, 0x548);
SNOWB_ASSERT_OFFSET(parms.trt, 0x710);
SNOWB_ASSERT_OFFSET(parms.tch, 0x7E8);
SNOWB_ASSERT_OFFSET(prevPos, 0x890);
SNOWB_ASSERT_OFFSET(impactScale, 0x910);
SNOWB_ASSERT_OFFSET(unk91C, 0x91C);
SNOWB_ASSERT_OFFSET(penetration, 0x920);
SNOWB_ASSERT_OFFSET(impactVel, 0x92C);
SNOWB_ASSERT_OFFSET(lastContactNormal, 0x948);
SNOWB_ASSERT_OFFSET(noGroundFrames, 0x95C);
SNOWB_ASSERT_OFFSET(airFrames, 0x9D4);
SNOWB_ASSERT_OFFSET(groundNormal, 0x9D8);
SNOWB_ASSERT_OFFSET(groundPlaneD, 0x9E4);
SNOWB_ASSERT_OFFSET(localUp, 0xA04);
SNOWB_ASSERT_OFFSET(unkA18, 0xA18);
SNOWB_ASSERT_OFFSET(groundColor, 0xA30);
SNOWB_ASSERT_OFFSET(onGround, 0xA4C);
SNOWB_ASSERT_OFFSET(groundGentle, 0xA50);
SNOWB_ASSERT_OFFSET(shake, 0xA5C);
SNOWB_ASSERT_OFFSET(spinHold, 0xA6C);
SNOWB_ASSERT_OFFSET(airSpin, 0xA70);
SNOWB_ASSERT_OFFSET(msgCooldown, 0xA80);
SNOWB_ASSERT_OFFSET(landImpact, 0xA84);
typedef char snowb_assert_accum_size[sizeof(SnowBAccum) == 0x34 ? 1 : -1];
typedef char snowb_assert_dparms_size[sizeof(SnowBDNodeParms) == 0x180 ? 1 : -1];
typedef char snowb_assert_parm_size[sizeof(SnowBParm) == 0x18 ? 1 : -1];
typedef char snowb_assert_child_size[sizeof(SnowBChild) == 0x154 ? 1 : -1];
typedef char snowb_assert_parms_size[sizeof(SnowBoarderParms) == 0x2C0 ? 1 : -1];
typedef char snowb_assert_size[sizeof(SnowBoarder) == 0xA8C ? 1 : -1];
typedef char snowb_assert_input_held[(int)&((RiderInput *)0)->held == 0x10 ? 1 : -1];
typedef char snowb_assert_input_pressed[(int)&((RiderInput *)0)->pressed == 0x18 ? 1 : -1];
#endif

#endif /* GAME_SNOWBOARDER_H */
