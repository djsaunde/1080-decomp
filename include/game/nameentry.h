#ifndef GAME_NAMEENTRY_H
#define GAME_NAMEENTRY_H

#include "../gdl/node.h" /* relative: NCC donors include this header without -I include */

struct BackupSession;    /* include/gdl/backup.h */
struct GuiMenu;          /* include/game/guimenu.h */
struct NameEnterMessage; /* include/game/nameent.h: the letter wheel */

/* The record name entry: nameent.uso's "NameEnter Process" (module 33, export 69 = vtable
 * Data+0x80, 0xD4 bytes). One class, two views merged here (agent-h run 805):
 *   - nameent.uso builds and runs it (NameEntry_init / _handleInput / _drawGui ...,
 *     src/nameent_uso/nameent_uso.c; include/game/nameent.h adds the NameEnterMessage);
 *   - the mode modules derive Tim / Trk / Arc / MgrNameEntry from it: each
 *     <Cls>NameEntry_ctor runs a 4-level in-place cascade (0x2C Node, 0x50 Process, 0xD4
 *     NameEntry, then its own size), storing the vptr at +0x28 on each level; the 0xD4 level's
 *     vptr is the import nameent:Data+0x80 (scripts/uso-imports.py sites timproc), the same
 *     vtable NameEntry_init's object carries. The mode's onRaceOver then calls
 *     NameEntry_init(nameEntry, race.session). */
typedef struct NameEntry {
    /* 0x00 */ char node[0x28];         /* Node; name (+0xC) = "NameEnter Process" (Data+0x140) */
    /* 0x28 */ char *vptr;
    /* 0x2C */ s32 fadingOut;           /* Process head (include/game/modeproc.h ProcessHead) */
    /* 0x30 */ s32 paused;              /* Process head; NameEntry_init sets 1 */
    /* 0x34 */ char unk34[0x48 - 0x34];
    /* 0x48 */ struct GuiMenu *pauseMenu; /* Process head; none (0) */
    /* 0x4C */ s32 unk4C;
    /* 0x50 */ struct BackupSession *session; /* NameEntry_init arg 1 (the race's record session,
                                         * 0 = none): the name is written to it; the modes'
                                         * <Cls>NameEntry_update pass it to Backup_sessionHasNew
                                         * (switch to phase 1 once a new record is pending) and
                                         * drawTitle to Backup_sessionCountNew */
    /* 0x54 */ struct NameEnterMessage *message; /* the letter wheel (NameEnterMessage_ctor) */
    /* 0x58 */ s32 cursorX;             /* 160: the name board is centred on it */
    /* 0x5C */ s32 cursorY;             /* 85 */
    /* 0x60 */ s32 phase;               /* the ctor's mode argument: 0 result, 1 entry, 2;
                                         * handleInput runs vslot 14 + phase, drawGui vslot 17 +
                                         * phase (17 drawEntryOverlay, 18 drawExitOverlay) */
    /* 0x64 */ s32 unk64;
    /* 0x68 */ s32 frame;               /* +1 per banner draw; bit 3 blinks the banner */
    /* 0x6C */ s32 entered;             /* set by NameEnterMessage_update on END: commit the name */
    /* 0x70 */ s32 showBoard;           /* 1: phases 0 / 1 draw the name board, else a cleared screen */
    /* 0x74 */ char unk74[0x8C - 0x74];
    /* 0x8C */ char frameTop[0x18];     /* gui sprite 0x280000: the board's top piece */
    /* 0xA4 */ char frameLeft[0x18];    /* 0x280001: the left column */
    /* 0xBC */ char frameRight[0x18];   /* 0x280002: the right column */
} NameEntry;

typedef char NameEntry_size_check[sizeof(NameEntry) == 0xD4 ? 1 : -1];
#ifndef __cplusplus /* edg (NCC donors): &((T *)0)->f is not an integral constant expression */
typedef char NameEntry_session_check[(int)&((NameEntry *)0)->session == 0x50 ? 1 : -1];
typedef char NameEntry_phase_check[(int)&((NameEntry *)0)->phase == 0x60 ? 1 : -1];
typedef char NameEntry_frameTop_check[(int)&((NameEntry *)0)->frameTop == 0x8C ? 1 : -1];
#endif

/* The Tim / Trk / Arc overlay (0x10C: TimNameEntry_ctor_000016F8, TrkNameEntry_ctor_00001660,
 * ArcNameEntry_ctor_0000199C). The three text positions are only set here (nameent.uso reads
 * them): (160, 130), (160, 29), (160, 105). */
typedef struct ModeNameEntry {
    /* 0x000 */ NameEntry base;
    /* 0x0D4 */ s32 race;           /* the owner race process (ctor arg 1): drawEntryOverlay passes
                                     * its finalTime, drawExitOverlay fades its record */
    /* 0x0D8 */ s32 pos[3][2];      /* x, y: (0xA0, 0x82), (0xA0, 0x1D), (0xA0, 0x69) */
    /* 0x0F0 */ char banner[0x18];  /* sprite, id ((import + 0x23) << 16) | (race word + 7) */
    /* 0x108 */ f32 alpha;          /* 1.0: the banner alpha (x 255) */
} ModeNameEntry;

typedef char ModeNameEntry_size_check[sizeof(ModeNameEntry) == 0x10C ? 1 : -1];

/* The championship (mgr) overlay (0x174, MgrNameEntry_ctor_00002940): five sprites, the
 * congratulations / credits draw (MgrNameEntry_drawCongrats_00002B7C) and a sliding panel. */
typedef struct MgrNameEntry {
    /* 0x000 */ NameEntry base;
    /* 0x0D4 */ s32 race;           /* the MgrRace (ctor arg 1) */
    /* 0x0D8 */ s32 pos[3][2];      /* as ModeNameEntry */
    /* 0x0F0 */ char sprite0[0x18]; /* id 0x230004 */
    /* 0x108 */ char sprite1[0x18]; /* id 0x230003 */
    /* 0x120 */ char banner1[0x18]; /* id 0x230005: banner when the entry state is 1 */
    /* 0x138 */ char banner2[0x18]; /* id 0x230006: banner otherwise */
    /* 0x150 */ char sprite4[0x18]; /* id ((import + 0x23) << 16) | (import word - 1) */
    /* 0x168 */ f32 alpha;          /* 1.0; drawExitOverlay steps it down to 0, then notifies */
    /* 0x16C */ s32 unk16C;         /* 0 */
    /* 0x170 */ s32 panelY;         /* 0xFA; drawCongrats slides it up to 0x78 */
} MgrNameEntry;

typedef char MgrNameEntry_size_check[sizeof(MgrNameEntry) == 0x174 ? 1 : -1];

/* Field access through the untyped `char *` / `int *` parameters (matching levers). */
#define NAMEENT(p)    ((NameEntry *)(p))
#define MODENAMEENT(p) ((ModeNameEntry *)(p))
#define MGRNAMEENT(p) ((MgrNameEntry *)(p))

#endif
