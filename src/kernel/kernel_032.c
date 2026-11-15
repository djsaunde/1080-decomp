#include "common.h"

extern void func_800073F8(void*, s32, s32);

typedef struct {
    s32 pad0; u8 type; char pad5; u16 flags;
    s32 pad8; s32 zeroed; s16 one_val; s16 pad_s; s32 code;
} RmonHdr24;
typedef struct { s32 pad0; u8 type; char pad5[4]; u8 domain; } RmonMsg2;

/* __rmonListProcesses (rmonmem.c): rmon dispatchTable[1] */
s32 __rmonListProcesses(RmonMsg2* msg) {
    RmonMsg2* p = msg;
    RmonHdr24 hdr;
    hdr.zeroed = 0;
    hdr.one_val = 1;
    if (p->domain == 1) {
        *(s32*)((char*)&hdr + 0x14) = 0x3E9;
    } else {
        *(s32*)((char*)&hdr + 0x14) = 0x3EA;
    }
    hdr.type = p->type;
    hdr.flags = 0;
    func_800073F8(&hdr, 0x18, 1);
    return 0;
}

