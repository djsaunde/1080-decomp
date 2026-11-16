/* IDO 7.1 NCC (C++) NM-only comparison donor for SnowBoarder_procColl: the NM C body lifted under NCC + one parameter empty test from a fz sweep (agent-c run 134, cfe 77.39; a second parameter empty test from round 2 gives 79.35).
 * agent-g run 152 (79.55 -> 85.17): arg 2 of the 0x38 lookup call is a 4-byte struct BY VALUE (target `sw a1,140(sp)` + `sw a1,4(sp)` in the jal delay), the
 * counter test is a post-increment compare on the old value (`slti v1,151`), the func_00008A7C sites are per-site symbols (target re-materialises
 * lui/addiu per call, one shared symbol is CSE-spilled to sp+52), the 0x2C mode word reads through a data symbol (lw %lo folded), sp40 declared first.
 * EXACT (agent-g run 236): spliced over the bootup_uso.c placeholder on both build paths via REPLACE_FUNC_BODY; the
 * 0x930..0x948 constants are bu_c234_k9xx pins (target relocs .L00000930 / SndHandle_triggerEvent3_00000940 + addend).
 * agent-g run 475: field form over include/game/snowboarder.h (included by relative path; the header's asserts are
 * C-only). Same bytes: every access keeps its width / type (a mismatched read stays `*(T *)&rider->field`).
 * agent-g run 558: the 151-frame airborne reset asks ItemMgr_queryType(MarkerMgr, "mrk", pos, 0) for the nearest
 * generic marker (sTag_mrk_8A78 = Data 0x8A78, D_C234_markerMgr = Data 0x7768 Sym 302) and moves the rider 200 above its
 * CourseMarker.pos. The .o's tag reloc is now sTag_mrk_8A78 + 0 (was func_00008A40 + 0x38); linked bytes identical. */
typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef unsigned short u16;
typedef int s32; typedef unsigned int u32; typedef long long s64; typedef unsigned long long u64;
typedef float f32; typedef double f64;
#include "../../include/game/snowboarder.h"
#include "../../include/game/input.h" /* GameFlow (InputMgr) */
#include "../../include/game/course.h" /* CourseMarker (the marker query result) */
#include "../../include/gdl/gl_data.h" /* gCourseColl = Bss+0x50 Sym 451 (the course CollShape *) */
#include "../../include/gdl/coll.h"    /* CollShape (C++-safe since agent-b run 553) */
#define COURSE_COLL (*(CollShape **)&gCourseColl)
/* GameFlow through gInputMgr_c234_g8: the token is pinned at 8 (undefined_syms_auto.txt), so the object starts 8 bytes below it; the reloc names Bss+0xF8 (GameFlow) +0x34 = nextFlowMode (uso-data-sites.py sites). */
#define C234_FLOW INPUT_MGR(gInputMgr_c234_g8[-8])
inline void zf(float &f, int z) { f = z; }
#undef NULL
#define NULL 0
typedef struct { f32 x, y, z; } V3C234;
typedef struct {
     s32 unk0;
     s32 unk4;
     s32 unk8;
     f32 d8;
     s16 dc;
     s16 de;
     s32 e0;
     f32 e4;
     f32 e8;
     f32 ec;
     f32 f0;
} C234Rec;
extern "C" s32 sTag_mrk_8A78;    /* Data 0x8A78: "mrk" (MARKER_MRK), passed by value */
extern "C" s32 D_C234_markerMgr; /* Data 0x7768 (Sym 302): the MarkerMgr (game_createMap: ItemMgr_ctor(0, "MarkerMgr", -1)) */
struct S4C234 { s32 v; };
extern "C" int bu_c234_sv(s32, S4C234, char *, int);
extern "C" {
extern float D_bu_ro_pool[];
extern double D_bu_ro_dpool[];
extern f32 D_00000128, kRo_SnowBoarder_ctor_868, kRo_SnowBoarder_ctor_86C, kRo_SnowBoarder_ctor_870, kRo_SnowBoarder_ctor_874, kRo_SnowBoarder_ctor_878, kRo_SnowBoarder_ctor_87C, kRo_SnowBoarder_ctor_880, kRo_SnowBoarder_ctor_884, kRo_SnowBoarder_ctor_888, kRo_SnowBoarder_ctor_88C, kRo_SnowBoarder_ctor_890, kRo_SnowBoarder_ctor_894, kRo_SnowBoarder_ctor_898, kRo_SnowBoarder_ctor_89C, kRo_SnowBoarder_ctor_8A0, kRo_SnowBoarder_ctor_8A4, kRo_SnowBoarder_ctor_8A8, kRo_SnowBoarder_ctor_8AC, kRo_SnowBoarder_ctor_8B0, kRo_SnowBoarder_ctor_8B4, D_000008B8, D_000008BC, D_000008E0, D_000008F0, D_00000910, D_00000920, D_00000924, D_00000928, kRo_SnowBoarder_procColl_938, kRo_SnowBoarder_procColl_93C, D_00000940, D_00000944, D_00000948, D_0000094C;
extern f32 D_00000128, kRo_SnowBoarder_ctor_868, kRo_SnowBoarder_ctor_86C, kRo_SnowBoarder_ctor_870, kRo_SnowBoarder_ctor_874, kRo_SnowBoarder_ctor_878, kRo_SnowBoarder_ctor_87C, kRo_SnowBoarder_ctor_880, kRo_SnowBoarder_ctor_884, kRo_SnowBoarder_ctor_888, kRo_SnowBoarder_ctor_88C, kRo_SnowBoarder_ctor_890, kRo_SnowBoarder_ctor_894, kRo_SnowBoarder_ctor_898, kRo_SnowBoarder_ctor_89C, kRo_SnowBoarder_ctor_8A0, kRo_SnowBoarder_ctor_8A4, kRo_SnowBoarder_ctor_8A8, kRo_SnowBoarder_ctor_8AC, kRo_SnowBoarder_ctor_8B0, kRo_SnowBoarder_ctor_8B4, D_000008B8, D_000008BC, D_000008E0, D_000008F0, D_00000910, D_00000920, D_00000924, D_00000928, kRo_SnowBoarder_procColl_938, kRo_SnowBoarder_procColl_93C, D_00000940, D_00000944, D_00000948, D_0000094C;
extern f32 D_00000128, kRo_SnowBoarder_ctor_868, kRo_SnowBoarder_ctor_86C, kRo_SnowBoarder_ctor_870, kRo_SnowBoarder_ctor_874, kRo_SnowBoarder_ctor_878, kRo_SnowBoarder_ctor_87C, kRo_SnowBoarder_ctor_880, kRo_SnowBoarder_ctor_884, kRo_SnowBoarder_ctor_888, kRo_SnowBoarder_ctor_88C, kRo_SnowBoarder_ctor_890, kRo_SnowBoarder_ctor_894, kRo_SnowBoarder_ctor_898, kRo_SnowBoarder_ctor_89C, kRo_SnowBoarder_ctor_8A0, kRo_SnowBoarder_ctor_8A4, kRo_SnowBoarder_ctor_8A8, kRo_SnowBoarder_ctor_8AC, kRo_SnowBoarder_ctor_8B0, kRo_SnowBoarder_ctor_8B4, D_000008B8, D_000008BC, D_000008E0, D_000008F0, D_00000910, D_00000920, D_00000924, D_00000928, kRo_SnowBoarder_procColl_938, kRo_SnowBoarder_procColl_93C, D_00000940, D_00000944, D_00000948, D_0000094C;
extern f32 D_00000128, kRo_SnowBoarder_ctor_868, kRo_SnowBoarder_ctor_86C, kRo_SnowBoarder_ctor_870, kRo_SnowBoarder_ctor_874, kRo_SnowBoarder_ctor_878, kRo_SnowBoarder_ctor_87C, kRo_SnowBoarder_ctor_880, kRo_SnowBoarder_ctor_884, kRo_SnowBoarder_ctor_888, kRo_SnowBoarder_ctor_88C, kRo_SnowBoarder_ctor_890, kRo_SnowBoarder_ctor_894, kRo_SnowBoarder_ctor_898, kRo_SnowBoarder_ctor_89C, kRo_SnowBoarder_ctor_8A0, kRo_SnowBoarder_ctor_8A4, kRo_SnowBoarder_ctor_8A8, kRo_SnowBoarder_ctor_8AC, kRo_SnowBoarder_ctor_8B0, kRo_SnowBoarder_ctor_8B4, D_000008B8, D_000008BC, D_000008E0, D_000008F0, D_00000910, D_00000920, D_00000924, D_00000928, kRo_SnowBoarder_procColl_938, kRo_SnowBoarder_procColl_93C, D_00000940, D_00000944, D_00000948, D_0000094C;
extern f32 D_00000128, kRo_SnowBoarder_ctor_868, kRo_SnowBoarder_ctor_86C, kRo_SnowBoarder_ctor_870, kRo_SnowBoarder_ctor_874, kRo_SnowBoarder_ctor_878, kRo_SnowBoarder_ctor_87C, kRo_SnowBoarder_ctor_880, kRo_SnowBoarder_ctor_884, kRo_SnowBoarder_ctor_888, kRo_SnowBoarder_ctor_88C, kRo_SnowBoarder_ctor_890, kRo_SnowBoarder_ctor_894, kRo_SnowBoarder_ctor_898, kRo_SnowBoarder_ctor_89C, kRo_SnowBoarder_ctor_8A0, kRo_SnowBoarder_ctor_8A4, kRo_SnowBoarder_ctor_8A8, kRo_SnowBoarder_ctor_8AC, kRo_SnowBoarder_ctor_8B0, kRo_SnowBoarder_ctor_8B4, D_000008B8, D_000008BC, D_000008E0, D_000008F0, D_00000910, D_00000920, D_00000924, D_00000928, kRo_SnowBoarder_procColl_938, kRo_SnowBoarder_procColl_93C, D_00000940, D_00000944, D_00000948, D_0000094C;
extern char D_00000000;
extern void bu_b75c_mvf(char *, char *, f32, SnowBoarder *);
extern int func_00000000(...);
extern void func_00000008(...);
extern float func_411c_rf(void);
extern void func_00008A7C(...);
extern char gInputMgr_c234_g8[];
extern void bu_c234_v5(char *, char *, f64, f64, f64);
extern char sStr_Player_Pos_Reset_8A7C[], sFmt_num_8A94[], bu_c234_k2[], sStr_SBProcColl_8AA4[], sStr_SBProcColl_8AB0[];
extern void func_00000000_p4d(void *, s32, f64, f32);
extern void func_00000000_p4d(void *, s32, f64, f32);
extern s32 unk0;
extern s32 unk4;
extern s32 unk8;
extern f32 d8;
extern s16 dc;
extern s16 de;
extern s32 e0;
extern f32 e4;
extern f32 e8;
extern f32 ec;
extern f32 f0;
extern f64 kRo_SnowBoarder_procColl_930;
extern f32 kRo_SnowBoarder_procColl_940;
extern f32 kRo_SnowBoarder_procColl_944;
extern f32 kRo_SnowBoarder_procColl_948;
extern s32 D_000081C0[];
}
extern "C" void SnowBoarder_procColl(SnowBoarder *rider) {
    /* agent-g run 232: frame fit (NCC homes every used local in declaration order, top down): 20 used scalars in the
     * target's gaps (1 / spCC / 3 / spB4 / 9 / sp8C / 1 / sp7C / 2 / sp68 / 2 / sp54 / 2 / sp40) give frame 0xf8 and
     * every struct offset of the target; the block products are function-scope ga..ge, temp_a1 / temp_v0 de-named */
    s16 temp_v0_2;
    C234Rec spCC;
    f32 temp_f0;
    f32 temp_f12;
    f32 temp_f12_2;
    V3C234 spB4;
    f32 temp_f16;
    f32 temp_f2;
    f32 temp_f2_3;
    f32 var_f2;
    f32 var_f2_2;
    s32 temp_v0_4;
    f32 temp_f14; /* agent-g run 233: takes var_t0's home slot (0x934 held in a variable: tail f12/f14/f16 order) */
    s32 sp3Ck; /* agent-g run 235: sp3C's old slot, now the first call's argument 0 (evaluated before sp3C's def) */
    s32 var_v0;
    S4C234 sp8C;
    char *temp_v0_3;
    V3C234 sp7C;
    char *var_v0_2;
    f32 ga;
    V3C234 sp68;
    f32 gb;
    f32 gc;
    V3C234 sp54;
    f32 gd;
    f32 ge;
    V3C234 sp40;
    s32 temp_a1_2;
    s32 temp_v1_2;
    char *sp3C; /* agent-g run 235: last home = the target's sp+0x3C (kit CCXLVIII) */


#define temp_v0 (rider->msgCooldown)
    if (temp_v0 != 0) {
        rider->msgCooldown = (s32) (temp_v0 - 1);
    }
    spB4 = *(V3C234 *)&rider->dnode.vqs.trans[0];
    rider->prevPos.x = spB4.x;
    rider->prevPos.y = spB4.y;
    rider->prevPos.z = spB4.z;
    spB4 = *(V3C234 *)&rider->dnode.vel[0];
    rider->prevVel.x = spB4.x;
    rider->prevVel.y = spB4.y;
    rider->prevVel.z = spB4.z;
    spCC.f0 = 0.0f;
    spCC.ec = 0.0f;
    spCC.e4 = 0.0f;
    spCC.e8 = 1.0f;
    rider->groundDist = D_bu_ro_pool[0x920 / 4]; /* agent-g run 231: above the 0x9A2 store (as1 line order: the pool load issues before the spCC stores) */
    rider->surface = 0; if (rider) {}
    /* agent-g run 235: sp3C is a frozen-home named pointer (saved at its own home across the call, not in the
     * temp area): the folded read of *rider stops uopt copy-propagating it; defined in the argument list after
     * argument 0 so its addiu follows the D_00000000 lui (99.84 -> 99.85, spill slot = target) */
    if ((var_v0 = func_00000000((sp3Ck = *(s32 *)&gCourseColl), sp3C = (char *)rider->dnode.cvqs.trans + 0 * (int)(*(char *)rider), 0x457A0000, &spCC)) != 0) {
        rider->groundSurface = func_00000000(COURSE_COLL->shape, spCC.dc);
        if (spCC.d8 < D_bu_ro_pool[0x924 / 4]) {
            rider->surface = rider->groundSurface;
        } else {
            rider->surface = 0;
        }
        rider->groundFlags = func_00000000(COURSE_COLL->shape, spCC.dc);
        rider->groundDist = spCC.d8;
        /* agent-g run 231: the plane distance on its own line before the test (spCC.f0 then loads after it, as1 line
         * order), operands K * (dc * e4 + (e0 + K2) * e8 + e4' * ec) + 4 in the target's load / multiply order: 95.45 -> 98.17 */
        /* agent-g run 232: the scaled sum alone on the line, `+ 4.0f` in the test (product first in the add, spCC.f0 loads before it): 98.68 -> 99.10 */
        /* agent-g run 233: the scaled sum in temp_f0 (target mul.s into f0) */
        temp_f0 = (D_bu_ro_pool[0x128 / 4] * (((rider->dnode.cvqs.trans[0] * spCC.e4) + ((rider->dnode.cvqs.trans[1] + D_bu_ro_pool[0x928 / 4]) * spCC.e8) + (rider->dnode.cvqs.trans[2] * spCC.ec))));
        if ((spCC.f0 < temp_f0 + 4.0f) ||(rider->contactCount != 0)) {
            rider->onGround = 1;
            spB4 = *(V3C234 *)&spCC.e4;
            rider->groundNormal.x = spB4.x;
            rider->groundNormal.y = spB4.y;
            rider->groundNormal.z = spB4.z;
            rider->groundPlaneD = spCC.f0;
            /* agent-g run 233: the compare stored directly -- the 0/1 is a ugen temp (t0), not a uopt-coloured
             * local (v0); this moves the ugen round-robin on by one, as the target's t1/t2 below */
            rider->groundGentle = (kRo_SnowBoarder_procColl_930 <(f64) rider->groundNormal.y);
        } else {
            rider->onGround = 0;
            rider->groundNormal.x = (f32) 0;
            rider->groundNormal.z = (f32) 0;
            rider->groundPlaneD = (f32) 0;
            rider->groundGentle = 0;
            rider->groundNormal.y = 1.0f;
        }
        if ((rider->groundDist < (f32) 0) && (spCC.e8 > (f32) 0)) {
            rider->dnode.vel[1] = 10.0f;
        }
        if ((rider->contactCount != 0) && (rider->dnode.speed > 10.0f)) {
#define temp_v1 (rider->groundSurface) /* run 184: de-named (lever-sweep) */
            var_v0 = 0;
            if (temp_v1 == 0x64) {
                rider->shake = 0x64;
                var_v0 = 2;
            } else if (temp_v1 == 0x63) {
                rider->shake = 0x46;
                var_v0 = 1;
            }
            if (var_v0 != 0) {
                temp_f2 = rider->edge;
                if (temp_f2 < 0.0f) {
                    temp_f0 = -temp_f2;
                } else {
                    temp_f0 = temp_f2;
                }
                var_v0 = (s32) ((f32) var_v0 + (temp_f0 * 3.0f)); /* agent-g run 233: index in var_v0 (target mfc1 v0, coloured, not a ugen temp) */
                func_00000000(rider->input, D_000081C0[var_v0], 2);
            }
        }
        /* Run 109 (agent-c, 69.25 -> 70.94): target block order is default,
         * 0x61, 0x62 (flat goto ladder); the 0x940/0x944/0x948 pool reads are
         * per-address scalars (D_bu_ro_pool[] here CSEs the base, -0.1). */
        temp_v0_2 = rider->groundSurface;
        if (temp_v0_2 == 0x61) goto c61;
        if (temp_v0_2 == 0x62) goto c62;
        rider->dnode.surfImp = (f32) kRo_SnowBoarder_procColl_938;
        rider->dnode.surfSpd = (f32) kRo_SnowBoarder_procColl_93C;
        rider->dnode.surfVsc = kRo_SnowBoarder_procColl_940; /* run 200: 288 / 28C / 290 together in both arms (statement-order grid) */
        rider->groundColor[3] = 0;
        rider->groundColor[2] = 0;
        rider->groundColor[1] = 0;
        rider->groundColor[0] = 0;
        rider->impactScale = (f32) rider->parms.icf.value;
        goto cend;
c61:
        rider->dnode.surfImp = (f32) rider->dnode.parms.imp.value;
        rider->dnode.surfSpd = (f32) rider->dnode.parms.spd.value;
        rider->dnode.surfVsc = (f32) rider->dnode.parms.vsc.value;
        rider->impactScale = (f32) rider->parms.snf.value;
        func_00000000(COURSE_COLL->shape, spCC.de, (char *)&rider->groundColor);
        goto cend;
c62:
        rider->dnode.surfImp = 0.0f;
        rider->dnode.surfSpd = kRo_SnowBoarder_procColl_944;
        rider->dnode.surfVsc = kRo_SnowBoarder_procColl_948;
        rider->impactScale = (f32) rider->parms.dsf.value;
        func_00000000(COURSE_COLL->shape, spCC.de, (char *)&rider->groundColor);
cend:
        rider->noGroundFrames = 0;
    } else {
        rider->onGround = 0;
        rider->groundNormal.x = (f32) 0;
        rider->groundNormal.z = (f32) 0;
        rider->groundPlaneD = (f32) 0;
        rider->groundNormal.y = 1.0f;
        /* agent-g run 235: the call result held in var_v0 with a folded use at the compare: its v0 LR now overlaps
         * the post-increment's old value, which colours v1 like the target (99.85 -> 99.87) */
        if ((rider->noGroundFrames)++ + 0 * var_v0 >= 0x97) {
            /* ItemMgr_queryType(gMarkerMgr, "mrk", pos, 0): the nearest generic course marker */
            sp8C.v = sTag_mrk_8A78;
            temp_v0_3 = (char *)(bu_c234_sv(D_C234_markerMgr, sp8C, sp3C, 0));
            if (temp_v0_3 != 0) {
                sp7C = *(V3C234 *)((CourseMarker *)temp_v0_3)->pos;
                sp7C.y += 200.0f;
                func_00000000(rider, &sp7C);
                rider->noGroundFrames = 0;
                func_00000000(sStr_Player_Pos_Reset_8A7C + 0);
                bu_c234_v5(sFmt_num_8A94 + 0x18, bu_c234_k2 + 0x14, (f64) sp7C.x, (f64) sp7C.y, (f64) sp7C.z);
            }
        }
    }
    rider->contactDepth = 0.0f;
    rider->contactCount = 0;
    zf(rider->contactNormal.x, 0); /* run 196: int-zero inline (kit CLVIII): the target's own f0 zero */
    zf(rider->contactNormal.y, 0);
    zf(rider->contactNormal.z, 0);
    func_00000000(sStr_SBProcColl_8AA4 + 0x28);
    func_00000000(rider);
    func_00000000(sStr_SBProcColl_8AB0 + 0x34);
    if (rider->groundFlags & 0x400) {
        func_00000000((char *)&rider->sound, 1);
        var_f2 = D_bu_ro_pool[0x94C / 4];
    } else {
        func_00000000((char *)&rider->sound, 0);
        var_f2 = 1.0f;
    }
    if (C234_FLOW.nextFlowMode != 2) {
        temp_f0 = rider->unk91C;
        rider->unk91C = (f32) ((f64) temp_f0 + ((f64) (var_f2 - temp_f0) * D_bu_ro_dpool[0x950 / 8]));
        func_00000000(rider->light, *(s32 *)&rider->unk91C);
    }
    temp_v0_4 = rider->contactCount;
    if (temp_v0_4 != 0) {
        /* agent-g run 236: the divisor is a named variable kept by a folded memory read (kit CCXLVIII), so uopt
         * colours it (f2) instead of leaving the cvt to ugen (f0) */
        temp_f2_3 = (f32) (temp_v0_4 + 0 * (int)(*(char *)rider));
#define temp_f2_2 temp_f2_3
        var_v0_2 = (char *)&rider->contactNormal;
        rider->contactNormal.x = (f32) (rider->contactNormal.x / temp_f2_2);
        rider->contactNormal.y = (f32) (rider->contactNormal.y / temp_f2_2);
        rider->contactNormal.z = (f32) (rider->contactNormal.z / temp_f2_2); /* run 200: 0x944 before 0x968 (target load order; div block word-for-word) */
        rider->contactDepth = (f32) (rider->contactDepth / temp_f2_2); /* agent-g run 236: no BB split here: temp_f12 shares the divisor's block, so it cannot take f2 (f12), ga gets f2 */
        temp_f12 = rider->contactDepth;
        if (temp_f12 < (f32) 0) {
            var_v0_2 = (char *)&rider->contactNormal; if (1) {} /* run 182 (lever-sweep bbsplit, +1 word toward the target) */
            { /* run 156: held products, stored afterwards (kit LXXXVI) */
                /* agent-g run 232: the target's mov.s copy (z scaled by a copy in f0, y's product back into f12):
                 * the copy is taken before temp_f12 is overwritten by the y product, so uopt cannot propagate it */
                temp_f0 = temp_f12;
                ga = ((V3C234 *)var_v0_2)->x * temp_f12;
                temp_f12 = ((V3C234 *)var_v0_2)->y * temp_f12;
                gb = ((V3C234 *)var_v0_2)->z * temp_f0;
                sp68.x = ga; sp68.y = temp_f12; sp68.z = gb;
            }
            spB4 = sp68;
            sp54 = spB4;
            rider->penetration.x = sp54.x;
            rider->penetration.y = sp54.y;
            rider->penetration.z = sp54.z;
        } else {
            rider->penetration.x = (f32) 0;
            rider->penetration.y = (f32) 0;
            rider->penetration.z = (f32) 0;
        }
        spB4 = *(V3C234 *)var_v0_2;
        rider->lastContactNormal.x = spB4.x;
        rider->lastContactNormal.y = spB4.y;
        rider->lastContactNormal.z = spB4.z;
        var_f2_2 = (rider->dnode.lastVel[0] * rider->contactNormal.x) + (rider->dnode.lastVel[1] * rider->contactNormal.y) + (rider->dnode.lastVel[2] * rider->contactNormal.z);
        if (var_f2_2 > 0.0f) {
            var_f2_2 = 0.0f;
        }
        temp_f0 = -var_f2_2; if (1) {} /* run 182 (lever-sweep bbsplit) */
        { /* run 156: held products */
            gc = ((V3C234 *)var_v0_2)->x * temp_f0;
            gd = ((V3C234 *)var_v0_2)->y * temp_f0;
            ge = ((V3C234 *)var_v0_2)->z * temp_f0; /* run 200: no longer volatile (run 182's +2-word lever is surplus after the 0x944 order fix) */
            sp40.x = gc; sp40.y = gd; sp40.z = ge;
        }
        spB4 = sp40;
        sp54 = spB4;
        rider->impactVel.x = sp54.x;
        rider->impactVel.y = sp54.y;
        rider->impactVel.z = sp54.z;
        rider->airFrames = 0; /* run 200: after the 0x934 store (statement-order grid) */
    } else {
        rider->penetration.x = (f32) 0;
        rider->penetration.y = (f32) 0;
        rider->airFrames = (s32) (rider->airFrames + 1);
        rider->penetration.z = (f32) 0;
        rider->impactVel.x = (f32) 0;
        rider->impactVel.y = (f32) 0;
        rider->impactVel.z = (f32) 0;
    }
    temp_f2_3 = rider->impactVel.x;
    temp_f12_2 = rider->impactVel.y;
    temp_f16 = rider->impactScale;
    /* agent-g run 232: each dot product in temp_f0 (the target's sums are all in f0, which pushes 0x910 out of f0), then `sum * scale` */
    /* agent-g run 234: 0x934 assigned inside its first use (target loads 0x3C4 before 0x934) */
    temp_f0 = (rider->dnode.axes[1][0] * temp_f2_3) + (rider->dnode.axes[1][1] * temp_f12_2) + (rider->dnode.axes[1][2] * (temp_f14 = rider->impactVel.z));
    rider->bumpSide = rider->bumpSide + (temp_f0 * temp_f16);
    temp_f0 = (rider->dnode.axes[0][0] * temp_f2_3) + (rider->dnode.axes[0][1] * temp_f12_2) + (rider->dnode.axes[0][2] * temp_f14);
    rider->bumpUp = rider->bumpUp + (temp_f0 * temp_f16);
    temp_f0 = (rider->dnode.axes[2][0] * temp_f2_3) + (rider->dnode.axes[2][1] * temp_f12_2) + (rider->dnode.axes[2][2] * temp_f14);
    rider->bumpFwd = rider->bumpFwd - temp_f0;
}
