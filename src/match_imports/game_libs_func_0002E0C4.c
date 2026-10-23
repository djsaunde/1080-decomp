/* Native original-compiler source port from src/game_libs/game_libs_post.c. Target bytes unchanged. */
#include "context_game_libs_post.h"
void game_libs_func_0002E0C4(GlState2E0C4 *arg0, s32 arg1, s32 arg2, s32 arg3) {
    f32 temp_f0;
    if (((arg0->field_48 == 0) && !(arg0->field_16 & 4)) || (((arg0->field_30 != 1) || (arg0->field_29 != 3)) && ((s32) arg0->field_29 >= (s32) arg0->field_2D))) {
        arg0->field_30 = arg1;
        arg0->field_16 = arg0->field_16 | 4;
        if (arg1 != 1) {
            if ((temp_f0 = arg0->field_00) >= 50.0f) {
                arg2 = 3;
            } else if (temp_f0 >= 38.0f) {
                arg2 = 2;
            } else if (temp_f0 >= 25.0f) {
                arg2 = 1;
            } else {
                arg2 = 0;
            }
        }
        arg0->field_29 = arg2;
        arg0->field_2E = arg3;
        arg0->field_2D = arg2;
    }
}
