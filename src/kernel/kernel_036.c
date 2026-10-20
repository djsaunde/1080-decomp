#include "common.h"

extern void* func_80009C30(void);

typedef struct RmonThreadNode {
    s32 unk0;
    s32 state;
    s32 unk8;
    struct RmonThreadNode* next;
    s32 unk10;
    s32 threadId;
} RmonThreadNode;

/* __rmonSendHeader — find free comm slot and configure it */
void __rmonSendHeader(void) {
    register s32* node = (s32*)func_80009C30();
    while (node[1] != -1) {
        if (node[1] == 0) {
            *(s32*)((char*)node + 0x118) = *(s32*)((char*)node + 0x118) & 0xFFFF00FE;
            *(s32*)((char*)node + 0x118) = *(s32*)((char*)node + 0x118) | 0x6C01;
            break;
        }
        node = (s32*)node[3];
    }
}

/* func_800073DC = __rmonGetTCB (rmon/rmonmisc), IDO 7.1 -O1, whole function
 * [0x73DC,0x745C). Splat had cut it at 0x73F8 (prologue + first jal here,
 * body + epilogue as func_800073F8.s in kernel_018_e) and this unit clipped
 * the C to 0x1C. The "external callers of func_800073F8" were never calling
 * this body: kernel jal targets sit 0x400 above our numbering, so
 * jal func_800073F8 is __rmonSendHeader at our 0x6FF8 and
 * jal func_800077DC is THIS function. Merged 2026-10-20 (agent-g run 112);
 * the only post-cc step left is TRUNCATE_TEXT dropping the 4-byte align pad. */
RmonThreadNode* func_800073DC(s32 threadId) {
    register RmonThreadNode* node;

    node = func_80009C30();
    if (threadId <= 0) {
        return 0;
    }
    if (node->state != -1) {
        do {
            if (node->threadId == threadId) {
                return node;
            }
            node = node->next;
        } while (node->state != -1);
    }
    return 0;
}
