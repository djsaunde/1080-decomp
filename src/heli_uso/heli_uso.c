#include "common.h"
#include "uso_call.h"
#include "gdl/node.h"
#include "gdl/mnode.h"
#include "gdl/item.h"

extern int gl_func_00000000();
extern char D_00000000;

/* heli.uso: the helicopter course object. IDO 7.1 cfe -O2 C (libgdl C++ classes, cfront
 * `this == 0` allocating ctors, vptr at +0x28), like the other course-object modules.
 *
 * Classes (Data strings "heli", "helib", "<QNode>" x4, "hem", "mrk"):
 *  - HeliItemType (0x54, vtable Data+0xCAF0): an ItemType that loads the helicopter's models
 *    once ("heli" body PolyShape +0x48, "helib" rotor blade PolyShape +0x4C, a Sprite +0x50)
 *    and builds the Heli (0x274).
 *  - Heli (0x174, vtable Data+0xCA88): a QNode. Children: the body shape, two HeliRotor
 *    QNodes (main rotor: pitched 90 deg, at y = 480, spinning about y; tail rotor: yawed
 *    90 deg, spinning about x) that share the blade shape, and a plain QNode holding the
 *    sprite. +0x108 Vec3 start position (loadData), path / flight state from +0x114.
 *  - HeliRotor (0x118, vtable Data+0xCA20): a QNode; +0x108 spin speed, +0x10C Vec3 axis.
 *  - two 0x40 ColourItem creates (0x140 module vtable, 0x1ACC bootup ColourItem vtable):
 *    the "hem" / "mrk" marker items (rgb = position, +0x3C an int id).
 * Module-local zero symbols: heli_data_00000000 (Data base), heli_rodata_00000000 (RoData
 * base, float constants); per-site bootup aliases in undefined_syms_auto.txt. */
extern char heli_data_00000000;
extern char heli_rodata_00000000;
#define HELI_RODATA_F(off) (*(float *)(&heli_rodata_00000000 + (off)))

/* typed blank (0) aliases for imports that take floats */
extern void *gl_func_00000000f3(void *, char *, float, float, float); /* QNode_ctor */
extern void gl_func_00000000_heli_ppf(void *, void *, float);         /* math_quatRotateAxisAngle */
extern int gl_func_00000000_heli_cull(void *, int, float, float, float, int, float); /* cam_isSphereInView */

/* the bootup globals heli touches, one alias per access site (no shared lui / addiu base) */
typedef struct HeliGeomMode { int mode0; int geomMode; } HeliGeomMode;
extern int heli_polyFlags_ld, heli_polyFlags_st;   /* gPolyFlags_3EEC4 */
extern HeliGeomMode heli_polyGeomMode;             /* gPolyGeomMode_3EEC8 */
extern int heli_mode_1EBD8;
extern char heli_model_1C1C, heli_model_B984;      /* module Data: PolyShape model data */
extern char heli_gGfxLib_a, heli_gGfxLib_b, heli_gGfxLib_c;

/* the 4-byte ItemType name tag, passed by value (lands in a1 and the a1 home slot) */
typedef struct HeliTypeTag { int v; } HeliTypeTag;

typedef struct HeliColour { float r, g, b, a; } HeliColour;
typedef struct HeliVec3f { float x, y, z; } HeliVec3f;
char *heli_uso_func_00000388(char *self);

char *heli_uso_func_000004C0(char *self, void *body, void *blades, void *sprite);
void heli_uso_func_000008A8(char *self);

/* Vec3 assignment as the C++ Vec3 operator= inlines it: a word copy into a stack temp,
 * then three float stores (dst / src are float addresses, t a HeliTri3i temp) */
#define HELI_VEC3_SET(dst, src, t) \
    (t = *(HeliTri3i *)(src), ((float *)(dst))[0] = *(float *)&t.a, \
     ((float *)(dst))[1] = *(float *)&t.b, ((float *)(dst))[2] = *(float *)&t.c)

typedef struct { int a, b, c; } HeliTri3i;
typedef struct { int a, b, c, d; } HeliQuad4;

/* heli_uso_func_00000000: read an int from the gdl file (cross-USO template, as checkpoint
 * checkpoint_gdlReadInt_00000034 / game_uso stream_readS32_00000280). */
void heli_uso_func_00000000(int *dst) {
    int buf[2];
    CALL(fs_gdlReadFile)(&D_00000000, buf, 4);
    *dst = buf[0];
}

/* heli_uso_func_0000003C: read a Vec3 from the gdl file (template, as balloon_gdlReadVec3_00000094 /
 * game_uso stream_readVec3_000001D4). */
void heli_uso_func_0000003C(float *dst) {
    int pad_top[1];
    HeliTri3i raw;
    int pad_mid[2];
    HeliTri3i tmp;
    int pad_bot[2];
    CALL(fs_gdlReadFile)(&D_00000000, &raw, 12);
    tmp = raw;
    dst[0] = *(float *)&tmp.a;
    dst[1] = *(float *)&tmp.b;
    dst[2] = *(float *)&tmp.c;
}

/* heli_uso_func_000000AC: read a float from the gdl file (template, as balloon_gdlReadFloat_00000104). */
void heli_uso_func_000000AC(float *dst) {
    float buf[2];
    CALL(fs_gdlReadFile)(&D_00000000, buf, 4);
    *dst = buf[0];
}

/* heli_uso_func_000000E8: read a 16-byte quad from the gdl file (template, as snoweffects
 * fs_gdlReadQuad_snow_00000144). */
void heli_uso_func_000000E8(HeliQuad4 *dst) {
    HeliQuad4 buf;
    CALL(fs_gdlReadFile)(&D_00000000, &buf, 16);
    *dst = buf;
}

/* heli_uso_func_00000140: ItemType create: a new 0x40 marker item (ColourItem, module vtable
 * Data+0x18, +0x3C id = 0), appended to the type's child list (+0x40). */
void *heli_uso_func_00000140(char *type) {
    char *q;
    char *p;
    p = (char *)CALL(mem_new)(0x40);
    if (p != 0) {
        CALL(ColourItem_ctor)(p);
        NODE(p)->vptr = &heli_data_00000000 + 0x18;
        *(int *)(p + 0x3C) = 0;
    }
    q = p;
    p = *(char **)(type + 0x40);
    if (*(char **)(type + 0x40) != 0) {
        CALL(list_append)(q + 0x10, p);
        if (NODE(p)->parent != 0) {
            NODE(p)->unk04 = 1;
        }
        NODE(p)->parent = (Node *)q;
    }
    return q;
}

/* heli_uso_func_000001D4: HeliItemType ctor (0x54; the ItemType base ctor is inlined with its
 * own this == 0 allocation, as bootup item_createBal). */
void *heli_uso_func_000001D4(char *self, HeliTypeTag name) {
    char *base;
    volatile int pad;
    HeliTypeTag tag;
    if (self != 0 || (self = (char *)CALL(mem_new)(0x54)) != 0) {
        tag = name;
        base = self;
        if (base != 0 || (base = (char *)CALL(mem_new)(0x48)) != 0) {
            CALL(ItemType_ctor)(base, tag, 0); NODE(base)->vptr = (char *)&D_00000000; /* bootup ItemType vtable Data+0x1F050 */
        }
        NODE(self)->vptr = (char *)&D_00000000; /* HeliItemType vtable (Data+0xCAF0) */
        *(void **)(self + 0x4C) = 0;
        *(void **)(self + 0x48) = 0;
    }
    return self;
}

/* heli_uso_func_00000274: HeliItemType item create: load the shapes on first use (body "heli"
 * with poly flag 8 and geometry mode 0x2A003, blades "helib" with geometry mode 11, the
 * sprite), then build the Heli. */
void heli_uso_func_00000274(char *self) {
    if (*(void **)(self + 0x48) == 0) {
        heli_polyFlags_st = heli_polyFlags_ld | 8; if (1) {}
        heli_polyGeomMode.geomMode = 0x2A003;
        heli_polyGeomMode.mode0 = 0;
        *(void **)(self + 0x40) = *(void **)(self + 0x48) =
            (void *)CALL(PolyShape_ctor)(0, &heli_data_00000000 + 0xCB78, &heli_model_1C1C, 0); /* "heli" */
        heli_polyFlags_st = heli_polyFlags_ld & ~8;
        heli_polyGeomMode.geomMode = 11;
        heli_polyGeomMode.mode0 = 0;
        *(void **)(self + 0x4C) = (void *)CALL(PolyShape_ctor)(0, &heli_data_00000000 + 0xCB80, &heli_model_B984, 0); /* "helib" */
        *(char **)(self + 0x50) = (char *)CALL(Sprite_ctor_0004B620)(0, &heli_data_00000000 + 0xCA10);
        *(int *)(*(char **)(self + 0x50) + 0xC4) = 1;
        *(float *)(*(char **)(self + 0x50) + 0xC8) = -200.0f;
    }
    heli_mode_1EBD8 = 6;
    {
        void *blades = *(void **)(self + 0x4C);
        void *body = *(void **)(self + 0x48);
        void *sprite = *(void **)(self + 0x50);
        CALL(heli_uso_func_000004C0)(0, body, blades, sprite);
    }
}

/* heli_uso_func_00000388: HeliRotor ctor (0x118, "<QNode>"; spin speed +0x108 from RoData). */
char *heli_uso_func_00000388(char *self) {
    if (self != 0 || (self = (char *)CALL(mem_new)(0x118)) != 0) {
        XCALL(gl_func_00000000f3, QNode_ctor)(self, &heli_data_00000000 + 0xCB88, 0.0f, 0.0f, 0.0f);
        NODE(self)->vptr = (char *)&D_00000000; /* HeliRotor vtable (Data+0xCA20) */
        *(float *)(self + 0x108) = HELI_RODATA_F(0x50);
    }
    return self;
}

/* heli_uso_func_000003F4: HeliRotor update: spin the local quaternion about the axis
 * (+0x10C) by speed / RoData+0x54, then QNode_update. The axis is read through a Vec3
 * pointer temp into x / y / z (colours x f0, z f12; loads and stores run z, y, x); direct
 * member reads colour the first-loaded value f0. */
void heli_uso_func_000003F4(char *self) {
    int pad_top[5];
    HeliVec3f axis;
    int pad_bot[1];
    HeliVec3f *a = (HeliVec3f *)(self + 0x10C);
    float x = a->x, y = a->y, z = a->z;
    float angle = *(float *)(self + 0x108) / HELI_RODATA_F(0x54);
    axis.z = z;
    axis.y = y;
    axis.x = x;
    XCALL(gl_func_00000000_heli_ppf, math_quatRotateAxisAngle)(self + 0xCC, &axis, angle);
    CALL(QNode_update)(self);
}

/* heli_uso_func_00000458: empty virtual (one-arg no-op slot). */
void heli_uso_func_00000458(int a0) {
    (void)&a0;
}

/* heli_uso_func_00000460: Heli default ctor (0x174, "<QNode>", no children). */
char *heli_uso_func_00000460(char *self) {
    if (self != 0 || (self = (char *)CALL(mem_new)(0x174)) != 0) {
        XCALL(gl_func_00000000f3, QNode_ctor)(self, &heli_data_00000000 + 0xCB90, 0.0f, 0.0f, 0.0f);
        NODE(self)->vptr = (char *)&D_00000000; /* Heli vtable (Data+0xCA88) */
    }
    return self;
}

/* heli_uso_func_000004C0: Heli ctor (0x174, "<QNode>"): attach the body shape, two HeliRotor
 * children sharing the blade shape (main: pitched 90 deg, y = 480, axis y; tail: yawed 90 deg,
 * scale / trans from RoData, axis x) and a QNode carrying the sprite, then reset the flight
 * state (0x8A8). NM ~82%: the rotor-field zero stores need their own constant web (the
 * `(float)0` int-cast literal gives it) but land in f2 where the target uses f0; the
 * RoData loads need one alias per site (heli_ro_*). */
#ifdef NON_MATCHING
extern char heli_ro_58, heli_ro_5C, heli_ro_60, heli_ro_64, heli_ro_68, heli_ro_6C;
extern void gl_func_00000000_heli_euler(void *, float, float, float); /* math_eulerToQuat */
#define HELI_RO_SITE(off) (*(float *)(&heli_ro_##off + 0x##off))
char *heli_uso_func_000004C0(char *self, void *body, void *blades, void *sprite) {
    char *mainRotor;
    char *tailRotor;
    char *n;
    if (self != 0 || (self = (char *)gl_func_00000000 /* mem_new */(0x174)) != 0) {
        gl_func_00000000f3 /* QNode_ctor */(self, &heli_data_00000000 + 0xCB98, 0.0f, 0.0f, 0.0f);
        NODE(self)->vptr = (char *)&D_00000000; /* Heli vtable (Data+0xCA88) */
        gl_func_00000000 /* MNode_addChild */(self, body);
        mainRotor = (char *)gl_func_00000000 /* heli_uso_func_00000388 */(0);
        gl_func_00000000 /* MNode_addChild */(self, mainRotor);
        tailRotor = (char *)gl_func_00000000 /* heli_uso_func_00000388 */(0);
        gl_func_00000000 /* MNode_addChild */(self, tailRotor);
        gl_func_00000000 /* MNode_addChild */(mainRotor, blades);
        gl_func_00000000 /* MNode_addChild */(tailRotor, blades);
        gl_func_00000000_heli_euler /* math_eulerToQuat */(mainRotor + 0xCC, 90.0f, 0.0f, 0.0f);
        *(float *)(mainRotor + 0xB4) = (float)0;
        *(float *)(mainRotor + 0xB8) = 480.0f;
        *(float *)(mainRotor + 0xBC) = (float)0;
        *(float *)(mainRotor + 0x10C) = (float)0;
        *(float *)(mainRotor + 0x110) = 1.0f;
        *(float *)(mainRotor + 0x114) = (float)0;
        gl_func_00000000_heli_euler /* math_eulerToQuat */(tailRotor + 0xCC, 0.0f, 90.0f, 0.0f);
        *(float *)(tailRotor + 0xC0) = *(float *)(tailRotor + 0xC4) = *(float *)(tailRotor + 0xC8) = HELI_RO_SITE(58);
        *(float *)(tailRotor + 0xB4) = HELI_RO_SITE(5C);
        *(float *)(tailRotor + 0xB8) = HELI_RO_SITE(60);
        *(float *)(tailRotor + 0xBC) = HELI_RO_SITE(64);
        *(float *)(tailRotor + 0x10C) = 1.0f;
        *(float *)(tailRotor + 0x110) = (float)0;
        *(float *)(tailRotor + 0x114) = (float)0;
        n = (char *)gl_func_00000000f3 /* QNode_ctor */(0, &heli_data_00000000 + 0xCBA0, 0.0f, 0.0f, 0.0f);
        gl_func_00000000 /* MNode_addChild */(self, n);
        *(float *)(n + 0xB4) = 0.0f;
        *(float *)(n + 0xB8) = HELI_RO_SITE(68);
        *(float *)(n + 0xBC) = HELI_RO_SITE(6C);
        gl_func_00000000 /* MNode_addChild */(n, sprite);
        gl_func_00000000 /* heli_uso_func_000008A8 */(self);
    }
    return self;
}
#else
INCLUDE_ASM("asm/nonmatchings/heli_uso/heli_uso", heli_uso_func_000004C0);
#endif

/* heli_uso_func_0000069C: Heli loadData: the start position (+0x108), then the local pose
 * (vqs.trans = start raised by RoData+0x70) is copied into the combined pose (cvqs). */
void heli_uso_func_0000069C(char *self) {
    int pad_top[2];
    HeliTri3i t1;
    int pad_mid[3];
    HeliTri3i t2;
    int pad_bot[4];
    heli_uso_func_0000003C((float *)(self + 0x108));
    HELI_VEC3_SET(self + 0xB4, self + 0x108, t1);
    *(float *)(self + 0xB8) += HELI_RODATA_F(0x70);
    HELI_VEC3_SET(self + 0xDC, self + 0xB4, t1);  /* cvqs.trans = vqs.trans */
    HELI_VEC3_SET(self + 0xE8, self + 0xC0, t2);  /* cvqs.scale = vqs.scale */
    HELI_VEC3_SET(self + 0xF4, self + 0xCC, t2);  /* cvqs.quat = vqs.quat */
    *(float *)(self + 0x100) = *(float *)(self + 0xD8);
}

/* heli_uso_func_000007B4: Heli handleMsg. 9 (reset): put the Heli back at its start position
 * (combined and local trans) and reset the flight state; 7 (store): the start position takes
 * the current local trans. The compare chain is `beq 9; beql 7; b out`: an if-goto chain. */
void heli_uso_func_000007B4(char *self, int *msg) {
    int pad_top[2];
    HeliTri3i t1;
    int pad_mid[3];
    HeliTri3i t2;
    int pad_bot[2];
    CALL(QNode_handleMsg)(self, msg);
    if (*msg == 9) goto reset;
    if (*msg != 7) goto out;
    goto store;
reset:
    HELI_VEC3_SET(self + 0xDC, self + 0x108, t1);
    HELI_VEC3_SET(self + 0xB4, self + 0xDC, t2);
    CALL(heli_uso_func_000008A8)(self); /* global: blank reloc'd jal */
    goto out;
store:
    HELI_VEC3_SET(self + 0x108, self + 0xB4, t1);
out:;
}

/* heli_uso_func_000008A8: Heli flight-state reset (+0x114 Vec3 zeroed and copied to +0x12C,
 * +0x120 = (0, 400, 2000), tuning constants +0x138..+0x164, int state +0x168..+0x170).
 * EXACT via the NCC (C++) donor src/heli_uso/heli_uso_ncc.cpp (REPLACE_FUNC_BODY). */
void heli_uso_func_000008A8(char *self) {
}

/* heli_uso_func_00000980: place the Heli at its "hem" marker: look the "hem" ItemType up in
 * the ItemMgr (bootup Data+0x776C) and walk its items (cursor / next iterator, as game_libs
 * hud_findMarkers_000029F8); a marker with id (+0x3C) 8 gives the combined position
 * (cvqs.trans = marker rgb +0x30, raised by RoData+0x78).
 * The loop is written rotated (do-while, dy in the preheader) so the RoData+0x78 load hoists
 * as in the target. The first iterator result goes through its own local r (v0, copied into
 * item's v1). Frame 0x58: every declared local has a home in declaration order (the nested
 * block's dy / r last), so type / item above t put the Vec3 temp at sp+0x24. */
typedef struct HeliIterNode {
    char *data;
    struct HeliIterNode *next;
} HeliIterNode;
#define HELI_ITER_NEXT(it) \
    (((it)[0] = (it)[1], (it)[1] != 0) ? ((it)[1] = (it)[1]->next, (it)[0]->data) : 0)
extern char *heli_itemMgr;
void heli_uso_func_00000980(char *self) {
    HeliTypeTag key;
    char padA[0x10];
    HeliIterNode *it[2];
    char padB[4];
    char *type;
    char *item;
    HeliTri3i t;
    key.v = *(int *)(&heli_data_00000000 + 0xCBA8); /* "hem" */
    type = (char *)CALL(ItemMgr_findType_0003EA98)(heli_itemMgr, key);
    if (type != 0) {
        float dy;
        char *r;
        it[1] = *(HeliIterNode **)(type + 0x10);
        r = HELI_ITER_NEXT(it);
        if (r != 0) {
            item = r;
            dy = HELI_RODATA_F(0x78);
            do {
                if (*(int *)(item + 0x3C) == 8) {
                    HELI_VEC3_SET(self + 0xDC, item + 0x30, t);
                    *(float *)(self + 0xE0) += dy;
                }
            } while ((item = HELI_ITER_NEXT(it)) != 0);
        }
    }
}

/* heli_uso_func_00000A70: Heli display: when the combined position (cvqs.trans) is in view,
 * update the channel-10 sound emitter there, set a white prim colour, draw the QNode. */
void heli_uso_func_00000A70(char *self) {
    HeliColour col;
    int pad[2];
    if (XCALL(gl_func_00000000_heli_cull, cam_isSphereInView)(*(void **)(&heli_gGfxLib_a + 0x254), 3,
            *(float *)(self + 0xDC), *(float *)(self + 0xE0), *(float *)(self + 0xE4), 0, 0.0f)) {
        void *win = *(void **)(&heli_gGfxLib_b + 0x254); /* GfxLib current ScreenWindow */
        CALL(snd_updateChannel10Emitter)(1, self + 0xDC, win);
        col.r = col.g = col.b = col.a = 1.0f;
        CALL(gfx_setPrimColour)(&heli_gGfxLib_c, &col);
        CALL(QNode_display)(self);
    }
}

INCLUDE_ASM("asm/nonmatchings/heli_uso/heli_uso", heli_uso_func_00000B08);

/* heli_uso_func_000019C4: empty virtual. */
void heli_uso_func_000019C4(int a0) {
    (void)&a0;
}

/* heli_uso_func_000019CC: getItemPos stub returning 0 (as game_libs ItemType_getItemPos). */
int heli_uso_func_000019CC(int a0, int a1, int a2) {
    (void)&a0;
    (void)&a1;
    (void)&a2;
    return 0;
}

/* heli_uso_func_000019E0: the colour item's load: the int at +0x3C, then ColourItem_load. */
void heli_uso_func_000019E0(char *self) {
    heli_uso_func_00000000((int *)(self + 0x3C));
    CALL(ColourItem_load)(self);
}

/* heli_uso_func_00001A0C..1A9C: Parm read methods (tag into a local, value at +0x10), as
 * eddproc edd_readIntParm_0000032C / edd_readFloatParm_0000035C. */
void heli_uso_func_00001A0C(char *dst) {
    int tag;
    heli_uso_func_00000000(&tag);
    heli_uso_func_000000AC((float *)(dst + 0x10));
}

void heli_uso_func_00001A3C(char *dst) {
    int tag;
    heli_uso_func_00000000(&tag);
    heli_uso_func_0000003C((float *)(dst + 0x10));
}

void heli_uso_func_00001A6C(char *dst) {
    int tag;
    heli_uso_func_00000000(&tag);
    heli_uso_func_000000E8((HeliQuad4 *)(dst + 0x10));
}

void heli_uso_func_00001A9C(char *dst) {
    int tag;
    heli_uso_func_00000000(&tag);
    heli_uso_func_00000000((int *)(dst + 0x10));
}

/* heli_uso_func_00001ACC: ItemType create: a new 0x40 colour item (ColourItem + the int at
 * +0x3C), linked into the type's list (twin of snoweffects_uso_func_00007D8C). */
void *heli_uso_func_00001ACC(int *arg0) {
    int *p2;
    int *head;
    int *p1;
    p1 = (int *)CALL(mem_new)(0x40);
    if (p1 != 0) {
        CALL(ColourItem_ctor)(p1);
        NODE(p1)->vptr = (char *)&D_00000000; /* bootup ColourItem vtable (Data+0x75A0) */
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

