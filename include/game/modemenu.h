#ifndef GAME_MODEMENU_H
#define GAME_MODEMENU_H

#include "gdl/node.h"

/* The mode modules' menu processes (agent-h run 509): "ARCMenu Process" (ARCMenuProcess_ctor,
 * 0x54) and "TIMMenu Process" (TimMenu_ctor / TrkMenu_ctor, 0x6C). Each ctor runs the in-place
 * Node (0x2C) -> Process (0x50) cascade, then builds a bootup GUI row menu (an import ctor
 * called with 0; a GuiMenu, include/game/guimenu.h) into +0x48, adds the rows (label id
 * ((x + 3) << 16) | n, id -1, the row callback *_row* as the row arg) and gives the menu a
 * frame window in its unk30. The update / handleInput functions run
 * rows[cursor].arg() when the confirm button is pressed on an enabled row. */
typedef struct MenuProcess {
    /* 0x00 */ Node node;
    /* 0x2C */ s32 unk2C;           /* 0 */
    /* 0x30 */ s32 unk30;           /* 1 */
    /* 0x34 */ char unk34[0x48 - 0x34];
    /* 0x48 */ s32 menu;            /* the GuiMenu (Process child slot: RaceProcess.menu) */
    /* 0x4C */ s32 unk4C;
    /* 0x50 */ s32 state;           /* 0. Tim / Trk: the update / draw switch; handleInput sets 0 */
} MenuProcess;

typedef char MenuProcess_size_check[sizeof(MenuProcess) == 0x54 ? 1 : -1];

/* "TIMMenu Process" (timproc / trkproc, 0x6C): adds the post-race save-ghost prompt that
 * TimMenu_handleInput_00002A8C / TrkMenu_handleInput_00002C98 run (state 1 / 2) and
 * *_draw / *_drawPair draw. */
typedef struct TimMenu {
    /* 0x00 */ MenuProcess base;
    /* 0x54 */ s32 unk54;           /* 0 */
    /* 0x58 */ s32 choice;          /* prompt cursor: 0 save the ghost, 1 skip */
    /* 0x5C */ s32 blink;           /* -16 per draw: the alpha of the highlighted prompt row */
    /* 0x60 */ s32 unk60;           /* 120 */
    /* 0x64 */ s32 unk64;           /* 161 */
    /* 0x68 */ s32 promptY;         /* 191: draw places the prompt rows from it */
} TimMenu;

typedef char TimMenu_size_check[sizeof(TimMenu) == 0x6C ? 1 : -1];

/* Field access through the untyped `char *` / `int *` parameters (matching levers). */
#define MENUPROC(p) ((MenuProcess *)(p))
#define TIMMENU(p)  ((TimMenu *)(p))

#endif
