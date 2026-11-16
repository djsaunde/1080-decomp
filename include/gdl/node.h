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
/* One record of a Node child list (list_append, game_libs post1b: {item, next}, 8 bytes, from
 * list_allocNode). Node.children points at the first; the item is usually the child Node. */
typedef struct NodeLink {
    /* 0x0 */ void *item;
    /* 0x4 */ struct NodeLink *next;
} NodeLink;

typedef struct Node {
    /* 0x00 */ s32 unk00;          /* -1 */
    /* 0x04 */ s32 unk04;          /* 0 */
    /* 0x08 */ s32 flags;          /* 0x8000 */
    /* 0x0C */ const char *name;   /* e.g. "App", "<Node>" */
    /* 0x10 */ NodeLink *children; /* child list head (list_append) */
    /* 0x14 */ struct Node *parent; /* the node whose child list (+0x10) holds this one */
    /* 0x18 */ s32 doFlags;        /* flags sub-object (initialised by a call on its address):
                                    * 0x4 DoDisplay (Node_display), 0x8 DoUpdate (Node_update),
                                    * 0x2 sent by hostio_sendNodeTree; Node_hostioParms publishes 4/8 */
    /* 0x1C */ s32 serial;         /* post-incremented global counter */
    /* 0x20 */ s32 handle;         /* result of the registry call (this, serial) */
    /* 0x24 */ s32 unk24;
    /* 0x28 */ char *vptr;         /* Cfront vtable pointer (slots via VT_SLOT) */
} Node;

#ifndef __cplusplus /* edg (NCC donors): &((T *)0)->f is not an integral constant expression */
typedef char Node_size_check[sizeof(Node) == 0x2C ? 1 : -1];
typedef char Node_name_check[(int)&((Node *)0)->name == 0x0C ? 1 : -1];
typedef char Node_serial_check[(int)&((Node *)0)->serial == 0x1C ? 1 : -1];
typedef char Node_vptr_check[(int)&((Node *)0)->vptr == 0x28 ? 1 : -1];
#endif

/* Cfront vtable: slot N is {s16 this-delta, pad, fn} at vtbl + 8 + 8*N, called as
 * vt->slot[N].fn((char *)this + vt->slot[N].delta, ...). VT_SLOT(vt, N) is slot N of a vtable
 * given as a `char *` / s32 (fn at vt + 0xC + 8*N, delta at vt + 8 + 8*N). */
typedef struct GdlVtEnt {
    /* 0x0 */ s16 delta;
    /* 0x2 */ s16 pad;
    /* 0x4 */ s32 (*fn)();
} GdlVtEnt;

#define VT_SLOT(vt, n) (((GdlVtEnt *)((char *)(vt) + 8))[n])

/* Field access through an untyped `char *` / `int` object pointer (agent-h run 492), e.g. the
 * child-attach idiom every ctor inlines: `if (NODE(c)->parent != 0) NODE(c)->unk04 = 1;
 * NODE(c)->parent = (Node *)this;`. Lowers to the same base + offset access as the raw form. */
#define NODE(p) ((Node *)(p))

/* A Node message, the second argument of the handleMsg / doCommand vtable slots (Node_handleMsg
 * 0x38830, MNode/SNode/QNode handlers): a tag, an int or payload-pointer argument and a reply word.
 * Node_handleMsg answers four tags: 0xC sets/clears doFlags bit 0x80 from payload +4; 0xD stores
 * this node in `reply` when its name compares equal (callback(name, *payload) == 0); 0xE does
 * the same when serial == (s16)payload +4; 0x2 sets/clears DoDisplay (0x4) and DoUpdate (0x8). */
typedef struct NodeMsg {
    /* 0x0 */ s32 tag;
    /* 0x4 */ union {
        s32 value;                 /* NODEMSG_SET_ACTIVE: the on / off flag */
        char *payload;             /* SET_FLAG80 / FIND_NAME / FIND_SERIAL: the tag's payload */
    } arg;
    /* 0x8 */ Node *reply;         /* the answering Node (find tags 0xD / 0xE) */
} NodeMsg;

#define NODEMSG_SET_ACTIVE  0x2    /* arg != 0: set DoDisplay | DoUpdate, else clear both */
#define NODEMSG_SET_FLAG80  0xC
#define NODEMSG_FIND_NAME   0xD
#define NODEMSG_FIND_SERIAL 0xE

#ifndef __cplusplus
typedef char NodeMsg_reply_check[(int)&((NodeMsg *)0)->reply == 0x8 ? 1 : -1];
#endif

/* The libgdl application object (App_main, 0x415A4, sets its name to "App").
 * Only the fields the App_* bodies touch are known. */
typedef struct App {
    /* 0x00 */ Node node;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ s32 unk30;          /* cleared by the vtable method App_restart */
} App;

#ifndef __cplusplus
typedef char App_unk30_check[(int)&((App *)0)->unk30 == 0x30 ? 1 : -1];
#endif

#endif
