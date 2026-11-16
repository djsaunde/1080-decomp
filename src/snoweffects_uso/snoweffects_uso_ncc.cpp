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

/* 0x39C4 (0x2E0): SnowFall fill. unk98 = camera position * world scale, then unk40 flakes from the pool
 * (0x6D0), each pushed back d along camRotT[2] and spread b / a along camRotT[1] / camRotT[0], with
 * random spin, fall speed and sway. The -0.015f / 0.15f / 0.15f literals are RoData+0x14..+0x1C (donor
 * .rodata 0..8, pinned as snoweffects_uso_func_000039C4_rodata). The `if (1) {}` keeps b's
 * (r - 0.5) * 60 right after its sys_randf call (else uopt sinks it to its first use); the gravity
 * store first and accel.y between accel.x and accel.z give the target's store schedule. */
extern "C" void snoweffects_uso_func_000039C4(SnowFall *t) {
    SnowParticle *p;
    int i;
    float d, a, b;
    SnowGfxLib *gl = (SnowGfxLib *)D_00000000;
    t->unk98[0] = gl->curWindow->camera->pos[0] * gl->worldScale[0];
    t->unk98[1] = gl->curWindow->camera->pos[1] * gl->worldScale[1];
    t->unk98[2] = gl->curWindow->camera->pos[2] * gl->worldScale[2];
    for (i = 0; i < t->unk40; i++) {
        p = (SnowParticle *)CALL(SnowPool_alloc_000006D0)(t);
        if (p != 0) {
            d = gl_func_00000000_snow_randf() * 100.0f;
            a = (gl_func_00000000_snow_randf() - 0.5f) * 60.0f;
            b = (gl_func_00000000_snow_randf() - 0.5f) * 60.0f;
            if (1) {}
            p->pos.x = t->unk98[0] - t->camRotT[2][0] * d;
            p->pos.y = t->unk98[1] - t->camRotT[2][1] * d;
            p->pos.z = t->unk98[2] - t->camRotT[2][2] * d;
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
            p->unk3C = 1.0f;
            p->minVelY = -1.0f;
            p->unk2C = gl_func_00000000_snow_randf() * 360.0f;
            p->vel.x = (gl_func_00000000_snow_randf() - 0.5f) * 0.0f;
            p->vel.y = -(gl_func_00000000_snow_randf() * 2.5f) - 1.0f;
            p->vel.z = (gl_func_00000000_snow_randf() - 0.5f) * 0.0f;
            p->accel.x = (gl_func_00000000_snow_randf() - 0.5f) * 0.15f;
            p->accel.y = 0.0f;
            p->accel.z = (gl_func_00000000_snow_randf() - 0.5f) * 0.15f;
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
