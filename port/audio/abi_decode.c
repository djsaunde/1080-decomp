/*
 * port/audio/abi_decode.c: walks the Acmd list that 1080's AudioSynth_*
 * builds (two u32 words per command, as the game wrote them) and dispatches
 * each command to the CPU mixer. The field layouts are those of the
 * builders in src/game_libs/game_libs_post.c (gl_e134_ / gl_aud_ macros and
 * the AudioSynth_* wrappers), which are OoT's abi.h encodings.
 */
#include "mixer.h"

void mixer_exec(Mixer *m, uint32_t w0, uint32_t w1) {
    uint32_t op = (w0 >> 24) & 0x7F; /* the ucode indexes with (w0 >> 23) & 0xFE */
    uint8_t f = (uint8_t)(w0 >> 16);
    uint16_t lo0 = (uint16_t)w0;
    uint16_t hi1 = (uint16_t)(w1 >> 16);
    uint16_t lo1 = (uint16_t)w1;
    uint16_t size16 = (uint16_t)((w0 >> 12) & 0xFF0); /* (size >> 4) in bits 16-23 */

    m->n_cmds++;
    switch (op) {
        case MIX_A_SPNOOP:
            mix_spnoop(m);
            break;
        case MIX_A_ADPCM: /* aADPCMdec(f, state) */
            mix_adpcm(m, f, w1);
            break;
        case MIX_A_CLEARBUFF: /* aClearBuffer(dmem, size) */
            mix_clearbuff(m, lo0, lo1);
            break;
        case MIX_A_UNK3: /* AudioSynth_UnkCmd3(a1, a2, size) */
            mix_unk3(m, hi1, lo1, lo0);
            break;
        case MIX_A_ADDMIXER: /* aAddMixer(count, dmemi, dmemo, 0x7FFF) */
            mix_addmixer(m, size16, hi1, lo1);
            break;
        case MIX_A_RESAMPLE: /* aResample(f, pitch, state) */
            mix_resample(m, f, lo0, w1);
            break;
        case MIX_A_RESAMPLE_ZOH: /* aResampleZoh(pitch, pitchAccu) */
            mix_resample_zoh(m, lo0, lo1);
            break;
        case MIX_A_FILTER: /* aFilter(f, countOrBuf, addr) */
            mix_filter(m, f, lo0, w1);
            break;
        case MIX_A_SETBUFF: /* aSetBuffer(f, in, out, count) */
            mix_setbuff(m, lo0, hi1, lo1);
            break;
        case MIX_A_DUPLICATE: /* aDuplicate(numCopies, src, dst) */
            mix_duplicate(m, f, lo0, hi1);
            break;
        case MIX_A_DMEMMOVE: /* aDMEMMove(in, out, count) */
            mix_dmemmove(m, lo0, hi1, lo1);
            break;
        case MIX_A_LOADADPCM: /* aLoadADPCM(count, book) */
            mix_loadadpcm(m, w0 & 0xFFFFFF, w1);
            break;
        case MIX_A_MIXER: /* aMix(count >> 4, gain, in, out) */
            mix_mixer(m, size16, (int16_t)lo0, hi1, lo1);
            break;
        case MIX_A_INTERLEAVE: /* aInterleave(out, l, r, c) */
            mix_interleave(m, size16, lo0, hi1, lo1);
            break;
        case MIX_A_HILOGAIN: /* AudioSynth_HiLoGain(gain, in, out, size) */
            mix_hilogain(m, f, lo0, hi1);
            break;
        case MIX_A_SETLOOP: /* aSetLoop(addr) */
            mix_setloop(m, w1);
            break;
        case MIX_A_INTERL: /* AudioSynth_InterL(in, out, numSamples) */
            mix_interl(m, lo0, hi1, lo1);
            break;
        case MIX_A_ENVSETUP1: /* aEnvSetup1(wet, rateWet, rateL, rateR) */
            mix_envsetup1(m, f, lo0, hi1, lo1);
            break;
        case MIX_A_ENVMIXER: /* aEnvMixer(..., dests, op) */
            mix_envmixer(m, w0, w1);
            break;
        case MIX_A_LOADBUFF: /* aLoadBuffer(src, dmem, size) */
            mix_loadbuff(m, size16, lo0, w1);
            break;
        case MIX_A_SAVEBUFF: /* aSaveBuffer(dmem, dst, size) */
            mix_savebuff(m, size16, lo0, w1);
            break;
        case MIX_A_ENVSETUP2: /* aEnvSetup2(volL, volR) */
            mix_envsetup2(m, hi1, lo1);
            break;
        case MIX_A_S8DEC: /* AudioSynth_S8Dec(f, state) */
            mix_s8dec(m, f, w1);
            break;
        case MIX_A_UNK19: /* AudioSynth_UnkCmd19 */
            mix_unk19(m, w0, w1);
            break;
        default: /* 16 (ucode 0x1780, never emitted), 24, 26+ */
            m->n_unknown++;
            break;
    }
}

void audio_abi_run(Mixer *m, const uint32_t *words, int ncmds) {
    int i;
    for (i = 0; i < ncmds; i++) {
        mixer_exec(m, words[i * 2], words[i * 2 + 1]);
    }
}
