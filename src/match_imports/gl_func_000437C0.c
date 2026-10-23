/* Native original-compiler source port from src/game_libs/game_libs_post0b.c. Target bytes unchanged. */
#include "context_game_libs_post0b.h"
void gl_func_000437C0(char *arg0, char *arg1) {
    s32 *temp_a0_2;
    s32 *temp_a0_3;
    GL470E4List *temp_a1;
    GL470E4Object *temp_v1;
    s32 temp_a2_6;
    char *scratch_ptr;
    s32 var_a2;
    int unused_home;
    s32 temp_a2;
    s32 temp_a2_2;
    s32 temp_a2_3;
    s32 temp_a2_4;
    s32 temp_a2_5;
    struct { f32 values[4]; char pad10[0x24]; Pair2 field34; char pad3C[0x1C]; int field58; void *field5C; } sp4C;
    char *var_a1;
    s32 temp_v0;
    s32 unused_flag_home;
    char *temp_a3;
    char *temp_t0;
    char *temp_a3_2;
    char *temp_t0_2;
    char *temp_a3_3;
    char *temp_t0_3;
    s32 var_v1;
    char *temp_a0;
    if ((*(int *)((char *)(arg0) + (0x14))) != 0) {
        temp_v0 = (*(int *)((char *)(arg0) + (0x50)));
        if (temp_v0 != -1) {
            temp_a0 = *(char **)((char *)&D_00000000 + 0x254);
            var_v1 = (*(int *)((char *)(arg0) + (0x54))) >= (*(int *)((char *)(temp_a0) + (0x10C)));
            if (var_v1 != 0) {
                var_v1 = (*(int *)((char *)(temp_a0) + (0x110))) >= temp_v0;
            }
            (*(int *)((char *)(arg0) + (0x4C))) = var_v1;
            if (var_v1 != 0) {
                goto block_5;
            }
        } else {
block_5:
            gl_437C0_link_43BEC(arg0, (*(int *)((char *)((*(char **)((char *)&D_00000000 + 0x254))) + (0x158))));
            temp_a0_2 = (*(int *)((char *)(arg0) + (0x60)));
            if (temp_a0_2 != 0) {
                if (((*(int *)((char *)(&D_00000000) + (0))) != 0) && ((*(int *)((char *)(arg1) + (0xA4))) == (*(int *)((char *)(arg0) + (0x18))))) {
                    gl_437C0_reset_56814(temp_a0_2);
                    var_a1 = (char *)&sp4C;
                    if (((s32)var_a1 != 0) || ((var_a1 = gl_437C0_alloc_410E4(0x60)) != 0)) {
                        (*(int *)((char *)(var_a1) + (0x5C))) = (s32) &gl_437C0_ctor_table;
                        var_a2 = (s32)(var_a1 + 0x34);
                        if (((s32)var_a2 != 0) || ((var_a2 = (s32)gl_437C0_alloc_410E4(8)) != 0)) {
                            (*(int *)((char *)(var_a2) + (0x4))) = 0;
                            (*(int *)((char *)(var_a2) + (0x0))) = 0;
                        }
                        var_a2 = (s32)(var_a1 + 0x58);
                        if (((s32)var_a2 != 0) || ((var_a2 = (s32)gl_437C0_alloc_410E4(4)) != 0)) {
                            (*(int *)((char *)(var_a2) + (0))) = 0;
                        }
                        gl_437C0_init_517E4(var_a1);
                    }
                    sp4C.field5C = &gl_437C0_value_table;
                    sp4C.values[0] = 1.0f;
                    sp4C.values[1] = 1.0f;
                    sp4C.values[2] = 0.0f;
                    sp4C.values[3] = 0.0f;
                    gl_437C0_light_45CB0((s32 *)&D_00000000, (*(int *)((char *)(arg0) + (0x60))), (char *)&sp4C);
                    gl_437C0_finish_57700((*(int *)((char *)(arg0) + (0x60))));
                    (*(int *)((char *)(arg0) + (0x84))) = 1;
                } else if (((*(int *)((char *)(&D_00000000) + (0x244))) != 0) || ((*(int *)((char *)(arg0) + (0x84))) != 0)) {
                    gl_437C0_reset_56814(temp_a0_2);
                    gl_437C0_update_454C4((s32 *)&D_00000000, (*(int *)((char *)(arg0) + (0x60))), arg0, (*(int *)((char *)(arg1) + (0x9C))));
                    gl_437C0_finish_57700((*(int *)((char *)(arg0) + (0x60))));
                    gl_437C0_enable_430D8((*(int *)((char *)(arg0) + (0x60))), ((*(int *)((char *)(arg1) + (0xA8))) != 0));
                    (*(int *)((char *)(arg0) + (0x84))) = 0;
                } else if (((*(int *)((char *)(&D_00000000) + (0x248))) != 0) || ((*(int *)((char *)(arg1) + (0xA8))) != 0)) {
                    gl_437C0_enable_430D8(temp_a0_2, ((*(int *)((char *)(arg1) + (0xA8))) != 0));
                }
            }
            if ((*(int *)((char *)(arg0) + (0x38))) & 0x100000) {
                temp_a0_3 = (*(int *)((char *)(arg0) + (0x68)));
                if (temp_a0_3 != 0) {
                    gl_437C0_reset_56814(temp_a0_3);
                    temp_v0 = (*(int *)((char *)(arg0) + (0x38)));
                    if (temp_v0 & 0x04000000) {
                        temp_v1 = *(GL470E4Object **)(arg0 + 0x68);
                        temp_a1 = temp_v1->list;
                        var_a2 = temp_a1->count;
                        temp_a1->count = (s32) (var_a2 + 1);
                        temp_a3 = (int)temp_v1->list->packets + (var_a2 * 8);
                        (*(int *)((char *)(temp_a3) + (0x0))) = 0x03840010;
                        (*(int *)((char *)(temp_a3) + (0x4))) = gl_437C0_physical_6C384(&gl_437C0_lightA, temp_a1, var_a2);
                        temp_v1 = *(GL470E4Object **)(arg0 + 0x68);
                        temp_a1 = temp_v1->list;
                        temp_a2_2 = temp_a1->count;
                        temp_a1->count = (s32) (temp_a2_2 + 1);
                        temp_t0 = (int)temp_v1->list->packets + (temp_a2_2 * 8);
                        (*(int *)((char *)(temp_t0) + (0x0))) = 0x03820010;
                        (*(int *)((char *)(temp_t0) + (0x4))) = (s32) (gl_437C0_physical_6C384(&gl_437C0_lightA, temp_a1, temp_a2_2) + 0x10);
                    } else if ((temp_v0 & 0xF00) == 0x200) {
                        temp_v1 = *(GL470E4Object **)(arg0 + 0x68);
                        temp_a1 = temp_v1->list;
                        temp_a2_3 = temp_a1->count;
                        temp_a1->count = (s32) (temp_a2_3 + 1);
                        temp_a3_2 = (int)temp_v1->list->packets + (temp_a2_3 * 8);
                        (*(int *)((char *)(temp_a3_2) + (0x0))) = 0x03840010;
                        (*(int *)((char *)(temp_a3_2) + (0x4))) = gl_437C0_physical_6C384(&gl_437C0_lightB, temp_a1, temp_a2_3);
                        temp_v1 = *(GL470E4Object **)(arg0 + 0x68);
                        temp_a1 = temp_v1->list;
                        temp_a2_4 = temp_a1->count;
                        temp_a1->count = (s32) (temp_a2_4 + 1);
                        temp_t0_2 = (int)temp_v1->list->packets + (temp_a2_4 * 8);
                        (*(int *)((char *)(temp_t0_2) + (0x0))) = 0x03820010;
                        (*(int *)((char *)(temp_t0_2) + (0x4))) = (s32) (gl_437C0_physical_6C384(&gl_437C0_lightB, temp_a1, temp_a2_4) + 0x10);
                    } else {
                        temp_v1 = *(GL470E4Object **)(arg0 + 0x68);
                        temp_a1 = temp_v1->list;
                        temp_a2_5 = temp_a1->count;
                        temp_a1->count = (s32) (temp_a2_5 + 1);
                        temp_a3_3 = (int)temp_v1->list->packets + (temp_a2_5 * 8);
                        (*(int *)((char *)(temp_a3_3) + (0x0))) = 0x03840010;
                        (*(int *)((char *)(temp_a3_3) + (0x4))) = gl_437C0_physical_6C384(&gl_437C0_lightC, temp_a1, temp_a2_5);
                        temp_v1 = *(GL470E4Object **)(arg0 + 0x68);
                        temp_a1 = temp_v1->list;
                        temp_a2_6 = temp_a1->count;
                        temp_a1->count = (s32) (temp_a2_6 + 1);
                        temp_t0_3 = (int)temp_v1->list->packets + (temp_a2_6 * 8);
                        (*(int *)((char *)(temp_t0_3) + (0x0))) = 0x03820010;
                        (*(int *)((char *)(temp_t0_3) + (0x4))) = (s32) (gl_437C0_physical_6C384(&gl_437C0_lightC, temp_a1, temp_a2_6) + 0x10);
                    }
                    gl_437C0_finish_57700((*(int *)((char *)(arg0) + (0x68))));
                }
            }
        }
    }
}
