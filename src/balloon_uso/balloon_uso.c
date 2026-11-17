#include "common.h"
#include "uso_call.h"
#include "gdl/node.h"  /* Node, NodeMsg, VT_SLOT */
#include "gdl/mnode.h" /* SNode (scale +0x70) */
#include "game/course_objects.h"

extern int gl_func_00000000();
extern char D_00000000;
extern char gl_data_00000000;

typedef struct { int a, b, c; } BalloonTri3i;

/* balloon.uso: the balloon course object (an SNode subclass, 0x9C bytes) and
 * its PolyShape-based template. IDO 7.1 cfe -O2, like game_uso.
 * Relocs (scripts/uso-imports.py sites balloon): bootup Bss+0x3C740 = the
 * gdl file system object (fs_gdlReadFile / fs_gdlWriteFile), Bss+0x3C768 =
 * the hostio object, Bss+0x3C168 = GfxLib. Object fields: 0x88 Vec3 bob
 * amplitude (hostio "x/y/z" +-500), 0x94 float bob speed, 0x98 float phase. */

/* balloon_gdlWriteVec3: write a Vec3 to the gdl file (the cross-USO
 * template, same body as game_libs fs_gdlWriteVec3_00037D48). */
void balloon_gdlWriteVec3(int *a0) {
    volatile int padA[1];
    float fbuf[3];
    volatile int padB[2];
    float tmp[3];
    volatile int padC[3];
    *(BalloonTri3i *)tmp = *(BalloonTri3i *)a0;
    fbuf[0] = tmp[0]; fbuf[1] = tmp[1]; fbuf[2] = tmp[2];
    CALL(fs_gdlWriteFile)(&D_00000000, fbuf, 12);
    (void)padA;
    (void)padB;
    (void)padC;
}

/* balloon_gdlWriteFloat_00000060: write a float to the gdl file. */
void balloon_gdlWriteFloat_00000060(float *a0) {
    float pad, scratch;
    scratch = *a0;
    CALL(fs_gdlWriteFile)(&D_00000000, &scratch, 4);
}

/* balloon_gdlReadVec3_00000094: read a Vec3 from the gdl file (game_uso
 * stream_readVec3_000001D4 template). */
void balloon_gdlReadVec3_00000094(float *dst) {
    int pad_top[1];
    BalloonTri3i raw;
    int pad_mid[2];
    BalloonTri3i tmp;
    int pad_bot[2];
    CALL(fs_gdlReadFile)(&D_00000000, &raw, 12);
    tmp = raw;
    dst[0] = *(float *)&tmp.a;
    dst[1] = *(float *)&tmp.b;
    dst[2] = *(float *)&tmp.c;
}

/* balloon_gdlReadFloat_00000104: read a float from the gdl file. */
void balloon_gdlReadFloat_00000104(float *dst) {
    float buf[2];
    CALL(fs_gdlReadFile)(&D_00000000, buf, 4);
    *dst = buf[0];
}

/* Balloon_ctor: Balloon ctor (0x9C bytes; cfront this==0 alloc). */
int *Balloon_ctor(char *self) {
    if (self != 0 || (self = (char *)CALL(mem_new)(0x9C)) != 0) {
        CALL(SNode_ctor)(self);
        NODE(self)->vptr = &D_00000000; /* Data+0x4128: Balloon vtable */
        BALLOON(self)->off[0] = 0.0f;
        BALLOON(self)->off[1] = 0.0f;
        BALLOON(self)->off[2] = 0.0f;
        /* RAW: data-table RoData+0x1C float constant (gl_data_00000000 here is the RoData base, a separate zero symbol) */
        BALLOON(self)->speed = *(float *)(&gl_data_00000000 + 0x1C);
        BALLOON(self)->phase = 0.0f;
    }
    return (int *)self;
}

/* Balloon_display: Balloon display (cull, phase += speed, bob the camera
 * vector by amp * fsin(phase)). EXACT via an IDO 7.1 NCC (C++) donor,
 * src/balloon_uso/balloon_uso_ncc_1A8.cpp, REPLACE_FUNC_BODY-spliced over this placeholder. */
void Balloon_display(char *self) {
}

/* Balloon_saveData: saveData: bob amplitude, bob speed, then the SNode part. */
void Balloon_saveData(char *self) {
    balloon_gdlWriteVec3((int *)BALLOON(self)->off);
    balloon_gdlWriteFloat_00000060(&BALLOON(self)->speed);
    CALL(SNode_saveData)(self);
}

/* Balloon_loadData: loadData (mirror of saveData). */
void Balloon_loadData(char *self) {
    balloon_gdlReadVec3_00000094(BALLOON(self)->off);
    balloon_gdlReadFloat_00000104(&BALLOON(self)->speed);
    CALL(SNode_loadData)(self);
}

/* Balloon_hostioParms: hostioParms (group named by the node's name). */
extern void gl_func_00000000_ff(void *, char *, void *, float, float, int);
void Balloon_hostioParms(char *self) {
    char *name = (char *)NODE(self)->name;
#define h (&D_00000000)
    CALL(hostio_beginGroup)(h, name, 0);
    XCALL(gl_func_00000000_ff, hostio_addFloat)(h, &D_00000000 + 0x4188, &BALLOON(self)->speed, 0.0f, 1.0f, 1); /* "Speed" */
    XCALL(gl_func_00000000_ff, hostio_addFloat)(h, &D_00000000 + 0x4190, &BALLOON(self)->off[0], -500.0f, 500.0f, 1); /* "Off.x" */
    XCALL(gl_func_00000000_ff, hostio_addFloat)(h, &D_00000000 + 0x4198, &BALLOON(self)->off[1], -500.0f, 500.0f, 1); /* "Off.y" */
    XCALL(gl_func_00000000_ff, hostio_addFloat)(h, &D_00000000 + 0x41A0, &BALLOON(self)->off[2], -500.0f, 500.0f, 1); /* "Off.z" */
    CALL(hostio_endGroup)(h);
    CALL(SNode_hostioParms)(self);
}
#undef h

/* Balloon_handleMsg: handleMsg: messages 7 and 19 reset the bob phase. */
void Balloon_handleMsg(char *self, int *msg) {
    CALL(SNode_handleMsg)(self, msg);
    if (*msg == 7 || *msg == 19) {
        BALLOON(self)->phase = 0.0f;
    }
}

/* BalloonTmpl_create: BalloonTmpl create. Builds the template's PolyShape
 * on first use (gPolyFlags |= 8 and gPolyGeomMode = {0, 0xAA002} around the
 * PolyShape_ctor, as bootup TreeTmpl_create), then new Balloon, scale 10,
 * handleMsg (vtable slot 4) with tag 19 and attach the shape under it (the
 * SNodeItemType_newItem template). Same levers as fire 148: 12-byte NodeMsg +
 * `volatile int pad` (frame 0x38), `if (o) {}` (o in v1), and the TreeTmpl
 * `if (1) {}` after the flag store (keeps the flag luis ahead of the branch).
 * The ctor jal field is 0 in the ROM (USO TextReloc), so it is called via CALL. */
extern int gPolyFlags_3EEC4_tmpl_rd, gPolyFlags_3EEC4_tmpl_wr;
extern int gPolyGeomMode_3EEC8_tmpl[2];
Node *BalloonTmpl_create(char *tmpl) {
    NodeMsg msg;
    Node *o;
    Node *shape;
    volatile int pad;

    if ((Node *)ITEMTYPE(tmpl)->unk40 == 0) {
        gPolyFlags_3EEC4_tmpl_wr = gPolyFlags_3EEC4_tmpl_rd | 8;
        if (1) {}
        gPolyGeomMode_3EEC8_tmpl[1] = 0xAA002;
        gPolyGeomMode_3EEC8_tmpl[0] = 0;
        *(Node **)&ITEMTYPE(tmpl)->unk40 = (Node *)CALL(PolyShape_ctor)(0, &D_00000000 + 0x41A8, SHAPETMPL(tmpl)->model, 0); /* "ball" */
    }
    o = (Node *)CALL(Balloon_ctor)(0);
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
    return o;
}
