#ifndef GDL_COLL_H
#define GDL_COLL_H

#include "gdl/node.h"

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
    /* 0x00 */ s32 owner;          /* the Node (CollObj_init arg1) */
    /* 0x04 */ struct CollObj *next; /* CollObjMgr list link */
    /* 0x08 */ s32 kind;           /* CollObj_init arg2; 1 = cylinder (CollObj_initCylinder) */
    /* 0x0C */ s16 id;             /* CollObj_init arg3 */
    /* 0x0E */ s16 unk0E;
    /* 0x10 */ s32 flags;          /* bit 0: CollObj_sphereHitTest reports an unresolved overlap as a zero contact */
    /* 0x14 */ s16 hitMask;        /* 0xFFFF at init: categories this object reacts to */
    /* 0x16 */ s16 category;       /* 0xFFFF at init */
    /* 0x18 */ f32 pos[3];
    /* 0x24 */ f32 vel[3];         /* CollObj_sphereHitTest sweeps pos by the relative velocity */
    /* 0x30 */ s32 vptr;
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

typedef char CollObj_size_check[sizeof(CollObj) == 0x34 ? 1 : -1];
typedef char CollObj_vptr_check[(int)&((CollObj *)0)->vptr == 0x30 ? 1 : -1];
typedef char CollCylinder_size_check[sizeof(CollCylinder) == 0x40 ? 1 : -1];
typedef char CollContact_size_check[sizeof(CollContact) == 0x40 ? 1 : -1];
typedef char CollObjMgr_size_check[sizeof(CollObjMgr) == 0x34 ? 1 : -1];

#define COLLOBJ(p) ((CollObj *)(p))
#define COLLCYL(p) ((CollCylinder *)(p))
#define COLLCONTACT(p) ((CollContact *)(p))
#define COLLOBJMGR(p) ((CollObjMgr *)(p))

#endif /* GDL_COLL_H */
