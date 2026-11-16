/* IDO 7.1 -O2 donor for PointsGadget_update_00003430 -- 8-slot state-machine updater
 * (0x4C4 bytes, 305 words; 8-way jumptable over s0->0x94 + a 4-entry
 * fade loop). EXACT 305/305 words (agent-c run 218).
 *
 * WIRED via REPLACE_FUNC_BODY donor splice into game_libs.c.o (the 3C0C /
 * 25A8 recipe): this TU's .rodata holds the 0.1f literal at +0, the 8-entry
 * jumptable at +4 and three more 0.1f literals at +0x24/+0x28/+0x2C; the
 * splice renames that reloc PointsGadget_update_00003430_rodata, pinned to bootup.uso
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
 *    spellings (PASS-3/4). Fields: TrickPointsGadget / PointsPopup (gdl/gadget.h;
 *    agent-h run 524, same bytes as the old FW / FF offset forms). */
#include "common.h"
#include "gdl/gl_data.h"
#include "gdl/gadget.h"
#include "gdl/race.h"
#include "game/input.h"
#include "uso_call.h"


extern int D_00000000;
extern int gl_func_00000000();

void PointsGadget_update_00003430(char *arg0) {
    f32 temp;
    s32 i;
    s32 j;
    char *s0;
    s32 *p;

    TPOINTS(arg0)->allIdle = 1;
    for (i = 0; i != 8; i++) {
        s0 = (char *)TPOINTS(arg0)->slots[i];
        switch (POPUP(s0)->state) {
        case 3:
            POPUP(s0)->velY = POPUP(s0)->velY + POPUP(s0)->accelY;
            temp = POPUP(s0)->velY;
            POPUP(s0)->unk74 = 0;
            POPUP(s0)->timer = POPUP(s0)->timer - 1;
            POPUP(s0)->parms.xps.value = POPUP(s0)->parms.xps.value + POPUP(s0)->velX;
            POPUP(s0)->parms.yps.value = temp + POPUP(s0)->parms.yps.value;
            if (110.0f < POPUP(s0)->parms.yps.value) {
                POPUP(s0)->velY = -temp;
            }
            if (POPUP(s0)->alpha < 0xFF) {
                POPUP(s0)->alpha += 0x20;
                if (POPUP(s0)->alpha >= 0x100) {
                    POPUP(s0)->alpha = 0xFF;
                }
            }
            if (POPUP(s0)->scaleX < 1.0f) {
                POPUP(s0)->scaleX = 0.1f + POPUP(s0)->scaleX;
                POPUP(s0)->scaleY = 0.1f + POPUP(s0)->scaleY;
            } else {
                POPUP(s0)->scaleX = 1.0f;
                POPUP(s0)->scaleY = 1.0f;
            }
            if (POPUP(s0)->timer < 0) {
                POPUP(s0)->state = 4;
                POPUP(s0)->velX = (100.0f - POPUP(s0)->parms.xps.value) / 10.625f;
                POPUP(s0)->velY = (40.0f - POPUP(s0)->parms.yps.value) / 10.625f;
            }
            if (POPUP(s0)->unk7C != 0) {
                TPOINTS(arg0)->allIdle = 0;
            }
            break;
        case 4:
            if (POPUP(s0)->alpha != 0) {
                POPUP(s0)->parms.xps.value = POPUP(s0)->parms.xps.value + POPUP(s0)->velX;
                POPUP(s0)->parms.yps.value = POPUP(s0)->parms.yps.value + POPUP(s0)->velY;
                POPUP(s0)->alpha -= 0x20;
                if (POPUP(s0)->alpha < 0) {
                    POPUP(s0)->alpha = 0;
                    POPUP(s0)->state = 0;
                    p = &GAMEFLOW(&gInputMgr)->dataMgr->entries->score; /* the player's score */
                    *p = POPUP(s0)->value + *p;
                }
            }
            if (POPUP(s0)->unk7C != 0) {
                TPOINTS(arg0)->allIdle = 0;
            }
            break;
        case 1:
            POPUP(s0)->velY = POPUP(s0)->velY + POPUP(s0)->accelY;
            POPUP(s0)->unk74 = 0;
            POPUP(s0)->timer = POPUP(s0)->timer - 1;
            POPUP(s0)->parms.xps.value = POPUP(s0)->parms.xps.value + POPUP(s0)->velX;
            POPUP(s0)->parms.yps.value = POPUP(s0)->parms.yps.value + POPUP(s0)->velY;
            if (POPUP(s0)->alpha < 0xFF) {
                POPUP(s0)->alpha += 0x20;
                if (POPUP(s0)->alpha >= 0x100) {
                    POPUP(s0)->alpha = 0xFF;
                }
            }
            if (POPUP(s0)->timer < 0) {
                POPUP(s0)->state = 2;
                POPUP(s0)->velX = (200.0f - POPUP(s0)->parms.xps.value) / 10.625f;
                POPUP(s0)->velY = (20.0f - POPUP(s0)->parms.yps.value) / 10.625f;
            }
            TPOINTS(arg0)->allIdle = 0;
            break;
        case 2:
            if (POPUP(s0)->alpha != 0) {
                POPUP(s0)->parms.xps.value = POPUP(s0)->parms.xps.value + POPUP(s0)->velX;
                POPUP(s0)->parms.yps.value = POPUP(s0)->parms.yps.value + POPUP(s0)->velY;
                POPUP(s0)->alpha -= 0x18;
                if (POPUP(s0)->alpha < 0) {
                    POPUP(s0)->alpha = 0;
                    POPUP(s0)->state = 0;
                }
            }
            TPOINTS(arg0)->allIdle = 0;
            break;
        case 5:
            POPUP(s0)->timer -= 1;
            if (POPUP(s0)->timer < 0) {
                POPUP(s0)->state = 6;
                POPUP(s0)->unk74 = 0;
            }
            break;
        case 6:
            if (CALL(PointsPopup_slideStep)(s0, 0xB4) != 0) {
                POPUP(s0)->state = 7;
            }
            break;
        case 7:
            if (POPUP(s0)->value != 0) {
                POPUP(s0)->value -= 0x32;
            }
            if (POPUP(s0)->value <= 0) {
                POPUP(s0)->value = 0;
                POPUP(s0)->state = 8;
            }
            CALL(gl_sprintf)(POPUP(s0)->text, GL_DATA(0xCCE8), POPUP(s0)->value);
            POPUP(s0)->unk74 += 0x10;
            if (POPUP(s0)->unk74 >= 0x100) {
                POPUP(s0)->unk74 = 0xFF;
            }
            CALL(PointsPopup_slideStep)(s0, 0x104);
            break;
        case 8:
            POPUP(s0)->unk74 += 0x10;
            if (POPUP(s0)->unk74 >= 0x100) {
                POPUP(s0)->unk74 = 0xFF;
            }
            POPUP(s0)->alpha -= 8;
            if (POPUP(s0)->alpha < 0) {
                POPUP(s0)->alpha = 0;
            }
            if (CALL(PointsPopup_slideStep)(s0, 0x104) != 0) {
                POPUP(s0)->state = 0;
            }
            break;
        }
    }
    for (j = 0; j != 4; j++) {
        if (TPOINTS(arg0)->age[j] < 0x80) {
            TPOINTS(arg0)->age[j] += 1;
            if (TPOINTS(arg0)->age[j] < 0x70) {
                if (TPOINTS(arg0)->alpha[j] < 0xFF) {
                    TPOINTS(arg0)->alpha[j] += 0x10;
                    if (TPOINTS(arg0)->alpha[j] >= 0x100) {
                        TPOINTS(arg0)->alpha[j] = 0xFF;
                    }
                }
            } else {
                if (TPOINTS(arg0)->alpha[j] > 0) {
                    TPOINTS(arg0)->alpha[j] -= 0x10;
                    if (TPOINTS(arg0)->alpha[j] < 0) {
                        TPOINTS(arg0)->alpha[j] = 0;
                        TPOINTS(arg0)->y[j] = 0x64;
                    }
                }
            }
        }
        if (TPOINTS(arg0)->scroll != 0) {
            TPOINTS(arg0)->y[j] = TPOINTS(arg0)->y[j] - 2;
        }
    }
    if (TPOINTS(arg0)->scroll > 0) {
        TPOINTS(arg0)->scroll -= 2;
    }
    if (1) {}
    if (1) {}
    if (1) {}
}
