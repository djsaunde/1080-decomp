#include "common.h"
#include "uso_call.h"
#include "gdl/node.h"  /* Node, NodeMsg, VT_SLOT */
#include "gdl/mnode.h" /* SNode (scale +0x70) */
#include "gdl/item.h"  /* ItemType */

extern int gl_func_00000000();
extern char D_00000000;
extern char gl_data_00000000;
extern int gPolyGeomMode_3EEC8_tmpl[2];

/* fire.uso: the fire course object (an SNode subclass with a Sprite and two
 * gfx colour parms) and its ItemType template. Compiler: IDO 7.1 cfe -O2 matches
 * the members below; the module is libgdl C++ (vtables at Data+0x228C / 0x231C,
 * cfront-style this==0 allocating ctors), written here as C.
 *
 * Relocs (scripts/uso-imports.py sites fire): every `&D_00000000` is a zero
 * placeholder; the target is in the TextReloc. Imports from bootup.uso:
 * Bss+0x3C168 = GfxLib, Bss+0x3C768 = the hostio object. */

/* fire_uso_func_00000000: FireTmpl ctor (0x68 bytes, cfront this==0 alloc). The
 * 0x48-byte SNodeItemType base is constructed inline (its own this==0 alloc,
 * ItemType_ctor(base, tag, 0) with the 4-byte tag by value, then the
 * SNodeItemType vptr, bootup Data+0x1EFC0); then the FireTmpl vptr (Data+0x228C),
 * gPolyGeomMode = {0, 10}, the Sprite (unk40, the node each new Fire attaches),
 * sprite +0xC4 = 1, and the prim (0x58) / env (0x48) colours as r/255.
 * Levers: the divisor is the local `d` (a literal / 255.0f is folded by cfe; the
 * target divides at run time) declared first for the frame (name copy at
 * sp+0x1C); the alpha is the int 255, a separate constant from d's 255.0f, so it
 * is reloaded (li.s 255) as the target does; the inline base ctor's call and vptr
 * store share one source line, and `if (1) {}` closes the base part (as1 order). */
typedef struct { s32 w; } FireTag;
extern char SNodeItemType_vtbl_fire;
#define FIRE_SETCOLOUR(c, r, g, b, a) { d = 255.0f; (c)[0] = (r) / d; (c)[1] = (g) / d; (c)[2] = (b) / d; (c)[3] = (a) / d; }
char *fire_uso_func_00000000(char *self, FireTag tag) {
    float d;
    if (self != 0 || (self = (char *)CALL(mem_new)(0x68)) != 0) {
        ItemType *base = (ItemType *)self;
        FireTag t = tag;
        if (base != 0 || (base = (ItemType *)CALL(mem_new)(0x48)) != 0) {
            CALL(ItemType_ctor)(base, t, 0); base->node.vptr = &SNodeItemType_vtbl_fire;
        }
        NODE(self)->vptr = &D_00000000;
        if (1) {}
        gPolyGeomMode_3EEC8_tmpl[1] = 10;
        gPolyGeomMode_3EEC8_tmpl[0] = 0;
        ((ItemType *)self)->unk40 = CALL(Sprite_ctor_0004B620)(0, &gl_data_00000000);
        *(s32 *)(((ItemType *)self)->unk40 + 0xC4) = 1;
        FIRE_SETCOLOUR((f32 *)(self + 0x58), 219.0f, 196.0f, 0.0f, 255);
        FIRE_SETCOLOUR((f32 *)(self + 0x48), 126.0f, 33.0f, 3.0f, 255);
    }
    return self;
}

int *Fire_ctor(char *self, int sprite);

/* FireTmpl_create: FireTmpl create: new Fire(tmpl->sprite), scale 1,
 * handleMsg (vtable slot 4) with tag 19, then attach the template's sprite node
 * under the new object (the SNodeItemType_newItem template, game_libs 3DD28).
 * Levers: the 12-byte NodeMsg plus one unused `volatile int pad` give frame 0x38
 * with the message at sp+0x2C; the code-free `if (o) {}` after the ctor call keeps
 * o off v0 (the target's `move v1,v0`, v0 reused for the vtable, empty jalr slot);
 * the second tmpl->sprite read is a fresh load.
 * The ctor jal field is 0 in the ROM (USO TextReloc), so it is called via CALL. */
Node *FireTmpl_create(char *tmpl) {
    NodeMsg msg;
    Node *o;
    Node *node;
    volatile int pad;

    node = *(Node **)(tmpl + 0x40);
    o = (Node *)CALL(Fire_ctor)(0, (int)node);
    if (o) {}
    SNODE(o)->scale = 1.0f;
    msg.tag = 19;
    VT_SLOT(o->vptr, 4).fn((char *)o + VT_SLOT(o->vptr, 4).delta, &msg);
    node = *(Node **)(tmpl + 0x40);
    CALL(list_append)(&o->children, node);
    if (node->parent != 0) {
        node->unk04 = 1;
    }
    node->parent = o;
    return o;
}

/* Fire_declColourParms: hostio colour parms for the env (0x48) and prim (0x58) colours. */
void FireTmpl_declColourParms(char *self) {
    CALL(gfx_declColourParm)(self + 0x48, &D_00000000 + 0x237C);
    CALL(gfx_declColourParm)(self + 0x58, &D_00000000 + 0x2380);
}

void FireTmpl_display(char *self) {
    CALL(gfx_setPrimColour)(&D_00000000, self + 0x58);
    CALL(gfx_setEnvColour)(&D_00000000, self + 0x48);
    CALL(Node_display)(self);
}

int *Fire_ctor(char *self, int sprite) {
    if (self != 0 || (self = (char *)CALL(mem_new)(0x94)) != 0) {
        CALL(SNode_ctor)(self);
        *(char **)(self + 0x28) = &D_00000000;
        *(short *)(self + 0x8C) = 0;
        *(float *)(self + 0x90) = 0.0f;
        *(int *)(self + 0x88) = sprite;
        *(float *)(self + 0x6C) = 6.0f;
        CALL(Node_setUnk24)(self, 2);
    }
    return (int *)self;
}

void Fire_display(char *self) {
    *(short *)(*(char **)(self + 0x88) + 0xC0) = *(short *)(self + 0x8C);
    *(float *)(*(char **)(self + 0x88) + 0xB0) = *(float *)(self + 0x70);
    *(float *)(*(char **)(self + 0x88) + 0xB4) = *(float *)(self + 0x70);
    *(short *)(self + 0x8C) = (*(short *)(self + 0x8C) + 1) & 7;
    CALL(SNode_display)(self);
}

extern void gl_func_00000000_ff(void *, char *, void *, float, float, int);
void Fire_hostioParms(char *self) {
    CALL(hostio_beginGroup)(&D_00000000, &D_00000000 + 0x2388, 0);
    XCALL(gl_func_00000000_ff, hostio_addFloat)(&D_00000000, &D_00000000 + 0x2390, self + 0x90, 0.0f, 1.0f, 0);
    CALL(hostio_endGroup)(&D_00000000);
    CALL(SNode_hostioParms)(self);
}
