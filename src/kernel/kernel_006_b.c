#include "common.h"

/* func_80005800 starts its own TU: kernel_006 (osCreatePiManager + the PI
 * event-pump loop) ends at 0x80005800 after IDO's 16-byte .text pad, and
 * this unit's .text starts on the next 32-byte boundary. Moved out of
 * kernel_006.c so func_800056EC's dead-epilogue alignment can match. */

extern s32 __osPiAccessQueue;
extern s32 D_8000A480;
extern s32 siacs_bss_0000;
extern void func_800053D0(s32*, s32*, s32);
extern void func_80005DC0(s32*, s32, s32);

/* __osSiCreateAccessQueue */
void func_80005800(void) {
    D_8000A480 = 1;
    func_800053D0(&__osPiAccessQueue, &siacs_bss_0000, 1);
    func_80005DC0(&__osPiAccessQueue, 0, 0);
}
