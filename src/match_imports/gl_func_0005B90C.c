/* Native original-compiler source port from src/game_libs/game_libs_post0b.c. Target bytes unchanged. */
#include "context_game_libs_post0b.h"
void gl_func_0005B90C(GLAllocatorBlock *self) {
    int first_count;
    int second_count;
    int i;
    short counts[10];
    int totals[10];
    GLAllocatorBlock *node;
    GLAllocatorBlock *sentinel;
    int category;
    first_count = 0;
    second_count = 0;
    for (i = 0; i != 10; i++) { counts[i] = 0; totals[i] = 0; }
    for (node = self->field_04; node != self; node = node->field_04) {
        first_count++;
    }
    sentinel = &((GLAllocatorState *)self)->field_10;
    for (node = sentinel->field_04; node != sentinel; node = node->field_04) {
        category = (node->field_00 >> 24) & 0xFF;
        if (category < 10 && category >= 0) {
            counts[category]++;
            totals[category] += (node->field_00 & 0xFFFFFF) << 4;
        }
        second_count++;
    }
    for (i = 0; i != 10; i++) {
        if (counts[i] != 0) {
            gl_5B90C_log((char *)&D_00000000 + 0x21A6C, i,
                (double)((float)totals[i] / 1024), (int)counts[i]);
        }
    }
    gl_5B90C_dump(self);
    gl_5B90C_log((char *)&D_00000000 + 0x21A84, first_count, second_count);
}
