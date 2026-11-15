/* __osSetGlobalIntMask — compiled at -O1, matching C source */
#include "common.h"

/* __osSetGlobalIntMask */
void __osSetGlobalIntMask(s32 mask) {
    register s32 sr = func_800066B0();
    __OSGlobalIntMask |= mask;
    func_800066D0(sr);
}
