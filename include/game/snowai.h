#ifndef GAME_SNOWAI_H
#define GAME_SNOWAI_H

#ifndef __cplusplus
#include "game/snowboarder.h"
#else
/* edg (NCC) donors include this header by relative path, without -I include: same-directory include,
 * and no &((T *)0)->f asserts (edg rejects that constant form). */
#include "snowboarder.h"
#endif

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
    f32 unk3C;              /* 0x3C: steering output computed from rider->dnode.speed (591C); SnowAI_update zeroes it every frame */
    s32 targetNode;         /* 0x40: index of the course node steered to (node table = game.uso Data base + 0x548); the scans A604 / A7F8 write it */
    s32 unk44;              /* 0x44: countdown of the 7538 stick-input step (91 / 13 / 60 / 2 / 104) */
    s32 unk48;              /* 0x48: timer, set to 20 when bit 0 of unk6C is cleared */
    s32 unk4C;              /* 0x4C: 7538 step counter */
    s32 unk50;              /* 0x50: 7538 cooldown (8, counts down) */
    s32 unk54;              /* 0x54: frame counter while rider->dnode.speed < 30 */
    s32 unk58;              /* 0x58: 7538 index into the (x, y) stick table at game.uso Data 0x638 */
    s32 scanIndex;          /* 0x5C: node being tested by the A604 / A7F8 scans (0..9) */
    f32 scanBestDist;       /* 0x60: best distance of the running scan */
    s32 aiState;            /* 0x64: SnowAI_update dispatch: 0 591C (steer), 1 6A30, 2 6CF0, 3 71E0 */
    s32 modeFlags;          /* 0x68: 1 -> SnowAI_pickFork, 2 -> A604, 4 -> A7F8 run instead of the steering step */
    s32 unk6C;              /* 0x6C: bit flags (bit 0 cleared with unk48 = 20) */
    s32 finishTimer;        /* 0x70: frames since the rider finished (contacts + raceResult): SnowAI_timeoutRun / 6A30
                             * / 71E0 count it to 60 (or stop early once speed <= 30), start RiderAnim_startTimeout
                             * and park it at 61; cleared by SnowAI_reset, onTimeout and enterFinish */
    s32 aimMode;            /* 0x74: SnowAI_steer's aim: 0 SnowAI_aimAlongSegment, 1 SnowAI_aimWeave (entered when
                             * rivalLeadZ <= weaveLead, not at a marker flag, and speed >= minSpeed; left when
                             * rivalLeadZ > the se parm or those fail) */
    void *lastMarker;       /* 0x78: the CourseMarker SnowAI_updateMarker found last (its *arg4 out flag = changed) */
    s32 altNode;            /* 0x7C: the other branch node; A3C4 keeps whichever of targetNode / altNode is nearer */
    s32 atFinish;           /* 0x80: SnowAI_isAtFinish's result, stored by SnowAI_onWin / onLose at the race end
                             * (non-zero: targetNode = SnowAI_pickFinishBranch); no reader in game_uso C */
    /* SnowAI_aimWeave (aimMode 1) state: two waypoints laid along the segment and a 4-phase side counter */
    SnowBVec3 weaveAim;     /* 0x84: current waypoint (XZ, y = 0); the aim is weaveAim - rider->dnode.vqs.trans */
    SnowBVec3 weavePrev;    /* 0x90: previous waypoint */
    s32 weavePhase;         /* 0x9C: (phase + 1) & 3 per new waypoint: 0 centre, 1 side * -1, 2 / 3 side * +1 */
    /* tunables, all set by SnowAI_ctor (values in parentheses; [Dxx] = a game.uso RoData float) */
    f32 leadLimit;          /* 0xA0: (1000) SnowAI_aimAlongSegment: sideBlock turns on at rivalLeadZ >= leadLimit, off at <= -leadLimit */
    s32 sideBlock;          /* 0xA4: 0/1 hysteresis flag of SnowAI_aimAlongSegment (offset half a width toward the rival's side) */
    f32 lookAhead;          /* 0xA8: (0) time scale on rider->dnode.vel for the look-ahead point (steer, scanNodes, scanSegments) */
    f32 turnGain;           /* 0xAC: [E8] steer: stick = -yaw * (1 + speed / turnSpeedRef) * turnGain / max */
    f32 turnSpeedRef;       /* 0xB0: (500) */
    f32 minSpeed;           /* 0xB4: (70) weave needs rider->dnode.speed >= minSpeed; below it steer may set out flag 4 */
    /* SnowAI_steer's out flags (its argument to SnowAI_applyStickInput, which turns them into the stick / button
     * word of the rider's input): 1 jump sequence (unk6C bit 0, 91 frames of buttons 0x500), 2 button 0x400 (the
     * rider's d-parms: useDParms), 4 stick up, 8 stick down (crouch), 0x10 finished (stick down), 0x20 / 0x80 the
     * table-driven stick gestures, 0x40 13 frames of button 0x100. agent-g run 506. */
    f32 crouchSteerRef;     /* 0xB8: (230) out flag 8 (crouch) when speed / crouchSteerRef * |unk3C| > 1: a hard turn
                             * at speed */
    f32 stallYaw;           /* 0xBC: [EC] out flag 0x40 when the aim yaw >= stallYaw at speed < 50 */
    f32 reachTime;          /* 0xC0: (12) the aim point counts as reached (out flag 1 / unk6C bit 0 cleared) within
                             * reachTime * speed (reachTime + 12 for the second test) */
    f32 dParmsSteer;        /* 0xC4: [F0] out flag 2 (button 0x400) when |unk3C| <= dParmsSteer * (useDParms ?
                             * dParmsSteerScale : 1) and the rival lead is past the level's de parm */
    f32 dParmsSteerScale;   /* 0xC8: (2) */
    f32 aimRange;           /* 0xCC: (50) SnowAI_aimAlongSegment: speed * aimRange compare */
    f32 rivalPathTime;      /* 0xD0: (20) SnowAI_isRivalInPath_000097EC: how far ahead (in vel units) the path runs */
    f32 rivalPathDist;      /* 0xD4: (500) SnowAI_isRivalInPath_000097EC: the distance limit to that path */
    f32 weaveLead;          /* 0xD8: [F8] steer enters weave (aimMode 1) when rivalLeadZ <= weaveLead */
    f32 weaveLeadTime;      /* 0xDC: (15) weave waypoints are speed * weaveLeadTime / 2 ahead / behind */
    f32 weaveSideScale;     /* 0xE0: [F4] weave side offset = 250 * width * weaveSideScale */
    SnowAIParms parms;      /* 0x0E4: embedded "SnowAI::Parms" (0x3E0) */
    s32 scaleTimer;         /* 0x4C4: the rider-parms boost state of SnowAI_steer (agent-g run 512): 0 = normal; when
                             * the dp parm < the steer metric it scales the rider parms (SnowAI_scaleRiderParms) and
                             * sets 0x10000 (armed); armed and the metric < dd0: SnowAI_scaleRiderMax, then dd1
                             * frames of countdown; reaching 0 (or steer flag 8, or SnowAI_enterFinish) restores
                             * them (SnowAI_restoreRiderParms) */
    /* the rider parm values SnowAI_scaleRiderParms saves before scaling them (and SnowAI_restoreRiderParms /
     * SnowAI_scaleRiderMax use): */
    f32 savedMax;           /* 0x4C8: rider->parms.max.value (scaled by dd2, or dd3 in scaleRiderMax) */
    f32 savedNts;           /* 0x4CC: rider->parms.nts.value (scaled by dd4) */
    f32 savedDts;           /* 0x4D0: rider->parms.dts.value (scaled by dd4) */
    f32 savedNbf;           /* 0x4D4: rider->child.parms.nbf.value (replaced by dbf while scaled) */
    s32 initPending;        /* 0x4D8: set by SnowAI_reset; the next SnowAI_update clears it and, for a CPU
                             * rider racing a human, sets rider->parms.max from ms1..ms8 by the rival's character */
    s32 unk4DC;             /* 0x4DC: 3 from the ctor; limit term in 7538 */
} SnowAI;

#ifndef __cplusplus
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
SNOWAI_ASSERT_OFFSET(SnowAI, aimMode, 0x74);
SNOWAI_ASSERT_OFFSET(SnowAI, weaveAim, 0x84);
SNOWAI_ASSERT_OFFSET(SnowAI, weavePhase, 0x9C);
SNOWAI_ASSERT_OFFSET(SnowAI, lookAhead, 0xA8);
SNOWAI_ASSERT_OFFSET(SnowAI, dParmsSteerScale, 0xC8);
SNOWAI_ASSERT_OFFSET(SnowAI, aimRange, 0xCC);
SNOWAI_ASSERT_OFFSET(SnowAI, weaveLead, 0xD8);
SNOWAI_ASSERT_OFFSET(SnowAI, weaveSideScale, 0xE0);
SNOWAI_ASSERT_OFFSET(SnowAI, parms, 0xE4);
SNOWAI_ASSERT_OFFSET(SnowAI, scaleTimer, 0x4C4);
SNOWAI_ASSERT_OFFSET(SnowAI, unk34, 0x34);
SNOWAI_ASSERT_OFFSET(SnowAI, targetNode, 0x40);
SNOWAI_ASSERT_OFFSET(SnowAI, unk58, 0x58);
SNOWAI_ASSERT_OFFSET(SnowAI, scanIndex, 0x5C);
SNOWAI_ASSERT_OFFSET(SnowAI, scanBestDist, 0x60);
SNOWAI_ASSERT_OFFSET(SnowAI, aiState, 0x64);
SNOWAI_ASSERT_OFFSET(SnowAI, finishTimer, 0x70);
SNOWAI_ASSERT_OFFSET(SnowAI, lastMarker, 0x78);
SNOWAI_ASSERT_OFFSET(SnowAI, altNode, 0x7C);
SNOWAI_ASSERT_OFFSET(SnowAI, leadLimit, 0xA0);
SNOWAI_ASSERT_OFFSET(SnowAI, sideBlock, 0xA4);
SNOWAI_ASSERT_OFFSET(SnowAI, savedMax, 0x4C8);
SNOWAI_ASSERT_OFFSET(SnowAI, savedNbf, 0x4D4);
SNOWAI_ASSERT_OFFSET(SnowAI, initPending, 0x4D8);
SNOWAI_ASSERT_OFFSET(SnowAI, unk4DC, 0x4DC);
typedef char snowai_assert_parm_size[sizeof(SnowAIParm) == 0x18 ? 1 : -1];
typedef char snowai_assert_parms_size[sizeof(SnowAIParms) == 0x3E0 ? 1 : -1];
typedef char snowai_assert_size[sizeof(SnowAI) == 0x4E0 ? 1 : -1];
#endif

#endif /* GAME_SNOWAI_H */
