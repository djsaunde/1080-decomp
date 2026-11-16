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
};
#define PKT(c, a, b) { Gfx *_g = (c)->next(); _g->w0 = (a); _g->w1 = (b); }

/* the trail emitter (C-side struct SnowRing in snoweffects_uso.c; only what the donors touch) */
struct SnowRing {
    char pad00[0xB0];
    /* 0xB0 */ DLCtx *dl;
};

#endif
