#include "uso_call.h"
/* game_libs ido53 carve: gl_osViSetYScale -- 1080 interrupt-bracket float
 * setter, verbatim shape at IDO 5.3 -O1, 22/22 first try with ALL words
 * (incl. reloc fields) matching: sr = __osDisableInt-class call;
 * P1->f_24 = arg; *P2 |= 4; restore(sr). Globals are reloc'd POINTER
 * variables (lui+lw, %lo=0) -- zero-alias placeholders. The 8-byte pad
 * sidecar is reproduced by an all-zero SUFFIX. */
extern int gl_func_00000000();
#include "libultra/os_vi.h"
extern __OSViContext *gl___osViNext_74D54_p1;  /* __osViNext */
extern unsigned short *gl___osViNext_74D54_p2;

void gl_osViSetYScale(float f) {
    register int sr;
    /* UNNAMED: mid-entry gl___osDisableInt+0x4 (libultra __osDisableInt entry, one insn into the build symbol) */
    sr = gl_func_00000000();
    gl___osViNext_74D54_p1->y.factor = f;
    *gl___osViNext_74D54_p2 |= VI_STATE_YSCALE_UPDATED;
    CALL(gl___osRestoreInt)(sr);
}
