#ifndef VIEWREC_H
#define VIEWREC_H

/* viewrec.uso ("GENNewRecords Process", ROM 0x642312): the records / options screens reached from the
 * title menu and after a race. One process (GENNewRecordsProcess, ctor 0x2D0) owns six Process
 * subclasses, one per screen; each has a Node name string in viewrec Data and a Cfront vtable that
 * overrides slot 3 (update), slot 4 (handleMsg, MainOptions only) and slot 13 (drawGui):
 *
 *   class            ctor    size  vtable     Data name   proc field     proc->mode
 *   MainOptions      0x1748  0x58  Data+0x98  +0x404      mainOptions    0x10000
 *   RecordOptions    0x1B4C  0xA0  Data+0x118 +0x418      recordOptions  0x1000 / 0x2000 / 0x4000 / 0x8000
 *   EraseOptions     0x36B4  0xC8  Data+0x198 +0x44C      eraseOptions   0x80000 / 0x2000000
 *   ReplayOptions    0x4518  0x9C  Data+0x218 +0x464      replayOptions  0x4000000
 *   SoundOptions     0x4854  0x98  Data+0x298 +0x47C      soundOptions   0x40000
 *   LanguageOptions  0x5200  0x70  Data+0x328 +0x494      languageOptions 0x20000
 *
 * The vtables were read from the module's Data Reloc section; see docs/1080_REPLAY.md (viewrec).
 * C only: the C++ donors (viewrec_uso_ncc*.cpp, viewrec_uso_mp72_*.cpp) keep their own mirrors of
 * these layouts, because their bytes depend on their exact source shapes. */

#include "gdl/node.h"
#include "game/guimenu.h"

/* the Process head every screen and the process start with (game/modeproc.h ProcessHead) */
#define VR_PROCESS_HEAD                                                                             \
    /* 0x00 */ Node node;                                                                           \
    /* 0x2C */ s32 fadingOut;        /* Process_fadeOut_0000F954 sets 1; the ctors clear it */    \
    /* 0x30 */ s32 paused;           /* the ctors set 1 (Process_stop) */                         \
    /* 0x34 */ char pad34[0x48 - 0x34];                                                             \
    /* 0x48 */ struct GuiMenu *pauseMenu; /* none here: the ctors clear it */                     \
    /* 0x4C */ s32 unk4C;

/* GENNewRecordsProcess.mode: the screen that is up (and its page) */
#define VR_MODE_RECORDS       0x1000     /* RecordOptions: the tab / course grid menu */
#define VR_MODE_COURSE_TIMES  0x2000     /* RecordOptions: the course times page */
#define VR_MODE_COURSE_SCORES 0x4000     /* RecordOptions: the course scores page */
#define VR_MODE_ARCADE        0x8000     /* RecordOptions: the arcade records page */
#define VR_MODE_MAIN          0x10000    /* MainOptions */
#define VR_MODE_LANGUAGE      0x20000    /* LanguageOptions */
#define VR_MODE_SOUND         0x40000    /* SoundOptions */
#define VR_MODE_ERASE_SELECT  0x80000    /* EraseOptions: pick the record set / course */
#define VR_MODE_ERASE_CONFIRM 0x2000000  /* EraseOptions: the yes / no prompt */
#define VR_MODE_REPLAY        0x4000000  /* ReplayOptions */

/* GENNewRecordsProcess.nextMode on the erase screen: the record set to erase */
#define VR_ERASE_COURSE_TIMES  0x100000
#define VR_ERASE_COURSE_SCORES 0x200000
#define VR_ERASE_ARCADE        0x400000
#define VR_ERASE_GHOST         0x800000
#define VR_ERASE_ALL           0x1000000

/* GENNewRecordsProcess.flags (ctor arg 1) */
#define VR_FLAG_FROM_MENU   0x1000   /* RecordOptions pages go back to the records menu, not out */
#define VR_FLAG_MAIN_MENU   0x10000  /* entered from the main menu: B returns to it; the footer labels */

/* the process (ctor 0x2D0, 0xE0 bytes, vtable Data+0x18) */
typedef struct GENNewRecordsProcess {
    VR_PROCESS_HEAD
    /* 0x50 */ s32 *session;         /* ctor arg 4 (a BackupSession: [0] course records, [1] arcade) */
    /* 0x54 */ Node *mainOptions;    /* MainOptions (0x1748) */
    /* 0x58 */ Node *soundOptions;   /* SoundOptions (0x4854) */
    /* 0x5C */ Node *languageOptions; /* LanguageOptions (0x5200) */
    /* 0x60 */ Node *recordOptions;  /* RecordOptions (0x1B4C) */
    /* 0x64 */ Node *replayOptions;  /* ReplayOptions (0x4518) */
    /* 0x68 */ Node *eraseOptions;   /* EraseOptions (0x36B4) */
    /* 0x6C */ s32 mode;             /* VR_MODE_*: which screen / page is up */
    /* 0x70 */ s32 nextMode;         /* RecordOptions: the page A opens (VR_MODE_COURSE_*);
                                      * EraseOptions: the record set to erase (VR_ERASE_*) */
    /* 0x74 */ s32 flags;            /* ctor arg 1: VR_FLAG_*; & 0xF -> RecordOptions.minCursor */
    /* 0x78 */ s32 enabledCourses;   /* (flags >> 4) & 0xFF: bit n = course n selectable */
    /* 0x7C */ s32 font;             /* gui_fontCreate */
    /* 0x80 */ char pad80[0x88 - 0x80];
    /* 0x88 */ GuiMenu *menu;        /* the main menu's GuiMenu (PauseMenu_create in 0x1748) */
    /* 0x8C */ char pad8C[0x98 - 0x8C];
    /* 0x98 */ s32 arrowBlink;       /* vr_drawTitleArrows: frame counter, the arrows show on bit 3 */
    /* 0x9C */ GuiMenu *menuStack[16]; /* vr_pushMenu / vr_popMenu */
    /* 0xDC */ s32 menuDepth;
} GENNewRecordsProcess;

/* MainOptions (0x58 bytes): the main menu screen */
typedef struct MainOptions {
    VR_PROCESS_HEAD
    /* 0x50 */ GENNewRecordsProcess *proc;
    /* 0x54 */ s32 font;
} MainOptions;

/* RecordOptions (0xA0 bytes): the records menu (three tabs over a 2 x 4 course grid) and its course
 * times / course scores / arcade pages */
typedef struct RecordOptions {
    VR_PROCESS_HEAD
    /* 0x50 */ GENNewRecordsProcess *proc;
    /* 0x54 */ s32 font;
    /* 0x58 */ s32 arcadeSel;        /* arcade page: the opened entry + 1 (0 = none) */
    /* 0x5C */ s32 arcadeRow;        /* arcade page: the highlighted entry (0..2) */
    /* 0x60 */ s32 subRows[3];       /* arcade page: sub-score rows shown per entry (grow to its count) */
    /* 0x6C */ s32 frame;            /* blink counter (the cursor / a new record blinks on bit 3) */
    /* 0x70 */ s32 gridDx;
    /* 0x74 */ s32 gridDy;
    /* 0x78 */ s32 gridX;
    /* 0x7C */ s32 gridY;
    /* 0x80 */ s32 tabDx;
    /* 0x84 */ s32 unk84;
    /* 0x88 */ s32 tabX;
    /* 0x8C */ s32 tabY;
    /* 0x90 */ s32 state;            /* records menu: 0 picking a tab, 1 in the course grid */
    /* 0x94 */ s32 tab;              /* 0 course times, 1 course scores, 2 arcade */
    /* 0x98 */ s32 cursor;           /* the course (grid cell 0..7) */
    /* 0x9C */ s32 minCursor;        /* the lowest selectable course */
} RecordOptions;

/* EraseOptions (0xC8 bytes): pick a record set (and a course), then confirm */
typedef struct EraseOptions {
    VR_PROCESS_HEAD
    /* 0x50 */ GENNewRecordsProcess *proc;
    /* 0x54 */ s32 font;
    /* 0x58 */ char pad58[0x6C - 0x58];
    /* 0x6C */ s32 frame;            /* blink counter; cleared when the course changes */
    /* 0x70 */ s32 no;               /* the prompt: 0 yes, 1 no */
    /* 0x74 */ s32 gridDx;
    /* 0x78 */ s32 gridDy;
    /* 0x7C */ s32 gridX;
    /* 0x80 */ s32 gridY;
    /* 0x84 */ s32 setDx;
    /* 0x88 */ s32 setDy;
    /* 0x8C */ s32 setX;
    /* 0x90 */ s32 setY;
    /* 0x94 */ s32 rowY;             /* y of the record named in the prompt */
    /* 0x98 */ s32 askY;
    /* 0x9C */ s32 yesY;
    /* 0xA0 */ s32 phase;            /* 0 picking the set, 1 picking the course */
    /* 0xA4 */ s32 set;              /* 0 course times, 1 course scores, 2 arcade, 3 ghost, 4 all */
    /* 0xA8 */ s32 course;
    /* 0xAC */ s32 minCourse;        /* 2 for course times, else 0 */
    /* 0xB0 */ f32 selFade;          /* the set / grid fade (1 shown) */
    /* 0xB4 */ f32 askFade;          /* the prompt fade */
    /* 0xB8 */ s32 ghostCourse;      /* the saved ghost, read when set 3 is picked */
    /* 0xBC */ s32 ghostRider;
    /* 0xC0 */ s32 ghostBoard;
    /* 0xC4 */ s32 ghostMode;        /* Backup_ghostGetMode - 1 */
} EraseOptions;

/* ReplayOptions (0x9C bytes): play back the saved ghost */
typedef struct ReplayOptions {
    VR_PROCESS_HEAD
    /* 0x50 */ GENNewRecordsProcess *proc;
    /* 0x54 */ s32 font;
    /* 0x58 */ s32 frame;            /* counted up per draw */
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ s32 unk60;            /* the ctor's layout constants 0x60..0x7C, 0x84, 0x88 are not */
    /* 0x64 */ s32 unk64;            /* read by any viewrec code */
    /* 0x68 */ s32 unk68;
    /* 0x6C */ s32 unk6C;
    /* 0x70 */ s32 unk70;
    /* 0x74 */ s32 unk74;
    /* 0x78 */ s32 unk78;
    /* 0x7C */ s32 unk7C;
    /* 0x80 */ s32 rowY;             /* y of the ghost's row (vr_drawGhostRow) */
    /* 0x84 */ s32 unk84;
    /* 0x88 */ s32 unk88;
    /* 0x8C */ s32 unk8C[4];
} ReplayOptions;

/* SoundOptions (0x98 bytes): BGM / SE / voice levels (0..10) and the sound mode */
typedef struct SoundOptions {
    VR_PROCESS_HEAD
    /* 0x50 */ GENNewRecordsProcess *proc;
    /* 0x54 */ s32 font;
    /* 0x58 */ s32 modeStep;         /* x step between the three sound-mode labels */
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ s32 modeX;
    /* 0x64 */ s32 modeY;
    /* 0x68 */ s32 rowStep;          /* y step between the level rows */
    /* 0x6C */ s32 labelX;
    /* 0x70 */ s32 unk70;
    /* 0x74 */ s32 barStep;          /* x step between the level bar cells */
    /* 0x78 */ s32 barX;
    /* 0x7C */ s32 rowY;
    /* 0x80 */ s32 blink;            /* -16 per frame: the highlighted row's alpha */
    /* 0x84 */ s32 cursor;           /* rows 0..2 the levels, 3 the sound mode */
    /* 0x88 */ s32 mode;             /* options flags & 3 */
    /* 0x8C */ s32 levels[3];        /* options +0x12..+0x14 */
} SoundOptions;

/* LanguageOptions (0x70 bytes): the two-way language choice (settings_setFlag4) */
typedef struct LanguageOptions {
    VR_PROCESS_HEAD
    /* 0x50 */ GENNewRecordsProcess *proc;
    /* 0x54 */ s32 font;
    /* 0x58 */ f32 colour[4];        /* white (255 / 255.0f) */
    /* 0x68 */ s32 language;         /* 0 / 1, from bootup Bss+0x28C (vr_gLanguage) */
    /* 0x6C */ s32 blink;            /* -16 per frame: the chosen label's alpha */
} LanguageOptions;

#ifndef __cplusplus
typedef char GENNewRecordsProcess_size_check[sizeof(GENNewRecordsProcess) == 0xE0 ? 1 : -1];
typedef char GENNewRecordsProcess_menu_check[(int)&((GENNewRecordsProcess *)0)->menu == 0x88 ? 1 : -1];
typedef char GENNewRecordsProcess_session_check[(int)&((GENNewRecordsProcess *)0)->session == 0x50 ? 1 : -1];
typedef char MainOptions_size_check[sizeof(MainOptions) == 0x58 ? 1 : -1];
typedef char RecordOptions_size_check[sizeof(RecordOptions) == 0xA0 ? 1 : -1];
typedef char EraseOptions_size_check[sizeof(EraseOptions) == 0xC8 ? 1 : -1];
typedef char EraseOptions_phase_check[(int)&((EraseOptions *)0)->phase == 0xA0 ? 1 : -1];
typedef char ReplayOptions_size_check[sizeof(ReplayOptions) == 0x9C ? 1 : -1];
typedef char ReplayOptions_rowY_check[(int)&((ReplayOptions *)0)->rowY == 0x80 ? 1 : -1];
typedef char SoundOptions_size_check[sizeof(SoundOptions) == 0x98 ? 1 : -1];
typedef char LanguageOptions_size_check[sizeof(LanguageOptions) == 0x70 ? 1 : -1];
#endif

#endif /* VIEWREC_H */
