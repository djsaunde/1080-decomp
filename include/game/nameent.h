#ifndef GAME_NAMEENT_H
#define GAME_NAMEENT_H

/* nameent.uso (module 33): the record name entry, seen from its own module (agent-b run 797;
 * docs/1080_MODES.md "nameent"). Two classes:
 *   - "NameEnter Process" (vtable Data+0x80, 0xD4 bytes) = NameEntry (include/game/nameentry.h,
 *     the one definition: the mode modules' Tim / Trk / Arc / MgrNameEntry derive from it);
 *     NEPROC() views an untyped pointer as one;
 *   - "NameEnterMessage" (vtable Data+0x18, 0x11C bytes): the letter wheel, built by
 *     NameEnterMessage_ctor. Its fields are typed in the NCC donor
 *     src/nameent_uso/nameent_uso_ncc.cpp (struct NeEntry); the C side touches only `len` and
 *     `letters`. */

#include "../gdl/node.h" /* relative: NCC donors include this header without -I include */
#include "nameentry.h"   /* NameEntry = the NameEnter Process */

typedef struct NameEnterMessage {
    /* 0x000 */ char node[0x2C];        /* Node (vptr 0x28) */
    /* 0x02C */ s32 session;            /* the record session (0 = none) */
    /* 0x030 */ char *owner;            /* the NameEntry (NameEnter Process) */
    /* 0x034 */ char unk34[0xB0 - 0x34];
    /* 0x0B0 */ s32 len;                /* letters entered (GameFlow +0x15C) */
    /* 0x0B4 */ s32 done;               /* 3 letters in: the wheel is held on END */
    /* 0x0B8 */ u8 letters[12];         /* 42 = blank (GameFlow +0x158..) */
    /* 0x0C4 */ char unkC4[0x11C - 0xC4];
} NameEnterMessage;

typedef char NameEnterMessage_size_check[sizeof(NameEnterMessage) == 0x11C ? 1 : -1];

#define NEPROC(p) ((NameEntry *)(p))
#define NEMSG(p)  ((NameEnterMessage *)(p))

#endif
