#ifndef GAME_RIDERANIM_H
#define GAME_RIDERANIM_H

#ifndef __cplusplus
#include "game/snowboarder.h"
#include "gdl/anim.h"
#include "game/rideranim_anims.h"
#include "game/tricktable.h"
#else
/* The edg (NCC) donors include this header by relative path, without -I include (so the includes
 * below are same-directory), and edg rejects gdl/anim.h's &((T *)0)->f asserts. They get this mirror
 * of include/gdl/anim.h's AnimPlayer instead: the same layout and member names, with the Node base
 * left opaque. Keep the two in step (C builds check the offsets below). */
#include "snowboarder.h"
#include "rideranim_anims.h"
#include "tricktable.h"
#define ANIM_PLAY       0x001
#define ANIM_LOOP       0x002
#define ANIM_PINGPONG   0x004
#define ANIM_WEIGHTED   0x080
#define ANIM_BLENDIN    0x100
#define ANIM_NO_KEY     (-1000)
typedef struct AnimPlayer {
    char node[0x2C];            /* 0x00: Node */
    s32 unk2C;
    s32 animators;              /* 0x30 */
    s32 slot0;                  /* 0x34 */
    s32 slot1;                  /* 0x38 */
    s32 unk3C;
    s32 unk40;
    s32 pose0;                  /* 0x44 */
    s32 pose1;                  /* 0x48 */
    s32 frame0;                 /* 0x4C */
    s32 frame1;                 /* 0x50 */
    f32 frac;                   /* 0x54 */
    s32 mgr;                    /* 0x58 */
    s32 unk5C;
    s32 unk60;
    s32 eventKey;               /* 0x64 */
    s32 blendEvent;             /* 0x68 */
    s32 blendFrames;            /* 0x6C */
    s32 blendCount;             /* 0x70 */
    s32 anim;                   /* 0x74 */
    f32 time;                   /* 0x78 */
    s32 startKey;               /* 0x7C */
    s32 endKey;                 /* 0x80 */
    s32 reqStartKey;            /* 0x84 */
    s32 reqEndKey;              /* 0x88 */
    s32 weightAnimNeg;          /* 0x8C */
    s32 weightAnimPos;          /* 0x90 */
    f32 weight;                 /* 0x94 */
    s32 flags;                  /* 0x98 */
    s32 unk9C;
    s32 updateCount;            /* 0xA0 */
    s32 updateInterval;         /* 0xA4 */
    f32 blendWeight;            /* 0xA8 */
    s32 blendLeft;              /* 0xAC */
} AnimPlayer;
#endif

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
 * RiderAnim_setState_0000D5F8 stores one in `queuedState` (entered at the key), RiderAnim_setNextState_0000D5BC
 * in `state` (entered now), RiderAnim_clearNextState_0000D5DC stores the null one in `state`.
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
    Pair2 queuedState;      /* 0xC0: state PMF to enter at the armed key: setState(pmf, key) stores it, and
                             * the onKey override (RiderAnim_reset_0000D438, vtable slot 10) copies it into
                             * `state` when playback passes player.eventKey (or the blend-in ends) */
    Pair2 state;            /* 0xC8: the running state PMF: RiderAnim_update (D204) calls it every frame;
                             * setNextState (D5BC) writes it directly (immediate change), clearNextState
                             * (D5DC) stores the null PMF (run 532: the old names state / nextState were
                             * swapped; evidence: Data 0x115C vtable +0x5C = D438, AnimPlayer's onKey call) */
    Pair2 pendingState;     /* 0xD0: state to enter at the next stateRide (null PMF = none): startFinish /
                             * startTimeout set stateFinish / stateTimeUp here, so a race that ends mid-move
                             * changes state once the rider is back to riding */
    s32 unkD8;              /* 0xD8: always 0: RiderAnim_init's clear of 0xBC..0x138 zeroes it and nothing writes it
                             * (run 506: no +0xD8 access through rider->anim (+0x840) in any built object, and the
                             * only +0xD8 reads with `this` are blendWeight / blendWeightDeep). So those two always
                             * fall back to stateRide / stateRideDeep: a dead hook */
    s32 rideContacts;       /* 0xDC: rider->contactCount sampled by stateRide; 1 from init */
    s32 unkE0;              /* 0xE0: no user found */
    s16 jumpCharge;         /* 0xE4: frames the jump has been held (statePreJumpHold, holdJumpLift: < jlt;
                             * jumpTakeoff scales jca by it); -1 = not charging */
    s16 holdFrames;         /* 0xE6: frames in statePreJumpHold (statePreJump clears it); past the jht parm the
                             * hold moves to its next state */
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
    s32 loadedAnim;         /* 0x104: player.anim as last handed to the anim data manager: RiderAnim_update calls
                             * AnimDataMgr_lookup(player.mgr, player.anim) (token game_uso_func_076E78 = game_libs
                             * 0x6280C) when anim != 0 and latches it here;
                             * -1 from init */
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
    f32 hitScale;           /* 0x11C: selectAnim (D9CC) scales the rider's 0xA1C..0xA24 hit vector by it; the lnc parm
                             * during stateCushionIn, 1.0 from init, after a hit and at stateCushionEnd; one hit
                             * case needs it == 1.0 */
    s32 endTrickFlag;       /* 0x120: set to 1 by endTrick; RiderAnim_update clears it when rider->dnode.vqs.trans[2] < D+0x1F4
                             * and, while it is set (on course 0 only: GameFlow.course == 0), sets RTF_FLUSH | RTF_POINTS
                             * in GameFlow.dataMgr->hudFlags (import_80020098 = the GameFlow object, input.h; +0x138 =
                             * dataMgr, a RaceTracker; +0xB4 = hudFlags), else clears them: the trick-points HUD */
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

/* Field access through an untyped `char *` / `int *` RiderAnim pointer (the donors' arg0) */
#define RA(p) ((RiderAnim *)(p))

#ifndef __cplusplus /* edg rejects the &((T *)0)->f constant form */
#define RIDERANIM_ASSERT_OFFSET(field, off) \
    typedef char rideranim_assert_##off[(int)&((RiderAnim *)0)->field == (off) ? 1 : -1]
RIDERANIM_ASSERT_OFFSET(player.eventKey, 0x64);
RIDERANIM_ASSERT_OFFSET(player.blendEvent, 0x68);
RIDERANIM_ASSERT_OFFSET(rider, 0xB4);
RIDERANIM_ASSERT_OFFSET(queuedState, 0xC0);
RIDERANIM_ASSERT_OFFSET(state, 0xC8);
RIDERANIM_ASSERT_OFFSET(pendingState, 0xD0);
RIDERANIM_ASSERT_OFFSET(unkD8, 0xD8);
RIDERANIM_ASSERT_OFFSET(rideContacts, 0xDC);
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
RIDERANIM_ASSERT_OFFSET(hitScale, 0x11C);
RIDERANIM_ASSERT_OFFSET(endTrickFlag, 0x120);
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
#endif

#endif /* GAME_RIDERANIM_H */
