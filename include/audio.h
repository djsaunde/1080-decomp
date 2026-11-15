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
 * verified are opaque blocks of their verified stride (AudioAsyncLoad).
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

/* Opaque until its fields are verified; the stride is verified. */
typedef struct AudioAsyncLoad { u8 data[0x54]; } AudioAsyncLoad;   /* AudioLoad_ProcessAsyncLoads: 16 x 0x54 */

/* AudioSynth_ProcessNote's views (game_libs_post.c gl_e134_*): the OoT sample
 * header with the u32 codec/medium/size bitfield word, the flat ADPCM loop
 * (no separate header struct) and the codebook. */
typedef struct AdpcmLoop {
    /* 0x00 */ u32 start;
    /* 0x04 */ u32 end;
    /* 0x08 */ u32 count;
    /* 0x0C */ char unk_0C[4];
    /* 0x10 */ s16 predictorState[16];
} AdpcmLoop; /* size = 0x30 */

typedef struct AdpcmBook {
    /* 0x0 */ s32 order;
    /* 0x4 */ s32 numPredictors;
    /* 0x8 */ s16 book[1];
} AdpcmBook;

typedef struct Sample {
    /* 0x0 */ u32 codec : 4;
    /* 0x0 */ u32 medium : 2;
    /* 0x0 */ u32 unk_bit26 : 1;
    /* 0x0 */ u32 isRelocated : 1;
    /* 0x1 */ u32 size : 24;
    /* 0x4 */ u8 *sampleAddr;
    /* 0x8 */ AdpcmLoop *loop;
    /* 0xC */ AdpcmBook *book;
} Sample; /* size = 0x10 */

typedef struct TunedSample {
    /* 0x0 */ Sample *sample;
    /* 0x4 */ f32 tuning;
} TunedSample; /* size = 0x8 */

/* 0x14 (the SM64-era item; OoT's is 0x1C). AudioSynth_InitNextRingBuf (every
 * field), AudioSynth_SaveReverbSamples (toDownsampleLeft),
 * AudioSynth_LoadRingBuffer1AtTemp / LoadAuxBufferC (startPos low half 0xE,
 * lengthA 0x10, lengthB 0x12). */
typedef struct ReverbRingBufferItem {
    /* 0x00 */ s16 numSamplesAfterDownsampling;
    /* 0x02 */ s16 chunkLen;
    /* 0x04 */ s16 *toDownsampleLeft;
    /* 0x08 */ s16 *toDownsampleRight;
    /* 0x0C */ s32 startPos;
    /* 0x10 */ s16 lengthA;
    /* 0x12 */ s16 lengthB;
} ReverbRingBufferItem; /* size = 0x14 */

/* 0x158 (AudioSynth_*: reverbIndex * 0x158 from gAudioCtx+0x18). The OoT
 * fields up to 0x13, then OoT's unk_14..unk_1B are absent (everything from
 * nextRingBufPos on is OoT - 8), the ring items are 0x14 and there is no
 * items2. Offsets from the AudioHeap_Init reverb setup (game_libs_post.c
 * gl_21498_Slot: settings -> downsampleRate 0x4, windowSize 0x6 = n * 64 /
 * downsampleRate, decayRatio 0xC, volume 0xA, leakRtl/Ltr 0x10/0x12, unk_05,
 * unk_08, useReverb = 8, ring buffers 0x20/0x24, nextRingBufPos 0x14 = 0,
 * unk_18 = 0, curFrame 0x3 = 0, framesToIgnore 0x2 = 2, resampleFlags 0x0 = 1,
 * bufSizePerChan 0x1C, tunedSample.sample = &sample, sample.loop = &loop,
 * loop start/end/count, unk_0E = 0x8000 / downsampleRate, the four 32-byte
 * resample states 0x28..0x34, filters 0x100..0x10C); AudioHeap_UpdateReverbs
 * (decayRatio -= decayRatio / 4); AudioSynth_InitNextRingBuf (items 0x38);
 * AudioSynth_LeakReverb (leakRtl / leakLtr); AudioSynth_FilterReverb (filters);
 * AudioSynth_MaybeMixRingBuffer1 (downsampleRate == 1 of reverb unk_05,
 * unk_08); AudioSynth_LoadAuxBufferC (resampleFlags, unk_0E, unk_28 / unk_2C).
 * Types follow the matched 1080 readers (u16 where they load with lhu). */
typedef struct SynthesisReverb {
    /* 0x000 */ u8 resampleFlags;
    /* 0x001 */ u8 useReverb;
    /* 0x002 */ u8 framesToIgnore;
    /* 0x003 */ u8 curFrame;
    /* 0x004 */ u8 downsampleRate;
    /* 0x005 */ s8 unk_05;
    /* 0x006 */ u16 windowSize;
    /* 0x008 */ u16 unk_08;
    /* 0x00A */ s16 volume;
    /* 0x00C */ u16 decayRatio;
    /* 0x00E */ u16 unk_0E;
    /* 0x010 */ u16 leakRtl;
    /* 0x012 */ u16 leakLtr;
    /* 0x014 */ s32 nextRingBufPos;
    /* 0x018 */ s32 unk_18;            /* OoT unk_20 */
    /* 0x01C */ s32 bufSizePerChan;
    /* 0x020 */ s16 *leftRingBuf;
    /* 0x024 */ s16 *rightRingBuf;
    /* 0x028 */ void *unk_28;          /* OoT unk_30..unk_3C: resample states */
    /* 0x02C */ void *unk_2C;
    /* 0x030 */ void *unk_30;
    /* 0x034 */ void *unk_34;
    /* 0x038 */ ReverbRingBufferItem items[2][5];
    /* 0x100 */ s16 *filterLeft;
    /* 0x104 */ s16 *filterRight;
    /* 0x108 */ s16 *filterLeftState;
    /* 0x10C */ s16 *filterRightState;
    /* 0x110 */ TunedSample tunedSample;
    /* 0x118 */ Sample sample;
    /* 0x128 */ AdpcmLoop loop;
} SynthesisReverb; /* size = 0x158 */

/* AudioSeq_AudioListPushBack (0x2A884): prev 0x0, next 0x4, count/value 0x8
 * (list->u.count++), pool 0xC (item->pool = list->pool). */
typedef struct AudioListItem {
    /* 0x0 */ struct AudioListItem *prev;
    /* 0x4 */ struct AudioListItem *next;
    /* 0x8 */ union { void *value; s32 count; } u;
    /* 0xC */ struct NotePool *pool;
} AudioListItem; /* size = 0x10 */

/* Audio_InitNoteLists / Audio_NotePoolClear: four lists, 0x40 (SequencePlayer
 * 0x9C..0xDC, SequenceChannel 0x94..0xD4, gAudioCtx 0x5378..0x53B8). */
typedef struct NotePool {
    /* 0x00 */ AudioListItem disabled;
    /* 0x10 */ AudioListItem decaying;
    /* 0x20 */ AudioListItem releasing;
    /* 0x30 */ AudioListItem active;
} NotePool; /* size = 0x40 */

/* Matched donors game_libs_o2_2BB7C.c / _2C7A4.c (script interpreters). */
typedef struct SeqScriptState {
    /* 0x00 */ u8 *pc;
    /* 0x04 */ u8 *stack[4];
    /* 0x14 */ u8 remLoopIters[4];
    /* 0x18 */ u8 depth;           /* AudioSeq_InitSequenceChannel clears channel+0x88 */
    /* 0x19 */ s8 value;
} SeqScriptState; /* size = 0x1C */

/* AudioSeq_InitSequenceChannel: decayIndex 0xF0, sustain 0, envelope = default;
 * AudioSeq_SeqChannelSetLayer copies the 8 bytes channel+0x8C -> layer+0x18. */
typedef struct AdsrSettings {
    /* 0x0 */ u8 decayIndex;
    /* 0x1 */ u8 sustain;
    /* 0x4 */ void *envelope;
} AdsrSettings; /* size = 0x8 */

/* Matched donor game_libs_o2_2BB7C.c (channel->stereo.asByte = cmd & 0x7F). */
typedef union Stereo {
    struct {
        u8 unused : 2;
        u8 bit2 : 2;
        u8 strongRight : 1;
        u8 strongLeft : 1;
        u8 stereoHeadsetEffects : 1;
        u8 usesHeadsetPanEffects : 1;
    } s;
    u8 asByte;
} Stereo; /* size = 0x1 */

struct SequenceChannel;
struct SequenceLayer;

/* 0x160, the OoT layout unchanged. AudioSeq_ResetSequencePlayer (stopScript
 * bit, delay 0x10, state 1, fade timers 0x12/0x14, tempoAcc 0xA, tempo 0x8 =
 * 0x1680 = 120 * 48, tempoChange 0xC, transposition 0xE, noteAllocPolicy 0x2,
 * velocity/gate tables 0x94/0x98, scriptCounter 0xE0, fadeVolume 0x1C,
 * fadeVelocity 0x20, volume 0x24, muteVolumeScale 0x28 = 0.5, 16 channels at
 * 0x38); AudioSeq_InitSequencePlayer (muteBehavior 0x3 = 0x60, fadeVolumeScale
 * 0x2C, bend 0x34, seqScriptIO 0x158 = -1, notePool 0x9C);
 * AudioSeq_SkipForwardSequence (skipTicks 0xDC); AudioSeq_SequencePlayerDisable
 * (seqId 0x4, defaultFont 0x5). The flag byte is declared with u32 bitfields
 * exactly as the matched donor game_libs_o2_2C7A4.c spells it (container type
 * steers IDO's bitfield loads). */
typedef struct SequencePlayer {
    /* 0x000 */ u32 enabled : 1;
    /* 0x000 */ u32 finished : 1;
    /* 0x000 */ u32 muted : 1;
    /* 0x000 */ u32 seqDmaInProgress : 1;
    /* 0x000 */ u32 fontDmaInProgress : 1;
    /* 0x000 */ u32 recalculateVolume : 1;
    /* 0x000 */ u32 stopScript : 1;
    /* 0x000 */ u32 applyBend : 1;
    /* 0x001 */ u8 state;
    /* 0x002 */ u8 noteAllocPolicy;
    /* 0x003 */ u8 muteBehavior;
    /* 0x004 */ u8 seqId;
    /* 0x005 */ u8 defaultFont;
    /* 0x006 */ u8 unk_06;
    /* 0x007 */ s8 playerIdx;
    /* 0x008 */ u16 tempo;
    /* 0x00A */ u16 tempoAcc;
    /* 0x00C */ s16 tempoChange;
    /* 0x00E */ s16 transposition;
    /* 0x010 */ u16 delay;
    /* 0x012 */ u16 fadeTimer;
    /* 0x014 */ u16 fadeTimerUnkEu;
    /* 0x016 */ char unk_16[2];
    /* 0x018 */ u8 *seqData;
    /* 0x01C */ f32 fadeVolume;
    /* 0x020 */ f32 fadeVelocity;
    /* 0x024 */ f32 volume;
    /* 0x028 */ f32 muteVolumeScale;
    /* 0x02C */ f32 fadeVolumeScale;
    /* 0x030 */ f32 appliedFadeVolume;
    /* 0x034 */ f32 bend;
    /* 0x038 */ struct SequenceChannel *channels[16];
    /* 0x078 */ SeqScriptState scriptState;
    /* 0x094 */ u8 *shortNoteVelocityTable;
    /* 0x098 */ u8 *shortNoteGateTimeTable;
    /* 0x09C */ NotePool notePool;
    /* 0x0DC */ s32 skipTicks;
    /* 0x0E0 */ u32 scriptCounter;
    /* 0x0E4 */ char unk_E4[0x158 - 0xE4];
    /* 0x158 */ s8 seqScriptIO[8];
} SequencePlayer; /* size = 0x160 (AudioThread_SetFadeOutTimer: i * 352) */

/* 0xE4 (AudioSeq_InitSequencePlayerChannels allocates 228 from miscPool), the
 * OoT layout up to 0x4C, then EIGHT layers (OoT: four) and everything after
 * shifted by 0x10. Offsets: AudioSeq_InitSequenceChannel (every field it
 * resets: changes 0x1 = 0xFF, newPan 0xA = 64, panChannelWeight 0xB = 128,
 * notePriority 0x5 = 3, someOtherPriority 0x6 = 1, vibrato 0x10..0x1C = 0x800 /
 * 0, delay 0x1E, combFilterGain 0x20, transposition 0x26, volumeScale 0x28,
 * volume 0x2C, freqScale 0x38, noteUnused 0x40, adsr 0x8C, notePool 0x94,
 * seqScriptIO 0xD4 = -1, filter 0xDC, stereo 0xE0); seqPlayer 0x4C / layers
 * 0x50 (InitSequencePlayerChannels). Bitfields as the matched donor
 * game_libs_o2_2BB7C.c declares them (u32 container covering 0x0..0x1). */
typedef struct SequenceChannel {
    /* 0x00 */ u32 enabled : 1;
    /* 0x00 */ u32 finished : 1;
    /* 0x00 */ u32 stopScript : 1;
    /* 0x00 */ u32 stopSomething2 : 1;
    /* 0x00 */ u32 hasInstrument : 1;
    /* 0x00 */ u32 stereoHeadsetEffects : 1;
    /* 0x00 */ u32 largeNotes : 1;
    /* 0x00 */ u32 unused : 1;
    /* 0x01 */ u32 changesFreqScale : 1;
    /* 0x01 */ u32 changesVolume : 1;
    /* 0x01 */ u32 changesPan : 1;
    /* 0x01 */ u32 changesRest : 5;
    /* 0x02 */ u8 noteAllocPolicy;
    /* 0x03 */ u8 muteBehavior;
    /* 0x04 */ u8 targetReverbVol;
    /* 0x05 */ u8 notePriority;
    /* 0x06 */ u8 someOtherPriority;
    /* 0x07 */ u8 fontId;
    /* 0x08 */ u8 reverbIndex;
    /* 0x09 */ u8 bookOffset;
    /* 0x0A */ u8 newPan;
    /* 0x0B */ u8 panChannelWeight;
    /* 0x0C */ u8 gain;
    /* 0x0D */ u8 velocityRandomVariance;
    /* 0x0E */ u8 gateTimeRandomVariance;
    /* 0x0F */ u8 combFilterSize;
    /* 0x10 */ u16 vibratoRateStart;
    /* 0x12 */ u16 vibratoDepthStart;
    /* 0x14 */ u16 vibratoRateTarget;
    /* 0x16 */ u16 vibratoDepthTarget;
    /* 0x18 */ u16 vibratoRateChangeDelay;
    /* 0x1A */ u16 vibratoDepthChangeDelay;
    /* 0x1C */ u16 vibratoDelay;
    /* 0x1E */ u16 delay;
    /* 0x20 */ u16 combFilterGain;
    /* 0x22 */ u16 unk_22;
    /* 0x24 */ s16 instOrWave;
    /* 0x26 */ s16 transposition;
    /* 0x28 */ f32 volumeScale;
    /* 0x2C */ f32 volume;
    /* 0x30 */ s32 pan;
    /* 0x34 */ f32 appliedVolume;
    /* 0x38 */ f32 freqScale;
    /* 0x3C */ u8 (*dynTable)[][2];
    /* 0x40 */ void *noteUnused;
    /* 0x44 */ void *layerUnused;
    /* 0x48 */ void *instrument;
    /* 0x4C */ SequencePlayer *seqPlayer;
    /* 0x50 */ struct SequenceLayer *layers[8];
    /* 0x70 */ SeqScriptState scriptState;
    /* 0x8C */ AdsrSettings adsr;
    /* 0x94 */ NotePool notePool;
    /* 0xD4 */ s8 seqScriptIO[8];
    /* 0xDC */ s16 *filter;
    /* 0xE0 */ Stereo stereo;
    /* 0xE1 */ char unk_E1[3];
} SequenceChannel; /* size = 0xE4 */

/* 0x80 (AudioSeq_InitLayerFreelist: 64 x 0x80 at gAudioCtx+0x3280, list item
 * at +0x70 registered on layerFreeList). Named: the flag word (the
 * game_libs_o2_2BB7C.c donor's enabled/finished bits), adsr 0x18 (copied from
 * channel->adsr by AudioSeq_SeqChannelSetLayer), channel 0x50 (SetLayer),
 * scriptState 0x54 (2BB7C donor), listItem 0x70. The rest differs from OoT's
 * layer and stays padding until verified. */
typedef struct SequenceLayer {
    /* 0x00 */ u32 enabled : 1;
    /* 0x00 */ u32 finished : 1;
    /* 0x00 */ u32 rest : 30;
    /* 0x04 */ char unk_04[0x18 - 0x04];
    /* 0x18 */ AdsrSettings adsr;
    /* 0x20 */ char unk_20[0x50 - 0x20];
    /* 0x50 */ SequenceChannel *channel;
    /* 0x54 */ SeqScriptState scriptState;
    /* 0x70 */ AudioListItem listItem;
} SequenceLayer; /* size = 0x80 */

/* Note 0xD0 = listItem 0x0, synthesisState 0x10, playbackState 0x30,
 * sampleState 0xB0 (OoT's order; OoT's playbackState is larger). Verified by
 * Audio_InitSampleState (sampleState 0xB0, stereoHeadsetEffects 0x35),
 * Audio_NoteSetResamplingRate (bitField1 0xB1, resamplingRateFixedPoint
 * 0xBC), Audio_NoteInit (parentLayer 0x44, adsr 0x60, adsrVolScaleUnused
 * 0x36, unk_04 0x34, sampleState = gDefaultNoteSub, 32 bytes), Audio_NoteDisable
 * (needsInit / enabled / finished, priority 0x30, prev/parentLayer 0x40/0x44 =
 * NO_LAYER, adsr state, adsr.current 0x70), Audio_AllocNote (wantedParentLayer
 * 0x48), Audio_ProcessNotes (attributes 0x4C, adsr fadeOutVel 0x6C),
 * Audio_SeqLayerDecayRelease (sustain 0x64), Audio_NoteVibratoUpdate /
 * Audio_NotePortamentoInit (portamento 0x80, vibratoState 0x8C, active 0xA0,
 * portamento/vibratoFreqScale 0x38/0x3C), AudioSynth_ProcessNote
 * (NoteSampleState / NoteSynthesisState). */

/* AudioSynth_ProcessNote's synthesis state (gl_e134_SynthState). */
typedef struct NoteSynthesisState {
    /* 0x00 */ u8 restart;
    /* 0x01 */ u8 sampleDmaIndex;
    /* 0x02 */ u8 prevHaasEffectLeftDelaySize;
    /* 0x03 */ u8 prevHaasEffectRightDelaySize;
    /* 0x04 */ u8 reverbVol;
    /* 0x05 */ u8 numParts;
    /* 0x06 */ u16 samplePosFrac;
    /* 0x08 */ s32 samplePosInt;
    /* 0x0C */ void *synthesisBuffers;
    /* 0x10 */ s16 curVolLeft;
    /* 0x12 */ s16 curVolRight;
    /* 0x14 */ char unk_14[6];
    /* 0x1A */ u8 combFilterNeedsInit;
    /* 0x1B */ char unk_1B[5];
} NoteSynthesisState; /* size = 0x20 */

/* 0x14 (Audio_ProcessNotes gl_27E24_Attrs, Audio_SeqLayerDecayRelease). Older
 * than OoT: no filterBuf, combFilterGain is the u8 and combFilterSize the u16. */
typedef struct NoteAttributes {
    /* 0x00 */ u8 reverb;
    /* 0x01 */ u8 gain;
    /* 0x02 */ u8 pan;
    /* 0x03 */ Stereo stereo;
    /* 0x04 */ u8 combFilterGain;
    /* 0x05 */ char unk_05;
    /* 0x06 */ u16 combFilterSize;
    /* 0x08 */ f32 freqScale;
    /* 0x0C */ f32 velocity;
    /* 0x10 */ s16 *filter;
} NoteAttributes; /* size = 0x14 */

/* 0x20, OoT's AdsrState. Audio_AdsrInit (action 0x0, delay 0x2, sustain 0x4,
 * current 0x10, envelope 0x1C), Audio_SeqLayerDecayRelease / AudioHeap_ResetStep
 * (action bits, fadeOutVel 0xC). */
typedef struct AdsrState {
    /* 0x00 */ union {
        struct {
            u8 unk_bit0 : 1;
            u8 hang : 1;
            u8 decay : 1;
            u8 release : 1;
            u8 state : 4;
        } s;
        u8 asByte;
    } action;
    /* 0x01 */ u8 envIndex;
    /* 0x02 */ s16 delay;
    /* 0x04 */ f32 sustain;
    /* 0x08 */ f32 velocity;
    /* 0x0C */ f32 fadeOutVel;
    /* 0x10 */ f32 current;
    /* 0x14 */ f32 target;
    /* 0x18 */ char unk_18[4];
    /* 0x1C */ void *envelope;
} AdsrState; /* size = 0x20 */

/* 0xC (Audio_GetPortamentoFreqScale: mode u8, cur / speed u16, extent f32;
 * Audio_NotePortamentoInit copies the layer's 12 bytes). SM64-era form. */
typedef struct Portamento {
    /* 0x0 */ u8 mode;
    /* 0x2 */ u16 cur;
    /* 0x4 */ u16 speed;
    /* 0x8 */ f32 extent;
} Portamento; /* size = 0xC */

/* 0x1C, OoT's VibratoState (Audio_GetVibratoFreqScale donor gl_29978_*,
 * Audio_GetVibratoPitchChange time / curve / rate). */
typedef struct VibratoState {
    /* 0x00 */ SequenceChannel *channel;
    /* 0x04 */ u32 time;
    /* 0x08 */ s16 *curve;
    /* 0x0C */ f32 depth;
    /* 0x10 */ f32 rate;
    /* 0x14 */ u8 active;
    /* 0x16 */ u16 rateChangeTimer;
    /* 0x18 */ u16 depthChangeTimer;
    /* 0x1A */ u16 delay;
} VibratoState; /* size = 0x1C */

typedef struct NotePlaybackState {
    /* 0x00 */ u8 priority;
    /* 0x01 */ u8 waveId;
    /* 0x02 */ u8 harmonicIndex;
    /* 0x03 */ u8 fontId;
    /* 0x04 */ u8 unk_04;
    /* 0x05 */ u8 stereoHeadsetEffects;
    /* 0x06 */ s16 adsrVolScaleUnused;
    /* 0x08 */ f32 portamentoFreqScale;
    /* 0x0C */ f32 vibratoFreqScale;
    /* 0x10 */ SequenceLayer *prevParentLayer;
    /* 0x14 */ SequenceLayer *parentLayer;
    /* 0x18 */ SequenceLayer *wantedParentLayer;
    /* 0x1C */ NoteAttributes attributes;
    /* 0x30 */ AdsrState adsr;
    /* 0x50 */ Portamento portamento;
    /* 0x5C */ VibratoState vibratoState;
    /* 0x78 */ char unk_78[8];
} NotePlaybackState; /* size = 0x80 */

/* 0x20 (the 0x18 named by AudioSynth_ProcessNote, then padding to the note
 * end; Audio_NoteInit copies 32 bytes). The flag bits are a u32 bitfield run
 * as most matched donors declare them (AudioSynth_ProcessNote,
 * Audio_ProcessNotes, AudioThread_ProcessGlobalCmd, AudioHeap_ResetStep);
 * Audio_InitSampleState's donor uses u8 containers -- the container type
 * steers IDO's loads, so keep the donor's own when converting. */
typedef struct NoteSampleState {
    /* 0x00 */ u32 enabled : 1;
    /* 0x00 */ u32 needsInit : 1;
    /* 0x00 */ u32 finished : 1;
    /* 0x00 */ u32 unused : 1;
    /* 0x00 */ u32 stereoStrongRight : 1;
    /* 0x00 */ u32 stereoStrongLeft : 1;
    /* 0x00 */ u32 stereoHeadsetEffects : 1;
    /* 0x00 */ u32 usesHeadsetPanEffects : 1;
    /* 0x01 */ u32 reverbIndex : 3;
    /* 0x01 */ u32 bookOffset : 2;
    /* 0x01 */ u32 isSyntheticWave : 1;
    /* 0x01 */ u32 hasTwoParts : 1;
    /* 0x01 */ u32 useHaasEffect : 1;
    /* 0x02 */ u8 gain;
    /* 0x03 */ u8 haasEffectLeftDelaySize;
    /* 0x04 */ u8 haasEffectRightDelaySize;
    /* 0x05 */ u8 reverbVol;
    /* 0x06 */ u8 harmonicIndexCurAndPrev;
    /* 0x07 */ u8 combFilterSize;
    /* 0x08 */ u16 targetVolLeft;
    /* 0x0A */ u16 targetVolRight;
    /* 0x0C */ u16 resamplingRateFixedPoint;
    /* 0x0E */ u16 combFilterGain;
    /* 0x10 */ TunedSample *tunedSample;
    /* 0x14 */ s16 *filter;
    /* 0x18 */ char unk_18[8];
} NoteSampleState; /* size = 0x20 */

typedef struct Note {
    /* 0x00 */ AudioListItem listItem;
    /* 0x10 */ NoteSynthesisState synthesisState;
    /* 0x30 */ NotePlaybackState playbackState;
    /* 0xB0 */ NoteSampleState sampleState;
} Note; /* size = 0xD0 */

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
    /* 0x1E14 */ s32 unused1E14;                     /* OoT unused2628 (DecreaseSampleDmaTtls = 0) */
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
    /* 0x2CFC */ Note *notes;                         /* Note[numNotes] */
    /* 0x2D00 */ SequencePlayer seqPlayers[4];
    /* 0x3280 */ SequenceLayer sequenceLayers[64];   /* AudioSeq_InitLayerFreelist */
    /* 0x5280 */ SequenceChannel sequenceChannelNone; /* InitSequenceChannel early-out, InitSequencePlayer */
    /* 0x5364 */ s32 noteSubEuOffset;                 /* AudioSeq_ProcessSequences */
    /* 0x5368 */ AudioListItem layerFreeList;         /* InitLayerFreelist, SeqChannelSetLayer */
    /* 0x5378 */ NotePool noteFreeLists;              /* NotePoolClear, AllocNote */
    /* 0x53B8 */ u8 threadCmdWritePos;                /* AudioThread_QueueCmd */
    /* 0x53B9 */ u8 threadCmdReadPos;
    /* 0x53BA */ u8 threadCmdQueueFinished;
    /* 0x53BB */ char unk_53BB;
    /* 0x53BC */ u16 threadCmdChannelMask[4];         /* ProcessGlobalCmd */
    /* 0x53C4 */ void *audioResetQueueP;              /* InitMesgQueuesImpl (game_libs_o2_26214.c view) */
    /* 0x53C8 */ void *taskStartQueueP;
    /* 0x53CC */ void *threadCmdProcQueueP;           /* ScheduleProcessCmds */
} AudioContext; /* size >= 0x53D0 (later fields not yet modelled) */

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
AUDIO_ASSERT_OFFSET(sequenceLayers, 0x3280);
AUDIO_ASSERT_OFFSET(sequenceChannelNone, 0x5280);
AUDIO_ASSERT_OFFSET(noteSubEuOffset, 0x5364);
AUDIO_ASSERT_OFFSET(layerFreeList, 0x5368);
AUDIO_ASSERT_OFFSET(noteFreeLists, 0x5378);
AUDIO_ASSERT_OFFSET(threadCmdChannelMask, 0x53BC);
AUDIO_ASSERT_OFFSET(threadCmdProcQueueP, 0x53CC);
typedef char audio_pool_size_assert[sizeof(AudioAllocPool) == 0x10 ? 1 : -1];
typedef char audio_reverb_size_assert[sizeof(SynthesisReverb) == 0x158 ? 1 : -1];
typedef char audio_seqplayer_size_assert[sizeof(SequencePlayer) == 0x160 ? 1 : -1];
typedef char audio_listitem_size_assert[sizeof(AudioListItem) == 0x10 ? 1 : -1];
typedef char audio_notepool_size_assert[sizeof(NotePool) == 0x40 ? 1 : -1];
typedef char audio_scriptstate_size_assert[sizeof(SeqScriptState) == 0x1C ? 1 : -1];
typedef char audio_channel_size_assert[sizeof(SequenceChannel) == 0xE4 ? 1 : -1];
typedef char audio_layer_size_assert[sizeof(SequenceLayer) == 0x80 ? 1 : -1];
typedef char audio_ringitem_size_assert[sizeof(ReverbRingBufferItem) == 0x14 ? 1 : -1];
typedef char audio_sample_size_assert[sizeof(Sample) == 0x10 ? 1 : -1];
typedef char audio_adpcmloop_size_assert[sizeof(AdpcmLoop) == 0x30 ? 1 : -1];
typedef char audio_synthstate_size_assert[sizeof(NoteSynthesisState) == 0x20 ? 1 : -1];
typedef char audio_attrs_size_assert[sizeof(NoteAttributes) == 0x14 ? 1 : -1];
typedef char audio_adsrstate_size_assert[sizeof(AdsrState) == 0x20 ? 1 : -1];
typedef char audio_portamento_size_assert[sizeof(Portamento) == 0xC ? 1 : -1];
typedef char audio_vibrato_size_assert[sizeof(VibratoState) == 0x1C ? 1 : -1];
typedef char audio_playback_size_assert[sizeof(NotePlaybackState) == 0x80 ? 1 : -1];
typedef char audio_samplestate_size_assert[sizeof(NoteSampleState) == 0x20 ? 1 : -1];
typedef char audio_note_size_assert[sizeof(Note) == 0xD0 ? 1 : -1];
#define AUDIO_ASSERT_MEMBER(type, field, off) \
    typedef char audio_##type##_##field##_assert[(int)&((type *)0)->field == (off) ? 1 : -1]
AUDIO_ASSERT_MEMBER(SequencePlayer, tempo, 0x008);
AUDIO_ASSERT_MEMBER(SequencePlayer, seqData, 0x018);
AUDIO_ASSERT_MEMBER(SequencePlayer, bend, 0x034);
AUDIO_ASSERT_MEMBER(SequencePlayer, channels, 0x038);
AUDIO_ASSERT_MEMBER(SequencePlayer, scriptState, 0x078);
AUDIO_ASSERT_MEMBER(SequencePlayer, shortNoteVelocityTable, 0x094);
AUDIO_ASSERT_MEMBER(SequencePlayer, notePool, 0x09C);
AUDIO_ASSERT_MEMBER(SequencePlayer, skipTicks, 0x0DC);
AUDIO_ASSERT_MEMBER(SequencePlayer, scriptCounter, 0x0E0);
AUDIO_ASSERT_MEMBER(SequencePlayer, seqScriptIO, 0x158);
AUDIO_ASSERT_MEMBER(SequenceChannel, noteAllocPolicy, 0x02);
AUDIO_ASSERT_MEMBER(SequenceChannel, vibratoRateStart, 0x10);
AUDIO_ASSERT_MEMBER(SequenceChannel, transposition, 0x26);
AUDIO_ASSERT_MEMBER(SequenceChannel, freqScale, 0x38);
AUDIO_ASSERT_MEMBER(SequenceChannel, seqPlayer, 0x4C);
AUDIO_ASSERT_MEMBER(SequenceChannel, layers, 0x50);
AUDIO_ASSERT_MEMBER(SequenceChannel, scriptState, 0x70);
AUDIO_ASSERT_MEMBER(SequenceChannel, adsr, 0x8C);
AUDIO_ASSERT_MEMBER(SequenceChannel, notePool, 0x94);
AUDIO_ASSERT_MEMBER(SequenceChannel, seqScriptIO, 0xD4);
AUDIO_ASSERT_MEMBER(SequenceChannel, filter, 0xDC);
AUDIO_ASSERT_MEMBER(SequenceChannel, stereo, 0xE0);
AUDIO_ASSERT_MEMBER(SequenceLayer, adsr, 0x18);
AUDIO_ASSERT_MEMBER(SequenceLayer, channel, 0x50);
AUDIO_ASSERT_MEMBER(SequenceLayer, scriptState, 0x54);
AUDIO_ASSERT_MEMBER(SequenceLayer, listItem, 0x70);
AUDIO_ASSERT_MEMBER(SynthesisReverb, windowSize, 0x006);
AUDIO_ASSERT_MEMBER(SynthesisReverb, leakRtl, 0x010);
AUDIO_ASSERT_MEMBER(SynthesisReverb, nextRingBufPos, 0x014);
AUDIO_ASSERT_MEMBER(SynthesisReverb, leftRingBuf, 0x020);
AUDIO_ASSERT_MEMBER(SynthesisReverb, items, 0x038);
AUDIO_ASSERT_MEMBER(SynthesisReverb, filterLeft, 0x100);
AUDIO_ASSERT_MEMBER(SynthesisReverb, tunedSample, 0x110);
AUDIO_ASSERT_MEMBER(SynthesisReverb, sample, 0x118);
AUDIO_ASSERT_MEMBER(SynthesisReverb, loop, 0x128);
AUDIO_ASSERT_MEMBER(NoteSynthesisState, curVolLeft, 0x10);
AUDIO_ASSERT_MEMBER(NoteSynthesisState, combFilterNeedsInit, 0x1A);
AUDIO_ASSERT_MEMBER(NotePlaybackState, adsrVolScaleUnused, 0x06);
AUDIO_ASSERT_MEMBER(NotePlaybackState, prevParentLayer, 0x10);
AUDIO_ASSERT_MEMBER(NotePlaybackState, attributes, 0x1C);
AUDIO_ASSERT_MEMBER(NotePlaybackState, adsr, 0x30);
AUDIO_ASSERT_MEMBER(NotePlaybackState, portamento, 0x50);
AUDIO_ASSERT_MEMBER(NotePlaybackState, vibratoState, 0x5C);
AUDIO_ASSERT_MEMBER(AdsrState, fadeOutVel, 0x0C);
AUDIO_ASSERT_MEMBER(AdsrState, envelope, 0x1C);
AUDIO_ASSERT_MEMBER(VibratoState, active, 0x14);
AUDIO_ASSERT_MEMBER(VibratoState, delay, 0x1A);
AUDIO_ASSERT_MEMBER(NoteSampleState, gain, 0x02);
AUDIO_ASSERT_MEMBER(NoteSampleState, resamplingRateFixedPoint, 0x0C);
AUDIO_ASSERT_MEMBER(NoteSampleState, tunedSample, 0x10);
AUDIO_ASSERT_MEMBER(Note, synthesisState, 0x10);
AUDIO_ASSERT_MEMBER(Note, playbackState, 0x30);
AUDIO_ASSERT_MEMBER(Note, sampleState, 0xB0);

/*
 * SoundObj: the game-side 0x64-byte sound object of the glue layer (game_libs 0x2DF98..0x31608,
 * agent-c runs 455 / 464). bootup 0x57C allocates it (mem_new(0x64)) and calls
 * snd_SoundObjInit(o, kind); bootup snd_updatePlayer drives it each frame through
 * snd_SoundObjSetAngle and snd_SoundObjUpdate. bootup keeps it in a SndHandle
 * (obj at +0, the event argument at +8) and the one-call wrappers 0x85C..0xA94 forward to the
 * snd_SoundObj* functions. Unnamed fields are named by offset.
 */
typedef struct SoundObj {
    f32 speed;              /* 0x00: snd_SoundObjTriggerEvent picks level 1/2/3 at 25 / 38 / 50 */
    f32 unk04;              /* 0x04 */
    f32 unk08;              /* 0x08 */
    f32 unk0C;              /* 0x0C: 0..1 */
    f32 unk10;              /* 0x10 */
    u8 unk14;               /* 0x14: 1 -> Start plays event 0x29, else 0x2A */
    u8 unk15;               /* 0x15 */
    u8 flags;               /* 0x16: bit0 disabled, bit2 event pending, bit5 started (Start / Stop latch) */
    u8 unk17;               /* 0x17 */
    s8 unk18;               /* 0x18 */
    u8 pad19;
    s16 unk1A;              /* 0x1A: snd_SoundObjSetFlag1A */
    u8 flags1C;             /* 0x1C: bit0 = positional (Start does nothing) */
    u8 unk1D;               /* 0x1D */
    u8 unk1E;               /* 0x1E */
    u8 unk1F;               /* 0x1F: bootup 0xA50 */
    u8 fired;               /* 0x20 */
    u8 kind;                /* 0x21: 0 -> player-0 channel 3, else 7; 2 = Stop requests no cue */
    u8 surface;             /* 0x22: with kind, the cue key of snd_SoundObjRequestCue; Update switches on it */
    u8 unk23;               /* 0x23 */
    u8 pad24[2];
    u8 unk26;               /* 0x26 */
    u8 pad27;
    u8 unk28;               /* 0x28: 0x40 at init; Start's last argument */
    u8 level;               /* 0x29 */
    u8 unk2A;               /* 0x2A: Update stores sp6B here */
    u8 unk2B;               /* 0x2B */
    u8 unk2C;               /* 0x2C */
    u8 lastLevel;           /* 0x2D */
    u8 eventArg;            /* 0x2E */
    u8 pad2F;
    u16 eventType;          /* 0x30: low byte 0x31 read by snd_SoundObjEventIndex */
    u8 scaleIdx;            /* 0x32: freq / vol multiplier table index (15 / 31) */
    u8 unk33;               /* 0x33 */
    u8 unk34;               /* 0x34 */
    u8 pad35[3];
    s32 unk38;              /* 0x38 */
    u8 pad3C[8];
    f32 unk44;              /* 0x44 */
    s32 unk48;              /* 0x48 */
    u8 pad4C[4];
    f32 unk50;              /* 0x50 */
    f32 volScale;           /* 0x54: Play multiplies the volume by it when flags1C bit0 */
    f32 unk58;              /* 0x58 */
    f32 unk5C;              /* 0x5C */
    u8 pan[4];              /* 0x60: pan bytes (SetAngle writes 0x60) */
} SoundObj;

AUDIO_ASSERT_MEMBER(SoundObj, flags, 0x16);
AUDIO_ASSERT_MEMBER(SoundObj, kind, 0x21);
AUDIO_ASSERT_MEMBER(SoundObj, eventType, 0x30);
AUDIO_ASSERT_MEMBER(SoundObj, unk48, 0x48);
AUDIO_ASSERT_MEMBER(SoundObj, volScale, 0x54);
AUDIO_ASSERT_MEMBER(SoundObj, pan, 0x60);
typedef char audio_soundobj_size_assert[sizeof(SoundObj) == 0x64 ? 1 : -1];

/* bootup's handle around a SoundObj (the bootup wrappers 0x85C..0xA94 take it) */
typedef struct SndHandle {
    SoundObj *obj;          /* 0x0: NULL -> the wrappers do nothing */
    s32 unk04;              /* 0x4 */
    s32 contactSurface;     /* 0x8: the rider's SnowBoarder.contactSurface (surface code of the last
                             * obstacle hit), set each frame by SnowBoarder_update via 0xA94; the
                             * 0x8F4..0x9D8 triggers map it to a SoundObj event type (0xA9C) */
} SndHandle;

/* The game's music cue state: bootup Bss 0x338, Sym 1448 (gl_data.h gSndCueQueues), 0x160 bytes.
 * Two kinds of reference reach it (scripts/uso-data-sites.py object 1448):
 * - relocations against Sym 1448 itself (offset = field): snd_TriggerChannel8Cue,
 *   snd_TriggerChannel8CueIfIdle, snd_TickChannel8Fade, snd_SetChannel8Mode, snd_HandleSeqEvent_30AF4.
 *   Use SND_CUEQ(gSndCueQueues) there.
 * - relocations against the Bss section base (offset = 0x338 + field): snd_RequestCue / Cue2 /
 *   Cue2Prio, snd_InitCueQueues, snd_SoundObjRequestCue, snd_SetPlayer2ChannelIO_3190C,
 *   snd_UpdateTimers. Those must keep the base symbol; SND_CUEQ_AT_BASE(D_00000000) is the view.
 * agent-c run 495. */
typedef struct SndCueQueue {
    char pad00[0x40];
    s32 unk40;              /* 0x40..0x4C, 0x5C: cleared by snd_InitCueQueues */
    s32 unk44;
    s32 unk48;
    s32 unk4C;
    char pad50[0x5C - 0x50];
    s32 unk5C;
    s16 unk60;              /* snd_SetPlayer2ChannelIO_3190C reads +0x60 / +0x63 */
    s16 cue;                /* 0x62: queue index (snd_InitCueQueues), then the requested cue
                             * (snd_SoundObjRequestCue); requests go to func 0x45DC0(queue, 1, id, 0x7F) */
} SndCueQueue;              /* 0x64 */

typedef struct SndCueQueues {
    /* channel 8 (the music cue channel): */
    /* 0x00 */ s32 mode;          /* snd_SetChannel8Mode; 4 = cues ignored, 2 = remaps cue ids 0/1/3..5 */
    /* 0x04 */ s32 cue;           /* current cue: sent as audio cmd 0x06000800 (channel IO), (s8) */
    /* 0x08 */ s32 fadeInFrames;  /* count down while level ramps up by upStep */
    /* 0x0C */ s32 holdFrames;    /* then count down */
    /* 0x10 */ f32 level;         /* sent as audio cmd 0x01000800 (channel volume) every tick */
    /* 0x14 */ f32 floor;         /* fade-down floor */
    /* 0x18 */ f32 upStep;        /* (1 - level) / fadeInFrames */
    /* 0x1C */ f32 downStep;
    /* 0x20 */ s32 timer[3];      /* snd_UpdateTimers */
    char pad2C[0x30 - 0x2C];
    /* 0x30 */ SndCueQueue queue[3]; /* snd_RequestCue(i) = queue[i]; snd_RequestCue2 = queue[2] */
    char pad15C[0x160 - 0x15C];
} SndCueQueues;
AUDIO_ASSERT_MEMBER(SndCueQueue, cue, 0x62);
AUDIO_ASSERT_MEMBER(SndCueQueues, downStep, 0x1C);
AUDIO_ASSERT_MEMBER(SndCueQueues, queue, 0x30);
typedef char audio_sndcuequeues_size_assert[sizeof(SndCueQueues) == 0x160 ? 1 : -1];
#define SND_CUEQ(sym)          (*(SndCueQueues *)&(sym))
#define SND_CUEQ_AT_BASE(sym)  (*(SndCueQueues *)((char *)&(sym) + 0x338))

#endif /* AUDIO_H */
