#include "common.h"

extern void func_800073F8(void*, s32, s32);

typedef struct { s32 pad0; u8 type; char pad5; u16 flags; s32 pad8; s32 id; s32 count; } RmonHdr;
typedef struct { s32 pad0; u8 type; char pad5[4]; u8 domain; char padA[2]; s32 id; } RmonMsg;

/* __rmonWriteMem */
s32 __rmonGetRegionCount(RmonMsg* msg) {
    RmonMsg* p = msg;
    RmonHdr hdr;
    hdr.type = p->type;
    hdr.flags = 0;
    hdr.id = p->id;
    if (p->domain == 1) {
        hdr.count = 2;
    } else {
        hdr.count = 5;
    }
    func_800073F8(&hdr, 0x14, 1);
    return 0;
}

/* __rmonListRegisters — build register descriptor packet */
extern s32 __rmonUtilityBuffer;

s32 __rmonGetRegions(char* msg) {
    char* p = msg;
    char* buf = (char*)&__rmonUtilityBuffer;
    s32 count;

    if (*(u8*)(p + 9) == 1) {
        count = 2;
    } else {
        count = 6;
    }
    *(s32*)(buf + 0x00) = count * 16 + 0x24;
    *(u8*)(buf + 0x04) = *(u8*)(p + 4);
    *(u16*)(buf + 0x06) = 0;
    *(s32*)(buf + 0x0C) = *(s32*)(p + 0xC);
    *(s32*)(buf + 0x10) = count;
    *(s32*)(buf + 0x24) = 0x04001000;
    *(s32*)(buf + 0x28) = 0x1000;
    *(s16*)(buf + 0x2C) = 7;
    *(s32*)(buf + 0x30) = 0x04001000;
    *(s32*)(buf + 0x14) = 0x04000000;
    *(s32*)(buf + 0x18) = 0x1000;
    *(s16*)(buf + 0x1C) = 3;
    *(s32*)(buf + 0x20) = 0x04000000;
    if (count >= 3) {
        *(s32*)(buf + 0x34) = (s32)0x88200000;
        *(s32*)(buf + 0x38) = 0x6130;
        *(s16*)(buf + 0x3C) = 5;
        *(s32*)(buf + 0x40) = 0;
        *(s32*)(buf + 0x44) = 4;
        *(s32*)(buf + 0x48) = 0x200000;
        *(s16*)(buf + 0x4C) = 3;
        *(s32*)(buf + 0x50) = 0;
        *(s32*)(buf + 0x54) = 0x04002000;
        *(s32*)(buf + 0x58) = 0x800000;
        *(s16*)(buf + 0x5C) = 3;
        *(s32*)(buf + 0x60) = 0;
        *(s32*)(buf + 0x64) = (s32)0x88206130;
        *(s32*)(buf + 0x68) = 0x9000;
        *(s16*)(buf + 0x6C) = 3;
        *(s32*)(buf + 0x70) = 0;
    }
    func_800073F8(buf, *(s32*)buf, 1);
    return 0;
}
