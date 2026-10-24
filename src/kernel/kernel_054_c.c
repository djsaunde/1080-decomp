#include "common.h"

extern void* func_800077DC(s32 id);
extern void func_80006B20(void* dst, void* src, s32 n);
extern void func_800073F8(void* hdr, s32 size, s32 flags);
extern void func_80007A98(s32 a, s32 b, void* buf);
extern void func_80008D0C(void* buf);

typedef struct { s32 pad0; u8 type; char pad5; s16 flags; s32 data[17]; } RmonBuf76;

typedef struct {
    s32  pad_00;
    u8   type;
    char pad_05;
    s16  flags;
    char pad_08[0x14];
    s32  field_1C;
    char pad_20[4];
    s16  field_24;
    s16  field_26;
    char pad_28[0x24];
} RmonBuf76B;

typedef struct {
    char pad_00[4];
    u8   field_04;
    char pad_05[4];
    u8   field_09;
    char pad_0A[2];
    s32  field_0C;
    char pad_10[0x80];
    s32  field_90;
} RmonMsg91FC;

typedef struct {
    char pad_00[4];
    u8   type;
    char pad_05;
    s16  flags;
    char pad_08[4];
    s32  field_0C;
} RmonHdr16;

typedef struct {
    char pad_00[4];
    u8   type;
    char pad_05;
    s16  flags;
    char pad_08[4];
    s32  field_0C;
    char pad_10[0x80];
    s32  field_90;
} RmonPkt98;

/* split from kernel_054.c - 2026-06-10 kernel ROM-order relayout */

extern s32 func_80008430();
extern void func_800090B4(s32, s32);
extern void func_80008498(void);
extern void func_800091F0(s32);
extern void func_80006A50(u32 addr, u32 val);
extern void func_80009030(s32, s32);

/* func_80009474 -- EXACT (agent-g run 145) via the IDO 5.3 -O1 donor
 * src/kernel/kernel_054_c_ido53_9474.c; this definition is only the splice
 * placeholder. The long-standing "andi into pre-coloured a1" residual
 * (target `lw a1; andi t8,a1,0xfff; or a1,t8`) is simply IDO 5.3: the mask
 * written inline in the call argument compiles to the target under 5.3 -O1
 * (7.1 folds it into `andi a1,a1`). */
s32 func_80009474(s32* volatile msg) {
    return 0;
}
