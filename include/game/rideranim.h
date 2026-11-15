#ifndef GAME_RIDERANIM_H
#define GAME_RIDERANIM_H

#include "game/snowboarder.h"
#include "gdl/anim.h"
#include "game/rideranim_anims.h"
#include "game/tricktable.h"

/*
 * RiderAnim: the rider animation state machine of game.uso (C++ class at 0xC48C, vtable Data 0x115C,
 * 0x444 bytes; agent-g runs 456 / 461). SnowBoarder_ctor creates it (rider->anim, +0x840).
 *
 * Its base (game_libs 0x62E80, vtable Data 0x21F58) is the libgdl AnimPlayer (include/gdl/anim.h).
 * The state machine runs on the player's onKey hook (vtable slot 10): setState(pmf, key) stores the
 * key in player.eventKey, so the state changes when playback passes that key; key -1 goes to
 * player.blendEvent instead (the change happens when the ANIM_BLENDIN cross-fade ends).
 * Anim ids: include/game/rideranim_anims.h. The state table (anims, keys, next states):
 * docs/1080_GAME_CLASSES.md section 17.
 *
 * States are Cfront pointers to member functions. Every record of the game.uso Data table
 * 0xDB0..0xF60 is {0x0000FFFF, method}: delta 0, index -1 (non-virtual), then the
 * RiderAnim_state_* function; the record at 0xDC8 is the null PMF {0, 0}.
 * RiderAnim_setState_0000D5F8 stores one in `state`, RiderAnim_setNextState_0000D5BC in
 * `nextState`, RiderAnim_clearNextState_0000D5DC stores the null one.
 */

/* an 8-byte pair; for RiderAnim it is a state PMF: a = 0x0000FFFF (delta 0, index -1), b = method */
typedef struct Pair2 {
    int a, b;
} Pair2;

/* one 0x18-byte parm: game_libs parm base + a float (Data 0xF78) or int (Data 0xFC0) subclass */
typedef struct RiderAnimParm {
    char pad00[0xC];        /* 0x00: owner list link and the 4-char tag */
    void *vptr;             /* 0x0C */
    union {
        f32 f;
        s32 i;              /* dm1..dm4, st1..st3, sl1 / sl2, slm / stm are read as ints */
    } value;                /* 0x10 */
    s32 unk14;              /* 0x14 */
} RiderAnimParm;

/* the 32 parms, named by their tag, in RiderAnim_ctor construction order */
typedef struct RiderAnimParms {
    const char *name;       /* 0x00 */
    s32 unk04;              /* 0x04: parm list head */
    RiderAnimParm lnc, jct, jht, jot, jca, jlt, jli; /* 0x008..0x098: jump timing (crouch / hang / out ...) */
    RiderAnimParm rfc, tkt; /* 0x0B0, 0x0C8 */
    RiderAnimParm b1f, b2f, xf1, f1f, f2f, f3f, f4f, l1f, l2f, l3f, r1f, r2f; /* 0x0E0..0x1E8 */
    RiderAnimParm dm1, dm2, dm3, dm4; /* 0x200..0x248: damage per hit kind (RiderAnim_applyDamage_0000EDD4) */
    RiderAnimParm st1, st2, st3;      /* 0x260..0x290: per-kind step of rider +0x960 (game_uso 0xECEC) */
    RiderAnimParm sl1, sl2;           /* 0x2A8, 0x2C0 */
    RiderAnimParm slm, stm;           /* 0x2D8, 0x2F0: the limits ECEC compares rider +0x960 with */
} RiderAnimParms;

typedef struct RiderAnim {
    AnimPlayer player;      /* 0x00: base. player.eventKey (0x64) = setState's key when it is not -1,
                             * player.blendEvent (0x68) = -1 for setState(.., -1); reset sets both to
                             * ANIM_NO_KEY */
    s32 spinAnim;           /* 0xB0: ground spin anim picked by checkGroundSpin (animSet | RANIM_TO_FAKIE_*),
                             * 0 = none; cleared at the top of every check */
    SnowBoarder *rider;     /* 0xB4 */
    char pad0B8[0xC0 - 0xB8];
    Pair2 state;            /* 0xC0: current state PMF */
    Pair2 nextState;        /* 0xC8: next state PMF (reset copies state into it) */
    char pad0D0[0xE4 - 0xD0];
    s16 jumpCharge;         /* 0xE4: frames the jump has been held (statePreJumpHold, holdJumpLift: < jlt;
                             * jumpTakeoff scales jca by it); -1 = not charging */
    s16 unkE6;              /* 0xE6 */
    s32 trickButtons;       /* 0xE8: trick button bits gathered by readTrickInput (cleared by clearTricks) */
    s32 trickInputFrames;   /* 0xEC: frames trickButtons has been non-zero; readTrickInput picks the entry
                             * whose inputMask equals trickButtons once it exceeds the tkt parm */
    TrickEntry *queuedTrick; /* 0xF0: the trick picked from the input, started by stateTrickStart /
                             * startQueuedTrick (cleared by clearTricks, jumpTakeoff) */
    TrickEntry *curTrick;   /* 0xF4: the trick being played (anim = curTrick->anim); stateTrickEnd clears it */
    TrickEntry *lastTrick;  /* 0xF8: the previous trick: starting the same one again resets comboCount;
                             * cleared by endTrick */
    s32 animSet;            /* 0xFC: 0x10000 regular / 0x50000 switch stance (flipStance); anim numbers are ORed in */
    s32 comboCount;         /* 0x100: tricks in the current combo (endTrick scores min(count, 10)) */
    s32 unk104;             /* 0x104: -1 from init */
    s32 selectedAnim;       /* 0x108: selectAnim's result (animSet | anim number) */
    s32 hitKind;            /* 0x10C: 1..5, set by game_uso 0xEDCC; ECEC uses it to pick st1..st3 / sl1 / sl2 */
    s32 slowCrash;          /* 0x110: updateSlowdown sets it (and clears hitKind) when rider->slowdown passes the
                             * slm / stm limit while in contact, else clears it; stateCrashRecover replays the
                             * crash anim (selectedAnim) while it is set */
    s32 trickPhase;         /* 0x114: 1 = in a trick (stateTrickAir), 2 = trick ended (stateTrickEnd; also the
                             * init value), 0 = riding (stateStand, checkReverseBrake, crashes in D9CC). D9CC
                             * drops a pending hit anim while it is 1 */
    s32 jumpRepress;        /* 0x118: set by statePreJumpCancel: checkJump then needs a fresh press (input
                             * pressed 0x100) instead of the held button, and clears it on the jump */
    f32 unk11C;             /* 0x11C: 1.0 from init and after a hit (D9CC compares it with 1.0) */
    s32 unk120;             /* 0x120: set to 1 by endTrick */
    s16 lastStickDir;       /* 0x124: rider->input->stickDir seen last frame (readStickSpin) */
    s16 spinFrames;         /* 0x126: frames of the running stick-spin gesture: 1 at the first step, counts up;
                             * the gesture ends (and is scored) at 6 frames or when the stick centres */
    s16 spinSteps;          /* 0x128: signed sum of the stick direction steps (+-1..2 each) during the gesture;
                             * negated for a mirrored / switch rider */
    s16 pad12A;
    s32 spinKind;           /* 0x12C: readStickSpin's result: 0 none, 1 / 2 = 180 / 360 one way (steps -1..-2 /
                             * < -2), 3 / 4 = 180 / 360 the other (1..2 / >= 3), 5 / 6 = stick flicked up / down
                             * (pressed 0x2000 / 0x8000). checkGroundSpin maps 1..4 to the TO_FAKIE_180F / 360F /
                             * 180B / 360B anims */
    s16 backHits;           /* 0x130: checkBackwardSwitch: consecutive frames with rider->fwdSpeed < -30 (riding
                             * backwards); past 3 the rider switches stance (anim SWITCH_STANCE / DEEP_SWITCH) */
    char pad132[0x13C - 0x132];
    RiderAnimParms parms;   /* 0x13C: read by RiderAnim_readParms (vtbl slot 8) */
} RiderAnim;

#define RIDERANIM_ASSERT_OFFSET(field, off) \
    typedef char rideranim_assert_##off[(int)&((RiderAnim *)0)->field == (off) ? 1 : -1]
RIDERANIM_ASSERT_OFFSET(player.eventKey, 0x64);
RIDERANIM_ASSERT_OFFSET(player.blendEvent, 0x68);
RIDERANIM_ASSERT_OFFSET(rider, 0xB4);
RIDERANIM_ASSERT_OFFSET(state, 0xC0);
RIDERANIM_ASSERT_OFFSET(nextState, 0xC8);
RIDERANIM_ASSERT_OFFSET(jumpCharge, 0xE4);
RIDERANIM_ASSERT_OFFSET(trickButtons, 0xE8);
RIDERANIM_ASSERT_OFFSET(queuedTrick, 0xF0);
RIDERANIM_ASSERT_OFFSET(curTrick, 0xF4);
RIDERANIM_ASSERT_OFFSET(lastTrick, 0xF8);
RIDERANIM_ASSERT_OFFSET(animSet, 0xFC);
RIDERANIM_ASSERT_OFFSET(comboCount, 0x100);
RIDERANIM_ASSERT_OFFSET(selectedAnim, 0x108);
RIDERANIM_ASSERT_OFFSET(hitKind, 0x10C);
RIDERANIM_ASSERT_OFFSET(slowCrash, 0x110);
RIDERANIM_ASSERT_OFFSET(trickPhase, 0x114);
RIDERANIM_ASSERT_OFFSET(jumpRepress, 0x118);
RIDERANIM_ASSERT_OFFSET(unk11C, 0x11C);
RIDERANIM_ASSERT_OFFSET(unk120, 0x120);
RIDERANIM_ASSERT_OFFSET(lastStickDir, 0x124);
RIDERANIM_ASSERT_OFFSET(spinSteps, 0x128);
RIDERANIM_ASSERT_OFFSET(spinKind, 0x12C);
RIDERANIM_ASSERT_OFFSET(backHits, 0x130);
RIDERANIM_ASSERT_OFFSET(parms, 0x13C);
RIDERANIM_ASSERT_OFFSET(parms.dm1, 0x33C);
RIDERANIM_ASSERT_OFFSET(parms.st1, 0x39C);
RIDERANIM_ASSERT_OFFSET(parms.stm, 0x42C);
typedef char rideranim_assert_parm_size[sizeof(RiderAnimParm) == 0x18 ? 1 : -1];
typedef char rideranim_assert_parms_size[sizeof(RiderAnimParms) == 0x308 ? 1 : -1];
typedef char rideranim_assert_size[sizeof(RiderAnim) == 0x444 ? 1 : -1];

#endif /* GAME_RIDERANIM_H */
