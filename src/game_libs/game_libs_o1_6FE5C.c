/* IDO -O1 donor for __osInsertTimer = libultra __osInsertTimer
 * (references/libreultra/src/os/timerintr.c, verbatim: walk
 * __osTimerList->next accumulating tim -= timep->value until
 * timep == __osTimerList || tim <= timep->value; store t->value,
 * decrement the successor, doubly-linked insert before timep, under
 * __osDisableInt/__osRestoreInt). PROVEN EXACT 98/98 standalone at
 * BOTH IDO 7.1 -O1 and 5.3 -O1 (identical output; built with 7.1).
 * Retires the old "64-bit free-list / heap-block search-and-split"
 * -O2 NM decode: the 64-bit sltu-chain compares are u64 `tim >
 * timep->value` at -O1, not heap sizes.
 * Blank externs: _disint = __osDisableInt, _resint = __osRestoreInt;
 * gl___osTimerList_timerlist = __osTimerList (OSTimer*, next+0 prev+4
 * interval+8 value+0x10).
 * Spliced into game_libs_post1b2c.c.o via REPLACE_FUNC_BODY. */
#include "libultra/os_timer.h"
#include "uso_call.h"

extern u32 gl_func_00000000_disint(void);   /* __osDisableInt */
extern void gl_func_00000000_resint(u32);   /* __osRestoreInt */
extern OSTimer *gl___osTimerList_timerlist;       /* __osTimerList */

OSTime __osInsertTimer(OSTimer *t)
{
    OSTimer *timep;
    OSTime tim;
    u32 savedMask;
    savedMask = gl_func_00000000_disint(); /* UNNAMED: mid-entry jal gl___osDisableInt+0x4 (__osDisableInt, entry 4 bytes into the build function, no symbol) */
    for (timep = gl___osTimerList_timerlist->next, tim = t->value;
         timep != gl___osTimerList_timerlist && tim > timep->value;
         tim -= timep->value, timep = timep->next)
    {
        ;
    }
    t->value = tim;
    if (timep != gl___osTimerList_timerlist)
        timep->value -= tim;
    t->next = timep;
    t->prev = timep->prev;
    timep->prev->next = t;
    timep->prev = t;
    XCALL(gl_func_00000000_resint, gl___osRestoreInt)(savedMask);
    return tim;
}
