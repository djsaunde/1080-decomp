/* Native original-compiler source port from src/game_libs/game_libs_post.c. Target bytes unchanged. */
#include "context_game_libs_post.h"
void gl_func_0003061C(s32 duration, s32 hold, s32 mode) {
    S307A8 *g = (S307A8 *)&D_00000000;
    s32 state;
    s32 remaining;
    remaining = g->cnt;
    if (g->flag != 4) {
        state = g->flag;
        if (state == 2) {
            if (mode == 0) {
                mode = 8;
            }
            if (mode == 1) {
                mode = 0xA;
            }
            if (mode == 3) {
                mode = 0xB;
            }
            if (mode == 4) {
                mode = 0xB;
            }
            if (mode == 5) {
                mode = 0xB;
            }
        }
        if ((remaining != 0) && (remaining < duration)) {
            if (mode == g->w4) {
                g->hold = hold;
            } else {
                g->acc /= 1.5f;
                g->cnt = duration;
                if (duration != 0) {
                    g->step = (1.0f - g->acc) / (f32) g->cnt;
                }
                g->hold = hold;
                remaining = 0;
            }
        } else {
            g->cnt = duration;
            if (duration != 0) {
                g->step = (1.0f - g->acc) / (f32) g->cnt;
            }
            g->hold = hold;
        }
        if ((state != 0) && ((mode != g->w4) || (remaining == 0)) && (g->cnt != 0)) {
            g->w4 = mode;
            gl_3061C_emit(0x06000800, (s8) mode);
        }
    }
}
