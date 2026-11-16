#include "common.h"
#include "game/guimenu.h"
#include "uso_call.h"

/* -O0 sub-file covering bootup_uso offset 0x10B6C..0x10C8B (single function,
 * 72 insns, 0x120 bytes), split from the middle of bootup_uso_tail3a.c
 * (-O2 -g3) on 2026-07-10. tail3a TRUNCATE_TEXT reduced 0x14D8 -> 0x848;
 * the 0x10C8C..0x116C7 remainder moved to bootup_uso_tail3a_10C8C.c
 * (INCLUDE_ASM, still -O2 -g3) and 0x116C8 to bootup_uso_o0_116C8.c. */

extern int func_00000000();

/* gui_addRowSub_00010B6C: record-append+init, the with-sub-object sibling of
 * gui_addRow_00010AB0 (GuiMenu rows[] / numRows, include/game/guimenu.h).
 * MATCHED 2026-07-10 (72/72, zero diffs pre-link).
 * Key -O0 shapes: (1) STRUCT MEMBER-ARRAY form is load-bearing for the call
 * arg: `&a0->rows[a0->numRows].sub` emits `addu a0, <base>, <idx>` (base-first,
 * matching target 0x10C2C); every pointer-arithmetic / cast-array spelling
 * of the same address emits idx-first (addu a0, t7, t2) — probed 10 variants.
 * Store addresses fold either way (offset folded into sw), so only the
 * address-as-value site distinguishes the forms. (2) cfe DAG-share inside
 * the else-arm statement `rows[numRows].id = numRows` reuses the numRows load
 * for both index and stored value (sw t9 after t9-indexed addu). (3) the
 * final `numRows += 1` goes through a REGISTER pointer var (p = a0; p->numRows
 * += 1) — that is what colors s0 (lw s0, 0x28(sp)) and forces the s0
 * save/restore at 0x18(sp). */
void gui_addRowSub_00010B6C(GuiMenu *a0, void *a1, int a2, GuiRowFn a3) {
    register GuiMenu *p;

    a0->rows[a0->numRows].enabled = 1;
    if (a2 != -1) {
        a0->rows[a0->numRows].id = a2;
    } else {
        a0->rows[a0->numRows].id = a0->numRows;
    }
    a0->rows[a0->numRows].label = 0;
    FCALL(gui_spriteInit)(&a0->rows[a0->numRows].sub, a1);
    a0->rows[a0->numRows].arg = a3;
    p = a0;
    p->numRows += 1;
}
