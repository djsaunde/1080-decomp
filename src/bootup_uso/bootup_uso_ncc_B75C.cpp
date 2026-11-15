/* IDO 7.1 NCC (C++) NM-only comparison donor for SnowBoarder_updateOrient (0x830 / 524 words), agent-g run 242.
 * The in-tree cfe C body (bootup_uso.c, 94.38) ported to NCC: the target is edg-shaped (kit CCXLIII tells --
 * `sp60 *= 2.0f` stays mul.s, the 20.0f / 30.0f literals hoist in the target order, the 0x974 store / reload
 * colours match). cfe-only levers dropped (sp1C home keeper, folded `0 * *arg0` uses, the goto block_67 tail):
 * the 0x3C4 field is read in place (no named sp1C: uopt CSEs the load and spills it to a fresh slot like the
 * target), the -0.1 call argument is an inline dot3() (its sum lands in f0), both `var_v0 = 0x9A8 & 1` tails
 * are explicit, and operand order comes from greedy single-commute sweeps (95.14 -> 98.76).
 * Then (98.76 -> 99.97, 524 words = target, frame 120 = target): `-x * y` for the 0x678 call argument; the
 * 0x800 pointer read into a named char * (target v0); no `if (1) {}` after the A6C store (compare on the old
 * value, slti v0,11); the {0,1,0} zeros converted from an int variable set to 0 (a distinct uopt expression:
 * the 0x960 / sp5C zeros re-materialise mtc1 zero like the target) + `if (1) {}` after them (zero in f0);
 * the single-range float temps merged into var_f0 / var_f2 (homes) and temp_f12 declared first.
 * Run 243 (99.97 -> 99.98): `temp_a1 = arg0 + 0x3B0;` as the first 0x9A8 & 1 arm's call argument gives the
 * 0.8 compare block the target hoist order (arg0 + 0x3B0 before arg0 + 0xCC) and puts the a0 / a1 spills in
 * the target's relative order (a0 below a1).
 * EXACT (agent-c run 258, 99.98 -> 100): uopt `spilltemps` gives every colourable expression a temp slot BEFORE
 * colouring, in expression-bit order, first-fit on the BB region (kit CCXCIII). Reading the 0x990 word in place at
 * both tests (no var_v0 copy) makes that load a cross-BB expression ahead of the 0x3BC / 0x3C0 / 0x3C4 loads, so
 * it takes the first slot (coloured v0: the never-stored word) and the 0x3C4 spill drops one slot; holding the
 * dot3 value in var_f0 drops its home, moving the whole temp block up one word (target float 0x1C, unused 0x20,
 * a0 0x24, a1 0x28). Spliced over the bootup_uso.c placeholder on both build paths; the literal pool is the donor
 * .rodata (SnowBoarder_updateOrient_rodata pin = 0x8C8, same layout as the USO's 0x8C8..0x914 pool).
 * agent-g run 475: field form over include/game/snowboarder.h (included by relative path; the header's asserts are
 * C-only). Same bytes: every access keeps its width / type (a mismatched read stays `*(T *)&rider->field`). */
typedef float f32; typedef double f64; typedef int s32; typedef short s16;
typedef unsigned char u8; typedef unsigned short u16; typedef unsigned int u32;
#include "../../include/game/snowboarder.h"
extern "C" {
/* math_quatRotateAxisAngle(quat, axis, angle, owner): turn quat about axis (per-site alias of the game_libs helper) */
void bu_b75c_mvf(char *, char *, f32, SnowBoarder *);
extern char D_00008170[];
void func_00000000(...);
#define temp_f12_2 (-0.2f)
#define temp_f12_3 (0.2f)
void SnowBoarder_updateOrient(SnowBoarder *rider);
}
static inline f32 dot3(SnowBoarder *p, f32 *v) { return (p->side.x * v[0]) + (p->side.y * v[1]) + (p->side.z * v[2]); }
void SnowBoarder_updateOrient(SnowBoarder *rider) {
    f32 sp6C[3];
    f32 temp_f12;
    s32 var_v0;
    f32 sp60;
    f32 sp5C;
    f32 sp58;
    f32 sp54;
    f32 temp_f2_2;
    f32 var_f0;
    f32 var_f12;
    f64 temp_f0;
    f32 var_f2;
    s16 temp_v0_2;
    char *var_a0;
    char *temp_a1;

    var_v0 = 0;
    sp6C[0] = var_v0;
    sp6C[2] = var_v0;
    if (1) {}
    sp6C[1] = 1.0f;
    temp_f0 = (f64) rider->input->stickX;
    var_f12 = (f32) (temp_f0 * 0.15);
    sp60 = (f32) (temp_f0 * -0.04);
    if (rider->slowdown == 0x64) {
        var_f12 = 0.0f;
        sp60 = 0.0f;
    }
    var_f0 = rider->edgeSteer;
    var_f2 = rider->turnRate;
    rider->edgeSteer = (f32) (var_f0 - (var_f0 * rider->parms.trt.value * var_f2));
    rider->edgeSteer = (f32) (*(volatile f32 *)&rider->edgeSteer + (var_f12 * var_f2));
    if (rider->crouch != 0) {
        sp5C = 0;
    } else {
        sp5C = -((rider->side.x * sp6C[0]) + (rider->side.y * sp6C[1]) + (rider->side.z * sp6C[2]));
    }
    var_f0 = rider->edgeSlope;
    temp_f2_2 = rider->velDir.x;
    temp_f12 = rider->velDir.y;
#define temp_f14 (rider->velDir.z)
    rider->edgeSlope = (f32) (var_f0 + (f64) ((f64) (sp5C - var_f0) * 0.4));
    sp58 = (rider->dir.x * temp_f2_2) + (rider->dir.y * temp_f12) + (rider->dir.z * temp_f14);
    sp54 = (rider->side.x * temp_f2_2) + (rider->side.y * temp_f12) + (rider->side.z * temp_f14);
    if (rider->crouch != 0) {
        sp60 = (f32) ((f64) sp60 * 0.5);
    }
    var_f12 = -1.0f;
    rider->edge = (f32) (rider->edgeSteer + rider->edgeSlope);
    if (rider->flags & SNOWB_FLAG_MIRROR) {
        rider->edge = (f32) -rider->edge;
    }
    var_f0 = rider->edge;
    if (var_f0 < -1.0f) {
        var_f2 = -1.0f;
    } else if (var_f0 > 1.0f) {
        var_f2 = 1.0f;
    } else {
        var_f2 = var_f0;
    }
    rider->edge = var_f2;
    if ((rider->groundFlags & 1) || (rider->groundGentle == 0)) {
        rider->localUp.x = (f32) rider->dir.y;
        rider->localUp.y = (f32) -rider->side.y;
        rider->localUp.z = (f32) rider->up.y;
    } else {
        var_f0 = rider->groundNormal.x;
        var_f12 = rider->groundNormal.z;
        rider->localUp.x = (f32) ((rider->dir.x * var_f0) + (rider->dir.y * rider->groundNormal.y) + (rider->dir.z * var_f12));
        rider->localUp.y = (f32) -((rider->side.x * var_f0) + (rider->side.y * rider->groundNormal.y) + (rider->side.z * var_f12));
        rider->localUp.z = (f32) ((rider->up.x * var_f0) + (rider->up.y * rider->groundNormal.y) + (rider->up.z * var_f12));
    }
    if (rider->contactCount == 0) {
        rider->shake = 0;
        rider->angVel.x = 0;
        rider->angVel.y = 0;
        rider->angVel.z = 0;
        rider->angVel.w = 1.0f;
        var_a0 = (char *)rider->input;
        if ((((RiderInput *)var_a0)->held & 0x200) && (rider->groundFlags & 1)) {
            temp_v0_2 = rider->spinHold;
            rider->spinHold = (s16) (temp_v0_2 + 1);
            if (temp_v0_2 >= 0xB) {
                var_f0 = (f32) ((f64) rider->airSpin + ((f64) rider->input->stickX * 0.015));
                if (var_f0 < temp_f12_2) {
                    var_f2 = temp_f12_2;
                } else {
                    if (temp_f12_3 < var_f0) {
                        var_f2 = temp_f12_3;
                    } else {
                        var_f2 = var_f0;
                    }
                }
                rider->airSpin = var_f2;
            }
        } else {
            rider->spinHold = 0;
            if (rider->groundFlags & 1) {
                temp_a1 = (char *)&rider->up;
                bu_b75c_mvf((char *)&rider->orient, temp_a1, -rider->input->stickX * rider->parms.ayr.value, rider);
            } else {
                var_a0 = (char *)&rider->orient;
                temp_a1 = (char *)&rider->dir;
                var_f0 = dot3(rider, sp6C);
                bu_b75c_mvf(var_a0, temp_a1, (f32) ((f64) var_f0 * -0.1), rider);
                if (rider->unkA18 != 0) {
                    bu_b75c_mvf(var_a0, temp_a1, rider->input->stickX * rider->parms.azr.value, rider);
                    bu_b75c_mvf(var_a0, (char *)&rider->side, rider->input->stickY * rider->parms.axr.value, rider);
                    bu_b75c_mvf(var_a0, (char *)&rider->up, -rider->input->stickX * rider->parms.ayr.value, rider);
                }
            }
        }
        var_v0 = rider->groundFlags & 1;
        if (var_v0 != 0) {
            if ((f64) rider->up.y < 0.8) {
                rider->vel.x = 0;
            }
            bu_b75c_mvf((char *)&rider->orient, (char *)&rider->up, -rider->airSpin, rider);
            var_v0 = rider->groundFlags & 1;
        }
    } else {
        rider->spinHold = 0;
        rider->unkA74 = 0;
        rider->airSpin = 0;
        if (rider->edge < 0.0f) {
            var_f0 = -rider->edge;
        } else {
            var_f0 = rider->edge;
        }
        rider->shake = (s32) (20.0f + (30.0f * var_f0));
        if (rider->groundFlags & 1) {
            func_00000000((char *)&rider->sound, *(s32 *)&rider->pos.x);
            if (rider->slowdown == 0) {
                if (rider->dir.x < 0.0f) {
                    var_f0 = -rider->dir.x;
                } else {
                    var_f0 = rider->dir.x;
                }
                rider->vel.x += rider->vel.x * rider->parms.hpf.value * var_f0;
            }
            if ((f64) rider->up.y < 0.8) {
                sp60 *= 2.0f;
            }
            var_a0 = (char *)&rider->orient;
        } else {
            if ((sp58 < rider->parms.mdr.value) && (((sp54 > 0.0f) && (sp60 < 0.0f)) || ((sp54 < 0.0f) && (sp60 > 0.0f)))) {
                rider->shake = (s32) (rider->shake + 0x14);
                func_00000000(rider->input, D_00008170, 1, rider);
                sp60 = 0.0f;
            }
            if (rider->localUp.y < 0.0f) {
                var_f0 = -rider->localUp.y;
            } else {
                var_f0 = rider->localUp.y;
            }
            if (rider->parms.mro.value < var_f0) {
                bu_b75c_mvf((char *)&rider->orient, (char *)&rider->dir, rider->localUp.y * rider->parms.ure.value, rider);
            }
            var_a0 = (char *)&rider->orient;
            if (rider->localUp.x < 0.0f) {
                var_f0 = -rider->localUp.x;
            } else {
                var_f0 = rider->localUp.x;
            }
            if (rider->parms.mpi.value < var_f0) {
                bu_b75c_mvf(var_a0, (char *)&rider->side, rider->localUp.x * rider->parms.ure.value, rider);
            }
        }
        bu_b75c_mvf(var_a0, (char *)&rider->up, sp60, rider);
        var_v0 = rider->groundFlags & 1;
    }
    if (var_v0 != 0) {
        var_f0 = 0.724f;
    } else {
        var_f0 = rider->parms.tch.value;
    }
    var_f2 = rider->angVel.w;
    rider->angVel.x = (f32) (rider->angVel.x * var_f0);
    rider->angVel.y = (f32) (rider->angVel.y * var_f0);
    rider->angVel.z = (f32) (rider->angVel.z * var_f0);
    rider->angVel.w = (f32) ((f64) var_f2 + ((f64) (1.0f - var_f2) * (1.0 - (f64) var_f0)));
}
#undef temp_f14
