/* osSetThreadPri — compiled at -O1, matching C source */
#include "common.h"

/* osSetThreadPri */


void osSetThreadPri(Thread* thread, s32 pri) {
    register s32 sr = func_800066B0();
    if (thread == 0) {
        thread = (Thread*)__osRunningThread;
    }
    if (thread->pri != pri) {
        thread->pri = pri;
        if (thread != (Thread*)__osRunningThread && thread->state != 1) {
            func_80003FF0(thread->queue, thread);
            func_80003E0C(thread->queue, thread);
        }
        if (((Thread*)__osRunningThread)->pri < ((Thread*)__osRunQueue)->pri) {
            ((Thread*)__osRunningThread)->state = 2;
            func_80003D0C(&__osRunQueue);
        }
    }
    func_800066D0(sr);
}
