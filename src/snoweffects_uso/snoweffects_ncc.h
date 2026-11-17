/* Shared C++ model of the snoweffects.uso classes for its IDO 7.1 NCC donors (agent-h run 628).
 * The module is C++ (see snoweffects_uso_ncc_9F44.cpp); the donors are REPLACE_FUNC_BODY-spliced over
 * INCLUDE_ASM placeholders in snoweffects_uso.c. NCC donors build without -I include. */
#ifndef SNOWEFFECTS_NCC_H
#define SNOWEFFECTS_NCC_H

#include "../../include/uso_call.h"

/* blank (0-pinned) USO jal target: imports and calls to the module's own global functions */
extern "C" int gl_func_00000000(...);
extern "C" char D_00000000[];

struct Gfx { unsigned int w0, w1; };
struct DLHead { Gfx *buf; int count; };

/* libgdl display-list context (gdl/dlctx.h): the Gfx head at +0xC. next() appends one packet; the
 * packet pointer has to be a named local (`Gfx *g = ..; return g;`) for the target's v1 = context,
 * v0 = head colouring (agent-h run 628, 0x40B4). */
struct DLCtx {
    char pad[0xC];
    DLHead *dl;
    Gfx *next() { int n = dl->count++; Gfx *g = &dl->buf[n]; return g; }
    /* the same append as one expression (0x4188; edg makes `dl` / old-count temps instead) */
    Gfx *append() { return &dl->buf[dl->count++]; }
};
#define PKT(c, a, b) { Gfx *_g = (c)->next(); _g->w0 = (a); _g->w1 = (b); }
#define APKT(c, a, b) { Gfx *_g = (c)->append(); _g->w0 = (a); _g->w1 = (b); }

/* the trail emitter (C-side struct SnowRing in snoweffects_uso.c; only what the donors touch) */
struct SnowRing {
    char pad00[0xB0];
    /* 0xB0 */ DLCtx *dl;
};

/* 3-float vector; += is a user operator (all three adds after the loads, 0x27A8) */
struct SnowV3 {
    float x, y, z;
    SnowV3 &operator+=(const SnowV3 &v) { x += v.x; y += v.y; z += v.z; return *this; }
};

/* one 0x58-byte particle (C-side SnowParticle in snoweffects_uso.c) */
struct SnowParticle {
    /* 0x00 */ SnowV3 pos;
    /* 0x0C */ SnowV3 vel;
    /* 0x18 */ SnowV3 accel;   /* snowfall sway */
    /* 0x24 */ float gravity;
    /* 0x28 */ float minVelY;
    /* 0x2C */ float angle;     /* sys_randf() * 360 at snowfall spawn */
    /* 0x30 */ float life;
    /* 0x34 */ float decay;     /* life lost per update */
    /* 0x38 */ float size;
    /* 0x3C */ float aspect;    /* sprite height / width */
    /* 0x40 */ float sizeVel;
    /* 0x44 */ float fadeAt;    /* below this life the size starts changing */
    /* 0x48 */ unsigned char col[4];
    /* 0x4C */ int unk4C;
    /* 0x50 */ float alpha;
    /* 0x54 */ float alphaVel;
};

/* the Node-derived particle pool (C-side struct SnowPool) */
struct SnowPool {
    char pad00[0x30];
    /* 0x30 */ SnowParticle *items;
    char pad34[0x3C - 0x34];
    /* 0x3C */ int count;
    /* 0x40 */ int max;
};

/* the snowfall effect (C-side struct SnowFall; only what the donors touch) */
struct SnowFall {
    char pad00[0x30];
    /* 0x30 */ SnowParticle *items;
    char pad34[0x40 - 0x34];
    /* 0x40 */ int unk40;            /* flakes to spawn in 0x39C4 */
    char pad44[0x50 - 0x44];
    /* 0x50 */ float camRotT[3][3];  /* camera rotation, transposed (0x3620) */
    /* 0x74 */ float camRot[3][3];
    /* 0x98 */ float unk98[3];       /* camera position * world scale (0x39C4) */
    char padA4[0xB8 - 0xA4];
    /* 0xB8 */ int count;
};

/* the libgdl globals reached through the blank D_00000000 (C side: GfxLib / Window, gdl/gfxlib.h) */
struct SnowCamera { char pad[0xA0]; float pos[3]; };
struct SnowWindow { char pad[0x70]; SnowCamera *camera; };
struct SnowGfxLib {
    char pad000[0x128];
    /* 0x128 */ float worldScale[3];
    char pad134[0x254 - 0x134];
    /* 0x254 */ SnowWindow *curWindow;
};

/* typed blank (0) alias of the sys_randf import (float return) */
extern "C" float gl_func_00000000_snow_randf(void);

#endif
