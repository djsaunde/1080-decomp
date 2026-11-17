#ifndef GAME_TRNPROC_H
#define GAME_TRNPROC_H

/* trnproc.uso (module 27): the training mode (agent-b run 797; docs/1080_MODES.md "trnproc").
 * "TRAINRace Process" is a RaceProcess subclass built like TimRace (TrnRace_ctor inlines
 * Node -> Process -> RaceProcess, vtable Data+0x1B8), and its "DataManager" (vtable Data+0x138)
 * is a RaceTracker subclass that runs the trick lessons. Field names come from the matched bodies
 * in src/trnproc_uso/trnproc_uso.c; the NCC donor trnproc_uso_ncc.cpp keeps its own C++ mirror
 * of the same layout (struct TrnRace / TrnDataManager there). */

#include "../gdl/node.h" /* relative: NCC donors include this header without -I include */
#include "../gdl/race.h"

/* ---- TrnRace (alloc 0x6BC) ---- */
typedef struct TrnRace {
    /* 0x000 */ RaceProcess race;
    /* 0x6A8 */ struct TrnDataManager *dataManager; /* TrnRace_ctor; also InputMgr.dataMgr */
    /* 0x6AC */ f32 unk6AC;             /* 0.0f in the ctor; not read in this module */
    /* 0x6B0 */ u8 trickDone[12];       /* lesson done bits (TrnDataManager_setTrickDone / isTrickDone,
                                         * bit idx & 7 of byte idx >> 3); settings_get8 / settings_set8
                                         * load and save them */
} TrnRace;

typedef char TrnRace_size_check[sizeof(TrnRace) == 0x6BC ? 1 : -1];

/* ---- TrnDataManager (alloc 0x19C; RaceTracker base 0xB8) ---- */
typedef struct TrnDataManager {
    /* 0x000 */ RaceTracker base;       /* hudFlags (0xB4) = 17 from the ctor; pointsListArg (0x54) =
                                         * spriteSet with a pause menu */
    /* 0x0B8 */ TrnRace *race;          /* the owning TrnRace (init arg 1) */
    /* 0x0BC */ f32 colour[4];          /* 1, 1, 1, 1 with a pause menu (stored a, b, g, r) */
    /* 0x0CC */ s32 alpha;              /* 0xFF */
    /* 0x0D0 */ s32 pulse;              /* the current results row's blinking alpha */
    /* 0x0D4 */ s32 frame;              /* lesson HUD frame counter */
    /* 0x0D8 */ s32 hudX;               /* lesson HUD layout: 24, 8, -1, 40 */
    /* 0x0DC */ s32 hudY;
    /* 0x0E0 */ s32 hudDX;
    /* 0x0E4 */ s32 hudDY;
    /* 0x0E8 */ s32 panelX;             /* the lesson panel sprite (Bss+0x150) at (270, 215) */
    /* 0x0EC */ s32 panelY;
    /* 0x0F0 */ s32 lesson;             /* the current lesson (TrickTable index) */
    /* 0x0F4 */ s32 armed;              /* set at a checkpoint; the lesson HUD shows while set */
    /* 0x0F8 */ s32 course;             /* 1 = gate +0x88 < 21 (the restricted course), -1 after reset */
    /* 0x0FC */ s32 results;            /* results screen: 0 off, 1 browsing, 2 picked */
    /* 0x100 */ s32 cursor;             /* results cursor */
    /* 0x104 */ s32 top;                /* first of the 12 shown rows */
    /* 0x108 */ s32 count;              /* lesson count (TrickTable_getCount) */
    /* 0x10C */ s32 repeatTimer;        /* scrollResults key repeat: 8 on the first press, then 2 */
    /* 0x110 */ s32 firstPress;         /* 1 while no key is held */
    /* 0x114 */ s32 trickDone;          /* onTrickStart saw the lesson's trick; onTrickEnd passes it */
    /* 0x118 */ s32 inputs[32];         /* buildLessonInputs: the lesson's input list, -1 terminated */
    /* 0x198 */ s32 button;             /* the button that picked the results row */
} TrnDataManager;

typedef char TrnDataManager_size_check[sizeof(TrnDataManager) == 0x19C ? 1 : -1];
#ifndef __cplusplus /* edg (NCC donors): &((T *)0)->f is not an integral constant expression */
typedef char TrnDataManager_lesson_check[(int)&((TrnDataManager *)0)->lesson == 0xF0 ? 1 : -1];
typedef char TrnDataManager_inputs_check[(int)&((TrnDataManager *)0)->inputs == 0x118 ? 1 : -1];
#endif

/* Field access through the untyped `char *` / `int *` parameters (the matched spellings). */
#define TRNRACE(p) ((TrnRace *)(p))
#define TRNDM(p)   ((TrnDataManager *)(p))

#endif
