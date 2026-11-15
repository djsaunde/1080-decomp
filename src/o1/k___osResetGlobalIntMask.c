/* k___osResetGlobalIntMask — compiled at -O1, matching C source */
#include "common.h"

/* __rmonWriteWordTo (bitwise) */
void k___osResetGlobalIntMask(s32 mask) {
    register s32 sr = func_800066B0();
    D_8000A3E0 &= ~(mask & ~0x401);
    func_800066D0(sr);
}
