/* Native original-compiler source port from src/game_libs/game_libs_post0b.c. Target bytes unchanged. */
#include "context_game_libs_post0b.h"
u8 *gl_func_00035268(s32 unused, u32 size) {
    u8 *result = *(u8 **)((char *)&gl_4C928_state_base + 0x218);
    s32 ready;
    f32 divisor = 1024.0f;
    ready = ((struct St4C928 *)result)->p1C != 0;
    if (ready == 0) {
        ready = ((struct St4C928 *)result)->p04 != 0;
        if (ready == 0) ready = ((struct St4C928 *)result)->p0C != 0;
    }
    if (ready != 0) gl_337AC_print(gl_35268_strings + 0x1E54C);
    result = (u8 *)gl_41148_alloc_5B070(&gl_35268_heap, size);
    if (result == 0) {
        gl_337AC_print(gl_35268_strings + 0x1E55C,
                      (double)((f32)size / divisor));
    }
    return result;
}
