#include "common.h"
#include "gdl/gl_data.h"

/* gl_osStartThread = libultra osStartThread (sched/startthread.c), statically
 * linked into the game_libs USO. Byte-exact masked-twin of matched kernel
 * osStartThread (kernel_037): move the thread to RUNNABLE and (re)enqueue it on
 * the run queue per its current state, then if it now outranks the running
 * thread, preempt. IDO 5.3 -O1, spliced into game_libs_post2b_e.c.o via
 * REPLACE_FUNC_BODY. All OS-API callees collapse to the address-0 placeholder
 * gl_func_00000000; the two distinct globals (run queue, kernel __osRunQueue, and
 * running-thread ptr, kernel __osRunningThread) map to the two distinct zero-resolving
 * placeholders D_00000000 / gl_data_00000000 (kept separate so IDO does not CSE
 * them and renumber the register pair). */
typedef struct { int field0; int pri; int queue; int pad0C; unsigned short state; } Thread_74C04;

extern int gl_func_00000000();
extern int D_00000000;        /* run queue   (kernel __osRunQueue) */
extern int gl_data_00000000;  /* running ptr  (kernel __osRunningThread) */

void gl_osStartThread(Thread_74C04 *t) {
    register int saveMask = gl_func_00000000();
    switch (t->state) {
    case 8:
        t->state = 2;
        gl_func_00000000(&gl___osRunQueue, t);
        break;
    case 1:
        if (t->queue == 0 || t->queue == (int)&gl___osRunQueue) {
            t->state = 2;
            gl_func_00000000(&gl___osRunQueue, t);
        } else {
            t->state = 8;
            gl_func_00000000((void *)t->queue, t);
            gl_func_00000000(&gl___osRunQueue, gl_func_00000000((void *)t->queue));
        }
        break;
    }
    if ((int)gl_data_00000000 == 0) {
        gl_func_00000000();
    } else {
        if (((Thread_74C04 *)gl_data_00000000)->pri < ((Thread_74C04 *)D_00000000)->pri) {
            ((Thread_74C04 *)gl_data_00000000)->state = 2;
            gl_func_00000000(&gl___osRunQueue);
        }
    }
    gl_func_00000000(saveMask);
}
