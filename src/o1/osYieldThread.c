/* osYieldThread — compiled at -O1, matching C source */
#include "common.h"

/* osYieldThread */
extern void* __osRunningThread;
extern s32 __osRunQueue;
extern void func_80003D0C(s32*);

void osYieldThread(void) {
    register s32 sr = func_800066B0();
    *(s16*)((char*)__osRunningThread + 0x10) = 2;
    func_80003D0C(&__osRunQueue);
    func_800066D0(sr);
}
