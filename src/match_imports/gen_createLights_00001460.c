/* Native original-compiler source port from src/timproc_uso_b5/timproc_uso_b5.c. Target bytes unchanged. */
#include "context_timproc_uso_b5.h"
#include "gdl/light.h"

/* gen_createLights(room): the select room's three BaseLights (GenRoom, select.h):
 * ambient (white, flags |= 0x40 | 0x80, mask 1), local (colour 1, mask 2) and
 * ambient2 ((174, 113, 61) / 255, flags |= 0xC0, mask 4); each is appended to the
 * GfxLib light list and to the room's child list (+0x10), parent = room. The colour
 * stores (+0x124..+0x130) are the "col" parm value inside BaseLight.parms. */
#define ROOM ((GenRoom *)arg0)
#define LIGHT(p) ((BaseLight *)(p))

void gen_createLights_00001460(char *arg0) {
    volatile int pad[4];
    char *o, *reg;
    float white;
    float denom = 255.0f;
    float retA[1];
    reg = arg0 + 0x10;
    o = (char *)gen_imp_BaseLight_ctor(0, &timproc_uso_b5_D_807FE8FC + 0x108C);
    ROOM->ambient = o;
    white = (float)255 / denom;
    *(float *)(o + 0x124) = white;
    *(float *)(o + 0x128) = (float)255 / denom;
    *(float *)(o + 0x12C) = (float)255 / denom;
    *(float *)(o + 0x130) = (float)255 / denom;
    o = ROOM->ambient;
    o = (char *)&LIGHT(o)->flags; /* the target steps o itself to +0xC4 */
    *(int *)o |= 0x40;
    o = ROOM->ambient;
    o = (char *)&LIGHT(o)->flags; /* the target steps o itself to +0xC4 */
    *(int *)o |= 0x80;
    o = ROOM->ambient;
    LIGHT(o)->mask = 1;
    o = ROOM->ambient;
    retA[0] = white;
    gen_imp_gfx_listAppend_0004E214(&gGfxLib_8005C108, o);
    o = ROOM->ambient;
    gen_imp_list_append(reg, o);
    if (LIGHT(o)->node.parent != 0) {
        LIGHT(o)->node.unk04 = 1;
    }
    LIGHT(o)->node.parent = (s32)arg0;
    o = (char *)gen_imp_BaseLight_ctor(0, &timproc_uso_b5_D_807FE90C + 0x109C);
    ROOM->local = o;
    *(float *)(o + 0x124) = 1.0f;
    *(float *)(o + 0x128) = 1.0f;
    *(float *)(o + 0x12C) = 1.0f;
    *(float *)(o + 0x130) = 1.0f;
    o = ROOM->local;
    LIGHT(o)->mask = 2;
    gen_imp_gfx_listAppend_0004E214(&gGfxLib_8005C108, ROOM->local);
    o = ROOM->local;
    gen_imp_list_append(reg, o);
    if (LIGHT(o)->node.parent != 0) {
        LIGHT(o)->node.unk04 = 1;
    }
    LIGHT(o)->node.parent = (s32)arg0;
    o = (char *)gen_imp_BaseLight_ctor(0, &timproc_uso_b5_D_807FE918 + 0x10A8);
    ROOM->ambient2 = o;
    *(float *)(o + 0x124) = 174.0f / denom;
    *(float *)(o + 0x128) = 113.0f / denom;
    *(float *)(o + 0x12C) = 61.0f / denom;
    *(float *)(o + 0x130) = retA[0];
    o = ROOM->ambient2;
    o = (char *)&LIGHT(o)->flags; /* the target steps o itself to +0xC4 */
    *(int *)o |= 0x40;
    o = ROOM->ambient2;
    o = (char *)&LIGHT(o)->flags; /* the target steps o itself to +0xC4 */
    *(int *)o |= 0x80;
    o = ROOM->ambient2;
    LIGHT(o)->mask = 4;
    gen_imp_gfx_listAppend_0004E214(&gGfxLib_8005C108, ROOM->ambient2);
    o = ROOM->ambient2;
    gen_imp_list_append(reg, o);
    if (LIGHT(o)->node.parent != 0) {
        LIGHT(o)->node.unk04 = 1;
    }
    LIGHT(o)->node.parent = (s32)arg0;
}
