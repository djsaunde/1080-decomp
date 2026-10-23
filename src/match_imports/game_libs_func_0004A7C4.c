/* Native original-compiler source port from src/game_libs/game_libs_post0b.c. Target bytes unchanged. */
#include "context_game_libs_post0b.h"
void game_libs_func_0004A7C4(void *entry) {
    struct GlPendingList4A7C4 *q;
    int index;
    if (!(*(int *)((char *)&D_00000000 + 0x1C4) & 1)) {
        q = *(struct GlPendingList4A7C4 **)((char *)&D_00000000_a[0] + 0x22C);
        if (q->field_0C >= q->field_08) {
            gl_4A7C4_report((char *)&D_00000000_b[0] + 0x2003C);
        }
        index = q->field_0C;
        q->field_0C = index + 1;
        q->field_00[index] = entry;
    }
    gl_4A7C4_render(entry);
}
