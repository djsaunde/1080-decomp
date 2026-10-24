#include "common.h"

/* IDO 7.1 -O2 donor for gl_func_0002BB7C (0xC28 / 778 words) = OoT's
 * AudioSeq_SequenceChannelProcessScript (seqplayer.c) in its older form: eight
 * layers (SM64 LAYERS_MAX, so the scriptState sits at +0x70 and every later
 * member is 0x10 above OoT), no 0xBD RANDPTR, a one-form LDSAMPLE. Spliced into
 * game_libs_post.c.o via REPLACE_FUNC_BODY; the splice renames the donor-local
 * .rodata jumptable relocs (0xB0..0xF1 table, 0x70..0x98 layer table, 0x00..0x60
 * low-nibble table) to gl_func_0002BB7C_rodata, pinned to the USO tables at
 * +0x10B0. The DYNCALL push is written as an s32 copy of the depth, a store, then
 * a separate increment (OoT's stack[depth++] colours the depth t3, target v1). */
typedef union { struct { u8 unused : 2; u8 bit2 : 2; u8 strongRight : 1; u8 strongLeft : 1; u8 stereoHeadsetEffects : 1; u8 usesHeadsetPanEffects : 1; } s; u8 asByte; } gl_2BB7C_Stereo;
typedef struct { u8 *pc; u8 *stack[4]; u8 remLoopIters[4]; u8 depth; s8 value; } gl_2BB7C_SeqScriptState;
typedef struct { u8 decayIndex; u8 sustain; void *envelope; } gl_2BB7C_AdsrSettings;
typedef struct { u32 enabled : 1; u32 finished : 1; u32 rest : 30; char pad4[0x54 - 4]; gl_2BB7C_SeqScriptState scriptState; } gl_2BB7C_Layer;
typedef struct gl_2BB7C_Channel gl_2BB7C_Channel;
typedef struct {
    u32 enabled : 1; u32 finished : 1; u32 muted : 1; u32 rest : 29;
    u8 seqId; u8 defaultFont; char pad6[0x18 - 6];
    u8 *seqData; char pad1c[0x38 - 0x1C]; gl_2BB7C_Channel *channels[16];
} gl_2BB7C_SeqPlayer;
struct gl_2BB7C_Channel {
    u32 enabled : 1; u32 finished : 1; u32 stopScript : 1; u32 stopSomething2 : 1;
    u32 hasInstrument : 1; u32 stereoHeadsetEffects : 1; u32 largeNotes : 1; u32 unused : 1;
    u32 changesFreqScale : 1; u32 changesVolume : 1; u32 changesPan : 1; u32 changesRest : 5;
    u8 noteAllocPolicy; u8 muteBehavior; u8 targetReverbVol; u8 notePriority; u8 someOtherPriority;
    u8 fontId; u8 reverbIndex; u8 bookOffset; u8 newPan; u8 panChannelWeight; u8 gain;
    u8 velocityRandomVariance; u8 gateTimeRandomVariance; u8 combFilterSize;
    u16 vibratoRateStart; u16 vibratoDepthStart; u16 vibratoRateTarget; u16 vibratoDepthTarget;
    u16 vibratoRateChangeDelay; u16 vibratoDepthChangeDelay; u16 vibratoDelay; u16 delay;
    u16 combFilterGain; u16 unk_22; s16 instOrWave; s16 transposition;
    f32 volumeScale; f32 volume; s32 pan; f32 appliedVolume; f32 freqScale;
    u8 (*dynTable)[][2]; void *noteUnused; void *layerUnused; void *instrument;
    gl_2BB7C_SeqPlayer *seqPlayer; gl_2BB7C_Layer *layers[8];
    gl_2BB7C_SeqScriptState scriptState; gl_2BB7C_AdsrSettings adsr; char notePool[0x40];
    s8 seqScriptIO[8]; s16 *filter; gl_2BB7C_Stereo stereo;
};
typedef struct { char pad[0x2028]; u8 *sequenceFontTable; char pad202c[0x2154 - 0x202C]; u32 audioRandom; } gl_2BB7C_Ctx;
extern gl_2BB7C_Ctx gl_2BB7C_gAudioCtx;
#define gAudioCtx gl_2BB7C_gAudioCtx
extern u8 gl_2BB7C_sSeqInstructionArgsTable[];
extern f32 gl_2BB7C_gBendPitchOneOctaveFrequencies[];
extern f32 gl_2BB7C_gBendPitchTwoSemitonesFrequencies[];
extern int gl_func_00000000();
extern u8 gl_func_0003F010(gl_2BB7C_SeqScriptState *);
extern s16 gl_func_0003F024(gl_2BB7C_SeqScriptState *);
extern s32 gl_func_0003E6E8(gl_2BB7C_SeqPlayer *, gl_2BB7C_SeqScriptState *, s32, s32);
extern s32 gl_func_0003EA18(gl_2BB7C_Channel *, s32);
extern void gl_func_0003EB78(gl_2BB7C_Channel *, s32);
extern void gl_func_0003EDAC(gl_2BB7C_SeqPlayer *, u8, void *);
extern void gl_func_0003F09C(gl_2BB7C_Layer *);
extern void gl_func_00040074(gl_2BB7C_Channel *, u8);
extern void gl_func_00040118(gl_2BB7C_Channel *, u8);
extern void gl_2BB7C_SequenceChannelDisable(gl_2BB7C_Channel *);
extern void gl_2BB7C_NotePoolClear(void *);
extern void gl_2BB7C_NotePoolFill(void *, s32);
extern void *gl_2BB7C_SearchCaches(s32, s32, s32);
extern void gl_2BB7C_SequenceChannelSetVolume(gl_2BB7C_Channel *, u8);
extern void gl_2BB7C_LoadFilter(s16 *, s32, s32);
extern s32 gl_2BB7C_SlowLoadSample(s32, u8, s8 *);
#define sSeqInstructionArgsTable gl_2BB7C_sSeqInstructionArgsTable
#define gBendPitchOneOctaveFrequencies gl_2BB7C_gBendPitchOneOctaveFrequencies
#define gBendPitchTwoSemitonesFrequencies gl_2BB7C_gBendPitchTwoSemitonesFrequencies
#define AudioSeq_ScriptReadU8 gl_func_0003F010
#define AudioSeq_ScriptReadS16 gl_func_0003F024
#define AudioSeq_HandleScriptFlowControl gl_func_0003E6E8
#define AudioSeq_SeqChannelSetLayer gl_func_0003EA18
#define AudioSeq_SeqLayerFree gl_func_0003EB78
#define AudioSeq_SequenceChannelEnable gl_func_0003EDAC
#define AudioSeq_SeqLayerProcessScript gl_func_0003F09C
#define AudioSeq_SetChannelPriorities gl_func_00040074
#define AudioSeq_SetInstrument gl_func_00040118
#define AudioSeq_SequenceChannelDisable gl_2BB7C_SequenceChannelDisable
#define Audio_NotePoolClear gl_2BB7C_NotePoolClear
#define Audio_NotePoolFill gl_2BB7C_NotePoolFill
#define AudioHeap_SearchCaches gl_2BB7C_SearchCaches
#define AudioSeq_SequenceChannelSetVolume gl_2BB7C_SequenceChannelSetVolume
#define AudioHeap_LoadFilter gl_2BB7C_LoadFilter
#define AudioLoad_SlowLoadSample gl_2BB7C_SlowLoadSample
#define true 1
#define false 0

void gl_func_0002BB7C(gl_2BB7C_Channel *channel) {
    s32 i;
    u8 *data;
    u8 *seqData;
    gl_2BB7C_SeqPlayer *seqPlayer;

    if (channel->stopScript) {
        goto exit_loop;
    }

    seqPlayer = channel->seqPlayer;
    if (seqPlayer->muted && (channel->muteBehavior & 0x80)) {
        return;
    }

    if (channel->delay >= 2) {
        channel->delay--;
        goto exit_loop;
    }

    while (true) {
        gl_2BB7C_SeqScriptState *scriptState = &channel->scriptState;
        s32 param;
        s16 temp1;
        u16 cmdArgU16;
        u32 cmdArgs[3];
        s8 cmdArgS8;
        u8 cmd = AudioSeq_ScriptReadU8(scriptState);
        u8 lowBits;
        u8 highBits;
        s32 delay;
        s32 temp2;

        if (cmd >= 0xB0) {
            highBits = sSeqInstructionArgsTable[cmd - 0xB0];
            lowBits = highBits & 3;

            for (i = 0; i < lowBits; i++, highBits <<= 1) {
                if (!(highBits & 0x80)) {
                    cmdArgs[i] = AudioSeq_ScriptReadU8(scriptState);
                } else {
                    cmdArgs[i] = AudioSeq_ScriptReadS16(scriptState);
                }
            }

            if (cmd >= 0xF2) {
                delay = AudioSeq_HandleScriptFlowControl(seqPlayer, scriptState, cmd, cmdArgs[0]);

                if (delay != 0) {
                    if (delay == -1) {
                        AudioSeq_SequenceChannelDisable(channel);
                    } else {
                        channel->delay = delay;
                    }
                    break;
                }
                continue;
            }

            switch (cmd) {
                case 0xEA:
                    channel->stopScript = true;
                    goto exit_loop;

                case 0xF1:
                    Audio_NotePoolClear(&channel->notePool);
                    cmd = (u8)cmdArgs[0];
                    Audio_NotePoolFill(&channel->notePool, cmd);
                    break;

                case 0xF0:
                    Audio_NotePoolClear(&channel->notePool);
                    break;

                case 0xC2:
                    cmdArgU16 = (u16)cmdArgs[0];
                    channel->dynTable = (void *)&seqPlayer->seqData[cmdArgU16];
                    break;

                case 0xC5:
                    if (scriptState->value != -1) {
                        data = (*channel->dynTable)[scriptState->value];
                        cmdArgU16 = (u16)((data[0] << 8) + data[1]);

                        channel->dynTable = (void *)&seqPlayer->seqData[cmdArgU16];
                    }
                    break;

                case 0xEB:
                    cmd = (u8)cmdArgs[0];

                    if (seqPlayer->defaultFont != 0xFF) {
                        cmdArgU16 = ((u16 *)gAudioCtx.sequenceFontTable)[seqPlayer->seqId];
                        lowBits = gAudioCtx.sequenceFontTable[cmdArgU16];
                        cmd = gAudioCtx.sequenceFontTable[cmdArgU16 + lowBits - cmd];
                    }

                    if (AudioHeap_SearchCaches(1, 2, cmd)) {
                        channel->fontId = cmd;
                    }

                    cmdArgs[0] = cmdArgs[1];
                    /* fallthrough */
                case 0xC1:
                    cmd = (u8)cmdArgs[0];
                    AudioSeq_SetInstrument(channel, cmd);
                    break;

                case 0xC3:
                    channel->largeNotes = false;
                    break;

                case 0xC4:
                    channel->largeNotes = true;
                    break;

                case 0xDF:
                    cmd = (u8)cmdArgs[0];
                    AudioSeq_SequenceChannelSetVolume(channel, cmd);
                    channel->changesVolume = true;
                    break;

                case 0xE0:
                    cmd = (u8)cmdArgs[0];
                    channel->volumeScale = (s32)cmd / 128.0f;
                    channel->changesVolume = true;
                    break;

                case 0xDE:
                    cmdArgU16 = (u16)cmdArgs[0];
                    channel->freqScale = (s32)cmdArgU16 / 32768.0f;
                    channel->changesFreqScale = true;
                    break;

                case 0xD3:
                    cmd = (u8)cmdArgs[0];
                    cmd += 0x80;
                    channel->freqScale = gBendPitchOneOctaveFrequencies[cmd];
                    channel->changesFreqScale = true;
                    break;

                case 0xEE:
                    cmd = (u8)cmdArgs[0];
                    cmd += 0x80;
                    channel->freqScale = gBendPitchTwoSemitonesFrequencies[cmd];
                    channel->changesFreqScale = true;
                    break;

                case 0xDD:
                    cmd = (u8)cmdArgs[0];
                    channel->newPan = cmd;
                    channel->changesPan = true;
                    break;

                case 0xDC:
                    cmd = (u8)cmdArgs[0];
                    channel->panChannelWeight = cmd;
                    channel->changesPan = true;
                    break;

                case 0xDB:
                    cmdArgS8 = (s8)cmdArgs[0];
                    channel->transposition = cmdArgS8;
                    break;

                case 0xDA:
                    cmdArgU16 = (u16)cmdArgs[0];
                    channel->adsr.envelope = (void *)&seqPlayer->seqData[cmdArgU16];
                    break;

                case 0xD9:
                    cmd = (u8)cmdArgs[0];
                    channel->adsr.decayIndex = cmd;
                    break;

                case 0xD8:
                    cmd = (u8)cmdArgs[0];
                    channel->vibratoDepthTarget = cmd * 8;
                    channel->vibratoDepthStart = 0;
                    channel->vibratoDepthChangeDelay = 0;
                    break;

                case 0xD7:
                    cmd = (u8)cmdArgs[0];
                    channel->vibratoRateChangeDelay = 0;
                    channel->vibratoRateTarget = cmd * 32;
                    channel->vibratoRateStart = cmd * 32;
                    break;

                case 0xE2:
                    cmd = (u8)cmdArgs[0];
                    channel->vibratoDepthStart = cmd * 8;
                    cmd = (u8)cmdArgs[1];
                    channel->vibratoDepthTarget = cmd * 8;
                    cmd = (u8)cmdArgs[2];
                    channel->vibratoDepthChangeDelay = cmd * 16;
                    break;

                case 0xE1:
                    cmd = (u8)cmdArgs[0];
                    channel->vibratoRateStart = cmd * 32;
                    cmd = (u8)cmdArgs[1];
                    channel->vibratoRateTarget = cmd * 32;
                    cmd = (u8)cmdArgs[2];
                    channel->vibratoRateChangeDelay = cmd * 16;
                    break;

                case 0xE3:
                    cmd = (u8)cmdArgs[0];
                    channel->vibratoDelay = cmd * 16;
                    break;

                case 0xD4:
                    cmd = (u8)cmdArgs[0];
                    channel->targetReverbVol = cmd;
                    break;

                case 0xC6:
                    cmd = (u8)cmdArgs[0];

                    if (seqPlayer->defaultFont != 0xFF) {
                        cmdArgU16 = ((u16 *)gAudioCtx.sequenceFontTable)[seqPlayer->seqId];
                        lowBits = gAudioCtx.sequenceFontTable[cmdArgU16];
                        cmd = gAudioCtx.sequenceFontTable[cmdArgU16 + lowBits - cmd];
                    }

                    if (AudioHeap_SearchCaches(1, 2, cmd)) {
                        channel->fontId = cmd;
                    }
                    break;

                case 0xC7:
                    cmd = (u8)cmdArgs[0];
                    cmdArgU16 = (u16)cmdArgs[1];
                    seqData = &seqPlayer->seqData[cmdArgU16];
                    seqData[0] = (u8)scriptState->value + cmd;
                    break;

                case 0xC8:
                case 0xCC:
                case 0xC9:
                    cmdArgS8 = (s8)cmdArgs[0];

                    if (cmd == 0xC8) {
                        scriptState->value -= cmdArgS8;
                    } else if (cmd == 0xCC) {
                        scriptState->value = cmdArgS8;
                    } else {
                        scriptState->value &= cmdArgS8;
                    }
                    break;

                case 0xCD:
                    cmd = (u8)cmdArgs[0];
                    AudioSeq_SequenceChannelDisable(seqPlayer->channels[cmd]);
                    break;

                case 0xCA:
                    cmd = (u8)cmdArgs[0];
                    channel->muteBehavior = cmd;
                    channel->changesVolume = true;
                    break;

                case 0xCB:
                    cmdArgU16 = (u16)cmdArgs[0];
                    scriptState->value = *(seqPlayer->seqData + (u32)(cmdArgU16 + scriptState->value));
                    break;

                case 0xCE:
                    cmdArgU16 = (u16)cmdArgs[0];
                    channel->unk_22 = cmdArgU16;
                    break;

                case 0xCF:
                    cmdArgU16 = (u16)cmdArgs[0];
                    seqData = &seqPlayer->seqData[cmdArgU16];
                    seqData[0] = (channel->unk_22 >> 8) & 0xFF;
                    seqData[1] = channel->unk_22 & 0xFF;
                    break;

                case 0xD0:
                    cmd = (u8)cmdArgs[0];
                    if (cmd & 0x80) {
                        channel->stereoHeadsetEffects = true;
                    } else {
                        channel->stereoHeadsetEffects = false;
                    }
                    channel->stereo.asByte = cmd & 0x7F;
                    break;

                case 0xD1:
                    cmd = (u8)cmdArgs[0];
                    channel->noteAllocPolicy = cmd;
                    break;

                case 0xD2:
                    cmd = (u8)cmdArgs[0];
                    channel->adsr.sustain = cmd;
                    break;

                case 0xE5:
                    cmd = (u8)cmdArgs[0];
                    channel->reverbIndex = cmd;
                    break;

                case 0xE4:
                    if (scriptState->value != -1) {
                        data = (*channel->dynTable)[scriptState->value];
                        temp2 = scriptState->depth;
                        scriptState->stack[temp2] = scriptState->pc;
                        scriptState->depth++;
                        cmdArgU16 = (u16)((data[0] << 8) + data[1]);
                        scriptState->pc = seqPlayer->seqData + cmdArgU16;
                    }
                    break;

                case 0xE6:
                    cmd = (u8)cmdArgs[0];
                    channel->bookOffset = cmd;
                    break;

                case 0xE7:
                    cmdArgU16 = (u16)cmdArgs[0];
                    data = &seqPlayer->seqData[cmdArgU16];
                    channel->muteBehavior = *data++;
                    channel->noteAllocPolicy = *data++;
                    AudioSeq_SetChannelPriorities(channel, *data++);
                    channel->transposition = (s8)*data++;
                    channel->newPan = *data++;
                    channel->panChannelWeight = *data++;
                    channel->targetReverbVol = *data++;
                    channel->reverbIndex = *data++;
                    channel->changesPan = true;
                    break;

                case 0xE8:
                    channel->muteBehavior = cmdArgs[0];
                    channel->noteAllocPolicy = cmdArgs[1];
                    cmd = (u8)cmdArgs[2];
                    AudioSeq_SetChannelPriorities(channel, cmd);
                    channel->transposition = (s8)AudioSeq_ScriptReadU8(scriptState);
                    channel->newPan = AudioSeq_ScriptReadU8(scriptState);
                    channel->panChannelWeight = AudioSeq_ScriptReadU8(scriptState);
                    channel->targetReverbVol = AudioSeq_ScriptReadU8(scriptState);
                    channel->reverbIndex = AudioSeq_ScriptReadU8(scriptState);
                    channel->changesPan = true;
                    break;

                case 0xEC:
                    channel->vibratoDepthTarget = 0;
                    channel->vibratoDepthStart = 0;
                    channel->vibratoDepthChangeDelay = 0;
                    channel->vibratoRateTarget = 0;
                    channel->vibratoRateStart = 0;
                    channel->vibratoRateChangeDelay = 0;
                    channel->filter = NULL;
                    channel->gain = 0;
                    channel->adsr.sustain = 0;
                    channel->velocityRandomVariance = 0;
                    channel->gateTimeRandomVariance = 0;
                    channel->combFilterSize = 0;
                    channel->combFilterGain = 0;
                    channel->bookOffset = 0;
                    channel->freqScale = 1.0f;
                    break;

                case 0xE9:
                    AudioSeq_SetChannelPriorities(channel, (u8)cmdArgs[0]);
                    break;

                case 0xED:
                    cmd = (u8)cmdArgs[0];
                    channel->gain = cmd;
                    break;

                case 0xB0:
                    cmdArgU16 = (u16)cmdArgs[0];
                    data = seqPlayer->seqData + cmdArgU16;
                    channel->filter = (s16 *)data;
                    break;

                case 0xB1:
                    channel->filter = NULL;
                    break;

                case 0xB3:
                    cmd = cmdArgs[0];

                    if (channel->filter != NULL) {
                        lowBits = (cmd >> 4) & 0xF;
                        cmd &= 0xF;
                        AudioHeap_LoadFilter(channel->filter, lowBits, cmd);
                    }
                    break;

                case 0xB2:
                    cmdArgU16 = (u16)cmdArgs[0];
                    channel->unk_22 = *(u16 *)(seqPlayer->seqData + (u32)(cmdArgU16 + scriptState->value * 2));
                    break;

                case 0xB4:
                    channel->dynTable = (void *)&seqPlayer->seqData[channel->unk_22];
                    break;

                case 0xB5:
                    channel->unk_22 = ((u16 *)(channel->dynTable))[scriptState->value];
                    break;

                case 0xB6:
                    scriptState->value = (*channel->dynTable)[0][scriptState->value];
                    break;

                case 0xB7:
                    channel->unk_22 =
                        (cmdArgs[0] == 0) ? gAudioCtx.audioRandom & 0xFFFF : gAudioCtx.audioRandom % cmdArgs[0];
                    break;

                case 0xB8:
                    scriptState->value =
                        (cmdArgs[0] == 0) ? gAudioCtx.audioRandom & 0xFFFF : gAudioCtx.audioRandom % cmdArgs[0];
                    break;

                case 0xB9:
                    channel->velocityRandomVariance = cmdArgs[0];
                    break;

                case 0xBA:
                    channel->gateTimeRandomVariance = cmdArgs[0];
                    break;

                case 0xBB:
                    channel->combFilterSize = cmdArgs[0];
                    channel->combFilterGain = cmdArgs[1];
                    break;

                case 0xBC:
                    channel->unk_22 += cmdArgs[0];
                    break;
            }
            continue;
        }

        if (cmd >= 0x70) {
            lowBits = cmd & 0x7;

            if ((cmd & 0xF8) != 0x70 && lowBits >= 8) {
                lowBits = 0;
            }

            switch (cmd & 0xF8) {
                case 0x80:
                    if (channel->layers[lowBits] != NULL) {
                        scriptState->value = channel->layers[lowBits]->finished;
                    } else {
                        scriptState->value = -1;
                    }
                    break;

                case 0x88:
                    cmdArgU16 = AudioSeq_ScriptReadS16(scriptState);
                    if (!AudioSeq_SeqChannelSetLayer(channel, lowBits)) {
                        channel->layers[lowBits]->scriptState.pc = &seqPlayer->seqData[cmdArgU16];
                    }
                    break;

                case 0x90:
                    AudioSeq_SeqLayerFree(channel, lowBits);
                    break;

                case 0x98:
                    if (scriptState->value != -1 && AudioSeq_SeqChannelSetLayer(channel, lowBits) != -1) {
                        data = (*channel->dynTable)[scriptState->value];
                        cmdArgU16 = (data[0] << 8) + data[1];
                        channel->layers[lowBits]->scriptState.pc = &seqPlayer->seqData[cmdArgU16];
                    }
                    break;

                case 0x70:
                    channel->seqScriptIO[lowBits] = scriptState->value;
                    break;

                case 0x78:
                    temp1 = AudioSeq_ScriptReadS16(scriptState);
                    if (!AudioSeq_SeqChannelSetLayer(channel, lowBits)) {
                        channel->layers[lowBits]->scriptState.pc = &scriptState->pc[temp1];
                    }
                    break;
            }
            continue;
        }

        lowBits = cmd & 0xF;

        switch (cmd & 0xF0) {
            case 0x00:
                channel->delay = lowBits;
                goto exit_loop;

            case 0x10:
                channel->seqScriptIO[lowBits] = -1;
                if (AudioLoad_SlowLoadSample(channel->fontId, scriptState->value, &channel->seqScriptIO[lowBits]) ==
                    -1) {}
                break;

            case 0x60:
                scriptState->value = channel->seqScriptIO[lowBits];
                if (lowBits < 2) {
                    channel->seqScriptIO[lowBits] = -1;
                }
                break;

            case 0x50:
                scriptState->value -= channel->seqScriptIO[lowBits];
                break;

            case 0x20:
                cmdArgU16 = AudioSeq_ScriptReadS16(scriptState);
                AudioSeq_SequenceChannelEnable(seqPlayer, lowBits, &seqPlayer->seqData[cmdArgU16]);
                break;

            case 0x30:
                cmd = AudioSeq_ScriptReadU8(scriptState);
                seqPlayer->channels[lowBits]->seqScriptIO[cmd] = scriptState->value;
                break;

            case 0x40:
                cmd = AudioSeq_ScriptReadU8(scriptState);
                scriptState->value = seqPlayer->channels[lowBits]->seqScriptIO[cmd];
                break;
        }
    }
exit_loop:

    for (i = 0; i < 8; i++) {
        if (channel->layers[i] != NULL) {
            AudioSeq_SeqLayerProcessScript(channel->layers[i]);
        }
    }
}
