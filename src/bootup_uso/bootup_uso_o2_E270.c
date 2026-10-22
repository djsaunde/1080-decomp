/* IDO 7.1 -O2 donor for func_0000E270 -- ratio wrapper (0x60 / 24 words).
 *
 * WIRED via REPLACE_FUNC_BODY donor splice into bootup_uso.c.o (the 2DA4 /
 * game_libs 675A4 recipe): the constant is the f32 LITERAL pi, which bootup.uso
 * ships in its RoData at +0x998 (ROM 0xD9FE44 + 0x998). The donor-local .rodata
 * reloc is renamed func_0000E270_rodata and pinned to 0x998.
 *
 * As a literal the quotient is an expression, not a named local: `ratio` as a
 * named f32 cost a frame word (0x28 vs 0x20) and swapped the f0/a1 spill slots
 * (the old "spill-slot pair swap" residue). Written twice inline, IDO CSEs the
 * division into one temp whose spill lands at sp+0x18 under the a1 home. */
#include "common.h"

extern void aE270(void *, void *, float); /* float-proto blank import */

void func_0000E270(char *arg0, float arg1) {
    aE270(arg0 + 0xCC, arg0 + 0x3B0, 3.14159265f / arg1);
    aE270(arg0 + 0xF4, arg0 + 0x3B0, 3.14159265f / arg1);
}
