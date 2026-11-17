/* Native original-compiler source port from src/game_libs/game_libs_post.c (the snd_SoundObjUpdate NON_MATCHING body,
 * comments dropped; see that file for the per-line lever history). agent-g run 284: the in-tree body compiles to the
 * target's 921 words once linked with the callees / tables at their own addresses (scripts/match-origins-snd_SoundObjUpdate.ld).
 * gl_E354_v: the switch-1 0x90000000 command call through a void prototype (result never read; an int result defines v0
 * inside the 8 / 0x40 pick's live range and uopt forbids it v0). arg1: the switch-1 selector and the o->0x15 holder
 * (the parameter is otherwise unused; each read follows its own write). */
#include "context_game_libs_post.h"
#define COMMON_H /* the context header already typedefs u8..f64; audio.h needs only the types */
#include "../../include/audio.h"
#include "uso_call.h"
/* arg0 stays char * (every callee here takes char *); SO() reads it as audio.h's 0x64-byte SoundObj.
 * Five reads are spelled (*SO(arg0)).field: there the -> spelling swaps a commutative operand
 * pair (mul.s / add.s / beql) relative to its neighbour, and the (*p).f spelling keeps the
 * original ucode order (each spelling checked by objdump -d -r of the object; agent-a run 531). */
#define SO(p) ((SoundObj *)(p))
extern int gl_func_000428FC();
extern void gl_E354_emitf(s32, s32, s32, PORT64_BITS(f32), s32, s32); /* PORT64_BITS: the callees take the bits (plan 21.10) */
extern int gl_func_00043824(float, ...);
extern int gl_func_000438F4();
extern int gl_func_00044C38();
extern s32 D_cfg_E354;
extern s32 D_stateB_E354;
extern s32 D_stateC_E354;
extern s32 D_stateD_E354;
extern s32 D_stateF_E354;
extern s32 D_stateE_E354;
extern s32 D_raceA_E354;
extern s32 D_raceB_E354;
extern s32 D_raceC_E354;
extern s32 D_raceD_E354;
extern f32 D_E354_1C330[], D_E354_1C3B0[], D_E354_1C310[];
extern s32 gl_E354_setf(s32, PORT64_BITS(f32));
extern f32 D_E354_1C738_a, D_E354_1C738_b, D_E354_1C738_c, D_E354_1C73C_a, D_E354_1C73C_b, D_E354_1C73C_c;
extern char D_E354_G[];
extern void D_E354_GA(void);
extern s32 D_E354_GB[];
extern void gl_E354_v(s32, s32);
#ifdef PORT /* one object for the bytes these tokens share, so gcc keeps the order (scripts/port-alias-audit.py, plan 18.7) */
extern char port_bootup_data[];
#define D_E354_1C738_a (*(__typeof__(D_E354_1C738_a) *)(port_bootup_data + 0x1C738))
#define D_E354_1C738_b (*(__typeof__(D_E354_1C738_b) *)(port_bootup_data + 0x1C738))
#define D_E354_1C738_c (*(__typeof__(D_E354_1C738_c) *)(port_bootup_data + 0x1C738))
#define D_E354_1C73C_a (*(__typeof__(D_E354_1C73C_a) *)(port_bootup_data + 0x1C73C))
#define D_E354_1C73C_b (*(__typeof__(D_E354_1C73C_b) *)(port_bootup_data + 0x1C73C))
#endif
void snd_SoundObjUpdate(char *arg0, s32 arg1) {
    u8 temp_v1_2;
    u8 temp_v1_3;
    u8 sp7D;
    u8 temp_v1_32;
    f32 sp78;
    f32 temp_f0;
    f32 temp_f10_2;
    f32 var_f4;
    u8 sp6B;
    u8 temp_v1_4;
    u8 temp_v1_5;
    u8 var_v0_2;
    f32 sp64;
    f32 sp60;
    f32 temp_f16;
    s32 var_v0;
    f32 temp_f18;
    u32 sp50;
    s32 temp_v0;
    s32 temp_v0_2;
    u32 temp_v0_7;
    s32 temp_v1;
    s32 var_t9;
    s32 var_v0_3;
    sp64 = 1.0f;
    temp_v0 = D_cfg_E354;
    sp60 = 1.0f;
    if (temp_v0 != 0) {
        if (temp_v0 == 4) {
            temp_v0_7 = D_stateC_E354;  if (1) {}
            if ((((u32)temp_v0_7 == 1) || ((u32)temp_v0_7 == 2) || ((u32)temp_v0_7 == 3)) && (SO(arg0)->flags1C == 1U) && (SO(arg0)->kind == 0)) {
                SO(arg0)->kind = 2U;
                SO(arg0)->voice.volScale = 0.0f;
            }
        }
        if (D_stateB_E354 == 1) {
            SO(arg0)->unk23 = 0U;
            return;
        }
        if ((D_stateD_E354 != 1) && (SO(arg0)->unk23 != 0)) {
            if (SO(arg0)->fired == 0) {
                FCALL(snd_SoundObjReset)(arg0);
                return;
            }
            temp_f18 = SO(arg0)->unk04 * (*SO(arg0)).speed * 100.0f;
            temp_v1_32 = SO(arg0)->scaleIdx;
            SO(arg0)->fired = 0U; if (1) {}
            if (temp_v1_32) {
                sp64 = D_E354_1C330[temp_v1_32];
                temp_f0 = D_E354_1C3B0[temp_v1_32];
                sp60 *= temp_f0;
                SO(arg0)->unk04 = (f32) (SO(arg0)->unk04 * temp_f0);
                SO(arg0)->scaleIdx = (u8) (temp_v1_32 - 1);
            }
            temp_f0 = SO(arg0)->unk50 - SO(arg0)->speed;
            if (0.5f <= temp_f0) {
                temp_f0 = temp_f0 + 0.5f;
                if (2.0f <= temp_f0) {
                    temp_f0 = 2.0f;
                }
                sp60 *= temp_f0;
            }
            SO(arg0)->unk50 = (f32) ((SO(arg0)->speed + (*SO(arg0)).unk50) * 0.5f);
            SO(arg0)->unk44 = (f32) ((SO(arg0)->unk44 + temp_f18) * 0.5f);
            sp7D = gl_func_00043824(SO(arg0)->unk04 * 120.0f, (s32)((char *)&D_00000000 + 0x1C250), 0x10);
            temp_v1 = (u8)((gl_func_00043824(SO(arg0)->unk44, (s32)((char *)&D_00000000 + 0x1C290), 0x20) * 4) & 0xFF);
            sp78 = *(f32 *)((char *)&D_00000000 + 0x16E0) + (*SO(arg0)).speed / 200.0f;
            temp_v0_2 = temp_v1;
            var_f4 = (f32) (u32) temp_v0_2;
            sp78 = sp78 * sp64;
            temp_f10_2 = var_f4 * sp60;
            var_t9 = (u32) temp_f10_2;
            SO(arg0)->unk26 = (u8) (((s32) SO(arg0)->unk26 / 2) + ((s32) (var_t9 & 0xFF) / 2));
            if (SO(arg0)->speed == 0.0f) {
                SO(arg0)->unk26 = 0U;
            }
            if (SO(arg0)->reverbOnSent != (*SO(arg0)).reverbOn) {
                if (SO(arg0)->reverbOn == 1) {
                    var_v0 = 0x40;
                } else {
                    var_v0 = 8;
                }
                arg1 = SO(arg0)->kind; if (var_v0 != arg1) {}
                switch (arg1) {
                case 0:
                    gl_E354_v(0x90000000, 6 + 0 * (int)(var_v0 + var_v0 + var_v0));
                    FCALL(AudioThread_QueueCmdS8)(0x0500FF00, (s8) var_v0);
                    FCALL(AudioThread_QueueCmdS8)(0x05020000, (s8) var_v0);
                    break;
                case 1:
                case 2:
                    gl_E354_v(0x90000000, 0x60 + 0 * (int)(var_v0 + var_v0 + var_v0));
                    FCALL(AudioThread_QueueCmdS8)(0x0500FF00, (s8) var_v0);
                    FCALL(AudioThread_QueueCmdS8)(0x05020100, (s8) var_v0);
                    break;
                }
                SO(arg0)->reverbOnSent = SO(arg0)->reverbOn;
            }
            if (SO(arg0)->flags1C & 1) {
                temp_f0 = SO(arg0)->voice.volScale;
                SO(arg0)->unk26 = (u32) ((f32) (u32) SO(arg0)->unk26 * temp_f0);
                sp78 *= SO(arg0)->voice.turnAtten;
                temp_v0 = SO(arg0)->voice.pan[0];
                SO(arg0)->unk28 = (u8) ((s32) (temp_v0 + SO(arg0)->unk28) >> 1);
                if (SO(arg0)->kind == 0) {
                    gl_E354_setf(0x01020000, temp_f0);
                    FCALL(AudioThread_QueueCmdS8)(0x03020000, (s8) SO(arg0)->unk28);
                    FCALL(AudioThread_QueueCmdU16)(0x90000000, 0x806);
                    FCALL(AudioThread_QueueCmdS8)(0x0C00FF00, *(s8 *)&SO(arg0)->voice.pan[3]);
                    temp_f10_2 = FCALL(AudioThread_QueueCmdU16)(0x0D00FF00, 0x3000);
                } else {
                    gl_E354_setf(0x01020100, temp_f0);
                    FCALL(AudioThread_QueueCmdS8)(0x03020100, (s8) SO(arg0)->unk28);
                    FCALL(AudioThread_QueueCmdU16)(0x90000000, 0x1060);
                    FCALL(AudioThread_QueueCmdS8)(0x0C00FF00, *(s8 *)&SO(arg0)->voice.pan[3]);
                    temp_f10_2 = FCALL(AudioThread_QueueCmdU16)(0x0D00FF00, 0x3000);
                }
            } else {
                if (D_stateF_E354 == 4) {
                    switch (SO(arg0)->kind) {
                    case 0:
                        SO(arg0)->unk28 = 0x10U;
                        break;
                    case 1:
                        SO(arg0)->unk28 = 0x70U;
                        break;
                    }
                } else {
                    SO(arg0)->unk28 = 0x40U;
                }
                if (SO(arg0)->kind == 0) {
                    FCALL(AudioThread_QueueCmdF32)(0x01020000, 0x3F800000);
                    temp_f10_2 = FCALL(AudioThread_QueueCmdS8)(0x03020000, (s8) SO(arg0)->unk28);
                } else {
                    FCALL(AudioThread_QueueCmdF32)(0x01020100, 0x3F800000);
                    temp_f10_2 = FCALL(AudioThread_QueueCmdS8)(0x03020100, (s8) SO(arg0)->unk28);
                }
            }
            if (SO(arg0)->flags & 1) {
                SO(arg0)->unk08 = (f32) (SO(arg0)->unk08 + 50.0f);
                gl_func_00043BE4(arg0, 0, SO(arg0)->unk2C - 1, SO(arg0)->unk08, (s32) SO(arg0)->unk28);
                temp_f10_2 = FCALL(snd_SoundObjRequestCue)(arg0, (*(u32 *)((char *)&D_00000000 + 0x2154) & 7) + 0x20);
                SO(arg0)->flags = (u8) (SO(arg0)->flags & ~1);
            }
            var_v0_2 = SO(arg0)->unk14;
            temp_v0_2 = var_v0_2; if (1) {}
            if (var_v0_2 != (*SO(arg0)).unk2C) {
                if (SO(arg0)->unk2C == 0) {
                    temp_v0 = SO(arg0)->unk38;
                    SO(arg0)->unk08 = (f32) ((f32) temp_v0 * 3.0f);
                    if (temp_v0 < 0xA) {
                        SO(arg0)->unk08 = 0.0f;
                    }
                    var_v0_2 = 1;
                    if (SO(arg0)->unk14 == 1) {
                        arg1 = SO(arg0)->unk15;
                        switch (arg1) {
                        case 1: var_t9 = 0; break;
                        case 2: var_t9 = 1; break;
                        default: var_t9 = 2; break;
                        }
                    } else {
                        var_t9 = 3;
                    }
                    switch (SO(arg0)->surface) {
                    case 3: case 4:
                        gl_func_00043BE4(arg0, 8, var_t9, SO(arg0)->unk08, (s32) SO(arg0)->unk28);
                        break;
                    case 1: case 2: case 5:
                    default:
                        gl_func_00043BE4(arg0, 0xC, var_t9, SO(arg0)->unk08, (s32) SO(arg0)->unk28);
                        break;
                    }
                    var_v0_2 = SO(arg0)->unk14;
                    goto block_2C;
                } else {
                    var_v0_2 = SO(arg0)->unk14; if (temp_v0_2 == 0) {
                        SO(arg0)->unk38 = 0;
                        SO(arg0)->unk33 = 0U;
                        goto block_81;
                    }
                    goto block_2C;
block_81:
                        var_v0_2 = SO(arg0)->unk14;
block_2C:
                    SO(arg0)->unk2C = var_v0_2;
                    goto block_83;
                }
            } else {
block_83:
                if (*(u16 *)&SO(arg0)->unk1A != 0) {
                    if (SO(arg0)->unk48 == 0) {
                        temp_f16 = SO(arg0)->speed;
                        sp6B = 0x11;
                        if (75.0f <= temp_f16) {
                            sp6B = 0x13;
                        } else if (50.0f <= temp_f16) {
                            sp6B = 0x12;
                        }
                        if (25.0f <= temp_f16) {
                            gl_func_00043BE4(arg0, 0x10, sp6B, 127.0f, (s32) SO(arg0)->unk28);
                        }
                    }
                    SO(arg0)->unk2A = sp6B;
                    *(u16 *)&SO(arg0)->unk1A = 0U;
                    SO(arg0)->unk48 = 5;
                }
                if (SO(arg0)->flags & 4) {
                    sp50 = 4;
                    { int rv = gl_func_000428FC(arg0); var_v0_2 = rv;
                    sp6B = rv;
                    temp_v1_2 = SO(arg0)->level;
                    temp_f10_2 = D_E354_1C310[temp_v1_2];
                    if (rv < 0xB) {
                        switch (temp_v1_2) {
                        default:
                            var_t9 = var_v0_2;
                            break;
                        case 0:
                        case 1:
                            var_t9 = var_v0_2 + 1;
                            break;
                        case 2:
                        case 3:
                            var_t9 = var_v0_2;
                            break;
                        }
                    } else {
                        var_t9 = var_v0_2;
                    } }
                    if ((SO(arg0)->eventType != 1) || (temp_v1_2 != 3)) {
                        temp_v1_5 = SO(arg0)->surface;
                        if ((s32) temp_v1_5 >= 6) {
                            var_t9 = temp_v1_5 + 0xE;
                        }
                        if ((*(s32 *)D_E354_G == 4) || (SO(arg0)->flags1C & 1)) { /* D_E354_G: Data 0x1C0B4 (Sym 1432), the word D_stateC / D_stateF read */
                            gl_func_00043BE4(arg0, 0x10, var_t9, temp_f10_2, (s32) SO(arg0)->unk28); var_v0_2 = 0;
                        } else {
                            var_v0_3 = SO(arg0)->eventArg;
                            if (var_v0_3 == 1) {
                                var_v0_3 = 0x7F;
                            } else if (var_v0_3 == 3) {
                                var_v0_3 = 0;
                            } else {
                                var_v0_3 = 0x40;
                            }
                            if (SO(arg0)->unk1E != 0) {
                                var_v0_3 = (0x7F - var_v0_3) & 0xFF;
                            }
                            gl_func_00043BE4(arg0, 0x10, var_t9, temp_f10_2, var_v0_3); var_v0_2 = 0;
                        }
                    }
                    switch ((u8) sp6B) {
                    case 0:
                    case 2:
                        temp_v1 = 8;
                        break;
                    case 4:
                    case 6:
                        temp_v1 = 0xC;
                        break;
                    case 8:
                        temp_v1 = 0x10;
                        break;
                    case 10:
                        temp_v1 = 0x38;
                        break;
                    default:
                        temp_v1 = 0x10;
                        temp_v0_7 = SO(arg0)->level;
                        switch ((unsigned)temp_v0_7) {
                        case 0:
                            sp50 = 6;
                            break;
                        case 1:
                            temp_v1 = 0x18;
                            break;
                        case 2:
                            temp_v1 = 0x1C;
                            break;
                        default:
                            temp_v1 = 8;
                            break;
                        }
                        break;
                    }
                    FCALL(snd_SoundObjRequestCue)(arg0, ((u32) *(s32 *)((char *)&D_00000000 + 0x2154) % sp50) + (0, temp_v1));
                    SO(arg0)->unk48 = 0xA;
                    SO(arg0)->level = 0U;
                    SO(arg0)->unk2B = sp6B;
                    SO(arg0)->flags = (u8) (SO(arg0)->flags & 0xFFFB);
                }
            }
            if (SO(arg0)->unk48 != 0) {
                if (--SO(arg0)->unk48 == 0) {
                    SO(arg0)->lastLevel = 0;
                }
            }
            if (SO(arg0)->unk23 != 0) {
                temp_v0_2 = SO(arg0)->unk14;
                if ((D_raceA_E354 == 1) && (((u32)D_raceB_E354 == 4) || ((u32)D_raceB_E354 == 0xA)) && (((u32)D_raceB_E354 != 4) || (SO(arg0)->kind != 2))) {
                    if (temp_v0_2 == 0) {
                        var_v0_3 = SO(arg0)->unk38;
                        if (var_v0_3 == 0) {
                            temp_f16 = SO(arg0)->speed;
                            D_E354_1C738_a = temp_f16;
                            if (temp_f16 == 0.0f) {
                                temp_f16 = 1.0;
                                D_E354_1C738_b = temp_f16;
                            }
                        } else if (var_v0_3 >= 7) {
                            temp_f16 = D_E354_1C738_c; temp_f10_2 = SO(arg0)->speed / temp_f16; if (1) {}
                            if (temp_f10_2 <= 0.5f) {
                                temp_f10_2 = 0.5f;
                            }
                            temp_f16 = 1.0;
                            if (temp_f16 <= temp_f10_2) {
                                temp_f10_2 = temp_f16;
                            }
                            temp_v0 = *(s32 *)D_E354_GA; /* D_E354_GA: the Bss 0x31C (Sym 1439) gain-wobble countdown */
                            if (temp_v0 != 0) {
#ifdef PORT /* the ROM's lui / addiu pair is relocated against bootup Data; the constant alone is a wild
             * address on the host (soak.py run 864: SIGSEGV here once the gain-wobble countdown runs) */
                                temp_f16 = *(f32 *)(port_bootup_data + ((temp_v0 & 0x1F) * 4) + kD_sndGainWobble_1C430);
#else
                                temp_f16 = *(f32 *)((char *)(((temp_v0 & 0x1F) * 4)) + kD_sndGainWobble_1C430);
#endif
                                temp_f10_2 *= temp_f16;
                                *(s32 *)D_E354_GA = (s32) (temp_v0 - 1);
                            }
                            temp_f16 = *(f32 *)((char *)&D_00000000 + 0x1724);
                            if (temp_f16 <= temp_f10_2) {
                                temp_f10_2 = temp_f16;
                            }
                            D_E354_GB[5] = 0x20;
                            D_E354_1C73C_a = temp_f10_2; if (temp_v0_2) {}
                            gl_E354_setf(0x04010900, temp_f10_2); temp_v0_2 = SO(arg0)->unk14;
                        }
                    } else {
                        temp_v0 = D_E354_GB[5];
                        if (temp_v0 != 0) {
                            temp_f0 = D_E354_1C73C_b;
                            temp_f16 = *(f32 *)((char *)&D_00000000 + 0x1728);
                            temp_f0 = ((1.0f - temp_f0) / (f32) temp_v0) + temp_f0;
                            if (temp_f16 <= temp_f0) {
                                temp_f0 = temp_f16;
                            }
                            temp_f16 = 0.5;
                            if (temp_f0 <= temp_f16) {
                                temp_f0 = temp_f16;
                            }
                            D_E354_GB[5] = (s32) (temp_v0 - 1);
                            D_E354_1C73C_c = temp_f0;
                            gl_E354_setf(0x04010900, temp_f0); temp_v0_2 = SO(arg0)->unk14;
                        } else {
                            gl_E354_setf(0x04010900, 1.0f); temp_v0_2 = SO(arg0)->unk14;
                        }
                    }
                }
                switch ((unsigned)temp_v0_2) {
                case 0:
                    if (D_raceC_E354 == 0) {
                        SO(arg0)->unk38 = (s32) (SO(arg0)->unk38 + 1);
                    } if (1) {}
                    if ((*(volatile u8 *)&SO(arg0)->kind == 0) && (SO(arg0)->unk38 == 0x1E)) {
                        FCALL(snd_TriggerChannel8CueIfIdle)(0x20, 0x12, 0);
                    }
                    if (D_raceD_E354) {}
                    var_v0_3 = D_raceD_E354;
                    if ((var_v0_3 != 0) && (var_v0_3 != 1) && (var_v0_3 != 8)) {
                        temp_v1 = SO(arg0)->unk48;
                        if (temp_v1 != 0) {
                            SO(arg0)->unk33 = 0x1AU;
                        }
                        if (*(f32 *)((char *)&D_00000000 + 0x172C) < SO(arg0)->unk0C) {
                            if (++SO(arg0)->unk33 == 0x19) {
                                FCALL(snd_SoundObjRequestCue)(arg0, 0x3C);
                            }
                        }
                    }
                    SO(arg0)->unk26 = 0U;
                    SO(arg0)->unk04 = 0.0f;
                case 1:
                    if (SO(arg0)->unk04 > 0.5f) {
                        var_t9 = SO(arg0)->unk18;
                        if (SO(arg0)->unk1E == 0) {
                            var_t9 = -var_t9;
                        }
                        FCALL(snd_SetPlayer0ChannelIO12_2FA90)(SO(arg0)->kind, SO(arg0)->unk15, var_t9);
                    } else {
                        FCALL(snd_ResetPlayer0ChannelIO1_2FB10)(SO(arg0)->kind);
                    }
                    gl_E354_emitf(SO(arg0)->kind, sp7D * 0x10, SO(arg0)->unk26, sp78, (s32) SO(arg0)->unk15, (s32) SO(arg0)->unk28);
                    break;
                default:
                    gl_E354_emitf(SO(arg0)->kind, sp7D * 0x10, SO(arg0)->unk26, sp78, temp_v0_2 + 2, (s32) SO(arg0)->unk28);
                    FCALL(snd_ResetPlayer0ChannelIO1_2FB10)(SO(arg0)->kind);
                    break;
                }
                gl_func_000438F4(arg0);
            }
            if ((D_stateE_E354 == 1) && (SO(arg0)->kind == 0)) {
                temp_f0 = SO(arg0)->unk10 + 1.0f;
                gl_func_00044C38((s32) (temp_f0 * 63.5f));
            }
        }
    }
}
#ifdef PORT
#undef D_E354_1C738_a
#undef D_E354_1C738_b
#undef D_E354_1C738_c
#undef D_E354_1C73C_a
#undef D_E354_1C73C_b
#endif
