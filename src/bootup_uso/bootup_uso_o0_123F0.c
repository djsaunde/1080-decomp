#include "common.h"
#include "game/input.h"

/* Run 9 subset: byte increment/decrement helpers at 0x123F0..0x1266C.
 * 3 decrementers (if byte > 0) and 3 incrementers (if byte < 10), at offsets
 * 0x12, 0x13, 0x14 within a pointer loaded from a0->0x154. At -O0 so IDO
 * emits stack-reload-per-use + `register char *p` for the $s0 pointer.
 * NOTE: the decrement guard must be the SIGNED `(int)... > 0` comparison —
 * the target emits `blez $t8`. The original `... != 0` (semantically equal
 * for a zero-extended lbu) emitted `beq $t8,$zero` instead: a 1-word silent
 * false match per fn, caught 2026-06-10 by the whole-segment byte walk. */

extern int func_00000000();

void settings_vol1Down_000123F0(char *a0) {
    register char *p;
    if ((int)*(unsigned char*)(GAMEFLOW(a0)->unlocks + 0x12) > 0) {
        p = GAMEFLOW(a0)->unlocks;
        *(p + 0x12) -= 1;
    }
    func_00000000(*(unsigned char*)(GAMEFLOW(a0)->unlocks + 0x12));
}

void settings_vol1Up_00012458(char *a0) {
    register char *p;
    if ((int)*(unsigned char*)(GAMEFLOW(a0)->unlocks + 0x12) < 10) {
        p = GAMEFLOW(a0)->unlocks;
        *(p + 0x12) += 1;
    }
    func_00000000(*(unsigned char*)(GAMEFLOW(a0)->unlocks + 0x12));
}

void settings_vol0Down_000124C4(char *a0) {
    register char *p;
    if ((int)*(unsigned char*)(GAMEFLOW(a0)->unlocks + 0x13) > 0) {
        p = GAMEFLOW(a0)->unlocks;
        *(p + 0x13) -= 1;
    }
    func_00000000(*(unsigned char*)(GAMEFLOW(a0)->unlocks + 0x13));
}

void settings_vol0Up_0001252C(char *a0) {
    register char *p;
    if ((int)*(unsigned char*)(GAMEFLOW(a0)->unlocks + 0x13) < 10) {
        p = GAMEFLOW(a0)->unlocks;
        *(p + 0x13) += 1;
    }
    func_00000000(*(unsigned char*)(GAMEFLOW(a0)->unlocks + 0x13));
}

void settings_vol2Down_00012598(char *a0) {
    register char *p;
    if ((int)*(unsigned char*)(GAMEFLOW(a0)->unlocks + 0x14) > 0) {
        p = GAMEFLOW(a0)->unlocks;
        *(p + 0x14) -= 1;
    }
    func_00000000(*(unsigned char*)(GAMEFLOW(a0)->unlocks + 0x14));
}

void settings_vol2Up_00012600(char *a0) {
    register char *p;
    if ((int)*(unsigned char*)(GAMEFLOW(a0)->unlocks + 0x14) < 10) {
        p = GAMEFLOW(a0)->unlocks;
        *(p + 0x14) += 1;
    }
    func_00000000(*(unsigned char*)(GAMEFLOW(a0)->unlocks + 0x14));
}
