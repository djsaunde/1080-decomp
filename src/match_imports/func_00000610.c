/* Native original-compiler source port from src/bootup_uso/bootup_uso_o2_0610.c. Target bytes unchanged. */
#include "context_bootup_uso_o2_0610.h"
void func_00000610(GlState2E0C4 **arg0, Vec3 *arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, s32 arg6, char *arg7, s32 arg8) {
    GlState2E0C4 *o;
    f32 sp28;
    int sp24;
    f32 var_f0;
    o = arg0[0];
    if (o != 0) {
        {
            arg5 = (f32) ((f64) arg5 / bootup_0610_scale.value);
            if (arg5 < 0.0f) {
                var_f0 = 0.0f;
            } else if (1.0f < arg5) {
                var_f0 = 1.0f;
            } else {
                var_f0 = arg5;
            }
            o->field_0C = var_f0;
            if (arg4 < 0.0f) {
                var_f0 = -arg4;
            } else {
                var_f0 = arg4;
            }
            arg0[0]->field_04 = (f32) ((f64) var_f0 + bootup_0610_bias.value);
            arg0[0]->field_18 = (s8) (s32) (arg4 * 63.0f);
            arg0[0]->field_1E = (s8) ((arg8 & 0x80) != 0);
            if (arg6 == 0x0) goto mode_0;
            if (arg6 == 0x61) goto mode_1;
            if (arg6 == 0x62) goto mode_2;
            if (arg6 == 0x63) goto mode_3;
            if (arg6 == 0x64) goto mode_4;
            if (1) {
                goto mode_done;
            }
        mode_0:
            if (1) {
                arg0[0]->field_15 = 1;
                arg0[0]->field_14 = 0;
                goto mode_done;
            }
        mode_1:
            if (1) {
                arg0[0]->field_15 = 1;
                arg0[0]->field_14 = 1;
                goto mode_done;
            }
        mode_2:
            if (1) {
                arg0[0]->field_15 = 3;
                arg0[0]->field_14 = 1;
                goto mode_done;
            }
        mode_3:
            if (1) {
                arg0[0]->field_15 = 2;
                arg0[0]->field_14 = 1;
                goto mode_done;
            }
        mode_4:
            if (1) {
                arg0[0]->field_15 = 2;
                arg0[0]->field_14 = 2;
                goto mode_done;
            }
        mode_done:
            var_f0 = 4;
            arg0[0]->field_00 = arg2 / var_f0;
            arg0[0]->field_08 = arg3 / var_f0;
            arg0[0]->field_20 = 1;
            arg0[0]->field_1C = (u8) ((arg8 & 0x8000) != 0);
        }
        if (arg0[0]->field_1C != 0) {
            bootup_0610_decode(arg1, &sp28, &sp24, arg7);
            bootup_0610_apply(arg0[0], sp28, (s8) sp24);
        }
        bootup_0610_select(&D_00006584);
        bootup_0610_reset(arg0[0], 0);
        bootup_0610_finish();
    }
}
