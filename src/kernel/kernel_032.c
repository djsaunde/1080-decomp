#include "common.h"
#include "libultra/rmon.h"

extern void func_800073F8(void*, s32, s32);


/* __rmonListProcesses (rmonmem.c): rmon dispatchTable[1] */
s32 __rmonListProcesses(KKHeader* req) {
    KKHeader* p = req;
    KKObjsEvent reply;
    reply.object = 0;
    reply.objs.number = 1;
    if (p->method == 1) {   /* 1080's 2.0I rmon: method 1 answers pid 1001 */
        reply.objs.objects[0] = 0x3E9;
    } else {
        reply.objs.objects[0] = 0x3EA;
    }
    reply.header.code = p->code;
    reply.header.error = TV_ERROR_NO_ERROR;
    func_800073F8(&reply, sizeof(KKObjsEvent), KK_TYPE_REPLY); /* __rmonSendReply */
    return 0;
}

