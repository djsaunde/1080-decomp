#ifndef GDL_BALLROT_H
#define GDL_BALLROT_H

#include "gdl/node.h"

/*
 * BallRotator: the libgdl virtual trackball (game_libs post0b 0x35FD4-0x36694; agent-b
 * run 515). A Node whose update (BallRotator_update_0003604C) runs
 * BallRotator_func_00036224(this, &parent->mtx) on its parent MNode (Node.parent +0x30):
 * the cursor's current and previous screen points (the input object at +0x1CC, +0xF8 /
 * +0xBC, minus 160 / 120 and y flipped) are projected onto a sphere of `radius` around
 * `center` (the 0x34458 helper, called once per point), turned into world space
 * through the camera QNode's orientation (+0x1D0, its VQS quat at +0xB4), and while the
 * input's button mask (input +0xB4 -> +0x10) has a bit of buttonMask set the drag
 * rotation is composed into rot; then rot goes into the parent matrix. HostIO parm:
 * "Radius" (+0x44, 10..400). BallRotator_reset_00036074 sets identity quaternions,
 * radius 200 and clears the rest.
 *
 * Field names below the radius are by role in those two bodies (no strings); the
 * quaternions are {x, y, z, w}.
 */
typedef struct BallRotator {
    /* 0x000 */ Node node;
    /* 0x02C */ f32 center[3];      /* sphere centre, passed by value with radius */
    /* 0x038 */ f32 offset[3];      /* applied to the target matrix last */
    /* 0x044 */ f32 radius;         /* "Radius" (200 at reset) */
    /* 0x048 */ f32 rot[4];         /* current orientation (identity at reset) */
    /* 0x058 */ f32 prevRot[4];     /* rot before this drag step */
    /* 0x068 */ f32 dragRot[4];     /* rotation between the two sphere points */
    /* 0x078 */ f32 baseRot[4];
    /* 0x088 */ f32 unk88[3];       /* 0 at reset; copied to unk94, then lastHit */
    /* 0x094 */ f32 unk94[3];
    /* 0x0A0 */ f32 hitCur[3];      /* sphere point under the current cursor (camera space,
                                     * then world) */
    /* 0x0AC */ f32 hitPrev[3];     /* the same for the previous cursor */
    /* 0x0B8 */ char padB8[0xD0 - 0xB8];
    /* 0x0D0 */ f32 lastHit[3];     /* hitPrev of the last drag step */
    /* 0x0DC */ f32 mtxA[4][4];     /* math_mtx4Identity at reset */
    /* 0x11C */ f32 mtxB[4][4];     /* math_mtx4Identity at reset */
    /* 0x15C */ s32 unk15C;         /* 0 at reset */
    /* 0x160 */ s32 unk160;         /* 0 at reset */
    /* 0x164 */ char pad164[0x184 - 0x164];
    /* 0x184 */ s32 unk184;         /* 0 at reset */
    /* 0x188 */ char pad188[0x1CC - 0x188];
    /* 0x1CC */ char *input;        /* cursor source: +0xF8 current / +0xBC previous screen
                                     * point, +0xB4 -> +0x10 button bits, +0x108 enable */
    /* 0x1D0 */ char *camera;       /* QNode whose VQS quaternion (+0xB4) maps camera space
                                     * to world */
    /* 0x1D4 */ s32 buttonMask;     /* drag while (input buttons & buttonMask) */
    /* 0x1D8 */ s32 invert;         /* nonzero: negate both cursor points */
} BallRotator;

typedef char BallRotator_radius_check[(int)&((BallRotator *)0)->radius == 0x44 ? 1 : -1];
typedef char BallRotator_mtxB_check[(int)&((BallRotator *)0)->mtxB == 0x11C ? 1 : -1];
typedef char BallRotator_invert_check[(int)&((BallRotator *)0)->invert == 0x1D8 ? 1 : -1];

#define BALLROT(p) ((BallRotator *)(p))

#endif /* GDL_BALLROT_H */
