/* Native original-compiler source port from src/game_libs/game_libs_post0b.c. Target bytes unchanged. */
#include "context_game_libs_post0b.h"
void game_libs_func_00035E64(char *o) {
    float sy;
    float lerp;
    float initial_distance;
    Vec3 point;
    float result[11];
    float f200;
    float sx;
    float tmp3[4];
    float tmp2[4];
    float tmp1[4];
    float sz, ox, oy, oz;
    struct { char *context; float distance; } state;
    state.context = D_00000000_50;
    if (state.context == 0) return;
    point.x = *(float *)(o + 0x30);
    point.y = *(float *)(o + 0x34);
    point.z = *(float *)(o + 0x38);
    if (gl_query_0003C43C_35E64(D_00000000_50, &point.x, 1000.0f, result) == 0) return;
    do { state.distance = result[3]; } while (0);
    f200 = 200.0f;
    if ((initial_distance = state.distance) < f200) {
        state.distance = initial_distance - f200;
        state.distance = -state.distance;
        sx = result[0] * state.distance;
        sy = result[1] * state.distance;
        sz = result[2] * state.distance;
        tmp1[0] = sx;
        tmp1[1] = sy;
        tmp1[2] = sz;
        do {
        lerp = *(float *)((char *)&D_00000000 + 0x19E0);
        *(Vec3 *)tmp2 = *(Vec3 *)tmp1;
        *(Vec3 *)tmp3 = *(Vec3 *)tmp2;
        } while (0);
        do {
        point.x += tmp3[0];
        point.y += tmp3[1];
        point.z += tmp3[2];
        ox = *(float *)(o + 0x30);
        oy = *(float *)(o + 0x34);
        oz = *(float *)(o + 0x38);
        *(float *)(o + 0x30) = (point.x - ox) * lerp + (0, ox);
        *(float *)(o + 0x34) = (point.y - oy) * lerp + (0, oy);
        *(float *)(o + 0x38) = (point.z - oz) * lerp + (0, oz);
        } while (0);
    }
}
