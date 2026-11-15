#include "common.h"
#include "libultra/rmon.h"

/* Forward declarations */
extern s32 __osPiAccessQueue;
extern s32 __osPiAccessQueueEnabled;
extern s32 gUsoErrno;
extern void func_80005DC0(s32*, s32, s32);
extern void func_80005C00(void);
extern void func_80004FE0(s32*, s32*, s32);
extern void func_800053D0(s32*, s32*, s32);
extern s32 __OSGlobalIntMask;
extern s32 func_800066B0(void);
extern void func_800066D0(s32);
extern void func_80003D0C(s32*);
extern void* __osRunningThread;
extern s32 __osRunQueue;
extern s32 piacs_piAccessBuf;
extern s32 func_800009D8(void*, s32, s32, void*);
extern void func_80003FF0(s32, void*);
extern void* func_80003E54(void*);
extern void func_8000A110(void*);
extern void func_8000857C(void);
extern void func_8000785C(s32);
extern void func_80008ED0(void);
extern void func_80007760(void);
typedef void (*FuncPtr2)(void*, void*);
extern FuncPtr2 gUsoPrintfHook;
extern s32 D_8000A32C;
extern s32 D_8000A340;
extern s32 gUsoCtx;
extern s32 gFsDeviceTable;
extern s32 __osActiveQueue;
extern s32 func_80002890(s32);
extern void func_8000A0E0(void);
extern void func_80005350(s32, s32);
extern void func_80005400(s32, s32);
extern s32 func_80008430(s32);
extern void func_80009148(s32);
extern void func_80009030(s32, s32);
extern void func_80008498(void);
extern s32 func_80006A98(s32);
extern void func_800091F0(s32);
typedef struct { s32 pad[3]; s32 position; } FileState;

typedef struct {
    void* mtqueue;
    void* fullqueue;
    s32 validCount;
    s32 first;
    s32 msgCount;
    s32** msg;
} OSMesgQueue;

typedef struct Thread {
    s32 field0;
    s32 pri;
    s32 queue;
    s32 pad0C;
    u16 state;
} Thread;

typedef struct { s32 queue; s32 msg; } OSEventState;
extern OSEventState __osEventStateTab[];


/* rmoncmds.c static NotImplemented: the dispatchTable filler, returns TV_ERROR_ILLEGAL_CALL */
s32 rmon_NotImplemented(s32 arg0) {
    return -1;
}


/* __rmonExecute + func_800065F0 (cross-file fragment merge from kernel_016.c).
 * Combined size 0x90 (36 insns). Function-pointer dispatch with bounds
 * check + error-packet send on negative result.
 *
 * Cross-file unblock per
 * feedback_cross_file_fragment_unblock_via_move_then_merge.md:
 * 800065F0 INCLUDE_ASM moved here from kernel_016.c step 1; this commit
 * is steps 2+3 (same-file merge into single C body at -O1).
 * 80006640 cross-function shared-tail entry preserved via
 * undefined_syms_auto.txt. */
extern s32 (*rmon_dispatchTable[])(KKHeader *);
extern void func_800073F8(void *buf, s32 size, s32 flag);
/* func_800073F8 = __rmonSendReply (+0x400 call alias) */
s32 __rmonExecute(KKHeader *req) {
    s32 v0;
    KKHeader reply;
    if (req->code >= 0x35) {
        return TV_ERROR_ILLEGAL_CALL;
    }
    v0 = rmon_dispatchTable[req->code](req);
    if (v0 < TV_ERROR_NO_ERROR) {
        reply.code = req->code;
        reply.error = (s16)v0;
        func_800073F8(&reply, sizeof(KKHeader), KK_TYPE_REPLY);
    }
    return v0;
}
