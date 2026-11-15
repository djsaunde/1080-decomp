#ifndef GDL_RACE_H
#define GDL_RACE_H

#include "gdl/backup.h"

/* libgdl race objects shared by every race mode (tim / trk / arc / mgr / h2h / trn
 * and titproc's GameDemoProcess). See docs/1080_SYSTEM_LAYER.md section 10 (the
 * RaceProcess vtable) and docs/1080_MODES.md (agent-c run 469).
 *
 * Only offsets that some matched body touches are named. Widths and signedness
 * follow the matched loads / stores (READABILITY_PLAN 9.1); pointer-valued fields
 * that the bodies treat as int stay s32. */

/* Cfront vtable: slot N is {s16 this-delta, pad, fn} at vtbl + 8 + 8*N, called as
 * vt->slot[N].fn((char *)this + vt->slot[N].delta, ...). */
typedef struct GdlVtEnt {
    /* 0x0 */ s16 delta;
    /* 0x2 */ s16 pad;
    /* 0x4 */ s32 (*fn)();
} GdlVtEnt;

/* ---- RaceProcess (game_libs 0x6900.., vtable bootup Data+0xCE18, 38 slots) ----
 * Node -> C338 -> Process (C3A0) -> RaceProcess. No out-of-line ctor: each mode's
 * race ctor inlines the chain, then calls RaceProcess_init / RaceProcess_restart. */
typedef struct RaceProcessVt {
    /* 0x00 */ s32 hdr[2];
    /* 0x08 */ GdlVtEnt slot[38];    /* names: 1080_SYSTEM_LAYER.md section 10 table */
} RaceProcessVt;

typedef char RaceProcessVt_size_check[sizeof(RaceProcessVt) == 0x138 ? 1 : -1];

/* Slot indices used by the base (vtbl offset of fn = 0xC + 8 * slot). */
#define RACEVT_ON_END_RACE      14
#define RACEVT_ON_END_RACE_ALT  15
#define RACEVT_DRAW_HUD         28
#define RACEVT_DRAW_INTRO       29
#define RACEVT_DRAW_RESULT      32
#define RACEVT_ON_MESSAGE_BOUNCE 37

/* One 2D sprite instance (ids 0x6000x, set up by RaceProcess_init). */
typedef struct RaceSprite {
    /* 0x00 */ char data[0x18];
} RaceSprite;

/* A colour parm block (address passed to the sprite colour call as a top / bottom pair). */
typedef struct RaceColour {
    /* 0x00 */ char data[0x24];
} RaceColour;

/* RaceProcess.phase (0x4E0) */
#define RACE_PHASE_INTRO      0   /* intro delay */
#define RACE_PHASE_COUNTDOWN  1   /* 3-2-1 */
#define RACE_PHASE_RACING     2
#define RACE_PHASE_END        3   /* end message (endRace / endRaceAlt) */
#define RACE_PHASE_OVER       7

/* RaceProcess.raceState (0x4DC) */
#define RACE_STATE_RACING     1
#define RACE_STATE_ENDED      2

/* RaceProcess.result (0x4F8) */
#define RACE_RESULT_FINISHED  0   /* RaceProcess_finishCourse */
#define RACE_RESULT_TIME_UP   6   /* RaceProcess_timeUp */

/* RaceProcess.flags (0x4F0) bits */
#define RACEF_SPRITES        0x00010000  /* bit 16: build the 0x6000x sprites */
#define RACEF_INTRO_DELAY    0x00040000  /* bit 18: reset sets timer = 120 */
#define RACEF_COUNTDOWN_FADE 0x00400000
#define RACEF_CLEAR_SCREEN   0x01000000
#define RACEF_NO_RESULT_MSG  0x04000000

typedef struct RaceProcess {
    /* 0x000 */ s32 nodeUnk00;
    /* 0x004 */ s32 nodeUnk04;
    /* 0x008 */ s32 nodeFlags;
    /* 0x00C */ s32 name;               /* RaceProcess_init arg2 ("TIMRace Process", ...) */
    /* 0x010 */ char children[4];       /* Node child list (address taken) */
    /* 0x014 */ s32 parent;
    /* 0x018 */ char unk018[0x10];
    /* 0x028 */ RaceProcessVt *vt;
    /* 0x02C */ s32 unk02C;
    /* 0x030 */ s32 unk030;             /* Process: drawGui draws nothing of its own while set */
    /* 0x034 */ char unk034[0x14];
    /* 0x048 */ s32 unk048;             /* child object (tim: its +0x30 gets spriteSet) */
    /* 0x04C */ s32 unk04C;             /* RaceProcess_init arg3 */
    /* 0x050 */ char parms[0x18];       /* RaceProcess_loadData reads into it */
    /* 0x068 */ RaceColour colour[6];   /* 0x68 / 0x8C, 0xB0 / 0xD4, 0xF8 / 0x11C pairs */
    /* 0x140 */ char unk140[0x398];
    /* 0x4D8 */ s32 subState;           /* handleInput: updateSub0-3 selector */
    /* 0x4DC */ s32 raceState;          /* RACE_STATE_* */
    /* 0x4E0 */ s32 phase;              /* RACE_PHASE_* (handleInput switch) */
    /* 0x4E4 */ s32 timer;              /* phase timer (frames); endRace arg1 */
    /* 0x4E8 */ s32 countStep;          /* frames per countdown digit (20) */
    /* 0x4EC */ s32 unk4EC;
    /* 0x4F0 */ s32 flags;              /* RACEF_*; init arg1 / restart arg1 */
    /* 0x4F4 */ s32 timeout;            /* 0x1FFFF after reset; restartWithTimeout arg & 0xFFFF */
    /* 0x4F8 */ s32 result;             /* RACE_RESULT_*; endRace arg2 */
    /* 0x4FC */ s32 unk4FC;
    /* 0x500 */ s32 leave;              /* set -> onLeave */
    /* 0x504 */ s32 unk504;
    /* 0x508 */ s32 unk508;
    /* 0x50C */ s32 unk50C;
    /* 0x510 */ s32 leftOnce;           /* RaceProcess_leave runs once */
    /* 0x514 */ s32 unk514;
    /* 0x518 */ s32 endCounter;         /* endRaceAlt: 60 */
    /* 0x51C */ s32 replayCam;          /* RaceProcess_createReplayCam */
    /* 0x520 */ s32 savedCam;           /* the world camera replaced by replayCam */
    /* 0x524 */ s32 unk524;
    /* 0x528 */ BackupSession *session; /* record session (*_openSession, include/gdl/backup.h); = RaceTracker.session */
    /* 0x52C */ char unk52C[8];
    /* 0x534 */ f32 scaleX;             /* countdown digit scale (1.0 after reset) */
    /* 0x538 */ f32 scaleY;
    /* 0x53C */ s32 unk53C;
    /* 0x540 */ s32 countAlpha;         /* countdown digit alpha (255 after reset) */
    /* 0x544 */ s32 msgAlpha;           /* end message alpha */
    /* 0x548 */ f32 msgX;
    /* 0x54C */ f32 msgY;
    /* 0x550 */ f32 msgVel;
    /* 0x554 */ f32 msgFloor;           /* dropMessage bounces at this y (150) */
    /* 0x558 */ char unk558[0x10];
    /* 0x568 */ s32 spriteSet;          /* sprite bank built by init when RACEF_SPRITES */
    /* 0x56C */ s32 handler;            /* child process created by init (parent = this) */
    /* 0x570 */ char unk570[0x18];
    /* 0x588 */ RaceSprite sprites[12]; /* ids 0x60000,3,4,5,1,2,6,7,B,8,9,A in slot order */
} RaceProcess;                          /* 0x6A8: derived classes start here (DataManager at +0x6A8) */

typedef char RaceProcess_size_check[sizeof(RaceProcess) == 0x6A8 ? 1 : -1];
typedef char RaceProcess_vt_check[(int)&((RaceProcess *)0)->vt == 0x28 ? 1 : -1];
typedef char RaceProcess_colour_check[(int)&((RaceProcess *)0)->colour[5] == 0x11C ? 1 : -1];
typedef char RaceProcess_subState_check[(int)&((RaceProcess *)0)->subState == 0x4D8 ? 1 : -1];
typedef char RaceProcess_result_check[(int)&((RaceProcess *)0)->result == 0x4F8 ? 1 : -1];
typedef char RaceProcess_leftOnce_check[(int)&((RaceProcess *)0)->leftOnce == 0x510 ? 1 : -1];
typedef char RaceProcess_session_check[(int)&((RaceProcess *)0)->session == 0x528 ? 1 : -1];
typedef char RaceProcess_scaleX_check[(int)&((RaceProcess *)0)->scaleX == 0x534 ? 1 : -1];
typedef char RaceProcess_msgFloor_check[(int)&((RaceProcess *)0)->msgFloor == 0x554 ? 1 : -1];
typedef char RaceProcess_spriteSet_check[(int)&((RaceProcess *)0)->spriteSet == 0x568 ? 1 : -1];
typedef char RaceProcess_sprites_check[(int)&((RaceProcess *)0)->sprites[1] == 0x5A0 ? 1 : -1];

/* ---- TimRace / TrkRace (timproc / trkproc, "TIMRace Process", alloc 0x730) ----
 * Identical layouts in both modules. The ctor inlines Node (0x2C) -> 0x50 ->
 * RaceProcess (0x6A8) and then builds the 0xE0 DataManager (a RaceTracker
 * subclass) into dataManager. */
typedef struct TimRace {
    /* 0x000 */ RaceProcess race;
    /* 0x6A8 */ s32 dataManager;        /* Tim/TrkDataManager (RaceTracker *) */
    /* 0x6AC */ s32 finalTime;          /* checkRecord: dataManager->elapsed */
    /* 0x6B0 */ s32 recordRank;         /* checkRecord: dataManager->entries[0].rank[0], then tie-scan */
    /* 0x6B4 */ RaceSprite labels[5];   /* initLabels: 0x1D / 0x1E / 0x210009 / 0x21000A / 0x21000D */
    /* 0x72C */ f32 recordFade;         /* drawHud: "new record" label alpha 0..1 */
} TimRace;

typedef char TimRace_size_check[sizeof(TimRace) == 0x730 ? 1 : -1];
typedef char TimRace_labels_check[(int)&((TimRace *)0)->labels[2] == 0x6E4 ? 1 : -1];

/* ---- RaceTracker (game_libs 0xC784.., alloc 0xB8; vtable Data+0xD678) ----
 * The race-progress / HUD object: per-rider entries, the time limit, and the
 * HUD gadgets (hud_add<X>Gadget, include/gdl/hud.h: Hud is this type). Each mode
 * derives a DataManager from it (TimDataManager etc., 0xE0). */
typedef struct RaceEntry {
    /* 0x00 */ s32 rider;               /* the rider Node (key of RaceTracker_findEntry) */
    /* 0x04 */ s32 flags;               /* bit 0: active */
    /* 0x08 */ f32 posX;                /* RaceTracker_setEntryXZ (x; progress is z) */
    /* 0x0C */ f32 progress;            /* course coordinate; -6553600 before the first gate */
    /* 0x10 */ f32 speed;               /* RaceTracker_setEntrySpeed; 0 on add / reset */
    /* 0x14 */ s32 time;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 pendingPoints;       /* accumulated, flushed by RaceTracker_endTrick */
    /* 0x20 */ s32 unk20;
    /* 0x24 */ s32 pending;             /* 1 while pendingPoints is unflushed */
    /* 0x28 */ s32 health;              /* 50 on add, 0 on reset; RaceTracker_setEntryHealth (active entries only); h2h shows it in its two gauges */
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ f32 pct;                 /* progress along the course, 0..100 */
    /* 0x34 */ s32 finished;            /* frames at 100 % */
    /* 0x38 */ s32 rank[3];             /* record ranks (3 = none), hud_loadRecords */
    /* 0x44 */ char unk44[8];
    /* 0x4C */ s32 gate;                /* next gate object (+0x64 f32 position, +0x84 next) */
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s32 gateA;               /* the two candidate gates after a fork */
    /* 0x58 */ s32 gateB;
    /* 0x5C */ s32 unk5C;
} RaceEntry;

typedef char RaceEntry_size_check[sizeof(RaceEntry) == 0x60 ? 1 : -1];
typedef char RaceEntry_pct_check[(int)&((RaceEntry *)0)->pct == 0x30 ? 1 : -1];
typedef char RaceEntry_gate_check[(int)&((RaceEntry *)0)->gate == 0x4C ? 1 : -1];

/* The entry at byte offset `off` (the matched loops walk entries in 0x60 byte steps). */
#define RACE_ENTRY_AT(t, off) ((RaceEntry *)((char *)(t)->entries + (off)))

/* RaceTracker.hudFlags (0xB4) bits */
#define RTF_FLUSH      1
#define RTF_POINTS     2
#define RTF_BONUS      4
#define RTF_SCORE      8
#define RTF_LOOKUP     0x10

typedef struct RaceTracker {
    /* 0x00 */ char unk00[0xC];
    /* 0x0C */ s32 name;                 /* Node name: "DataManager"; h2hDataManager_ctor stores base + 0x41C */
    /* 0x10 */ char children[0x18];      /* node child list (address taken only) */
    /* 0x28 */ s32 vptr;                 /* Cfront vtable (Data+0xD678 + ...) */
    /* 0x2C */ s32 active;               /* RaceTracker_update runs while set */
    /* 0x30 */ s32 elapsed;              /* race time (ms) */
    /* 0x34 */ s32 timeLimit;            /* 600000 after reset; checkTimeLimit */
    /* 0x38 */ f32 courseStart;          /* world->0x98 +0xC4 */
    /* 0x3C */ f32 courseZ;              /* world->0x98 +0xCC */
    /* 0x40 */ f32 courseLength;         /* world->0x98 +0xD8 - courseZ */
    /* 0x44 */ RaceEntry *entries;       /* alloc 0x300 = 8 entries */
    /* 0x48 */ s32 numEntries;
    /* 0x4C */ BackupSession *session;   /* = RaceProcess.session (include/gdl/backup.h) */
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s32 pointsListArg;        /* 3rd ctor arg of PointsGadget_ctor_00003138 */
    /* 0x58 */ s32 digitsTall;           /* hud_initSprites_0000E4A8: 16x320 grid, 1x10 cells */
    /* 0x5C */ s32 digitsWide;           /* hud_initSprites_0000E4A8: 160x16 grid, 10x1 cells */
    /* 0x60 */ s32 sprites60;            /* hud_initSprites_0000E5D0 (Speed / Time / Points font) */
    /* 0x64 */ s32 sprites64;            /* hud_initSprites_0000E53C (CountDown) */
    /* 0x68 */ s32 sprites68;            /* hud_initSprites_0000E53C (CountDown) */
    /* 0x6C */ s32 pointsList;           /* PointsGadget_ctor_00003138; spawns the score pop-ups */
    /* 0x70 */ s32 speed;                /* SpeedGadget */
    /* 0x74 */ s32 speedValue;           /* f32 * the speed is read from */
    /* 0x78 */ s32 time;                 /* TimeGadget */
    /* 0x7C */ s32 timeValue;            /* s32 * the time (ms) is read from */
    /* 0x80 */ s32 course;               /* CourseGadget */
    /* 0x84 */ s32 countDown;            /* CountDownGadget */
    /* 0x88 */ s32 countDownValue;       /* s32 * the count is read from */
    /* 0x8C */ s32 points;               /* PointsGadget */
    /* 0x90 */ s32 pointsValue;
    /* 0x94 */ s32 points94;             /* PointsGadget_ctor_00000ED0 class (empty release) */
    /* 0x98 */ s32 cool;                 /* CoolGadget */
    /* 0x9C */ s32 coolValue;            /* s32 * the cool value is read from */
    /* 0xA0 */ s32 score;                /* += 2000 per bonus; shown / 1000 */
    /* 0xA4 */ s32 bonus;                /* +100 per bonus, capped at 500 */
    /* 0xA8 */ char unkA8[8];
    /* 0xB0 */ s32 unkB0;                /* 1 after reset */
    /* 0xB4 */ s32 hudFlags;             /* RTF_* */
} RaceTracker;

typedef char RaceTracker_size_check[sizeof(RaceTracker) == 0xB8 ? 1 : -1];
typedef char RaceTracker_children_check[(int)&((RaceTracker *)0)->children == 0x10 ? 1 : -1];
typedef char RaceTracker_entries_check[(int)&((RaceTracker *)0)->entries == 0x44 ? 1 : -1];
typedef char RaceTracker_pointsListArg_check[(int)&((RaceTracker *)0)->pointsListArg == 0x54 ? 1 : -1];
typedef char RaceTracker_course_check[(int)&((RaceTracker *)0)->course == 0x80 ? 1 : -1];
typedef char RaceTracker_coolValue_check[(int)&((RaceTracker *)0)->coolValue == 0x9C ? 1 : -1];
typedef char RaceTracker_hudFlags_check[(int)&((RaceTracker *)0)->hudFlags == 0xB4 ? 1 : -1];

/* ---- DataManager (agent-h run 473) ----
 * "DataManager" is the node name of the RaceTracker class itself (game_libs 0xC784,
 * vtable Data+0xD678): titproc's demo processes build a plain one (base ctor with 0),
 * every race mode derives its own (TimDataManager / ArcDataManager 0xD8 / MgrDataManager
 * 0xE0 / H2HDataManager 0xF0, include/game/modeproc.h). InputMgr.dataMgr (bootup Bss
 * 0xF8 + 0x138) points at the running one. The mode subclasses agree on nothing past
 * 0xB8 (arc / mgr: 0xB8 course list, 0xBC owner race; tim / h2h: 0xB8 owner race), so
 * the base is the RaceTracker layout. */
typedef RaceTracker DataManager;

#endif
