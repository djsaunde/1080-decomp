/* IDO 7.1 NCC (C++) EXACT donors for snoweffects.uso functions at 0x6000 and above (agent-e run 639),
 * REPLACE_FUNC_BODY-spliced over the placeholders in snoweffects_uso.c on both build paths
 * (Makefile SNOW_NCC_E_FUNCS). Classes: snoweffects_ncc_e.h. */
#include "snoweffects_ncc_e.h"

/* 0x8C60 (0x110): the third TU's trail draw, SnowRing_draw_000040B4's twin. Reset the display list, let
 * 0x4188 fill it, then append ClearGeometryMode(0x33000), SetGeometryMode(0x2205), SetOtherMode_L
 * (0xB900031D / 0x00404A50) and EndDL. The first append goes through two zero-code me() levels and the
 * unnamed-index nextr() (target ctx v0 / head v1 / count a0 / packet a1); the other three are the inline
 * put() emitter. Found by a per-site search over append spellings. */
extern "C" void snoweffects_uso_func_00008C60(SnowTrail *t) {
    CALL(gfx_resetDLContext)(t->dl);
    CALL(snoweffects_uso_func_00004188)(t, t->dl);
    { Gfx *g = me(me(t->dl))->nextr(); g->w0 = 0xB6000000; g->w1 = 0x00033000; }
    t->dl->put(0xB7000000, 0x00002205);
    t->dl->put(0xB900031D, 0x00404A50);
    t->dl->put(0xB8000000, 0);
}

/* 0x9FE8 (0x124): add one light's contribution to a colour. The light's position scaled component-wise
 * by a bootup Vec3 (bootup Bss+0x3C168 + 0x128) is normalized in a copy; its dot with the module's
 * direction vector (Bss+0x50) adds d * colour to *out when positive. Both Vec3 locals are declared up
 * front and n is assigned (target v at sp+0x44, n at sp+0x38); the scale vector is reached through a
 * pointer local and a reference to it (frame 0x50, its lui/addiu kept apart from the loads). */
extern "C" void snoweffects_uso_func_00009FE8(void *self, SnowLightObj *o, SnowVec3 *out) {
    SnowVec3 v;
    SnowVec3 n;
    v.x = o->pos.x;
    v.y = o->pos.y;
    v.z = o->pos.z;
    SnowVec3 *gp = (SnowVec3 *)(D_00000000 + 0x128);
    const SnowVec3 &g = *gp;
    v *= g;
    n = v;
    n.normalize();
    float d = dot(*(SnowVec3 *)(D_00000000 + 0x50), n);
    if (d > 0.0f) {
        out->x += d * o->colour.x;
        out->y += d * o->colour.y;
        out->z += d * o->colour.z;
    }
}
