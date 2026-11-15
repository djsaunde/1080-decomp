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
 * SnowAI_func_0000591C (the per-frame steering step) reads the parms through
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
    char pad034[0x3C - 0x34];
    f32 unk3C;              /* 0x3C: steering output computed from rider->speed (591C) */
    s32 unk40;
    char pad044[0x48 - 0x44];
    s32 unk48;              /* 0x48: timer, set to 20 when bit 0 of unk6C is cleared */
    char pad04C[0x54 - 0x4C];
    s32 unk54;              /* 0x54: frame counter while rider->speed < 30 */
    char pad058[0x68 - 0x58];
    s32 modeFlags;          /* 0x68: 1 -> SnowAI_func_0000A3C4, 2 -> A604, 4 -> A7F8 run instead of the steering step */
    s32 unk6C;              /* 0x6C: bit flags (bit 0 cleared with unk48 = 20) */
    char pad070[0x74 - 0x70];
    s32 unk74;              /* 0x74: two-state switch in 591C (0 / 1) */
    char pad078[0x7C - 0x78];
    s32 unk7C;
    char pad080[0xA8 - 0x80];
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
    char pad4C8[0x4E0 - 0x4C8];
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
SNOWAI_ASSERT_OFFSET(SnowAI, unk40, 0x40);
SNOWAI_ASSERT_OFFSET(SnowAI, unk48, 0x48);
SNOWAI_ASSERT_OFFSET(SnowAI, unk54, 0x54);
SNOWAI_ASSERT_OFFSET(SnowAI, modeFlags, 0x68);
SNOWAI_ASSERT_OFFSET(SnowAI, unk6C, 0x6C);
SNOWAI_ASSERT_OFFSET(SnowAI, unk74, 0x74);
SNOWAI_ASSERT_OFFSET(SnowAI, unk7C, 0x7C);
SNOWAI_ASSERT_OFFSET(SnowAI, unkA8, 0xA8);
SNOWAI_ASSERT_OFFSET(SnowAI, unkC8, 0xC8);
SNOWAI_ASSERT_OFFSET(SnowAI, unkD8, 0xD8);
SNOWAI_ASSERT_OFFSET(SnowAI, parms, 0xE4);
SNOWAI_ASSERT_OFFSET(SnowAI, unk4C4, 0x4C4);
typedef char snowai_assert_parm_size[sizeof(SnowAIParm) == 0x18 ? 1 : -1];
typedef char snowai_assert_parms_size[sizeof(SnowAIParms) == 0x3E0 ? 1 : -1];
typedef char snowai_assert_size[sizeof(SnowAI) == 0x4E0 ? 1 : -1];

#endif /* GAME_SNOWAI_H */
