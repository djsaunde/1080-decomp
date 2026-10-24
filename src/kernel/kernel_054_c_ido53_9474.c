/* IDO 5.3 -O1 donor for func_80009474 (0x110 / 68 words): rmon RSP state
 * restore -- fail with -4 while the RCP runs (func_80008430), then write the
 * 32 saved words through func_80006A50 (__rmonWriteWordTo) with a per-word
 * step (func_80009030 / func_80008498), SP_DRAM_ADDR / SP_MEM_ADDR, the PC
 * masked to 12 bits, SP_STATUS fields, and reply with a 16-byte header
 * (func_800073F8).
 *
 * The "andi into a pre-coloured a1" cap (target `lw a1; andi t8,a1,0xfff;
 * or a1,t8`) is IDO 5.3: with the mask written inline in the call argument,
 * 5.3 -O1 emits exactly that temp copy (7.1 folds it into `andi a1,a1`).
 * Same compiler as the other rmon donors (func_8000858C). Kept out of
 * C_FILES (Makefile). */
#include "common.h"

typedef struct {
    char pad_00[4];
    u8   type;
    char pad_05;
    s16  flags;
    char pad_08[4];
    s32  field_0C;
} RmonHdr16;

extern s32 func_80008430();
extern void func_80009148(void *);
extern void func_80008498(void);
extern void func_800091F0(s32);
extern void func_80006A50(u32 addr, u32 val);
extern void func_80009030(s32, s32);
extern void func_800073F8(void *hdr, s32 size, s32 flags);

s32 func_80009474(s32 *volatile msg) {
    register s32 *p;
    RmonHdr16 hdr;
    register s32 i;

    p = msg;
    if (func_80008430() != 0) {
        return -4;
    }
    func_80009148(0);
    for (i = 0; i < 0x20; i++) {
        func_80006A50(0x04000000, ((u32 *)p)[4 + i]);
        func_80009030(0x23, i);
        func_80008498();
    }
    func_800091F0(0);
    func_80006A50(0x04040004, ((u32 *)p)[0x24]);
    func_80006A50(0x04040000, ((u32 *)p)[0x25]);
    func_80006A50(0x04080000, ((u32 *)p)[0x27] & 0xFFF);
    func_80006A50(0x0404000C, ((u32 *)p)[0x28]);
    func_80006A50(0x04040010, ((u32 *)p)[0x29]);
    hdr.field_0C = p[3];
    hdr.type = ((u8 *)p)[4];
    hdr.flags = 0;
    func_800073F8(&hdr, 0x10, 1);
    return 0;
}
