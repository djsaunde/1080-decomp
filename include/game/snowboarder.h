#ifndef GAME_SNOWBOARDER_H
#define GAME_SNOWBOARDER_H

/*
 * SnowBoarder: the rider object of bootup.uso (C++ class, agent-g run 447).
 *
 * Evidence: SnowBoarder_ctor (bootup 0x90CC) does `this == 0 -> new(0xA8C)`,
 * calls the base Node ctor with the name "SnowB" and stores the vtable
 * (bootup Data 0x83D0) at +0x28. Its vtable slots are SnowBoarder_func_0000CFA0
 * (2), SnowBoarder_update (3, the "SnowB" perf section with the SnowBai /
 * SnowBDyn / SnowBAnm / SnowBChild sub-sections), SnowBoarder_checkRaceEnd (4)
 * and SnowBoarder_func_0000E4DC (8). The physics step SnowBoarder_updateDyn and
 * the collision pass SnowBoarder_procColl take the same `this`.
 *
 * Only the fields below are typed. Their offsets are proven by the matched
 * bodies of SnowBoarder_func_0000DDCC / _0000DF04 / _0000E014 / _0000E124;
 * everything else is padding. Names stay unkNNN where the meaning is unknown.
 */

/* a 12-byte record returned by the table lookup in the DDCC/DF04/E014 family */
typedef struct SnowBRec3 {
    int a, b, c;
} SnowBRec3;

typedef struct SnowBoarder {
    char pad000[0x844];
    char *unk844;           /* 0x844: child object A; its +0x30 float is negated on a mirror flip */
    char pad848[0x864 - 0x848];
    char *unk864;           /* 0x864: child object B (may be NULL); transform at +0x30 */
    char pad868[0x8C4 - 0x868];
    u32 state;              /* 0x8C4: SnowBoarder_func_0000DDCC skips its update for states 6 and 7 */
    char pad8C8[0x8F4 - 0x8C8];
    SnowBRec3 *unk8F4[3];   /* 0x8F4: three record slots refilled from the table by DDCC/DF04/E014 */
    s16 unk900;             /* 0x900: table index used when (flags & SNOWB_FLAG_MIRROR) == 0 */
    s16 unk902;             /* 0x902: table index used when mirrored */
    char pad904[0x978 - 0x904];
    f32 unk978;             /* 0x978: axis/spin value, sign-flipped on a mirror flip */
    char pad97C[0xA38 - 0x97C];
    f32 unkA38;             /* 0xA38: cleared on a mirror flip */
    char padA3C[0xA58 - 0xA3C];
    u32 flags;              /* 0xA58: SNOWB_FLAG_* */
    char padA5C[0xA8C - 0xA5C];
} SnowBoarder;

#define SNOWB_FLAG_08     0x08  /* SnowBoarder_func_0000E124: selects the notify-call form */
#define SNOWB_FLAG_20     0x20  /* SnowBoarder_func_0000E124: gates the 180-degree re-orient */
#define SNOWB_FLAG_MIRROR 0x80  /* left/right stance flip, toggled by SnowBoarder_func_0000E124 */

/* compile-time offset checks (array size -1 => error) */
#define SNOWB_ASSERT_OFFSET(field, off) \
    typedef char snowb_assert_##off[(int)&((SnowBoarder *)0)->field == (off) ? 1 : -1]
SNOWB_ASSERT_OFFSET(unk844, 0x844);
SNOWB_ASSERT_OFFSET(unk864, 0x864);
SNOWB_ASSERT_OFFSET(state, 0x8C4);
SNOWB_ASSERT_OFFSET(unk8F4, 0x8F4);
SNOWB_ASSERT_OFFSET(unk900, 0x900);
SNOWB_ASSERT_OFFSET(unk902, 0x902);
SNOWB_ASSERT_OFFSET(unk978, 0x978);
SNOWB_ASSERT_OFFSET(unkA38, 0xA38);
SNOWB_ASSERT_OFFSET(flags, 0xA58);
typedef char snowb_assert_size[sizeof(SnowBoarder) == 0xA8C ? 1 : -1];

#endif /* GAME_SNOWBOARDER_H */
