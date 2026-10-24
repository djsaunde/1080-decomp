#include "common.h"

/* IDO 7.1 -O2 donor for gl_func_0002AD1C (0x380 / 224 words) = OoT's
 * AudioSeq_SeqLayerProcessScriptStep2 (seqplayer.c) verbatim. Spliced into
 * game_libs_post.c.o via REPLACE_FUNC_BODY; the splice renames the donor-local
 * .rodata relocs (the SQ(127.0f) literal at +0, the 15-entry 0xC1..0xCF jumptable
 * at +4) to gl_func_0002AD1C_rodata, pinned to the USO RoData at +0x1058.
 * Layer: flags 0, stereo 1, instOrWave 2, gateTime 3, portamentoTargetNote 4,
 * pan 5, portamentoTime 0xE, transposition 0x10, shortNoteDefaultDelay 0x12,
 * adsr 0x18, portamento 0x20, bend 0x34, velocitySquare 0x3C, instrument 0x48,
 * channel 0x50, scriptState 0x54. Channel transposition 0x26, seqPlayer 0x4C;
 * seqPlayer transposition 0xE, seqData 0x18, short-note tables 0x94 / 0x98. */
typedef union { struct { u8 unused : 2; u8 bit2 : 2; u8 strongRight : 1; u8 strongLeft : 1; u8 stereoHeadsetEffects : 1; u8 usesHeadsetPanEffects : 1; } s; u8 asByte; } gl_2AD1C_Stereo;
typedef struct { u8 decayIndex; u8 sustain; void *envelope; } gl_2AD1C_AdsrSettings;
typedef struct { u8 mode; u16 cur; u16 speed; f32 extent; } gl_2AD1C_Portamento;
typedef struct { u8 *pc; } gl_2AD1C_SeqScriptState;
typedef struct {
    char pad0[0xE]; s16 transposition; char pad10[0x18 - 0x10]; u8 *seqData;
    char pad1c[0x94 - 0x1C]; u8 *shortNoteVelocityTable; u8 *shortNoteGateTimeTable;
} gl_2AD1C_SeqPlayer;
typedef struct {
    char pad0[0x26]; s16 transposition; char pad28[0x4C - 0x28]; gl_2AD1C_SeqPlayer *seqPlayer;
} gl_2AD1C_Channel;
typedef struct {
    u8 enabled : 1; u8 finished : 1; u8 muted : 1; u8 continuousNotes : 1; u8 bit3 : 1; u8 ignoreDrumPan : 1; u8 bit1 : 1; u8 notePropertiesNeedInit : 1;
    gl_2AD1C_Stereo stereo; u8 instOrWave; u8 gateTime; u8 portamentoTargetNote; u8 pan;
    char pad6[0xE - 6]; u16 portamentoTime; s16 transposition; s16 shortNoteDefaultDelay; s16 lastDelay;
    char pad16[2]; gl_2AD1C_AdsrSettings adsr; gl_2AD1C_Portamento portamento; char pad2c[0x34 - 0x2C];
    f32 bend; f32 velocitySquare2; f32 velocitySquare; char pad40[0x48 - 0x40]; void *instrument; char pad4c[4];
    gl_2AD1C_Channel *channel; gl_2AD1C_SeqScriptState scriptState;
} gl_2AD1C_Layer;
extern u8 gl_func_0003F010(gl_2AD1C_SeqScriptState *);
extern s16 gl_func_0003F024(gl_2AD1C_SeqScriptState *);
extern u16 gl_func_0003F05C(gl_2AD1C_SeqScriptState *);
extern u16 gl_func_0003E680(gl_2AD1C_SeqScriptState *, u8);
extern s32 gl_func_0003E6E8(gl_2AD1C_SeqPlayer *, gl_2AD1C_SeqScriptState *, s32, s32);
extern u8 gl_func_000400A4(gl_2AD1C_Channel *, u8, void **, gl_2AD1C_AdsrSettings *);
extern void gl_2AD1C_SeqLayerDisable(gl_2AD1C_Layer *);
extern void gl_2AD1C_SeqLayerNoteDecay(gl_2AD1C_Layer *);
extern f32 gl_2AD1C_gBendPitchTwoSemitonesFrequencies[];
#define AudioSeq_ScriptReadU8 gl_func_0003F010
#define AudioSeq_ScriptReadS16 gl_func_0003F024
#define AudioSeq_ScriptReadCompressedU16 gl_func_0003F05C
#define AudioSeq_GetScriptControlFlowArgument gl_func_0003E680
#define AudioSeq_HandleScriptFlowControl gl_func_0003E6E8
#define AudioSeq_GetInstrument gl_func_000400A4
#define AudioSeq_SeqLayerDisable gl_2AD1C_SeqLayerDisable
#define Audio_SeqLayerNoteDecay gl_2AD1C_SeqLayerNoteDecay
#define gBendPitchTwoSemitonesFrequencies gl_2AD1C_gBendPitchTwoSemitonesFrequencies
#define SQ(x) ((x) * (x))

s32 gl_func_0002AD1C(gl_2AD1C_Layer *layer) {
    gl_2AD1C_Channel *channel = layer->channel;
    gl_2AD1C_SeqScriptState *state = &layer->scriptState;
    gl_2AD1C_SeqPlayer *seqPlayer = channel->seqPlayer;
    u8 cmd;
    u8 cmdArg8;
    u16 cmdArg16;
    u16 velocity;

    while (1) {
        cmd = AudioSeq_ScriptReadU8(state);

        if (cmd <= 0xC0) {
            return cmd;
        }

        if (cmd >= 0xF2) {
            cmdArg16 = AudioSeq_GetScriptControlFlowArgument(state, cmd);

            if (AudioSeq_HandleScriptFlowControl(seqPlayer, state, cmd, cmdArg16) == 0) {
                continue;
            }
            AudioSeq_SeqLayerDisable(layer);
            return -1;
        }

        switch (cmd) {
            case 0xC1:
            case 0xCA:
                cmdArg8 = *(state->pc++);
                if (cmd == 0xC1) {
                    layer->velocitySquare = SQ(cmdArg8) / SQ(127.0f);
                } else {
                    layer->pan = cmdArg8;
                }
                break;

            case 0xC9:
            case 0xC2:
                cmdArg8 = *(state->pc++);
                if (cmd == 0xC9) {
                    layer->gateTime = cmdArg8;
                } else {
                    layer->transposition = cmdArg8;
                }
                break;

            case 0xC4:
            case 0xC5:
                if (cmd == 0xC4) {
                    layer->continuousNotes = 1;
                } else {
                    layer->continuousNotes = 0;
                }
                layer->bit1 = 0;
                Audio_SeqLayerNoteDecay(layer);
                break;

            case 0xC3:
                cmdArg16 = AudioSeq_ScriptReadCompressedU16(state);
                layer->shortNoteDefaultDelay = cmdArg16;
                break;

            case 0xC6:
                cmd = AudioSeq_ScriptReadU8(state);
                if (cmd >= 0x7E) {
                    if (cmd == 0x7E) {
                        layer->instOrWave = 1;
                    } else if (cmd == 0x7F) {
                        layer->instOrWave = 0;
                    } else {
                        layer->instOrWave = cmd;
                        layer->instrument = NULL;
                    }

                    if (cmd == 0xFF) {
                        layer->adsr.decayIndex = 0;
                    }
                } else {
                    if ((layer->instOrWave = AudioSeq_GetInstrument(channel, cmd, &layer->instrument, &layer->adsr)) == 0) {
                        layer->instOrWave = 0xFF;
                    }
                }
                break;

            case 0xC7:
                layer->portamento.mode = AudioSeq_ScriptReadU8(state);

                cmd = AudioSeq_ScriptReadU8(state);
                cmd += channel->transposition;
                cmd += layer->transposition;
                cmd += seqPlayer->transposition;

                if (cmd >= 0x80) {
                    cmd = 0;
                }

                layer->portamentoTargetNote = cmd;

                if (layer->portamento.mode & 0x80) {
                    layer->portamentoTime = *(state->pc++);
                    break;
                }

                cmdArg16 = AudioSeq_ScriptReadCompressedU16(state);
                layer->portamentoTime = cmdArg16;
                break;

            case 0xC8:
                layer->portamento.mode = 0;
                break;

            case 0xCB:
                cmdArg16 = AudioSeq_ScriptReadS16(state);
                layer->adsr.envelope = (void *)(seqPlayer->seqData + cmdArg16);
            case 0xCF:
                layer->adsr.decayIndex = AudioSeq_ScriptReadU8(state);
                break;

            case 0xCC:
                layer->ignoreDrumPan = 1;
                break;

            case 0xCD:
                layer->stereo.asByte = AudioSeq_ScriptReadU8(state);
                break;

            case 0xCE:
                cmdArg8 = AudioSeq_ScriptReadU8(state);
                layer->bend = gBendPitchTwoSemitonesFrequencies[(u8)(cmdArg8 + 0x80)];
                break;

            default:
                switch (cmd & 0xF0) {
                    case 0xD0:
                        velocity = seqPlayer->shortNoteVelocityTable[cmd & 0xF];
                        layer->velocitySquare = SQ(velocity) / SQ(127.0f);
                        break;

                    case 0xE0:
                        layer->gateTime = seqPlayer->shortNoteGateTimeTable[cmd & 0xF];
                        break;
                }
        }
    }
}
