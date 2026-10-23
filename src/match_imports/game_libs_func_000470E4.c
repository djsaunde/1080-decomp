/* Native original-compiler source port from src/game_libs/game_libs_post0b.c. Target bytes unchanged. */
#include "context_game_libs_post0b.h"
void game_libs_func_000470E4(GL470E4Self *self, const f32 *c) {
    float r, g;
    unsigned char col[4];
    float b, a;
    GL470E4Object *obj; GL470E4List *gp; GL470E4Packet *p; s32 i;
    obj = self->context->object;
    r = c[0];
    col[0] = (g = c[1], b = c[2], a = c[3], r * 255.0f);
    col[1] = g * 255.0f;
    col[2] = b * 255.0f;
    col[3] = a * 255.0f;
    gp = obj->list; i = gp->count; gp->count = i + 1;
    p = obj->list->packets + i;
    p->command = 0xFA000000;
    p->color = (col[3] & 0xFF) | (((col[2] & 0xFF) << 8) | (((col[1] & 0xFF) << 16) | ((col[0] & 0xFF) << 24)));
}
