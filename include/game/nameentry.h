#ifndef GAME_NAMEENTRY_H
#define GAME_NAMEENTRY_H

#include "gdl/node.h"

/* The record name-entry overlay (agent-h run 509). The base class lives in nameent.uso
 * (module 33, export 69, vtable Data 0x80; docs/1080_MODES.md) and is 0xD4: each mode's
 * <Cls>NameEntry_ctor runs a 4-level in-place cascade (0x2C Node, 0x50 Process, 0xD4 NameEntry,
 * then its own size) and stores the vptr at +0x28 on each level. Only the fields the mode
 * modules touch are named. */
typedef struct NameEntry {
    /* 0x00 */ char process[0x50];  /* Node + Process head (vptr 0x28) */
    /* 0x50 */ s32 entry;           /* the name-entry widget: its state query returns 0 (none),
                                     * 1 (Mgr: the 0x120 banner) or other (0x138); <Cls>NameEntry_update
                                     * switches to phase 1 once it reports nonzero */
    /* 0x54 */ char unk54[0x60 - 0x54];
    /* 0x60 */ s32 phase;           /* the ctor's mode argument; display (slot 13) switches on it:
                                     * 0 slot 17 drawEntryOverlay, 1 slot 18 drawExitOverlay, 2 slot 19 */
    /* 0x64 */ s32 unk64;
    /* 0x68 */ s32 frame;           /* +1 per banner draw; bit 3 blinks the banner */
    /* 0x6C */ char unk6C[0xD4 - 0x6C];
} NameEntry;

typedef char NameEntry_size_check[sizeof(NameEntry) == 0xD4 ? 1 : -1];
typedef char NameEntry_phase_check[(int)&((NameEntry *)0)->phase == 0x60 ? 1 : -1];

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
