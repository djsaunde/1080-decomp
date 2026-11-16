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

#include "../gdl/node.h" /* relative: NCC donors include this header without -I include */
#include "../gdl/race.h"

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

/* ---- MgrCourseList (mgrproc, mem_new(0x3C) in mgr_openSession_00000504) ----
 * mgr_initMatchRace_00000700(flow, level) fills it: 3 retries, level 1 / 2 / 3 = 4 / 5 / 6 courses
 * (ids 2..7; only 3 courses when the cart check word at 0xA0000284 differs), one rider per course:
 * riders[0..3] the CPU rivals from mgr_pickRivals_000005D0, riders[4] the player's own rider
 * (GameFlow.playerRider), riders[5] the boss 5 / 6 / 7 picked by options->seqStep. mgr_procMain's
 * race state reads courses[idx] into GameFlow.course and riders[idx] into GameFlow +0x17D (run 552). */
typedef struct MgrCourseList {
    /* 0x00 */ s32 retries;
    /* 0x04 */ s32 idx;              /* current course */
    /* 0x08 */ s32 count;
    /* 0x0C */ s32 courses[6];
    /* 0x24 */ s32 riders[6];        /* the opponent of each course */
} MgrCourseList;

typedef char MgrCourseList_size_check[sizeof(MgrCourseList) == 0x3C ? 1 : -1];

/* ---- ModeFlowProc (GameFlow.flowProc: the per-mode entry object GameFlow_startMode builds) ----
 * mem_new(8) or (0xC) for flow modes 2 / 3 / 5; the inlined ctor cascade stores the base vtable
 * (bootup Data 0xC550) and then the mode module's vtable at +4. GameFlow_startMode calls the vtable
 * entry at +8 (`<pfx>_procMain(this + delta, state)`). The arc / mgr procMains keep the record
 * session at +0 (<pfx>_openSession / closeSession) and the course list at +8 (run 552). */
typedef struct ModeFlowProc {
    /* 0x0 */ struct BackupSession *session;
    /* 0x4 */ void *vptr;            /* Cfront vtable: VT_SLOT(vptr, 0) = <pfx>_procMain */
    /* 0x8 */ MgrCourseList *courseList; /* 0xC objects only (ArcCourseList for arcproc) */
} ModeFlowProc;

#define FLOWPROC(p) ((ModeFlowProc *)(p))

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
#ifndef __cplusplus /* edg (NCC donors): &((T *)0)->f is not an integral constant expression */
typedef char ArcRace_retry_check[(int)&((ArcRace *)0)->retry == 0x6B8 ? 1 : -1];
typedef char ArcRace_resultFade_check[(int)&((ArcRace *)0)->resultFade == 0x77C ? 1 : -1];
#endif

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
    /* 0x7A0 */ f32 overlayAlpha;     /* 0..1: drawIntro / the 23FC draws scale it to 0..255;
                                      * rises by Data 0x5D0 to Data 0x5CC while timer >= 11, falls after */
    /* 0x7A4 */ f32 fade7A4;          /* 0..1: rises with overlayAlpha in drawIntro; drawHud decays
                                      * it in phase 3 when the result is 0 / 2; startEnding zeroes it */
    /* 0x7A8 */ f32 retriesX;        /* 160 - (retries * 2 - 2) */
    /* 0x7AC */ f32 retriesY;        /* 210 */
    /* 0x7B0 */ f32 lostX;            /* the retry icon that falls away when a retry is used:
                                      * onMessageBounce snapshots retriesX / Y here */
    /* 0x7B4 */ f32 lostY;            /* drawHud: += lostVelY, stops at 256 */
    /* 0x7B8 */ f32 retriesVel;       /* drawHud: velocity of retriesX (bounces at 36, -v / 4) */
    /* 0x7BC */ f32 lostVelX;         /* retriesVel / 2 */
    /* 0x7C0 */ f32 lostVelY;         /* 1.0, + gravity per frame */
    /* 0x7C4 */ s32 lostFalling;      /* onMessageBounce latch: set once, drawHud draws the icon */
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
#ifndef __cplusplus /* edg (NCC donors): &((T *)0)->f is not an integral constant expression */
typedef char MgrRace_overlayAlpha_check[(int)&((MgrRace *)0)->overlayAlpha == 0x7A0 ? 1 : -1];
typedef char MgrRace_retriesVel_check[(int)&((MgrRace *)0)->retriesVel == 0x7B8 ? 1 : -1];
typedef char MgrRace_retries_check[(int)&((MgrRace *)0)->retries == 0x7C8 ? 1 : -1];
typedef char MgrRace_endingFrame_check[(int)&((MgrRace *)0)->endingFrame == 0x7EC ? 1 : -1];
#endif

/* ---- H2HRace (H2HRaceProcess, alloc 0x6BC) ---- */
typedef struct H2HRace {
    /* 0x000 */ RaceProcess race;
    /* 0x6A8 */ s32 wins;            /* -> s32[2] per-player win tally (ctor arg 3; the
                                      * WinsDisplay reads ((int *)wins)[0] / [1]) */
    /* 0x6AC */ s32 postMenu;        /* onRaceOver: the post-race menu (onTimeout waits for its
                                      * +0x34 ->0x3C alpha to reach 0xFF) */
    /* 0x6B0 */ s32 dataMgr;         /* H2HDataManager * (also InputMgr.dataMgr) */
    /* 0x6B4 */ s32 pausePlayer;     /* onPause: the player who pressed START */
    /* 0x6B8 */ s32 winner;          /* h2h_winByFinish / winByRetire; -1 = draw */
} H2HRace;

typedef char H2HRace_size_check[sizeof(H2HRace) == 0x6BC ? 1 : -1];
#ifndef __cplusplus /* edg (NCC donors): &((T *)0)->f is not an integral constant expression */
typedef char H2HRace_winner_check[(int)&((H2HRace *)0)->winner == 0x6B8 ? 1 : -1];
#endif

/* ---- H2HDataManager (h2hproc, alloc 0xF0; h2h_DataManager_init) ---- */
typedef struct H2HDataManager {
    /* 0x00 */ DataManager base;
    /* 0xB8 */ s32 race;             /* the owning H2HRace (init arg 1) */
    /* 0xBC */ f32 colour[4];        /* 1, 1, 1, 1 */
    /* 0xCC */ s32 alpha;            /* 0xFF */
    /* 0xD0 */ s32 unkD0;
    /* 0xD4 */ s32 beepTimer;        /* update: 0..50; sound 309 (0x135) when it wraps while a
                                      * player's cool gadget +0xC4 >= 12 (as tim / mgr) */
    /* 0xD8 */ char unkD8[0x10];
    /* 0xE8 */ s32 health1;          /* CoolGadget (ctor (0, 90), at 68, 21): entries[0].health; update beeps
                                      * while its seg >= 12 and player 1 retires when its cur reaches 0 */
    /* 0xEC */ s32 health2;          /* the same for entries[1] (at 68, 130) */
} H2HDataManager;

typedef char H2HDataManager_size_check[sizeof(H2HDataManager) == 0xF0 ? 1 : -1];

/* ---- TrkDataManager (trkproc, timproc_uso_b3: TrkDataManager_init_000019CC / _loadData / _update_00001C68) ----
 * Built by the trick-mode race process at RaceProcess + 0x6A8 (TimRace.dataManager). init writes the same
 * 0xB8..0xD7 tail as TimDataManager (gdl/race.h); 0xD8 differs (agent-h run 529). */
typedef struct TrkDataManager {
    /* 0x00 */ DataManager base;
    /* 0xB8 */ char *race;           /* the owning race process (init arg 1; also base.owner): update reads its
                                      * RaceProcess flags / raceState / result and calls timeUp / finishCourse on it */
    /* 0xBC */ s32 rankLabel;        /* BestScoreGadget with the course record score */
    /* 0xC0 */ f32 colour[4];        /* 1, 1, 1, 1 */
    /* 0xD0 */ s32 alpha;            /* 0xFF */
    /* 0xD4 */ s32 unkD4;
    /* 0xD8 */ s32 ghostReplayMode;  /* loadData: 1 (PAD_REPLAY_PLAY); with 2 entries update mirrors entries[1]'s pad
                                      * replayMode into it and a 1 -> 0 edge (the ghost replay ran out) sends
                                      * RaceTracker_sendEntryLoseResult(this, 1) */
    /* 0xDC */ s32 unkDC;
} TrkDataManager;

typedef char TrkDataManager_size_check[sizeof(TrkDataManager) == 0xE0 ? 1 : -1];
#ifndef __cplusplus /* edg (NCC donors): &((T *)0)->f is not an integral constant expression */
typedef char TrkDataManager_ghost_check[(int)&((TrkDataManager *)0)->ghostReplayMode == 0xD8 ? 1 : -1];
#endif

#define TRKDATAMGR(p) ((TrkDataManager *)(p))

/* ---- H2HWinsDisplay (h2hproc, alloc 0x9C; h2h_createPauseMenu_00000FD0, h2h_WinsDisplay_update / display) ----
 * The head-to-head post-race overlay (H2HRace.postMenu): a Node with the race, a GuiMenu (PauseMenu_create:
 * five rows, ids 1 / 4 / 3 / 2 / 5), a WhiteFader and four 0x18-byte gui sprites (agent-h run 529). */
typedef struct H2HWinsDisplay {
    /* 0x00 */ Node node;            /* vptr 0x28 */
    /* 0x2C */ s32 race;             /* the H2HRace (ctor arg 2): display reads its winner / wins / spriteSet;
                                      * update its timeout (< 2 fades the fader in, else out) */
    /* 0x30 */ s32 menu;             /* GuiMenu (PauseMenu_create); its unk30 gets the gui_fontCreate font */
    /* 0x34 */ s32 fader;            /* h2h_WhiteFader_ctor; onTimeout waits for its alpha to reach 0xFF */
    /* 0x38 */ RaceSprite sprites[4]; /* gui_spriteLoad: base-data sprite words 0x4C / 0x54 / 0x58 / 0x60; display
                                      * draws [0] / [1] (player 1) and [2] / [3] (player 2) */
    /* 0x98 */ s32 frame;            /* display: +1 per frame (bit 4 blinks the text) */
} H2HWinsDisplay;

typedef char H2HWinsDisplay_size_check[sizeof(H2HWinsDisplay) == 0x9C ? 1 : -1];

#define H2HWINS(p) ((H2HWinsDisplay *)(p))

/* ---- GameDemoProcess (titproc, alloc 0x6C8) ----
 * The attract-mode race: a RaceProcess with the title overlays on top. */
typedef struct GameDemoProcess {
    /* 0x000 */ RaceProcess race;
    /* 0x6A8 */ s32 unused6A8;        /* no titproc access (tim / trk keep their DataManager here) */
    /* 0x6AC */ s32 dataMgr;         /* a plain DataManager (the base ctor, import 0xB6C40, with 0) */
    /* 0x6B0 */ s32 keyMessage;      /* KeyMessage_ctor; only built when flags bit 25 is clear */
    /* 0x6B4 */ s32 mainLogo;        /* MainLogo_ctor; 0 when flags bit 25 is set */
    /* 0x6B8 */ s32 langMenu;        /* LangMenu_ctor; only built with the menu (flags bit 23) */
    /* 0x6BC */ s32 whiteFader;      /* tit_WhiteFader_ctor; +0x3C alpha */
    /* 0x6C0 */ s32 menu;            /* the title menu (flags bit 23), 0 otherwise */
    /* 0x6C4 */ s32 menuState;       /* onQuitButton switch: 0 logo, 1 menu, 2 language */
} GameDemoProcess;

typedef char GameDemoProcess_size_check[sizeof(GameDemoProcess) == 0x6C8 ? 1 : -1];
#ifndef __cplusplus /* edg (NCC donors): &((T *)0)->f is not an integral constant expression */
typedef char GameDemoProcess_menu_check[(int)&((GameDemoProcess *)0)->menu == 0x6C0 ? 1 : -1];
#endif

/* ---- TitleDemoProcess (titproc, alloc 0x78) ----
 * A plain Process (0x50 head, no RaceProcess) with the same overlays as GameDemoProcess. */
typedef struct TitleDemoProcess {
    /* 0x00 */ char process[0x3C];   /* Node / Process head (vptr 0x28) */
    /* 0x3C */ s32 timer;            /* frames left in the current state: handleInput counts it down;
                                      * state 0 starts at unk6C * 30, state 1 at unk70 * 30 (agent-h run 496) */
    /* 0x40 */ s32 state;            /* TitleDemoProcess_handleInput's switch (the ctor starts at 0 or 1) */
    /* 0x44 */ char unk44[0x50 - 0x44];
    /* 0x50 */ s32 dataMgr;          /* plain DataManager (also InputMgr.dataMgr) */
    /* 0x54 */ s32 keyMessage;       /* KeyMessage_ctor */
    /* 0x58 */ s32 mainLogo;         /* MainLogo_ctor */
    /* 0x5C */ s32 menu;             /* the title menu */
    /* 0x60 */ s32 langMenu;         /* LangMenu_ctor */
    /* 0x64 */ s32 whiteFader;       /* tit_WhiteFader_ctor */
    /* 0x68 */ s32 keyDelay;         /* 40 on entering state 1; counts down, then the keyMessage is shown */
    /* 0x6C */ s32 unk6C;            /* 5 (seconds; timer = x * 30) */
    /* 0x70 */ s32 unk70;            /* 0x48 (seconds; timer = x * 30) */
    /* 0x74 */ s32 unk74;            /* 15000 */
} TitleDemoProcess;

typedef char TitleDemoProcess_size_check[sizeof(TitleDemoProcess) == 0x78 ? 1 : -1];
#ifndef __cplusplus /* edg (NCC donors): &((T *)0)->f is not an integral constant expression */
typedef char TitleDemoProcess_state_check[(int)&((TitleDemoProcess *)0)->state == 0x40 ? 1 : -1];
#endif

/* Field access through an untyped pointer (agent-h run 492), as RACEPROC / TIMRACE in
 * gdl/race.h: the race ctors, DataManager methods and name-entry overlays keep their
 * `char *` / `int` parameters and locals (matching levers), and `ARCRACE(p)->dataMgr`
 * lowers to the same base + 0x6AC access as `*(int *)(p + 0x6AC)`. */
#define ARCRACE(p)    ((ArcRace *)(p))
#define MGRRACE(p)    ((MgrRace *)(p))
#define H2HRACE(p)    ((H2HRace *)(p))
#define ARCDATAMGR(p) ((ArcDataManager *)(p))
#define MGRDATAMGR(p) ((MgrDataManager *)(p))
#define H2HDATAMGR(p) ((H2HDataManager *)(p))
#define TITLEDEMO(p)  ((TitleDemoProcess *)(p))
#define GAMEDEMO(p)   ((GameDemoProcess *)(p))

/* ---- titproc overlays (agent-h run 509) ----
 * KeyMessage (0x3C, KeyMessage_ctor): the pulsing "press start" sprite. KeyMessage_update
 * moves alpha by 6 between 65 and 250, flipping `fading` at each bound; KeyMessage_display
 * draws the sprite (the shared sprite table import +0x0, or +0x18 tinted when Sym import
 * +0x18C is set) at (x, y). */
typedef struct KeyMessage {
    /* 0x00 */ Node node;
    /* 0x2C */ s32 alpha;            /* 0 at ctor; 65..250 */
    /* 0x30 */ s32 fading;           /* 0: alpha rising, 1: falling */
    /* 0x34 */ f32 x;                /* 160 */
    /* 0x38 */ f32 y;                /* 210 */
} KeyMessage;

typedef char KeyMessage_size_check[sizeof(KeyMessage) == 0x3C ? 1 : -1];

/* MainLogo (0x74, MainLogo_ctor): the title logos. Phase 0 shows the two logo halves
 * (sprites +0x60 / +0x78 at pos + part1 / pos + part2) for `hold` frames, then fades
 * alpha out by 4 per frame; at 0 it switches to phase 1, which fades the main logo (sprite
 * +0x90 at mainPos) in to 255. MainLogo_reset: phase 0, hold 150, alpha 255. */
typedef struct MainLogo {
    /* 0x00 */ Node node;
    /* 0x2C */ s32 alpha;            /* 255 */
    /* 0x30 */ s32 unk30;            /* 255 */
    /* 0x34 */ s32 hold;             /* frames before the phase-0 fade (reset: 150) */
    /* 0x38 */ s32 phase;            /* 0 logo halves, 1 main logo */
    /* 0x3C */ f32 colour[4];        /* 1, 1, 1, 0 */
    /* 0x4C */ s32 posX;             /* 163 */
    /* 0x50 */ s32 posY;             /* 154 */
    /* 0x54 */ s32 part1X;           /* 1: offset of the first half from pos */
    /* 0x58 */ s32 part1Y;           /* -57 */
    /* 0x5C */ s32 unk5C;            /* 28 (not read here) */
    /* 0x60 */ s32 unk60;            /* 17 */
    /* 0x64 */ s32 part2X;           /* -3 */
    /* 0x68 */ s32 part2Y;           /* 33 */
    /* 0x6C */ s32 mainX;            /* 67 */
    /* 0x70 */ s32 mainY;            /* 44 */
} MainLogo;

typedef char MainLogo_size_check[sizeof(MainLogo) == 0x74 ? 1 : -1];

#define KEYMSG(p)   ((KeyMessage *)(p))
#define MAINLOGO(p) ((MainLogo *)(p))

/* WhiteFader (0x40; tit_WhiteFader_ctor / h2h_WhiteFader_ctor, the same class in both modules,
 * agent-h run 519): a full-screen white fill. display clamps alpha to 0xFF, draws nothing at 0,
 * else fills (0, 0) .. (319, 239) with colour at that alpha. GameDemoProcess_handleInput steps
 * alpha by 16 toward 255 (then parks it at 720) or down to 0, and onQuitButton waits for 0. */
typedef struct WhiteFader {
    /* 0x00 */ char node[0x2C];     /* Node (vptr 0x28, name 0xC) */
    /* 0x2C */ f32 colour[4];       /* 1.0 x 4 */
    /* 0x3C */ s32 alpha;           /* 0 in the ctor */
} WhiteFader;

typedef char WhiteFader_size_check[sizeof(WhiteFader) == 0x40 ? 1 : -1];

#define WFADER(p) ((WhiteFader *)(p))

#endif
