#include "common.h"

typedef struct Thread {
    s32 field0;
    s32 pri;
    s32 queue;
    s32 pad0C;
    u16 state;
} Thread;

extern void* __osRunQueue;
extern void* __osRunningThread;
extern s32 func_800066B0(void);
extern void func_800066D0(s32);
extern void func_80003E0C(void*, void*);
extern void* func_80003E54(void*);
extern void func_80003E64(void);
extern void func_80003D0C(void*);

/* osStartThread */
void osStartThread(Thread* t) {
    register s32 saveMask = func_800066B0();
    switch (t->state) {
    case 8: /* OS_STATE_WAITING */
        t->state = 2;
        func_80003E0C(&__osRunQueue, t);
        break;
    case 1: /* OS_STATE_STOPPED */
        if (t->queue == 0 || t->queue == (s32)&__osRunQueue) {
            t->state = 2;
            func_80003E0C(&__osRunQueue, t);
        } else {
            t->state = 8;
            func_80003E0C((void*)t->queue, t);
            func_80003E0C(&__osRunQueue, func_80003E54((void*)t->queue));
        }
        break;
    }
    if ((s32)__osRunningThread == 0) {
        func_80003E64();
    } else {
        if (((Thread*)__osRunningThread)->pri < ((Thread*)__osRunQueue)->pri) {
            ((Thread*)__osRunningThread)->state = 2;
            func_80003D0C(&__osRunQueue);
        }
    }
    func_800066D0(saveMask);
}

/* func_80009E50 (4-insn `lw s0,0x14(sp); lw s1,0x18(sp); jr ra; addiu sp,+0x28`)
 * merged into predecessor func_80009DF0 (size 0x60 → 0x70). func_80009E50 was
 * the natural shared-tail epilogue. Address re-exported via undefined_syms_auto.txt
 * for cross-jal callers (func_80005F10 etc.). */
