#include "common.h"

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
extern s32 sFmt_USO_Version_8000A32C;
extern s32 sStr_3_1_8000A340;
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


/* osSendMesg */
s32 osSendMesg(OSMesgQueue* mq, s32 msg, s32 flags) {
    register s32 saveMask = func_800066B0();
    register s32 last;
    while (mq->validCount >= mq->msgCount) {
        if (flags == 1) {
            ((Thread*)__osRunningThread)->state = 8;
            func_80003D0C((s32*)&mq->fullqueue);
        } else {
            func_800066D0(saveMask);
            return -1;
        }
    }
    last = (mq->first + mq->validCount) % mq->msgCount;
    mq->msg[last] = (s32*)msg;
    mq->validCount++;
    if (*(void**)mq->mtqueue != 0) {
        register void* t = (void*)func_80003E54(mq);
        func_8000A110(t);
    }
    func_800066D0(saveMask);
    return 0;
}


