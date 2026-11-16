#include "common.h"
#include "gdl/gl_data.h"
#include "uso_call.h"
extern int D_00000000;
typedef struct { int a, b, c, d; } Quad4;
typedef struct { float x, y, z; } Vec3;


/* gl_osStartThread = libultra osStartThread. LANDED 2026-06-21 as a byte-identical
 * TWIN-PORT of matched kernel osStartThread (kernel_037): move the thread to
 * RUNNABLE and (re)enqueue it per its current state, then preempt if it now
 * outranks the running thread. The 2026-06-10 cap analysis (44/84, frame/reload
 * residuals) was an in-context approximation; the verbatim kernel twin is exact.
 * Real C lives in the donor unit game_libs_ido53_74C04.c (IDO 5.3 -O1), spliced
 * via REPLACE_FUNC_BODY. OS-API callees -> gl_func_00000000; the two distinct
 * globals (run queue -> D_00000000, running ptr -> gl_data_00000000) kept
 * separate so IDO does not CSE them. */
extern int gl_func_00000000();
extern int gl_data_00000000;
typedef struct { int field0; int pri; int queue; int pad0C; unsigned short state; } Thread_74C04;
void gl_osStartThread(Thread_74C04 *t) {
    /* UNNAMED: mid-entry gl___osDisableInt+0x4 (libultra __osDisableInt entry, one insn into the build symbol) */
    register int saveMask = gl_func_00000000();
    switch (t->state) {
    case 8:
        t->state = 2;
        /* UNNAMED: mid-entry gl___osExceptionPreamble+0x730 (libultra __osEnqueueThread inside the exceptasm block) */
        gl_func_00000000(&gl___osRunQueue, t);
        break;
    case 1:
        if (t->queue == 0 || t->queue == (int)&gl___osRunQueue) {
            t->state = 2;
            /* UNNAMED: mid-entry gl___osExceptionPreamble+0x730 (libultra __osEnqueueThread inside the exceptasm block) */
            gl_func_00000000(&gl___osRunQueue, t);
        } else {
            t->state = 8;
            /* UNNAMED: mid-entry gl___osExceptionPreamble+0x730 (libultra __osEnqueueThread inside the exceptasm block) */
            gl_func_00000000((void *)t->queue, t);
            /* UNNAMED: mid-entry gl___osExceptionPreamble+0x730 (libultra __osEnqueueThread inside the exceptasm block) */
            gl_func_00000000(&gl___osRunQueue, CALL(gl___osPopThread)((void *)t->queue));
        }
        break;
    }
    if ((int)gl_data_00000000 == 0) {
        CALL(gl___osDispatchThread)();
    } else {
        if (((Thread_74C04 *)gl_data_00000000)->pri < ((Thread_74C04 *)gl___osRunQueue)->pri) {
            ((Thread_74C04 *)gl_data_00000000)->state = 2;
            /* UNNAMED: mid-entry gl___osExceptionPreamble+0x630 (libultra __osEnqueueAndYield inside the exceptasm block) */
            gl_func_00000000(&gl___osRunQueue);
        }
    }
    CALL(gl___osRestoreInt)(saveMask);
}
