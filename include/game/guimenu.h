#ifndef GAME_GUIMENU_H
#define GAME_GUIMENU_H

#include "gdl/node.h"

/* The bootup GUI menu (agent-h run 507): the pause menu PauseMenu_create builds (0x304,
 * Process base, PauseMenu_vtbl) and the other row menus that use the gui_* helpers
 * (bootup_uso_o0_100F0 / 10324 / 10AB0 / 10B6C, PauseMenu_update in o0_1034C).
 * gui_initRows sets arg / clears the cursor and the row height 17; gui_addRow /
 * gui_addRowSub append a row; gui_centerRows centres the block at x 160 around y 120, sets
 * alpha 255 and restarts timer; PauseMenu_update fades alpha up to 255, counts timer and moves the
 * cursor up (pad 0x2004) / down (0x8008) to the next enabled row, wrapping. */
typedef struct GuiRow {
    /* 0x00 */ s32 label;           /* gui_addRow arg 1 (gui_addRowSub: 0, the label goes to sub) */
    /* 0x04 */ s32 enabled;         /* 1 when added; the cursor skips rows with 0 */
    /* 0x08 */ s32 id;              /* arg 2, or the row index when arg 2 is -1 */
    /* 0x0C */ s32 arg;             /* arg 3 */
    /* 0x10 */ char sub[0x18];      /* gui_addRowSub initialises it with its arg 1 */
} GuiRow;

/* A gui_uso sprite slot (0x18; gui_spriteInit / spriteLoad / spriteRelease / spriteRefresh, agent-h
 * run 533): fileId is the BinFileMgr id (spriteLoad arg 2: bank << 16 | entry), image the acquired
 * file (0 until loaded). The h2h WinsDisplay reads the image's s16 at +0x20 (its width) to place
 * the label after a sprite. */
typedef struct GuiSprite {
    /* 0x00 */ char unk00[0x10];
    /* 0x10 */ char *image;         /* BinFileMgr_getAcquire(fileId); +0x20 = s16 width */
    /* 0x14 */ s32 fileId;
} GuiSprite;

typedef char GuiSprite_size_check[sizeof(GuiSprite) == 0x18 ? 1 : -1];

typedef struct GuiMenu {
    /* 0x00 */ Node node;
    /* 0x2C */ s32 arg;             /* gui_initRows arg 1 */
    /* 0x30 */ s32 unk30;
    /* 0x34 */ s32 alpha;           /* 0xFF on centerRows; PauseMenu_update counts a lower value up to 0xFF */
    /* 0x38 */ s32 timer;           /* 0 on centerRows, +1 per update */
    /* 0x3C */ f32 colour[4];       /* rgb from the RoData triple at 0xC10, a = 0 */
    /* 0x4C */ char unk4C[0x6C - 0x4C];
    /* 0x6C */ f32 x;               /* 160 (centerRows) */
    /* 0x70 */ f32 y;               /* 120 - rowHeight / 2 * numRows */
    /* 0x74 */ f32 rowHeight;       /* 17 (initRows) */
    /* 0x78 */ s32 numRows;
    /* 0x7C */ s32 cursor;          /* the current row (gui_getCurrentRow) */
    /* 0x80 */ s32 locked;          /* nonzero: PauseMenu_update ignores the pad */
    /* 0x84 */ GuiRow rows[16];     /* 0x304 = 0x84 + 16 * 0x28 (PauseMenu_create) */
} GuiMenu;

typedef char GuiRow_size_check[sizeof(GuiRow) == 0x28 ? 1 : -1];
typedef char GuiMenu_size_check[sizeof(GuiMenu) == 0x304 ? 1 : -1];
typedef char GuiMenu_cursor_check[(int)&((GuiMenu *)0)->cursor == 0x7C ? 1 : -1];

/* Field access through an untyped `char *` / `int *` (the -O0 bodies keep them). */
#define GUIMENU(p) ((GuiMenu *)(p))

#endif
