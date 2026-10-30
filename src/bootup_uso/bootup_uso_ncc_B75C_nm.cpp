/* IDO 7.1 NCC (C++) NM-only comparison donor for func_0000B75C (0x830 / 524 words), agent-g run 242.
 * The in-tree cfe C body (bootup_uso.c, 94.38) ported to NCC: the target is edg-shaped (kit CCXLIII tells --
 * `sp60 *= 2.0f` stays mul.s, the 20.0f / 30.0f literals hoist in the target order, the 0x974 store / reload
 * colours match). cfe-only levers dropped (sp1C home keeper, folded `0 * *arg0` uses, the goto block_67 tail):
 * the 0x3C4 field is read in place (no named sp1C: uopt CSEs the load and spills it to a fresh slot like the
 * target), the -0.1 call argument is an inline dot3() (its sum lands in f0), both `var_v0 = 0x9A8 & 1` tails
 * are explicit, and operand order comes from greedy single-commute sweeps (95.14 -> 98.76).
 * Left: the {0,1,0} zero is shared with the 0x960 == 100 zeros (target re-materialises mtc1 zero twice), the
 * A6C counter compare is done on the incremented value (slti v1,12; target slti v0,11), the 0x800 pointer is
 * not held in v0, the 0.8 compare's a0 / a1 hoist order, frame 128 vs 120 (homes 12 bytes low).
 * Still NON_MATCHING: build/non_matching/ only. */
typedef float f32; typedef double f64; typedef int s32; typedef short s16;
extern "C" {
void bu_b75c_mvf(char *, char *, f32, char *);
extern char D_00008170[];
void func_00000000(...);
#define temp_f12_2 (-0.2f)
#define temp_f12_3 (0.2f)
void func_0000B75C(char *arg0);
}
static inline f32 dot3(char *p, f32 *v) { return (*(f32 *)(p + 0x3BC) * v[0]) + (*(f32 *)(p + 0x3C0) * v[1]) + (*(f32 *)(p + 0x3C4) * v[2]); }
void func_0000B75C(char *arg0) {
    f32 sp6C[3];
    s32 var_v0;
    f32 sp60;
    f32 sp5C;
    f32 sp58;
    f32 sp54;
    f32 temp_f0_2;
    f32 temp_f12;
    f32 temp_f2;
    f32 temp_f2_2;
    f32 temp_f2_9;
    f32 var_f0;
    f32 var_f12;
    f64 temp_f0;
    f32 var_f2;
    s16 temp_v0_2;
    char *var_a0;
    char *temp_a1;

    sp6C[0] = 0.0;
    sp6C[2] = 0.0;
    sp6C[1] = 1.0f;
    temp_f0 = (f64) *(f32 *)(*(s32 *)((char *)(arg0) + 0x800));
    var_f12 = (f32) (temp_f0 * 0.15);
    sp60 = (f32) (temp_f0 * -0.04);
    if (*(s32 *)((char *)(arg0) + 0x960) == 0x64) {
        var_f12 = 0.0f;
        sp60 = 0.0f;
    }
    temp_f0_2 = *(f32 *)((char *)(arg0) + 0x974);
    temp_f2 = *(f32 *)((char *)(arg0) + 0xA00);
    var_v0 = *(s32 *)((char *)(arg0) + 0x990);
    *(f32 *)((char *)(arg0) + 0x974) = (f32) (temp_f0_2 - (temp_f0_2 * *(f32 *)((char *)(arg0) + 0x720) * temp_f2));
    *(f32 *)((char *)(arg0) + 0x974) = (f32) (*(volatile f32 *)((char *)(arg0) + 0x974) + (var_f12 * temp_f2));
    if (var_v0 != 0) {
        sp5C = 0;
    } else {
        sp5C = -((*(f32 *)((char *)(arg0) + 0x3BC) * sp6C[0]) + (*(f32 *)((char *)(arg0) + 0x3C0) * sp6C[1]) + (*(f32 *)((char *)(arg0) + 0x3C4) * sp6C[2]));
    }
    temp_f0_2 = *(f32 *)((char *)(arg0) + 0x978);
    temp_f2_2 = *(f32 *)((char *)(arg0) + 0x30C);
    temp_f12 = *(f32 *)((char *)(arg0) + 0x310);
#define temp_f14 (*(f32 *)((char *)(arg0) + 0x314))
    *(f32 *)((char *)(arg0) + 0x978) = (f32) (temp_f0_2 + (f64) ((f64) (sp5C - temp_f0_2) * 0.4));
    sp58 = (*(f32 *)((char *)(arg0) + 0x3C8) * temp_f2_2) + (*(f32 *)((char *)(arg0) + 0x3CC) * temp_f12) + (*(f32 *)((char *)(arg0) + 0x3D0) * temp_f14);
    sp54 = (*(f32 *)((char *)(arg0) + 0x3BC) * temp_f2_2) + (*(f32 *)((char *)(arg0) + 0x3C0) * temp_f12) + (*(f32 *)((char *)(arg0) + 0x3C4) * temp_f14);
    if (var_v0 != 0) {
        sp60 = (f32) ((f64) sp60 * 0.5);
    }
    var_f12 = -1.0f;
    *(f32 *)((char *)(arg0) + 0x970) = (f32) (*(f32 *)((char *)(arg0) + 0x974) + *(f32 *)((char *)(arg0) + 0x978));
    if (*(s32 *)((char *)(arg0) + 0xA58) & 0x80) {
        *(f32 *)((char *)(arg0) + 0x970) = (f32) -*(f32 *)((char *)(arg0) + 0x970);
    }
    temp_f0_2 = *(f32 *)((char *)(arg0) + 0x970);
    if (temp_f0_2 < -1.0f) {
        var_f2 = -1.0f;
    } else if (temp_f0_2 > 1.0f) {
        var_f2 = 1.0f;
    } else {
        var_f2 = temp_f0_2;
    }
    *(f32 *)((char *)(arg0) + 0x970) = var_f2;
    if ((*(s32 *)((char *)(arg0) + 0x9A8) & 1) || (*(s32 *)((char *)(arg0) + 0xA50) == 0)) {
        *(f32 *)((char *)(arg0) + 0xA04) = (f32) *(f32 *)((char *)(arg0) + 0x3CC);
        *(f32 *)((char *)(arg0) + 0xA08) = (f32) -*(f32 *)((char *)(arg0) + 0x3C0);
        *(f32 *)((char *)(arg0) + 0xA0C) = (f32) *(f32 *)((char *)(arg0) + 0x3B4);
    } else {
        temp_f0_2 = *(f32 *)((char *)(arg0) + 0x9D8);
        var_f12 = *(f32 *)((char *)(arg0) + 0x9E0);
        *(f32 *)((char *)(arg0) + 0xA04) = (f32) ((*(f32 *)((char *)(arg0) + 0x3C8) * temp_f0_2) + (*(f32 *)((char *)(arg0) + 0x3CC) * *(f32 *)((char *)(arg0) + 0x9DC)) + (*(f32 *)((char *)(arg0) + 0x3D0) * var_f12));
        *(f32 *)((char *)(arg0) + 0xA08) = (f32) -((*(f32 *)((char *)(arg0) + 0x3BC) * temp_f0_2) + (*(f32 *)((char *)(arg0) + 0x3C0) * *(f32 *)((char *)(arg0) + 0x9DC)) + (*(f32 *)((char *)(arg0) + 0x3C4) * var_f12));
        *(f32 *)((char *)(arg0) + 0xA0C) = (f32) ((*(f32 *)((char *)(arg0) + 0x3B0) * temp_f0_2) + (*(f32 *)((char *)(arg0) + 0x3B4) * *(f32 *)((char *)(arg0) + 0x9DC)) + (*(f32 *)((char *)(arg0) + 0x3B8) * var_f12));
    }
    if (*(s32 *)((char *)(arg0) + 0x938) == 0) {
        *(s32 *)((char *)(arg0) + 0xA5C) = 0;
        *(f32 *)((char *)(arg0) + 0x2FC) = 0;
        *(f32 *)((char *)(arg0) + 0x300) = 0;
        *(f32 *)((char *)(arg0) + 0x304) = 0;
        *(f32 *)((char *)(arg0) + 0x308) = 1.0f;
        if ((*(s32 *)((char *)(*(s32 *)((char *)(arg0) + 0x800)) + 0x10) & 0x200) && (*(s32 *)((char *)(arg0) + 0x9A8) & 1)) {
            temp_v0_2 = *(s16 *)((char *)(arg0) + 0xA6C);
            *(s16 *)((char *)(arg0) + 0xA6C) = (s16) (temp_v0_2 + 1); if (1) {}
            if (temp_v0_2 >= 0xB) {
                temp_f0_2 = (f32) ((f64) *(f32 *)((char *)(arg0) + 0xA70) + ((f64) *(f32 *)(*(s32 *)((char *)(arg0) + 0x800)) * 0.015));
                if (temp_f0_2 < temp_f12_2) {
                    var_f2 = temp_f12_2;
                } else {
                    if (temp_f12_3 < temp_f0_2) {
                        var_f2 = temp_f12_3;
                    } else {
                        var_f2 = temp_f0_2;
                    }
                }
                *(f32 *)((char *)(arg0) + 0xA70) = var_f2;
            }
        } else {
            *(s16 *)((char *)(arg0) + 0xA6C) = 0;
            if (*(s32 *)((char *)(arg0) + 0x9A8) & 1) {
                bu_b75c_mvf(arg0 + 0xCC, arg0 + 0x3B0, -*(f32 *)(*(s32 *)((char *)(arg0) + 0x800)) * *(f32 *)((char *)(arg0) + 0x678), arg0);
            } else {
                var_a0 = arg0 + 0xCC;
                temp_a1 = arg0 + 0x3C8;
                bu_b75c_mvf(var_a0, temp_a1, (f32) ((f64) dot3(arg0, sp6C) * -0.1), arg0);
                if (*(s32 *)((char *)(arg0) + 0xA18) != 0) {
                    bu_b75c_mvf(var_a0, temp_a1, *(f32 *)(*(s32 *)((char *)(arg0) + 0x800)) * *(f32 *)((char *)(arg0) + 0x690), arg0);
                    bu_b75c_mvf(var_a0, arg0 + 0x3BC, *(f32 *)((char *)(*(s32 *)((char *)(arg0) + 0x800)) + 0x4) * *(f32 *)((char *)(arg0) + 0x660), arg0);
                    bu_b75c_mvf(var_a0, arg0 + 0x3B0, *(f32 *)((char *)(arg0) + 0x678) * -*(f32 *)(*(s32 *)((char *)(arg0) + 0x800)), arg0);
                }
            }
        }
        var_v0 = *(s32 *)((char *)(arg0) + 0x9A8) & 1;
        if (var_v0 != 0) {
            if ((f64) *(f32 *)((char *)(arg0) + 0x3B4) < 0.8) {
                *(f32 *)((char *)(arg0) + 0x318) = 0;
            }
            bu_b75c_mvf(arg0 + 0xCC, arg0 + 0x3B0, -*(f32 *)((char *)(arg0) + 0xA70), arg0);
            var_v0 = *(s32 *)((char *)(arg0) + 0x9A8) & 1;
        }
    } else {
        *(s16 *)((char *)(arg0) + 0xA6C) = 0;
        *(f32 *)((char *)(arg0) + 0xA74) = 0;
        *(f32 *)((char *)(arg0) + 0xA70) = 0;
        if (*(f32 *)((char *)(arg0) + 0x970) < 0.0f) {
            var_f0 = -*(f32 *)((char *)(arg0) + 0x970);
        } else {
            var_f0 = *(f32 *)((char *)(arg0) + 0x970);
        }
        *(s32 *)((char *)(arg0) + 0xA5C) = (s32) (20.0f + (30.0f * var_f0));
        if (*(s32 *)((char *)(arg0) + 0x9A8) & 1) {
            func_00000000(arg0 + 0x808, *(s32 *)((char *)(arg0) + 0xB4));
            if (*(s32 *)((char *)(arg0) + 0x960) == 0) {
                if (*(f32 *)((char *)(arg0) + 0x3C8) < 0.0f) {
                    var_f0 = -*(f32 *)((char *)(arg0) + 0x3C8);
                } else {
                    var_f0 = *(f32 *)((char *)(arg0) + 0x3C8);
                }
                *(f32 *)((char *)(arg0) + 0x318) += *(f32 *)((char *)(arg0) + 0x318) * *(f32 *)((char *)(arg0) + 0x558) * var_f0;
            }
            if ((f64) *(f32 *)((char *)(arg0) + 0x3B4) < 0.8) {
                sp60 *= 2.0f;
            }
            var_a0 = arg0 + 0xCC;
        } else {
            if ((sp58 < *(f32 *)((char *)(arg0) + 0x648)) && (((sp54 > 0.0f) && (sp60 < 0.0f)) || ((sp54 < 0.0f) && (sp60 > 0.0f)))) {
                *(s32 *)((char *)(arg0) + 0xA5C) = (s32) (*(s32 *)((char *)(arg0) + 0xA5C) + 0x14);
                func_00000000(*(s32 *)((char *)(arg0) + 0x800), D_00008170, 1, arg0);
                sp60 = 0.0f;
            }
            if (*(f32 *)((char *)(arg0) + 0xA08) < 0.0f) {
                var_f0 = -*(f32 *)((char *)(arg0) + 0xA08);
            } else {
                var_f0 = *(f32 *)((char *)(arg0) + 0xA08);
            }
            if (*(f32 *)((char *)(arg0) + 0x618) < var_f0) {
                bu_b75c_mvf(arg0 + 0xCC, arg0 + 0x3C8, *(f32 *)((char *)(arg0) + 0x600) * *(f32 *)((char *)(arg0) + 0xA08), arg0);
            }
            var_a0 = arg0 + 0xCC;
            if (*(f32 *)((char *)(arg0) + 0xA04) < 0.0f) {
                var_f0 = -*(f32 *)((char *)(arg0) + 0xA04);
            } else {
                var_f0 = *(f32 *)((char *)(arg0) + 0xA04);
            }
            if (*(f32 *)((char *)(arg0) + 0x630) < var_f0) {
                bu_b75c_mvf(var_a0, arg0 + 0x3BC, *(f32 *)((char *)(arg0) + 0xA04) * *(f32 *)((char *)(arg0) + 0x600), arg0);
            }
        }
        bu_b75c_mvf(var_a0, arg0 + 0x3B0, sp60, arg0);
        var_v0 = *(s32 *)((char *)(arg0) + 0x9A8) & 1;
    }
    if (var_v0 != 0) {
        var_f0 = 0.724f;
    } else {
        var_f0 = *(f32 *)((char *)(arg0) + 0x7F8);
    }
    temp_f2_9 = *(f32 *)((char *)(arg0) + 0x308);
    *(f32 *)((char *)(arg0) + 0x2FC) = (f32) (*(f32 *)((char *)(arg0) + 0x2FC) * var_f0);
    *(f32 *)((char *)(arg0) + 0x300) = (f32) (*(f32 *)((char *)(arg0) + 0x300) * var_f0);
    *(f32 *)((char *)(arg0) + 0x304) = (f32) (*(f32 *)((char *)(arg0) + 0x304) * var_f0);
    *(f32 *)((char *)(arg0) + 0x308) = (f32) ((f64) temp_f2_9 + ((f64) (1.0f - temp_f2_9) * (1.0 - (f64) var_f0)));
}
#undef temp_f14
