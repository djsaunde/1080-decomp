/* Native original-compiler source port from src/game_libs/game_libs_post0b.c. Target bytes unchanged. */
#include "context_game_libs_post0b.h"
void gl_func_00046BC4(int *a0) {
    int *s0 = a0;
    int *clear_flags = (int*)((char*)s0 + 0x1C4);
    int *state_flags = (int*)((char*)s0 + 0x138);
    int *child;
    int condition;
    gl_46BC4_first(a0);
    *clear_flags &= ~1;
    gl_46BC4_second(s0);
    gl_46BC4_third(s0);
    child = (int*)((char*)*(int**)((char*)s0 + 0x254) + 0x100);
    condition=child[1];
    condition &=2;
    if (condition == 0) {
        goto clear_state;
    }
    state_flags[1] |= 2;
    goto done;
clear_state:
    state_flags[1] &= ~2;
done:
    ;
}
