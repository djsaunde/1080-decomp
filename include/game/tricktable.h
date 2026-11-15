#ifndef GAME_TRICKTABLE_H
#define GAME_TRICKTABLE_H

/*
 * TrickTable (game.uso C2D4, vtbl Data 0x10FC): the trick list read from "rom/tricks.bin" by
 * TrickTable_load_0000C3F8 (slot 8): a count, then `count` 0x40-byte TrickEntry records, kept in
 * game.uso Bss (entries = Bss 0, count = Bss 4). The retail file (ROM 0x91F94E) has 29 entries;
 * listing in docs/1080_GAME_CLASSES.md section 18. agent-g run 494.
 *
 * RiderAnim holds three entry pointers: queuedTrick (0xF0, picked from the input while airborne),
 * curTrick (0xF4, set from queuedTrick by stateTrickStart / startQueuedTrick) and lastTrick (0xF8,
 * the previous one: repeating it resets the combo count).
 *
 * This header is plain C so the IDO NCC (C++) donors can include it by relative path.
 */

/* TrickEntry.inputMask spin bits (the low byte is the grab button / stick combination) */
#define TRICK_INPUT_SPIN180 0x3000      /* 0x1000 / 0x2000: 180 one way / the other (spin sound idx 15) */
#define TRICK_INPUT_SPIN360 0xC000      /* 0x4000 / 0x8000: 360 (spin sound idx 31) */

/* TrickEntry.flags */
#define TRICK_FLAG_HALF_TURN 1          /* odd number of half turns: stateTrickStart / stateTrickEnd flip the stance */
#define TRICK_FLAG_GRAB      2          /* a grab: anim keys 2..3 then the hold state (stateTrickWindup / Hold) */
#define TRICK_FLAG_GROUND    4          /* readTrickInput skips it unless rider groundFlags bit 0 (unused in retail) */

typedef struct TrickEntry {
    char name[0x10];        /* 0x00: display name ("MELANCHOLY", "LIEN AIR", ... "1080AIR", "ONE FOOT") */
    char unk10[0x10];       /* 0x10: TrickTable_getEntry10_0000C35C; the same 16 bytes in every retail entry */
    s32 anim;               /* 0x20: the anim stateTrickStart plays (RANIM_ID); written at trick start from
                             * animSwitch (rider flags 0x40 = switch stance, and animSwitch != 0) or animRegular */
    s32 animRegular;        /* 0x24 */
    s32 animSwitch;         /* 0x28 */
    u32 inputMask;          /* 0x2C: TrickTable_getInputMask_0000C378; TRICK_INPUT_* */
    s32 points;             /* 0x30: TrickTable_getPoints_0000C3CC: 100 (180, grabs) .. 3000 (1080) */
    u32 riderMask;          /* 0x34: TrickTable_getField34_0000C394: readTrickInput only picks the entry when
                             * (riderMask & rider +0x8C0) != 0 (0xFFFF, 0xFEFF, -1, 0xFFFFFF01 ... in retail) */
    u32 flags;              /* 0x38: TrickTable_getFlags_0000C3B0; TRICK_FLAG_* */
    s32 mirrorIndex;        /* 0x3C: index of the opposite-direction entry (180AIR 12 <-> 13); reported
                             * instead of the entry's own index while rider SNOWB_FLAG_20 is set */
} TrickEntry;

#ifndef __cplusplus
#define TRICKENTRY_ASSERT_OFFSET(field, off) \
    typedef char trickentry_assert_##off[(int)&((TrickEntry *)0)->field == (off) ? 1 : -1]
TRICKENTRY_ASSERT_OFFSET(anim, 0x20);
TRICKENTRY_ASSERT_OFFSET(inputMask, 0x2C);
TRICKENTRY_ASSERT_OFFSET(points, 0x30);
TRICKENTRY_ASSERT_OFFSET(flags, 0x38);
TRICKENTRY_ASSERT_OFFSET(mirrorIndex, 0x3C);
typedef char trickentry_assert_size[sizeof(TrickEntry) == 0x40 ? 1 : -1];
#endif

#endif /* GAME_TRICKTABLE_H */
