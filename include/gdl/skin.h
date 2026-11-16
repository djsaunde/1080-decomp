#ifndef GDL_SKIN_H
#define GDL_SKIN_H

#include "node.h" /* same directory: the NCC donors include this header by relative path */

/* libgdl vertex skinning (skin.cpp; game_libs 0x68C14-0x69C58, agent-b run 486).
 * Port-author description: docs/1080_GRAPHICS.md section 12.
 *
 * A SkinMgr deforms one shape (a MorphPoly over a "SkinShape" BasePolyShape) between a main
 * bone (an MNode) and any number of extra bones (SkinAnims). Every vertex has a main-bone
 * weight in percent (SkinMgr.weights, 100 at init); each SkinAnim lists the vertices it moves
 * and their weights (SkinWeight), and SkinAnim_bind subtracts those from the main weights.
 * SkinMgr_update (once per frame): out[v] = R(main) * bind[v] * weights[v] / 100, then every
 * SkinAnim adds R(bone) * boneBind[i] + (bone pos - main pos), times weight / 100 (or stores
 * it when the weight is 100), then MorphPoly_setVerts(shape, out). R = the 3x3 part of the
 * bone's world matrix (MNode +0x70; translation +0xA0). Vertices are s16[3]. */

/* one vertex of a SkinAnim (4 bytes) */
typedef struct SkinWeight {
    /* 0x0 */ s16 vert;            /* vertex index in the shape */
    /* 0x2 */ s16 weight;          /* percent; <= 0 skipped, 100 = owned outright */
} SkinWeight;

/* one entry of SkinMgr.boneTable (8 bytes; skin_cmdSkinAnim picks one by index) */
typedef struct SkinBone {
    /* 0x0 */ s16 count;
    /* 0x2 */ s16 pad2;
    /* 0x4 */ struct SkinWeight *weights; /* 0 = bone not skinned */
} SkinBone;

/* SkinMgr (0x98, "skinmgr", vtable Data 0x2C490; ctor 0x695F4). Up to 4 (MAX_SKIN_MGRS),
 * registered by index by the SkinMgr node command. */
typedef struct SkinMgr {
    /* 0x00 */ Node node;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ s16 *weights;        /* main-bone weight per vertex (percent) */
    /* 0x34 */ struct MorphPoly *shape; /* its +0x40 nVerts, +0x60 verts */
    /* 0x38 */ struct MNode *bone;  /* main bone; 0 = not initialised */
    /* 0x3C */ s16 (*bindVerts)[3]; /* copy of the shape's verts at init */
    /* 0x40 */ s16 (*outVerts)[3];  /* skinned verts handed to MorphPoly_setVerts */
    /* 0x44 */ s32 unk44;
    /* 0x48 */ NodeLink *anims;    /* list of SkinAnim * (item) (SkinMgr_addAnim) */
    /* 0x4C */ s32 lastFrame;      /* frame counter of the last update */
    /* 0x50 */ f32 invBone[4][4];  /* inverse of the main bone matrix at init */
    /* 0x90 */ SkinBone *boneTable; /* from the SkinMgr command */
    /* 0x94 */ s32 jitter;         /* one-shot random offset amplitude (cleared by update) */
} SkinMgr;

/* SkinAnim (0xA8, "skinanimator"; SkinAnim_ctor 0x68FBC (this, count, weights, bone)). Its
 * first 0x18 bytes are a vtable-less named-list base ({-1, 0, 0x8000, name, list, 0}). */
typedef struct SkinAnim {
    /* 0x00 */ s32 unk00;          /* -1 */
    /* 0x04 */ s32 unk04;
    /* 0x08 */ s32 flags;          /* 0x8000 */
    /* 0x0C */ s32 name;           /* "skinanimator" */
    /* 0x10 */ s32 list;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ struct MNode *bone;
    /* 0x1C */ SkinWeight *weights;
    /* 0x20 */ s32 count;
    /* 0x24 */ s16 (*bindVerts)[3]; /* the vertices in bone space (SkinAnim_bind) */
    /* 0x28 */ f32 invBone[4][4];  /* inverse of the bone matrix at bind time */
    /* 0x68 */ f32 boneAtBind[4][4];
} SkinAnim;

typedef char SkinMgr_size_check[sizeof(SkinMgr) == 0x98 ? 1 : -1];
#ifndef __cplusplus /* edg (NCC donors): &((T *)0)->f is not an integral constant expression */
typedef char SkinMgr_boneTable_check[(int)&((SkinMgr *)0)->boneTable == 0x90 ? 1 : -1];
#endif
typedef char SkinAnim_size_check[sizeof(SkinAnim) == 0xA8 ? 1 : -1];

#endif
