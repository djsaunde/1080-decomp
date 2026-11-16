/* IDO 7.1 NCC (C++) EXACT donors for snoweffects.uso (agent-h run 628), REPLACE_FUNC_BODY-spliced over the
 * INCLUDE_ASM placeholders in snoweffects_uso.c on both build paths. Classes: snoweffects_ncc.h. */
#include "snoweffects_ncc.h"

/* 0x40B4 (0xD4): the emitter draw. Reset its display list, let 0x4188 fill it, then append
 * ClearGeometryMode(0x33000), SetGeometryMode(0x2204) and EndDL through the c->next() packets
 * (the gfx_initRDPState family, game_libs_ncc_59E80.cpp). */
extern "C" void SnowRing_draw_000040B4(SnowRing *t) {
    CALL(gfx_resetDLContext)(t->dl);
    CALL(snoweffects_uso_func_00004188)(t, t->dl);
    PKT(t->dl, 0xB6000000, 0x00033000);
    PKT(t->dl, 0xB7000000, 0x00002204);
    PKT(t->dl, 0xB8000000, 0);
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
