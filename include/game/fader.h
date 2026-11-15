#ifndef GAME_FADER_H
#define GAME_FADER_H

#include "gdl/node.h"

/* The screen Fader (bootup_uso_tail4.c, 0x78; agent-h run 507). One instance, built by
 * Fader_ctor ("Fader", Process vtable then its own: Data 0xC338 / 0xC6C8) and kept in
 * GameFlow.fader (game/input.h +0x190). The fade is a 32x32 "fire" grid: Fader_start clears
 * the grid and scatters 5 random hot cells, Fader_display (NCC donor bootup_uso_ncc_1304C.cpp)
 * diffuses it every frame and uploads it as a 32x32 texture, Fader_buildTexture maps the
 * cells through the GfxLib palette (mode != 0) or fills the texture with 1 (mode 0).
 * progress reaches 1.0 when the fade is done (Fader_isDone / Fader_checkDone).
 * Field names are from those five functions. */
typedef struct Fader {
    /* 0x00 */ Node node;           /* node.name = "Fader" */
    /* 0x2C */ s32 arg;             /* ctor arg 1 */
    /* 0x30 */ f32 colour[4];       /* 1.0 x 4 (ctor) */
    /* 0x40 */ s32 key;             /* Fader_start arg 1: the requested fade; a repeat request is ignored */
    /* 0x44 */ s32 state;           /* 4 on start */
    /* 0x48 */ f32 progress;        /* 0 on start, 1.0 = done (ctor starts at 1.0) */
    /* 0x4C */ u16 *texture;        /* 0x800 bytes: the 32x32 16-bit texture Fader_buildTexture writes */
    /* 0x50 */ s32 buf50;           /* 0x400-byte work buffer */
    /* 0x54 */ s32 *screenIdx;      /* 0x400 ints: framebuffer index (y * 320 + x) of each cell's centre */
    /* 0x58 */ f32 *grid;           /* 0x400 floats: the 32x32 heat cells */
    /* 0x5C */ s32 cols;            /* 32 */
    /* 0x60 */ s32 rows;            /* 32 */
    /* 0x64 */ s32 cells;           /* 1024 */
    /* 0x68 */ s32 mode;            /* Fader_start arg 2: 0 = plain fill (checkDone then signals GfxLib), else palette */
    /* 0x6C */ s32 unk6C;           /* 1 for mode 0, else 0 */
    /* 0x70 */ s32 unk70;           /* 3 for mode 0 */
    /* 0x74 */ s32 unk74;           /* 0 on start */
} Fader;

typedef char Fader_size_check[sizeof(Fader) == 0x78 ? 1 : -1];
typedef char Fader_grid_check[(int)&((Fader *)0)->grid == 0x58 ? 1 : -1];

/* Field access through an untyped pointer (the bodies keep `char *` parameters). */
#define FADER(p) ((Fader *)(p))

#endif
