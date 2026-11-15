#include "common.h"
#include "gdl/gl_data.h"
#include "game/guimenu.h"
#include "uso_call.h"

extern int func_00000000();
extern char D_00000000;

/* PauseMenu_update: -O0 island split out of bootup_uso_tail3a.c on 2026-07-10
 * and MATCHED at -O0 (125/125 target words; the 3 trailing pad words are
 * clipped by TRUNCATE_TEXT 0x1F4). Same carve recipe as o0_10B6C / o0_116C8.
 *
 * Button-scan cursor advance (game/guimenu.h GuiMenu): on 0x2004 (up) walk the
 * rows backwards from the cursor to the previous enabled row; on 0x8008 (down)
 * walk forwards, wrapping. alpha fades up to 0xFF, timer counts frames, locked
 * blocks the pad.
 *
 * -O0 levers that produced the exact bytes:
 *  - `register char *s` (colors s0) used for the three RMW sites
 *    (alpha++, timer++, cursor--/++). Being the only register var it still
 *    reserves a dead home slot -> frame 0x28 with s0 saved at 0x18.
 *  - the alpha site pre-adds the offset (s = &alpha; *(int*)s += 1)
 *    while the others fold it (s = a0; GUIMENU(s)->timer += 1) -- exactly
 *    reproduces the target's addiu-s0 vs folded-displacement split.
 *  - slot probe via the ARRAY FORM rows[cursor].enabled: gives the
 *    base-first `addu t6, t3, t5` + folded 0x88 displacement; the pointer-
 *    arithmetic spelling emits offset-first addu (9-word diff cascade).
 *  - `break` inside the do-while emits the target's b-to-merge; the
 *    while condition `++counter < numRows` gives pre-inc slt operand order.
 *  - wrap fixups written as plain a0-based compares/stores (one home reload
 *    DAG-shared across both derefs of the same expression). */
void PauseMenu_update(char *a0) {
    int counter;
    register char *s;

    if (GUIMENU(a0)->alpha < 0xFF) {
        s = (char *)&GUIMENU(a0)->alpha;
        *(int*)s += 1;
    }
    s = a0;
    GUIMENU(s)->timer += 1;
    if (GUIMENU(a0)->locked == 0) {
        if (FCALL(input_checkPressed_00011E00)(&gInputMgr, 0x2004) != 0) {
            FCALL(snd_postEvent)(1);
            counter = 0;
            if (GUIMENU(a0)->numRows > 0) {
                do {
                    s = a0;
                    GUIMENU(s)->cursor -= 1;
                    if (GUIMENU(a0)->cursor < 0) {
                        GUIMENU(a0)->cursor = GUIMENU(a0)->numRows - 1;
                    }
                    if (GUIMENU(a0)->rows[GUIMENU(a0)->cursor].enabled != 0) {
                        break;
                    }
                } while (++counter < GUIMENU(a0)->numRows);
            }
        }
    }
    if (GUIMENU(a0)->locked == 0) {
        if (FCALL(input_checkPressed_00011E00)(&gInputMgr, 0x8008) != 0) {
            FCALL(snd_postEvent)(1);
            counter = 0;
            if (GUIMENU(a0)->numRows > 0) {
                do {
                    s = a0;
                    GUIMENU(s)->cursor += 1;
                    if (GUIMENU(a0)->cursor >= GUIMENU(a0)->numRows) {
                        GUIMENU(a0)->cursor = 0;
                    }
                    if (GUIMENU(a0)->rows[GUIMENU(a0)->cursor].enabled != 0) {
                        break;
                    }
                } while (++counter < GUIMENU(a0)->numRows);
            }
        }
    }
}
