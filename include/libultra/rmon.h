#ifndef LIBULTRA_RMON_H
#define LIBULTRA_RMON_H

/* rmon / rdb host-protocol packets (ultralib include/PRinternal/dbgproto.h,
 * references/ultralib). The kernel links the debug libultra's rmon; every
 * dispatchTable command (rmon_dispatchTable, rmoncmds.c) takes a request that
 * starts with a KKHeader and answers with __rmonSendReply(reply, size, type).
 *
 * ultralib spells the byte fields `char`; IDO's char is unsigned, so u8 here
 * is the same type (lbu/sb). Opt-in header: older kernel TUs still carry
 * private per-function packet typedefs (RmonMsg, RmonAck, ...), so do NOT add
 * it to common.h until they are migrated. Offsets and sizes are asserted
 * below (array size -1 => compile error). */

#include "common.h"

typedef s32 TVid;
typedef u16 TVushort;

#define KK_REV 2
#define KK_CODE_THREAD_STATUS 4

#ifndef OS_STATE_STOPPED
#define OS_STATE_STOPPED 1
#endif

#define KK_TYPE_REQUEST      0
#define KK_TYPE_REPLY        1
#define KK_TYPE_EXCEPTION    2
#define KK_TYPE_THREAD_EXIT  3
#define KK_TYPE_PROCESS_EXIT 4
#define KK_TYPE_CONSOLE      5

#define TV_ERROR_NO_ERROR             0
#define TV_ERROR_ILLEGAL_CALL         -1
#define TV_ERROR_INVALID_ID           -2
#define TV_ERROR_INVALID_TYPE         -3
#define TV_ERROR_OPERATIONS_PROTECTED -4
#define TV_ERROR_INVALID_ADDRESS      -5

/* rmon process ids (rmonint.h) */
#define RMON_PID_CPU 1001
#define RMON_PID_RSP 1002

typedef struct KKHeader {
    /* 0x00 */ s32 length;
    /* 0x04 */ u8 code;     /* request: dispatchTable index */
    /* 0x05 */ u8 type;     /* KK_TYPE_* (set by __rmonSendHeader) */
    /* 0x06 */ s16 error;   /* TV_ERROR_* */
    /* 0x08 */ u8 rev;      /* KK_REV (set by __rmonSendHeader) */
    /* 0x09 */ u8 method;   /* 0 = CPU, 1 = RSP */
    /* 0x0A */ s16 notused2;
} KKHeader; /* size 0x0C */

typedef struct KKObjs {
    /* 0x00 */ TVushort number;
    /* 0x04 */ TVid objects[1];
} KKObjs; /* size 0x08 */

/* header + one object id (thread / process) */
typedef struct KKObjectRequest {
    /* 0x00 */ KKHeader header;
    /* 0x0C */ TVid object;
} KKObjectRequest; /* size 0x10 */

typedef KKObjectRequest KKObjectEvent;

typedef struct KKObjsEvent {
    /* 0x00 */ KKHeader header;
    /* 0x0C */ TVid object;
    /* 0x10 */ KKObjs objs;
} KKObjsEvent; /* size 0x18 */

typedef struct KKFaultInfo {
    /* 0x00 */ u32 addr;
    /* 0x04 */ s16 major;
    /* 0x06 */ s16 minor;
} KKFaultInfo; /* size 0x08 */

typedef struct KKThreadStatus {
    /* 0x00 */ s32 flags;
    /* 0x04 */ s16 why;
    /* 0x06 */ s16 what;
    /* 0x08 */ TVid tid;
    /* 0x0C */ TVid pid;
    /* 0x10 */ u32 instr;
    /* 0x14 */ KKFaultInfo info;
    /* 0x1C */ s32 priority;
    /* 0x20 */ s32 start;
    /* 0x24 */ s32 rv;
    /* 0x28 */ u32 args[6];
} KKThreadStatus; /* size 0x40 */

/* __rmonGetThreadStatus fills one; ThreadStatus / StopThread / RunThread send it */
typedef struct KKStatusEvent {
    /* 0x00 */ KKHeader header;
    /* 0x0C */ KKThreadStatus status;
} KKStatusEvent; /* size 0x4C */

#define RMON_ASSERT(name, cond) typedef char rmon_assert_##name[(cond) ? 1 : -1]
RMON_ASSERT(hdr_code, (int)&((KKHeader *)0)->code == 0x04);
RMON_ASSERT(hdr_error, (int)&((KKHeader *)0)->error == 0x06);
RMON_ASSERT(hdr_rev, (int)&((KKHeader *)0)->rev == 0x08);
RMON_ASSERT(hdr_method, (int)&((KKHeader *)0)->method == 0x09);
RMON_ASSERT(hdr_size, sizeof(KKHeader) == 0x0C);
RMON_ASSERT(objreq_object, (int)&((KKObjectRequest *)0)->object == 0x0C);
RMON_ASSERT(objsev_number, (int)&((KKObjsEvent *)0)->objs.number == 0x10);
RMON_ASSERT(objsev_objects, (int)&((KKObjsEvent *)0)->objs.objects[0] == 0x14);
RMON_ASSERT(objsev_size, sizeof(KKObjsEvent) == 0x18);

RMON_ASSERT(status_why, (int)&((KKStatusEvent *)0)->status.why == 0x10);
RMON_ASSERT(status_size, sizeof(KKStatusEvent) == 0x4C);

#endif /* LIBULTRA_RMON_H */
