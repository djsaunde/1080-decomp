#include "common.h"

/* __osPiCreateAccessQueue starts its own TU: kernel_006 (osCreatePiManager + the PI
 * event-pump loop) ends at 0x80005800 after IDO's 16-byte .text pad, and
 * this unit's .text starts on the next 32-byte boundary. Moved out of
 * kernel_006.c so pimgr_ramromMain's dead-epilogue alignment can match. */

extern s32 __osPiAccessQueue;
extern s32 __osPiAccessQueueEnabled;
extern s32 piacs_piAccessBuf;
extern void func_800053D0(s32*, s32*, s32);
extern void func_80005DC0(s32*, s32, s32);

/* __osSiCreateAccessQueue */
void __osPiCreateAccessQueue(void) {
    __osPiAccessQueueEnabled = 1;
    func_800053D0(&__osPiAccessQueue, &piacs_piAccessBuf, 1);
    func_80005DC0(&__osPiAccessQueue, 0, 0);
}
