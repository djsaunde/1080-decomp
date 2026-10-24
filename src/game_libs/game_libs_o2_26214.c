#include "common.h"

/* IDO 7.1 -O2 donor for gl_func_00026214 (0x57C / 351 words) = OoT's
 * AudioThread_UpdateImpl (thread.c) in its older form: no AudioLoad_ProcessScriptLoads,
 * the AI buffer to queue is (curAiBufIndex + 1) % 3, and the two statics
 * (sMaxAbiCmdCnt, sWaitingAudioTask) live in the module data at 0x1B3D0 / 0x1B3D4.
 * Levers: OoT's `volatile u32 resetTimer` (three separate reads), an `if (sp5C) {}`
 * block split before the queue-count read (sp5C / sp48 then spill like the target
 * instead of taking s1), and the sMaxAbiCmdCnt store bound to its own symbol at the
 * same address (the target does not share the lui/addiu base between the load and the
 * store). Spliced into game_libs_post.c.o via REPLACE_FUNC_BODY (no rodata). */
typedef struct { void *a; void *b; s32 validCount; } gl_26214_OSMesgQueue;
typedef struct {
    u32 type; u32 flags; u64 *ucode_boot; u32 ucode_boot_size; u64 *ucode; u32 ucode_size;
    u64 *ucode_data; u32 ucode_data_size; u64 *dram_stack; u32 dram_stack_size;
    u64 *output_buff; u64 *output_buff_size; u64 *data_ptr; u32 data_size;
    u64 *yield_data_ptr; u32 yield_data_size;
} gl_26214_OSTask_t;
typedef struct { gl_26214_OSTask_t t; void *msgQueue; void *unk_44; char pad48[8]; } gl_26214_AudioTask;
typedef struct {
    char pad0[0x16BC]; gl_26214_OSMesgQueue curAudioFrameDmaQueue;
    char pad16c8[0x2034 - 0x16C8]; s16 specUnk4; char pad2036[4]; s16 samplesPerFrameTarget;
    s16 maxAiBufferLength; s16 minAiBufferLength; char pad2040[0x2078 - 0x2040];
    s32 totalTaskCount; s32 curAudioFrameDmaCount; s32 rspTaskIndex; s32 curAiBufIndex;
    u64 *abiCmdBufs[2]; u64 *curAbiCmdBuf; gl_26214_AudioTask *curTask; gl_26214_AudioTask rspTask[2];
    char pad2138[0x2140 - 0x2138]; s16 *aiBuffers[3]; s16 aiBufLengths[3]; char pad2152[2];
    u32 audioRandom; char pad2158[4]; volatile u32 resetTimer; char pad2160[0x2CF0 - 0x2160];
    volatile u8 resetStatus; u8 specId; char pad2cf2[0x53BA - 0x2CF2]; u8 threadCmdQueueFinished;
    char pad53bb[0x53C4 - 0x53BB]; void *audioResetQueueP; void *taskStartQueueP; void *threadCmdProcQueueP;
} gl_26214_Ctx;
extern gl_26214_Ctx gl_26214_gAudioCtx;
#define gAudioCtx gl_26214_gAudioCtx
extern void (*gl_26214_gAudioCustomUpdateFunction)(void);
extern s32 gl_26214_sMaxAbiCmdCnt;
extern s32 gl_26214_sMaxAbiCmdCntW;
extern gl_26214_AudioTask *gl_26214_sWaitingAudioTask;
extern s16 *gl_26214_gWaveSamples[];
extern u64 gl_26214_aspMainTextStart[];
extern u64 gl_26214_aspMainDataStart[];
extern u64 gl_26214_aspMainDataEnd[];
extern gl_26214_AudioTask *gl_26214_AudioThread_Update(void);
extern void gl_ref_0003B3D0(u32);
extern int gl_func_00000000();
#define gAudioCustomUpdateFunction gl_26214_gAudioCustomUpdateFunction
#define sMaxAbiCmdCnt gl_26214_sMaxAbiCmdCnt
#define sWaitingAudioTask gl_26214_sWaitingAudioTask
#define gWaveSamples gl_26214_gWaveSamples
#define aspMainTextStart gl_26214_aspMainTextStart
#define aspMainDataStart gl_26214_aspMainDataStart
#define aspMainDataEnd gl_26214_aspMainDataEnd
#define AudioThread_Update gl_26214_AudioThread_Update
#define AudioThread_ProcessCmds gl_ref_0003B3D0
extern s32 gl_26214_osSendMesg(void *, void *, s32);
extern s32 gl_26214_osRecvMesg(void *, void *, s32);
extern u32 gl_26214_osAiGetLength(void);
extern s32 gl_26214_osAiSetNextBuffer(void *, u32);
extern u32 gl_26214_osGetCount(void);
extern void gl_26214_AudioLoad_DecreaseSampleDmaTtls(void);
extern void gl_26214_AudioLoad_ProcessLoads(s32);
extern s32 gl_26214_AudioHeap_ResetStep(void);
extern void gl_26214_AudioThread_ScheduleProcessCmds(void);
extern u64 *gl_26214_AudioSynth_Update(u64 *, s32 *, s16 *, s32);
#define osSendMesg gl_26214_osSendMesg
#define osRecvMesg gl_26214_osRecvMesg
#define osAiGetLength gl_26214_osAiGetLength
#define osAiSetNextBuffer gl_26214_osAiSetNextBuffer
#define osGetCount gl_26214_osGetCount
#define AudioLoad_DecreaseSampleDmaTtls gl_26214_AudioLoad_DecreaseSampleDmaTtls
#define AudioLoad_ProcessLoads gl_26214_AudioLoad_ProcessLoads
#define AudioHeap_ResetStep gl_26214_AudioHeap_ResetStep
#define AudioThread_ScheduleProcessCmds gl_26214_AudioThread_ScheduleProcessCmds
#define AudioSynth_Update gl_26214_AudioSynth_Update
#define MQ_GET_COUNT(q) ((q)->validCount)

gl_26214_AudioTask *gl_func_00026214(void) {
    u32 samplesRemainingInAi;
    s32 abiCmdCnt;
    s32 pad;
    s32 j;
    s32 sp5C;
    s16 *curAiBuffer;
    gl_26214_OSTask_t *task;
    s32 index;
    u32 sp4C;
    s32 sp48;
    s32 i;

    gAudioCtx.totalTaskCount++;
    if (gAudioCtx.totalTaskCount % (gAudioCtx.specUnk4) != 0) {
        if (gAudioCustomUpdateFunction != NULL) {
            gAudioCustomUpdateFunction();
        }

        if ((gAudioCtx.totalTaskCount % gAudioCtx.specUnk4) + 1 == gAudioCtx.specUnk4) {
            return sWaitingAudioTask;
        } else {
            return NULL;
        }
    }

    osSendMesg(gAudioCtx.taskStartQueueP, (void *)gAudioCtx.totalTaskCount, 0);
    gAudioCtx.rspTaskIndex ^= 1;
    gAudioCtx.curAiBufIndex++;
    gAudioCtx.curAiBufIndex %= 3;
    index = (gAudioCtx.curAiBufIndex + 1) % 3;
    samplesRemainingInAi = osAiGetLength() / 4;

    if (gAudioCtx.resetTimer < 16) {
        if (gAudioCtx.aiBufLengths[index] != 0) {
            osAiSetNextBuffer(gAudioCtx.aiBuffers[index], gAudioCtx.aiBufLengths[index] * 4);
            if (gAudioCtx.aiBuffers[index]) {}
            if (gAudioCtx.aiBufLengths[index]) {}
        }
    }

    if (gAudioCustomUpdateFunction != NULL) {
        gAudioCustomUpdateFunction();
    }

    sp5C = gAudioCtx.curAudioFrameDmaCount;
    for (i = 0; i < gAudioCtx.curAudioFrameDmaCount; i++) {
        if (osRecvMesg(&gAudioCtx.curAudioFrameDmaQueue, NULL, 0) == 0) {
            sp5C--;
        }
    }

    if (sp5C != 0) {
        for (i = 0; i < sp5C; i++) {
            osRecvMesg(&gAudioCtx.curAudioFrameDmaQueue, NULL, 1);
        }
    }

    if (sp5C) {}
    sp48 = MQ_GET_COUNT(&gAudioCtx.curAudioFrameDmaQueue);
    if (sp48 != 0) {
        for (i = 0; i < sp48; i++) {
            osRecvMesg(&gAudioCtx.curAudioFrameDmaQueue, NULL, 0);
        }
    }

    gAudioCtx.curAudioFrameDmaCount = 0;
    AudioLoad_DecreaseSampleDmaTtls();
    AudioLoad_ProcessLoads(gAudioCtx.resetStatus);

    if (gAudioCtx.resetStatus != 0) {
        if (AudioHeap_ResetStep() == 0) {
            if (gAudioCtx.resetStatus == 0) {
                osSendMesg(gAudioCtx.audioResetQueueP, (void *)(u32)gAudioCtx.specId, 0);
            }

            sWaitingAudioTask = NULL;
            return NULL;
        }
    }

    if (gAudioCtx.resetTimer > 16) {
        return NULL;
    }
    if (gAudioCtx.resetTimer != 0) {
        gAudioCtx.resetTimer++;
    }

    gAudioCtx.curTask = &gAudioCtx.rspTask[gAudioCtx.rspTaskIndex];
    gAudioCtx.curAbiCmdBuf = gAudioCtx.abiCmdBufs[gAudioCtx.rspTaskIndex];

    index = gAudioCtx.curAiBufIndex;
    curAiBuffer = gAudioCtx.aiBuffers[index];

    gAudioCtx.aiBufLengths[index] =
        (s16)((((gAudioCtx.samplesPerFrameTarget - samplesRemainingInAi) + 0x80) & ~0xF) + 0x10);
    if (gAudioCtx.aiBufLengths[index] < gAudioCtx.minAiBufferLength) {
        gAudioCtx.aiBufLengths[index] = gAudioCtx.minAiBufferLength;
    }

    if (gAudioCtx.aiBufLengths[index] > gAudioCtx.maxAiBufferLength) {
        gAudioCtx.aiBufLengths[index] = gAudioCtx.maxAiBufferLength;
    }

    j = 0;
    if (gAudioCtx.resetStatus == 0) {
        while (osRecvMesg(gAudioCtx.threadCmdProcQueueP, &sp4C, 0) != -1) {
            if (1) {}
            AudioThread_ProcessCmds(sp4C);
            j++;
        }
        if ((j == 0) && (gAudioCtx.threadCmdQueueFinished)) {
            AudioThread_ScheduleProcessCmds();
        }
    }

    gAudioCtx.curAbiCmdBuf =
        AudioSynth_Update(gAudioCtx.curAbiCmdBuf, &abiCmdCnt, curAiBuffer, gAudioCtx.aiBufLengths[index]);

    gAudioCtx.audioRandom = (gAudioCtx.audioRandom + gAudioCtx.totalTaskCount) * osGetCount();
    gAudioCtx.audioRandom = gAudioCtx.audioRandom + gAudioCtx.aiBuffers[index][gAudioCtx.totalTaskCount & 0xFF];

    gWaveSamples[8] = (s16 *)((u8 *)AudioThread_Update + (gAudioCtx.audioRandom & 0xFFF0));

    index = gAudioCtx.rspTaskIndex;
    gAudioCtx.curTask->msgQueue = NULL;
    gAudioCtx.curTask->unk_44 = NULL;

    task = &gAudioCtx.curTask->t;
    task->type = 2;
    task->flags = 0;
    task->ucode_boot = aspMainTextStart;
    task->ucode_boot_size = 0x1000;
    task->ucode_data_size = (aspMainDataEnd - aspMainDataStart) * sizeof(u64) - 1;
    task->ucode = aspMainTextStart;
    task->ucode_data = aspMainDataStart;
    task->ucode_size = 0x1000;
    task->dram_stack = NULL;
    task->dram_stack_size = 0;
    task->output_buff = NULL;
    task->output_buff_size = NULL;
    if (1) {}
    task->data_ptr = gAudioCtx.abiCmdBufs[index];
    task->data_size = abiCmdCnt * 8;
    task->yield_data_ptr = NULL;
    task->yield_data_size = 0;

    if (sMaxAbiCmdCnt < abiCmdCnt) {
        gl_26214_sMaxAbiCmdCntW = abiCmdCnt;
    }

    if (gAudioCtx.specUnk4 == 1) {
        return gAudioCtx.curTask;
    } else {
        sWaitingAudioTask = gAudioCtx.curTask;
        return NULL;
    }
}
