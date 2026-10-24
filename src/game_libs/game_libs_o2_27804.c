#include "common.h"

/* IDO 7.1 -O2 donor for gl_func_00027804 (0x444 / 273 words) = OoT's
 * Audio_InitSampleState (playback.c; Audio_InitNoteSub in older names) in its
 * older form: the target sample state IS note->sampleState (+0xB0), so there is
 * no bitField / waveSampleAddr copy and no separate parameter, and the resampling
 * rate is set (on the note) before the attribute reads. Spliced into
 * game_libs_post.c.o via REPLACE_FUNC_BODY; the splice renames the donor-local
 * .rodata relocs (0.707f, 0x1000 - 0.001f) to gl_func_00027804_rodata, pinned to
 * the USO RoData at +0xFC0. The extra s32 pad2 after stereoData is the 8-byte
 * larger frame of the target. */
typedef struct {
    u8 unused : 2; u8 bit2 : 2; u8 strongRight : 1; u8 strongLeft : 1; u8 stereoHeadsetEffects : 1; u8 usesHeadsetPanEffects : 1;
} gl_27804_StereoData;
typedef union { gl_27804_StereoData s; u8 asByte; } gl_27804_Stereo;
typedef struct {
    u8 reverbVol; u8 gain; u8 pan; gl_27804_Stereo stereo; f32 frequency; f32 velocity; char padC[4];
    s16 *filter; u8 combFilterSize; u16 combFilterGain;
} gl_27804_Attrs;
typedef struct {
    struct {
        u8 enabled : 1; u8 needsInit : 1; u8 finished : 1; u8 unused : 1;
        u8 stereoStrongRight : 1; u8 stereoStrongLeft : 1; u8 stereoHeadsetEffects : 1; u8 usesHeadsetPanEffects : 1;
    } bitField0;
    struct {
        u8 reverbIndex : 3; u8 bookOffset : 2; u8 isSyntheticWave : 1; u8 hasTwoParts : 1; u8 useHaasEffect : 1;
    } bitField1;
    u8 gain; u8 haasEffectLeftDelaySize; u8 haasEffectRightDelaySize; u8 reverbVol; u8 harmonicIndexCurAndPrev;
    u8 combFilterSize; u16 targetVolLeft; u16 targetVolRight; u16 resamplingRateFixedPoint; u16 combFilterGain;
    void *waveSampleAddr; s16 *filter;
} gl_27804_NoteSampleState;
typedef struct {
    char pad0[0x35]; u8 stereoHeadsetEffects; char pad36[0xB0 - 0x36];
    gl_27804_NoteSampleState sampleState;
} gl_27804_Note;
typedef struct { char pad[0x2076]; s8 soundOutputMode; } gl_27804_Ctx;
extern gl_27804_Ctx gl_27804_gAudioCtx;
#define gAudioCtx gl_27804_gAudioCtx
extern u16 gl_27804_gHaasEffectDelaySizes[];
extern f32 gl_27804_gHeadsetPanVolume[];
extern f32 gl_27804_gStereoPanVolume[];
extern f32 gl_27804_gDefaultPanVolume[];
extern void gl_ref_0003C2B4(gl_27804_Note *, f32);
#define gHaasEffectDelaySizes gl_27804_gHaasEffectDelaySizes
#define gHeadsetPanVolume gl_27804_gHeadsetPanVolume
#define gStereoPanVolume gl_27804_gStereoPanVolume
#define gDefaultPanVolume gl_27804_gDefaultPanVolume
#define Audio_NoteSetResamplingRate gl_ref_0003C2B4
#define true 1
#define false 0

void gl_func_00027804(gl_27804_Note *note, gl_27804_Attrs *attrs) {
    gl_27804_NoteSampleState *sampleState;
    f32 volLeft;
    f32 volRight;
    s32 halfPanIndex;
    u64 pad;
    u8 strongLeft;
    u8 strongRight;
    f32 vel;
    u8 pan;
    u8 reverbVol;
    gl_27804_StereoData stereoData;
    s32 pad2;
    s32 stereoHeadsetEffects = note->stereoHeadsetEffects;

    Audio_NoteSetResamplingRate(note, attrs->frequency);

    vel = attrs->velocity;
    pan = attrs->pan;
    reverbVol = attrs->reverbVol;
    stereoData = attrs->stereo.s;

    pan &= 0x7F;

    sampleState = &note->sampleState;
    sampleState->bitField0.stereoStrongRight = false;
    sampleState->bitField0.stereoStrongLeft = false;
    sampleState->bitField0.stereoHeadsetEffects = stereoData.stereoHeadsetEffects;
    sampleState->bitField0.usesHeadsetPanEffects = stereoData.usesHeadsetPanEffects;
    if (stereoHeadsetEffects && (gAudioCtx.soundOutputMode == 1)) {
        halfPanIndex = pan >> 1;
        if (halfPanIndex > 0x3F) {
            halfPanIndex = 0x3F;
        }

        sampleState->haasEffectRightDelaySize = gHaasEffectDelaySizes[halfPanIndex];
        sampleState->haasEffectLeftDelaySize = gHaasEffectDelaySizes[0x3F - halfPanIndex];
        sampleState->bitField1.useHaasEffect = true;

        volLeft = gHeadsetPanVolume[pan];
        volRight = gHeadsetPanVolume[0x7F - pan];
    } else if (stereoHeadsetEffects && (gAudioCtx.soundOutputMode == 0)) {
        strongLeft = strongRight = 0;
        sampleState->haasEffectLeftDelaySize = 0;
        sampleState->haasEffectRightDelaySize = 0;
        sampleState->bitField1.useHaasEffect = false;

        volLeft = gStereoPanVolume[pan];
        volRight = gStereoPanVolume[0x7F - pan];
        if (pan < 0x20) {
            strongLeft = 1;
        } else if (pan > 0x60) {
            strongRight = 1;
        }

        sampleState->bitField0.stereoStrongRight = strongRight;
        sampleState->bitField0.stereoStrongLeft = strongLeft;

        switch (stereoData.bit2) {
            case 0:
                break;

            case 1:
                sampleState->bitField0.stereoStrongRight = stereoData.strongRight;
                sampleState->bitField0.stereoStrongLeft = stereoData.strongLeft;
                break;

            case 2:
                sampleState->bitField0.stereoStrongRight = stereoData.strongRight | strongRight;
                sampleState->bitField0.stereoStrongLeft = stereoData.strongLeft | strongLeft;
                break;

            case 3:
                sampleState->bitField0.stereoStrongRight = stereoData.strongRight ^ strongRight;
                sampleState->bitField0.stereoStrongLeft = stereoData.strongLeft ^ strongLeft;
                break;
        }

    } else if (gAudioCtx.soundOutputMode == 3) {
        sampleState->bitField0.stereoHeadsetEffects = false;
        sampleState->bitField0.usesHeadsetPanEffects = false;
        volLeft = 0.707f;
        volRight = 0.707f;
    } else {
        sampleState->bitField0.stereoStrongRight = stereoData.strongRight;
        sampleState->bitField0.stereoStrongLeft = stereoData.strongLeft;
        volLeft = gDefaultPanVolume[pan];
        volRight = gDefaultPanVolume[0x7F - pan];
    }

    vel = 0.0f > vel ? 0.0f : vel;
    vel = 1.0f < vel ? 1.0f : vel;

    sampleState->targetVolLeft = (s32)((vel * volLeft) * (0x1000 - 0.001f));
    sampleState->targetVolRight = (s32)((vel * volRight) * (0x1000 - 0.001f));

    sampleState->gain = attrs->gain;
    sampleState->filter = attrs->filter;
    sampleState->combFilterSize = attrs->combFilterSize;
    sampleState->combFilterGain = attrs->combFilterGain;
    sampleState->reverbVol = reverbVol;
}
