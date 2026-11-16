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
#define TV_ERROR_INVALID_PRIORITY     -6
#define TV_ERROR_INVALID_FLAGS        -7
#define TV_ERROR_INVALID_CAPABILITY   -8

/* rmon methods / ids (ultralib include/PRinternal/rmonint.h). Checked against the
 * kernel: __rmonGetThreadStatus stores 0x3E9 for method 1 (RSP) and __rmonListThreads
 * answers object -1 with 0x3EA (CPU), so CPU = 1002, RSP = 1001. */
#define RMON_CPU 0
#define RMON_RSP 1
#define RMON_TID_RSP 1000
#define RMON_PRI_RSP 42
#define RMON_PID_CPU 1002
#define RMON_PID_RSP 1001

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

/* __rmonReadMem request (reply: a KKBufferEvent in __rmonUtilityBuffer) */
typedef struct KKReadRequest {
    /* 0x00 */ KKHeader header;
    /* 0x0C */ TVid object;
    /* 0x10 */ u32 addr;
    /* 0x14 */ u32 nbytes;
} KKReadRequest; /* size 0x18 */

typedef struct KKSetBkptRequest {     /* ultralib rmon: set-breakpoint request */
    /* 0x00 */ KKHeader header;
    /* 0x0C */ TVid object;
    /* 0x10 */ u32 addr;
    /* 0x14 */ u32 nbytes;
} KKSetBkptRequest; /* size 0x18 */

typedef struct KKBkptEvent {          /* ultralib rmon: set-breakpoint reply */
    /* 0x00 */ KKHeader header;
    /* 0x0C */ TVid object;
    /* 0x10 */ s32 bp;                 /* slot index in the breakpoint table */
    /* 0x14 */ u32 instruction;        /* the word the BREAK replaced */
} KKBkptEvent; /* size 0x18 */

typedef struct KKWriteHeader {
    /* 0x00 */ KKHeader header;
    /* 0x0C */ TVid object;
    /* 0x10 */ u32 addr;
    /* 0x14 */ u32 nbytes;
} KKWriteHeader; /* size 0x18 */

/* __rmonWriteMem request: the header, then nbytes of data */
typedef struct KKWriteRequest {
    /* 0x00 */ KKWriteHeader writeHeader;
    /* 0x18 */ u8 buffer[1];
} KKWriteRequest;

typedef struct KKBufferEvent {
    /* 0x00 */ KKHeader header;
    /* 0x0C */ TVid object;
    /* 0x10 */ u8 buffer[1];
} KKBufferEvent;

#define KK_RUN_SSTEP (1 << 0)
#define KK_RUN_SETPC (1 << 1)

typedef struct KKRun {
    /* 0x00 */ s32 flags;   /* KK_RUN_* */
    /* 0x04 */ u32 vaddr;   /* new PC when KK_RUN_SETPC */
} KKRun; /* size 0x08 */

/* __rmonRunThread request */
typedef struct KKRunThreadRequest {
    /* 0x00 */ KKHeader header;
    /* 0x0C */ TVid tid;
    /* 0x10 */ KKRun actions;
} KKRunThreadRequest; /* size 0x18 */

typedef struct KKRegion {
    /* 0x00 */ u32 vaddr;
    /* 0x04 */ u32 size;
    /* 0x08 */ s16 flags;   /* 1 read, 2 write, 4 execute */
    /* 0x0C */ u32 paddr;
} KKRegion; /* size 0x10 */

/* __rmonGetRegions reply: length = number * sizeof(KKRegion) + sizeof(KKRegionEvent) */
typedef struct KKRegionEvent {
    /* 0x00 */ KKHeader header;
    /* 0x0C */ TVid object;
    /* 0x10 */ u32 number;
    /* 0x14 */ KKRegion regions[1];
} KKRegionEvent; /* size 0x24 */

/* __rmonGetRegionCount reply */
typedef struct KKNumberEvent {
    /* 0x00 */ KKHeader header;
    /* 0x0C */ TVid object;
    /* 0x10 */ s32 number;
} KKNumberEvent; /* size 0x14 */

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

RMON_ASSERT(write_buffer, (int)&((KKWriteRequest *)0)->buffer[0] == 0x18);
RMON_ASSERT(bufev_buffer, (int)&((KKBufferEvent *)0)->buffer[0] == 0x10);
RMON_ASSERT(runreq_vaddr, (int)&((KKRunThreadRequest *)0)->actions.vaddr == 0x14);
RMON_ASSERT(region_size, sizeof(KKRegion) == 0x10);
RMON_ASSERT(regev_size, sizeof(KKRegionEvent) == 0x24);
RMON_ASSERT(status_why, (int)&((KKStatusEvent *)0)->status.why == 0x10);
RMON_ASSERT(status_size, sizeof(KKStatusEvent) == 0x4C);

#endif /* LIBULTRA_RMON_H */
