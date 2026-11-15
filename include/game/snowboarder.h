#ifndef GAME_SNOWBOARDER_H
#define GAME_SNOWBOARDER_H

/*
 * SnowBoarder: the rider object of bootup.uso (C++ class, agent-g run 447).
 *
 * Evidence: SnowBoarder_ctor (bootup 0x90CC) does `this == 0 -> new(0xA8C)`,
 * calls the base Node ctor with the name "SnowB" and stores the vtable
 * (bootup Data 0x83D0) at +0x28. Its vtable slots are SnowBoarder_display
 * (2), SnowBoarder_update (3, the "SnowB" perf section with the SnowBai /
 * SnowBDyn / SnowBAnm / SnowBChild sub-sections), SnowBoarder_handleMsg (4)
 * and SnowBoarder_readParms (8). The physics step SnowBoarder_updateDyn and
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

/* one 0x18-byte float parm (game_libs parm base 0x35B1C + float subclass; same layout as SnowAIParm) */
typedef struct SnowBParm {
    char pad00[0xC];        /* 0x00: owner list link and the 4-char tag (game_libs Parms_addParm) */
    void *vptr;             /* 0x0C: float-parm vtable (bootup Data 0x8018) */
    f32 value;              /* 0x10: ctor default, overwritten by the parms file */
    s32 unk14;              /* 0x14 */
} SnowBParm;

/* "SnowBoarder::Parms" (0x2C0), embedded at SnowBoarder +0x540. Members are named by their
 * parms-file tag, in SnowBoarder_ctor construction order (bootup 0x9680..). Meanings are only
 * given where a reader proves them. */
typedef struct SnowBoarderParms {
    const char *name;       /* 0x00: "SnowBoarder::Parms" */
    s32 unk04;              /* 0x04: parm list head */
    SnowBParm hpf, snf, dsf, icf, tfc, edf, bdf, ure, mro, mpi, mdr; /* 0x08..0x0F8 */
    SnowBParm axr, ayr, azr, tlt; /* 0x110..0x158 */
    SnowBParm wfr;          /* 0x170 (SnowBoarder +0x6B0): XZ drag; SnowBoarder_limitSpeed scales vel.x / vel.z by 1 - value */
    SnowBParm sfr, dts;     /* 0x188, 0x1A0 (+0x6E0: dts, scaled by SnowAI_scaleRiderParms) */
    SnowBParm nts;          /* 0x1B8 (+0x6F8): steering rate; SnowAI_finishRun divides its turn output by it */
    SnowBParm trt, twt, min; /* 0x1D0..0x200 */
    SnowBParm max;          /* 0x218 (+0x758): max speed; maxSpeed eases toward max.value (SnowBoarder_limitSpeed);
                             * SnowAI_update sets it per rival character from ms1..ms8 */
    SnowBParm jmp, djp, ufs, bdm, cdm, tch; /* 0x230..0x2A8 */
} SnowBoarderParms;

/* The 0x154-byte child at SnowBoarder +0x3EC (vtbl bootup Data 0x8108, perf label "SnowBChild"; its vtbl slot 8 is
 * SnowBChild_readParms_0000E720). Its embedded "Parms" (+0x2C = rider +0x418) holds the ground-handling parms that
 * SnowBoarder_updateDyn selects every frame. Tags in construction order; the letters read as n = normal set,
 * d = the alternate set chosen while useDParms is set, i / d (ied, ifr / ded, dfr) = the surface 'c' / 'b' scales. */
typedef struct SnowBChildParms {
    const char *name;       /* 0x00: "Parms" */
    s32 unk04;              /* 0x04: parm list head */
    SnowBParm ied, ifr;     /* 0x08, 0x20 (rider value 0x430, 0x448): surface 'c' scales of edgeGrip / friction */
    SnowBParm ded, dfr;     /* 0x38, 0x50 (0x460, 0x478): surface 'b' scales of edgeGrip / friction */
    SnowBParm efr;          /* 0x68 (0x490): copied to edgeFriction every frame */
    SnowBParm nbf, nel;     /* 0x80, 0x98 (0x4A8, 0x4C0): base friction / edge grip, normal set */
    SnowBParm dbf, del;     /* 0xB0, 0xC8 (0x4D8, 0x4F0): base friction / edge grip, alternate set */
    SnowBParm res;          /* 0xE0 (0x508): turnRate = res * nts (or dts) */
    SnowBParm flx, sta;     /* 0xF8, 0x110 */
} SnowBChildParms;

typedef struct SnowBChild {
    char pad00[0x2C];
    SnowBChildParms parms;  /* 0x2C */
} SnowBChild;

struct SnowAI;
struct RiderAnim;

typedef struct SnowBoarder {
    char pad000[0x0B4];
    SnowBVec3 pos;          /* 0x0B4: world position (the node position: the ctor also sets a child node's
                             * +0xB4 to (0, 100, 0)). SnowBoarder_limitSpeed compares rival->pos.z - pos.z;
                             * SnowAI (game.uso) seeds its target from rider->pos */
    char pad0C0[0x318 - 0x0C0];
    SnowBVec3 vel;          /* 0x318: velocity. B520 adds dir * nudge, then rescales it to maxSpeed
                             * (normalize + scale) when speed > maxSpeed */
    char pad324[0x348 - 0x324];
    f32 speed;              /* 0x348: |vel|, compared against maxSpeed (B520) and 30 / 10 thresholds */
    char pad34C[0x360 - 0x34C];
    f32 friction;           /* 0x360: ground friction, rebuilt each frame by SnowBoarder_updateDyn: nbf / dbf, + |spin.x| * k
                             * while crouching, * ifr / dfr on surface 'c' / 'b', + k off the ground, * parms.bdf,
                             * + slowdown / 100 */
    char pad364[0x38C - 0x364];
    s32 unk38C;             /* 0x38C: set to 1 by SnowBoarder_ctor */
    char pad390[0x3A4 - 0x390];
    SnowBVec3 startPos;     /* 0x3A4: spawn point: ctor args (x, z), y = ground height at (x, z) + a RoData offset */
    char pad3B0[0x3C8 - 0x3B0];
    SnowBVec3 dir;          /* 0x3C8: unit forward direction (B520 nudge axis; dotted with vel in updateDyn) */
    char pad3D4[0x3EC - 0x3D4];
    SnowBChild child;       /* 0x3EC: the "SnowBChild" sub-object and its ground-handling Parms */
    SnowBoarderParms parms; /* 0x540: "SnowBoarder::Parms" (the ctor builds it in place) */
    char *input;            /* 0x800: input object (SnowBoarder_ctor arg 4: the pad / replay / AI input built
                             * by replay_addInput); its +0x10 word carries the direction bits 4 / 8 (B520) */
    struct SnowAI *ai;      /* 0x804: the AI controller (game.uso SnowAI_ctor(0, this, ...), the ctor's last child);
                             * SnowBoarder_handleMsg forwards win / lose / timeout to SnowAI_onWin / onLose / onTimeout */
    char sub808[0xC];       /* 0x808: embedded 0xC-byte member, constructed in place and initialised
                             * with (ctor arg 1, ctor arg 2) */
    char pad814[0x840 - 0x814];
    struct RiderAnim *anim; /* 0x840: the RiderAnim (game.uso C48C) built by the ctor; the SnowAI run-out steps and
                             * SnowBoarder_handleMsg's ending ("en1" / "en2") call its state methods */
    char *unk844;           /* 0x844: child object A; its +0x30 float is negated on a mirror flip */
    s32 controller;         /* 0x848: ctor arg 1 (also passed to the 0x808 member). 0 = pad rider, 2 = CPU rider
                             * (game_setupVsCpu; SnowAI_update and SnowAI_onLose test == 2); 1 = other (not yet proven) */
    char pad84C[0x864 - 0x84C];
    char *unk864;           /* 0x864: child object B (may be NULL); transform at +0x30 */
    char pad868[0x89C - 0x868];
    SnowBVec3 prevVel;      /* 0x89C: vel saved at the start of SnowBoarder_procColl */
    char pad8A8[0x8C0 - 0x8A8];
    s32 unk8C0;             /* 0x8C0: 1 << character (one-hot mask, set by the ctor) */
    u32 character;          /* 0x8C4: rider character 1..8: ctor arg 2 = game_createPlayers' index + 1; SnowAI_update
                             * picks ms1..ms8 by it; 6 and 7 get special ctor handling and DDCC skips them */
    char pad8C8[0x8E0 - 0x8C8];
    f32 tint[4];            /* 0x8E0: r, g, b, a as 0..1 (ctor: colour byte / 255, alpha 255 / 164 / 117 / 70) */
    char pad8F0[0x8F4 - 0x8F0];
    SnowBRec3 *unk8F4[3];   /* 0x8F4: three record slots refilled from the table by DDCC/DF04/E014 */
    s16 unk900;             /* 0x900: table index used when (flags & SNOWB_FLAG_MIRROR) == 0 */
    s16 unk902;             /* 0x902: table index used when mirrored */
    char pad904[0x908 - 0x904];
    struct SnowBoarder *rival; /* 0x908: the other rider; game_setupVsCpu links the two riders through it,
                                * B520's catch-up compares positions with it */
    char pad90C[0x938 - 0x90C];
    s32 contactCount;       /* 0x938: collider contacts this frame: SnowBoarder_procColl clears it, SnowBCollider_onContact
                             * increments it; SnowBoarder_limitSpeed applies the wfr air drag only while it is 0 */
    SnowBVec3 contactNormal; /* 0x93C: summed contact directions, divided by contactCount in procColl */
    char pad948[0x960 - 0x948];
    s32 slowdown;           /* 0x960: percent: updateDyn zeroes edgeGrip, adds slowdown / 100 to friction and scales vel.xz
                             * by 1 - sfr; 100 with speed < 30 stops the rider (RiderAnim sets 100; st / sl parms drive it) */
    char pad964[0x968 - 0x964];
    f32 contactDepth;       /* 0x968: summed contact amounts, averaged like contactNormal */
    char pad96C[0x970 - 0x96C];
    SnowBVec3 spin;         /* 0x970: rotation rates; RiderAnim_clearRiderEdge_00011868 zeroes all three,
                             * RiderAnim_selectAnim tests |spin.x|, spin.z is negated on a mirror flip */
    char pad97C[0x990 - 0x97C];
    s32 crouch;             /* 0x990: input stick y < -0.75 (updateDyn); adds |spin.x| * k to friction */
    char pad994[0x9A0 - 0x994];
    s16 groundSurface;      /* 0x9A0: surface code of the ground below (procColl ground query) */
    s16 surface;            /* 0x9A2: groundSurface while close enough to the ground, else 0. updateDyn: 'a' (0x61) plain,
                             * 'b' / 'c' scale by ded, dfr / ied, ifr, 0 (airborne) zeroes the edge terms */
    s16 contactSurface;     /* 0x9A4: surface code of the last object contact */
    char pad9A6[0x9A8 - 0x9A6];
    s32 groundFlags;        /* 0x9A8: ground attribute bits; 0x400 switches the sub808 member on */
    char pad9AC[0x9CC - 0x9AC];
    s32 useDParms;          /* 0x9CC: input bit 0x400 held: updateDyn uses the d* parms (dbf, del, dts) instead of n* */
    f32 groundDist;         /* 0x9D0: distance to the ground below (a RoData default when nothing is found) */
    char pad9D4[0x9F8 - 0x9D4];
    f32 edgeFriction;       /* 0x9F8: child parms efr; 0 off the ground */
    f32 edgeGrip;           /* 0x9FC: nel / del, * ied / ded on 'c' / 'b', * parms.edf; 0 off the ground or while slowed */
    f32 turnRate;           /* 0xA00: res * nts (or dts with useDParms) */
    char padA04[0xA10 - 0xA04];
    u16 hitFlags;           /* 0xA10: SnowBoarder_handleMsg msg 20 (bumped by an object) sets 0x1F0;
                             * RiderAnim_selectAnim picks a stumble anim while (hitFlags & 0x1F0) */
    char padA12[0xA14 - 0xA12];
    s32 health;             /* 0xA14: damage meter; RiderAnim_applyDamage_0000EDD4 subtracts dm1..dm4;
                             * game_setupVsCpu gives the CPU rider 10000000 */
    char padA18[0xA1C - 0xA18];
    f32 unkA1C;             /* 0xA1C: lateral wobble: msg 20 adds the bump along the side axis 0x3BC;
                             * selectAnim compares |unkA1C| with RiderAnim parms to choose a stumble side */
    char padA20[0xA38 - 0xA20];
    f32 unkA38;             /* 0xA38: cleared on a mirror flip */
    char padA3C[0xA54 - 0xA3C];
    s32 raceResult;         /* 0xA54: SnowBoarder_handleMsg: 1 win (msg 1000), 2 lose (1001), 3 timeout (1003),
                             * 4 (1002), 5 (1004), 6 ending (1005..1007) */
    u32 flags;              /* 0xA58: SNOWB_FLAG_* */
    char padA5C[0xA78 - 0xA5C];
    f32 maxSpeed;           /* 0xA78: speed cap; eased toward baseMaxSpeed (+-catch-up) by B520 */
    char padA7C[0xA8C - 0xA7C];
} SnowBoarder;

#define SNOWB_FLAG_08     0x08  /* SnowBoarder_func_0000E124: selects the notify-call form */
#define SNOWB_FLAG_20     0x20  /* SnowBoarder_func_0000E124: gates the 180-degree re-orient */
#define SNOWB_FLAG_MIRROR 0x80  /* left/right stance flip, toggled by SnowBoarder_func_0000E124 */
#define SNOWB_FLAG_NUDGE  0x100 /* SnowBoarder_limitSpeed: input bits 4 / 8 push the rider along dir */
#define SNOWB_FLAG_CATCHUP 0x20000 /* SnowBoarder_limitSpeed: maxSpeed gets +-k when the rival is > k' ahead / behind in z */

/* compile-time offset checks (array size -1 => error) */
#define SNOWB_ASSERT_OFFSET(field, off) \
    typedef char snowb_assert_##off[(int)&((SnowBoarder *)0)->field == (off) ? 1 : -1]
SNOWB_ASSERT_OFFSET(pos, 0x0B4);
SNOWB_ASSERT_OFFSET(vel, 0x318);
SNOWB_ASSERT_OFFSET(speed, 0x348);
SNOWB_ASSERT_OFFSET(unk38C, 0x38C);
SNOWB_ASSERT_OFFSET(startPos, 0x3A4);
SNOWB_ASSERT_OFFSET(dir, 0x3C8);
SNOWB_ASSERT_OFFSET(parms, 0x540);
SNOWB_ASSERT_OFFSET(parms.wfr, 0x6B0);
SNOWB_ASSERT_OFFSET(parms.nts, 0x6F8);
SNOWB_ASSERT_OFFSET(parms.max, 0x758);
SNOWB_ASSERT_OFFSET(input, 0x800);
SNOWB_ASSERT_OFFSET(ai, 0x804);
SNOWB_ASSERT_OFFSET(anim, 0x840);
SNOWB_ASSERT_OFFSET(sub808, 0x808);
SNOWB_ASSERT_OFFSET(unk844, 0x844);
SNOWB_ASSERT_OFFSET(controller, 0x848);
SNOWB_ASSERT_OFFSET(unk864, 0x864);
SNOWB_ASSERT_OFFSET(unk8C0, 0x8C0);
SNOWB_ASSERT_OFFSET(character, 0x8C4);
SNOWB_ASSERT_OFFSET(tint, 0x8E0);
SNOWB_ASSERT_OFFSET(unk8F4, 0x8F4);
SNOWB_ASSERT_OFFSET(unk900, 0x900);
SNOWB_ASSERT_OFFSET(unk902, 0x902);
SNOWB_ASSERT_OFFSET(rival, 0x908);
SNOWB_ASSERT_OFFSET(friction, 0x360);
SNOWB_ASSERT_OFFSET(child, 0x3EC);
SNOWB_ASSERT_OFFSET(child.parms.efr, 0x480);
SNOWB_ASSERT_OFFSET(child.parms.nbf, 0x498);
SNOWB_ASSERT_OFFSET(child.parms.res, 0x4F8);
SNOWB_ASSERT_OFFSET(prevVel, 0x89C);
SNOWB_ASSERT_OFFSET(contactCount, 0x938);
SNOWB_ASSERT_OFFSET(contactNormal, 0x93C);
SNOWB_ASSERT_OFFSET(slowdown, 0x960);
SNOWB_ASSERT_OFFSET(contactDepth, 0x968);
SNOWB_ASSERT_OFFSET(crouch, 0x990);
SNOWB_ASSERT_OFFSET(groundSurface, 0x9A0);
SNOWB_ASSERT_OFFSET(surface, 0x9A2);
SNOWB_ASSERT_OFFSET(groundFlags, 0x9A8);
SNOWB_ASSERT_OFFSET(useDParms, 0x9CC);
SNOWB_ASSERT_OFFSET(groundDist, 0x9D0);
SNOWB_ASSERT_OFFSET(edgeFriction, 0x9F8);
SNOWB_ASSERT_OFFSET(edgeGrip, 0x9FC);
SNOWB_ASSERT_OFFSET(turnRate, 0xA00);
SNOWB_ASSERT_OFFSET(spin, 0x970);
SNOWB_ASSERT_OFFSET(hitFlags, 0xA10);
SNOWB_ASSERT_OFFSET(health, 0xA14);
SNOWB_ASSERT_OFFSET(unkA1C, 0xA1C);
SNOWB_ASSERT_OFFSET(raceResult, 0xA54);
SNOWB_ASSERT_OFFSET(unkA38, 0xA38);
SNOWB_ASSERT_OFFSET(flags, 0xA58);
SNOWB_ASSERT_OFFSET(maxSpeed, 0xA78);
typedef char snowb_assert_parm_size[sizeof(SnowBParm) == 0x18 ? 1 : -1];
typedef char snowb_assert_child_size[sizeof(SnowBChild) == 0x154 ? 1 : -1];
typedef char snowb_assert_parms_size[sizeof(SnowBoarderParms) == 0x2C0 ? 1 : -1];
typedef char snowb_assert_size[sizeof(SnowBoarder) == 0xA8C ? 1 : -1];

#endif /* GAME_SNOWBOARDER_H */
