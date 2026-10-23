/* Native original-compiler source port from src/game_libs/game_libs_tail.c. Target bytes unchanged. */
#include "context_game_libs_tail.h"
void game_libs_func_0000CBF0(GameLibsC784 *self) {
    int i;
    union { s32 count; GameLibsC784Record *record; } work;
    float difference;
    CBF0_Bounds *g = (((CBF0_Global *)&D_00000000)->registry->bounds);
    float range = g->hi - g->lo;
    work.count = self->count;
    for (i = 0; i < work.count; i++) {
        if (self->field_2C) {
            GameLibsC784Record *rec = &self->records[i];
            g = (((CBF0_Global *)&D_00000000)->registry->bounds);
            if (g) {}
            if (rec->field_0C < g->lo) rec->percentage = 0.0f;
            else if (g->hi < rec->field_0C) rec->percentage = 100.0f;
            else {
                difference = rec->field_0C - g->lo;
                rec->percentage = difference / range * 100.0f;
            }
            work.record = &self->records[i];
            if (work.record->percentage == 100.0f) work.record->field_34++;
            work.count = self->count;
        } else {
            continue;
        }
    }
    if (self->field_70) game_libs_CBF0_handler0(self->field_70, (int)*self->field_74);
    if (self->field_78) game_libs_CBF0_handler1(self->field_78, *self->field_7C);
    if (self->field_8C) game_libs_CBF0_handler2(self->field_8C, *self->field_90);
    if (self->field_98) game_libs_CBF0_handler3(self->field_98, *self->field_9C);
}
