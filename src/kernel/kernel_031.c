#include "common.h"
#include "libultra/rmon.h"

extern void func_800073F8(void*, s32, s32);


/* __rmonSetComm (rmonmisc.c): rmon dispatchTable[30] */
s32 __rmonSetComm(KKHeader* req) {
    KKObjectEvent reply;
    reply.header.code = req->code;
    reply.object = 0;
    reply.header.error = TV_ERROR_NO_ERROR;
    func_800073F8(&reply, sizeof(KKObjectEvent), KK_TYPE_REPLY); /* __rmonSendReply */
    return 0;
}

/* __rmonRCPrunning moved to kernel_031b.c (file-split for -O2 OPT_FLAGS).
 * INSN_PATCH applied on non-reloc offsets only, per
 * feedback_insn_patch_on_reloc_instructions_breaks_byte_verify.md. */
