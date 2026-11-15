#ifndef GAME_MODEPROC_H
#define GAME_MODEPROC_H

/* The mode-module race processes (arcproc / mgrproc / h2hproc) and titproc's demo
 * processes (agent-h run 473; docs/1080_MODES.md H2). Every race process embeds the
 * libgdl RaceProcess (include/gdl/race.h, 0x6A8 bytes); the sizes are the outermost
 * alloc of each ctor's inlined Node -> Process -> RaceProcess chain. timproc / trkproc
 * share TimRace (race.h, agent-c).
 *
 * Only offsets that a matched body touches are named. Pointer-valued fields that the
 * bodies use as ints stay s32 (READABILITY_PLAN 9.1). */

#include "gdl/race.h"

/* ---- the course list (InputMgr.session +8; RaceProcess ctor arg 3) ----
 * arc_initCourses fills it with {2, 0, 3, 1, 4}; mgr_initMatchRace with 4..6
 * course ids and one rider per course. *_loadSprites show "course idx+1 of count"
 * (idx + 1 == count picks the "final" label). Typed as a pointer in ArcRace /
 * ArcDataManager; mgr keeps s32 (its list has riders at +0x24 instead of times, and
 * mgr onRaceOver's `list[list[1] + 4]` int-array form is load-bearing). */
typedef struct ArcCourseList {
    /* 0x00 */ s32 retries;
    /* 0x04 */ s32 idx;              /* current course */
    /* 0x08 */ s32 count;
    /* 0x0C */ s32 courses[5];
    /* 0x20 */ s32 times[5];         /* per-course time (onLeave records into +0x20) */
    /* 0x34 */ s32 total;            /* DataManager_init / reset: the running total */
} ArcCourseList;

typedef char ArcCourseList_size_check[sizeof(ArcCourseList) == 0x38 ? 1 : -1];

/* ---- ArcDataManager (arcproc, alloc 0xD8; arc_DataManager_init) ---- */
typedef struct ArcDataManager {
    /* 0x00 */ DataManager base;
    /* 0xB8 */ ArcCourseList *courseList; /* init arg 3 */
    /* 0xBC */ s32 race;             /* the owning ArcRace (init arg 1) */
    /* 0xC0 */ f32 colour[4];        /* 192/255, 1, 192/255, 0 */
    /* 0xD0 */ s32 alpha;            /* 0xFF */
    /* 0xD4 */ s32 unkD4;
} ArcDataManager;

typedef char ArcDataManager_size_check[sizeof(ArcDataManager) == 0xD8 ? 1 : -1];

/* ---- ArcRace (ARCRaceProcess, "ARCRace Process", alloc 0x780) ---- */
typedef struct ArcRace {
    /* 0x000 */ RaceProcess race;
    /* 0x6A8 */ ArcCourseList *courseList; /* ctor arg 3 */
    /* 0x6AC */ s32 dataMgr;         /* ArcDataManager * (also InputMgr.dataMgr) */
    /* 0x6B0 */ s32 finalTime;       /* onRaceOver: dataMgr->entries[0].time */
    /* 0x6B4 */ s32 recordRank;      /* onRaceOver: entries[0] +0x40, then the tie scan */
    /* 0x6B8 */ s32 retry;           /* onLeave: the time was not accepted; onEndTimerDone
                                      * then goes to state 4 (restart the course) */
    /* 0x6BC */ RaceSprite sprites[8]; /* arc_loadSprites: 0x6BC 0x6D4 0x6EC(course n of m)
                                      * 0x704 0x71C 0x734 0x74C 0x764 */
    /* 0x77C */ f32 resultFade;      /* drawIntro / drawResultScore: alpha 0..1 */
} ArcRace;

typedef char ArcRace_size_check[sizeof(ArcRace) == 0x780 ? 1 : -1];
typedef char ArcRace_retry_check[(int)&((ArcRace *)0)->retry == 0x6B8 ? 1 : -1];
typedef char ArcRace_resultFade_check[(int)&((ArcRace *)0)->resultFade == 0x77C ? 1 : -1];

/* ---- MgrDataManager (mgrproc, alloc 0xE0; mgr_DataManager_init) ---- */
typedef struct MgrDataManager {
    /* 0x00 */ DataManager base;
    /* 0xB8 */ s32 courseList;       /* init arg 3 */
    /* 0xBC */ s32 race;             /* the owning MgrRace (init arg 1) */
    /* 0xC0 */ s32 rankLabel;        /* built from the session record's bytes 5-7 */
    /* 0xC4 */ f32 colour[4];
    /* 0xD4 */ s32 alpha;            /* 0xFF */
    /* 0xD8 */ s32 unkD8;
    /* 0xDC */ s32 beepTimer;        /* update: 0..0x32, sound 0x135 when it wraps */
} MgrDataManager;

typedef char MgrDataManager_size_check[sizeof(MgrDataManager) == 0xE0 ? 1 : -1];

/* ---- MgrRace (MGRRaceProcess, alloc 0x7F0) ---- */
typedef struct MgrRace {
    /* 0x000 */ RaceProcess race;
    /* 0x6A8 */ s32 courseList;      /* course list (ctor arg 3): +0 retries, +4 idx, +8 count,
                                      * +0xC courses, +0x24 riders */
    /* 0x6AC */ s32 dataMgr;         /* MgrDataManager * (also InputMgr.dataMgr) */
    /* 0x6B0 */ RaceSprite sprites[10]; /* mgr_loadSprites: 0x6B0 0x6C8 0x6E0(course n of m)
                                      * 0x6F8 (0x710 unused) 0x728 0x740 0x758 0x770 0x788 */
    /* 0x7A0 */ f32 unk7A0;
    /* 0x7A4 */ f32 unk7A4;
    /* 0x7A8 */ f32 retriesX;        /* 160 - (retries * 2 - 2) */
    /* 0x7AC */ f32 retriesY;        /* 210 */
    /* 0x7B0 */ char unk7B0[8];
    /* 0x7B8 */ f32 unk7B8;
    /* 0x7BC */ char unk7BC[8];
    /* 0x7C4 */ s32 unk7C4;
    /* 0x7C8 */ s32 retries;         /* courseList->retries at ctor; 0 = cart check failed */
    /* 0x7CC */ s32 unk7CC;
    /* 0x7D0 */ s32 lastResult;      /* endTimerEnabled: race.result */
    /* 0x7D4 */ s32 exitStep;        /* updateSub2 switch; 1 = leave the race view */
    /* 0x7D8 */ s32 nameEntryMode;   /* MgrNameEntry_ctor arg 3 */
    /* 0x7DC */ s32 creditLine;      /* ending: credits row, +1 per 16 scroll steps */
    /* 0x7E0 */ s32 creditScroll;    /* ending: 0..15 */
    /* 0x7E4 */ s32 endingPhase;     /* ending flyover phase 0..4 (Data 0x5F0 per course) */
    /* 0x7E8 */ s32 endingTimer;     /* frames left in endingPhase (Data 0x5FC per course) */
    /* 0x7EC */ s32 endingFrame;
} MgrRace;

typedef char MgrRace_size_check[sizeof(MgrRace) == 0x7F0 ? 1 : -1];
typedef char MgrRace_unk7A0_check[(int)&((MgrRace *)0)->unk7A0 == 0x7A0 ? 1 : -1];
typedef char MgrRace_retries_check[(int)&((MgrRace *)0)->retries == 0x7C8 ? 1 : -1];
typedef char MgrRace_endingFrame_check[(int)&((MgrRace *)0)->endingFrame == 0x7EC ? 1 : -1];

/* ---- H2HRace (H2HRaceProcess, alloc 0x6BC) ---- */
typedef struct H2HRace {
    /* 0x000 */ RaceProcess race;
    /* 0x6A8 */ s32 wins;            /* s32[2] per-player win tally (ctor arg 3; the
                                      * WinsDisplay reads [0] / [1]) */
    /* 0x6AC */ s32 postMenu;        /* onRaceOver: the post-race menu (onTimeout waits for its
                                      * +0x34 ->0x3C alpha to reach 0xFF) */
    /* 0x6B0 */ s32 dataMgr;         /* H2HDataManager * (also InputMgr.dataMgr) */
    /* 0x6B4 */ s32 pausePlayer;     /* onPause: the player who pressed START */
    /* 0x6B8 */ s32 winner;          /* h2h_winByFinish / winByRetire; -1 = draw */
} H2HRace;

typedef char H2HRace_size_check[sizeof(H2HRace) == 0x6BC ? 1 : -1];
typedef char H2HRace_winner_check[(int)&((H2HRace *)0)->winner == 0x6B8 ? 1 : -1];

/* ---- H2HDataManager (h2hproc, alloc 0xF0; h2h_DataManager_init) ---- */
typedef struct H2HDataManager {
    /* 0x00 */ DataManager base;
    /* 0xB8 */ s32 race;             /* the owning H2HRace (init arg 1) */
    /* 0xBC */ f32 colour[4];        /* 1, 1, 1, 1 */
    /* 0xCC */ s32 alpha;            /* 0xFF */
    /* 0xD0 */ s32 unkD0;
    /* 0xD4 */ s32 unkD4;
    /* 0xD8 */ char unkD8[0x10];
    /* 0xE8 */ s32 gadgetE8;         /* the first of the gadgets init builds (gl ctor (0, 90)) */
    /* 0xEC */ s32 unkEC;
} H2HDataManager;

typedef char H2HDataManager_size_check[sizeof(H2HDataManager) == 0xF0 ? 1 : -1];

/* ---- GameDemoProcess (titproc, alloc 0x6C8) ----
 * The attract-mode race: a RaceProcess with the title overlays on top. */
typedef struct GameDemoProcess {
    /* 0x000 */ RaceProcess race;
    /* 0x6A8 */ s32 unk6A8;
    /* 0x6AC */ s32 dataMgr;         /* a plain DataManager (the base ctor, import 0xB6C40, with 0) */
    /* 0x6B0 */ s32 keyMessage;      /* KeyMessage_ctor; only built when flags bit 25 is clear */
    /* 0x6B4 */ s32 mainLogo;        /* MainLogo_ctor; 0 when flags bit 25 is set */
    /* 0x6B8 */ s32 langMenu;        /* LangMenu_ctor; only built with the menu (flags bit 23) */
    /* 0x6BC */ s32 whiteFader;      /* tit_WhiteFader_ctor; +0x3C alpha */
    /* 0x6C0 */ s32 menu;            /* the title menu (flags bit 23), 0 otherwise */
    /* 0x6C4 */ s32 menuState;       /* onQuitButton switch: 0 logo, 1 menu, 2 language */
} GameDemoProcess;

typedef char GameDemoProcess_size_check[sizeof(GameDemoProcess) == 0x6C8 ? 1 : -1];
typedef char GameDemoProcess_menu_check[(int)&((GameDemoProcess *)0)->menu == 0x6C0 ? 1 : -1];

/* ---- TitleDemoProcess (titproc, alloc 0x78) ----
 * A plain Process (0x50 head, no RaceProcess) with the same overlays as GameDemoProcess. */
typedef struct TitleDemoProcess {
    /* 0x00 */ char process[0x50];   /* Node / Process head (vptr 0x28) */
    /* 0x50 */ s32 dataMgr;          /* plain DataManager (also InputMgr.dataMgr) */
    /* 0x54 */ s32 keyMessage;       /* KeyMessage_ctor */
    /* 0x58 */ s32 mainLogo;         /* MainLogo_ctor */
    /* 0x5C */ s32 menu;             /* the title menu */
    /* 0x60 */ s32 langMenu;         /* LangMenu_ctor */
    /* 0x64 */ s32 whiteFader;       /* tit_WhiteFader_ctor */
    /* 0x68 */ s32 unk68;
    /* 0x6C */ s32 unk6C;            /* 5 (seconds; timer = x * 30) */
    /* 0x70 */ s32 unk70;            /* 0x48 (seconds; timer = x * 30) */
    /* 0x74 */ s32 unk74;            /* 15000 */
} TitleDemoProcess;

typedef char TitleDemoProcess_size_check[sizeof(TitleDemoProcess) == 0x78 ? 1 : -1];

#endif
