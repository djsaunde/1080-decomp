#ifndef GAME_SNOWAI_H
#define GAME_SNOWAI_H

#include "game/snowboarder.h"

/*
 * SnowAI: the computer rider controller of game.uso (C++ class; agent-g run 452).
 * This is the "0x4E0 main object" of READABILITY_PLAN section 9.
 *
 * Evidence: SnowAI_ctor (game_uso 0x44F4) does `this == 0 -> new(0x4E0)`, calls the
 * Node base ctor (game_libs 0x3800C) with "<Node>" (Data 0x6D0), stores the vtable
 * (Data 0x5D8) at +0x28, and constructs the embedded 0x3E0-byte SnowAIParms at +0xE4
 * (the `this + 0xE4 == 0 -> new(0x3E0)` member-ctor idiom). The parms block is named
 * "SnowAI::Parms" (Data 0x6D8) and holds 41 float parms, each built by the game_libs
 * parm ctor 0x35B1C(parm, owner, tag, 1) from a 4-char tag (Data 0x6E8..0x788).
 * SnowAI_steer (the per-frame steering step) reads the parms through
 * `level`: level 3 -> tag "xx2", level 2 -> "xx1", otherwise "xx0".
 */

/* one 0x18-byte float parm (game_libs parm base + float subclass) */
typedef struct SnowAIParm {
    char pad00[0xC];        /* 0x00: filled by the game_libs parm ctor 0x35B1C (owner list, tag) */
    void *vptr;             /* 0x0C: float-parm vtable (game_uso Data 0x3C8) */
    f32 value;              /* 0x10: the default from the ctor, overwritten by the parms file */
    s32 unk14;              /* 0x14: cleared by the ctor */
} SnowAIParm;

/* parm indices in SnowAIParms.parm[], in ctor (= Data tag table) order.
 * Every group of three is one value per difficulty level (index base + level - 1). */
#define SNOWAI_PARM_DE    0   /* "de0".."de2" */
#define SNOWAI_PARM_BD    3   /* "bd0".."bd2" */
#define SNOWAI_PARM_BT    6   /* "bt0".."bt2" */
#define SNOWAI_PARM_BU    9   /* "bu0".."bu2" */
#define SNOWAI_PARM_BJ    12  /* "bj0".."bj2" */
#define SNOWAI_PARM_CF    15  /* "cf0".."cf2" */
#define SNOWAI_PARM_DD0   18  /* "dd0", "dd5", "dd6" */
#define SNOWAI_PARM_DD1   21  /* "dd1", "dd7", "dd8" */
#define SNOWAI_PARM_DD2   24  /* "dd2", "dd3", "dd4" */
#define SNOWAI_PARM_DP    27  /* "dp0".."dp2" */
#define SNOWAI_PARM_SE    30  /* "se0".."se2" */
#define SNOWAI_PARM_MS    33  /* "ms1".."ms8" (8 parms, not per level) */
#define SNOWAI_PARM_COUNT 41

typedef struct SnowAIParms {
    const char *name;       /* 0x00: "SnowAI::Parms" */
    s32 unk04;              /* 0x04: cleared by the ctor (parm list head) */
    SnowAIParm parm[SNOWAI_PARM_COUNT]; /* 0x08 */
} SnowAIParms;

typedef struct SnowAI {
    char pad000[0x28];      /* 0x00: Node base */
    void *vptr;             /* 0x28: SnowAI vtable (game_uso Data 0x5D8) */
    s32 level;              /* 0x2C: difficulty 1..3, selects the parm of each triple */
    SnowBoarder *rider;     /* 0x30: the rider being steered (reads its pos / vel / speed) */
    s32 unk34;              /* 0x34: cleared by SnowAI_reset */
    f32 unk38;              /* 0x38: cleared by SnowAI_reset */
    f32 unk3C;              /* 0x3C: steering output computed from rider->speed (591C); SnowAI_update zeroes it every frame */
    s32 targetNode;         /* 0x40: index of the course node steered to (node table = game.uso Data base + 0x548); the scans A604 / A7F8 write it */
    s32 unk44;              /* 0x44: countdown of the 7538 stick-input step (91 / 13 / 60 / 2 / 104) */
    s32 unk48;              /* 0x48: timer, set to 20 when bit 0 of unk6C is cleared */
    s32 unk4C;              /* 0x4C: 7538 step counter */
    s32 unk50;              /* 0x50: 7538 cooldown (8, counts down) */
    s32 unk54;              /* 0x54: frame counter while rider->speed < 30 */
    s32 unk58;              /* 0x58: 7538 index into the (x, y) stick table at game.uso Data 0x638 */
    s32 scanIndex;          /* 0x5C: node being tested by the A604 / A7F8 scans (0..9) */
    f32 scanBestDist;       /* 0x60: best distance of the running scan */
    s32 aiState;            /* 0x64: SnowAI_update dispatch: 0 591C (steer), 1 6A30, 2 6CF0, 3 71E0 */
    s32 modeFlags;          /* 0x68: 1 -> SnowAI_pickFork, 2 -> A604, 4 -> A7F8 run instead of the steering step */
    s32 unk6C;              /* 0x6C: bit flags (bit 0 cleared with unk48 = 20) */
    s32 unk70;              /* 0x70: cleared by SnowAI_reset; read by 71E0 */
    s32 unk74;              /* 0x74: two-state switch in 591C (0 / 1) */
    void *unk78;            /* 0x78: last result cached by 7ACC */
    s32 altNode;            /* 0x7C: the other branch node; A3C4 keeps whichever of targetNode / altNode is nearer */
    char pad080[0xA0 - 0x80];
    f32 unkA0;              /* 0xA0: 1000.0 from the ctor; hysteresis limit of 7C1C */
    s32 unkA4;              /* 0xA4: 0/1 hysteresis flag of 7C1C */
    f32 unkA8;              /* 0xA8: scale applied to rider->vel for the look-ahead point */
    f32 unkAC;
    f32 unkB0;
    f32 unkB4;              /* 0xB4: compared with rider->speed */
    f32 unkB8;
    f32 unkBC;
    f32 unkC0;
    f32 unkC4;
    f32 unkC8;
    char pad0CC[0xD8 - 0xCC];
    f32 unkD8;
    char pad0DC[0xE4 - 0xDC];
    SnowAIParms parms;      /* 0x0E4: embedded "SnowAI::Parms" (0x3E0) */
    s32 unk4C4;             /* 0x4C4: countdown, reloaded from a level parm */
    char pad4C8[0x4D8 - 0x4C8];
    s32 initPending;        /* 0x4D8: set by SnowAI_reset; the next SnowAI_update clears it and, for a CPU
                             * rider racing a human, sets rider +0x758 from ms1..ms8 by the rival's 0x8C4 */
    s32 unk4DC;             /* 0x4DC: 3 from the ctor; limit term in 7538 */
} SnowAI;

/* compile-time offset checks (array size -1 => error) */
#define SNOWAI_ASSERT_OFFSET(type, field, off) \
    typedef char snowai_assert_##type##_##off[(int)&((type *)0)->field == (off) ? 1 : -1]
SNOWAI_ASSERT_OFFSET(SnowAIParm, value, 0x10);
SNOWAI_ASSERT_OFFSET(SnowAIParms, parm, 0x08);
SNOWAI_ASSERT_OFFSET(SnowAI, vptr, 0x28);
SNOWAI_ASSERT_OFFSET(SnowAI, level, 0x2C);
SNOWAI_ASSERT_OFFSET(SnowAI, rider, 0x30);
SNOWAI_ASSERT_OFFSET(SnowAI, unk3C, 0x3C);
SNOWAI_ASSERT_OFFSET(SnowAI, unk48, 0x48);
SNOWAI_ASSERT_OFFSET(SnowAI, unk54, 0x54);
SNOWAI_ASSERT_OFFSET(SnowAI, modeFlags, 0x68);
SNOWAI_ASSERT_OFFSET(SnowAI, unk6C, 0x6C);
SNOWAI_ASSERT_OFFSET(SnowAI, unk74, 0x74);
SNOWAI_ASSERT_OFFSET(SnowAI, unkA8, 0xA8);
SNOWAI_ASSERT_OFFSET(SnowAI, unkC8, 0xC8);
SNOWAI_ASSERT_OFFSET(SnowAI, unkD8, 0xD8);
SNOWAI_ASSERT_OFFSET(SnowAI, parms, 0xE4);
SNOWAI_ASSERT_OFFSET(SnowAI, unk4C4, 0x4C4);
SNOWAI_ASSERT_OFFSET(SnowAI, unk34, 0x34);
SNOWAI_ASSERT_OFFSET(SnowAI, targetNode, 0x40);
SNOWAI_ASSERT_OFFSET(SnowAI, unk58, 0x58);
SNOWAI_ASSERT_OFFSET(SnowAI, scanIndex, 0x5C);
SNOWAI_ASSERT_OFFSET(SnowAI, scanBestDist, 0x60);
SNOWAI_ASSERT_OFFSET(SnowAI, aiState, 0x64);
SNOWAI_ASSERT_OFFSET(SnowAI, unk70, 0x70);
SNOWAI_ASSERT_OFFSET(SnowAI, unk78, 0x78);
SNOWAI_ASSERT_OFFSET(SnowAI, altNode, 0x7C);
SNOWAI_ASSERT_OFFSET(SnowAI, unkA0, 0xA0);
SNOWAI_ASSERT_OFFSET(SnowAI, unkA4, 0xA4);
SNOWAI_ASSERT_OFFSET(SnowAI, initPending, 0x4D8);
SNOWAI_ASSERT_OFFSET(SnowAI, unk4DC, 0x4DC);
typedef char snowai_assert_parm_size[sizeof(SnowAIParm) == 0x18 ? 1 : -1];
typedef char snowai_assert_parms_size[sizeof(SnowAIParms) == 0x3E0 ? 1 : -1];
typedef char snowai_assert_size[sizeof(SnowAI) == 0x4E0 ? 1 : -1];

#endif /* GAME_SNOWAI_H */
