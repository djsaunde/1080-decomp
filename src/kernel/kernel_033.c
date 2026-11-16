#include "common.h"
#include "libultra/rmon.h"

extern s32 __rmonUtilityBuffer[];
extern s32 D_8000A828;
extern s32 D_8000A830;
extern void func_80006B54(void*, void*);
extern void func_800073F8(void*, s32, s32);

/* __rmonGetExeName (rmonmem.c): rmon dispatchTable[2] */
s32 __rmonGetExeName(KKObjectRequest* msg) {
    KKObjectRequest* p = msg;
    KKBufferEvent* buf = (KKBufferEvent*)__rmonUtilityBuffer;
    buf->header.code = p->header.code;
    buf->header.error = TV_ERROR_NO_ERROR;
    buf->object = p->object;
    if (msg->header.method == RMON_RSP) {
        func_80006B54(buf->buffer, &D_8000A828); /* strcpy "imem" */
    } else {
        func_80006B54(buf->buffer, &D_8000A830); /* strcpy "rmon" */
    }
    func_800073F8(buf, 0x18, KK_TYPE_REPLY); /* __rmonSendReply */
    return 0;
}
