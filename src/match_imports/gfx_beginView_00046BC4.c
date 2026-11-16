/* Native original-compiler source port from src/game_libs/game_libs_post0b.c. Target bytes unchanged. */
#include "context_game_libs_post0b.h"
#include "gdl/window.h"
#include "gdl/gfxlib.h"
/* a0 = the GfxLib (agent-b run 550: field form over gdl/gfxlib.h + gdl/window.h) */
void gfx_beginView_00046BC4(GfxLib *a0) {
    GfxLib *s0 = a0;
    int *clear_flags = &s0->flags;
    int *state_flags = &s0->unk138;     /* [1] = renderFlags (+0x13C); this base is the byte lever */
    WinClear *child;
    int condition;
    gl_46BC4_first(a0);          /* gfx_updateLights_00047F9C */
    *clear_flags &= ~1;
    gl_46BC4_second(s0);         /* gfx_resetModelView_0004E180 */
    gl_46BC4_third(s0);          /* gfx_matrixModeModelView_000478EC */
    child = (WinClear *)&s0->curWindow->win.unk100;
    condition = child->clearFlags;
    condition &= 2;
    if (condition == 0) {
        goto clear_state;
    }
    state_flags[1] |= 2;
    goto done;
clear_state:
    state_flags[1] &= ~2;
done:
    ;
}
