#ifndef LIBULTRA_OS_TASK_H
#define LIBULTRA_OS_TASK_H

/* libultra RSP task descriptor (2.0I PR/sptask.h, references/libreultra).
 * Opt-in like os_thread.h / os_message.h. Only OSTask_t (no 8-byte-aligned
 * union wrapper) so it can be embedded at a 4-byte offset; offsets and size
 * are asserted below. */

#define M_GFXTASK 1
#define M_AUDTASK 2

typedef struct {
    /* 0x00 */ u32 type;
    /* 0x04 */ u32 flags;
    /* 0x08 */ u64 *ucode_boot;
    /* 0x0C */ u32 ucode_boot_size;
    /* 0x10 */ u64 *ucode;
    /* 0x14 */ u32 ucode_size;
    /* 0x18 */ u64 *ucode_data;
    /* 0x1C */ u32 ucode_data_size;
    /* 0x20 */ u64 *dram_stack;
    /* 0x24 */ u32 dram_stack_size;
    /* 0x28 */ u64 *output_buff;
    /* 0x2C */ u64 *output_buff_size;
    /* 0x30 */ u64 *data_ptr;
    /* 0x34 */ u32 data_size;
    /* 0x38 */ u64 *yield_data_ptr;
    /* 0x3C */ u32 yield_data_size;
} OSTask_t;

#ifndef __cplusplus /* edg (C++ donors) rejects the &((T *)0)->f constant form */
typedef char OSTask_t_size_check[sizeof(OSTask_t) == 0x40 ? 1 : -1];
typedef char OSTask_t_data_ptr_check[(int)&((OSTask_t *)0)->data_ptr == 0x30 ? 1 : -1];
#endif /* !__cplusplus */

#endif /* LIBULTRA_OS_TASK_H */
