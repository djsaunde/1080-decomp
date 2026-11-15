#include "common.h"
#include "game/input.h"
#include "uso_call.h"

extern int func_00000000();

/* Sibling in the 0x11A34 O0 cluster. Clears the small object lists/counts,
 * probes a0->0xC0 twice, and initializes each live child slot. */
void GameFlow_addPads_00011AB4(char *a0) {
    s32 i;

    GAMEFLOW(a0)->numPads = 0;
    GAMEFLOW(a0)->numRacePads = 0;
    GAMEFLOW(a0)->activePad = -1;
    i = 0;
    do {
        if (FCALL(pad_isConnected)(GAMEFLOW(a0)->ctrls[0]) == 0) {
            GAMEFLOW(a0)->locked = 1;
        } else {
            FCALL(input_addPad_00011C70)(a0, *(s32*)(a0 + 0xC0 + (i * 4)));
        }
    } while (++i < 2);
}
