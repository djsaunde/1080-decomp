#include "common.h"

/* IDO 7.1 -O2 donor for gl_func_00025320 (0x90 / 36 words) = OoT's AudioLoad_AsyncDma
 * (load.c) verbatim; the `if (size) {}` block split after the ALIGN16 keeps size in s1
 * (without it the parameter is homed and reloaded). "BGLOAD" is the module string at
 * 0x1B0C0 (pinned extern). Spliced into game_libs_post.c.o via REPLACE_FUNC_BODY. */
typedef struct { void *a; void *b; s32 validCount; s32 first; s32 msgCount; void *msg; } gl_25320_OSMesgQueue;
typedef struct {
    s8 status; s8 delay; s8 medium; u8 *ramAddr; u32 curDevAddr; u8 *curRamAddr; u32 bytesRemaining; u32 chunkSize;
    u32 retMsg; gl_25320_OSMesgQueue *retQueue; gl_25320_OSMesgQueue msgQueue; void *msg; char ioMesg[0x18];
} gl_25320_AudioAsyncLoad;
extern void gl_25320_Audio_InvalDCache(void *, u32);
extern void gl_25320_osCreateMesgQueue(void *, void *, s32);
extern s32 gl_ref_00038604(void *, s32, s32, u32, void *, u32, void *, s32, char *);
extern char gl_25320_BGLOAD[];
#define ALIGN16(x) (((x) + 0xF) & ~0xF)

void gl_func_00025320(gl_25320_AudioAsyncLoad *asyncLoad, u32 size) {
    size = ALIGN16(size);
    gl_25320_Audio_InvalDCache(asyncLoad->curRamAddr, size);
    if (size) {}
    gl_25320_osCreateMesgQueue(&asyncLoad->msgQueue, &asyncLoad->msg, 1);
    gl_ref_00038604(&asyncLoad->ioMesg, 0, 0, asyncLoad->curDevAddr, asyncLoad->curRamAddr, size, &asyncLoad->msgQueue,
                    asyncLoad->medium, gl_25320_BGLOAD);
}
