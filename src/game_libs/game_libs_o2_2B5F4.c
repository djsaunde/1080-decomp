#include "common.h"

/* IDO 7.1 -O2 donor for gl_func_0002B5F4 (0x444 / 273 words) = OoT's
 * AudioSeq_SeqLayerProcessScriptStep3 (seqplayer.c) verbatim. Spliced into
 * game_libs_post.c.o via REPLACE_FUNC_BODY; the splice renames the donor-local
 * .rodata reloc (the SQ(127.0f) = 16129.0f literal) to gl_func_0002B5F4_rodata,
 * pinned to the USO RoData word at +0x10AC. Layer: flags byte 0 (muted 0x20,
 * bit1 0x02), gateTime +3, delay +8, gateDelay +0xA, shortNoteDefaultDelay +0x12,
 * lastDelay +0x14, velocitySquare2 +0x38, velocitySquare +0x3C, channel +0x50,
 * scriptState +0x54. Channel: flag word 0 (muted bit 3, largeNotes bit 5),
 * muteBehavior +3, velocity / gate random variance +0xD / +0xE, seqPlayer +0x4C.
 * SeqPlayer: muted bit 2, skipTicks +0xDC. audioRandom (+0x2154) is read through its
 * own symbol (gl_2B5F4_audioRandom, SM64's separate gAudioRandom global): through the
 * shared gAudioCtx base both random-variance products load their operands in the
 * other order (mul.s / multu operand swap). */
typedef struct {
    u32 enabled : 1; u32 finished : 1; u32 muted : 1; u32 seqDmaInProgress : 1;
    u32 fontDmaInProgress : 1; u32 recalculateVolume : 1; u32 stopScript : 1; u32 applyBend : 1; u32 rest : 24;
    char pad4[0xDC - 4]; s32 skipTicks;
} gl_2B5F4_SeqPlayer;
typedef struct {
    u8 enabled : 1; u8 finished : 1; u8 stopScript : 1; u8 muted : 1; u8 hasInstrument : 1;
    u8 stereoHeadsetEffects : 1; u8 largeNotes : 1; u8 unused : 1; u8 changes; u8 noteAllocPolicy; u8 muteBehavior;
    char pad4[0xD - 4]; u8 velocityRandomVariance; u8 gateTimeRandomVariance;
    char padF[0x4C - 0xF]; gl_2B5F4_SeqPlayer *seqPlayer;
} gl_2B5F4_Channel;
typedef struct { u8 *pc; } gl_2B5F4_SeqScriptState;
typedef struct {
    u8 enabled : 1; u8 finished : 1; u8 muted : 1; u8 continuousNotes : 1; u8 bit3 : 1; u8 ignoreDrumPan : 1; u8 bit1 : 1; u8 notePropertiesNeedInit : 1;
    char pad1[2]; u8 gateTime; char pad4[4]; s16 delay; s16 gateDelay; char padC[0x12 - 0xC];
    s16 shortNoteDefaultDelay; s16 lastDelay; char pad16[0x38 - 0x16]; f32 velocitySquare2; f32 velocitySquare;
    char pad40[0x50 - 0x40]; gl_2B5F4_Channel *channel; gl_2B5F4_SeqScriptState scriptState;
} gl_2B5F4_Layer;
typedef struct { char pad[0x2154]; u32 audioRandom; } gl_2B5F4_Ctx;
extern int D_00000000;
extern gl_2B5F4_Ctx gl_2B5F4_audioRandom;
#define gAudioCtx gl_2B5F4_audioRandom
extern int gl_ref_0003F05C();
#define AudioSeq_ScriptReadCompressedU16 gl_ref_0003F05C
#define SQ(x) ((x) * (x))

s32 gl_func_0002B5F4(gl_2B5F4_Layer *layer, s32 cmd) {
    gl_2B5F4_SeqScriptState *state = &layer->scriptState;
    u16 delay;
    s32 velocity;
    gl_2B5F4_Channel *channel = layer->channel;
    gl_2B5F4_SeqPlayer *seqPlayer = channel->seqPlayer;
    s32 intDelta;
    f32 floatDelta;

    if (cmd == 0xC0) {
        layer->delay = AudioSeq_ScriptReadCompressedU16(state);
        layer->muted = 1;
        layer->bit1 = 0;
        return -1;
    }

    layer->muted = 0;

    if (channel->largeNotes == 1) {
        switch (cmd & 0xC0) {
            case 0x00:
                delay = AudioSeq_ScriptReadCompressedU16(state);
                velocity = *(state->pc++);
                layer->gateTime = *(state->pc++);
                layer->lastDelay = delay;
                break;

            case 0x40:
                delay = AudioSeq_ScriptReadCompressedU16(state);
                velocity = *(state->pc++);
                layer->gateTime = 0;
                layer->lastDelay = delay;
                break;

            case 0x80:
                delay = layer->lastDelay;
                velocity = *(state->pc++);
                layer->gateTime = *(state->pc++);
                break;
        }

        if (velocity > 0x7F || velocity < 0) {
            velocity = 0x7F;
        }
        layer->velocitySquare = SQ((f32)velocity) / SQ(127.0f);
        cmd -= (cmd & 0xC0);
    } else {
        switch (cmd & 0xC0) {
            case 0x00:
                delay = AudioSeq_ScriptReadCompressedU16(state);
                layer->lastDelay = delay;
                break;

            case 0x40:
                delay = layer->shortNoteDefaultDelay;
                break;

            case 0x80:
                delay = layer->lastDelay;
                break;
        }
        cmd -= (cmd & 0xC0);
    }

    if (channel->velocityRandomVariance != 0) {
        floatDelta = layer->velocitySquare * (gAudioCtx.audioRandom % channel->velocityRandomVariance) / 100.0f;
        if ((gAudioCtx.audioRandom & 0x8000) != 0) {
            floatDelta = -floatDelta;
        }

        layer->velocitySquare2 = layer->velocitySquare + floatDelta;

        if (layer->velocitySquare2 < 0.0f) {
            layer->velocitySquare2 = 0.0f;
        } else if (layer->velocitySquare2 > 1.0f) {
            layer->velocitySquare2 = 1.0f;
        }
    } else {
        layer->velocitySquare2 = layer->velocitySquare;
    }

    layer->delay = delay;
    layer->gateDelay = (layer->gateTime * delay) >> 8;

    if (channel->gateTimeRandomVariance != 0) {
        intDelta = (layer->gateDelay * (gAudioCtx.audioRandom % channel->velocityRandomVariance)) / 100;
        if ((gAudioCtx.audioRandom & 0x4000) != 0) {
            intDelta = -intDelta;
        }

        layer->gateDelay += intDelta;
        if (layer->gateDelay < 0) {
            layer->gateDelay = 0;
        } else if (layer->gateDelay > layer->delay) {
            layer->gateDelay = layer->delay;
        }
    }

    if ((seqPlayer->muted && (channel->muteBehavior & (0x40 | 0x10))) || channel->muted) {
        layer->muted = 1;
        return -1;
    }

    if (seqPlayer->skipTicks != 0) {
        layer->muted = 1;
        return -1;
    }

    return cmd;
}
