#ifndef GAME_CAMERA_H
#define GAME_CAMERA_H

struct SnowBoarder;

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
    s32 unkB0;              /* 0xB0 */
} CamMNode;                 /* 0xB4 (the MNode alloc size in MCamera_ctor gl 32E18) */

/* MCamera: the libgdl "camera" node (game_libs ctor gl_func_00032E18(this, target, x, y, z), 0x150). Every 3D view's
 * camera derives from it; the per-frame lookAt (gl 33094, NCC donor game_libs_ncc_33094.cpp) builds `view` from the
 * eye (world[3]) toward target's world position (or eye + lookDir * 10 without a target), then turns it by
 * pitch / yaw while rotEnable. Field names are from those bodies (agent-g run 475; the ctor defaults in brackets). */
typedef struct MCamera {
    CamMNode mnode;         /* 0x00: the eye: local[3] / world[3] = eye position (the ctor sets local[3] = (x, y, z)) */
    f32 view[4][4];         /* 0xB4: view matrix (lookAt output; CamFollow / FreeCam read its rows as the camera axes) */
    CamMNode *target;       /* 0xF4: the node looked at (MCamera command 0x69 picks it from a table) */
    s32 unkF8;
    f32 unkFC;              /* 0xFC: [0] */
    s32 unk100;
    CamVec3 lookDir;        /* 0x104: [(0, 0, 1)] look direction while target is NULL */
    char unk110[0x20];      /* 0x110: a sub-object */
    s32 rotEnable;          /* 0x130: [0] pitch / yaw are applied only while set */
    f32 pitch;              /* 0x134: [0] CamDirector manual mode adds stick y * 2; setActiveCam clears 0x134..0x13C */
    f32 yaw;                /* 0x138: [0] CamDirector manual mode adds stick x * 2 */
    f32 unk13C;             /* 0x13C: [0] cleared with pitch / yaw (roll? unproven) */
    CamVec3 up;             /* 0x140: [(0, 1, 0)] the lookAt up vector */
    f32 fov;                /* 0x14C: [85] field of view in degrees; CamDirector manual mode keeps it in 45..120,
                             * RiderCam (bootup 4948) sets it from distance / angle tables */
} MCamera;

/* the 3D window a camera renders into: only the active-camera slot is known */
typedef struct CamWindow {
    char pad00[0x70];
    MCamera *cam;           /* 0x70: the camera drawn (CamDirector_setActiveCam) */
} CamWindow;

/* RiderCam (bootup, 0x15C, vtbl bootup Data 0x7120): the camera bootup cam_create builds in mode 1 (MCamera_ctor(lookat,
 * 0, 500, -1500)). Slot 3 RiderCam_update_00004948 = per-frame tick (fov by distance / angle tables, mode 2 drift with the
 * rider's velocity), slot 12 RiderCam_command_00004914 = command (mode = arg, eye = the course "cam" node position + 100 up). */
typedef struct RiderCam {
    MCamera cam;            /* 0x000 */
    f32 baseFov;            /* 0x150: the ctor's fov (85); restored in mode 2 and by default */
    struct SnowBoarder *rider; /* 0x154: the rider; mode 2 moves the eye by rider->dnode.vel * k */
    s32 mode;               /* 0x158: the course "cam" node kind (CamDirector passes 3..0x26) */
} RiderCam;

struct RiderInput;

/* CamDirector (bootup, 0x50, vtbl Data 0x6FD8, base Node "<Node>"): picks the race camera. Auto mode: every 60
 * frames it finds the course "cam" node (kind at +0x3C) nearest the rider; kinds 3..0x12 go to cams[0] (the game.uso
 * path camera), others to cams[1] (RiderCam), through the camera's command slot. Pad button 0x40 toggles manual mode
 * (fov / pitch / yaw by pad). CamDirector_ctor(this, pad, rider, window, cam0, cam1). */
typedef struct CamDirector {
    char node[0x2C];        /* 0x00: Node; +0x18 bit 8 = enabled (onMessage 0x3ED clears it) */
    CamWindow *window;      /* 0x2C */
    MCamera *cams[2];       /* 0x30 */
    struct RiderInput *pad; /* 0x38: stick x / y floats at 0 / 4, raw s8 stick 8 / 9, held 0x10, pressed 0x18 */
    s32 activeCam;          /* 0x3C: index into cams (setActiveCam) */
    struct SnowBoarder *rider; /* 0x40: the nearest-node search is keyed on rider->dnode.vqs.trans */
    void *lastCamNode;      /* 0x44: the course "cam" node last acted on */
    s32 pollTimer;          /* 0x48: [10000] search when it reaches 60, then 0 */
    s32 manual;             /* 0x4C: toggled by pad pressed 0x40 */
} CamDirector;

/* FreeCam (game_libs, 0x50; renames.tsv CamMan_*: the matching C++ calls its update FreeCam::update): the debug fly
 * camera. Its parent (Node +0x14) is the MCamera it moves: eye = pos + (0, 0, dist) turned by pitch / yaw. */
typedef struct FreeCam {
    char node[0x14];
    MCamera *cam;           /* 0x14: Node parent */
    char pad18[0x2C - 0x18];
    f32 dist;               /* 0x2C: [1000] boom length */
    f32 pitch;              /* 0x30: [-90] clamped to +-89; -= stick y * 4 while button 0x1000 */
    f32 yaw;                /* 0x34: [180] += stick x * 4 while button 0x1000 */
    f32 unk38;
    s32 accel;              /* 0x3C: ramps to 100 while moving; speed = accel / 10 */
    CamVec3 pos;            /* 0x40: the look-at point (also written to the target's local[3]) */
    void *ctl;              /* 0x4C: its +0xB4 is the pad (buttons 0x40 / 0x80 = +-z, 0x400 = stick xy) */
} FreeCam;

/* CamFollow (game_libs, 0x3C, vtbl gl Data 0x1E878): a debug orbit controller on its parent camera (Node +0x14):
 * pans camera and target by owner's pan vector (button 0x200), and orbits the eye about the target by +-2 degrees
 * (buttons 1 / 2 about Y, 4 / 8 about the view's right axis). It does not follow the rider. */
typedef struct CamFollow {
    char node[0x14];
    MCamera *cam;           /* 0x14: Node parent */
    char pad18[0x2C - 0x18];
    CamVec3 unk2C;          /* 0x2C: cleared by the ctor */
    void *owner;            /* 0x38: +0xB4 pad, +0xD4..0xDC pan vector, +0x110 window */
} CamFollow;

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

/* SnowChaseActorCam (0x124, node name "SnowChaseActorCam" at game.uso Data 0x340; SnowChaseActorCam_ctor(this, target, source, parmsFile)):
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

/* SnowChaseActorLook (0x64, node name at Data 0x360; SnowChaseActorLook_ctor(this, anchor, cam, parmsFile)): moves its parent body (the look-at point). */
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

/* SnowChaseActor (0x138, node name at Data 0x3A0; SnowChaseActor_ctor(this, target, lookBody, parmsFile)) */
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

/* ---- the course camera (game.uso cam_ctor_00000858, 0x27C, vtbl Data 0x184; agent-g run 480) ----
 * What bootup cam_create builds in modes 0 / 2 from "rom/cam_info.bin" (cam_readParms_00001644). It is an
 * MCamera that plays scripted camera PATHS: a view (0..14, chosen by cam_command / cam_pickResultView) names a
 * CamList, a list of path indices played in order; each CamPath is a run of CamKeys interpolated by a cubic
 * B-spline (cam_updateView_00000B3C: keys[k-1..k+2] weighted by the basis at frame / key->frames). A path can
 * instead hand the eye and / or the look-at point to the chase components (eyeMode / lookMode == 1), which is
 * the normal race view; the player can then cycle chaseMode 0..2 (pad pressed 0x40) or hold 0x80 for mode 3. */
typedef struct CamKey {     /* 0x38, read in this order by cam_readParms */
    CamVec3 look;           /* 0x00: look-at point */
    CamVec3 eye;            /* 0x0C: eye position */
    CamVec3 up;             /* 0x18: up vector */
    f32 fov;                /* 0x24 */
    s32 frames;             /* 0x28: frames from this key to the next */
    CamVec3 rot;            /* 0x2C: MCamera pitch / yaw / unk13C */
} CamKey;

typedef struct CamPath {    /* 0x7C, camPath_read_00000724 */
    /* channel modes: lookMode / eyeMode 1 = the body is moved by its chase component (camPath_init's default),
     * 2 = keys relative (eye: + the chase aim point; look: + the eye; cam_applyRelative_0000052C), 3 = absolute */
    s32 lookMode;           /* 0x00 */
    s32 lookSpline;         /* 0x04: 0 = key.look of the current key; else spline */
    s32 unk08;              /* 0x08: read with lookMode / lookSpline (a 4-word record), not used */
    s32 unk0C;
    s32 eyeMode;            /* 0x10: as lookMode (1 = MCamera world[3] is kept) */
    s32 eyeSpline;          /* 0x14 */
    s32 unk18;              /* 0x18: read with eyeMode / eyeSpline, not used */
    s32 unk1C;
    s32 upSpline;           /* 0x20: 0 = key.up, else spline */
    s32 unk24;
    s32 fovMode;            /* 0x28: 1 = key.fov, 2 = spline */
    s32 unk2C;
    s32 rotMode;            /* 0x30: 1 = key.rot, 2 = spline; also MCamera.rotEnable = (rotMode != 0) */
    s32 numKeys;            /* 0x34: keys 1..numKeys are played (at most 15) */
    CamKey *keys[17];       /* 0x38: keys[1..numKeys] read from the file; keys[0] = keys[1] and
                             * keys[numKeys + 1] = keys[numKeys] pad the ends, so the spline at key k reads
                             * keys[k - 1 .. k + 2]. A path with any spline channel (camPath_hasSpline) ends
                             * at key numKeys, one key early */
} CamPath;

typedef struct CamList {
    s32 unk00;
    s32 paths[8];           /* 0x04: path indices, played in order */
    s32 count;              /* 0x24 */
} CamList;

typedef struct CourseCam {
    MCamera cam;            /* 0x000: the eye; cam.target (0xF4) = the "snowlookat" node (its +0xB4 = its chase
                               component, a SnowChaseActorLook) */
    struct SnowBoarder *rider; /* 0x150: the followed rider (raceResult 0xA54 picks the result view) */
    SnowChaseActorCam *eyeChase; /* 0x154: the eye's chase component ("rom/cam_ca.bin") */
    CamList *lists[30];     /* 0x158: per view */
    s32 numLists;           /* 0x1D0 */
    CamPath *paths[30];     /* 0x1D4 */
    s32 numPaths;           /* 0x24C */
    s32 view;               /* 0x250: current view (cam_setView_00000AEC; ignored when lists[0] is empty) */
    s32 listPos;            /* 0x254: position in lists[view]->paths */
    s32 path;               /* 0x258: = lists[view]->paths[listPos] */
    s32 key;                /* 0x25C: current key (starts at 1) */
    s32 frame;              /* 0x260: frame within the key */
    s32 lastResult;         /* 0x264: rider->raceResult seen last; a change re-runs cam_pickResultView */
    s32 freeRot;            /* 0x268: set = rot is not driven by the path (rotEnable 1, rot 0) */
    s32 chaseMode;          /* 0x26C: chase-path views only. eye / look component modes: 0 = 1 / 1, 1 = 2 / 1
                               (eye height ty2), 2 = 3 / 2 (fixed eye, look ahead), 3 = 3 / 3 (look back, while
                               pad held 0x80); pad pressed 0x40 cycles 0..2. 2 / 3 set rider +0xA58 bit 0x2000 */
    s32 savedChaseMode;     /* 0x270: chaseMode to restore when the look-back button is released */
    struct RiderInput *pad; /* 0x274: input pad (cam_create p4): held 0x80 = look back, pressed 0x40 = next chaseMode */
    s32 unk278;
} CourseCam;                /* 0x27C */

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
CAM_ASSERT(MCamera, view, 0xB4);
CAM_ASSERT(MCamera, target, 0xF4);
CAM_ASSERT(MCamera, lookDir, 0x104);
CAM_ASSERT(MCamera, rotEnable, 0x130);
CAM_ASSERT(MCamera, up, 0x140);
CAM_ASSERT(MCamera, fov, 0x14C);
CAM_ASSERT(RiderCam, rider, 0x154);
CAM_ASSERT(CamDirector, window, 0x2C);
CAM_ASSERT(CamDirector, pad, 0x38);
CAM_ASSERT(CamDirector, manual, 0x4C);
CAM_ASSERT(FreeCam, pos, 0x40);
CAM_ASSERT(CamFollow, owner, 0x38);
typedef char cam_assert_mnode_size[sizeof(CamMNode) == 0xB4 ? 1 : -1];
typedef char cam_assert_mcamera_size[sizeof(MCamera) == 0x150 ? 1 : -1];
typedef char cam_assert_ridercam_size[sizeof(RiderCam) == 0x15C ? 1 : -1];
typedef char cam_assert_director_size[sizeof(CamDirector) == 0x50 ? 1 : -1];
typedef char cam_assert_freecam_size[sizeof(FreeCam) == 0x50 ? 1 : -1];
typedef char cam_assert_follow_size[sizeof(CamFollow) == 0x3C ? 1 : -1];
typedef char cam_assert_cam_size[sizeof(SnowChaseActorCam) == 0x124 ? 1 : -1];
typedef char cam_assert_look_size[sizeof(SnowChaseActorLook) == 0x64 ? 1 : -1];
typedef char cam_assert_actor_size[sizeof(SnowChaseActor) == 0x138 ? 1 : -1];
CAM_ASSERT(CamKey, frames, 0x28);
CAM_ASSERT(CamPath, numKeys, 0x34);
CAM_ASSERT(CamList, count, 0x24);
CAM_ASSERT(CourseCam, rider, 0x150);
CAM_ASSERT(CourseCam, lists, 0x158);
CAM_ASSERT(CourseCam, numLists, 0x1D0);
CAM_ASSERT(CourseCam, paths, 0x1D4);
CAM_ASSERT(CourseCam, numPaths, 0x24C);
CAM_ASSERT(CourseCam, lastResult, 0x264);
CAM_ASSERT(CourseCam, pad, 0x274);
typedef char cam_assert_camkey_size[sizeof(CamKey) == 0x38 ? 1 : -1];
typedef char cam_assert_campath_size[sizeof(CamPath) == 0x7C ? 1 : -1];
typedef char cam_assert_coursecam_size[sizeof(CourseCam) == 0x27C ? 1 : -1];
typedef char cam_assert_camparms_size[sizeof(SnowChaseActorCamParms) == 0xD0 ? 1 : -1];
#endif

#endif /* GAME_CAMERA_H */
