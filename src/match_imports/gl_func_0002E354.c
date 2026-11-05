/* Native original-compiler source port from src/game_libs/game_libs_post.c (the gl_func_0002E354 NON_MATCHING body,
 * comments dropped; see that file for the per-line lever history). agent-g run 284: the in-tree body compiles to the
 * target's 921 words once linked with the callees / tables at their own addresses (scripts/match-origins-gl_func_0002E354.ld).
 * gl_E354_v: the switch-1 0x90000000 command call through a void prototype (result never read; an int result defines v0
 * inside the 8 / 0x40 pick's live range and uopt forbids it v0). arg1: the switch-1 selector and the o->0x15 holder
 * (the parameter is otherwise unused; each read follows its own write). */
#include "context_game_libs_post.h"
extern int gl_func_000428FC();
extern void gl_E354_emitf(s32, s32, s32, f32, s32, s32);
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
extern s32 gl_E354_setf(s32, f32);
extern f32 D_E354_1C738_a, D_E354_1C738_b, D_E354_1C738_c, D_E354_1C73C_a, D_E354_1C73C_b, D_E354_1C73C_c;
extern char D_E354_G[];
extern void D_E354_GA(void);
extern s32 D_E354_GB[];
extern void gl_E354_v(s32, s32);
void gl_func_0002E354(char *arg0, s32 arg1) {
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
            if ((((u32)temp_v0_7 == 1) || ((u32)temp_v0_7 == 2) || ((u32)temp_v0_7 == 3)) && (*(u8 *)((char *)(arg0) + 0x1C) == 1U) && (*(u8 *)((char *)(arg0) + 0x21) == 0)) {
                *(u8 *)((char *)(arg0) + 0x21) = 2U;
                *(f32 *)((char *)(arg0) + 0x54) = 0.0f;
            }
        }
        if (D_stateB_E354 == 1) {
            *(u8 *)((char *)(arg0) + 0x23) = 0U;
            return;
        }
        if ((D_stateD_E354 != 1) && (*(u8 *)((char *)(arg0) + 0x23) != 0)) {
            if (*(u8 *)((char *)(arg0) + 0x20) == 0) {
                func_00000000(arg0);
                return;
            }
            temp_f18 = *(f32 *)((char *)(arg0) + 0x4) * *(f32 *)((char *)(arg0) + 0x0) * 100.0f;
            temp_v1_32 = *(u8 *)((char *)(arg0) + 0x32);
            *(u8 *)((char *)(arg0) + 0x20) = 0U; if (1) {}
            if (temp_v1_32) {
                sp64 = D_E354_1C330[temp_v1_32];
                temp_f0 = D_E354_1C3B0[temp_v1_32];
                sp60 *= temp_f0;
                *(f32 *)((char *)(arg0) + 0x4) = (f32) (*(f32 *)((char *)(arg0) + 0x4) * temp_f0);
                *(u8 *)((char *)(arg0) + 0x32) = (u8) (temp_v1_32 - 1);
            }
            temp_f0 = *(f32 *)((char *)(arg0) + 0x50) - *(f32 *)((char *)(arg0) + 0x0);
            if (0.5f <= temp_f0) {
                temp_f0 = temp_f0 + 0.5f;
                if (2.0f <= temp_f0) {
                    temp_f0 = 2.0f;
                }
                sp60 *= temp_f0;
            }
            *(f32 *)((char *)(arg0) + 0x50) = (f32) ((*(f32 *)((char *)(arg0) + 0x0) + *(f32 *)((char *)(arg0) + 0x50)) * 0.5f);
            *(f32 *)((char *)(arg0) + 0x44) = (f32) ((*(f32 *)((char *)(arg0) + 0x44) + temp_f18) * 0.5f);
            sp7D = gl_func_00043824(*(f32 *)((char *)(arg0) + 0x4) * 120.0f, (s32)((char *)&D_00000000 + 0x1C250), 0x10);
            temp_v1 = (u8)((gl_func_00043824(*(f32 *)((char *)(arg0) + 0x44), (s32)((char *)&D_00000000 + 0x1C290), 0x20) * 4) & 0xFF);
            sp78 = *(f32 *)((char *)&D_00000000 + 0x16E0) + *(f32 *)((char *)(arg0) + 0x0) / 200.0f;
            temp_v0_2 = temp_v1;
            var_f4 = (f32) (u32) temp_v0_2;
            sp78 = sp78 * sp64;
            temp_f10_2 = var_f4 * sp60;
            var_t9 = (u32) temp_f10_2;
            *(u8 *)((char *)(arg0) + 0x26) = (u8) (((s32) *(u8 *)((char *)(arg0) + 0x26) / 2) + ((s32) (var_t9 & 0xFF) / 2));
            if (*(f32 *)((char *)(arg0) + 0x0) == 0.0f) {
                *(u8 *)((char *)(arg0) + 0x26) = 0U;
            }
            if (*(u8 *)((char *)(arg0) + 0x34) != *(u8 *)((char *)(arg0) + 0x1D)) {
                if (*(u8 *)((char *)(arg0) + 0x1D) == 1) {
                    var_v0 = 0x40;
                } else {
                    var_v0 = 8;
                }
                arg1 = *(u8 *)((char *)(arg0) + 0x21); if (var_v0 != arg1) {}
                switch (arg1) {
                case 0:
                    gl_E354_v(0x90000000, 6 + 0 * (int)(var_v0 + var_v0 + var_v0));
                    func_00000000(0x0500FF00, (s8) var_v0);
                    func_00000000(0x05020000, (s8) var_v0);
                    break;
                case 1:
                case 2:
                    gl_E354_v(0x90000000, 0x60 + 0 * (int)(var_v0 + var_v0 + var_v0));
                    func_00000000(0x0500FF00, (s8) var_v0);
                    func_00000000(0x05020100, (s8) var_v0);
                    break;
                }
                *(u8 *)((char *)(arg0) + 0x34) = *(u8 *)((char *)(arg0) + 0x1D);
            }
            if (*(u8 *)((char *)(arg0) + 0x1C) & 1) {
                temp_f0 = *(f32 *)((char *)(arg0) + 0x54);
                *(u8 *)((char *)(arg0) + 0x26) = (u32) ((f32) (u32) *(u8 *)((char *)(arg0) + 0x26) * temp_f0);
                sp78 *= *(f32 *)((char *)(arg0) + 0x58);
                temp_v0 = *(u8 *)((char *)(arg0) + 0x60);
                *(u8 *)((char *)(arg0) + 0x28) = (u8) ((s32) (temp_v0 + *(u8 *)((char *)(arg0) + 0x28)) >> 1);
                if (*(u8 *)((char *)(arg0) + 0x21) == 0) {
                    gl_E354_setf(0x01020000, temp_f0);
                    func_00000000(0x03020000, (s8) *(u8 *)((char *)(arg0) + 0x28));
                    func_00000000(0x90000000, 0x806);
                    func_00000000(0x0C00FF00, *(s8 *)((char *)(arg0) + 0x63));
                    temp_f10_2 = func_00000000(0x0D00FF00, 0x3000);
                } else {
                    gl_E354_setf(0x01020100, temp_f0);
                    func_00000000(0x03020100, (s8) *(u8 *)((char *)(arg0) + 0x28));
                    func_00000000(0x90000000, 0x1060);
                    func_00000000(0x0C00FF00, *(s8 *)((char *)(arg0) + 0x63));
                    temp_f10_2 = func_00000000(0x0D00FF00, 0x3000);
                }
            } else {
                if (D_stateF_E354 == 4) {
                    switch (*(u8 *)((char *)(arg0) + 0x21)) {
                    case 0:
                        *(u8 *)((char *)(arg0) + 0x28) = 0x10U;
                        break;
                    case 1:
                        *(u8 *)((char *)(arg0) + 0x28) = 0x70U;
                        break;
                    }
                } else {
                    *(u8 *)((char *)(arg0) + 0x28) = 0x40U;
                }
                if (*(u8 *)((char *)(arg0) + 0x21) == 0) {
                    func_00000000(0x01020000, 0x3F800000);
                    temp_f10_2 = func_00000000(0x03020000, (s8) *(u8 *)((char *)(arg0) + 0x28));
                } else {
                    func_00000000(0x01020100, 0x3F800000);
                    temp_f10_2 = func_00000000(0x03020100, (s8) *(u8 *)((char *)(arg0) + 0x28));
                }
            }
            if (*(u8 *)((char *)(arg0) + 0x16) & 1) {
                *(f32 *)((char *)(arg0) + 0x8) = (f32) (*(f32 *)((char *)(arg0) + 0x8) + 50.0f);
                gl_func_00043BE4(arg0, 0, *(u8 *)((char *)(arg0) + 0x2C) - 1, *(f32 *)((char *)(arg0) + 0x8), (s32) *(u8 *)((char *)(arg0) + 0x28));
                temp_f10_2 = func_00000000(arg0, (*(u32 *)((char *)&D_00000000 + 0x2154) & 7) + 0x20);
                *(u8 *)((char *)(arg0) + 0x16) = (u8) (*(u8 *)((char *)(arg0) + 0x16) & ~1);
            }
            var_v0_2 = *(u8 *)((char *)(arg0) + 0x14);
            temp_v0_2 = var_v0_2; if (1) {}
            if (var_v0_2 != *(u8 *)((char *)(arg0) + 0x2C)) {
                if (*(u8 *)((char *)(arg0) + 0x2C) == 0) {
                    temp_v0 = *(s32 *)((char *)(arg0) + 0x38);
                    *(f32 *)((char *)(arg0) + 0x8) = (f32) ((f32) temp_v0 * 3.0f);
                    if (temp_v0 < 0xA) {
                        *(f32 *)((char *)(arg0) + 0x8) = 0.0f;
                    }
                    var_v0_2 = 1;
                    if (*(u8 *)((char *)(arg0) + 0x14) == 1) {
                        arg1 = *(u8 *)((char *)(arg0) + 0x15);
                        switch (arg1) {
                        case 1: var_t9 = 0; break;
                        case 2: var_t9 = 1; break;
                        default: var_t9 = 2; break;
                        }
                    } else {
                        var_t9 = 3;
                    }
                    switch (*(u8 *)((char *)(arg0) + 0x22)) {
                    case 3: case 4:
                        gl_func_00043BE4(arg0, 8, var_t9, *(f32 *)((char *)(arg0) + 0x8), (s32) *(u8 *)((char *)(arg0) + 0x28));
                        break;
                    case 1: case 2: case 5:
                    default:
                        gl_func_00043BE4(arg0, 0xC, var_t9, *(f32 *)((char *)(arg0) + 0x8), (s32) *(u8 *)((char *)(arg0) + 0x28));
                        break;
                    }
                    var_v0_2 = *(u8 *)((char *)(arg0) + 0x14);
                    goto block_2C;
                } else {
                    var_v0_2 = *(u8 *)((char *)(arg0) + 0x14); if (temp_v0_2 == 0) {
                        *(s32 *)((char *)(arg0) + 0x38) = 0;
                        *(u8 *)((char *)(arg0) + 0x33) = 0U;
                        goto block_81;
                    }
                    goto block_2C;
block_81:
                        var_v0_2 = *(u8 *)((char *)(arg0) + 0x14);
block_2C:
                    *(u8 *)((char *)(arg0) + 0x2C) = var_v0_2;
                    goto block_83;
                }
            } else {
block_83:
                if (*(u16 *)((char *)(arg0) + 0x1A) != 0) {
                    if (*(s32 *)((char *)(arg0) + 0x48) == 0) {
                        temp_f16 = *(f32 *)((char *)(arg0) + 0x0);
                        sp6B = 0x11;
                        if (75.0f <= temp_f16) {
                            sp6B = 0x13;
                        } else if (50.0f <= temp_f16) {
                            sp6B = 0x12;
                        }
                        if (25.0f <= temp_f16) {
                            gl_func_00043BE4(arg0, 0x10, sp6B, 127.0f, (s32) *(u8 *)((char *)(arg0) + 0x28));
                        }
                    }
                    *(u8 *)((char *)(arg0) + 0x2A) = sp6B;
                    *(u16 *)((char *)(arg0) + 0x1A) = 0U;
                    *(s32 *)((char *)(arg0) + 0x48) = 5;
                }
                if (*(u8 *)((char *)(arg0) + 0x16) & 4) {
                    sp50 = 4;
                    { int rv = gl_func_000428FC(arg0); var_v0_2 = rv;
                    sp6B = rv;
                    temp_v1_2 = *(u8 *)((char *)(arg0) + 0x29);
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
                    if ((*(u16 *)((char *)(arg0) + 0x30) != 1) || (temp_v1_2 != 3)) {
                        temp_v1_5 = *(u8 *)((char *)(arg0) + 0x22);
                        if ((s32) temp_v1_5 >= 6) {
                            var_t9 = temp_v1_5 + 0xE;
                        }
                        if ((*(s32 *)(D_E354_G + 0)  == 4) || (*(u8 *)((char *)(arg0) + 0x1C) & 1)) {
                            gl_func_00043BE4(arg0, 0x10, var_t9, temp_f10_2, (s32) *(u8 *)((char *)(arg0) + 0x28)); var_v0_2 = 0;
                        } else {
                            var_v0_3 = *(u8 *)((char *)(arg0) + 0x2E);
                            if (var_v0_3 == 1) {
                                var_v0_3 = 0x7F;
                            } else if (var_v0_3 == 3) {
                                var_v0_3 = 0;
                            } else {
                                var_v0_3 = 0x40;
                            }
                            if (*(u8 *)((char *)(arg0) + 0x1E) != 0) {
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
                        temp_v0_7 = *(u8 *)((char *)(arg0) + 0x29);
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
                    func_00000000(arg0, ((u32) *(s32 *)((char *)&D_00000000 + 0x2154) % sp50) + (0, temp_v1));
                    *(s32 *)((char *)(arg0) + 0x48) = 0xA;
                    *(u8 *)((char *)(arg0) + 0x29) = 0U;
                    *(u8 *)((char *)(arg0) + 0x2B) = sp6B;
                    *(u8 *)((char *)(arg0) + 0x16) = (u8) (*(u8 *)((char *)(arg0) + 0x16) & 0xFFFB);
                }
            }
            if (*(s32 *)((char *)(arg0) + 0x48) != 0) {
                if (--*(s32 *)((char *)(arg0) + 0x48) == 0) {
                    *(u8 *)((char *)(arg0) + 0x2D) = 0;
                }
            }
            if (*(u8 *)((char *)(arg0) + 0x23) != 0) {
                temp_v0_2 = *(u8 *)((char *)(arg0) + 0x14);
                if ((D_raceA_E354 == 1) && (((u32)D_raceB_E354 == 4) || ((u32)D_raceB_E354 == 0xA)) && (((u32)D_raceB_E354 != 4) || (*(u8 *)((char *)(arg0) + 0x21) != 2))) {
                    if (temp_v0_2 == 0) {
                        var_v0_3 = *(s32 *)((char *)(arg0) + 0x38);
                        if (var_v0_3 == 0) {
                            temp_f16 = *(f32 *)((char *)(arg0) + 0x0);
                            D_E354_1C738_a = temp_f16;
                            if (temp_f16 == 0.0f) {
                                temp_f16 = 1.0;
                                D_E354_1C738_b = temp_f16;
                            }
                        } else if (var_v0_3 >= 7) {
                            temp_f16 = D_E354_1C738_c; temp_f10_2 = *(f32 *)((char *)(arg0) + 0x0) / temp_f16; if (1) {}
                            if (temp_f10_2 <= 0.5f) {
                                temp_f10_2 = 0.5f;
                            }
                            temp_f16 = 1.0;
                            if (temp_f16 <= temp_f10_2) {
                                temp_f10_2 = temp_f16;
                            }
                            temp_v0 = *(s32 *)((char *)D_E354_GA + 0);
                            if (temp_v0 != 0) {
                                temp_f16 = *(f32 *)((char *)(((temp_v0 & 0x1F) * 4)) + 0x1C430);
                                temp_f10_2 *= temp_f16;
                                *(s32 *)((char *)D_E354_GA + 0) = (s32) (temp_v0 - 1);
                            }
                            temp_f16 = *(f32 *)((char *)&D_00000000 + 0x1724);
                            if (temp_f16 <= temp_f10_2) {
                                temp_f10_2 = temp_f16;
                            }
                            D_E354_GB[5] = 0x20;
                            D_E354_1C73C_a = temp_f10_2; if (temp_v0_2) {}
                            gl_E354_setf(0x04010900, temp_f10_2); temp_v0_2 = *(u8 *)((char *)(arg0) + 0x14);
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
                            gl_E354_setf(0x04010900, temp_f0); temp_v0_2 = *(u8 *)((char *)(arg0) + 0x14);
                        } else {
                            gl_E354_setf(0x04010900, 1.0f); temp_v0_2 = *(u8 *)((char *)(arg0) + 0x14);
                        }
                    }
                }
                switch ((unsigned)temp_v0_2) {
                case 0:
                    if (D_raceC_E354 == 0) {
                        *(s32 *)((char *)(arg0) + 0x38) = (s32) (*(s32 *)((char *)(arg0) + 0x38) + 1);
                    } if (1) {}
                    if ((*(volatile u8 *)((char *)(arg0) + 0x21) == 0) && (*(s32 *)((char *)(arg0) + 0x38) == 0x1E)) {
                        func_00000000(0x20, 0x12, 0);
                    }
                    if (D_raceD_E354) {}
                    var_v0_3 = D_raceD_E354;
                    if ((var_v0_3 != 0) && (var_v0_3 != 1) && (var_v0_3 != 8)) {
                        temp_v1 = *(s32 *)((char *)(arg0) + 0x48);
                        if (temp_v1 != 0) {
                            *(u8 *)((char *)(arg0) + 0x33) = 0x1AU;
                        }
                        if (*(f32 *)((char *)&D_00000000 + 0x172C) < *(f32 *)((char *)(arg0) + 0xC)) {
                            if (++*(u8 *)((char *)(arg0) + 0x33) == 0x19) {
                                func_00000000(arg0, 0x3C);
                            }
                        }
                    }
                    *(u8 *)((char *)(arg0) + 0x26) = 0U;
                    *(f32 *)((char *)(arg0) + 0x4) = 0.0f;
                case 1:
                    if (*(f32 *)((char *)(arg0) + 0x4) > 0.5f) {
                        var_t9 = *(s8 *)((char *)(arg0) + 0x18);
                        if (*(u8 *)((char *)(arg0) + 0x1E) == 0) {
                            var_t9 = -var_t9;
                        }
                        func_00000000(*(u8 *)((char *)(arg0) + 0x21), *(u8 *)((char *)(arg0) + 0x15), var_t9);
                    } else {
                        func_00000000(*(u8 *)((char *)(arg0) + 0x21));
                    }
                    gl_E354_emitf(*(u8 *)((char *)(arg0) + 0x21), sp7D * 0x10, *(u8 *)((char *)(arg0) + 0x26), sp78, (s32) *(u8 *)((char *)(arg0) + 0x15), (s32) *(u8 *)((char *)(arg0) + 0x28));
                    break;
                default:
                    gl_E354_emitf(*(u8 *)((char *)(arg0) + 0x21), sp7D * 0x10, *(u8 *)((char *)(arg0) + 0x26), sp78, temp_v0_2 + 2, (s32) *(u8 *)((char *)(arg0) + 0x28));
                    func_00000000(*(u8 *)((char *)(arg0) + 0x21));
                    break;
                }
                gl_func_000438F4(arg0);
            }
            if ((D_stateE_E354 == 1) && (*(u8 *)((char *)(arg0) + 0x21) == 0)) {
                temp_f0 = *(f32 *)((char *)(arg0) + 0x10) + 1.0f;
                gl_func_00044C38((s32) (temp_f0 * 63.5f));
            }
        }
    }
}
