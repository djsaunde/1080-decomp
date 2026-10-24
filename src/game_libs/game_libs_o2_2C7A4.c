#include "common.h"

/* IDO 7.1 -O2 donor for gl_func_0002C7A4 (0x7CC / 499 words) = OoT's
 * AudioSeq_SequencePlayerProcessSequence (seqplayer.c) in its older form: the
 * load-incomplete path calls two 1-arg blanks on seqId / defaultFont before
 * SequencePlayerDisable, no RUNSEQ (0xC4) / EF opcodes, explicit empty 0x10 / 0x20 /
 * 0x60 low-nibble cases (the empty 0x60 = stubbed LDRES keeps the 0x40..0xB0 range
 * dense enough for one table), and a single tempo bound
 * (gAudioCtx +0x2074) for both the tempo clamp and the tempo accumulator.
 * Spliced into game_libs_post.c.o via REPLACE_FUNC_BODY; the splice renames the
 * donor-local .rodata jumptable relocs (0xC5..0xF1 table, then the 0x40..0xB0
 * low-nibble table) to gl_func_0002C7A4_rodata, pinned to the USO table at
 * +0x13E0. */
typedef struct { u8 *pc; u8 *stack[4]; u8 remLoopIters[4]; u8 depth; s8 value; } gl_2C7A4_SeqScriptState;
typedef struct { u32 enabled : 1; u32 rest : 31; } gl_2C7A4_Channel;
typedef struct {
    u32 enabled : 1; u32 finished : 1; u32 muted : 1; u32 seqDmaInProgress : 1;
    u32 fontDmaInProgress : 1; u32 recalculateVolume : 1; u32 stopScript : 1; u32 applyBend : 1;
    u8 state; u8 noteAllocPolicy; u8 muteBehavior; u8 seqId; u8 defaultFont; u8 unk_06; s8 playerIdx;
    u16 tempo; u16 tempoAcc; s16 unk_0C; s16 transposition; u16 delay; u16 fadeTimer; u16 fadeTimerUnkEu; char pad16[2];
    u8 *seqData; f32 fadeVolume; f32 fadeVelocity; f32 volume; f32 muteVolumeScale; f32 fadeVolumeScale; f32 appliedFadeVolume;
    f32 unk_34; gl_2C7A4_Channel *channels[16]; gl_2C7A4_SeqScriptState scriptState;
    u8 *shortNoteVelocityTable; u8 *shortNoteGateTimeTable; char notePool[0x44]; s32 scriptCounter;
    char padE4[0x158 - 0xE4]; s8 seqScriptIO[8];
} gl_2C7A4_SeqPlayer;
typedef struct { char pad[0x2074]; s16 tempoInternalToExternal; char pad2076[0x2154 - 0x2076]; u32 audioRandom; } gl_2C7A4_Ctx;
extern int D_00000000;
#define gAudioCtx (*(gl_2C7A4_Ctx *)&D_00000000)
extern int gl_func_00000000();
extern u8 gl_func_0003F010(gl_2C7A4_SeqScriptState *);
extern s16 gl_func_0003F024(gl_2C7A4_SeqScriptState *);
extern u16 gl_func_0003E680(gl_2C7A4_SeqScriptState *, u8);
extern s32 gl_func_0003E6E8(gl_2C7A4_SeqPlayer *, gl_2C7A4_SeqScriptState *, s32, s32);
extern void gl_func_0003EC34(gl_2C7A4_SeqPlayer *, u16);
extern void gl_func_0003EDAC(gl_2C7A4_SeqPlayer *, u8, void *);
extern void gl_func_000401E8(gl_2C7A4_Channel *);
#define AudioSeq_ScriptReadU8 gl_func_0003F010
#define AudioSeq_ScriptReadS16 gl_func_0003F024
#define AudioSeq_GetScriptControlFlowArgument gl_func_0003E680
#define AudioSeq_HandleScriptFlowControl gl_func_0003E6E8
#define AudioSeq_SequencePlayerSetupChannels gl_func_0003EC34
#define AudioSeq_SequenceChannelEnable gl_func_0003EDAC
#define AudioSeq_SequenceChannelProcessScript gl_func_000401E8

void gl_func_0002C7A4(gl_2C7A4_SeqPlayer *seqPlayer) {
    u8 cmd;
    u8 cmdLowBits;
    gl_2C7A4_SeqScriptState *seqScript = &seqPlayer->scriptState;
    s16 tempS;
    u16 temp;
    s32 i;
    s32 value;
    u8 *data;
    u8 *data2;
    u8 *data3;
    s32 pad3;
    s32 dummy;

    if (!seqPlayer->enabled) {
        return;
    }

    if (!gl_func_00000000(seqPlayer->seqId) || !gl_func_00000000(seqPlayer->defaultFont)) {
        gl_func_00000000(seqPlayer->seqId);
        gl_func_00000000(seqPlayer->defaultFont);
        gl_func_00000000(seqPlayer);
        return;
    }

    gl_func_00000000(seqPlayer->seqId, 2);
    gl_func_00000000(seqPlayer->defaultFont, 2);

    if (seqPlayer->muted && (seqPlayer->muteBehavior & 0x80)) {
        return;
    }

    seqPlayer->scriptCounter++;

    seqPlayer->tempoAcc += seqPlayer->tempo;
    seqPlayer->tempoAcc += (s16)seqPlayer->unk_0C;

    if (seqPlayer->tempoAcc < gAudioCtx.tempoInternalToExternal) {
        return;
    }

    seqPlayer->tempoAcc -= (u16)gAudioCtx.tempoInternalToExternal;

    if (seqPlayer->stopScript == 1) {
        return;
    }

    if (seqPlayer->delay > 1) {
        seqPlayer->delay--;
    } else {
        seqPlayer->recalculateVolume = 1;

        while (1) {
            cmd = AudioSeq_ScriptReadU8(seqScript);

            if (cmd >= 0xF2) {
                s32 delay = AudioSeq_HandleScriptFlowControl(
                    seqPlayer, seqScript, cmd, AudioSeq_GetScriptControlFlowArgument(&seqPlayer->scriptState, cmd));

                if (delay != 0) {
                    if (delay == -1) {
                        gl_func_00000000(seqPlayer);
                    } else {
                        seqPlayer->delay = delay;
                    }
                    break;
                }
                continue;
            }

            if (cmd >= 0xC0) {
                switch (cmd) {
                    case 0xF1:
                        gl_func_00000000(&seqPlayer->notePool);
                        cmd = AudioSeq_ScriptReadU8(seqScript);
                        gl_func_00000000(&seqPlayer->notePool, cmd);
                        dummy = -1;
                        if (dummy < 0) {
                            dummy = 0;
                        }
                        if (dummy > 1) {
                            dummy = 1;
                        }
                        if (dummy) {}
                        break;

                    case 0xF0:
                        gl_func_00000000(&seqPlayer->notePool);
                        break;

                    case 0xDF:
                        seqPlayer->transposition = 0;
                    case 0xDE:
                        seqPlayer->transposition += (s8)AudioSeq_ScriptReadU8(seqScript);
                        break;

                    case 0xDD:
                        seqPlayer->tempo = AudioSeq_ScriptReadU8(seqScript) * 48;
                        if (seqPlayer->tempo > gAudioCtx.tempoInternalToExternal) {
                            seqPlayer->tempo = (u16)gAudioCtx.tempoInternalToExternal;
                        }

                        if ((s16)seqPlayer->tempo <= 0) {
                            seqPlayer->tempo = 1;
                        }
                        break;

                    case 0xDC:
                        seqPlayer->unk_0C = (s8)AudioSeq_ScriptReadU8(seqScript) * 48;
                        break;

                    case 0xDA:
                        cmd = AudioSeq_ScriptReadU8(seqScript);
                        temp = AudioSeq_ScriptReadS16(seqScript);
                        switch (cmd) {
                            case 0:
                            case 1:
                                if (seqPlayer->state != 2) {
                                    seqPlayer->fadeTimerUnkEu = temp;
                                    seqPlayer->state = cmd;
                                }
                                break;

                            case 2:
                                seqPlayer->fadeTimer = temp;
                                seqPlayer->state = cmd;
                                seqPlayer->fadeVelocity = (0.0f - seqPlayer->fadeVolume) / (s32)seqPlayer->fadeTimer;
                                break;
                        }
                        break;

                    case 0xDB:
                        value = AudioSeq_ScriptReadU8(seqScript);
                        switch (seqPlayer->state) {
                            case 1:
                                seqPlayer->state = 0;
                                seqPlayer->fadeVolume = 0.0f;
                            case 0:
                                seqPlayer->fadeTimer = seqPlayer->fadeTimerUnkEu;
                                if (seqPlayer->fadeTimerUnkEu != 0) {
                                    seqPlayer->fadeVelocity =
                                        ((value / 127.0f) - seqPlayer->fadeVolume) / (s32)seqPlayer->fadeTimer;
                                } else {
                                    seqPlayer->fadeVolume = value / 127.0f;
                                }
                                break;

                            case 2:
                                break;
                        }
                        break;

                    case 0xD9:
                        seqPlayer->fadeVolumeScale = (s8)AudioSeq_ScriptReadU8(seqScript) / 127.0f;
                        break;

                    case 0xD7:
                        temp = AudioSeq_ScriptReadS16(seqScript);
                        AudioSeq_SequencePlayerSetupChannels(seqPlayer, temp);
                        break;

                    case 0xD6:
                        AudioSeq_ScriptReadS16(seqScript);
                        break;

                    case 0xD5:
                        seqPlayer->muteVolumeScale = (s8)AudioSeq_ScriptReadU8(seqScript) / 127.0f;
                        break;

                    case 0xD4:
                        seqPlayer->muted = 1;
                        break;

                    case 0xD3:
                        seqPlayer->muteBehavior = AudioSeq_ScriptReadU8(seqScript);
                        break;

                    case 0xD1:
                    case 0xD2:
                        temp = AudioSeq_ScriptReadS16(seqScript);
                        data3 = &seqPlayer->seqData[temp];
                        if (cmd == 0xD2) {
                            seqPlayer->shortNoteVelocityTable = data3;
                        } else {
                            seqPlayer->shortNoteGateTimeTable = data3;
                        }
                        break;

                    case 0xD0:
                        seqPlayer->noteAllocPolicy = AudioSeq_ScriptReadU8(seqScript);
                        break;

                    case 0xCE:
                        cmd = AudioSeq_ScriptReadU8(seqScript);
                        if (cmd == 0) {
                            seqScript->value = (gAudioCtx.audioRandom >> 2) & 0xFF;
                        } else {
                            seqScript->value = (gAudioCtx.audioRandom >> 2) % cmd;
                        }
                        break;

                    case 0xCD:
                        temp = AudioSeq_ScriptReadS16(seqScript);
                        if ((seqScript->value != -1) && (seqScript->depth != 3)) {
                            data = seqPlayer->seqData + (u32)(temp + (seqScript->value << 1));
                            seqScript->stack[seqScript->depth] = seqScript->pc;
                            seqScript->depth++;

                            temp = (data[0] << 8) + data[1];
                            seqScript->pc = &seqPlayer->seqData[temp];
                        }
                        break;

                    case 0xCC:
                        seqScript->value = AudioSeq_ScriptReadU8(seqScript);
                        break;

                    case 0xC9:
                        seqScript->value &= AudioSeq_ScriptReadU8(seqScript);
                        break;

                    case 0xC8:
                        seqScript->value -= AudioSeq_ScriptReadU8(seqScript);
                        break;

                    case 0xC7:
                        cmd = AudioSeq_ScriptReadU8(seqScript);
                        temp = AudioSeq_ScriptReadS16(seqScript);
                        data2 = &seqPlayer->seqData[temp];
                        *data2 = (u8)seqScript->value + cmd;
                        break;

                    case 0xC6:
                        seqPlayer->stopScript = 1;
                        return;

                    case 0xC5:
                        seqPlayer->scriptCounter = (u16)AudioSeq_ScriptReadS16(seqScript);
                        break;
                }
                continue;
            }

            cmdLowBits = cmd & 0x0F;

            switch (cmd & 0xF0) {
                case 0x00:
                    seqScript->value = seqPlayer->channels[cmdLowBits]->enabled ^ 1;
                    break;

                case 0x10:
                    break;

                case 0x20:
                    break;

                case 0x60:
                    break;

                case 0x50:
                    seqScript->value -= seqPlayer->seqScriptIO[cmdLowBits];
                    break;

                case 0x70:
                    seqPlayer->seqScriptIO[cmdLowBits] = seqScript->value;
                    break;

                case 0x80:
                    seqScript->value = seqPlayer->seqScriptIO[cmdLowBits];
                    if (cmdLowBits < 2) {
                        seqPlayer->seqScriptIO[cmdLowBits] = -1;
                    }
                    break;

                case 0x40:
                    gl_func_00000000(seqPlayer->channels[cmdLowBits]);
                    break;

                case 0x90:
                    temp = AudioSeq_ScriptReadS16(seqScript);
                    AudioSeq_SequenceChannelEnable(seqPlayer, cmdLowBits, (void *)&seqPlayer->seqData[temp]);
                    break;

                case 0xA0:
                    tempS = AudioSeq_ScriptReadS16(seqScript);
                    AudioSeq_SequenceChannelEnable(seqPlayer, cmdLowBits, (void *)&seqScript->pc[tempS]);
                    break;

                case 0xB0:
                    cmd = AudioSeq_ScriptReadU8(seqScript);
                    temp = AudioSeq_ScriptReadS16(seqScript);
                    data2 = &seqPlayer->seqData[temp];
                    gl_func_00000000(cmd, data2, &seqPlayer->seqScriptIO[cmdLowBits]);
                    break;
            }
        }
    }

    for (i = 0; i < 16; i++) {
        if (seqPlayer->channels[i]->enabled) {
            AudioSeq_SequenceChannelProcessScript(seqPlayer->channels[i]);
        }
    }
}
