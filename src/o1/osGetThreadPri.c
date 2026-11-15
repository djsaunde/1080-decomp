/* osGetThreadPri — compiled at -O1, matching C source */
#include "common.h"

/* osGetThreadPri */
s32 osGetThreadPri(Thread* thread) {
    if (thread == 0) {
        thread = (Thread*)__osRunningThread;
    }
    return thread->pri;
}
