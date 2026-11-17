#include "common.h"
#include "uso_call.h"
#include "gdl/node.h"  /* Node, NodeMsg, VT_SLOT */
#include "gdl/mnode.h" /* SNode (scale +0x70) */
#include "gdl/gfxlib.h"
#include "game/course_objects.h"

extern int gl_func_00000000();
extern char D_00000000;

/* checkpoint.uso: checkpoint course objects (two SNode subclasses, a CollObj
 * cylinder gate at 0xE0 and a box gate at 0x58C, plus their templates).
 * IDO 7.1 cfe -O2, like game_uso. Relocs (scripts/uso-imports.py sites
 * checkpoint): bootup Bss+0x3C740 = the gdl file system object, Bss+0x3C768 =
 * the hostio object. */

/* checkpoint_gdlWriteInt: write an int to the gdl file (cross-USO
 * template, as game_libs fs_gdlWriteInt_0003D000). */
void checkpoint_gdlWriteInt(int *a0) {
    int pad, scratch;
    scratch = *a0;
    CALL(fs_gdlWriteFile)(&D_00000000, &scratch, 4);
}

/* checkpoint_gdlReadInt_00000034: read an int from the gdl file (template, as
 * game_uso stream_readS32_00000280). */
void checkpoint_gdlReadInt_00000034(int *dst) {
    int buf[2];
    CALL(fs_gdlReadFile)(&D_00000000, buf, 4);
    *dst = buf[0];
}

/* checkpoint_gdlWriteInt_00000070: second copy of the int writer (one per class TU). */
void checkpoint_gdlWriteInt_00000070(int *a0) {
    int pad, scratch;
    scratch = *a0;
    CALL(fs_gdlWriteFile)(&D_00000000, &scratch, 4);
}

/* checkpoint_gdlReadInt_000000A4: second copy of the int reader. */
void checkpoint_gdlReadInt_000000A4(int *dst) {
    int buf[2];
    CALL(fs_gdlReadFile)(&D_00000000, buf, 4);
    *dst = buf[0];
}

/* CheckpointCyl_ctor: cylinder Checkpoint ctor (0x10C bytes, cfront this==0
 * alloc): SNode_ctor, vptr Data+0x3E20, two inline CollObj box ctors at +0x8C / +0xCC
 * (coll_boxCtor + CollObj vptr, bootup Data+0x1EDF8), +0x88 = 20, then both become
 * cylinders (CollObj_initCylinder(c, self, 100, 100.0f, 1000.0f)).
 * Frame lever: only the inline ctor's `p` is a named local (its home is sp+0x34);
 * the two CollObj addresses are plain `self + K` expressions, CSE'd by uopt into
 * spill temps (sp+0x30 / 0x2C). Named c1 / c2 locals each add a dead home word
 * (frame 0x40 instead of 0x38). */
extern char CollObj_vtbl_chkE0;
extern void gl_func_00000000_chkcyl(char *, char *, int, float, float);
int *CheckpointCyl_ctor(char *self) {
    char *p;
    if (self != 0 || (self = (char *)CALL(mem_new)(0x10C)) != 0) {
        CALL(SNode_ctor)(self);
        NODE(self)->vptr = &D_00000000;
        p = self + 0x8C;
        if (p != 0 || (p = (char *)CALL(mem_new)(0x40)) != 0) {
            CALL(coll_boxCtor_0003CAA0)(p);
            ((CollObj *)p)->vptr = &CollObj_vtbl_chkE0;
        }
        p = self + 0xCC;
        if (p != 0 || (p = (char *)CALL(mem_new)(0x40)) != 0) {
            CALL(coll_boxCtor_0003CAA0)(p);
            ((CollObj *)p)->vptr = &CollObj_vtbl_chkE0;
        }
        CHECKPOINTCYL(self)->time = 20;
        XCALL(gl_func_00000000_chkcyl, CollObj_initCylinder)(self + 0x8C, self, 100, 100.0f, 1000.0f);
        XCALL(gl_func_00000000_chkcyl, CollObj_initCylinder)(self + 0xCC, self, 100, 100.0f, 1000.0f);
    }
    return (int *)self;
}

/* CheckpointCyl_loadData: cylinder checkpoint loadData (int at 0x88). */
void CheckpointCyl_loadData(char *self) {
    checkpoint_gdlReadInt_00000034(&CHECKPOINTCYL(self)->time);
    CALL(SNode_loadData)(self);
}

/* CheckpointCyl_saveData: cylinder checkpoint saveData. */
void CheckpointCyl_saveData(char *self) {
    checkpoint_gdlWriteInt(&CHECKPOINTCYL(self)->time);
    CALL(SNode_saveData)(self);
}

/* CheckpointCyl_hostioParms: cylinder checkpoint hostioParms (one int, 0..100). */
void CheckpointCyl_hostioParms(char *self) {
    char *name = (char *)NODE(self)->name;
    CALL(hostio_beginGroup)(&D_00000000, name, 0);
    CALL(hostio_addInt)(&D_00000000, &D_00000000 + 0x3EE0, &CHECKPOINTCYL(self)->time, 0, 100, 0); /* "Time" */
    CALL(hostio_endGroup)(&D_00000000);
    CALL(SNode_hostioParms)(self);
}

/* CheckpointCyl_handleMsg: cylinder checkpoint handleMsg: after
 * SNode_handleMsg, messages 7 (move) and 19 (rebuild) place the two gate posts
 * at (-/+ r, 0, 0) through the SNode's 3x4 Mtx (+0x2C, rows of 4) plus pos,
 * r = 117 / GfxLib.worldScale[0]; posts stored at +0xA4 and +0xE4.
 * Levers: the zero is `z = 0.0` (a double literal keeps the live `mul.s` by an
 * mtc1 zero; `0.0f` folds the terms away); each sum is spelled right-nested
 * (IDO evaluates the right operand first); `m` declared before the two Vec3
 * temps gives their homes (sp+0x5C / 0x50). */
typedef struct { float x, y, z; } ChkVec3;
void CheckpointCyl_handleMsg(char *self, int *msg) {
    float r;
    float z;
    float nr;
    float *m;
    ChkVec3 a;
    ChkVec3 b;

    CALL(SNode_handleMsg)(self, msg);
    if (*msg == 7 || *msg == 19) {
        r = 117.0f / GFXLIB(&D_00000000)->worldScale[0];
        z = 0.0;
        nr = -r;
        m = (float *)SNODE(self)->mtx;
        a.x = m[12] + (m[8] * z + (m[4] * z + m[0] * nr));
        a.y = m[13] + (m[9] * z + (m[5] * z + m[1] * nr));
        a.z = m[14] + (m[10] * z + (m[6] * z + m[2] * nr));
        CHECKPOINTCYL(self)->postA.obj.pos[0] = a.x;
        CHECKPOINTCYL(self)->postA.obj.pos[1] = a.y;
        CHECKPOINTCYL(self)->postA.obj.pos[2] = a.z;
        b.x = m[12] + (m[8] * z + (m[4] * z + m[0] * r));
        b.y = m[13] + (m[9] * z + (m[5] * z + m[1] * r));
        b.z = m[14] + (m[10] * z + (m[6] * z + m[2] * r));
        CHECKPOINTCYL(self)->postB.obj.pos[0] = b.x;
        CHECKPOINTCYL(self)->postB.obj.pos[1] = b.y;
        CHECKPOINTCYL(self)->postB.obj.pos[2] = b.z;
    }
}

/* CheckpointCyl_isBetweenPosts: cylinder checkpoint "between the posts" test (pos - postA and
 * pos - postB in the XZ plane, dot < 0). EXACT via an IDO 7.1 NCC (C++) donor,
 * src/checkpoint_uso/checkpoint_uso_ncc_45C.cpp, REPLACE_FUNC_BODY-spliced over this placeholder. */
int CheckpointCyl_isBetweenPosts(char *self, float *p) {
    return 0;
}

/* CheckpointBox_ctor: box Checkpoint ctor (0xCC bytes, cfront this==0
 * alloc), the E0 shape with one CollObj: SNode_ctor, vptr Data+0x3E80, inline
 * CollObj ctor at +0x88, CollObj_initCylinder(c, self, 100, 20.0f, 500.0f), then
 * +0xC8 = arg (the template's +0x48 word). Same frame lever as E0. */
int *CheckpointBox_ctor(char *self, int arg) {
    char *p;
    if (self != 0 || (self = (char *)CALL(mem_new)(0xCC)) != 0) {
        CALL(SNode_ctor)(self);
        NODE(self)->vptr = &D_00000000;
        p = self + 0x88;
        if (p != 0 || (p = (char *)CALL(mem_new)(0x40)) != 0) {
            CALL(coll_boxCtor_0003CAA0)(p);
            ((CollObj *)p)->vptr = &CollObj_vtbl_chkE0;
        }
        XCALL(gl_func_00000000_chkcyl, CollObj_initCylinder)(self + 0x88, self, 100, 20.0f, 500.0f);
        CHECKPOINTBOX(self)->tmplArg = arg;
    }
    return (int *)self;
}

/* CheckpointBox_loadData: box checkpoint loadData (int at 0xC8). */
void CheckpointBox_loadData(char *self) {
    checkpoint_gdlReadInt_000000A4(&CHECKPOINTBOX(self)->tmplArg);
    CALL(SNode_loadData)(self);
}

/* CheckpointBox_saveData: box checkpoint saveData. */
void CheckpointBox_saveData(char *self) {
    checkpoint_gdlWriteInt_00000070(&CHECKPOINTBOX(self)->tmplArg);
    CALL(SNode_saveData)(self);
}

/* CheckpointBox_handleMsg: box checkpoint handleMsg: messages 7 and 19
 * copy the Vec3 at 0x5C into 0xA0. */
void CheckpointBox_handleMsg(char *self, int *msg) {
    CALL(SNode_handleMsg)(self, msg);
    if (*msg == 7 || *msg == 19) {
        CHECKPOINTBOX(self)->coll.obj.pos[0] = SNODE(self)->pos[0];
        CHECKPOINTBOX(self)->coll.obj.pos[1] = SNODE(self)->pos[1];
        CHECKPOINTBOX(self)->coll.obj.pos[2] = SNODE(self)->pos[2];
    }
}

int *CheckpointCyl_ctor(char *self);
int *CheckpointBox_ctor(char *self, int arg);
extern int gPolyFlags_3EEC4_tmpl_rd, gPolyFlags_3EEC4_tmpl_wr;
extern int gPolyGeomMode_3EEC8_tmpl[2];
extern int gCollObjMgr_tmpl;

/* CheckpointCylTmpl_create: cylinder CheckpointTmpl create. Builds the
 * template's PolyShape on first use (as balloon 47C / bootup TreeTmpl_create),
 * new cylinder checkpoint (0xE0), scale 10, handleMsg (vtable slot 4) tag 19,
 * attach the shape, then register its two CollObjs (+0x8C, +0xCC) as statics.
 * Levers as fire 148 / balloon 47C (NodeMsg + volatile pad frame, `if (o) {}`
 * after the ctor, `if (1) {}` after the flag store); here o lands in s0.
 * The ROM leaves these local ctor jal fields 0 (filled by the USO TextReloc),
 * so the ctor calls go through CALL (the zero placeholder), not a direct call. */
Node *CheckpointCylTmpl_create(char *tmpl) {
    NodeMsg msg;
    Node *o;
    Node *shape;
    volatile int pad;

    if ((Node *)ITEMTYPE(tmpl)->unk40 == 0) {
        gPolyFlags_3EEC4_tmpl_wr = gPolyFlags_3EEC4_tmpl_rd | 8;
        if (1) {}
        gPolyGeomMode_3EEC8_tmpl[1] = 0xAA002;
        gPolyGeomMode_3EEC8_tmpl[0] = 0;
        *(Node **)&ITEMTYPE(tmpl)->unk40 = (Node *)CALL(PolyShape_ctor)(0, &D_00000000 + 0x3EE8, SHAPETMPL(tmpl)->model, 0); /* "chkp" */
    }
    o = (Node *)CALL(CheckpointCyl_ctor)(0);
    if (o) {}
    SNODE(o)->scale = 10.0f;
    msg.tag = 19;
    VT_SLOT(o->vptr, 4).fn((char *)o + VT_SLOT(o->vptr, 4).delta, &msg);
    shape = (Node *)ITEMTYPE(tmpl)->unk40;
    CALL(list_append)(&o->children, shape);
    if (shape->parent != 0) {
        shape->unk04 = 1;
    }
    shape->parent = o;
    CALL(CollObjMgr_addStatic)(gCollObjMgr_tmpl, (char *)o + 0x8C);
    CALL(CollObjMgr_addStatic)(gCollObjMgr_tmpl, (char *)o + 0xCC);
    return o;
}

/* CheckpointBoxTmpl_create: box CheckpointTmpl create: the 6F0 shape with
 * geomMode 0x8A002, the shape data at tmpl+0x4C, new box checkpoint
 * (0x58C, passed tmpl+0x48) and one static CollObj (+0x88). */
Node *CheckpointBoxTmpl_create(char *tmpl) {
    NodeMsg msg;
    Node *o;
    Node *shape;
    volatile int pad;

    if ((Node *)ITEMTYPE(tmpl)->unk40 == 0) {
        gPolyFlags_3EEC4_tmpl_wr = gPolyFlags_3EEC4_tmpl_rd | 8;
        if (1) {}
        gPolyGeomMode_3EEC8_tmpl[1] = 0x8A002;
        gPolyGeomMode_3EEC8_tmpl[0] = 0;
        *(Node **)&ITEMTYPE(tmpl)->unk40 = (Node *)CALL(PolyShape_ctor)(0, &D_00000000 + 0x3EF0, CHECKPOINTBOXTMPL(tmpl)->model, 0); /* "flag" */
    }
    o = (Node *)CALL(CheckpointBox_ctor)(0, CHECKPOINTBOXTMPL(tmpl)->boxArg);
    if (o) {}
    SNODE(o)->scale = 10.0f;
    msg.tag = 19;
    VT_SLOT(o->vptr, 4).fn((char *)o + VT_SLOT(o->vptr, 4).delta, &msg);
    shape = (Node *)ITEMTYPE(tmpl)->unk40;
    CALL(list_append)(&o->children, shape);
    if (shape->parent != 0) {
        shape->unk04 = 1;
    }
    shape->parent = o;
    CALL(CollObjMgr_addStatic)(gCollObjMgr_tmpl, (char *)o + 0x88);
    return o;
}

/* CheckpointColourItemType_create: the checkpoint ColourItem type's create: alloc
 * 0x40, ColourItem_ctor, vtable (bootup Data+0x75A0), clear +0x3C, attach the
 * type's +0x40 node. Byte-identical to eddproc edd_ItemType_create_000003BC;
 * its body is ported verbatim (the p1 / p2 split and the p1 reuse for tmpl are
 * load-bearing, see the edd comment). */
void *CheckpointColourItemType_create(int *arg0) {
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

