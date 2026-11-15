#ifndef LIBULTRA_OS_TIMER_H
#define LIBULTRA_OS_TIMER_H

/* libultra timers (2.0I PR/os.h, references/libreultra), shared by the kernel
 * and game_libs copies. Opt-in: drop a TU's private OSTimer / OSTime typedef
 * before including this. Offsets and size are asserted below. */

#include "libultra/os_message.h"

typedef u64 OSTime;

typedef struct OSTimer_s {
    /* 0x00 */ struct OSTimer_s *next;
    /* 0x04 */ struct OSTimer_s *prev;
    /* 0x08 */ OSTime interval;
    /* 0x10 */ OSTime value;
    /* 0x18 */ OSMesgQueue *mq;
    /* 0x1C */ OSMesg msg;
} OSTimer; /* size 0x20 */

#define OS_TIMER_ASSERT(name, cond) typedef char os_timer_assert_##name[(cond) ? 1 : -1]
OS_TIMER_ASSERT(interval, (int)&((OSTimer *)0)->interval == 0x08);
OS_TIMER_ASSERT(value, (int)&((OSTimer *)0)->value == 0x10);
OS_TIMER_ASSERT(mq, (int)&((OSTimer *)0)->mq == 0x18);
OS_TIMER_ASSERT(size, sizeof(OSTimer) == 0x20);

#endif /* LIBULTRA_OS_TIMER_H */
