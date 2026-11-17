/* IDO 7.1 NCC (C++) EXACT donors for snoweffects.uso functions at 0x6000 and above (agent-e run 639),
 * REPLACE_FUNC_BODY-spliced over the placeholders in snoweffects_uso.c on both build paths
 * (Makefile SNOW_NCC_E_FUNCS). Classes: snoweffects_ncc_e.h. */
#include "snoweffects_ncc_e.h"

/* 0x8C60 (0x110): the third TU's trail draw, SnowRing_draw_000040B4's twin. Reset the display list, let
 * 0x4188 fill it, then append ClearGeometryMode(0x33000), SetGeometryMode(0x2205), SetOtherMode_L
 * (0xB900031D / 0x00404A50) and EndDL. The first append goes through two zero-code me() levels and the
 * unnamed-index nextr() (target ctx v0 / head v1 / count a0 / packet a1); the other three are the inline
 * put() emitter. Found by a per-site search over append spellings. */
extern "C" void BoardSpray_draw_00008C60(SnowTrail *t) {
    CALL(gfx_resetDLContext)(t->dl);
    CALL(SnowRing_setupDL_00004188)(t, t->dl);
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
extern "C" void MapLighter_addDiffuse_00009FE8(void *self, SnowLightObj *o, SnowVec3 *out) {
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
 * over (MapLighter_syncLight_00009F44's copy set). The position copy reads through a pointer local, which
 * gives the target's x, y, z load order (agent-g's heli 0x3F4 lever). */
extern "C" void LocalLight_checkDirty_00009768(SnowTargetE *t) {
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
 * (LocalLight_buildLitVerts_00009884_rodata). */
extern "C" void LocalLight_buildLitVerts_00009884(SnowGlowE *t, SnowMesh *m) {
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

/* 0x8D70 (0x2A0): trail point probe. Reset the hit count, +0x148 = -dot(+0x114, +0x108), copy the start
 * points (+0xE4 / +0xF0) to +0x120 / +0x12C, then probe each one through the CollShape test 0x52D4
 * (distance into +0x14C / +0x150, normal into n) and push it back by -d * n when 0 < d < +0x15C;
 * finally +0xD4 = +0xC4. The SV helpers are the libgdl Vec3 shapes (operator= by value, operator* through
 * set(), += by const reference); each push is a named local of its if-block.
 * Frame (target 168, read from -O0 -g2 .mdebug: n at vfp-12, push / r at -56 / -72 and -116 / -128): the
 * two dead scratch stores in blocks of their own are code-free frame words (agent-c run 643 lever) that
 * put each if-block's locals at the target's offsets (edg orders a block's words before its nested
 * blocks, agent-a run 648). */
extern "C" float gl_func_00000000_snow_52d4(void *self, void *pos, void *out, void *a3);
struct Quad4 { int a, b, c, d; };
struct SV {
    float x, y, z;
    SV() {}
    SV(float a, float b, float c) { x = a; y = b; z = c; }
    SV &operator=(SV o) { x = o.x; y = o.y; z = o.z; return *this; }
    void set(float a, float b, float c) { x = a; y = b; z = c; }
    SV operator*(float s) const { SV r; r.set(x * s, y * s, z * s); return r; }
    SV &operator+=(const SV &v) { x += v.x; y += v.y; z += v.z; return *this; }
};
struct SnowProbe {
    char pad00[0xB4];
    /* 0xB4 */ Quad4 qB4;
    /* 0xC4 */ Quad4 qC4;
    /* 0xD4 */ Quad4 qD4;
    /* 0xE4 */ SV startA;
    /* 0xF0 */ SV startB;
    /* 0xFC */ char padFC[0x108 - 0xFC];
    /* 0x108 */ SV n;
    /* 0x114 */ SV p;
    /* 0x120 */ SV a;
    /* 0x12C */ SV b;
    char pad138[0x144 - 0x138];
    /* 0x144 */ int hits;
    /* 0x148 */ float dist;
    /* 0x14C */ float da;
    /* 0x150 */ float db;
    char pad154[0x15C - 0x154];
    /* 0x15C */ float maxDist;
};
inline float dot(const SV &a, const SV &b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
extern "C" void BoardSpray_placeEnds_00008D70(SnowProbe *t) {
    SV n;
    t->hits = 0;
    t->dist = -dot(t->p, t->n);
    t->a = t->startA;
    t->b = t->startB;
    t->da = XCALL(gl_func_00000000_snow_52d4, SnowRing_probe_000052D4)(t, &t->a, &n, &t->qB4);
    { int sc[2]; sc[0] = 0; }
    if (t->da != 0.0f && t->da < t->maxDist) {
        SV push = n * -t->da;
        t->a += push;
        t->hits++;
    }
    { int sc2[3]; sc2[0] = 0; }
    t->db = XCALL(gl_func_00000000_snow_52d4, SnowRing_probe_000052D4)(t, &t->b, &n, &t->qC4);
    if (t->db != 0.0f && t->db < t->maxDist) {
        SV push = n * -t->db;
        t->b += push;
        t->hits++;
    }
    t->qD4 = t->qC4;
}

/* 0x7960: trail head update. +0x148 = -dot(+0x114, +0x108); ang = acosf of it, negated when the camera's
 * +0x318 / +0x3C8 dot is negative; +0x144 = 0; when |speed| (+0x4C) >= RoData+0xB4, probe +0x138 through the
 * CollShape test 0x52D4 and on 0 < d < +0x15C push it back by n * -d, +0x144 = 2 and rebuild the side
 * points +0x120 / +0x12C = (cos * +-w, 0, sin * +-h) through the +0x30 matrix.
 * Levers (agent-e runs 653-673): float ang declared before SnowV n; a named push in the inner block; the two
 * scratch blocks fill the target's frame words (176). The zero / |speed| colour (target zero f12, |speed|
 * f2) comes from the +0x148 store being its own statement: `ang = acosf(t->dist = -dot)` makes edg share
 * one temporary (M-28) between the acosf argument and the ?: result, so |speed| inherits the argument's
 * f12 preference (uopt -zdbug:5 trace, run 673). uopt forwards the stored value, so no reload appears. */
extern "C" float gl_func_00000000_snow_ff(float);  /* math_acosf / fcos / fsin: float(float) blank jal */
extern "C" float snow_ro_B4;  /* RoData+0xB4: the minimum speed */
/* the libgdl Vec3 as the 0x7960 body uses it (value temporaries, inline helpers) */
struct SnowV {
    float x, y, z;
    SnowV() {}
    SnowV(float a, float b, float c) { x = a; y = b; z = c; }
    SnowV operator*(float s) const { return SnowV(x * s, y * s, z * s); }
    SnowV &operator+=(const SnowV &v) { x += v.x; y += v.y; z += v.z; return *this; }
};
inline float dotv(const SnowV &a, const SnowV &b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
struct M44 { float m[4][4]; };
inline void xform(SnowV &v, const M44 &m) { float x = m.m[0][0] * v.x + m.m[1][0] * v.y + m.m[2][0] * v.z + m.m[3][0]; float y = m.m[0][1] * v.x + m.m[1][1] * v.y + m.m[2][1] * v.z + m.m[3][1]; float z = m.m[0][2] * v.x + m.m[1][2] * v.y + m.m[2][2] * v.z + m.m[3][2]; v.x = x; v.y = y; v.z = z; }
struct SnowCam { char pad[0x318]; SnowV fwd; char pad324[0x3C8 - 0x324]; SnowV dir; };
struct SnowBody7 {
    char pad00[0x2C];
    /* 0x2C */ SnowCam *cam;
    /* 0x30 */ M44 *mtx;
    char pad34[0x3C - 0x34];
    /* 0x3C */ float w;
    /* 0x40 */ float h;
    char pad44[0x4C - 0x44];
    /* 0x4C */ float speed;
    char pad50[0xD4 - 0x50];
    /* 0xD4 */ int qD4[4];
    char padE4[0x108 - 0xE4];
    /* 0x108 */ SnowV n;
    /* 0x114 */ SnowV p;
    /* 0x120 */ SnowV a;
    /* 0x12C */ SnowV b;
    /* 0x138 */ SnowV pos;
    /* 0x144 */ int hits;
    /* 0x148 */ float dist;
    char pad14C[0x15C - 0x14C];
    /* 0x15C */ float maxDist;
};
extern "C" void HandTrail_placeEnds_00007960(SnowBody7 *t) {
    float ang;
    SnowV n;
    t->dist = -dotv(t->p, t->n);
    ang = XCALL(gl_func_00000000_snow_ff, math_acosf)(t->dist);
    if (dotv(t->cam->fwd, t->cam->dir) < 0.0f) {
        ang = -ang;
    }
    t->hits = 0;
    if (!((t->speed < 0.0f ? -t->speed : t->speed) < snow_ro_B4)) {
        float d = XCALL(gl_func_00000000_snow_52d4, SnowRing_probe_000052D4)(t, &t->pos, &n, t->qD4);
        if (d != 0.0f && d < t->maxDist) {
            SnowV push = n * -d;
            t->pos += push;
            t->hits = 2;
            { int sc[2]; sc[0] = 0; }
            t->a.x = XCALL(gl_func_00000000_snow_ff, fcos)(ang) * t->w;
            t->a.y = 0.0f;
            t->a.z = XCALL(gl_func_00000000_snow_ff, fsin)(ang) * t->h;
            xform(t->a, *t->mtx);
            t->b.x = XCALL(gl_func_00000000_snow_ff, fcos)(ang) * -t->w;
            t->b.y = 0.0f;
            t->b.z = XCALL(gl_func_00000000_snow_ff, fsin)(ang) * -t->h;
            xform(t->b, *t->mtx);
            { int sc2[2]; sc2[0] = 0; }
        }
    }
}

