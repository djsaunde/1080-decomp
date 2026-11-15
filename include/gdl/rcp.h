#ifndef GDL_RCP_H
#define GDL_RCP_H

/* libgdl RCP task manager ("RCP Mgr", game_libs 0x4D05C-0x4DCD4 in
 * src/game_libs/game_libs_post0b.c).
 *
 * RcpTask (0x78): one RSP/RDP job. RcpMgr_ctor news 16 of them. The libultra
 * OSTask sits at +0x10: the 0x4D0E4 initializer stores type 1 (M_GFXTASK),
 * ucode_boot = the 0x100-byte rspboot the ctor DMAs from cart 0xB0000B70,
 * ucode_size 0x1000, ucode_data_size 0x800, yield_data_size 0xC00; 0x4D150
 * sets data_ptr / data_size = n << 3 (Gfx words) / dram_stack_size 0x400; the
 * 0x4D16C.. setters pick a microcode (ucode / ucode_data) and the output FIFO.
 * Queued tasks form a circular doubly linked list through next / prev
 * (RcpMgr_insertTask / RcpMgr_removeTask, "RcpTask remove").
 *
 * RcpMgr (0x50): queue = pending tasks, active = the task RcpMgr_startTask
 * moved out of the queue ("RcpTask already active"). startTask marks the RSP
 * busy with name "Gfx", then the RDP with "gfx", then logs "RSP"; the busy /
 * name pairs at +0x1C/+0x3C and +0x20/+0x40 are read that way. */

#include "libultra/os_task.h"

typedef struct RcpTask {
    /* 0x00 */ s32 unk00[4];
    /* 0x10 */ OSTask_t task;
    /* 0x50 */ struct RcpTask *next;
    /* 0x54 */ struct RcpTask *prev;
    /* 0x58 */ s32 unk58;
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ s32 unk60;
    /* 0x64 */ s32 unk64;
    /* 0x68 */ s32 unk68;          /* 1 or 2, set with the output buffer */
    /* 0x6C */ s32 unk6C;
    /* 0x70 */ s32 unk70;
    /* 0x74 */ s32 unk74;
} RcpTask;

typedef struct RcpMgr {
    /* 0x00 */ s32 unk00;
    /* 0x04 */ RcpTask *queue;     /* pending tasks (head of the ring) */
    /* 0x08 */ s32 unk08;
    /* 0x0C */ RcpTask *active;    /* task being run */
    /* 0x10 */ s32 unk10;
    /* 0x14 */ RcpTask *spare;     /* extra 0x78 task newed by the ctor */
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 rspBusy;
    /* 0x20 */ s32 rdpBusy;
    /* 0x24 */ s32 unk24;
    /* 0x28 */ s32 taskCount;      /* post-incremented by RcpMgr_initTask */
    /* 0x2C */ s32 buf2C;          /* 0x400-byte buffer from the ctor */
    /* 0x30 */ s32 unk30[3];
    /* 0x3C */ char *rspTaskName;
    /* 0x40 */ char *rdpTaskName;
    /* 0x44 */ s32 unk44;
    /* 0x48 */ s32 unk48[2];
} RcpMgr;

typedef char RcpTask_size_check[sizeof(RcpTask) == 0x78 ? 1 : -1];
typedef char RcpTask_next_check[(int)&((RcpTask *)0)->next == 0x50 ? 1 : -1];
typedef char RcpMgr_size_check[sizeof(RcpMgr) == 0x50 ? 1 : -1];
typedef char RcpMgr_name_check[(int)&((RcpMgr *)0)->rspTaskName == 0x3C ? 1 : -1];

#endif /* GDL_RCP_H */
