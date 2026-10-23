/* Native original-compiler source port from src/game_libs/game_libs_o2_31608.c. Target bytes unchanged. */
#include "context_game_libs_o2_31608.h"
void gl_func_00031608(GlState2E0C4 *a0, int a1) {
    int b21 = a0->field_21;
    char *base;
    int val;
    int g;
    base = gl_31608_storage + 0x368;
    val = a0->field_22 - 1;
    if (gl_31608_gate != 4) return;
    if (a0->field_22 >= 6) {
        if (a0->field_22 != 8) return;
        val = 5;
    }
    if (b21 == 2) {
        g = gl_31608_state;
        if (g == 1) return;
        if (g == 2) return;
        if (g == 3) return;
        if (g == 5) return;
    }
    if (b21 == 2) b21 = 1;
    ((Ring3183C *)(base + b21 * 100))->field_62 = val;
    gl_31608_set((Ring3183C *)(base + b21 * 100),
                 1, val * 64 + a1, 0x7F);
}
