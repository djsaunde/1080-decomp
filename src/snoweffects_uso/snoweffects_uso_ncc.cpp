/* IDO 7.1 NCC (C++) EXACT donors for snoweffects.uso (agent-h run 628), REPLACE_FUNC_BODY-spliced over the
 * INCLUDE_ASM placeholders in snoweffects_uso.c on both build paths. Classes: snoweffects_ncc.h. */
#include "snoweffects_ncc.h"
#include "../../include/uso_call.h"  /* NCC donors build without -I include */

/* 0x40B4 (0xD4): the emitter draw. Reset its display list, let 0x4188 fill it, then append
 * ClearGeometryMode(0x33000), SetGeometryMode(0x2204) and EndDL through the c->next() packets
 * (the gfx_initRDPState family, game_libs_ncc_59E80.cpp). */
extern "C" void SnowRing_draw_000040B4(SnowRing *t) {
    CALL(gfx_resetDLContext)(t->dl);
    CALL(SnowRing_setupDL_00004188)(t, t->dl);
    PKT(t->dl, 0xB6000000, 0x00033000);
    PKT(t->dl, 0xB7000000, 0x00002204);
    PKT(t->dl, 0xB8000000, 0);
}

/* 0x4188 (0x1EC): the emitter's display-list setup. PipeSync, SetOtherMode_H(0x1402: cycle type 0),
 * SetOtherMode_L(0x31D, 0x404240), SetCombine(0xFC409881, 0xFF33FFFF), an identity matrix (guMtxIdentF /
 * guMtxF2L into the module matrix) loaded through 0x01020040, then texture-filter / texture-LUT modes and
 * Texture(0x8000, 0x8000).
 * The ctx is used through a reference-bound copy (`ctx` / `dc`), not the parameter: uopt counts every
 * load of a variable whose value it cannot forward (a parameter, or a temp holding a memory read) toward
 * the ~20-load live unit, and a load of a copy it can forward is free. Each packet through the parameter
 * costs 2 more counted loads; with the copy, packet 3's buffer pointer lands in the first live unit and
 * takes a3 as in the target (agent-h run 687, instrumented uopt readnxtinst). The two locals also give
 * the 144-byte frame and the sp+44 spill slot. */
extern "C" void SnowRing_setupDL_00004188(SnowRing *t, DLCtx *c) {
    float m[4][4];
    DLCtx &ctx = *c;
    DLCtx *dc = &ctx;
    APKT(dc, 0xE7000000, 0);
    APKT(dc, 0xBA001402, 0);
    APKT(dc, 0xB900031D, 0x00404240);
    APKT(dc, 0xFC409881, 0xFF33FFFF);
    CALL(guMtxIdentF)(m);
    CALL(guMtxF2L)(m, D_00000000);
    APKT(dc, 0x01020040, CALL(gl_osVirtualToPhysical)(D_00000000));
    APKT(dc, 0xBA001001, 0);
    APKT(dc, 0xBA001301, 0x00080000);
    APKT(dc, 0xBB000001, 0x80008000);
}

/* 0x27A8 (0x138): SnowFall flake motion. Gravity down to a terminal speed, a sideways sway that
 * accelerates against the current x / z velocity, then integrate. The 0.0015f literal is the RoData+0xC
 * word (loop-hoisted lwc1, pinned as SnowFall_moveFlakes_000027A8_rodata); `pos += vel` must be the
 * user operator+= (the target computes z before y), which is why this is an NCC donor. */
extern "C" void SnowFall_moveFlakes_000027A8(SnowFall *t) {
    SnowParticle *p = t->items;
    int i;
    for (i = 0; i < t->count; i++, p++) {
        p->vel.y += p->gravity;
        if (p->vel.y < p->minVelY) {
            p->vel.y = p->minVelY;
        }
        if (p->vel.x < 0.0f) {
            p->accel.x += 0.0015f;
        }
        if (p->vel.x >= 0.0f) {
            p->accel.x -= 0.0015f;
        }
        if (p->vel.z < 0.0f) {
            p->accel.z += 0.0015f;
        }
        if (p->vel.z >= 0.0f) {
            p->accel.z -= 0.0015f;
        }
        p->vel.x += p->accel.x;
        p->vel.z += p->accel.z;
        p->pos += p->vel;
    }
}

/* 0xB44 (0x14C): SnowPool update. Age each particle and drop the dead ones (0x718 moves the last one into
 * the slot, so it is revisited), integrate, then grow / shrink the size once life falls below fadeAt and
 * fade the alpha; either reaching 0 kills the particle. NCC for the user `pos += vel` schedule. */
extern "C" void SnowPool_update_00000B44(SnowPool *t) {
    SnowParticle *p = t->items;
    int i;
    for (i = 0; i < t->count; i++, p++) {
        p->life -= p->decay;
        if (p->life <= 0.0f) {
            CALL(SnowPool_remove_00000718)(t, p);
            p--;
            i--;
            continue;
        }
        p->vel.y += p->gravity;
        p->pos += p->vel;
        if (p->life < p->fadeAt) {
            p->size += p->sizeVel;
            if (p->size < 0.0f) {
                p->size = 0.0f;
                p->life = 0.0f;
            }
        }
        p->alpha -= p->alphaVel;
        if (p->alpha < 0.0f) {
            p->alpha = 0.0f;
            p->life = 0.0f;
        }
    }
}

/* 0x39C4 (0x2E0): SnowFall fill. unk98 = camera position * world scale, then unk40 flakes from the pool
 * (0x6D0), each pushed back d along camRotT[2] and spread b / a along camRotT[1] / camRotT[0], with
 * random spin, fall speed and sway. The -0.015f / 0.15f / 0.15f literals are RoData+0x14..+0x1C (donor
 * .rodata 0..8, pinned as SnowFall_fill_000039C4_rodata). The `if (1) {}` keeps b's
 * (r - 0.5) * 60 right after its sys_randf call (else uopt sinks it to its first use); the gravity
 * store first and accel.y between accel.x and accel.z give the target's store schedule. */
extern "C" void SnowFall_fill_000039C4(SnowFall *t) {
    SnowParticle *p;
    int i;
    float d, a, b;
    SnowGfxLib *gl = (SnowGfxLib *)D_00000000;
    t->lastCam[0] = gl->curWindow->camera->pos[0] * gl->worldScale[0];
    t->lastCam[1] = gl->curWindow->camera->pos[1] * gl->worldScale[1];
    t->lastCam[2] = gl->curWindow->camera->pos[2] * gl->worldScale[2];
    for (i = 0; i < t->fillCount; i++) {
        p = (SnowParticle *)CALL(SnowPool_alloc_000006D0)(t);
        if (p != 0) {
            d = XCALL(gl_func_00000000_snow_randf, sys_randf)() * 100.0f;
            a = (XCALL(gl_func_00000000_snow_randf, sys_randf)() - 0.5f) * 60.0f;
            b = (XCALL(gl_func_00000000_snow_randf, sys_randf)() - 0.5f) * 60.0f;
            if (1) {}
            p->pos.x = t->lastCam[0] - t->camRotT[2][0] * d;
            p->pos.y = t->lastCam[1] - t->camRotT[2][1] * d;
            p->pos.z = t->lastCam[2] - t->camRotT[2][2] * d;
            p->pos.x = p->pos.x + t->camRotT[1][0] * b;
            p->pos.y = p->pos.y + t->camRotT[1][1] * b;
            p->pos.z = p->pos.z + t->camRotT[1][2] * b;
            p->pos.x = p->pos.x + t->camRotT[0][0] * a;
            p->pos.y = p->pos.y + t->camRotT[0][1] * a;
            p->pos.z = p->pos.z + t->camRotT[0][2] * a;
            p->gravity = -0.015f;
            p->size = 1.0f;
            p->sizeVel = 0.0f;
            p->fadeAt = 0.0f;
            p->aspect = 1.0f;
            p->minVelY = -1.0f;
            p->angle = XCALL(gl_func_00000000_snow_randf, sys_randf)() * 360.0f;
            p->vel.x = (XCALL(gl_func_00000000_snow_randf, sys_randf)() - 0.5f) * 0.0f;
            p->vel.y = -(XCALL(gl_func_00000000_snow_randf, sys_randf)() * 2.5f) - 1.0f;
            p->vel.z = (XCALL(gl_func_00000000_snow_randf, sys_randf)() - 0.5f) * 0.0f;
            p->accel.x = (XCALL(gl_func_00000000_snow_randf, sys_randf)() - 0.5f) * 0.15f;
            p->accel.y = 0.0f;
            p->accel.z = (XCALL(gl_func_00000000_snow_randf, sys_randf)() - 0.5f) * 0.15f;
            p->decay = 0.0f;
            p->alpha = 0.0f;
            p->alphaVel = 0.0f;
            p->col[3] = 0;
            p->col[2] = 251;
            p->col[1] = 251;
            p->col[0] = 251;
            p->life = 256.0f;
            t->count++;
        }
    }
}

/* 0x4E5C: the emitter's end points from its basis angle. ang = acos(-(dir . v108)), mirrored when the
 * camera's v318 . v3C8 is negative and again for camera flag 0x80; p = (-len cos, 0, -u40 u50 sin) and
 * q = (len cos, 0, u40 u50 sin) through the world matrix (+0x30); each is probed with 0x52D4 (hit distance
 * into d1 / d2, hit vector into a local) and, on a hit nearer than maxDist, pushed back along the hit
 * vector (hits++); uD4 = uC4. Frame (216): the point transform declares its results z, y, x (the z temp
 * the target spills sits right under the pool words), the push is a named local of its if-block with
 * operator* as a member and += by const reference (agent-e's 0x8D70 shape), and one dead scratch block of
 * four words after the first push fills the gap above the second probe's locals (agent-e run 659). */
struct SnowBV3 {
    float x, y, z;
    SnowBV3() {}
    void set(float a, float b, float c) { x = a; y = b; z = c; }
    SnowBV3 &operator=(SnowBV3 o) { x = o.x; y = o.y; z = o.z; return *this; }
    SnowBV3 &operator+=(const SnowBV3 &o) { x += o.x; y += o.y; z += o.z; return *this; }
    SnowBV3 operator*(float s) const { SnowBV3 r; r.set(x * s, y * s, z * s); return r; }
};
inline SnowBV3 operator-(const SnowBV3 &a, const SnowBV3 &b) {
    SnowBV3 r;
    r.set(a.x - b.x, a.y - b.y, a.z - b.z);
    { int sc[1]; sc[0] = 0; }  /* dead scratch word: puts the operator= param at the target's sp+60 */
    if (1) {}  /* ends the live unit of the subtraction loads (the result-temp address then takes v0) */
    return r;
}
inline float dot(const SnowBV3 &a, const SnowBV3 &b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
inline void xf(SnowBV3 *v, float (*m)[4]) {
    float nz, ny, nx;
    nx = m[0][0] * v->x + m[1][0] * v->y + m[2][0] * v->z + m[3][0];
    ny = m[0][1] * v->x + m[1][1] * v->y + m[2][1] * v->z + m[3][1];
    nz = m[0][2] * v->x + m[1][2] * v->y + m[2][2] * v->z + m[3][2];
    v->x = nx; v->y = ny; v->z = nz;
}
struct SnowBCam {
    char pad[0x318];
    SnowBV3 v318;
    char pad324[0x3B0 - 0x324];
    SnowBV3 up;      /* 3B0 */
    char pad3BC[0x3C8 - 0x3BC];
    SnowBV3 v3C8;
    char pad3D4[0xA58 - 0x3D4];
    int flags;       /* A58 */
};
struct SnowQuad { int a, b, c, d; };
struct SnowVEnt { short d; short pad; void (*fn)(void *); };
struct SnowBasis {
    char pad00[0x28];
    SnowVEnt *vptr;  /* 28 */
    SnowBCam *cam;   /* 2C */
    float (*m)[4];   /* 30: world matrix */
    char pad34[0x3C - 0x34];
    float endX;       /* 3C */
    float endZ;
    char pad44[0x50 - 0x44];
    float width;
    char pad54[0xB4 - 0x54];
    SnowQuad uB4;    /* B4: 0x52D4 out-param of the p probe */
    SnowQuad uC4;    /* C4: 0x52D4 out-param of the q probe */
    SnowQuad uD4;    /* D4: copy of uC4 */
    SnowBV3 endA;       /* E4: +len end point */
    SnowBV3 endB;       /* F0: -len end point */
    SnowBV3 across;  /* FC: normalized a - b */
    SnowBV3 velDir;    /* 108 */
    SnowBV3 perp;     /* 114 */
    SnowBV3 endP;       /* 120 */
    SnowBV3 endQ;       /* 12C */
    SnowBV3 origin;       /* 138: origin */
    int hits;        /* 144 */
    float cosA;      /* 148 */
    float hitDistP;        /* 14C */
    float hitDistQ;        /* 150 */
    char pad154[0x15C - 0x154];
    float maxDist;   /* 15C */
};
/* 0x4374: rebuild the emitter's basis from its world matrix (+0x30): the end points (+-len, 0, 0) go
 * through the matrix; dir = a - b (operator- through set(), by-value operator=), normalized and kept as
 * fwd, then crossed with the camera's up vector (+0x3B0) in place; v108 = camera v318, normalized; the
 * origin goes through the matrix; then virtual slots 14 and 15. Frame (160): the `if (1) {}` in operator-
 * puts the result-temp address in v0 right before the copy (as the target), and one dead two-word scratch
 * block after the first normalize gives the 8 bytes under the temps (run 671 placement search). */
extern "C" void SnowRing_updateBasis_00004374(SnowBasis *t) {
    t->endA.x = t->endX; t->endA.y = 0.0f; t->endA.z = 0.0f;
    xf(&t->endA, t->m);
    t->endB.x = -t->endX; t->endB.z = 0.0f; t->endB.y = 0.0f;
    xf(&t->endB, t->m);
    t->perp = t->endA - t->endB;
    CALL(math_vec3Normalize)(&t->perp);
    { int sc[2]; sc[0] = 0; }
    t->across = t->perp;
    CALL(math_vec3Cross)(&t->perp, &t->cam->up, &t->perp);
    t->velDir = t->cam->v318;
    CALL(math_vec3Normalize)(&t->velDir);
    t->origin.x = 0.0f; t->origin.y = 0.0f; t->origin.z = 0.0f;
    xf(&t->origin, t->m);
    t->vptr[14].fn((void *)((int)t->vptr[14].d + (int)t));
    t->vptr[15].fn((void *)((int)t->vptr[15].d + (int)t));
}
extern "C" float gl_func_00000000_snow_ff(float);  /* math_acosf / fcos / fsin: float(float) blank jal */
extern "C" float gl_func_00000000_snow_hitf(SnowBasis *, SnowBV3 *, SnowBV3 *, SnowQuad *);  /* 0x52D4 */
extern "C" void SnowRing_placeEnds_00004E5C(SnowBasis *t) {
    float ang;
    SnowBV3 hit;
    t->cosA = -(t->perp.x * t->velDir.x + t->perp.y * t->velDir.y + t->perp.z * t->velDir.z);
    ang = XCALL(gl_func_00000000_snow_ff, math_acosf)(t->cosA);
    if (dot(t->cam->v318, t->cam->v3C8) < 0.0f) {
        ang = -ang;
    }
    if (t->cam->flags & 0x80) {
        ang = -ang;
    }
    t->hits = 0;
    t->endP.x = XCALL(gl_func_00000000_snow_ff, fcos)(ang) * -t->endX;
    t->endP.y = 0.0f;
    t->endP.z = XCALL(gl_func_00000000_snow_ff, fsin)(ang) * (-t->endZ * t->width);
    xf(&t->endP, t->m);
    t->endQ.x = XCALL(gl_func_00000000_snow_ff, fcos)(ang) * t->endX;
    t->endQ.y = 0.0f;
    t->endQ.z = XCALL(gl_func_00000000_snow_ff, fsin)(ang) * (t->endZ * t->width);
    xf(&t->endQ, t->m);
    t->hitDistP = XCALL(gl_func_00000000_snow_hitf, SnowRing_probe_000052D4)(t, &t->endP, &hit, &t->uB4);
    if (t->hitDistP != 0.0f && t->hitDistP < t->maxDist) {
        SnowBV3 push = hit * -t->hitDistP;
        { int sc[4]; sc[0] = 0; }
        t->endP += push;
        t->hits++;
    }
    t->hitDistQ = XCALL(gl_func_00000000_snow_hitf, SnowRing_probe_000052D4)(t, &t->endQ, &hit, &t->uC4);
    if (t->hitDistQ != 0.0f && t->hitDistQ < t->maxDist) {
        SnowBV3 push = hit * -t->hitDistQ;
        t->endQ += push;
        t->hits++;
    }
    t->uD4 = t->uC4;
}
