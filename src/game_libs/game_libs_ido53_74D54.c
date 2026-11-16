#include "uso_call.h"
/* game_libs ido53 carve: gl_osViSetYScale -- 1080 interrupt-bracket float
 * setter, verbatim shape at IDO 5.3 -O1, 22/22 first try with ALL words
 * (incl. reloc fields) matching: sr = __osDisableInt-class call;
 * P1->f_24 = arg; *P2 |= 4; restore(sr). Globals are reloc'd POINTER
 * variables (lui+lw, %lo=0) -- zero-alias placeholders. The 8-byte pad
 * sidecar is reproduced by an all-zero SUFFIX. */
extern int gl_func_00000000();
extern char *gl___osViNext_74D54_p1;
extern unsigned short *gl___osViNext_74D54_p2;

void gl_osViSetYScale(float f) {
    register int sr;
    sr = gl_func_00000000();
    /* RAW: unknown-object: libultra __OSViContext (y scale 0x24) has no header in include/libultra yet */
    *(float *)(gl___osViNext_74D54_p1 + 0x24) = f;
    *gl___osViNext_74D54_p2 |= 4;
    CALL(gl___osRestoreInt)(sr);
}
