#include "common.h"
#include "uso_call.h"
#include "gdl/gadget.h" /* PointsPopup */

/* PointsGadget_display_FFFFFFE0: the FIRST function of the game_libs text, bootup.uso
 * Text section offset 0x1464C (USO Sym export 871, ROM 0xDE50B8). Its first 8
 * words (the 255 / 235 / 80 / 0 FP constants and the first div.s, hoisted above
 * the prologue) sit 0x20 bytes BEFORE game_libs VRAM 0, so the function used to
 * be split across the old bootup_uso_post / game_libs_pre bins and the INCLUDE_ASM
 * gl_func_00000000 (0x1CC). It now lives in its own unit, linked as its own
 * output section ahead of .game_libs (which now starts at VRAM 0x1CC); the
 * blank-call placeholder gl_func_00000000 is pinned to 0 in
 * undefined_syms_auto.txt.
 *
 * A PointsPopup's display: the colour fades toward (235, 80, 80) / 255 by p->unk74 / 255
 * (PointsGadget_update_00003430 ramps unk74 by 0x10), then the text is drawn with the arg
 * font at (x + 2, y) scaled by scaleX / scaleY, and (if set) the font sprite at
 * (x + unk80, y + unk84), both at alpha x the global fade scale. cfe -O2, one pass:
 * - base[] / col[] are arrays (memory: base[3] is a dead store, base[1] / [2]
 *   are reloaded); r / g / b get homes (stored, then reloaded into col);
 * - the declaration order base, col, d255, r, g, b, t plus an unused int pad[4]
 *   gives the 0x70 frame (b / g / r at 64 / 68 / 72, col at 80, base at 96);
 * - every sum is written `product + field` (cfe emits `add.s field, product`);
 *   on the typed fields the drawSprite sums read `field + (f32)offset` (same add.s order);
 * - the 5-argument call takes a float at 16(sp): prototyped gl_func_00000000f. */
extern int gl_func_00000000();
extern int gl_func_00000000f(char *, int, int, int, f32, char *);
extern f32 D_00000000;


void PointsGadget_display_FFFFFFE0(PointsPopup *p) {
    f32 base[4];
    f32 col[4];
    f32 d255 = 255.0f;
    f32 r, g, b;
    f32 t;
    int pad[4];

    base[0] = 235.0f / d255;
    base[1] = 80.0f / d255;
    base[2] = 80.0f / d255;
    base[3] = 0.0f / d255;
    t = (f32)p->unk74;
    r = (base[0] - p->colour[0]) * t / d255 + p->colour[0];
    g = (base[1] - p->colour[1]) * t / d255 + p->colour[1];
    b = (base[2] - p->colour[2]) * t / d255 + p->colour[2];
    col[0] = r;
    col[1] = g;
    col[2] = b;
    col[3] = 1.0f;
    CALL(gui_setupRenderMode_0000055C)((char *)p->arg);
    CALL(gui_setPrimColor_000006B8)((char *)p->arg, (int)((f32)p->alpha * D_00000000), col, 255);
    /* scaleX goes in a3 as its raw bits (the callee is K&R here: lw, not lwc1 + mfc1) */
    XCALL(gl_func_00000000f, gui_drawStringScaled_00000F04)((char *)p->arg, (int)(p->parms.xps.value + 2.0f), (int)p->parms.yps.value,
                      *(int *)&p->scaleX, p->scaleY, p->text);
    if (p->font != 0) {
        CALL(gui_spriteRefresh)(p->font);
        CALL(gui_setColorEnvPrim_000017DC)(p->font, (int)((f32)p->alpha * D_00000000), col, 255);
        CALL(gui_drawSprite)(p->font, (int)(p->parms.xps.value + (f32)p->unk80),
                         (int)(p->parms.yps.value + (f32)p->unk84), 3);
    }
}
