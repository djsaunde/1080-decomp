#include "common.h"
#include "uso_call.h"
#include "gdl/node.h"  /* Node, NodeMsg, VT_SLOT */
#include "gdl/mnode.h" /* SNode (scale +0x70) */

extern int gl_func_00000000();
extern char D_00000000;

/* checkpoint.uso: checkpoint course objects (two SNode subclasses, a CollObj
 * cylinder gate at 0xE0 and a box gate at 0x58C, plus their templates).
 * IDO 7.1 cfe -O2, like game_uso. Relocs (scripts/uso-imports.py sites
 * checkpoint): bootup Bss+0x3C740 = the gdl file system object, Bss+0x3C768 =
 * the hostio object. */

/* checkpoint_uso_func_00000000: write an int to the gdl file (cross-USO
 * template, as game_libs fs_gdlWriteInt_0003D000). */
void checkpoint_uso_func_00000000(int *a0) {
    int pad, scratch;
    scratch = *a0;
    CALL(fs_gdlWriteFile)(&D_00000000, &scratch, 4);
}

/* checkpoint_uso_func_00000034: read an int from the gdl file (template, as
 * game_uso stream_readS32_00000280). */
void checkpoint_uso_func_00000034(int *dst) {
    int buf[2];
    CALL(fs_gdlReadFile)(&D_00000000, buf, 4);
    *dst = buf[0];
}

/* checkpoint_uso_func_00000070: second copy of the int writer (one per class TU). */
void checkpoint_uso_func_00000070(int *a0) {
    int pad, scratch;
    scratch = *a0;
    CALL(fs_gdlWriteFile)(&D_00000000, &scratch, 4);
}

/* checkpoint_uso_func_000000A4: second copy of the int reader. */
void checkpoint_uso_func_000000A4(int *dst) {
    int buf[2];
    CALL(fs_gdlReadFile)(&D_00000000, buf, 4);
    *dst = buf[0];
}

INCLUDE_ASM("asm/nonmatchings/checkpoint_uso/checkpoint_uso", checkpoint_uso_func_000000E0);

/* checkpoint_uso_func_000001E4: cylinder checkpoint loadData (int at 0x88). */
void checkpoint_uso_func_000001E4(char *self) {
    checkpoint_uso_func_00000034((int *)(self + 0x88));
    CALL(SNode_loadData)(self);
}

/* checkpoint_uso_func_00000210: cylinder checkpoint saveData. */
void checkpoint_uso_func_00000210(char *self) {
    checkpoint_uso_func_00000000((int *)(self + 0x88));
    CALL(SNode_saveData)(self);
}

/* checkpoint_uso_func_0000023C: cylinder checkpoint hostioParms (one int, 0..100). */
void checkpoint_uso_func_0000023C(char *self) {
    char *name = *(char **)(self + 0xC); /* Node name */
    CALL(hostio_beginGroup)(&D_00000000, name, 0);
    CALL(hostio_addInt)(&D_00000000, &D_00000000 + 0x3EE0, self + 0x88, 0, 100, 0);
    CALL(hostio_endGroup)(&D_00000000);
    CALL(SNode_hostioParms)(self);
}

INCLUDE_ASM("asm/nonmatchings/checkpoint_uso/checkpoint_uso", checkpoint_uso_func_000002AC);

INCLUDE_ASM("asm/nonmatchings/checkpoint_uso/checkpoint_uso", checkpoint_uso_func_0000045C);

INCLUDE_ASM("asm/nonmatchings/checkpoint_uso/checkpoint_uso", checkpoint_uso_func_0000058C);

/* checkpoint_uso_func_0000063C: box checkpoint loadData (int at 0xC8). */
void checkpoint_uso_func_0000063C(char *self) {
    checkpoint_uso_func_000000A4((int *)(self + 0xC8));
    CALL(SNode_loadData)(self);
}

/* checkpoint_uso_func_00000668: box checkpoint saveData. */
void checkpoint_uso_func_00000668(char *self) {
    checkpoint_uso_func_00000070((int *)(self + 0xC8));
    CALL(SNode_saveData)(self);
}

/* checkpoint_uso_func_00000694: box checkpoint handleMsg: messages 7 and 19
 * copy the Vec3 at 0x5C into 0xA0. */
void checkpoint_uso_func_00000694(char *self, int *msg) {
    CALL(SNode_handleMsg)(self, msg);
    if (*msg == 7 || *msg == 19) {
        *(float *)(self + 0xA0) = *(float *)(self + 0x5C);
        *(float *)(self + 0xA4) = *(float *)(self + 0x60);
        *(float *)(self + 0xA8) = *(float *)(self + 0x64);
    }
}

int *checkpoint_uso_func_000000E0(char *self);
int *checkpoint_uso_func_0000058C(char *self, int arg);
extern int gPolyFlags_3EEC4_tmpl_rd, gPolyFlags_3EEC4_tmpl_wr;
extern int gPolyGeomMode_3EEC8_tmpl[2];
extern int gCollObjMgr_tmpl;

/* checkpoint_uso_func_000006F0: cylinder CheckpointTmpl create. Builds the
 * template's PolyShape on first use (as balloon 47C / bootup TreeTmpl_create),
 * new cylinder checkpoint (0xE0), scale 10, handleMsg (vtable slot 4) tag 19,
 * attach the shape, then register its two CollObjs (+0x8C, +0xCC) as statics.
 * Levers as fire 148 / balloon 47C (NodeMsg + volatile pad frame, `if (o) {}`
 * after the ctor, `if (1) {}` after the flag store); here o lands in s0.
 * The ROM leaves these local ctor jal fields 0 (filled by the USO TextReloc),
 * so the ctor calls go through CALL (the zero placeholder), not a direct call. */
Node *checkpoint_uso_func_000006F0(char *tmpl) {
    NodeMsg msg;
    Node *o;
    Node *shape;
    volatile int pad;

    if (*(Node **)(tmpl + 0x40) == 0) {
        gPolyFlags_3EEC4_tmpl_wr = gPolyFlags_3EEC4_tmpl_rd | 8;
        if (1) {}
        gPolyGeomMode_3EEC8_tmpl[1] = 0xAA002;
        gPolyGeomMode_3EEC8_tmpl[0] = 0;
        *(Node **)(tmpl + 0x40) = (Node *)CALL(PolyShape_ctor)(0, &D_00000000 + 0x3EE8, *(int *)(tmpl + 0x48), 0);
    }
    o = (Node *)CALL(checkpoint_uso_func_000000E0)(0);
    if (o) {}
    SNODE(o)->scale = 10.0f;
    msg.tag = 19;
    VT_SLOT(o->vptr, 4).fn((char *)o + VT_SLOT(o->vptr, 4).delta, &msg);
    shape = *(Node **)(tmpl + 0x40);
    CALL(list_append)(&o->children, shape);
    if (shape->parent != 0) {
        shape->unk04 = 1;
    }
    shape->parent = o;
    CALL(CollObjMgr_addStatic)(gCollObjMgr_tmpl, (char *)o + 0x8C);
    CALL(CollObjMgr_addStatic)(gCollObjMgr_tmpl, (char *)o + 0xCC);
    return o;
}

/* checkpoint_uso_func_000007F4: box CheckpointTmpl create: the 6F0 shape with
 * geomMode 0x8A002, the shape data at tmpl+0x4C, new box checkpoint
 * (0x58C, passed tmpl+0x48) and one static CollObj (+0x88). */
Node *checkpoint_uso_func_000007F4(char *tmpl) {
    NodeMsg msg;
    Node *o;
    Node *shape;
    volatile int pad;

    if (*(Node **)(tmpl + 0x40) == 0) {
        gPolyFlags_3EEC4_tmpl_wr = gPolyFlags_3EEC4_tmpl_rd | 8;
        if (1) {}
        gPolyGeomMode_3EEC8_tmpl[1] = 0x8A002;
        gPolyGeomMode_3EEC8_tmpl[0] = 0;
        *(Node **)(tmpl + 0x40) = (Node *)CALL(PolyShape_ctor)(0, &D_00000000 + 0x3EF0, *(int *)(tmpl + 0x4C), 0);
    }
    o = (Node *)CALL(checkpoint_uso_func_0000058C)(0, *(int *)(tmpl + 0x48));
    if (o) {}
    SNODE(o)->scale = 10.0f;
    msg.tag = 19;
    VT_SLOT(o->vptr, 4).fn((char *)o + VT_SLOT(o->vptr, 4).delta, &msg);
    shape = *(Node **)(tmpl + 0x40);
    CALL(list_append)(&o->children, shape);
    if (shape->parent != 0) {
        shape->unk04 = 1;
    }
    shape->parent = o;
    CALL(CollObjMgr_addStatic)(gCollObjMgr_tmpl, (char *)o + 0x88);
    return o;
}

/* checkpoint_uso_func_000008E8: the checkpoint ColourItem type's create: alloc
 * 0x40, ColourItem_ctor, vtable (bootup Data+0x75A0), clear +0x3C, attach the
 * type's +0x40 node. Byte-identical to eddproc edd_ItemType_create_000003BC;
 * its body is ported verbatim (the p1 / p2 split and the p1 reuse for tmpl are
 * load-bearing, see the edd comment). */
void *checkpoint_uso_func_000008E8(int *arg0) {
    int *p2;
    int *head;
    int *p1;
    p1 = (int *)CALL(mem_new)(0x40);
    if (p1 != 0) {
        CALL(ColourItem_ctor)(p1);
        NODE(p1)->vptr = (char *)&D_00000000;
        p1[0x3C / 4] = 0;
    }
    p2 = p1;
    p1 = arg0;
    head = (int *)p1[0x40 / 4];
    if ((int *)p1[0x40 / 4] != 0) {
        CALL(list_append)((char *)p2 + 0x10, head);
        if ((int)NODE(head)->parent != 0) {
            NODE(head)->unk04 = 1;
        }
        NODE(head)->parent = (Node *)p2;
    }
    return p2;
}

