#ifndef LIBULTRA_OS_EVENT_H
#define LIBULTRA_OS_EVENT_H

/* libultra __osEventStateTab entry (osint.h, references/libreultra): the queue
 * + message an event posts (osSetEventMesg). Opt-in, separate from
 * os_message.h because game_libs_post1b.c and kernel_000_b.c still carry a
 * private __OSEventState typedef (duplicate typedef = cfe error). */

#include "libultra/os_message.h"

typedef struct {
    /* 0x00 */ OSMesgQueue *messageQueue;
    /* 0x04 */ OSMesg message;
} __OSEventState; /* size 0x08 */

#define OS_EVENT_PI 8

#ifndef __cplusplus
typedef char os_event_assert_message[(int)&((__OSEventState *)0)->message == 4 ? 1 : -1];
typedef char os_event_assert_size[sizeof(__OSEventState) == 8 ? 1 : -1];
#endif

#endif /* LIBULTRA_OS_EVENT_H */
