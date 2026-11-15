#ifndef LIBULTRA_OS_MESSAGE_H
#define LIBULTRA_OS_MESSAGE_H

/* libultra message queues (2.0I PR/os.h, references/libreultra), shared by
 * the kernel and game_libs copies. Opt-in like os_thread.h: TUs that still
 * carry a private OSMesg / OSMesgQueue typedef must drop it before including
 * this (duplicate typedef = cfe error). Offsets and size are asserted below,
 * so a wrong field is a compile error, not a silent byte change. */

#include "libultra/os_thread.h"

typedef u32 OSEvent;
typedef void *OSMesg;

typedef struct OSMesgQueue_s {
    /* 0x00 */ OSThread *mtqueue;   /* threads blocked on an empty queue */
    /* 0x04 */ OSThread *fullqueue; /* threads blocked on a full queue */
    /* 0x08 */ s32 validCount;
    /* 0x0C */ s32 first;
    /* 0x10 */ s32 msgCount;
    /* 0x14 */ OSMesg *msg;
} OSMesgQueue; /* size 0x18 */

#define OS_MESG_NOBLOCK 0
#define OS_MESG_BLOCK 1

/* compile-time offset checks (array size -1 => error) */
#define OS_MESG_ASSERT_OFFSET(field, off) \
    typedef char os_mesg_assert_##off[(int)&((OSMesgQueue *)0)->field == (off) ? 1 : -1]
OS_MESG_ASSERT_OFFSET(fullqueue, 0x04);
OS_MESG_ASSERT_OFFSET(validCount, 0x08);
OS_MESG_ASSERT_OFFSET(first, 0x0C);
OS_MESG_ASSERT_OFFSET(msgCount, 0x10);
OS_MESG_ASSERT_OFFSET(msg, 0x14);
typedef char os_mesg_assert_size[sizeof(OSMesgQueue) == 0x18 ? 1 : -1];

#endif /* LIBULTRA_OS_MESSAGE_H */
