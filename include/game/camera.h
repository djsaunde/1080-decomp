#ifndef GAME_CAMERA_H
#define GAME_CAMERA_H

/*
 * The race camera (agent-g run 475). docs/1080_CAMERA.md explains how it follows the rider.
 *
 * The camera is two physics-free bodies, an EYE and a LOOK-AT point, each a libgdl matrix node
 * (CamMNode). Each body has a "chase" component child (a Node whose parent, +0x14, is the body):
 *   - SnowChaseActorCam (game.uso, 0x124) moves the eye: it keeps it txz behind the rider in the
 *     horizontal plane and toy above (ty2 in mode 2), with spring gains cxz / chy;
 *   - SnowChaseActorLook (game.uso, 0x64) moves the look-at point toward the rider (mode 1) or
 *     1000 units ahead of / behind the rider (modes 2 / 3), with gain ch0;
 *   - SnowChaseActor (game.uso, 0x138) is the one-component variant ("LookAtChase", bootup
 *     cam_create mode 1): it chases a rotated offset `off` from the target and keeps xxz..mxz from a
 *     second body.
 * The components move their body by adding a translation to its local matrix
 * (math_mtxAddTranslation on body->local, game_uso_func_072EE8 in the donors). There is no explicit
 * velocity: every update moves the body by gain * error, an exponential ease toward the goal.
 *
 * Parms tags and order are from the Parms ctors (game.uso Data 0x318.. / 0x354.. / 0x374..).
 * Every component's mode is set by its setMode (0 = off; applyMode snaps the body to the goal).
 */

/* a float 3-vector (same layout as the TUs' Vec3 / snowboarder.h SnowBVec3) */
typedef struct CamVec3 {
    f32 x, y, z;
} CamVec3;

/* the libgdl matrix node the camera bodies (and the rider's transform) are. Only the two matrices
 * are known: local (0x30) is what math_mtxAddTranslation moves; world (0x70) is read for positions
 * (row 3 = translation) and for the rider's axes (rows 0..2: cam_toTargetLocal_000023D4). */
typedef struct CamMNode {
    char node[0x2C];        /* 0x00: Node (include/gdl/node.h); +0x14 = parent */
    char pad2C[0x30 - 0x2C];
    f32 local[4][4];        /* 0x30: local matrix; local[3] = the body's own translation */
    f32 world[4][4];        /* 0x70: world matrix; world[3][0..2] = world position */
} CamMNode;

/* one 0x18-byte float parm (the same layout as snowboarder.h SnowBParm) */
typedef struct CamParm {
    char pad00[0xC];        /* 0x00: owner list link and the 4-char tag */
    void *vptr;             /* 0x0C */
    f32 value;              /* 0x10: ctor default, overwritten by the parms file */
    s32 unk14;              /* 0x14 */
} CamParm;

/* one 0x20-byte Vec3 parm */
typedef struct CamVec3Parm {
    char pad00[0xC];
    void *vptr;             /* 0x0C */
    CamVec3 value;          /* 0x10 */
    s32 unk1C;              /* 0x1C */
} CamVec3Parm;

/* "Parms" of SnowChaseActorCam (0xD0, SnowChaseActorCamParms_ctor; tags at game.uso Data 0x320). */
typedef struct SnowChaseActorCamParms {
    const char *name;       /* 0x00: "Parms" */
    s32 unk04;              /* 0x04: parm list head */
    CamVec3Parm cof;        /* 0x08 (cam value 0x5C): offset from the rider's origin, in the rider's axes, to the
                             * point the eye aims for (default (0, 64, 0)); cam_toTargetLocal_000023D4 rotates it */
    CamParm mxz;            /* 0x28 (0x7C): minimum horizontal eye distance; closer pushes the eye out at full rate */
    CamParm txz;            /* 0x40 (0x94): target horizontal eye distance behind the aim point */
    CamParm cxz;            /* 0x58 (0xAC): horizontal spring gain toward txz */
    CamParm miy;            /* 0x70 (0xC4): minimum eye height above the aim point */
    CamParm toy;            /* 0x88 (0xDC): target eye height (modes 1 and 3) */
    CamParm ty2;            /* 0xA0 (0xF4): target eye height in mode 2 */
    CamParm chy;            /* 0xB8 (0x10C): vertical spring gain toward toy / ty2 */
} SnowChaseActorCamParms;

/* SnowChaseActorCam (0x124, node name "SnowChaseActorCam" at game.uso Data 0x340; SnowChaseActorCam_ctor(this, target, source, parent)):
 * moves its parent body (the eye). */
typedef struct SnowChaseActorCam {
    char node[0x14];
    CamMNode *body;         /* 0x14: Node parent = the eye body it moves */
    char pad18[0x2C - 0x18];
    CamVec3 eyeOffset;      /* 0x2C: last desired eye offset from the aim point: horizontal (dir * txz), then minus
                             * up * height; cleared by reset */
    CamMNode *target;       /* 0x38: the followed body (the rider); its world matrix positions and orients cof */
    CamMNode *source;       /* 0x3C: mode 3 parks the eye at this body's position */
    s32 mode;               /* 0x40: 0 off, 1 chase (toy), 2 chase (ty2), 3 fixed at source */
    SnowChaseActorCamParms parms; /* 0x44 */
    char pad114[0x124 - 0x114];
} SnowChaseActorCam;

/* "Parms" of SnowChaseActorLook (0x20; tag "ch0" at game.uso Data 0x35C) */
typedef struct SnowChaseActorLookParms {
    const char *name;       /* 0x00: "Parms" */
    s32 unk04;              /* 0x04 */
    CamParm ch0;            /* 0x08 (look value 0x5C): the gain: each frame the look-at point moves ch0 * distance
                             * toward its goal */
} SnowChaseActorLookParms;

/* SnowChaseActorLook (0x64, node name at Data 0x360; SnowChaseActorLook_ctor(this, anchor, cam, parent)): moves its parent body (the look-at point). */
typedef struct SnowChaseActorLook {
    char node[0x14];
    CamMNode *body;         /* 0x14: the look-at body it moves */
    char pad18[0x2C - 0x18];
    CamVec3 unk2C;          /* 0x2C: cleared by reset */
    CamMNode *anchor;       /* 0x38: modes 2 / 3: the goal is this body's position + cam->target's z axis * 1000
                             * (mode 2) or * -1000 (mode 3) */
    SnowChaseActorCam *cam; /* 0x3C: mode 1: look at cam's aim point (cam->target + rotated cof) */
    s32 mode;               /* 0x40: 0 off, 1 aim point, 2 ahead, 3 behind */
    SnowChaseActorLookParms parms; /* 0x44 */
} SnowChaseActorLook;

/* "Parms" of SnowChaseActor (0xE8, SnowChaseActorParms_ctor; tags at game.uso Data 0x37C) */
typedef struct SnowChaseActorParms {
    const char *name;       /* 0x00: "Parms" */
    s32 unk04;
    CamVec3Parm off;        /* 0x08 (actor value 0x68): goal offset in the target's axes */
    CamParm xxz;            /* 0x28 (0x88): maximum horizontal distance from the look body (pulled in beyond it) */
    CamParm mxz;            /* 0x40 (0xA0): minimum horizontal distance from the look body (pushed out below it) */
    CamParm txz;            /* 0x58 (0xB8): target distance to the goal */
    CamParm mxy, miy, toy;  /* 0x70, 0x88, 0xA0 (0xD0, 0xE8, 0x100): not read by SnowChaseActor_update */
    CamParm cxz;            /* 0xB8 (0x118): gain toward the goal distance */
    CamParm chy;            /* 0xD0 (0x130): vertical gain (the goal's height difference) */
} SnowChaseActorParms;

/* SnowChaseActor (0x138, node name at Data 0x3A0; SnowChaseActor_ctor(this, target, lookBody, parent)) */
typedef struct SnowChaseActor {
    char node[0x14];
    CamMNode *body;         /* 0x14: the body it moves */
    char pad18[0x2C - 0x18];
    s32 unk2C;              /* 0x2C: cleared by the ctor */
    CamVec3 unk30;          /* 0x30: cleared by the ctor */
    CamVec3 toGoal;         /* 0x3C: goal - body position of the last update */
    CamMNode *target;       /* 0x48: the followed body (goal = its position + its rotation * off) */
    CamMNode *lookBody;     /* 0x4C: the body whose horizontal distance is kept in [mxz, xxz] */
    SnowChaseActorParms parms; /* 0x50 */
} SnowChaseActor;

#ifndef __cplusplus /* edg (the NCC donors) rejects the &((T *)0)->f constant form */
#define CAM_ASSERT(T, f, off) typedef char cam_assert_##T##_##off[(int)&((T *)0)->f == (off) ? 1 : -1]
CAM_ASSERT(CamMNode, local, 0x30);
CAM_ASSERT(CamMNode, world, 0x70);
CAM_ASSERT(SnowChaseActorCam, eyeOffset, 0x2C);
CAM_ASSERT(SnowChaseActorCam, mode, 0x40);
CAM_ASSERT(SnowChaseActorCam, parms, 0x44);
CAM_ASSERT(SnowChaseActorCam, parms.cof.value, 0x5C);
CAM_ASSERT(SnowChaseActorCam, parms.mxz.value, 0x7C);
CAM_ASSERT(SnowChaseActorCam, parms.chy.value, 0x10C);
CAM_ASSERT(SnowChaseActorLook, parms.ch0.value, 0x5C);
CAM_ASSERT(SnowChaseActor, toGoal, 0x3C);
CAM_ASSERT(SnowChaseActor, target, 0x48);
CAM_ASSERT(SnowChaseActor, parms.off.value, 0x68);
CAM_ASSERT(SnowChaseActor, parms.xxz.value, 0x88);
CAM_ASSERT(SnowChaseActor, parms.cxz.value, 0x118);
CAM_ASSERT(SnowChaseActor, parms.chy.value, 0x130);
typedef char cam_assert_cam_size[sizeof(SnowChaseActorCam) == 0x124 ? 1 : -1];
typedef char cam_assert_look_size[sizeof(SnowChaseActorLook) == 0x64 ? 1 : -1];
typedef char cam_assert_actor_size[sizeof(SnowChaseActor) == 0x138 ? 1 : -1];
typedef char cam_assert_camparms_size[sizeof(SnowChaseActorCamParms) == 0xD0 ? 1 : -1];
#endif

#endif /* GAME_CAMERA_H */
