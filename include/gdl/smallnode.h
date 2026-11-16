#ifndef GDL_SMALLNODE_H
#define GDL_SMALLNODE_H

#include "gdl/node.h"

/* Two small libgdl Node subclasses of game_libs post0b (agent-b run 583). Plain C types only. */

/* Spinner (Node "<Node>" base): Spinner_update turns its parent's matrix (parent +0x30) by the
 * Euler angles (0, speed, 0) every frame (math_mtxRotateEuler); Spinner_hostioParms publishes
 * speed as hostio parm "Speed" (range -6..6). */
typedef struct Spinner {
    /* 0x00 */ Node node;
    /* 0x2C */ f32 speed;
} Spinner;

/* SeekNode (0x34, vtbl SeekNode_vtbl Data 0x1E938, Node "<Node>" base): SeekNode_ctor(this, target,
 * rate) stores both; SeekNode_update translates its parent by (target's world position - the
 * parent's) * rate every frame (the NCC donor game_libs_ncc_37938.cpp). */
typedef struct SeekNode {
    /* 0x00 */ Node node;
    /* 0x2C */ s32 target;          /* the node sought */
    /* 0x30 */ f32 rate;
} SeekNode;

#ifndef __cplusplus /* edg (NCC donors): &((T *)0)->f is not an integral constant expression */
typedef char Spinner_speed_check[(int)&((Spinner *)0)->speed == 0x2C ? 1 : -1];
typedef char SeekNode_size_check[sizeof(SeekNode) == 0x34 ? 1 : -1];
#endif

#endif /* GDL_SMALLNODE_H */
