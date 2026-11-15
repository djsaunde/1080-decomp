#include "common.h"
#include "gdl/gl_data.h"
#include "uso_call.h"

/* gl_osSetEventMesg = libultra osSetEventMesg (sched/seteventmesg.c) variant,
 * statically linked into the game_libs USO. Byte-exact masked-twin of matched
 * kernel osSetEventMesg (kernel_003): disable-int, write queue+msg into the
 * event-state table entry, restore-int. IDO 5.3 -O1, spliced into
 * game_libs_post1b2c.c.o via REPLACE_FUNC_BODY. The two OS-API callees collapse
 * to the address-0 placeholder gl_func_00000000; the event-state table base
 * (kernel __osEventStateTab) maps to the zero-resolving global D_00000000. */
extern int gl_func_00000000();

typedef struct { int queue; int msg; } EventState_0006E1A4;
extern EventState_0006E1A4 D_00000000[];

void gl_osSetEventMesg(int event, void *queue, int msg) {
    register int sr = gl_func_00000000();
    EventState_0006E1A4 *es = &gl___osEventStateTab[event];
    es->queue = (int)queue;
    es->msg = msg;
    CALL(gl___osRestoreInt)(sr);
}
