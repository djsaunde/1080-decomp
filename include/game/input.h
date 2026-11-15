#ifndef GAME_INPUT_H
#define GAME_INPUT_H

/*
 * The input manager: bootup.uso's exported Bss object at Bss 0xF8 (agent-h run 466).
 *
 * Every mode module (titproc, n64proc, h2hproc, mgrproc, arcproc, timproc, trkproc) imports
 * it as one symbol and passes it as arg 0 to the bootup input_* functions
 * (input_checkPressed_00011E00 / input_checkHeld_00011ED4 / input_checkReleased_00011FA8,
 * input_setActivePad_0001207C, input_setActive_00011CD8). Besides the pad list it holds the
 * game-flow words the mode state machines run on, so the mode modules read and write
 * those fields through the same import (relocations against bootup Bss 0xF8 + offset).
 *
 * Only the fields with evidence are named; the rest is padding. The size is not known
 * (the object reaches at least +0x194, the Fader pointer).
 *
 * Mode modules model the import as `extern InputMgr D_00000000;` (or cast the import
 * address), which keeps the relocated symbol and offset unchanged (READABILITY_PLAN 9, rule 4e).
 */

/* one controller as the input manager sees it (the objects in InputMgr.pads[]) */
typedef struct InputPad {
    char pad00[0x10];
    /* 0x10 */ s32 held;      /* button mask tested by input_checkHeld_00011ED4 */
    char pad14[0x18 - 0x14];
    /* 0x18 */ s32 pressed;   /* tested by input_checkPressed_00011E00 */
    /* 0x1C */ s32 released;  /* tested by input_checkReleased_00011FA8 */
} InputPad;

typedef struct InputMgr {
    char pad00[0x34];
    /* 0x34 */ s32 nextFlowMode;   /* index into bootup Data 0xC450 (0 N64DEMO .. 8 EDITORS);
                                    * written by the title menu rows */
    char pad38[0x40 - 0x38];
    /* 0x40 */ s32 procState;      /* state of the running mode's <pfx>_procMain loop */
    /* 0x44 */ s32 nextProcState;  /* state to enter after the current step (select screen, fade) */
    /* 0x48 */ s32 thenProcState;  /* the state after that (arc: 10 / 9, h2h: 6) */
    /* 0x4C */ s32 playerRider;    /* player 1's rider; mgr_pickRivals never picks it */
    char pad50[0x64 - 0x50];
    /* 0x64 */ s32 course;         /* course id of the next race (mode procMain race state) */
    /* 0x68 */ s32 level;          /* difficulty 1..3 (mgr_initMatchRace) */
    /* 0x6C */ s32 demoIndex;      /* which attract demo n64proc / titproc runs next */
    char pad70[0xE0 - 0x70];
    /* 0xE0 */ InputPad *pads[8];  /* input_addPad_00011C70 appends; numPads counts */
    /* 0x100 */ void *replayInputs[8]; /* replay_addInput appends; numReplayInputs counts */
    /* 0x120 */ s32 numPads;
    /* 0x124 */ s32 numReplayInputs;
    /* 0x128 */ s32 activePad;     /* -1 = any pad; input_setActivePad / input_clearActivePad */
    char pad12C[0x18C - 0x12C];
    /* 0x18C */ s32 locked;        /* nonzero: every input_check* returns 0 */
} InputMgr;

typedef char InputPad_held_check[(int)&((InputPad *)0)->held == 0x10 ? 1 : -1];
typedef char InputPad_pressed_check[(int)&((InputPad *)0)->pressed == 0x18 ? 1 : -1];
typedef char InputPad_released_check[(int)&((InputPad *)0)->released == 0x1C ? 1 : -1];
typedef char InputMgr_nextFlowMode_check[(int)&((InputMgr *)0)->nextFlowMode == 0x34 ? 1 : -1];
typedef char InputMgr_procState_check[(int)&((InputMgr *)0)->procState == 0x40 ? 1 : -1];
typedef char InputMgr_thenProcState_check[(int)&((InputMgr *)0)->thenProcState == 0x48 ? 1 : -1];
typedef char InputMgr_course_check[(int)&((InputMgr *)0)->course == 0x64 ? 1 : -1];
typedef char InputMgr_demoIndex_check[(int)&((InputMgr *)0)->demoIndex == 0x6C ? 1 : -1];
typedef char InputMgr_pads_check[(int)&((InputMgr *)0)->pads == 0xE0 ? 1 : -1];
typedef char InputMgr_replayInputs_check[(int)&((InputMgr *)0)->replayInputs == 0x100 ? 1 : -1];
typedef char InputMgr_numPads_check[(int)&((InputMgr *)0)->numPads == 0x120 ? 1 : -1];
typedef char InputMgr_activePad_check[(int)&((InputMgr *)0)->activePad == 0x128 ? 1 : -1];
typedef char InputMgr_locked_check[(int)&((InputMgr *)0)->locked == 0x18C ? 1 : -1];

#endif
