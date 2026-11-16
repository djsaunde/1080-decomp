#ifndef LIBULTRA_OS_CONT_H
#define LIBULTRA_OS_CONT_H

/* libultra controller + controller pak types (2.0I PR/os.h, references/libreultra),
 * shared by the kernel and game_libs copies. Opt-in: drop a TU's private
 * OSContStatus / OSContPad / OSPfs typedef before including this. Offsets and
 * sizes are asserted below. */

#include "libultra/os_message.h"

typedef struct {
    /* 0x00 */ u16 type;
    /* 0x02 */ u8 status;
    /* 0x03 */ u8 errno;
} OSContStatus; /* size 0x4 */

typedef struct {
    /* 0x00 */ u16 button;
    /* 0x02 */ s8 stick_x;
    /* 0x03 */ s8 stick_y;
    /* 0x04 */ u8 errno;
} OSContPad; /* size 0x6 */

typedef struct {
    /* 0x00 */ s32 status;
    /* 0x04 */ OSMesgQueue *queue;
    /* 0x08 */ s32 channel;
    /* 0x0C */ u8 id[32];
    /* 0x2C */ u8 label[32];
    /* 0x4C */ s32 version;
    /* 0x50 */ s32 dir_size;
    /* 0x54 */ s32 inode_table;
    /* 0x58 */ s32 minode_table;
    /* 0x5C */ s32 dir_table;
    /* 0x60 */ s32 inode_start_page;
    /* 0x64 */ u8 banks;
    /* 0x65 */ u8 activebank;
} OSPfs; /* size 0x68 */

/* one controller's 8-byte read record in __osContPifRam (libultra controller.h, internal):
 * osContStartReadData builds them, osContGetReadData parses them (agent-b run 591) */
typedef struct {
    /* 0x00 */ u8 dummy;
    /* 0x01 */ u8 txsize;
    /* 0x02 */ u8 rxsize;
    /* 0x03 */ u8 cmd;
    /* 0x04 */ u16 button;
    /* 0x06 */ s8 stick_x;
    /* 0x07 */ s8 stick_y;
} __OSContReadFormat; /* size 0x8 */

#define OS_CONT_ASSERT(name, cond) typedef char os_cont_assert_##name[(cond) ? 1 : -1]
OS_CONT_ASSERT(readfmt_button, (int)&((__OSContReadFormat *)0)->button == 0x4);
OS_CONT_ASSERT(readfmt_size, sizeof(__OSContReadFormat) == 0x8);
OS_CONT_ASSERT(status_size, sizeof(OSContStatus) == 0x4);
OS_CONT_ASSERT(pad_errno, (int)&((OSContPad *)0)->errno == 0x4);
OS_CONT_ASSERT(pad_size, sizeof(OSContPad) == 0x6);
OS_CONT_ASSERT(pfs_label, (int)&((OSPfs *)0)->label == 0x2C);
OS_CONT_ASSERT(pfs_isp, (int)&((OSPfs *)0)->inode_start_page == 0x60);
OS_CONT_ASSERT(pfs_activebank, (int)&((OSPfs *)0)->activebank == 0x65);
OS_CONT_ASSERT(pfs_size, sizeof(OSPfs) == 0x68);

#endif /* LIBULTRA_OS_CONT_H */
