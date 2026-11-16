#ifndef GDL_NODELIST_H
#define GDL_NODELIST_H

/* NodeListState (0x4C): the ProcNodeList interpreter state (game_libs 0x5591C), passed by
 * pointer to every command handler (shape_cmd*, skin_cmd*, nodelist_check*) and by value to
 * the sub-list calls. The edg donor nodelist_dispatch (game_libs_ncc_55470.cpp) calls it
 * Strm. ProcNodeList builds it: cursor = &list pointer, mnode = node = a1, animMgr = a2,
 * obj / unk18 / op113 / parent = 0, linkNodes = 1. docs/1080_GAME_CLASSES.md section 20 has
 * the command set; docs/1080_GRAPHICS.md section 19 the handler fields.
 *
 * Plain C types only (the NCC donors may include it). */

typedef struct NodeListState {
    /* 0x00 */ int **cursor;     /* the next word is *(*cursor)++ */
    /* 0x04 */ int cmd;          /* the last word read: the command, then each argument */
    /* 0x08 */ int unk08;
    /* 0x0C */ char *mnode;      /* the current MNode (nodelist_checkMNode "No MNode") */
    /* 0x10 */ char *node;       /* the current Node (nodelist_checkNode "No Node") */
    /* 0x14 */ char *animMgr;    /* ProcNodeList a2; anim_createAnimator "Null AnimatorMgr specified" */
    /* 0x18 */ int unk18;
    /* 0x1C */ int unk1C[3];
    /* 0x28 */ char *obj;        /* the object the last constructing command built */
    /* 0x2C */ char *light;      /* light_addDefault_000513CC: the DefLight it built */
    /* 0x30 */ int unk30;
    /* 0x34 */ int op113;        /* node op 113 sets it; shape_cmdShape picks mode 3 / 6 by it */
    /* 0x38 */ int linkNodes;    /* op 1016 sets it (ProcNodeList: 1): the *_def commands append
                                * each new node to parent->node's child list */
    /* 0x3C */ struct NodeListState *parent; /* the calling state of a 1003 sub-list
                                              * (nodelist_checkPrevNode / PrevMNode) */
    /* 0x40 */ int character;    /* op 1010 runs its block only when the argument equals it */
    /* 0x44 */ int boardIdx;     /* op 1011 likewise */
    /* 0x48 */ int mask;         /* op 1012 runs its block only when (mask & argument) */
} NodeListState;

typedef char NodeListState_size_check[sizeof(NodeListState) == 0x4C ? 1 : -1];
#ifndef __cplusplus /* edg (NCC donors): &((T *)0)->f is not an integral constant expression */
typedef char NodeListState_obj_check[(int)&((NodeListState *)0)->obj == 0x28 ? 1 : -1];
typedef char NodeListState_parent_check[(int)&((NodeListState *)0)->parent == 0x3C ? 1 : -1];
#endif

#endif /* GDL_NODELIST_H */
