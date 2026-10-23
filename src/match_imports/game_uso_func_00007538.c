/* Native original-compiler source port from src/game_uso/game_uso.c. Target bytes unchanged. */
#include "context_game_uso.h"
void game_uso_func_00007538(int *a0, int arg1) {
    GameEventStateLayout *arg0 = (GameEventStateLayout *)a0;
    f32 temp_f12;
    f32 output_44;
    f32 magnitude;
    f32 output_48;
    f64 temp_f0;
    s32 temp_a1;
    s32 temp_a1_2;
    s32 temp_a1_3;
    s32 temp_a1_4;
    s32 temp_a1_5;
    s32 temp_a1_6;
    s32 temp_a1_7;
    s32 temp_a2;
    s32 temp_a3_2;
    s32 active_mask;
    s32 var_a2;
    s32 var_a3;
    s32 result_mask;
    s32 suppress_direction;
    GameEventOutputLayout *temp_v1;
    char *table_pair;
    temp_a2 = arg0->field_50;
    result_mask = 0;
    suppress_direction = 0;
    output_44 = 0.0f;
    output_48 = 0.0f;
    if (temp_a2 > 0) {
        arg0->field_50 = (s32) (temp_a2 - 1);
    }
    temp_a1 = arg0->field_48;
    if (temp_a1 > 0) {
        arg0->field_48 = (s32) (temp_a1 - 1);
    }
    active_mask = arg0->field_6C;
    if (active_mask == 0) {
        if (arg1 & 0x10) {
            if (arg0->field_30->field_938 != 0) {
                output_48 = -1.0f;
                if (arg0->field_38 >= 0.0f) {
                    arg0->field_3C = 1.0f;
                } else {
                    arg0->field_3C = -1.0f;
                }
                if (1) { active_mask = arg0->field_6C; }
            }
        } else if ((arg1 & 1) && (arg0->field_48 == 0)) {
            arg0->field_6C = active_mask | 1;
            arg0->field_44 = 0x5B;
            if (1) { active_mask = arg0->field_6C; }
        } else if (arg1 & 0x40) {
            arg0->field_6C = active_mask | 0x40;
            arg0->field_44 = 0xD;
            if (1) { active_mask = arg0->field_6C; }
        } else if ((arg1 & 4) && (arg0->field_50 == 0)) {
            output_48 = 1.0f;
            suppress_direction = 1;
            arg0->field_50 = 8;
            active_mask = *(volatile int *)((char *)arg0 + 0x6C);
        } else if (arg1 & 0x20) {
            arg0->field_6C = (s32) (active_mask | 0x20);
            arg0->field_4C = 0x26;
            temp_f12 = arg0->field_30->field_B4.x;
            if (temp_f12 < 0.0f) {
                magnitude = -temp_f12;
            } else {
                magnitude = temp_f12;
            }
            arg0->field_58 = (u32)(s32)magnitude % 9U;
            arg0->field_44 = 0x3C;
            if (1) { active_mask = arg0->field_6C; }
        } else if (arg1 & 0x80) {
            arg0->field_6C = active_mask | 0x80;
            arg0->field_4C = 0;
            arg0->field_44 = 2;
            if (1) { active_mask = arg0->field_6C; }
        } else if (arg1 & 8) {
            if (arg0->field_30->field_938 != 0) {
                output_48 = -1.0f;
            }
        } else if (arg1 & 2) {
            if (arg0->field_30->field_938 != 0) {
                result_mask = 0x400;
            }
        } else {
            if (arg1 & 0x100) {
                arg0->field_6C = active_mask | 0x100;
                arg0->field_44 = 0x68;
                if (1) { active_mask = arg0->field_6C; }
            }
        }
    }
    if (active_mask & 1) {
        result_mask |= 0x500;
        temp_a1_2 = arg0->field_44 - 1;
        arg0->field_44 = temp_a1_2;
        if (temp_a1_2 == 0) {
            arg0->field_6C = (s32) (arg0->field_6C & ~0xA1);
        }
    } else if (active_mask & 0x40) {
        temp_a1_3 = arg0->field_44 - 1;
        arg0->field_44 = temp_a1_3;
        if (temp_a1_3 == 0) {
            arg0->field_6C = (s32) (arg0->field_6C & ~0x40);
        } else {
            result_mask |= 0x100;
        }
    } else if (active_mask & 0x20) {
        if (arg0->field_30->field_938 == 0) {
            arg1 = arg0->field_4C;
            suppress_direction = 1;
            result_mask |= 0x200;
            if (arg1 >= 0x1F) {
                table_pair = (char*)&game_uso_D_807FEC28 + 0x638;
                if (1) { table_pair += arg0->field_58 * 8; }
                output_44 = *(float *)table_pair;
                output_48 = *(float *)(table_pair + 4);
            }
            arg0->field_4C = (s32) (arg1 - 1);
        }
        if ((arg0->field_4C == 0) || (temp_a1_4 = arg0->field_44 - 1, arg0->field_44 = temp_a1_4, (temp_a1_4 == 0))) {
            arg0->field_6C = (s32) (arg0->field_6C & ~0x20);
        }
    } else if (active_mask & 0x80) {
        while (0) { temp_a1_5 = temp_a2 + temp_a2; }
        temp_a3_2 = arg0->field_4C;
        suppress_direction = 1;
        if ((temp_a3_2 < 8) || ((temp_a2 = arg0->field_4DC, (temp_a3_2 >= (temp_a2 + 8))) && (temp_a3_2 < (temp_a2 + 0x10))) || (temp_a3_2 >= (((temp_a2 + temp_a2)) + 0x10))) {
            if (1) { temp_a2 = arg0->field_4DC; }
            temp_a1_5 = (temp_a2 + temp_a2);
            if (temp_a3_2 >= (temp_a1_5 + 0x10)) {
                arg0->field_58 = (u32) ((temp_a3_2 - temp_a1_5) - 0x10);
                if ((temp_a3_2 >= (temp_a1_5 + 0x15)) && ((temp_a1_5 + 0x17) >= temp_a3_2)) {
                    result_mask |= 0x1600;
                }
            } else if (temp_a3_2 >= (temp_a2 + 8)) {
                arg0->field_58 = (u32) ((temp_a3_2 - temp_a2) - 8);
                if ((temp_a3_2 >= (temp_a2 + 0xD)) && ((temp_a2 + 0xF) >= temp_a3_2)) {
                    result_mask |= 0x1200;
                }
            } else {
                arg0->field_58 = (u32) temp_a3_2;
                if ((temp_a3_2 >= 5) && (temp_a3_2 < 8)) {
                    result_mask |= 0x1000;
                }
            }
            table_pair = (char*)&game_uso_D_807FEC28 + 0x638;
            if (1) { table_pair += arg0->field_58 * 8; }
            output_44 = *(float *)table_pair;
            output_48 = *(float *)(table_pair + 4);
        }
        if (1) { temp_a2 = arg0->field_44; }
        temp_a1_6 = temp_a3_2 + 1;
        arg0->field_4C = temp_a1_6;
        if (temp_a2 == 0) {
            var_a2 = 8;
        } else {
            if (temp_a2 == 1) {
                var_a3 = arg0->field_4DC + 0x10;
            } else {
                var_a3 = ((arg0->field_4DC + arg0->field_4DC)) + 0x18;
            }
            var_a2 = var_a3;
        }
        if (temp_a1_6 >= var_a2) {
            arg0->field_6C = (s32) (arg0->field_6C & ~0x80);
        }
    } else if (active_mask & 0x100) {
        if (1) { temp_a2 = arg0->field_44; }
        temp_a1_7 = temp_a2 - 1;
        if (temp_a2 >= 0x5D) {
            output_48 = 1.0f;
            suppress_direction = 1;
        } else if (temp_a2 >= 0x49) {
            suppress_direction = 1;
        } else if (temp_a2 >= 0x3D) {
            result_mask |= 0x100;
        }
        arg0->field_44 = temp_a1_7;
        if (temp_a1_7 == 0) {
            arg0->field_6C = (s32) (arg0->field_6C & ~0x100);
        }
    }
    if (suppress_direction != 0) {
        arg0->field_38 = 0.0f;
    } else {
        temp_f0 = (f64) arg0->field_3C;
        if (temp_f0 > 1.0) {
            arg0->field_3C = 1.0f;
        } else if (temp_f0 < -1.0) {
            arg0->field_3C = -1.0f;
        }
        arg0->field_38 = arg0->field_3C;
        output_44 = arg0->field_3C;
    }
    temp_v1 = arg0->field_30->field_800;
    temp_v1->field_44 = output_44;
    temp_v1->field_48 = output_48;
    temp_v1 = arg0->field_30->field_800;
    temp_v1->field_40 = result_mask;
}
