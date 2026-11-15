#ifndef AUDIO_H
#define AUDIO_H

/* 1080 Snowboarding audio driver (game_libs 0x1C9E4-0x2D374): the 1998 EAD
 * driver that OoT's src/audio/internal/ (references/oot) descends from.
 * Function names follow OoT verbatim (AudioSynth_*, AudioHeap_*, AudioLoad_*,
 * AudioThread_*, Audio_*, AudioSeq_*); renames.tsv holds the mapping.
 *
 * gAudioCtx is NOT a linked global here: it is the start of bootup.uso's audio
 * data block (section base, D_00000000 + 0). Each matched function reaches it
 * through its own base-0 extern (gl_<addr>_gAudioCtx and friends, pinned to 0
 * in undefined_syms_auto.txt) or through `*(T *)&D_00000000`. Keep those
 * symbols when retyping: the per-function symbols are what keep the matched
 * relocations and lui/addiu CSE shapes. Retype the extern, not the symbol.
 *
 * Layout: ONLY offsets read or written by matched 1080 code are named (the
 * function that proves each one is cited); everything else is padding. This
 * is the 1080 layout, not OoT's: OoT's context is larger and shifted (OoT
 * notes at 0x3530, here 0x2CFC). Sub-structures whose fields are not yet
 * verified are opaque blocks of their verified stride.
 * Opt-in header: include it only in TUs that do not already typedef these
 * names (a duplicate typedef is a cfe error). */

#include "common.h"

/* AudioHeap_InitPool / AudioHeap_Alloc: start 0x0, cur 0x4, size 0x8, numEntries 0xC. */
typedef struct AudioAllocPool {
    /* 0x0 */ u8 *startRamAddr;
    /* 0x4 */ u8 *curRamAddr;
    /* 0x8 */ s32 size;
    /* 0xC */ s32 numEntries;
} AudioAllocPool; /* size = 0x10 */

/* Opaque until their fields are verified; the strides are verified. */
typedef struct SynthesisReverb { u8 data[0x158]; } SynthesisReverb; /* AudioSynth_*: reverbIndex * 0x158 */
typedef struct SequencePlayer { u8 data[0x160]; } SequencePlayer;   /* AudioThread_SetFadeOutTimer: i * 352 */
typedef struct AudioAsyncLoad { u8 data[0x54]; } AudioAsyncLoad;   /* AudioLoad_ProcessAsyncLoads: 16 x 0x54 */

typedef struct AudioContext {
    /* 0x0000 */ char unk_0000;
    /* 0x0001 */ s8 numSynthesisReverbs;              /* DoOneAudioUpdate */
    /* 0x0002 */ char unk_0002[0x0010 - 0x0002];
    /* 0x0010 */ s16 *curLoadedBook;                  /* ProcessNote */
    /* 0x0014 */ void *sampleStates;                  /* DoOneAudioUpdate: [updateIndex * numNotes + i] */
    /* 0x0018 */ SynthesisReverb synthesisReverbs[3]; /* at most 3 fit below usedSamples */
    /* 0x0420 */ char unk_0420[0x0430 - 0x0420];
    /* 0x0430 */ void *usedSamples[0x80];             /* RelocateFontAndPreloadSamples, PreloadSamplesForFont */
    /* 0x0630 */ u8 preloadSampleStack[0x1030 - 0x0630]; /* 0x80 preload requests (ProcessSamplePreloads) */
    /* 0x1030 */ s32 numUsedSamples;
    /* 0x1034 */ s32 preloadSampleStackTop;
    /* 0x1038 */ AudioAsyncLoad asyncLoads[16];
    /* 0x1578 */ char unk_1578[0x157C - 0x1578];
    /* 0x157C */ u8 slowLoads[0x1644 - 0x157C];      /* two slow-load slots (ProcessSlowLoads) */
    /* 0x1644 */ void *cartHandle;                    /* AudioLoad_Dma */
    /* 0x1648 */ void *driveHandle;                   /* AudioLoad_Dma */
    /* 0x164C */ u8 externalLoadQueue[0x18];          /* OSMesgQueue (AudioLoad_Init) */
    /* 0x1664 */ void *externalLoadMsgBuf[8];
    /* 0x1684 */ u8 preloadSampleQueue[0x18];         /* OSMesgQueue */
    /* 0x169C */ void *preloadSampleMsgBuf[8];
    /* 0x16BC */ u8 curAudioFrameDmaQueue[0x18];      /* OSMesgQueue (UpdateImpl, DmaSampleData) */
    /* 0x16D4 */ void *curAudioFrameDmaMsgBuf[0x40];
    /* 0x17D4 */ u8 curAudioFrameDmaIoMsgBuf[0x40][0x18]; /* OSIoMesg[64] (DmaSampleData) */
    /* 0x1DD4 */ u8 syncDmaQueue[0x18];               /* OSMesgQueue (AudioLoad_SyncDma) */
    /* 0x1DEC */ void *syncDmaMesg;
    /* 0x1DF0 */ u8 syncDmaIoMesg[0x18];              /* OSIoMesg */
    /* 0x1E08 */ void *sampleDmas;                    /* InitSampleDmaBuffers, DmaSampleData */
    /* 0x1E0C */ u32 sampleDmaCount;
    /* 0x1E10 */ u32 sampleDmaListSize1;
    /* 0x1E14 */ char unk_1E14[4];
    /* 0x1E18 */ u8 sampleDmaReuseQueue1[0x100];
    /* 0x1F18 */ u8 sampleDmaReuseQueue2[0x100];
    /* 0x2018 */ u8 sampleDmaReuseQueue1RdPos;
    /* 0x2019 */ u8 sampleDmaReuseQueue2RdPos;
    /* 0x201A */ u8 sampleDmaReuseQueue1WrPos;
    /* 0x201B */ u8 sampleDmaReuseQueue2WrPos;
    /* 0x201C */ void *sequenceTable;                 /* AudioLoad_GetLoadTable case 0 */
    /* 0x2020 */ void *soundFontTable;                /* case 1 */
    /* 0x2024 */ void *sampleBankTable;               /* case 2 */
    /* 0x2028 */ u8 *sequenceFontTable;               /* SyncLoadSeqFonts, SequenceChannelProcessScript */
    /* 0x202C */ s16 numSequences;                    /* SyncLoadSeqParts bound */
    /* 0x202E */ char unk_202E[2];
    /* 0x2030 */ void *soundFontList;                 /* SyncLoadFont, RelocateFont */
    /* 0x2034 */ s16 specUnk4;                        /* AudioBufferParameters start (ResetStep, UpdateImpl) */
    /* 0x2036 */ char unk_2036[0x203A - 0x2036];
    /* 0x203A */ s16 samplesPerFrameTarget;           /* UpdateImpl */
    /* 0x203C */ s16 maxAiBufferLength;
    /* 0x203E */ s16 minAiBufferLength;
    /* 0x2040 */ s16 ticksPerUpdate;                  /* AudioSynth_Update */
    /* 0x2042 */ char unk_2042[0x2048 - 0x2042];
    /* 0x2048 */ s16 numSequencePlayers;              /* DiscardSequence, ProcessGlobalCmd */
    /* 0x204A */ char unk_204A[0x204C - 0x204A];
    /* 0x204C */ f32 resampleRate;
    /* 0x2050 */ f32 ticksPerUpdateInv;
    /* 0x2054 */ f32 ticksPerUpdateInvScaled;         /* InitAdsrDecayTable (D->0x2054 * 256) */
    /* 0x2058 */ char unk_2058[0x2060 - 0x2058];
    /* 0x2060 */ s32 sampleDmaBufSize1;               /* InitSampleDmaBuffers */
    /* 0x2064 */ s32 sampleDmaBufSize2;
    /* 0x2068 */ s32 sampleDmaBufSize;
    /* 0x206C */ char unk_206C[4];
    /* 0x2070 */ s32 numNotes;
    /* 0x2074 */ s16 tempoInternalToExternal;         /* SequencePlayerProcessSequence */
    /* 0x2076 */ s8 soundOutputMode;                  /* Audio_InitSampleState */
    /* 0x2077 */ char unk_2077;
    /* 0x2078 */ s32 totalTaskCount;                  /* UpdateImpl */
    /* 0x207C */ s32 curAudioFrameDmaCount;
    /* 0x2080 */ s32 rspTaskIndex;
    /* 0x2084 */ s32 curAiBufIndex;
    /* 0x2088 */ u64 *abiCmdBufs[2];
    /* 0x2090 */ u64 *curAbiCmdBuf;
    /* 0x2094 */ void *curTask;
    /* 0x2098 */ u8 rspTask[2][0x50];
    /* 0x2138 */ char unk_2138[0x2140 - 0x2138];
    /* 0x2140 */ s16 *aiBuffers[3];
    /* 0x214C */ s16 aiBufLengths[3];
    /* 0x2152 */ char unk_2152[2];
    /* 0x2154 */ u32 audioRandom;                     /* SeqLayerProcessScriptStep3 */
    /* 0x2158 */ char unk_2158[4];
    /* 0x215C */ volatile u32 resetTimer;             /* AudioLoad_Dma, ProcessAsyncLoads */
    /* 0x2160 */ char unk_2160[0x2168 - 0x2160];
    /* 0x2168 */ AudioAllocPool sessionPool;          /* InitMainPools */
    /* 0x2178 */ AudioAllocPool externalPool;         /* AllocAttemptExternal, InitExternalPool */
    /* 0x2188 */ AudioAllocPool initPool;             /* InitMainPools */
    /* 0x2198 */ AudioAllocPool miscPool;             /* InitSampleDmaBuffers */
    /* 0x21A8 */ char unk_21A8[0x2C10 - 0x21A8];      /* cache pools, seq/font/sample caches, permanent + sample caches */
    /* 0x2C10 */ u8 sampleFontLoadStatus[0x30];       /* ResetLoadStatus, AsyncLoadInner */
    /* 0x2C40 */ u8 fontLoadStatus[0x30];
    /* 0x2C70 */ u8 seqLoadStatus[0x80];
    /* 0x2CF0 */ volatile u8 resetStatus;             /* ResetStep */
    /* 0x2CF1 */ u8 specId;
    /* 0x2CF2 */ char unk_2CF2[2];
    /* 0x2CF4 */ s32 audioResetFadeOutFramesLeft;
    /* 0x2CF8 */ f32 *adsrDecayTable;
    /* 0x2CFC */ void *notes;                         /* Note[numNotes], stride 0xD0 */
    /* 0x2D00 */ SequencePlayer seqPlayers[4];
    /* 0x3280 */ char unk_3280[0x5378 - 0x3280];
    /* 0x5378 */ u8 noteFreeLists[0x53B8 - 0x5378];   /* NotePool (NotePoolClear, AllocNote) */
    /* 0x53B8 */ u8 threadCmdWritePos;                /* AudioThread_QueueCmd */
    /* 0x53B9 */ u8 threadCmdReadPos;
    /* 0x53BA */ u8 threadCmdQueueFinished;
    /* 0x53BB */ char unk_53BB;
    /* 0x53BC */ u16 threadCmdChannelMask[4];         /* ProcessGlobalCmd */
} AudioContext; /* size >= 0x53C4 (later fields not yet modelled) */

#define AUDIO_ASSERT_OFFSET(field, off) \
    typedef char audio_ctx_assert_##off[(int)&((AudioContext *)0)->field == (off) ? 1 : -1]
AUDIO_ASSERT_OFFSET(curLoadedBook, 0x0010);
AUDIO_ASSERT_OFFSET(synthesisReverbs, 0x0018);
AUDIO_ASSERT_OFFSET(usedSamples, 0x0430);
AUDIO_ASSERT_OFFSET(numUsedSamples, 0x1030);
AUDIO_ASSERT_OFFSET(asyncLoads, 0x1038);
AUDIO_ASSERT_OFFSET(cartHandle, 0x1644);
AUDIO_ASSERT_OFFSET(preloadSampleQueue, 0x1684);
AUDIO_ASSERT_OFFSET(curAudioFrameDmaQueue, 0x16BC);
AUDIO_ASSERT_OFFSET(syncDmaQueue, 0x1DD4);
AUDIO_ASSERT_OFFSET(syncDmaIoMesg, 0x1DF0);
AUDIO_ASSERT_OFFSET(sampleDmas, 0x1E08);
AUDIO_ASSERT_OFFSET(sampleDmaReuseQueue1WrPos, 0x201A);
AUDIO_ASSERT_OFFSET(sequenceTable, 0x201C);
AUDIO_ASSERT_OFFSET(sequenceFontTable, 0x2028);
AUDIO_ASSERT_OFFSET(soundFontList, 0x2030);
AUDIO_ASSERT_OFFSET(specUnk4, 0x2034);
AUDIO_ASSERT_OFFSET(numSequencePlayers, 0x2048);
AUDIO_ASSERT_OFFSET(ticksPerUpdateInv, 0x2050);
AUDIO_ASSERT_OFFSET(numNotes, 0x2070);
AUDIO_ASSERT_OFFSET(soundOutputMode, 0x2076);
AUDIO_ASSERT_OFFSET(rspTask, 0x2098);
AUDIO_ASSERT_OFFSET(aiBuffers, 0x2140);
AUDIO_ASSERT_OFFSET(audioRandom, 0x2154);
AUDIO_ASSERT_OFFSET(resetTimer, 0x215C);
AUDIO_ASSERT_OFFSET(externalPool, 0x2178);
AUDIO_ASSERT_OFFSET(miscPool, 0x2198);
AUDIO_ASSERT_OFFSET(sampleFontLoadStatus, 0x2C10);
AUDIO_ASSERT_OFFSET(resetStatus, 0x2CF0);
AUDIO_ASSERT_OFFSET(notes, 0x2CFC);
AUDIO_ASSERT_OFFSET(seqPlayers, 0x2D00);
AUDIO_ASSERT_OFFSET(noteFreeLists, 0x5378);
AUDIO_ASSERT_OFFSET(threadCmdChannelMask, 0x53BC);
typedef char audio_pool_size_assert[sizeof(AudioAllocPool) == 0x10 ? 1 : -1];
typedef char audio_reverb_size_assert[sizeof(SynthesisReverb) == 0x158 ? 1 : -1];
typedef char audio_seqplayer_size_assert[sizeof(SequencePlayer) == 0x160 ? 1 : -1];

#endif /* AUDIO_H */
