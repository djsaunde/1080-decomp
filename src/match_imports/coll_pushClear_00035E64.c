/* Native original-compiler source port from src/game_libs/game_libs_post0b.c. Target bytes unchanged. */
#include "context_game_libs_post0b.h"
/* o is a node's local matrix (bootup ncc_4948 passes cam.mnode.local): row 3 is its translation.
 * The point is moved out along the nearest course hit's normal (gCourseColl, CollShape_nearestHit)
 * when it is within 200 units of the surface, eased by the lerp factor. */
#define TRANS(i) (((float (*)[4])o)[3][i])
void coll_pushClear_00035E64(char *o) {
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
    point.x = TRANS(0);
    point.y = TRANS(1);
    point.z = TRANS(2);
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
        ox = TRANS(0);
        oy = TRANS(1);
        oz = TRANS(2);
        TRANS(0) = (point.x - ox) * lerp + (0, ox);
        TRANS(1) = (point.y - oy) * lerp + (0, oy);
        TRANS(2) = (point.z - oz) * lerp + (0, oz);
        } while (0);
    }
}
