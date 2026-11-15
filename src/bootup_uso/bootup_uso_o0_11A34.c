#include "common.h"
#include "gdl/gl_data.h"
#include "game/input.h"

extern int func_00000000();
extern char D_00000000;

/* -O0 linked-list clear/free helper split out of bootup_uso_tail3a.c.
 * Walks a0->0x130, notifies with node->0, frees each node, then clears the
 * head pointer. */
void GameFlow_unloadUso_00011A34(char *a0) {
    char *cur;
    char *next;
    if ((cur = (char *)GAMEFLOW(a0)->usoList) != 0) {
        do {
            next = *(char**)(cur + 4);
            func_00000000(&gSysLib, *(int*)cur);
            func_00000000(cur);
        } while ((cur = next) != 0);
    }
    GAMEFLOW(a0)->usoList = 0;
}
