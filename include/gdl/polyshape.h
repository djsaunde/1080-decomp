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
    /* 0x0C */ s32 unk0C[12];
    /* 0x3C */ s32 unk3C;
    /* 0x40 */ s32 nVerts;
    /* 0x44 */ u32 nTris;
    /* 0x48 */ s32 unk48;
    /* 0x4C */ s32 unk4C;
    /* 0x50 */ s32 unk50;
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
    /* 0x7C */ s32 unk7C;
    /* 0x80 */ s32 unk80;
    /* 0x84 */ s32 unk84[5];
    /* 0x98 */ s32 unk98;          /* -1 after reset */
    /* 0x9C */ f32 unk9C;          /* 1.0f after reset */
    /* 0xA0 */ s32 unkA0;
    /* 0xA4 */ s32 unkA4;          /* -1 after reset */
    /* 0xA8 */ s32 unkA8;
    /* 0xAC */ s32 unkAC;
} BasePolyShape;

typedef char PolyTri_size_check[sizeof(PolyTri) == 8 ? 1 : -1];
typedef char BasePolyShape_size_check[sizeof(BasePolyShape) == 0xB0 ? 1 : -1];
typedef char BasePolyShape_verts_check[(int)&((BasePolyShape *)0)->verts == 0x60 ? 1 : -1];
typedef char BasePolyShape_remap_check[(int)&((BasePolyShape *)0)->remap == 0x70 ? 1 : -1];
typedef char BasePolyShape_9C_check[(int)&((BasePolyShape *)0)->unk9C == 0x9C ? 1 : -1];

#endif /* GDL_POLYSHAPE_H */
