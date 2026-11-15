#include "common.h"

extern void func_800073F8(void*, s32, s32);

typedef struct { s32 pad0; u8 type; char pad5; u16 flags; s32 pad8; s32 pad12; } RmonAck;
typedef struct { s32 pad0; u8 type; } RmonMsgSmall;

/* __rmonSetComm (rmonmisc.c): rmon dispatchTable[30] */
s32 __rmonSetComm(RmonMsgSmall* msg) {
    RmonAck ack;
    ack.type = msg->type;
    ack.pad12 = 0;
    ack.flags = 0;
    func_800073F8(&ack, 0x10, 1);
    return 0;
}

/* __rmonRCPrunning moved to kernel_031b.c (file-split for -O2 OPT_FLAGS).
 * INSN_PATCH applied on non-reloc offsets only, per
 * feedback_insn_patch_on_reloc_instructions_breaks_byte_verify.md. */
