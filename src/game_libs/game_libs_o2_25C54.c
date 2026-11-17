#include "common.h"
#include "audio.h"
#include "uso_call.h"

/* IDO 7.1 -O2 donor for AudioLoad_PreloadSamplesForFont (0x460 / 280 words) = OoT's
 * AudioLoad_PreloadSamplesForFont (load.c) in its older form: no sound-effect loop,
 * instruments before drums, a four-argument AllocSampleCache (no cache-type
 * argument; the persistent / temporary calls are the same blank jal), and every
 * preloadSampleStack field written through gAudioCtx.preloadSampleStack[top] (the
 * repeated `* 0x14` makes IDO hold 20 in s5 for multu, as the target). The
 * `if (numInstruments) {}` block split gives numInstruments the spill and numDrums
 * s7. Spliced into game_libs_post.c.o via REPLACE_FUNC_BODY (no rodata). */
/* Sample / TunedSample / Instrument / Drum / AudioPreloadReq / SoundFont / the
 * context from include/audio.h (agent-c run 522). */
extern AudioContext gAudioContext_25C54;
#define gAudioCtx gAudioContext_25C54
extern Instrument *gl_25C54_GetInstrumentInner(s32, s32);
extern Drum *gl_25C54_GetDrum(s32, s32);
extern void gl_ref_0003A268(TunedSample *);
extern u8 *gl_25C54_AllocSampleCache(u32, s32, void *, s8);
extern void gl_25C54_SyncDmaUnkMedium(u32, u8 *, u32, s32);
extern void gl_25C54_SyncDma(u32, u8 *, u32, s32);
extern void gl_ref_0003959C(u32, void *, u32, s32, s32, void *, s32);
#define AudioLoad_AddUsedSample gl_ref_0003A268
#define AudioLoad_StartAsyncLoad gl_ref_0003959C
#define ALIGN16(x) (((x) + 0xF) & ~0xF)

void AudioLoad_PreloadSamplesForFont(s32 fontId, s32 async, SampleBankRelocInfo *sampleBankReloc) {
    s32 numDrums;
    s32 numInstruments;
    Drum *drum;
    Instrument *instrument;
    AudioPreloadReq *preload;
    AudioPreloadReq *topPreload;
    u8 *addr;
    s32 size;
    s32 i;
    Sample *sample;
    s32 preloadInProgress;
    s32 nChunks;

    preloadInProgress = 0;
    if (gAudioCtx.preloadSampleStackTop != 0) {
        preloadInProgress = 1;
    }

    gAudioCtx.numUsedSamples = 0;

    numInstruments = gAudioCtx.soundFontList[fontId].numInstruments;
    numDrums = gAudioCtx.soundFontList[fontId].numDrums;

    if (numInstruments) {}
    for (i = 0; i < numInstruments; i++) {
        instrument = gl_25C54_GetInstrumentInner(fontId, i);
        if (instrument != NULL) {
            if (instrument->normalRangeLo != 0) {
                AudioLoad_AddUsedSample(&instrument->lowPitchTunedSample);
            }
            if (instrument->normalRangeHi != 0x7F) {
                AudioLoad_AddUsedSample(&instrument->highPitchTunedSample);
            }
            AudioLoad_AddUsedSample(&instrument->normalPitchTunedSample);
        }
    }

    for (i = 0; i < numDrums; i++) {
        drum = gl_25C54_GetDrum(fontId, i);
        if (drum != NULL) {
            AudioLoad_AddUsedSample(&drum->tunedSample);
        }
    }

    if (gAudioCtx.numUsedSamples == 0) {
        return;
    }

    size = 0;
    for (i = 0; i < gAudioCtx.numUsedSamples; i++) {
        size += ALIGN16(gAudioCtx.usedSamples[i]->size);
    }
    if (size) {}

    for (i = 0; i < gAudioCtx.numUsedSamples; i++) {
        if (gAudioCtx.preloadSampleStackTop == 120) {
            break;
        }

        sample = gAudioCtx.usedSamples[i];
        if (sample->medium == 0) {
            continue;
        }

        switch (async) {
            case 0:
                if (sample->medium == sampleBankReloc->medium1) {
                    addr = XCALL(gl_25C54_AllocSampleCache, aud_AllocSampleCachePersistent)(sample->size, sampleBankReloc->sampleBankId1, sample->sampleAddr,
                                                     sample->medium);
                } else if (sample->medium == sampleBankReloc->medium2) {
                    addr = XCALL(gl_25C54_AllocSampleCache, aud_AllocSampleCachePersistent)(sample->size, sampleBankReloc->sampleBankId2, sample->sampleAddr,
                                                     sample->medium);
                }
                break;

            case 1:
                if (sample->medium == sampleBankReloc->medium1) {
                    addr = XCALL(gl_25C54_AllocSampleCache, aud_AllocSampleCacheTemporary)(sample->size, sampleBankReloc->sampleBankId1, sample->sampleAddr,
                                                     sample->medium);
                } else if (sample->medium == sampleBankReloc->medium2) {
                    addr = XCALL(gl_25C54_AllocSampleCache, aud_AllocSampleCacheTemporary)(sample->size, sampleBankReloc->sampleBankId2, sample->sampleAddr,
                                                     sample->medium);
                }
                break;
        }
        if (addr == NULL) {
            continue;
        }

        switch (async) {
            case 0:
                if (sample->medium == 1) {
                    gl_25C54_SyncDmaUnkMedium((u32)sample->sampleAddr, addr, sample->size,
                                              gAudioCtx.sampleBankTable->header.unkMediumParam);
                    sample->sampleAddr = addr;
                    sample->medium = 0;
                } else {
                    gl_25C54_SyncDma((u32)sample->sampleAddr, addr, sample->size, sample->medium);
                    sample->sampleAddr = addr;
                    sample->medium = 0;
                }
                break;

            case 1:
                gAudioCtx.preloadSampleStack[gAudioCtx.preloadSampleStackTop].sample = sample;
                gAudioCtx.preloadSampleStack[gAudioCtx.preloadSampleStackTop].ramAddr = addr;
                gAudioCtx.preloadSampleStack[gAudioCtx.preloadSampleStackTop].encodedInfo =
                    (gAudioCtx.preloadSampleStackTop << 24) | 0xFFFFFF;
                gAudioCtx.preloadSampleStack[gAudioCtx.preloadSampleStackTop].isFree = 0;
                gAudioCtx.preloadSampleStack[gAudioCtx.preloadSampleStackTop].endAndMediumKey =
                    (u32)sample->sampleAddr + sample->size + sample->medium;
                gAudioCtx.preloadSampleStackTop++;
                break;
        }
    }
    gAudioCtx.numUsedSamples = 0;

    if (gAudioCtx.preloadSampleStackTop != 0 && !preloadInProgress) {
        sample = gAudioCtx.preloadSampleStack[gAudioCtx.preloadSampleStackTop - 1].sample;
        nChunks = (sample->size >> 12) + 1;
        AudioLoad_StartAsyncLoad((u32)sample->sampleAddr,
                                 gAudioCtx.preloadSampleStack[gAudioCtx.preloadSampleStackTop - 1].ramAddr,
                                 sample->size, sample->medium, nChunks, &gAudioCtx.preloadSampleQueue,
                                 gAudioCtx.preloadSampleStack[gAudioCtx.preloadSampleStackTop - 1].encodedInfo);
    }
}
