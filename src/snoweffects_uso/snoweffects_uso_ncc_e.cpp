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

/* 0x9768 (0x11C): SnowTarget dirty check. When the drawn colour, the two float parms or the drawn
 * position differ from the live parm values and the position, set the dirty flag to 2 and copy them
 * over (SnowLink_update_00009F44's copy set). The position copy reads through a pointer local, which
 * gives the target's x, y, z load order (agent-g's heli 0x3F4 lever). */
extern "C" void snoweffects_uso_func_00009768(SnowTargetE *t) {
    if (t->colour.r != t->colourParm.get().r || t->colour.g != t->colourParm.get().g ||
        t->colour.b != t->colourParm.get().b || t->f98 != t->parm248.get() || t->f9C != t->parm260.get() ||
        t->pos.x != t->drawPos.x || t->pos.y != t->drawPos.y || t->pos.z != t->drawPos.z) {
        snowDirtyFlag_A30 = 2;
        t->colour = t->colourParm.get();
        t->f98 = t->parm248.get();
        t->f9C = t->parm260.get();
        SnowVec3 *p = &t->pos;
        t->drawPos.x = p->x;
        t->drawPos.y = p->y;
        t->drawPos.z = p->z;
    }
}

/* 0x9884 (0x364): rebuild a snow light's lit-vertex list. Restore the saved vertex colours and free the
 * old MorphPoly update list, then scale the light position by gGfxLib's scale (bootup Bss+0x3C168 +
 * 0x128), lift it by +0x270 and collect up to 64 mesh vertices with 0.1 < |pos - v| < radius: index
 * into +0xAC, falloff (1 - d / radius) * 255 into the alpha of +0x12C, then MorphPoly_genUpdateList.
 * Frame: the restore loop and the vertex loop have their own function-scope counters, declared with
 * the scale pointer before the two Vec3 locals, and the vertex local is declared before the position
 * (target position sp+0xB8 under the vertex sp+0xC4). The scale reference binds through the pointer
 * assignment (keeps the lui / addiu 0x128 pair). The 0.1f literal is RoData+0xF0
 * (snoweffects_uso_func_00009884_rodata). */
extern "C" void snoweffects_uso_func_00009884(SnowGlowE *t, SnowMesh *m) {
    unsigned char (*cols)[4] = m->cols;
    SnowLV *gp;
    int i;
    unsigned int j;
    if (t->updateList != 0) {
        for (i = 0; i < t->n; i++) {
            int idx = t->vtx[i];
            cols[idx][0] = t->col[i][0];
            cols[idx][1] = t->col[i][1];
            cols[idx][2] = t->col[i][2];
        }
        CALL(MorphPoly_applyUpdateList_0004A9F0)(m, m->cols, t->updateList);
        CALL(mem_delete)(t->updateList);
    }
    t->updateList = 0;
    t->n = 0;
    SnowLV vf;
    SnowLV p;
    p.x = t->pos.x;
    p.y = t->pos.y;
    p.z = t->pos.z;
    const SnowLV &g = *(gp = (SnowLV *)(D_00000000 + 0x128));
    p *= g;
    p.y += t->yofs;
    for (j = 0; j < m->nverts; j++) {
        short *v = (short *)CALL(PolyShape_getVertex)(m, j);
        vf.x = v[0];
        vf.y = v[1];
        vf.z = v[2];
        SnowLV dd = p - vf;
        float len = dd.length();
        if (0.1f < len && len < t->radius) {
            float f = 1.0f - len / t->radius;
            t->vtx[t->n] = j;
            t->col[t->n][3] = (unsigned int)(f * 255.0f);
            t->n++;
            if (t->n == 64) {
                j = m->nverts;
            }
        }
    }
    if (t->n > 0) {
        t->updateList = (void *)CALL(MorphPoly_genUpdateList)(m, t->n, t->vtx);
    }
}
