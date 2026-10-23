/* Native original-compiler source port from src/bootup_uso/bootup_uso_o2_BF8C.c. Target bytes unchanged. */
#include "context_bootup_uso_o2_BF8C.h"
void func_0000BF8C(BootupPhysicsState *arg0) {
    f32 temp_f0;
    f32 temp_f0_2;
    f32 var_f12;
    f32 var_f2;
    s16 temp_v0_2;
    s32 temp_v0_3;
    BootupPhysicsInput *temp_v0;
    var_f12 = 0.0f;
    temp_v0 = arg0->field_800;
    arg0->field_990 = (f64)temp_v0->field_004 < -0.75;
    arg0->field_9CC = (temp_v0->field_010 & 0x400) != 0;
    arg0->field_9F8 = arg0->field_490;
    if (arg0->field_9CC != 0) {
        arg0->field_A00 = arg0->field_6F0 * arg0->field_508;
        arg0->field_9FC = arg0->field_4F0;
        arg0->field_360 = arg0->field_4D8;
    } else {
        arg0->field_A00 = arg0->field_708 * arg0->field_508;
        arg0->field_9FC = arg0->field_4C0;
        arg0->field_360 = arg0->field_4A8;
    }
    if (arg0->field_990 != 0) {
        if (arg0->field_9CC != 0) {
            var_f12 = bootup_BF8C_constants[8];
        } else {
            var_f12 = 0.5f;
        }
        temp_f0 = arg0->field_970;
        if (temp_f0 < 0.0f) {
            var_f2 = -temp_f0;
        } else {
            var_f2 = temp_f0;
        }
        arg0->field_360 = (f32) ((arg0->field_360) + (var_f2 * var_f12));
    }
    temp_v0_2 = arg0->field_9A2;
    if (temp_v0_2 != 0x63) {
        if (temp_v0_2 != 0x61) {
            if (temp_v0_2 != 0x62) {
                arg0->field_360 += bootup_BF8C_double[-5];
                arg0->field_9FC = 0.0f;
                arg0->field_9F8 = 0.0f;
                goto mode_done;
            } else {
                goto mode_62;
            }
        } else {
            goto mode_done;
        }
    } else {
        goto mode_63;
    }
mode_63:
    if (1) {
        arg0->field_360 *= arg0->field_448;
        arg0->field_9FC *= arg0->field_430;
    }
    goto mode_done;
mode_62:
    if (1) {
        arg0->field_360 *= arg0->field_478;
        arg0->field_9FC *= arg0->field_460;
    }
mode_done:
    temp_v0_3 = arg0->field_960;
    arg0->field_9FC = (f32) ((arg0->field_9FC) * (arg0->field_5D0));
    arg0->field_360 = (f32) ((arg0->field_360) * (arg0->field_5E8));
    if (temp_v0_3 != 0) {
        arg0->field_9FC = 0.0f;
        arg0->field_360 = (f32) ((arg0->field_360) + ((f32) temp_v0_3 / 100.0f));
        temp_f0_2 = 1.0f - arg0->field_6D8;
        arg0->field_318 = (f32) ((arg0->field_318) * temp_f0_2);
        arg0->field_320 = (f32) ((arg0->field_320) * temp_f0_2);
    }
    bootup_BF8C_call0(arg0);
    bootup_BF8C_call1(arg0);
    bootup_BF8C_call2(bootup_BF8C_ref20 + 0x20);
    bootup_BF8C_call3(arg0);
    bootup_BF8C_call4(bootup_BF8C_ref2C + 0x2C);
    bootup_BF8C_call5(arg0);
    if ((arg0->field_960 == 0x64) && ((arg0->field_348) < 30.0f)) {
        arg0->field_318 = 0.0f;
        arg0->field_320 = 0.0f;
    }
    bootup_BF8C_call6(arg0);
    bootup_BF8C_call7(arg0);
}
