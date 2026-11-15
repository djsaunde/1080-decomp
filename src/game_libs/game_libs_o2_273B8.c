/* AudioThread_ProcessSeqPlayerCmd -- 14-case jumptable message handler (switch on
 * msg[0]): float/s8/short setters on the object with change-flag bits in
 * byte 1, a bit-4 insert into byte 0, and one indexed byte store.
 *
 * IDO 7.1 -O2 REPLACE_FUNC_BODY donor for game_libs_post.c.o (99/99 words;
 * the only word the build cannot bake is the `lw t7,%lo(jtbl)(at)` load).
 * A C `switch` puts its jumptable in this unit's local .rodata; the splice
 * renames that reloc to AudioThread_ProcessSeqPlayerCmd_rodata, pinned in
 * undefined_syms_auto.txt to the table the USO ships in its data at +0xF88
 * (14 entries; entry 7 = default). Same recipe as game_libs_o2_29CCC.c.
 *
 * Load-bearing: the change flags (byte 1 bits 0x80/0x40/0x20) and the
 * byte-0 bit 0x10 are 1-bit unsigned BITFIELDS, not `a0[1] |= K` /
 * `(a0[0] & ~0x10) | ((v << 4) & 0x10)`. Each bitfield insert lowers through
 * one extra eliminated temp, which burns the ugen ring slot the old byte
 * spelling was missing (the documented "+5 temp-ring rotation" = cases
 * 1, 2, 3, 4 and 8, one slot each). See docs/IDO_CODEGEN.md
 * #single-bit-bitfield-set-extra-ring-temp-29000. */
#include "common.h"
#include "audio.h"

/* Typed (agent-c run 522): OoT's AudioThread_ProcessChannelCmd on SequenceChannel /
 * AudioCmd. The changes flags are SequenceChannel's changesFreqScale (0x80) /
 * changesVolume (0x40) / changesPan (0x20) and byte-0 bit 0x10 is stopSomething2:
 * the audio.h u32 bitfields, so the load-bearing bitfield inserts are kept.
 * Compare order: channel field FIRST (`channel->volumeScale != cmd->arg.asFloat`)
 * gives the target's `c.eq.s $f0,$f4` / `beq v0,t4`; the command-first field
 * spelling swaps both (the raw form needed the opposite order). */
void AudioThread_ProcessSeqPlayerCmd(SequenceChannel *channel, AudioCmd *cmd) {
    switch (cmd->op) {
    case 1: /* SET_VOL_SCALE */
        if (channel->volumeScale != cmd->arg.asFloat) {
            channel->volumeScale = cmd->arg.asFloat;
            channel->changesVolume = 1;
        }
        return;
    case 2: /* SET_VOL */
        if (channel->volume != cmd->arg.asFloat) {
            channel->volume = cmd->arg.asFloat;
            channel->changesVolume = 1;
        }
        return;
    case 3: /* SET_PAN */
        if (channel->newPan != cmd->arg.asSbyte) {
            channel->newPan = cmd->arg.asSbyte;
            channel->changesPan = 1;
        }
        return;
    case 4: /* SET_FREQ_SCALE */
        if (channel->freqScale != cmd->arg.asFloat) {
            channel->freqScale = cmd->arg.asFloat;
            channel->changesFreqScale = 1;
        }
        return;
    case 5: /* SET_REVERB_VOLUME */
        if (channel->targetReverbVol != cmd->arg.asSbyte) {
            channel->targetReverbVol = cmd->arg.asSbyte;
        }
        return;
    case 6: { /* SET_IO */
        int idx = cmd->arg2;
        if (idx < 8) {
            channel->seqScriptIO[idx] = cmd->arg.asSbyte;
        }
        return;
    }
    case 8: /* SET_MUTE */
        channel->stopSomething2 = cmd->arg.asSbyte;
        return;
    case 9: /* SET_MUTE_BEHAVIOR */
        channel->muteBehavior = cmd->arg.asSbyte;
        return;
    case 10: /* SET_VIBRATO_DEPTH */
        channel->vibratoDepthTarget = cmd->arg.asUbyte << 3;
        channel->vibratoDepthChangeDelay = 1;
        return;
    case 11: /* SET_VIBRATO_RATE */
        channel->vibratoRateTarget = cmd->arg.asUbyte << 5;
        channel->vibratoRateChangeDelay = 1;
        return;
    case 12: /* SET_COMB_FILTER_SIZE */
        channel->combFilterSize = cmd->arg.asUbyte;
        return;
    case 13: /* SET_COMB_FILTER_GAIN */
        channel->combFilterGain = cmd->arg.asUShort;
        return;
    case 14: /* SET_STEREO */
        channel->stereo.asByte = cmd->arg.asUbyte;
        return;
    }
}
