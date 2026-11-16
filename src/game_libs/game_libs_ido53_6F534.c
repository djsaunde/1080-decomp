#include "common.h"
#include "gdl/gl_data.h"
#include "uso_call.h"

/* gl_osSetThreadPri = libultra osSetThreadPri (sched/setthreadpri.c / thread
 * priority), statically linked into the game_libs USO. Byte-exact masked-twin
 * of the matched kernel osSetThreadPri (kernel_011_b.c) -- identical 56-insn
 * body after masking the collapsed cross-references. Compiled IDO 5.3 -O1
 * (same per-file carve recipe as the other game_libs_ido53_* libultra units),
 * spliced into game_libs_post1b2c.c.o via REPLACE_FUNC_BODY.
 *
 * Every cross-reference the USO loader fixes up at runtime is stored as a bare
 * placeholder in the ROM (jal 0 / lui 0,lw 0). All five callees collapse to the
 * address-0 placeholder gl_func_00000000; the two distinct globals (the running
 * thread ptr __osRunningThread and the run-queue head __osRunQueue) map to the two
 * distinct zero-resolving placeholders D_00000000 / gl_data_00000000 -- kept
 * separate so IDO does not CSE them (which would renumber the t-reg pair). */
extern int gl_func_00000000();
extern void *D_00000000;       /* running thread ptr placeholder */
extern void *gl_data_00000000; /* run-queue head ptr placeholder */

typedef struct Thread {
    int field0;
    int pri;
    int queue;
    int pad0C;
    unsigned short state;
} Thread;

void gl_osSetThreadPri(Thread *thread, int pri) {
    register int sr = gl_func_00000000(); /* UNNAMED: mid-entry jal gl___osDisableInt+0x4 (__osDisableInt, entry 4 bytes into the build function, no symbol) */
    if (thread == 0) {
        thread = (Thread *)gl___osRunningThread;
    }
    if (thread->pri != pri) {
        thread->pri = pri;
        if ((Thread *)(void *)thread != (Thread *)gl___osRunningThread && thread->state != 1) {
            CALL(gl___osDequeueThread)(thread->queue, thread);
            gl_func_00000000(thread->queue, thread); /* UNNAMED: mid-entry jal gl___osExceptionPreamble+0x730 (= __osEnqueueThread, no symbol of its own) */
        }
        if (((Thread *)gl___osRunningThread)->pri < ((Thread *)gl_data_00000000)->pri) {
            ((Thread *)gl___osRunningThread)->state = 2;
            gl_func_00000000(&gl_data_00000000); /* UNNAMED: mid-entry jal gl___osExceptionPreamble+0x630 (= __osEnqueueAndYield, no symbol of its own) */
        }
    }
    CALL(gl___osRestoreInt)(sr);
}
