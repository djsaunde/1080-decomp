#include "common.h"
#include "game/input.h"
#include "uso_call.h"

extern int func_00000000();

/* Mid-tail assembly gap after GameFlow_addPads_00011AB4 was split to an -O0 file.
 * Holds 0x11B5C..0x11C70 so the following o0_11C70 split keeps its offset. */
/* EXACT via an IDO 7.1 NCC (C++) -O0 donor: src/bootup_uso/bootup_uso_ncc_11B5C.cpp,
 * REPLACE_FUNC_BODY-spliced over this placeholder (agent-c run 119). */
s32 input_pauseRacePads(s32 *a0) {
    return 0;
}

void input_unpauseRacePads(char *a0) {
    s32 i;

    i = 0;
    if (GAMEFLOW(a0)->numRacePads > 0) {
        do {
            FCALL(pad_setPause_00067470)(*(s32*)(a0 + 0x100 + i * 4), 0);
        } while (++i < GAMEFLOW(a0)->numRacePads);
    }
    GAMEFLOW(a0)->racePadsPaused = 0;
}
