/* Native original-compiler source port from src/game_libs/game_libs_post.c. Target bytes unchanged. */
#include "context_game_libs_post.h"
void game_libs_func_00029BC8(struct GameLibsAnimRecord *obj) {
    u16 state;
    struct GameLibsAnimDescriptor *desc;
    struct GameLibsAnimChannel *channel;
    obj->field8C.field14 = 1;
    obj->field8C.field04 = 0;
    obj->field3C = 1.0f;
    obj->field8C.field08 = gl_anim_defaults_1236.field08;
    obj->field8C.field00 = obj->field44->field50;
    channel = &obj->field8C;
    desc = channel->field00;
    channel->field18 = state = desc->field1A;
    if (state == 0) {
        do {} while (0);
        channel->field0C = (f32)(s32)desc->field16;
    } else {
        do {} while (0);
        channel->field0C = (f32)(s32)desc->field12;
    }
    state = desc->field18;
    channel->field16 = state;
    if (state == 0) {
        channel->field10 = (f32)(s32)desc->field14;
    } else {
        channel->field10 = (f32)(s32)desc->field10;
    }
    channel->field1A = desc->field1C;
}
