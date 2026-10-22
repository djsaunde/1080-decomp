#include "common.h"

extern int func_00000000();
extern char D_00000000;

/* bootup_uso_o0_11E00.c (formerly bootup_uso_tail3a_bot_11DF8.c, the tail slice
 * of the 2026-07-10 tail3a_bot carve; renamed + moved to -O0 2026-09-05).
 * Holds the func_00011E00 / func_00011ED4 / func_00011FA8 -O0 triplet: three
 * 53-word "find first slot whose mask word has an a1 bit set" leaves that differ
 * only in the slot mask offset (0x18 / 0x10 / 0x1C). The old -O2 -g3 setting was
 * a leftover of the "adjacent empty stub blocks the -O0 move" note: that 8-byte
 * "func_00011DF8" stub was func_00011DBC's dead -O0 `$exit` pair (absorbed into
 * o0_11DBC.c), and the 1207C/12090 fall-off tail is now o0_1207C.c, so nothing
 * blocks -O0 here. TRUNCATE_TEXT 0x27C = 3 x 0xD4 (INCLUDE_ASM path). */

/* EXACT via an IDO 7.1 NCC (C++) -O0 donor: src/bootup_uso/bootup_uso_ncc_11E00.cpp,
 * REPLACE_FUNC_BODY-spliced over this placeholder (agent-c run 119). */
int func_00011E00(int *a0, int a1) {
    return 0;
}

/* EXACT via an IDO 7.1 NCC (C++) -O0 donor: src/bootup_uso/bootup_uso_ncc_11E00.cpp,
 * REPLACE_FUNC_BODY-spliced over this placeholder (agent-c run 119). */
int func_00011ED4(int *a0, int a1) {
    return 0;
}

/* EXACT via an IDO 7.1 NCC (C++) -O0 donor: src/bootup_uso/bootup_uso_ncc_11E00.cpp,
 * REPLACE_FUNC_BODY-spliced over this placeholder (agent-c run 119). */
int func_00011FA8(int *a0, int a1) {
    return 0;
}
