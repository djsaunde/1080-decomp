#ifndef GDL_POLYSHAPE_H
#define GDL_POLYSHAPE_H

/* libgdl BasePolyShape (0xB0 bytes; game_libs 0x4F9E4-0x54144 in
 * src/game_libs/game_libs_post0b.c). Only the fields the matched bodies
 * pin down are named:
 *   - BasePolyShape_reset (0x52918) sets flags bit 0x800, zeroes 0x3C-0x80,
 *     0xA0/0xA8/0xAC, sets 0x98 / 0xA4 to -1 and the f32 at 0x9C to 1.0f;
 *   - BasePolyShape_triCentroid / genFaceNormals walk tris (8-byte PolyTri,
 *     nTris at 0x44) and fetch s16 vertex rows from verts (nVerts at 0x40);
 *   - genFaceNormals writes one f32[3] face normal per triangle at 0x54,
 *     genNormals writes one f32[3] vertex normal per vertex at 0x58;
 *   - BasePolyShape_getVtxNorm maps a vertex through the optional u16 remap
 *     table at 0x70 (6-byte rows, first u16 = normal index) into vtxNormals;
 *   - 0x5C is an RGB888 palette (the 0x5323C lookup-copy). */

typedef struct PolyTri {
    /* 0x0 */ u16 flags;
    /* 0x2 */ u16 v[3];            /* vertex indices */
} PolyTri;

typedef struct BasePolyShape {
    /* 0x00 */ s32 unk00[2];
    /* 0x08 */ s32 flags;
    /* 0x0C */ s32 unk0C[9];
    /* 0x30 */ s32 unk30;          /* flag word: BasePolyShape_parseCmd 13 / 14 / 17 set bits 2 / 1 / 4 */
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s32 shapeFlags;     /* parse cmd 8 sets 0x8 (alphas present), cmd 11 0x2000, cmd 21
                                    * 0x4000, parseCmd 15 / 22 / 23 bits 2 / 1 / 0x20000, cmd 24 ORs
                                    * a word in;
                                    * 0x8000 = vertex normals come from PolyShape_getNormal8 */
    /* 0x3C */ s32 unk3C;
    /* 0x40 */ s32 nVerts;
    /* 0x44 */ u32 nTris;
    /* 0x48 */ s32 unk48;
    /* 0x4C */ s16 *triList;     /* parse cmd 26 (with nTris): optional tri index list; when set,
                                  * the CollShape ctorPoly / query paths use tris[triList[i]] */
    /* 0x50 */ u32 *vtxColours;  /* parse cmd 9: one RGBA word per vertex (PolyShape_shade copies RGB) */
    /* 0x54 */ f32 *faceNormals;   /* f32[3] per triangle */
    /* 0x58 */ f32 *vtxNormals;    /* f32[3] per vertex */
    /* 0x5C */ u8 *palette;        /* RGB888 */
    /* 0x60 */ s16 (*verts)[3];    /* vertex rows (x, y, z) */
    /* 0x64 */ s32 unk64;
    /* 0x68 */ PolyTri *tris;
    /* 0x6C */ s32 unk6C;
    /* 0x70 */ u16 *remap;         /* optional, 3 u16 per vertex */
    /* 0x74 */ s32 unk74;
    /* 0x78 */ s32 unk78;
    /* 0x7C */ u8 *alphas;       /* parse cmd 8 (sets shapeFlags 0x8): one alpha byte per vertex */
    /* 0x80 */ s32 unk80;
    /* 0x84 */ struct BaseMat **mats; /* parse cmd 29: alloc(matCap * 4); PolyShape_clipToWindow_00049308 culls
                                       * each by its minCut / maxCut, getMatCodeNum / Char read its code */
    /* 0x88 */ s32 unk88;
    /* 0x8C */ s32 matCap;         /* parse cmd 29 */
    /* 0x90 */ s32 nMats;          /* 0 at cmd 29 */
    /* 0x94 */ s32 unk94;
    /* 0x98 */ s32 unk98;          /* -1 after reset */
    /* 0x9C */ f32 unk9C;          /* 1.0f after reset */
    /* 0xA0 */ struct CollGrid *collGrid; /* parse cmd 35: the XZ cell grid (gdl/coll.h) that
                                  * CollShape_query looks the point up in; 0 = linear scan */
    /* 0xA4 */ s32 unkA4;          /* -1 after reset */
    /* 0xA8 */ s32 skinInvBone;     /* f32 (*)[4]: SkinMgr_init stores the inverse main-bone
                                     * matrix (SkinMgr.invBone); 0 = not skinned. The display
                                     * path passes skinBoneMtx and it to a call when set */
    /* 0xAC */ s32 skinBoneMtx;     /* f32 (*)[4]: the main bone's world matrix (MNode +0x70) */
} BasePolyShape;

/* one row of BasePolyShape.verts (6 bytes) and one face / vertex normal (BasePolyShape_genNormals) */
typedef struct PolyVert {
    /* 0x0 */ s16 x, y, z;
} PolyVert;

typedef struct PolyNormal {
    /* 0x0 */ f32 x, y, z;
} PolyNormal;

#define POLYVERT(p) ((PolyVert *)(p))
#define POLYNORMAL(p) ((PolyNormal *)(p))
#define POLYTRI(p) ((PolyTri *)(p))
#define BASEPOLY(p) ((BasePolyShape *)(p))

typedef char PolyVert_size_check[sizeof(PolyVert) == 6 ? 1 : -1];
typedef char PolyTri_size_check[sizeof(PolyTri) == 8 ? 1 : -1];
typedef char BasePolyShape_size_check[sizeof(BasePolyShape) == 0xB0 ? 1 : -1];
#ifndef __cplusplus /* edg (NCC donors): &((T *)0)->f is not an integral constant expression */
typedef char BasePolyShape_verts_check[(int)&((BasePolyShape *)0)->verts == 0x60 ? 1 : -1];
typedef char BasePolyShape_remap_check[(int)&((BasePolyShape *)0)->remap == 0x70 ? 1 : -1];
typedef char BasePolyShape_9C_check[(int)&((BasePolyShape *)0)->unk9C == 0x9C ? 1 : -1];
typedef char BasePolyShape_shapeFlags_check[(int)&((BasePolyShape *)0)->shapeFlags == 0x38 ? 1 : -1];
typedef char BasePolyShape_alphas_check[(int)&((BasePolyShape *)0)->alphas == 0x7C ? 1 : -1];
#endif

/* PolyShape (0xC0, PolyShape_ctor 0x48A74) = BasePolyShape + 0x10 bytes;
 * PolyShape_display builds the F3DEX list (G_VTX batches of up to 32 points,
 * G_TRI1 / G_LINE3D, material state through gfx_emitStateAndPrimColour).
 *
 * MorphPoly (0xDC, MorphPoly_ctor 0x4A670) is a PolyShape whose vertices are
 * rewritten every frame: MorphPoly_setVerts stores the new vertex source at
 * +0xC0 and bumps the update counter at +0xD8 (genNormalsFast on every 5th
 * update when flag 0x800 is set); MorphPoly_init copies the s16 update list
 * between unkD4 and the static DL's Vtx count (vtxEnd) to its own buffer at
 * +0xD0, which MorphPoly_applyUpdateList walks (-1 terminated groups). */
typedef struct PolyShape {
    /* 0x00 */ BasePolyShape base;
    /* 0xB0 */ s32 unkB0[4];
} PolyShape;

typedef struct MorphPoly {
    /* 0x00 */ PolyShape poly;
    /* 0xC0 */ void *verts;            /* new vertex source, MorphPoly_setVerts */
    /* 0xC4 */ s32 unkC4;
    /* 0xC8 */ s32 unkC8;
    /* 0xCC */ s32 unkCC;
    /* 0xD0 */ s16 *updateList;        /* -1 terminated index groups */
    /* 0xD4 */ s16 vtxStart;
    /* 0xD6 */ s16 vtxEnd;             /* Vtx count of the static DL (gfx +0x214) at init */
    /* 0xD8 */ s32 updateCount;
} MorphPoly;

typedef char PolyShape_size_check[sizeof(PolyShape) == 0xC0 ? 1 : -1];
typedef char MorphPoly_size_check[sizeof(MorphPoly) == 0xDC ? 1 : -1];
#ifndef __cplusplus
typedef char MorphPoly_updateList_check[(int)&((MorphPoly *)0)->updateList == 0xD0 ? 1 : -1];
typedef char MorphPoly_updateCount_check[(int)&((MorphPoly *)0)->updateCount == 0xD8 ? 1 : -1];
#endif

#endif /* GDL_POLYSHAPE_H */
