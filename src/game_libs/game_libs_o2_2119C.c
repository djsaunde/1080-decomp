#include "common.h"
#include "gdl/gl_data.h"
#include "audio.h" /* Note (docs/1080_AUDIO.md) */

/* IDO 7.1 -O2 donor for AudioHeap_ResetStep (0x2FC / 191 words) = OoT's
 * AudioHeap_ResetStep (heap.c) verbatim, minus the AudioHeap_DiscardSampleBanks
 * call (1080 has only the sample-cache discard); resetStatus is `volatile u8` as in
 * OoT (each case re-reads it). Spliced into game_libs_post.c.o via
 * REPLACE_FUNC_BODY; the splice renames the donor-local .rodata jumptable reloc
 * (resetStatus switch, 5 entries) to AudioHeap_ResetStep_rodata, pinned to the USO
 * table. gAudioCtx offsets: specUnk4 0x2034, maxAiBufferLength 0x203C,
 * numSequencePlayers 0x2048, ticksPerUpdateInv 0x2050, numNotes 0x2070,
 * aiBuffers 0x2140, aiBufLengths 0x214C, resetStatus 0x2CF0,
 * audioResetFadeOutFramesLeft 0x2CF4, notes 0x2CFC (stride 0xD0), seqPlayers
 * 0x2D00 (stride 0x160). */
typedef struct { char pad[0x160]; } gl_2119C_SeqPlayer;
typedef struct {
    char pad0[0x2034]; s16 specUnk4; char pad2036[0x203C - 0x2036]; s16 maxAiBufferLength;
    char pad203e[0x2048 - 0x203E]; s16 numSequencePlayers; char pad204a[0x2050 - 0x204A]; f32 ticksPerUpdateInv;
    char pad2054[0x2070 - 0x2054]; s32 numNotes; char pad2074[0x2140 - 0x2074];
    s16 *aiBuffers[3]; s16 aiBufLengths[3]; char pad2152[0x2CF0 - 0x2152];
    volatile u8 resetStatus; char pad2cf1[3]; s32 audioResetFadeOutFramesLeft; char pad2cf8[4]; Note *notes;
    gl_2119C_SeqPlayer seqPlayers[4];
} gl_2119C_Ctx;
extern int D_00000000;
#define gAudioCtx (*(gl_2119C_Ctx *)&gAudioContext)
extern int gl_func_00000000();
extern int gl_func_00035668();
extern int gl_func_0003579C();
extern int gl_func_00036AD0();
extern int gl_func_00035B04();
#define AudioSeq_SequencePlayerDisableAsFinished gl_func_00000000
#define AudioHeap_UpdateReverbs gl_func_00035668
#define AudioHeap_ClearCurrentAiBuffer gl_func_0003579C
#define AudioHeap_DiscardSampleCaches gl_func_00036AD0
#define AudioHeap_Init gl_func_00035B04

s32 AudioHeap_ResetStep(void) {
    s32 i;
    s32 j;
    s32 sp24;

    if (gAudioCtx.specUnk4 == 2) {
        sp24 = 2;
    } else {
        sp24 = 1;
    }

    switch (gAudioCtx.resetStatus) {
        case 5:
            for (i = 0; i < gAudioCtx.numSequencePlayers; i++) {
                AudioSeq_SequencePlayerDisableAsFinished(&gAudioCtx.seqPlayers[i]);
            }
            gAudioCtx.audioResetFadeOutFramesLeft = 2 / sp24;
            gAudioCtx.resetStatus--;
            break;

        case 4:
            if (gAudioCtx.audioResetFadeOutFramesLeft != 0) {
                gAudioCtx.audioResetFadeOutFramesLeft--;
                AudioHeap_UpdateReverbs();
            } else {
                for (i = 0; i < gAudioCtx.numNotes; i++) {
                    if (gAudioCtx.notes[i].sampleState.enabled &&
                        gAudioCtx.notes[i].playbackState.adsr.action.s.state != 0) {
                        gAudioCtx.notes[i].playbackState.adsr.fadeOutVel = gAudioCtx.ticksPerUpdateInv;
                        gAudioCtx.notes[i].playbackState.adsr.action.s.release = 1;
                    }
                }
                gAudioCtx.audioResetFadeOutFramesLeft = 8 / sp24;
                gAudioCtx.resetStatus--;
            }
            break;

        case 3:
            if (gAudioCtx.audioResetFadeOutFramesLeft != 0) {
                gAudioCtx.audioResetFadeOutFramesLeft--;
                AudioHeap_UpdateReverbs();
            } else {
                gAudioCtx.audioResetFadeOutFramesLeft = 2 / sp24;
                gAudioCtx.resetStatus--;
            }
            break;

        case 2:
            AudioHeap_ClearCurrentAiBuffer();
            if (gAudioCtx.audioResetFadeOutFramesLeft != 0) {
                gAudioCtx.audioResetFadeOutFramesLeft--;
            } else {
                gAudioCtx.resetStatus--;
                AudioHeap_DiscardSampleCaches();
            }
            break;

        case 1:
            AudioHeap_Init();
            gAudioCtx.resetStatus = 0;
            for (i = 0; i < 3; i++) {
                gAudioCtx.aiBufLengths[i] = gAudioCtx.maxAiBufferLength;
                for (j = 0; j < 0x580; j++) {
                    gAudioCtx.aiBuffers[i][j] = 0;
                }
            }
            break;
    }

    if (gAudioCtx.resetStatus < 3) {
        return 0;
    }

    return 1;
}
