#include "common.h"

/* IDO 7.1 -O2 donor for gl_func_00026790 (0x314 / 197 words) = OoT's
 * AudioThread_ProcessGlobalCmd (thread.c) in its older form (the 1080 command set:
 * 0x82 / 0x83+0x89 / 0x86 / 0x84 / 0xF1 / 0xF2 mute / 0xF3 stop-notes falling into
 * 0xF0 unmute / 0xF4..0xF7 / 0x90 / 0xFA / 0x81 / 0xFB). Spliced into
 * game_libs_post.c.o via REPLACE_FUNC_BODY; the splice renames the donor-local
 * .rodata jumptable relocs (0xF0..0xFB at +0, 0x81..0x90 at +0x30) to
 * gl_func_00026790_rodata, pinned to the USO tables at +0xEE0. Load-bearing:
 * gAudioCtx as a typed extern struct (cursor-first notes[i] addu), seqPlayer /
 * NoteSampleState flags as u32 bitfield runs, the mute / unmute loops walking a
 * byte pointer from their own base symbol (gl_26790_ent) so the bound re-reads
 * and the two read-modify-write stores stay. */

typedef struct {
    u8 op; u8 arg0; u8 arg1; u8 arg2;
    union { void *data; f32 asFloat; s32 asInt; u16 asUShort; s8 asSbyte; u8 asUbyte; u32 asUInt; };
} gl_26790_AudioCmd;
typedef struct {
    u32 enabled : 1; u32 finished : 1; u32 muted : 1; u32 seqDmaInProgress : 1;
    u32 fontDmaInProgress : 1; u32 recalculateVolume : 1; u32 stopScript : 1; u32 applyBend : 1; u32 rest : 24;
    char pad[0x15C];
} gl_26790_SeqPlayer;
typedef struct { char pad[3]; u8 muteBehavior; } gl_26790_Channel;
typedef struct { char pad[0x50]; gl_26790_Channel *channel; } gl_26790_Layer;
typedef struct { struct { u32 enabled : 1; u32 needsInit : 1; u32 finished : 1; u32 rest : 29; } bitField0; } gl_26790_SampleState;
typedef struct {
    char pad00[0x34]; u8 unk_04; char pad35[0x44 - 0x35]; gl_26790_Layer *parentLayer;
    char pad48[0xB0 - 0x48];
    gl_26790_SampleState sampleState;
    char padB4[0xD0 - 0xB4];
} gl_26790_Note;
typedef struct {
    char pad0[0x164C]; char externalLoadQueue[0x18];
    char pad1664[0x2048 - 0x1664]; s16 numSequencePlayers;
    char pad204a[0x2070 - 0x204A]; s32 numNotes; char pad2074[2]; s8 soundOutputMode;
    char pad2077[0x2CF0 - 0x2077]; u8 resetStatus; u8 specId; char pad2cf2[0x2CFC - 0x2CF2];
    gl_26790_Note *notes; gl_26790_SeqPlayer seqPlayers[4];
    char pad[0x53BC - 0x2D00 - 4 * 0x160]; u16 threadCmdChannelMask[4];
} gl_26790_Ctx;
extern int D_00000000;
extern gl_26790_Ctx gl_26790_gAudioCtx;
#define gAudioCtx gl_26790_gAudioCtx
extern u8 gl_26790_ent[];
extern int gl_26790_w;
extern int gl_func_00000000();
extern int func_3b164(), func_3b110();

void gl_func_00026790(gl_26790_AudioCmd *cmd) {
    s32 i;
    s32 pad[3];
    u32 flags;
    u8 *p;

    switch (cmd->op) {
        case 0x82:
            gl_func_00000000(cmd->arg1, 3);
            break;

        case 0x83:
        case 0x89:
            gl_func_00000000(cmd->arg0, cmd->arg1, cmd->arg2);
            func_3b164(cmd->arg0, cmd->asInt);
            break;

        case 0x86:
            gl_func_00000000(cmd->arg0, cmd->arg1, cmd->asInt);
            break;

        case 0x84:
            if (gAudioCtx.seqPlayers[cmd->arg0].enabled) {
                if (cmd->asInt == 0) {
                    gl_func_00000000(&gAudioCtx.seqPlayers[cmd->arg0]);
                } else {
                    func_3b110(cmd->arg0, cmd->asInt);
                }
            }
            break;

        case 0xF1:
            gAudioCtx.soundOutputMode = cmd->asUInt;
            break;

        case 0xF2:
            p = gl_26790_ent;
            for (i = 0; i < gAudioCtx.numSequencePlayers; i++) {
                ((gl_26790_Ctx *)p)->seqPlayers[0].muted = 1;
                ((gl_26790_Ctx *)p)->seqPlayers[0].recalculateVolume = 1;
                p += sizeof(gl_26790_SeqPlayer);
            }
            break;

        case 0xF3:
            if (cmd->asUInt == 1) {
                for (i = 0; i < gAudioCtx.numNotes; i++) {
                    gl_26790_Note *note = &gAudioCtx.notes[i];
                    gl_26790_SampleState *sampleState = &note->sampleState;

                    if (sampleState->bitField0.enabled && (note->unk_04 == 0) &&
                        (note->parentLayer->channel->muteBehavior & 8)) {
                        sampleState->bitField0.finished = 1;
                    }
                }
            }
        case 0xF0:
            p = gl_26790_ent;
            for (i = 0; i < gAudioCtx.numSequencePlayers; i++) {
                ((gl_26790_Ctx *)p)->seqPlayers[0].muted = 0;
                ((gl_26790_Ctx *)p)->seqPlayers[0].recalculateVolume = 1;
                p += sizeof(gl_26790_SeqPlayer);
            }
            break;

        case 0xF4:
            gl_func_00000000(cmd->arg0, cmd->arg1, cmd->arg2);
            break;

        case 0xF5:
            gl_func_00000000(cmd->arg0, cmd->arg1, cmd->arg2, &gAudioCtx.externalLoadQueue);
            break;

        case 0xF6:
            gl_func_00000000(cmd->arg0, cmd->arg1, cmd->arg2, &gAudioCtx.externalLoadQueue);
            break;

        case 0xF7:
            gl_func_00000000(cmd->arg1);
            break;

        case 0x90:
            gAudioCtx.threadCmdChannelMask[cmd->arg0] = cmd->asUShort;
            break;

        case 0xFA:
            gAudioCtx.resetStatus = 5;
            gAudioCtx.specId = cmd->asUInt;
            break;

        case 0x81:
            gl_26790_w = cmd->asUInt;
            break;

        case 0xFB:
            break;
    }
}
