#ifndef GDL_NODE_H
#define GDL_NODE_H

/* libgdl Node, the base of every engine-core object (game_libs post0b).
 * Node_ctor (0x3800C) builds it; all 45 "<Node>" ctors call it first, and
 * MNode/SNode/QNode, App, CollShape and the HUD gadgets derive from it.
 * The vtable is Cfront-style: each slot is {s16 this-delta, pad, fn}, called
 * as vptr->fn(this + vptr->delta). *_hostioParms is the HostIO parm slot.
 *
 * Node_ctor stores: +0x00 = -1, +0x04 = 0, +0x08 = 0x8000, +0x0C = name,
 * +0x10 = 0 (child list), +0x14 = 0 (parent), +0x18 = the DoDisplay/DoUpdate flags sub-object,
 * +0x1C = serial from a global counter, +0x20 = a handle from a registry call,
 * +0x28 = vptr. Size 0x2C (alloc(0x2C) when this == 0). The HUD's child
 * list (include/gdl/hud.h, +0x10) and the gadget parent back-link (+0x14)
 * are the same two fields. */
typedef struct Node {
    /* 0x00 */ s32 unk00;          /* -1 */
    /* 0x04 */ s32 unk04;          /* 0 */
    /* 0x08 */ s32 flags;          /* 0x8000 */
    /* 0x0C */ s32 name;           /* const char *, e.g. "App", "<Node>" */
    /* 0x10 */ s32 children;       /* child list head */
    /* 0x14 */ s32 parent;
    /* 0x18 */ s32 doFlags;        /* flags sub-object (initialised by a call on its address):
                                    * 0x4 DoDisplay (Node_display), 0x8 DoUpdate (Node_update),
                                    * 0x2 sent by hostio_sendNodeTree; Node_hostioParms publishes 4/8 */
    /* 0x1C */ s32 serial;         /* post-incremented global counter */
    /* 0x20 */ s32 handle;         /* result of the registry call (this, serial) */
    /* 0x24 */ s32 unk24;
    /* 0x28 */ s32 vptr;           /* Cfront vtable pointer */
} Node;

typedef char Node_size_check[sizeof(Node) == 0x2C ? 1 : -1];
typedef char Node_name_check[(int)&((Node *)0)->name == 0x0C ? 1 : -1];
typedef char Node_serial_check[(int)&((Node *)0)->serial == 0x1C ? 1 : -1];
typedef char Node_vptr_check[(int)&((Node *)0)->vptr == 0x28 ? 1 : -1];

/* The libgdl application object (App_main, 0x415A4, sets its name to "App").
 * Only the fields the App_* bodies touch are known. */
typedef struct App {
    /* 0x00 */ Node node;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ s32 unk30;          /* cleared by the vtable method App_func_000417CC */
} App;

typedef char App_unk30_check[(int)&((App *)0)->unk30 == 0x30 ? 1 : -1];

#endif
