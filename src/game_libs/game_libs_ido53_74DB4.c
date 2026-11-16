/* game_libs ido53 carve: gl_osViSetXScale -- bigger sibling of the 74D54
 * interrupt-bracket setter, verbatim shape at IDO 5.3 -O1, 74/74 first
 * try (+ one register-declaration-order swap for the s0/s1 assignment:
 * v declared before sr puts v in s0, sr in s1). Shape: sr bracket;
 * P1->f_18 = arg; *P2 |= 2; v = P1->p_8->w_20 & 0xFFF;
 * P1->w_20 = (u32)(P1->f_18 * (float)(u32)v) & 0xFFF. The entire FCSR
 * save/set/convert/check/restore block is IDO's inline (u32)float cast;
 * the 0x4F800000 add is the standard (float)(u32) correction. The two
 * statements share one global load via same-BB CSE. */
#include "libultra/os_vi.h"
#include "uso_call.h"
extern int gl_func_00000000();
extern __OSViContext *gl___osViNext_74DB4_p1;  /* __osViNext */
extern unsigned short *gl___osViNext_74DB4_p2; /* &__osViNext->state (its own reloc'd load) */

void gl_osViSetXScale(float f) {
    register int v;
    register int sr;
    /* UNNAMED: mid-entry gl___osDisableInt+0x4 (libultra __osDisableInt entry, one insn into the build symbol) */
    sr = gl_func_00000000();
    gl___osViNext_74DB4_p1->x.factor = f;
    *gl___osViNext_74DB4_p2 |= VI_STATE_XSCALE_UPDATED;
    v = gl___osViNext_74DB4_p1->modep->comRegs.xScale & VI_SCALE_MASK;
    gl___osViNext_74DB4_p1->x.scale = (u32)(gl___osViNext_74DB4_p1->x.factor * (float)(u32)v) & VI_SCALE_MASK;
    CALL(gl___osRestoreInt)(sr);
}
