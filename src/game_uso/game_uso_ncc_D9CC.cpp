/* IDO 7.1 NCC (C++) donor for RiderAnim_selectAnim_0000D9CC (0x830 / 524 words).
 *
 * Per-frame landing/turn-state classifier. Under NCC two edg spellings fix
 * the cfe "andi temp coalesced into a1" t-ring cascade (kit XXI #2): the
 * 0x108 field is re-read through memory (no value local) in the `| 0x60000`
 * store and the `!= 0` test, and the three `bump * hitScale` products of
 * the 0xA1C..0xA24 block are written field-first (edg keeps source operand
 * order).
 * The first gate is TWO nested ifs (x > 30.0f, then the 0x1F0 mask), not one
 * `&&`, and the early goto lands after BOTH closing braces. The mask test and
 * the 0x22C test branch to the inner end label, which is directly followed by
 * the outer one, so as1 finds no instruction to steal and keeps `beqz; nop` /
 * `bc1f; nop`; only the first gate (whose target starts with the 500.0f lui)
 * becomes `bc1fl` with the lui duplicated. Found with a branch-offset-aware
 * diff (docs/IDO_CODEGEN.md kit XXV); the masked diffs showed it as a
 * 4-word as1 cap.
 * The two doubles are data-symbol reads whose addends carry the %lo offsets
 * (0x1F8/0x200; the game_uso block is extracted unlinked); the host
 * placeholder in game_uso.c re-emits the .rodata words.
 * Spliced via REPLACE_FUNC_BODY into both trees. */
extern "C" {
typedef int s32; typedef unsigned short u16; typedef float f32; typedef double f64;
typedef short s16; typedef unsigned char u8; typedef unsigned int u32;
#include "../../include/game/rideranim.h"
#include "../../include/game/game_uso_data.h"
#define RIDER(p) ((SnowBoarder *)(p))
int game_uso_func_077C44(...);
void RiderAnim_clearState_0000D5DC(char *a0);
void RiderAnim_queueState_0000D5F8(char *a0, Pair2 pair, int a3);
void RiderAnim_endTrick_0000D63C(char *a0, int a1);
void game_uso_func_07C0FC(...);
void RiderAnim_setHitKind_0000EDCC(int *a0, int a1);
void RiderAnim_applyDamage_0000EDD4(int *a0, unsigned int a1, char *a2);
void RiderAnim_hitFeedback_0001189C(int *a0, int a1, char *a2);
extern char game_uso_D_807FF478;
extern char game_uso_D_807FF480;
extern char game_uso_D_807FF3B8;
extern char game_uso_D_807FF68C;
extern char game_uso_D_807FFB18;
extern char game_uso_D_807FFB20;
void RiderAnim_selectAnim_0000D9CC(char *arg0) {
    s32 var_a3;
    s32 sp30;
    char *sp2C;
    s32 sp28;
    f32 temp_f0;
    f32 var_f0;
    f32 temp_f2;
    f32 temp_f2_2;
    f32 var_f0_4;
    f32 var_f12;
    f32 var_f2;
    f32 *temp_v0;
    char *temp_v0_2;
    f32 *temp_v0_3;
    char *var_v1;

    RA(arg0)->selectedAnim = 0;
    sp30 = 1;
    sp2C = 0;
    sp28 = 0;
    var_v1 = (char *)RA(arg0)->rider;
    var_a3 = 0;
    if (RIDER(var_v1)->dnode.speed > 30.0f) { if (RIDER(var_v1)->hitFlags & 0x1F0) {
        temp_f2 = RIDER(var_v1)->bumpSide;
        if (temp_f2 < 0.0f) {
            var_f0 = -temp_f2;
        } else {
            var_f0 = temp_f2;
        }
        if (RA(arg0)->parms.b2f.value.f < var_f0) {
            sp30 = 0;
            sp28 = 2;
            RiderAnim_setHitKind_0000EDCC((int *)arg0, 1);
            var_a3 = 3;
            if (RA(arg0)->rider->bumpSide > 0.0f) {
                RA(arg0)->selectedAnim = (s32) (RA(arg0)->animSet | 0x16);
                sp2C = (char *)1;
            } else {
                RA(arg0)->selectedAnim = (s32) (RA(arg0)->animSet | 0x17);
                sp2C = (char *)3;
            }
            var_v1 = (char *)RA(arg0)->rider;
            goto after_24;
        }
        if (temp_f2 < 0.0f) {
            var_f0 = -temp_f2;
        } else {
            var_f0 = temp_f2;
        }
        if (RA(arg0)->parms.b1f.value.f < var_f0) {
            sp30 = 0;
            RiderAnim_setHitKind_0000EDCC((int *)arg0, 4);
            var_v1 = (char *)RA(arg0)->rider;
            var_a3 = 2;
            temp_f2_2 = RIDER(var_v1)->edge;
            if (temp_f2_2 < 0.0f) {
                var_f0 = -temp_f2_2;
            } else {
                var_f0 = temp_f2_2;
            }
            if (*(f64 *)((char *)&game_uso_D_807FFB18 + kRoD_0p8_1F8) < (f64) var_f0) {
                if (temp_f2_2 <= 0.0f) {
                    RA(arg0)->selectedAnim = (s32) (RA(arg0)->animSet | 0xE);
                    sp2C = (char *)3;
                } else {
                    RA(arg0)->selectedAnim = (s32) (RA(arg0)->animSet | 0xF);
                    sp2C = (char *)1;
                }
            } else if (RIDER(var_v1)->bumpSide > 0.0f) {
                RA(arg0)->selectedAnim = (s32) (RA(arg0)->animSet | 0xC);
                sp2C = (char *)1;
            } else {
                RA(arg0)->selectedAnim = (s32) (RA(arg0)->animSet | 0xD);
                sp2C = (char *)3;
            }
            var_v1 = (char *)RA(arg0)->rider;
        }
    }
after_24:;
    }
    if (RIDER(var_v1)->groundDist < 500.0f) {
        temp_v0 = RIDER(var_v1)->dnode.vqs.quat;
        if ((f64) RIDER(var_v1)->localUp.z < *(f64 *)((char *)&game_uso_D_807FFB20 + kRoD_m0p4_200)) {
            temp_v0[0] = 0.0f; temp_v0[1] = 0.0f; temp_v0[2] = 0.0f; temp_v0[3] = 1.0f;
            sp28 = 2;
            var_a3 = 4;
            RiderAnim_setHitKind_0000EDCC((int *)arg0, 2);
            if (RA(arg0)->rider->dnode.axes[2][1] < 0.0f) {
                RA(arg0)->selectedAnim = 0x1002A;
            } else {
                RA(arg0)->selectedAnim = 0x1002B;
            }
            var_v1 = (char *)RA(arg0)->rider;
        }
    }
    temp_v0_2 = (char *)&RIDER(var_v1)->child.parms.sta;
    if (!(RIDER(var_v1)->groundFlags & 1)) {
        var_f12 = RIDER(var_v1)->bumpSide * RA(arg0)->hitScale;
        var_f2 = RIDER(var_v1)->bumpUp * RA(arg0)->hitScale;
        var_f0_4 = RIDER(var_v1)->bumpFwd * RA(arg0)->hitScale;
        if (!(RIDER(var_v1)->flags & 0x4000)) {
            var_f0_4 = 0.0f;
            var_f2 = 0.0f;
            var_f12 = 0.0f;
        }
        if ((RA(arg0)->parms.l3f.value.f * ((volatile SnowBParm *)temp_v0_2)->value) <= var_f2) {
            sp28 = 3;
            sp30 = 0;
            RiderAnim_setHitKind_0000EDCC((int *)arg0, 2);
            var_a3 = 5;
            RA(arg0)->selectedAnim = (s32) (RA(arg0)->animSet | 0x15);
        } else if ((RA(arg0)->parms.l2f.value.f * ((volatile SnowBParm *)temp_v0_2)->value) <= var_f2) {
            sp28 = 0;
            sp30 = 0;
            RiderAnim_setHitKind_0000EDCC((int *)arg0, 5);
            var_a3 = 1;
            RA(arg0)->selectedAnim = (s32) (RA(arg0)->animSet | 0x2D);
        } else if ((RA(arg0)->parms.l1f.value.f * ((volatile SnowBParm *)temp_v0_2)->value) <= var_f2) {
            sp28 = 0;
            sp30 = 0;
            RiderAnim_setHitKind_0000EDCC((int *)arg0, 4);
            var_a3 = 1;
            RA(arg0)->selectedAnim = (s32) (RA(arg0)->animSet | 0x2C);
        } else if (RA(arg0)->parms.f4f.value.f <= var_f0_4) {
            sp28 = 4;
            RiderAnim_setHitKind_0000EDCC((int *)arg0, 3);
            var_a3 = 5;
            RA(arg0)->selectedAnim = (s32) (RA(arg0)->animSet | 0x15);
        } else if (RA(arg0)->parms.f3f.value.f <= var_f0_4) {
            sp28 = 3;
            RiderAnim_setHitKind_0000EDCC((int *)arg0, 2);
            var_a3 = 4;
            RA(arg0)->selectedAnim = (s32) (RA(arg0)->animSet | 0xB);
        } else if (RA(arg0)->parms.f2f.value.f <= var_f0_4) {
            sp28 = 2;
            RiderAnim_setHitKind_0000EDCC((int *)arg0, 1);
            var_a3 = 3;
            RA(arg0)->selectedAnim = (s32) (RA(arg0)->animSet | 0xA);
        } else if (RA(arg0)->parms.f1f.value.f <= var_f0_4) {
            sp28 = 0;
            sp30 = 0;
            RiderAnim_setHitKind_0000EDCC((int *)arg0, 1);
            var_a3 = 1;
            RA(arg0)->selectedAnim = (s32) (RA(arg0)->animSet | 0x2C);
        } else if (var_f0_4 <= -RA(arg0)->parms.r2f.value.f) {
            sp28 = 2;
            sp2C = (char *)2;
            var_a3 = 3;
            sp30 = 0; RiderAnim_setHitKind_0000EDCC((int *)arg0, 5);
            RA(arg0)->selectedAnim = (s32) (RA(arg0)->animSet | 0x16);
        } else if (var_f0_4 <= -RA(arg0)->parms.r1f.value.f) {
            sp28 = 0;
            sp2C = (char *)2;
            sp30 = 0;
            RiderAnim_setHitKind_0000EDCC((int *)arg0, 5);
            var_a3 = 1;
            RA(arg0)->selectedAnim = (s32) (RA(arg0)->animSet | 0x14);
        } else {
            temp_f0 = RA(arg0)->parms.xf1.value.f;
            if (temp_f0 <= var_f12) {
                sp28 = 3;
                RiderAnim_setHitKind_0000EDCC((int *)arg0, 2);
                var_a3 = 2;
                RA(arg0)->selectedAnim = (s32) (RA(arg0)->animSet | 0x13);
            } else if (var_f12 <= -temp_f0) {
                sp28 = 3;
                RiderAnim_setHitKind_0000EDCC((int *)arg0, 2);
                var_a3 = 2;
                RA(arg0)->selectedAnim = (s32) (RA(arg0)->animSet | 0x12);
            } else if ((*(f32 *)((char *)&game_uso_D_807FF68C + kDataF_60_1098) <= var_f2) && ((f64) RA(arg0)->hitScale == 1.0) && (RIDER(var_v1)->useDParms == 0)) {
                sp28 = 0;
                sp30 = 0;
                RA(arg0)->selectedAnim = (s32) (RA(arg0)->animSet | 0x2F);
                game_uso_func_07C0FC((int)RIDER(var_v1)->input,
                                     (char *)&game_uso_D_807FF68C + kRumble_hit1_109C, 0xA);
            }
        }
    }
    if ((RA(arg0)->trickPhase == 1) && (sp28 == 0)) {
        RA(arg0)->selectedAnim = 0;
    }
    if (RA(arg0)->selectedAnim != 0) {
        RiderAnim_hitFeedback_0001189C((int *)arg0, (s32) var_a3, sp2C);
        RA(arg0)->hitScale = 1.0f;
        RiderAnim_applyDamage_0000EDD4((int *)arg0, sp28, sp2C);
        temp_v0_3 = RA(arg0)->rider->dnode.angVel;
        temp_v0_3[0] = 0.0f; temp_v0_3[1] = 0.0f; temp_v0_3[2] = 0.0f; temp_v0_3[3] = 1.0f;
        RiderAnim_endTrick_0000D63C(arg0, (sp28 == 0));
        RA(arg0)->rider->riding = 0;
        if (sp30 != 0) {
            game_uso_func_077C44(arg0, RA(arg0)->selectedAnim, 0, 2, 1, 1);
            RiderAnim_queueState_0000D5F8(arg0, *(Pair2 *)((char *)&game_uso_D_807FF478 + kPmf_RiderAnim_stateCrashRecover), 2);
        } else {
            game_uso_func_077C44(arg0, RA(arg0)->selectedAnim, 0, 1, 1, 1);
            RiderAnim_queueState_0000D5F8(arg0, *(Pair2 *)((char *)&game_uso_D_807FF480 + kPmf_RiderAnim_stateResumeRide), 1);
        }
        RiderAnim_clearState_0000D5DC(arg0);
        RA(arg0)->trickPhase = 0;
    }
    if (RA(arg0)->rider->health <= 0) {
        RA(arg0)->trickPhase = 0;
        if (RA(arg0)->selectedAnim != 0) {
            RA(arg0)->selectedAnim = (RA(arg0)->selectedAnim & 0xFFFF) | 0x60000;
            game_uso_func_077C44(arg0, RA(arg0)->selectedAnim, 0, 1, 1, 1);
            RiderAnim_queueState_0000D5F8(arg0, *(Pair2 *)((char *)&game_uso_D_807FF3B8 + kPmfNull_DC8), 1);
        }
        RA(arg0)->rider->slowdown = 0x64;
        RiderAnim_clearState_0000D5DC(arg0);
    }
}
}
