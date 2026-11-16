/* IDO 7.1 NCC (C++) donor for SelectWin_dimOtherItems_00008AFC (0x120 / 72 words):
 * per-group element refresh through the cfront virtual slot 0x58/0x5C.
 * K = 0.9f is a source literal (donor .rodata pinned at RoData +0x238 as
 * SelectWin_dimOtherItems_00008AFC_rodata); the this-adjust is spelled
 * `(char *)(delta + (int)e)` for the target's addu operand order.
 * Spliced into timproc_uso_b5.c.o via REPLACE_FUNC_BODY. */
typedef short s16;
typedef float f32;
typedef int s32;
#include "../../include/game/select.h"  /* NCC donors build without -I include */
#define WIN ((SelectWin *)scr)
extern "C" {
extern int SelectWin_curItem_00008A38(...);
}
extern "C" void SelectWin_dimOtherItems_00008AFC(char *scr) {
    f32 K = 0.9f;
    int g, i;
    char *e;
    char *vt;
    char *o;

    for (g = 0; g < 4; g++) {
        for (i = 0; i < WIN->itemsMgr->lists[g]->count; i++) {
            o = (char *)SelectWin_curItem_00008A38(scr);
            e = (char *)WIN->itemsMgr->lists[g]->items[i];
            if (o != e) {
                o = (char *)SELITEM(e)->vptr;
                ((void (*)(char *, f32))SELVT(o)->updateHighlight.fn)((char *)(SELVT(o)->updateHighlight.delta + (int)e), 1.0f - WIN->fade * K);
            }
        }
    }
    o = (char *)SelectWin_curItem_00008A38(scr);
    vt = (char *)SELITEM(o)->vptr;
    ((void (*)(char *, f32))SELVT(vt)->updateHighlight.fn)((char *)(SELVT(vt)->updateHighlight.delta + (int)o), 1.0f);
}
