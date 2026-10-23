/* Native original-compiler source port from src/game_libs/game_libs.c. Target bytes unchanged. */
#include "context_game_libs.h"
void game_libs_func_00000B94(GameLibsTwoFieldDisplay *arg0, s32 arg1, s32 arg2) {
    GameLibsTwoFieldDisplay *obj = arg0;
    s32 centiseconds;
    s32 rem;
    s32 old_counter;
    rem = arg1 - (arg1 / 60000) * 60000;
    obj->field_D0 = (rem / 1000) + (arg1 / 60000) * 60;
    obj->field_C8 = arg2;
    if (obj->field_D0 == 0) {
        centiseconds = (rem - (rem / 1000) * 1000) / 10;
        if (centiseconds == 0) goto set8;
    }
    centiseconds = (rem - (rem / 1000) * 1000) / 10;
    if (obj->field_C8 == 0) {
set8:
        obj->field_C4 = 8;
    }
    arg2 = obj->field_D0;
    if (arg2 >= 100) {
        game_libs_B94_format(obj->field_B4, (char *)&D_00000000 + 0xCB9C, arg2);
    } else if (arg2 >= 10) {
        game_libs_B94_format(obj->field_B4, (char *)&D_00000000 + 0xCBA0, arg2);
    } else {
        if (arg2 != obj->field_CC) {
            game_libs_B94_notify(0x13, arg2);
        }
        old_counter = obj->field_C4;
        obj->field_C4 = old_counter + 1;
        if (old_counter & 8) {
            game_libs_B94_format(obj->field_B4, (char *)&D_00000000 + 0xCBA8, obj->field_D0);
        } else {
            game_libs_B94_format(obj->field_B4, (char *)&D_00000000 + 0xCBB0, obj->field_D0);
        }
        obj->field_CC = obj->field_D0;
    }
    game_libs_B94_format(obj->field_BC, (char *)&D_00000000 + 0xCBB8, centiseconds);
}
