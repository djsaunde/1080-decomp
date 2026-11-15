#include "common.h"
extern int D_00000000;
typedef struct { int a, b, c, d; } Quad4;
typedef struct { float x, y, z; } Vec3;



/* The six all-zero words at 0x74EDC..0x74EF4 (Sym oracle: section 0x89548
 * not exported, no baked jal) are the tail pad of the previous object
 * (gl_osViSetXScale, game_libs_ido53_74DB4.c) ahead of the vimgr.c object;
 * they are appended to gl_osViSetXScale as an all-zero SUFFIX_BYTES_FORCE
 * (Makefile, both trees; was the pseudo-function game_libs_func_00074EDC). */

/* osCreateViManager = osCreateViManager + gl_viMgrMain = viMgrMain
 * (libultra vimgr.c verbatim, ONE IDO 5.3 -O1 -mips2 TU donor:
 * game_libs_ido53_74EF4.c). LANDED 2026-08-22 via REPLACE_FUNC_BODY
 * donor splice, both first-compile word-exact (98/98 + 115/115 incl.
 * the 6-word zero gap before viMgrMain's dead epilogue -- as1 32-byte-
 * aligns the unreachable block, offset falls out once create is the
 * right 0x188 size). Keys that cracked it:
 *   - whole-TU compile (viMgrMain static; .bss layout viThread 0x1B0 +
 *     stack 0x1000 + queue + buf + msgs + static retrace == USO
 *     0x44080..0x45292 exactly);
 *   - __osViDevMgr DEFINED in the TU (USO .data offset 0 -- the baked
 *     "blank-looking" lui/lo 0 words are its real address): local-.data
 *     definition makes IDO share $at across the 7 field stores; extern
 *     decl re-luis $at each store (+5 words, the old 65% residual);
 *   - viMgrMain fn-ptr materialized against .text (baked original-link
 *     0x896E8, addend 0x188 -> pin osCreateViManager_text = 0x89560).
 * The old "-Olimit allocator quirk" verdict on 7507C is RETRACTED --
 * whole-TU -O1 gives the s0 switch value + 0x38 frame naturally.
 * Bodies below are placeholders for the splice. */
void osCreateViManager(int pri) {
    volatile int vimgr_spliced = 0;
    if (pri != 0) {
        vimgr_spliced = pri;
    }
}

void gl_viMgrMain(char *arg0) {
    volatile int vimgrmain_spliced = 0;
    if (arg0 != 0) {
        vimgrmain_spliced = 1;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_libs/game_libs/game_libs_func_00075248_pad.s")
/* gl___osViGetCurrentContext: the standard cross-USO int-reader template
 * (return D_global), matched in-place at the unit's default 7.1 -O2.
 * The symbol was mis-split as 75248 (3 leading zero pad words now in the
 * _pad sidecar); cc's natural function padding supplies the trailing
 * alignment pad (see below). */
extern int gl___osViCurr_75254_v;
int gl___osViGetCurrentContext(void) {
    return gl___osViCurr_75254_v;
}

/* The zero word at 0x75260 is 75254's trailing alignment pad, appended to
 * gl___osViGetCurrentContext as an all-zero SUFFIX_BYTES_FORCE (Makefile, both
 * trees; was the pseudo-function game_libs_func_00075260). */

/* gl_func_0007526C MATCHED 2026-06-10 as gl___osSpSetPc (own
 * -O1 carve unit game_libs_ido_75264.c): the "dual-entry" theory was
 * wrong -- 75260's lui/lw was the fn's own first statement (boundary
 * mis-split) and the leading nop is 75254's trailing pad (kept below as
 * the 1-word 75260 pad INCLUDE). */
