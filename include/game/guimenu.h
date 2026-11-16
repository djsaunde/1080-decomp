#ifndef GAME_GUIMENU_H
#define GAME_GUIMENU_H

#ifndef __cplusplus
#include "gdl/node.h"
#else
#include "../gdl/node.h" /* the edg (NCC) donors include this header by relative path, without -I include */
#endif

/* The bootup GUI menu (agent-h run 507): the pause menu PauseMenu_create builds (0x304,
 * Process base, PauseMenu_vtbl) and the other row menus that use the gui_* helpers
 * (bootup_uso_o0_100F0 / 10324 / 10AB0 / 10B6C, PauseMenu_update in o0_1034C).
 * gui_initRows sets arg / clears the cursor and the row height 17; gui_addRow /
 * gui_addRowSub append a row; gui_centerRows centres the block at x 160 around y 120, sets
 * alpha 255 and restarts timer; PauseMenu_update fades alpha up to 255, counts timer and moves the
 * cursor up (pad 0x2004) / down (0x8008) to the next enabled row, wrapping. */
/* The row callback: the pause / menu processes call rows[cursor].arg() when the row is
 * picked (H2HRaceProcess_onPauseSelect, the arc / tim / trk / tit menu handleInput). */
typedef void (*GuiRowFn)(void);

typedef struct GuiRow {
    /* 0x00 */ s32 label;           /* gui_addRow arg 1 (gui_addRowSub: 0, the label goes to sub) */
    /* 0x04 */ s32 enabled;         /* 1 when added; the cursor skips rows with 0 */
    /* 0x08 */ s32 id;              /* arg 2, or the row index when arg 2 is -1 */
    /* 0x0C */ GuiRowFn arg;        /* arg 3: the row callback (0 = none) */
    /* 0x10 */ char sub[0x18];      /* gui_addRowSub initialises it with its arg 1 */
} GuiRow;

typedef struct GuiMenu {
    /* 0x00 */ Node node;
    /* 0x2C */ s32 arg;             /* gui_initRows arg 1 */
    /* 0x30 */ s32 unk30;
    /* 0x34 */ s32 alpha;           /* 0xFF on centerRows; PauseMenu_update counts a lower value up to 0xFF */
    /* 0x38 */ s32 timer;           /* 0 on centerRows, +1 per update */
    /* 0x3C */ f32 colour[4];       /* rgb from the RoData triple at 0xC10, a = 0 */
    /* 0x4C */ f32 selColour[4];    /* the cursor row's rgba (PauseMenu_create: 119 / 117 / 229 / 255 over 255;
                                     * PauseMenu_display's q4C) */
    /* 0x5C */ f32 rowColour[4];    /* the other rows' rgba (192 / 194 / 229 / 255 over 255; display's q5C) */
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
#ifndef __cplusplus /* edg (NCC donors): &((T *)0)->f is not an integral constant expression */
typedef char GuiMenu_cursor_check[(int)&((GuiMenu *)0)->cursor == 0x7C ? 1 : -1];
typedef char GuiMenu_rowColour_check[(int)&((GuiMenu *)0)->rowColour == 0x5C ? 1 : -1];
#endif

/* Field access through an untyped `char *` / `int *` (the -O0 bodies keep them). */
#define GUIMENU(p) ((GuiMenu *)(p))

#endif
