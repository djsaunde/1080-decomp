/* IDO 7.1 -O2 donor for gl_func_00003430 -- 8-slot state-machine updater
 * (0x4C4 bytes, 305 words; 8-way jumptable over s0->0x94 + a 4-entry
 * fade loop). EXACT 305/305 words (agent-c run 218).
 *
 * WIRED via REPLACE_FUNC_BODY donor splice into game_libs.c.o (the 3C0C /
 * 25A8 recipe): this TU's .rodata holds the 0.1f literal at +0, the 8-entry
 * jumptable at +4 and three more 0.1f literals at +0x24/+0x28/+0x2C; the
 * splice renames that reloc gl_func_00003430_rodata, pinned to bootup.uso
 * RoData +0xCC8 (the target's words at 0xCC8 / 0xCCC / 0xCEC / 0xCF0 / 0xCF4).
 *
 * Load-bearing shapes:
 *  - the step is the LITERAL 0.1f (no variable): uopt allocates it to
 *    caller-saved f2 and rematerialises it after the calls in cases 6/7/8;
 *    as1 emits a fresh literal per load (the four rodata words).
 *  - three trailing `if (1) {}` raise the basic-block count so a fresh
 *    callee-saved register (f24) costs more than the post-call reloads
 *    (globalcolor firstUseCost); with 0-2 the literal is held in f24.
 *  - see the comment above the game_libs.c placeholder for the field-level
 *    spellings (PASS-3/4). */
#include "common.h"

#define FW(p, o) (*(int *)((char *)(p) + (o)))
#define FF(p, o) (*(f32 *)((char *)(p) + (o)))

extern int D_00000000;
extern int gl_func_00000000();

void gl_func_00003430(char *arg0) {
    f32 temp;
    s32 i;
    s32 j;
    char *s0;
    s32 *p;

    FW(arg0, 0xEC) = 1;
    for (i = 0; i != 8; i++) {
        s0 = ((char **)(arg0 + 0x2C))[i];
        switch (FW(s0, 0x94)) {
        case 3:
            FF(s0, 0xA8) = FF(s0, 0xAC) + FF(s0, 0xA8);
            temp = FF(s0, 0xA8);
            FW(s0, 0x74) = 0;
            FW(s0, 0x9C) = FW(s0, 0x9C) - 1;
            FF(s0, 0x44) = FF(s0, 0xA4) + FF(s0, 0x44);
            FF(s0, 0x5C) = temp + FF(s0, 0x5C);
            if (110.0f < FF(s0, 0x5C)) {
                FF(s0, 0xA8) = -temp;
            }
            if (FW(s0, 0x78) < 0xFF) {
                FW(s0, 0x78) += 0x20;
                if (FW(s0, 0x78) >= 0x100) {
                    FW(s0, 0x78) = 0xFF;
                }
            }
            if (FF(s0, 0x88) < 1.0f) {
                FF(s0, 0x88) = 0.1f + FF(s0, 0x88);
                FF(s0, 0x8C) = 0.1f + FF(s0, 0x8C);
            } else {
                FF(s0, 0x88) = 1.0f;
                FF(s0, 0x8C) = 1.0f;
            }
            if (FW(s0, 0x9C) < 0) {
                FW(s0, 0x94) = 4;
                FF(s0, 0xA4) = (100.0f - FF(s0, 0x44)) / 10.625f;
                FF(s0, 0xA8) = (40.0f - FF(s0, 0x5C)) / 10.625f;
            }
            if (FW(s0, 0x7C) != 0) {
                FW(arg0, 0xEC) = 0;
            }
            break;
        case 4:
            if (FW(s0, 0x78) != 0) {
                FF(s0, 0x44) = FF(s0, 0xA4) + FF(s0, 0x44);
                FF(s0, 0x5C) = FF(s0, 0xA8) + FF(s0, 0x5C);
                FW(s0, 0x78) -= 0x20;
                if (FW(s0, 0x78) < 0) {
                    FW(s0, 0x78) = 0;
                    FW(s0, 0x94) = 0;
                    p = (s32 *)(FW(*(char **)((char *)&D_00000000 + 0x138), 0x44) + 0x18);
                    *p = FW(s0, 0x98) + *p;
                }
            }
            if (FW(s0, 0x7C) != 0) {
                FW(arg0, 0xEC) = 0;
            }
            break;
        case 1:
            FF(s0, 0xA8) = FF(s0, 0xAC) + FF(s0, 0xA8);
            FW(s0, 0x74) = 0;
            FW(s0, 0x9C) = FW(s0, 0x9C) - 1;
            FF(s0, 0x44) = FF(s0, 0xA4) + FF(s0, 0x44);
            FF(s0, 0x5C) = FF(s0, 0xA8) + FF(s0, 0x5C);
            if (FW(s0, 0x78) < 0xFF) {
                FW(s0, 0x78) += 0x20;
                if (FW(s0, 0x78) >= 0x100) {
                    FW(s0, 0x78) = 0xFF;
                }
            }
            if (FW(s0, 0x9C) < 0) {
                FW(s0, 0x94) = 2;
                FF(s0, 0xA4) = (200.0f - FF(s0, 0x44)) / 10.625f;
                FF(s0, 0xA8) = (20.0f - FF(s0, 0x5C)) / 10.625f;
            }
            FW(arg0, 0xEC) = 0;
            break;
        case 2:
            if (FW(s0, 0x78) != 0) {
                FF(s0, 0x44) = FF(s0, 0xA4) + FF(s0, 0x44);
                FF(s0, 0x5C) = FF(s0, 0xA8) + FF(s0, 0x5C);
                FW(s0, 0x78) -= 0x18;
                if (FW(s0, 0x78) < 0) {
                    FW(s0, 0x78) = 0;
                    FW(s0, 0x94) = 0;
                }
            }
            FW(arg0, 0xEC) = 0;
            break;
        case 5:
            FW(s0, 0x9C) -= 1;
            if (FW(s0, 0x9C) < 0) {
                FW(s0, 0x94) = 6;
                FW(s0, 0x74) = 0;
            }
            break;
        case 6:
            if (gl_func_00000000(s0, 0xB4) != 0) {
                FW(s0, 0x94) = 7;
            }
            break;
        case 7:
            if (FW(s0, 0x98) != 0) {
                FW(s0, 0x98) -= 0x32;
            }
            if (FW(s0, 0x98) <= 0) {
                FW(s0, 0x98) = 0;
                FW(s0, 0x94) = 8;
            }
            gl_func_00000000(s0 + 0xB4, (char *)&D_00000000 + 0xCCE8, FW(s0, 0x98));
            FW(s0, 0x74) += 0x10;
            if (FW(s0, 0x74) >= 0x100) {
                FW(s0, 0x74) = 0xFF;
            }
            gl_func_00000000(s0, 0x104);
            break;
        case 8:
            FW(s0, 0x74) += 0x10;
            if (FW(s0, 0x74) >= 0x100) {
                FW(s0, 0x74) = 0xFF;
            }
            FW(s0, 0x78) -= 8;
            if (FW(s0, 0x78) < 0) {
                FW(s0, 0x78) = 0;
            }
            if (gl_func_00000000(s0, 0x104) != 0) {
                FW(s0, 0x94) = 0;
            }
            break;
        }
    }
    for (j = 0; j != 4; j++) {
        if (((s32 *)(arg0 + 0x8C))[j] < 0x80) {
            ((s32 *)(arg0 + 0x8C))[j] += 1;
            if (((s32 *)(arg0 + 0x8C))[j] < 0x70) {
                if (((s32 *)(arg0 + 0x9C))[j] < 0xFF) {
                    ((s32 *)(arg0 + 0x9C))[j] += 0x10;
                    if (((s32 *)(arg0 + 0x9C))[j] >= 0x100) {
                        ((s32 *)(arg0 + 0x9C))[j] = 0xFF;
                    }
                }
            } else {
                if (((s32 *)(arg0 + 0x9C))[j] > 0) {
                    ((s32 *)(arg0 + 0x9C))[j] -= 0x10;
                    if (((s32 *)(arg0 + 0x9C))[j] < 0) {
                        ((s32 *)(arg0 + 0x9C))[j] = 0;
                        ((s32 *)(arg0 + 0xBC))[j] = 0x64;
                    }
                }
            }
        }
        if (FW(arg0, 0x88) != 0) {
            ((s32 *)(arg0 + 0xBC))[j] = ((s32 *)(arg0 + 0xBC))[j] - 2;
        }
    }
    if (FW(arg0, 0x88) > 0) {
        FW(arg0, 0x88) -= 2;
    }
    if (1) {}
    if (1) {}
    if (1) {}
}
