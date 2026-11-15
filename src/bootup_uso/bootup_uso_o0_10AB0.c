#include "common.h"
#include "game/guimenu.h"

/* gui_addRow_00010AB0: -O0 island split out of bootup_uso_tail3a.c on 2026-07-10
 * and MATCHED at -O0 (47/47 target words; 1 trailing pad word clipped by
 * TRUNCATE_TEXT 0xBC). Record-append into the 0x28-stride ring at a0+0x84:
 * row.enabled = 1, row.id = (a2 == -1 ? numRows : a2), row.label = a1,
 * row.arg = a3, then numRows++ (GuiMenu / GuiRow, include/game/guimenu.h).
 *
 * -O0 levers that produced the exact bytes:
 *  - REGISTER PARAMS (register char *a0, register int a1/a2/a3): suppresses
 *    the -O0 arg homing entirely (args used straight from a0-a3, matching
 *    the island's unhomed shape).
 *  - `register char *s` -- leaf fn, so the register local colors t0 (the
 *    `or t0, a0, zero` before the cursor increment), and as the only
 *    (register-first) local it reserves a dead home slot -> frame 0x8.
 *  - every field store recomputes the record base from scratch via the
 *    ARRAY FORM GUIMENU(a0)->rows[numRows].field: base-first addu
 *    (addu $t9, $a0, $t8) + per-field folded displacement.
 *  - `if (a2 != -1) {...} else {...}` puts the a2-store arm first with the
 *    b-over-else, matching the target block order. */
void gui_addRow_00010AB0(register char *a0, register int a1, register int a2, register int a3) {
    register char *s;

    GUIMENU(a0)->rows[GUIMENU(a0)->numRows].enabled = 1;
    if (a2 != -1) {
        GUIMENU(a0)->rows[GUIMENU(a0)->numRows].id = a2;
    } else {
        GUIMENU(a0)->rows[GUIMENU(a0)->numRows].id = GUIMENU(a0)->numRows;
    }
    GUIMENU(a0)->rows[GUIMENU(a0)->numRows].label = a1;
    GUIMENU(a0)->rows[GUIMENU(a0)->numRows].arg = a3;
    s = a0;
    GUIMENU(s)->numRows += 1;
}
