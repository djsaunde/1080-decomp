/* IDO 7.1 NCC (C++) EXACT donors for snoweffects.uso (agent-h run 628), REPLACE_FUNC_BODY-spliced over the
 * INCLUDE_ASM placeholders in snoweffects_uso.c on both build paths. Classes: snoweffects_ncc.h. */
#include "snoweffects_ncc.h"

/* 0x40B4 (0xD4): the emitter draw. Reset its display list, let 0x4188 fill it, then append
 * ClearGeometryMode(0x33000), SetGeometryMode(0x2204) and EndDL through the c->next() packets
 * (the gfx_initRDPState family, game_libs_ncc_59E80.cpp). */
extern "C" void snoweffects_uso_func_000040B4(SnowRing *t) {
    CALL(gfx_resetDLContext)(t->dl);
    CALL(snoweffects_uso_func_00004188)(t, t->dl);
    PKT(t->dl, 0xB6000000, 0x00033000);
    PKT(t->dl, 0xB7000000, 0x00002204);
    PKT(t->dl, 0xB8000000, 0);
}
