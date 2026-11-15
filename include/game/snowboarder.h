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
 * bodies of SnowBoarder_func_0000DDCC / _0000DF04 / _0000E014 / _0000E124 and
 * (physics: pos / vel / speed / dir / maxSpeed / rival, run 452) _0000B520;
 * everything else is padding. Names stay unkNNN where the meaning is unknown.
 */

/* a 12-byte record returned by the table lookup in the DDCC/DF04/E014 family */
typedef struct SnowBRec3 {
    int a, b, c;
} SnowBRec3;

/* a float 3-vector as SnowBoarder stores it (the TUs' own Vec3 typedefs have the same layout) */
typedef struct SnowBVec3 {
    f32 x, y, z;
} SnowBVec3;

typedef struct SnowBoarder {
    char pad000[0x0B4];
    SnowBVec3 pos;          /* 0x0B4: world position. SnowBoarder_func_0000B520 compares rival->pos.z - pos.z;
                             * SnowAI (game.uso) seeds its target from rider->pos */
    char pad0C0[0x318 - 0x0C0];
    SnowBVec3 vel;          /* 0x318: velocity. B520 adds dir * nudge, then rescales it to maxSpeed
                             * (normalize + scale) when speed > maxSpeed */
    char pad324[0x348 - 0x324];
    f32 speed;              /* 0x348: |vel|, compared against maxSpeed (B520) and 30 / 10 thresholds */
    char pad34C[0x3C8 - 0x34C];
    SnowBVec3 dir;          /* 0x3C8: unit forward direction (B520 nudge axis; dotted with vel in updateDyn) */
    char pad3D4[0x6C0 - 0x3D4];
    f32 dragXZ;             /* 0x6C0: B520 scales vel.x / vel.z by (1 - dragXZ) while unk938 == 0 */
    char pad6C4[0x768 - 0x6C4];
    f32 baseMaxSpeed;       /* 0x768: the value maxSpeed eases toward (B520) */
    char pad76C[0x800 - 0x76C];
    char *input;            /* 0x800: control block; its +0x10 word carries the direction bits 4 / 8 (B520) */
    char pad804[0x844 - 0x804];
    char *unk844;           /* 0x844: child object A; its +0x30 float is negated on a mirror flip */
    char pad848[0x864 - 0x848];
    char *unk864;           /* 0x864: child object B (may be NULL); transform at +0x30 */
    char pad868[0x8C4 - 0x868];
    u32 state;              /* 0x8C4: SnowBoarder_func_0000DDCC skips its update for states 6 and 7 */
    char pad8C8[0x8F4 - 0x8C8];
    SnowBRec3 *unk8F4[3];   /* 0x8F4: three record slots refilled from the table by DDCC/DF04/E014 */
    s16 unk900;             /* 0x900: table index used when (flags & SNOWB_FLAG_MIRROR) == 0 */
    s16 unk902;             /* 0x902: table index used when mirrored */
    char pad904[0x908 - 0x904];
    struct SnowBoarder *rival; /* 0x908: the other rider; game_setupVsCpu links the two riders through it,
                                * B520's catch-up compares positions with it */
    char pad90C[0x938 - 0x90C];
    s32 unk938;             /* 0x938: B520 applies dragXZ only while this is 0 (contact / state flag) */
    char pad93C[0x978 - 0x93C];
    f32 unk978;             /* 0x978: axis/spin value, sign-flipped on a mirror flip */
    char pad97C[0xA38 - 0x97C];
    f32 unkA38;             /* 0xA38: cleared on a mirror flip */
    char padA3C[0xA58 - 0xA3C];
    u32 flags;              /* 0xA58: SNOWB_FLAG_* */
    char padA5C[0xA78 - 0xA5C];
    f32 maxSpeed;           /* 0xA78: speed cap; eased toward baseMaxSpeed (+-catch-up) by B520 */
    char padA7C[0xA8C - 0xA7C];
} SnowBoarder;

#define SNOWB_FLAG_08     0x08  /* SnowBoarder_func_0000E124: selects the notify-call form */
#define SNOWB_FLAG_20     0x20  /* SnowBoarder_func_0000E124: gates the 180-degree re-orient */
#define SNOWB_FLAG_MIRROR 0x80  /* left/right stance flip, toggled by SnowBoarder_func_0000E124 */
#define SNOWB_FLAG_NUDGE  0x100 /* SnowBoarder_func_0000B520: input bits 4 / 8 push the rider along dir */
#define SNOWB_FLAG_CATCHUP 0x20000 /* SnowBoarder_func_0000B520: maxSpeed gets +-k when the rival is > k' ahead / behind in z */

/* compile-time offset checks (array size -1 => error) */
#define SNOWB_ASSERT_OFFSET(field, off) \
    typedef char snowb_assert_##off[(int)&((SnowBoarder *)0)->field == (off) ? 1 : -1]
SNOWB_ASSERT_OFFSET(pos, 0x0B4);
SNOWB_ASSERT_OFFSET(vel, 0x318);
SNOWB_ASSERT_OFFSET(speed, 0x348);
SNOWB_ASSERT_OFFSET(dir, 0x3C8);
SNOWB_ASSERT_OFFSET(dragXZ, 0x6C0);
SNOWB_ASSERT_OFFSET(baseMaxSpeed, 0x768);
SNOWB_ASSERT_OFFSET(input, 0x800);
SNOWB_ASSERT_OFFSET(unk844, 0x844);
SNOWB_ASSERT_OFFSET(unk864, 0x864);
SNOWB_ASSERT_OFFSET(state, 0x8C4);
SNOWB_ASSERT_OFFSET(unk8F4, 0x8F4);
SNOWB_ASSERT_OFFSET(unk900, 0x900);
SNOWB_ASSERT_OFFSET(unk902, 0x902);
SNOWB_ASSERT_OFFSET(rival, 0x908);
SNOWB_ASSERT_OFFSET(unk938, 0x938);
SNOWB_ASSERT_OFFSET(unk978, 0x978);
SNOWB_ASSERT_OFFSET(unkA38, 0xA38);
SNOWB_ASSERT_OFFSET(flags, 0xA58);
SNOWB_ASSERT_OFFSET(maxSpeed, 0xA78);
typedef char snowb_assert_size[sizeof(SnowBoarder) == 0xA8C ? 1 : -1];

#endif /* GAME_SNOWBOARDER_H */
