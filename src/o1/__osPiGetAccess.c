/* __osPiGetAccess — compiled at -O1, matching C source */
#include "common.h"

/* __osSiCreateAccessQueue */


/* __osPiGetAccess */

void __osPiGetAccess(void) {
    s32 msg;
    if (__osPiAccessQueueEnabled == 0) {
        func_80005C00();
    }
    func_80004FE0(&__osPiAccessQueue, &msg, 1);
}
