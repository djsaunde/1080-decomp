#ifndef GAME_NAMEENT_H
#define GAME_NAMEENT_H

/* nameent.uso (module 33): the record name entry, seen from its own module (agent-b run 797;
 * docs/1080_MODES.md "nameent"). Two classes:
 *   - "NameEnter Process" (vtable Data+0x80, 0xD4 bytes): the NameEntry base the mode modules
 *     derive Tim / Trk / Arc / MgrNameEntry from (include/game/nameentry.h, owner agent-h, names
 *     only the words those modules touch; this is the full view of the same object);
 *   - "NameEnterMessage" (vtable Data+0x18, 0x11C bytes): the letter wheel, built by
 *     NameEnterMessage_ctor. Its fields are typed in the NCC donor
 *     src/nameent_uso/nameent_uso_ncc.cpp (struct NeEntry); the C side touches only `len` and
 *     `letters`. */

#include "../gdl/node.h" /* relative: NCC donors include this header without -I include */

struct BackupSession; /* include/gdl/backup.h */

typedef struct NameEnterMessage {
    /* 0x000 */ char node[0x2C];        /* Node (vptr 0x28) */
    /* 0x02C */ s32 session;            /* the record session (0 = none) */
    /* 0x030 */ char *owner;            /* the NameEnterProcess */
    /* 0x034 */ char unk34[0xB0 - 0x34];
    /* 0x0B0 */ s32 len;                /* letters entered (GameFlow +0x15C) */
    /* 0x0B4 */ s32 done;               /* 3 letters in: the wheel is held on END */
    /* 0x0B8 */ u8 letters[12];         /* 42 = blank (GameFlow +0x158..) */
    /* 0x0C4 */ char unkC4[0x11C - 0xC4];
} NameEnterMessage;

typedef char NameEnterMessage_size_check[sizeof(NameEnterMessage) == 0x11C ? 1 : -1];

typedef struct NameEnterProcess {
    /* 0x00 */ char node[0x28];         /* Node; name (+0xC) = "NameEnter Process" (Data+0x140) */
    /* 0x28 */ char *vptr;
    /* 0x2C */ s32 fadingOut;           /* Process head (include/game/modeproc.h ProcessHead) */
    /* 0x30 */ s32 paused;              /* Process head; NameEntry_init sets 1 */
    /* 0x34 */ char unk34[0x48 - 0x34];
    /* 0x48 */ struct GuiMenu *pauseMenu; /* Process head; none (0) */
    /* 0x4C */ s32 unk4C;
    /* 0x50 */ struct BackupSession *session; /* NameEntry_init arg 1: the record session the name is
                                         * written to (0 = none; nameentry.h calls it `entry`) */
    /* 0x54 */ NameEnterMessage *message; /* the letter wheel (NameEnterMessage_ctor) */
    /* 0x58 */ s32 cursorX;             /* 160: the name board is centred on it */
    /* 0x5C */ s32 cursorY;             /* 85 */
    /* 0x60 */ s32 phase;               /* 0 result, 1 entry, 2: handleInput runs vslot 14 + phase,
                                         * drawGui vslot 17 + phase */
    /* 0x64 */ s32 unk64;
    /* 0x68 */ s32 frame;               /* the mode overlays' banner blink counter */
    /* 0x6C */ s32 entered;             /* set by NameEnterMessage_update on END: commit the name */
    /* 0x70 */ s32 showBoard;           /* 1: phases 0 / 1 draw the name board, else a cleared screen */
    /* 0x74 */ char unk74[0x8C - 0x74];
    /* 0x8C */ char frameTop[0x18];     /* gui sprite 0x280000: the board's top piece */
    /* 0xA4 */ char frameLeft[0x18];    /* 0x280001: the left column */
    /* 0xBC */ char frameRight[0x18];   /* 0x280002: the right column */
} NameEnterProcess;

typedef char NameEnterProcess_size_check[sizeof(NameEnterProcess) == 0xD4 ? 1 : -1];
#ifndef __cplusplus /* edg (NCC donors): &((T *)0)->f is not an integral constant expression */
typedef char NameEnterProcess_phase_check[(int)&((NameEnterProcess *)0)->phase == 0x60 ? 1 : -1];
typedef char NameEnterProcess_frameTop_check[(int)&((NameEnterProcess *)0)->frameTop == 0x8C ? 1 : -1];
#endif

#define NEPROC(p) ((NameEnterProcess *)(p))
#define NEMSG(p)  ((NameEnterMessage *)(p))

#endif
