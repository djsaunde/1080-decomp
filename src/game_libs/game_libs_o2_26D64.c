/* IDO 7.1 -O2 donor for AudioThread_ProcessCmds -- sound-command queue drain
 * (230/230 words; st_size 0x398).
 *
 * WIRED via REPLACE_FUNC_BODY donor splice into game_libs_post.c.o (the
 * 29CCC / 273B8 / 31334 recipe): the switch (code) jumptable lands in this
 * unit's local .rodata at +0; the splice renames the reloc to
 * AudioThread_ProcessCmds_rodata, pinned to 0xF50, so the `lw t2,0xF50(at)` %lo links
 * as the USO ships it (an in-unit compile cannot bake the %lo).
 *
 * Load-bearing shapes (run 132, agent-c: 43 raw -> exact):
 *  - NO mask on the CUR_D increment; the two dead u8 masks go in the flags
 *    read-modify-writes instead (`obj->flags = (obj->flags & 0xFF) | 4` in
 *    case 0x41 and `| 1` in case 0x4C): each burns one ugen t-register at the
 *    point where the target's temps run one ahead.
 *  - case 0x41's compare operand order: the target's `c.eq.s $f0,$f4` needs the
 *    seq-player field FIRST (`obj->fadeVolumeScale != ent->arg.asFloat`); the
 *    command-first field spelling gives `c.eq.s $f4,$f0`. (Run 132 got the same
 *    order from a raw `*(f32 *)((u8 *)obj + 0x2C)` read; run 522 typed it.)
 *  - ent is the AudioCmd queue slot (audio.h): op 0x0, arg0 = player 0x1,
 *    arg1 = channel / volume 0x2, arg2 = IO slot 0x3, arg 0x4. gl_func_0003ADFC
 *    is AudioThread_ProcessGlobalCmd (splat 0x26790), gl_func_0003BA24 the
 *    channel handler (splat 0x273B8).
 *  - the levers documented above AudioThread_ProcessCmds in game_libs_post.c (the
 *    extern struct-array base, per-access cursor aliases, dead masks in the
 *    prologue, `if (ent) {}` / `if (cur) {}` empty tests). */
#include "common.h"
#include "audio.h"

extern int gl_func_0003ADFC();
extern int gl_func_0003BA24();

extern int D_26D64_a, D_26D64_b, D_26D64_c, D_26D64_d;
#define CUR_A (*(u8 *)((char *)&D_26D64_a + 0x1B5DC))
#define CUR_B (*(u8 *)((char *)&D_26D64_b + 0x1B5DC))
#define CUR_C (*(u8 *)((char *)&D_26D64_c + 0x1B5DC))
#define CUR_D (*(u8 *)((char *)&D_26D64_d + 0x1B5DC))
/* SequencePlayer (audio.h, 0x160) seen with its bitfield word as a plain flags
 * byte: enabled 0x80, recalculateVolume 0x04, applyBend 0x01. The byte view is
 * what keeps the dead-mask RMWs below (the audio.h u32 bitfields lower
 * differently). Field names follow audio.h's SequencePlayer (run 522). */
typedef struct {
    /* 0x000 */ u8 flags;
    /* 0x001 */ u8 state;
    /* 0x002 */ u8 pad02[6];
    /* 0x008 */ s16 tempo;
    /* 0x00A */ s16 tempoAcc;
    /* 0x00C */ s16 tempoChange;
    /* 0x00E */ s16 transposition;
    /* 0x010 */ s16 delay;
    /* 0x012 */ s16 fadeTimer;
    /* 0x014 */ u8 pad14[8];
    /* 0x01C */ f32 fadeVolume;
    /* 0x020 */ f32 fadeVelocity;
    /* 0x024 */ f32 volume;
    /* 0x028 */ u8 pad28[4];
    /* 0x02C */ f32 fadeVolumeScale;
    /* 0x030 */ u8 pad30[4];
    /* 0x034 */ f32 bend;
    /* 0x038 */ SequenceChannel *channels[16];
    /* 0x078 */ u8 pad78[0x158 - 0x78];
    /* 0x158 */ s8 seqScriptIO[8];
} Obj26D64;
typedef struct {
    /* 0x0000 */ u8 pad0[0x2048];
    /* 0x2048 */ s16 numSequencePlayers;
    /* 0x204A */ u8 pad1[0x2D00 - 0x204A];
    /* 0x2D00 */ Obj26D64 seqPlayers[0x1B];
    /* 0x5300 */ u8 pad2[0x53BA - 0x2D00 - 0x1B * 0x160];
    /* 0x53BA */ u8 threadCmdQueueFinished;
    /* 0x53BB */ u8 pad3;
    /* 0x53BC */ u16 threadCmdChannelMask[0x3A];
    /* 0x5430 */ AudioCmd threadCmdBuf[0x100];
} St26D64;
extern St26D64 gAudioContext_26D64_arr[];
#define g (&gAudioContext_26D64_arr[0])
void AudioThread_ProcessCmds(u32 arg0) {
    s32 lo;
    s32 cur;
    s32 idx;
    AudioCmd *ent;
    Obj26D64 *obj;  /* s2: object slot  (g + pidx*0x160 + 0x2D00) */
    s32 code;
    s32 tmp;
    u8 sel;
    s32 i;
    u16 mask;
    f32 f2;

    if (g->threadCmdQueueFinished == 0) {
        CUR_A = (arg0 >> 8) & 0xFF;
    }
    lo = (u8) (arg0 & 0xFF);
    for (;;) {
        cur = CUR_B;
        idx = (cur & 0xFF) * 8;
        ent = (AudioCmd *)((u8 *)g->threadCmdBuf + idx);
        if (lo == cur) {
            g->threadCmdQueueFinished = 0;
            return;
        } if (ent) {}
        CUR_D = CUR_C + 1; /* run 132: no mask here; the t-register burns belong in the flags RMWs below (43 -> 2) */
        code = ent->op;
        if (code == 0xF8) { if (cur) {}
            g->threadCmdQueueFinished = 1;
            return;
        }
        if ((code & 0xF0) == 0xF0) {
            gl_func_0003ADFC(ent);
        } else {
            if ((s32) ent->arg0 < g->numSequencePlayers) {
                obj = &g->seqPlayers[ent->arg0];
                if (code & 0x80) {
                    gl_func_0003ADFC(ent);
                } else if (code & 0x40) {
                    switch (code) {
                    case 0x41:
                        if (obj->fadeVolumeScale != ent->arg.asFloat) { /* field-first = the target c.eq.s $f0,$f4 order (run 522) */
                            obj->fadeVolumeScale = ent->arg.asFloat;
                            obj->flags = (obj->flags & 0xFF) | 4; /* run 132: dead u8 mask = one t-register burn */
                        }
                        break;
                    case 0x42:
                        obj->tempo = ent->arg.asInt * 0x30;
                        break;
                    case 0x43:
                        obj->tempoChange = ent->arg.asInt * 0x30;
                        break;
                    case 0x44:
                        obj->tempoChange = ent->arg.asInt;
                        break;
                    case 0x45:
                        obj->transposition = ent->arg.asSbyte;
                        break;
                    case 0x46:
                        obj->seqScriptIO[ent->arg2] = ent->arg.asSbyte;
                        break;
                    case 0x47:
                        f2 = (f32) (s32) ent->arg1 / 127.0f;
                        goto block_24;
                    case 0x48:
                        f2 = ((f32) (s32) ent->arg1 / 100.0f) * obj->fadeVolume;
block_24:
                        if (obj->state != 2) {
                            obj->volume = obj->fadeVolume;
                            tmp = ent->arg.asInt;
                            if (tmp == 0) {
                                obj->fadeVolume = f2;
                            } else {
                                obj->state = 0;
                                obj->fadeTimer = tmp;
                                obj->fadeVelocity = (f2 - obj->fadeVolume) / (f32) tmp;
                            }
                        }
                        break;
                    case 0x4A:
                        if (obj->state != 2) {
                            tmp = ent->arg.asInt;
                            if (tmp == 0) {
                                obj->fadeVolume = obj->volume;
                            } else {
                                obj->state = 0;
                                obj->fadeTimer = tmp;
                                obj->fadeVelocity = (obj->volume - obj->fadeVolume) / (f32) tmp;
                            }
                        }
                        break;
                    case 0x4C:
                        obj->bend = ent->arg.asFloat;
                        if (obj->bend == 1.0f) {
                            obj->flags &= ~1;
                        } else {
                            obj->flags = (obj->flags & 0xFF) | 1;
                        }
                        break;
                    case 0x4D:
                    case 0x4E:
                        break;
                    }
                } else {
                    if (((u32) *(s32 *)obj >> 0x1F) != 0) {
                        sel = ent->arg1;
                        if (sel < 0x10) {
                            gl_func_0003BA24(obj->channels[sel], ent);
                        } else if (sel == 0xFF) {
                            mask = g->threadCmdChannelMask[ent->arg0];
                            i = 0;
                            do {
                                if (mask & 1) {
                                    gl_func_0003BA24(obj->channels[i], ent);
                                }
                                i += 1;
                                mask = ((s32) mask >> 1) & 0xFFFF;
                            } while (i != 0x10);
                        }
                    }
                }
            }
        }
        ent->op = 0;
    }
}
#undef g
#undef CUR_A
#undef CUR_B
#undef CUR_C
#undef CUR_D
