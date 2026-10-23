/* Native original-compiler source port from src/game_libs/game_libs_tail.c. Target bytes unchanged. */
#include "context_game_libs_tail.h"
void gl_func_0000D288(D288Self *self) {
    D288Node *child;
    D288Parent *parent;
    void *tmp;
    parent = self->field_44;
    child = parent->field_4C;
    if (child != 0 && child->field_64 <= parent->field_0C) {
        tmp = parent->field_4C->field_84;
        parent->field_4C = tmp;
        if (gl_import_D288(child, self->field_44->field_00 + 0xB4, parent) != 0) {
            tmp = self->field_28;
            ((D288Dispatch *)tmp)->field_64(
                ((D288Dispatch *)tmp)->field_60 + (char *)self, child);
        }
    }
}
