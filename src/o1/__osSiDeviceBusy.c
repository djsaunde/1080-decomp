/* __osSiDeviceBusy — compiled at -O1, matching C source */
#include "common.h"

/* __osSiDeviceBusy */
s32 __osSiDeviceBusy(void) {
    register u32 status = *(volatile u32*)0xA4800018;
    if (status & 3) {
        return 1;
    }
    return 0;
}
