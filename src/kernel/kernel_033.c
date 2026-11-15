#include "common.h"

extern s32 __rmonUtilityBuffer[];
extern s32 D_8000A828;
extern s32 D_8000A830;
extern void func_80006B54(void*, void*);
extern void func_800073F8(void*, s32, s32);

typedef struct { s32 pad0; u8 type; char pad5[4]; u8 domain; char padA[2]; s32 id; } RmonMsg;

/* __rmonGetExeName (rmonmem.c): rmon dispatchTable[2] */
s32 __rmonGetExeName(RmonMsg* msg) {
    RmonMsg* p = msg;
    s32* buf = (s32*)__rmonUtilityBuffer;
    *(u8*)((char*)buf + 4) = p->type;
    *(s16*)((char*)buf + 6) = 0;
    *(s32*)((char*)buf + 0xC) = p->id;
    if (msg->domain == 1) {
        func_80006B54((char*)buf + 0x10, &D_8000A828);
    } else {
        func_80006B54((char*)buf + 0x10, &D_8000A830);
    }
    func_800073F8(buf, 0x18, 1);
    return 0;
}

