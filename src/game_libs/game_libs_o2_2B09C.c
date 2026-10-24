#include "common.h"

/* IDO 7.1 -O2 donor for gl_func_0002B09C (0x558 / 342 words) = OoT's
 * AudioSeq_SeqLayerProcessScriptStep4 (seqplayer.c) in its older (SM64-era)
 * form: no layer->semitone field (layer +4 is portamentoTargetNote), no
 * `layer->delay != 0` guard on the special-portamento speed, and the
 * zero-delay tail has no NULL-sample / 0x7FFE clamp / speed2 block. Spliced into
 * game_libs_post.c.o via REPLACE_FUNC_BODY; the splice renames the donor-local
 * .rodata jumptable reloc (portamento mode switch) to gl_func_0002B09C_rodata,
 * pinned to the USO table at +0x1098. Load-bearing: channel / seqPlayer loaded before
 * instOrWave; ticksPerUpdate read through its own audio-buffer-parameters symbol
 * (gl_2B09C_bufParams, SM64's separate gAudioBufferParameters global): through
 * gAudioCtx the portamentoTime * ticksPerUpdate operands load in the other order. */
typedef struct { void *sample; f32 tuning; } gl_2B09C_TunedSample;
typedef struct { u8 adsrDecayIndex; u8 pan; u8 isRelocated; gl_2B09C_TunedSample tunedSample; void *envelope; } gl_2B09C_Drum;
typedef struct { gl_2B09C_TunedSample tunedSample; } gl_2B09C_SoundEffect;
typedef struct { u8 mode; u16 cur; u16 speed; f32 extent; } gl_2B09C_Portamento;
typedef struct { u8 decayIndex; u8 sustain; void *envelope; } gl_2B09C_AdsrSettings;
typedef struct { char pad[8]; u16 tempo; char padA[4]; s16 transposition; } gl_2B09C_SeqPlayer;
typedef struct {
    u8 enabled : 1; u8 finished : 1; u8 stopScript : 1; u8 muted : 1; u8 hasInstrument : 1; u8 b5 : 3;
    char pad1[6]; u8 fontId; char pad8[0x24 - 8]; s16 instOrWave; s16 transposition;
    char pad28[0x48 - 0x28]; void *instrument; gl_2B09C_SeqPlayer *seqPlayer;
} gl_2B09C_Channel;
typedef struct {
    u8 enabled : 1; u8 finished : 1; u8 muted : 1; u8 continuousNotes : 1; u8 bit3 : 1; u8 ignoreDrumPan : 1; u8 bit1 : 1; u8 notePropertiesNeedInit : 1;
    u8 stereo; u8 instOrWave; u8 gateTime; u8 portamentoTargetNote; u8 pan; char pad6[2];
    s16 delay; s16 gateDelay; s16 delay2; u16 portamentoTime; s16 transposition; s16 shortNoteDefaultDelay; s16 lastDelay; char pad16[2];
    gl_2B09C_AdsrSettings adsr; gl_2B09C_Portamento portamento; char pad2c[4]; f32 freqScale; f32 bend;
    char pad38[0x48 - 0x38]; void *instrument; gl_2B09C_TunedSample *tunedSample; gl_2B09C_Channel *channel;
} gl_2B09C_Layer;
typedef struct { char pad[0x128]; gl_2B09C_TunedSample tunedSample; char pad130[0x158 - 0x130]; } gl_2B09C_SynthesisReverb;
typedef struct {
    gl_2B09C_SynthesisReverb synthesisReverbs[4]; char pad560[0x2040 - 0x560]; s16 ticksPerUpdate;
    char pad2042[0x205C - 0x2042]; f32 unk_2870; char pad2060[0x2074 - 0x2060]; s16 maxTempo;
} gl_2B09C_Ctx;
extern int D_00000000;
#define gAudioCtx (*(gl_2B09C_Ctx *)&D_00000000)
extern gl_2B09C_Ctx gl_2B09C_bufParams;
extern f32 gl_2B09C_gPitchFrequencies[];
#define gPitchFrequencies gl_2B09C_gPitchFrequencies
extern void *gl_func_00000000();
#define Audio_GetDrum (gl_2B09C_Drum *)gl_func_00000000
#define Audio_GetSoundEffect (gl_2B09C_SoundEffect *)gl_func_00000000
#define Audio_GetInstrumentTunedSample (gl_2B09C_TunedSample *)gl_func_00000000
#define PORTAMENTO_MODE(p) ((p).mode & ~0x80)
#define PORTAMENTO_IS_SPECIAL(p) ((p).mode & 0x80)

s32 gl_func_0002B09C(gl_2B09C_Layer *layer, s32 cmd) {
    s32 sameTunedSample = 1;
    s32 instOrWave;
    s32 speed;
    f32 temp_f14;
    f32 temp_f2;
    gl_2B09C_Portamento *portamento;
    f32 freqScale;
    f32 freqScale2;
    gl_2B09C_TunedSample *tunedSample;
    void *instrument;
    gl_2B09C_Drum *drum;
    gl_2B09C_SoundEffect *soundEffect;
    gl_2B09C_Channel *channel;
    gl_2B09C_SeqPlayer *seqPlayer;
    u8 semitone = cmd;
    u16 sfxId;
    s32 semitone2;
    s32 vel;
    f32 time;
    f32 tuning;

    channel = layer->channel;
    seqPlayer = channel->seqPlayer;
    instOrWave = layer->instOrWave;

    if (instOrWave == 0xFF) {
        if (!channel->hasInstrument) {
            return -1;
        }
        instOrWave = channel->instOrWave;
    }

    switch (instOrWave) {
        case 0:
            semitone += channel->transposition + layer->transposition;

            drum = Audio_GetDrum(channel->fontId, semitone);
            if (drum == NULL) {
                layer->muted = 1;
                layer->delay2 = layer->delay;
                return -1;
            }

            tunedSample = &drum->tunedSample;
            layer->adsr.envelope = drum->envelope;
            layer->adsr.decayIndex = drum->adsrDecayIndex;
            if (!layer->ignoreDrumPan) {
                layer->pan = drum->pan;
            }
            layer->tunedSample = tunedSample;
            layer->freqScale = tunedSample->tuning;
            break;

        case 1:
            sfxId = (layer->transposition << 6) + semitone;

            soundEffect = Audio_GetSoundEffect(channel->fontId, sfxId);
            if (soundEffect == NULL) {
                layer->muted = 1;
                layer->delay2 = layer->delay + 1;
                return -1;
            }

            tunedSample = &soundEffect->tunedSample;
            layer->tunedSample = tunedSample;
            layer->freqScale = tunedSample->tuning;
            break;

        default:
            semitone += seqPlayer->transposition + channel->transposition + layer->transposition;
            semitone2 = semitone;

            if (semitone >= 0x80) {
                layer->muted = 1;
                return -1;
            }

            if (layer->instOrWave == 0xFF) {
                instrument = channel->instrument;
            } else {
                instrument = layer->instrument;
            }

            if (layer->portamento.mode != 0) {
                portamento = &layer->portamento;
                vel = (semitone > layer->portamentoTargetNote) ? semitone : layer->portamentoTargetNote;

                if (instrument != NULL) {
                    tunedSample = Audio_GetInstrumentTunedSample(instrument, vel);
                    sameTunedSample = (layer->tunedSample == tunedSample);
                    layer->tunedSample = tunedSample;
                    tuning = tunedSample->tuning;
                } else {
                    layer->tunedSample = NULL;
                    tuning = 1.0f;
                    if (instOrWave >= 0xC0) {
                        layer->tunedSample = &gAudioCtx.synthesisReverbs[instOrWave - 0xC0].tunedSample;
                    }
                }

                temp_f2 = gPitchFrequencies[semitone2] * tuning;
                temp_f14 = gPitchFrequencies[layer->portamentoTargetNote] * tuning;

                switch (PORTAMENTO_MODE(*portamento)) {
                    case 1:
                    case 3:
                    case 5:
                        freqScale2 = temp_f2;
                        freqScale = temp_f14;
                        break;

                    case 2:
                    case 4:
                        freqScale = temp_f2;
                        freqScale2 = temp_f14;
                        break;

                    default:
                        freqScale = temp_f2;
                        freqScale2 = temp_f2;
                        break;
                }

                portamento->extent = (freqScale2 / freqScale) - 1.0f;

                if (PORTAMENTO_IS_SPECIAL(*portamento)) {
                    speed = seqPlayer->tempo * 0x8000 / gAudioCtx.maxTempo;
                    speed = speed * 0x100 / (layer->delay * layer->portamentoTime);
                } else {
                    speed = 0x20000 / (layer->portamentoTime * gl_2B09C_bufParams.ticksPerUpdate);
                }

                if (speed >= 0x7FFF) {
                    speed = 0x7FFF;
                } else if (speed < 1) {
                    speed = 1;
                }

                portamento->speed = speed;
                portamento->cur = 0;
                layer->freqScale = freqScale;
                if (PORTAMENTO_MODE(*portamento) == 5) {
                    layer->portamentoTargetNote = semitone;
                }
                break;
            }

            if (instrument != NULL) {
                tunedSample = Audio_GetInstrumentTunedSample(instrument, semitone);
                sameTunedSample = (tunedSample == layer->tunedSample);
                layer->tunedSample = tunedSample;
                layer->freqScale = gPitchFrequencies[semitone2] * tunedSample->tuning;
            } else {
                layer->tunedSample = NULL;
                layer->freqScale = gPitchFrequencies[semitone2];
                if (instOrWave >= 0xC0) {
                    layer->tunedSample = &gAudioCtx.synthesisReverbs[instOrWave - 0xC0].tunedSample;
                }
            }
            break;
    }

    layer->delay2 = layer->delay;
    layer->freqScale *= layer->bend;

    if (layer->delay == 0) {
        time = (*(u32 **)((u8 *)layer->tunedSample->sample + 8))[1];
        time *= seqPlayer->tempo;
        time *= gAudioCtx.unk_2870;
        time /= layer->freqScale;
        layer->gateDelay = 0;
        layer->delay = (u16)(s32)time + 1;
    }
    return sameTunedSample;
}
