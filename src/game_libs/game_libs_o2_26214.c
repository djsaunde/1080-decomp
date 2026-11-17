#include "common.h"
#include "audio.h"
#include "uso_call.h"

/* IDO 7.1 -O2 donor for AudioThread_UpdateImpl (0x57C / 351 words) = OoT's
 * AudioThread_UpdateImpl (thread.c) in its older form: no AudioLoad_ProcessScriptLoads,
 * the AI buffer to queue is (curAiBufIndex + 1) % 3, and the two statics
 * (sMaxAbiCmdCnt, sWaitingAudioTask) live in the module data at 0x1B3D0 / 0x1B3D4.
 * Levers: OoT's `volatile u32 resetTimer` (three separate reads), an `if (sp5C) {}`
 * block split before the queue-count read (sp5C / sp48 then spill like the target
 * instead of taking s1), and the sMaxAbiCmdCnt store bound to its own symbol at the
 * same address (the target does not share the lui/addiu base between the load and the
 * store). Spliced into game_libs_post.c.o via REPLACE_FUNC_BODY (no rodata). */
/* AudioContext / AudioTask / OSTask_t / OSMesgQueue from include/audio.h (agent-c run 535). */
extern AudioContext gAudioContext_26214;
#define gAudioCtx gAudioContext_26214
extern void (*gl_26214_gAudioCustomUpdateFunction)(void);
extern s32 gl_26214_sMaxAbiCmdCnt;
extern s32 gl_26214_sMaxAbiCmdCntW;
extern AudioTask *gl_26214_sWaitingAudioTask;
extern s16 *gl_26214_gWaveSamples[];
extern u64 gl_26214_aspMainTextStart[];
extern u64 gl_26214_aspMainDataStart[];
extern u64 gl_26214_aspMainDataEnd[];
extern AudioTask *gl_26214_AudioThread_Update(void);
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
#ifdef PORT
extern unsigned char *port_audio_noise_base(void);
#endif

#ifdef PORT /* one object for the bytes these tokens share, so gcc keeps the order (scripts/port-alias-audit.py, plan 18.7) */
extern char port_bootup_data[];
#define gl_26214_sMaxAbiCmdCnt (*(__typeof__(gl_26214_sMaxAbiCmdCnt) *)(port_bootup_data + 0x1B3D0))
#define gl_26214_sMaxAbiCmdCntW (*(__typeof__(gl_26214_sMaxAbiCmdCntW) *)(port_bootup_data + 0x1B3D0))
#endif
AudioTask *AudioThread_UpdateImpl(void) {
    u32 samplesRemainingInAi;
    s32 abiCmdCnt;
    s32 pad;
    s32 j;
    s32 sp5C;
    s16 *curAiBuffer;
    OSTask_t *task;
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

#ifdef PORT
    /* The N64 reads noise from the code bytes after AudioThread_Update; the port has its
     * own copy of those ROM bytes (port/audio/port_audio.c, plan 15.6). */
    gWaveSamples[8] = (s16 *)(port_audio_noise_base() + (gAudioCtx.audioRandom & 0xFFF0));
#else
    gWaveSamples[8] = (s16 *)((u8 *)AudioThread_Update + (gAudioCtx.audioRandom & 0xFFF0));
#endif

    index = gAudioCtx.rspTaskIndex;
    gAudioCtx.curTask->taskQueue = NULL;
    gAudioCtx.curTask->unk_44 = NULL;

    task = &gAudioCtx.curTask->task;
    task->type = 2;
    task->flags = 0;
    task->ucode_boot = XDATA(aspMainTextStart, port_bootup_text, 0x46470);
    task->ucode_boot_size = 0x1000;
    task->ucode_data_size = (aspMainDataEnd - aspMainDataStart) * sizeof(u64) - 1;
    task->ucode = XDATA(aspMainTextStart, port_bootup_text, 0x46470);
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
#ifdef PORT
#undef gl_26214_sMaxAbiCmdCnt
#undef gl_26214_sMaxAbiCmdCntW
#endif
