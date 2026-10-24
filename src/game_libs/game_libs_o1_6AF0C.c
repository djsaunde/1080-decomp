#include "common.h"

/* -O1 donor for gl_func_0006AF0C = libultra __osDequeueThread (os/thread.c).
 * The plain libultra source at IDO 7.1 -O1 is byte-exact (16 insns: the two
 * loads scheduled ahead of the register locals' empty 8-byte frame). post1b
 * builds -O2, so this is spliced into game_libs_post1b.c.o via
 * REPLACE_FUNC_BODY (real compiler output). Boundary fixed 2026-10-24
 * (agent-g run 158): the first two instructions used to sit at the end of
 * game_libs_func_0006AD78. */
typedef struct GlDqThread_s { struct GlDqThread_s *next; } GlDqThread;

void gl_func_0006AF0C(GlDqThread **queue, GlDqThread *t) {
    register GlDqThread *pred;
    register GlDqThread *succ;

    pred = (GlDqThread *)queue;
    succ = pred->next;
    while (succ != 0) {
        if (succ == t) {
            pred->next = t->next;
            return;
        }
        pred = succ;
        succ = pred->next;
    }
}
