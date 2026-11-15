#ifndef GDL_MNODE_H
#define GDL_MNODE_H

#include "gdl/node.h"
#include "gdl/anim.h"

/* libgdl Node subclasses (game_libs post0b). Field names are the HostIO parm tags their
 * *_hostioParms register (Data strings after the class name: MNode 0x1ECD0.., SNode
 * 0x1ECFC.., QNode 0x1F494..).
 *
 * MNode (0xB4, "<MNode>", MNode_ctor 0x3E0F0 / MNode_def 0x37AF0): a matrix node. "Mtx" is the local
 * matrix, "CMtx" the combined (world) matrix that MNode_updateChildren (0x38DC0) builds from parent and local
 * (row 3 = world position; docs/1080_CAMERA.md calls it world, include/game/camera.h CamMNode
 * is the same layout). Vtable slot 9 (MNode_setAnimPose) builds Mtx from an
 * Animator's dst pose (Animator +0xD0), slot 10 returns &cmtx. MNode_initMatrices sets
 * Node.flags 0x200 and doFlags 0x10. */
typedef struct MNode {
    /* 0x00 */ Node node;
    /* 0x2C */ s32 mflags;         /* "Flags" */
    /* 0x30 */ f32 mtx[4][4];      /* "Mtx": local */
    /* 0x70 */ f32 cmtx[4][4];     /* "CMtx": combined (world); cmtx[3] = world position */
    /* 0xB0 */ s32 mtxMode;        /* "MtxMode" */
} MNode;

/* QNode (MNode + two VQS poses, "QNode"): "VQS" (+0xB4) is the local translation / scale /
 * quaternion in the AnimPose layout (QNode_setAnimPose copies an Animator's dst pose into
 * it), "CVQS" (+0xDC) the combined one. QNode_translate adds to vqs.trans. DNode and the
 * physics nodes derive from it. */
typedef struct QNode {
    /* 0x000 */ MNode mnode;
    /* 0x0B4 */ AnimPose vqs;      /* "VQS" */
    /* 0x0DC */ AnimPose cvqs;     /* "CVQS" */
} QNode;

/* SNode (0x88, "<SNode>", SNode_ctor 0x39A04): a node with a position, a uniform scale, Euler
 * angles in degrees, a height offset and a cull radius. SNode_handleMsg 19 rebuilds the
 * 0x30-byte "Mtx" object at +0x2C from pos, scale and rotZ / rotX / rotY; SNode_display
 * culls on pos (with the radius) and raises the camera vector by height. SNodeItemType chains
 * the item SNodes through next. */
typedef struct SNode {
    /* 0x00 */ Node node;
    /* 0x2C */ char mtx[0x30];     /* "Mtx" (initialised by a call on its address in the ctor) */
    /* 0x5C */ f32 pos[3];         /* message 7 sets it; SNode_loadData / saveData */
    /* 0x68 */ s32 unk68;
    /* 0x6C */ f32 height;         /* "Height" (-500..500) */
    /* 0x70 */ f32 scale;          /* "Scale" (0.01..100; 1.0 at ctor) */
    /* 0x74 */ f32 rotX;           /* "RotX" (-180..180) */
    /* 0x78 */ f32 rotY;           /* "RotY" */
    /* 0x7C */ f32 rotZ;           /* "RotZ" */
    /* 0x80 */ f32 radius;         /* "Radius" */
    /* 0x84 */ struct SNode *next; /* SNodeItemType item chain */
} SNode;

typedef char MNode_size_check[sizeof(MNode) == 0xB4 ? 1 : -1];
typedef char MNode_cmtx_check[(int)&((MNode *)0)->cmtx == 0x70 ? 1 : -1];
typedef char QNode_vqs_check[(int)&((QNode *)0)->vqs == 0xB4 ? 1 : -1];
typedef char QNode_cvqs_check[(int)&((QNode *)0)->cvqs == 0xDC ? 1 : -1];
typedef char SNode_size_check[sizeof(SNode) == 0x88 ? 1 : -1];
typedef char SNode_radius_check[(int)&((SNode *)0)->radius == 0x80 ? 1 : -1];

#define MNODE(p) ((MNode *)(p))
#define QNODE(p) ((QNode *)(p))
#define SNODE(p) ((SNode *)(p))

#endif /* GDL_MNODE_H */
