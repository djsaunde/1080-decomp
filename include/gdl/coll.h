#ifndef GDL_COLL_H
#define GDL_COLL_H

#include "node.h" /* same directory: the NCC donors include this header by relative path */

/* libgdl dynamic collision objects (game_libs post0b: CollObjMgr_ctor 0x3C814, CollObjMgr_update 0x3C86C,
 * coll_boxCtor_0003CAA0, CollObj_init 0x3CB00, CollObj_initCylinder 0x3CB6C,
 * CollObj_sphereHitTest 0x3CBB4).
 *
 * A CollObj is a small shape record owned by a Node. CollObjMgr_update tests every
 * dynamic object against every other dynamic one and then against every static one
 * through vtable slot 1 (test(self, other, &contact)); when the test hits, each side whose
 * hitMask shares a bit with the other's category gets vtable slot 2 (onContact(self,
 * &contact)) with contact.other set and, for the second side, the normal negated.
 * CollObj_sendBump sends message 0x14 to the owner through the owner's Node vtable.
 * The vtable pointer sits at +0x30 (not in a Node header) with Cfront {s16 delta, pad, fn}
 * entries: slot 1 = delta +0x8 / fn +0xC, slot 2 = delta +0x10 / fn +0x14. */
typedef struct CollObj {
    /* 0x00 */ Node *owner;        /* CollObj_init arg1 */
    /* 0x04 */ struct CollObj *next; /* CollObjMgr list link */
    /* 0x08 */ s32 kind;           /* CollObj_init arg2; 1 = cylinder (CollObj_initCylinder) */
    /* 0x0C */ s16 id;             /* CollObj_init arg3 */
    /* 0x0E */ s16 unk0E;
    /* 0x10 */ s32 flags;          /* bit 0: CollObj_sphereHitTest reports an unresolved overlap as a zero contact */
    /* 0x14 */ s16 hitMask;        /* 0xFFFF at init: categories this object reacts to */
    /* 0x16 */ s16 category;       /* 0xFFFF at init */
    /* 0x18 */ f32 pos[3];
    /* 0x24 */ f32 vel[3];         /* CollObj_sphereHitTest sweeps pos by the relative velocity */
    /* 0x30 */ char *vptr;         /* Cfront vtable pointer */
} CollObj;

/* the cylinder object of CollObj_initCylinder / CollObj_sphereHitTest (Y-up, XZ circle) */
typedef struct CollCylinder {
    /* 0x00 */ CollObj obj;
    /* 0x34 */ f32 radius;
    /* 0x38 */ f32 halfHeight;     /* |dy| < a.halfHeight + b.halfHeight */
    /* 0x3C */ f32 radius2;        /* set to the radius too */
} CollCylinder;

/* the contact record test() fills and onContact() receives (0x40) */
typedef struct CollContact {
    /* 0x00 */ f32 normal[3];
    /* 0x0C */ f32 depth;
    /* 0x10 */ char pad10[0x28 - 0x10];
    /* 0x28 */ CollObj *other;
    /* 0x2C */ char pad2C[0x40 - 0x2C];
} CollContact;

/* CollObjMgr (0x34, CollObjMgr_ctor, sCollObjMgrName) */
typedef struct CollObjMgr {
    /* 0x00 */ Node node;
    /* 0x2C */ CollObj *statics;   /* CollObjMgr_addStatic pushes at the head */
    /* 0x30 */ CollObj *dynamics;  /* CollObjMgr_addDynamic */
} CollObjMgr;

/* Static collision mesh: CollShape (0x90; CollShape_ctor 0x3AC5C reads a token stream,
 * CollShape_ctorPoly 0x3AE58 builds one from a BasePolyShape, NullCollShape_ctor is the empty
 * one). The queries (CollShape_height / below / raycast / query / nearestHit, NCC donors
 * game_libs_ncc_3B2EC / 3B6A0 / 3B9C0 / 3BE1C / 3C43C) walk tris[] in step with the shape's
 * triangles: tris[i] is the plane record of shape->tris[i] (or of shape->tris[remap[i]] when
 * the shape's 0x4C tri list is set) and its face normal is shape->faceNormals[i]. */

/* one triangle's plane record (12 bytes; CollShape_initTriPlane(tri, vertex0, normal, index)) */
typedef struct CollTri {
    /* 0x0 */ s8 axU;              /* the two axes kept by the 2D edge tests (the dominant */
    /* 0x1 */ s8 axV;              /* axis dropped: 0 -> (2, 1), 1 -> (0, 2), 2 -> (1, 0)) */
    /* 0x2 */ s8 axis;             /* index of the normal's largest |component| */
    /* 0x3 */ s8 sign;             /* 1 / -1: sign of normal[axis] */
    /* 0x4 */ s16 tag;             /* triangle index at build; < 0 = disabled (the queries skip it) */
    /* 0x8 */ f32 d;               /* plane offset: normal . vertex0 */
} CollTri;

/* the XZ cell grid at BasePolyShape +0xA0 (parse cmd 35) that CollShape_query looks the
 * point up in: cells[z * w + x] is an index into lists (-1 = empty), each list a -1-terminated
 * run of triangle indices */
typedef struct CollGrid {
    /* 0x00 */ s32 w;
    /* 0x04 */ s32 h;
    /* 0x08 */ s32 x0;
    /* 0x0C */ s32 z0;
    /* 0x10 */ f32 cellW;
    /* 0x14 */ f32 cellH;
    /* 0x18 */ s16 *lists;
    /* 0x1C */ s32 *cells;
} CollGrid;

typedef struct CollShape {
    /* 0x00 */ Node node;          /* "CollShape" / "polyshape CollShape" / "NullCollShape" */
    /* 0x2C */ CollTri *tris;      /* one per shape triangle (ctorPoly: alloc(nTris * 12)) */
    /* 0x30 */ s32 unk30;          /* stream cmd 2 */
    /* 0x34 */ s32 *stream;        /* CollShape_ctor arg 1: the token stream */
    /* 0x38 */ f32 unk38;          /* stream cmd 1 */
    /* 0x3C */ f32 box[6];         /* stream cmd 0: six ints converted to float */
    /* 0x54 */ f32 unk54[3];       /* 0.0f at ctor */
    char pad60[0x84 - 0x60];
    /* 0x84 */ struct BasePolyShape *shape; /* stream cmd 3 (BasePolyShape_ctor "colshape") or
                                    * the ctorPoly source; 0 = NullCollShape */
    /* 0x88 */ s32 unk88;          /* 0 at ctor unless stream cmd 4 set the flag */
    /* 0x8C */ s32 unk8C;
} CollShape;

typedef char CollObj_size_check[sizeof(CollObj) == 0x34 ? 1 : -1];
#ifndef __cplusplus /* edg (NCC donors): &((T *)0)->f is not an integral constant expression */
typedef char CollObj_vptr_check[(int)&((CollObj *)0)->vptr == 0x30 ? 1 : -1];
#endif
typedef char CollCylinder_size_check[sizeof(CollCylinder) == 0x40 ? 1 : -1];
typedef char CollContact_size_check[sizeof(CollContact) == 0x40 ? 1 : -1];
typedef char CollObjMgr_size_check[sizeof(CollObjMgr) == 0x34 ? 1 : -1];

typedef char CollTri_size_check[sizeof(CollTri) == 12 ? 1 : -1];
typedef char CollGrid_size_check[sizeof(CollGrid) == 0x20 ? 1 : -1];
typedef char CollShape_size_check[sizeof(CollShape) == 0x90 ? 1 : -1];
#ifndef __cplusplus /* edg (NCC donors): &((T *)0)->f is not an integral constant expression */
typedef char CollShape_shape_check[(int)&((CollShape *)0)->shape == 0x84 ? 1 : -1];
#endif

#define COLLOBJ(p) ((CollObj *)(p))
#define COLLCYL(p) ((CollCylinder *)(p))
#define COLLCONTACT(p) ((CollContact *)(p))
#define COLLOBJMGR(p) ((CollObjMgr *)(p))
#define COLLSHAPE(p) ((CollShape *)(p))

#endif /* GDL_COLL_H */
