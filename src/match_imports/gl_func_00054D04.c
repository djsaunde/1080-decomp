/* Native original-compiler source port from src/game_libs/game_libs_post0b.c. Target bytes unchanged. */
#include "context_game_libs_post0b.h"
void gl_func_00054D04(char *self, float *plane, Vec3 *out) {
    Vec3 position;
    Vec3 direction;
    float nx, ny;
    double scale;
    struct { Vec3 v; } tmp;
    Vec3 negative;
    char *matrix;
    GLLightPrefix *light;
    Vec3 *p;
    float nz;
    p = &position;
    if (gl_54d04_get(&gl_54d04_root) != 0) {
        light = gl_54d04_get(&gl_54d04_root);
        scale = gl_54d04_scale.value;
        position.x = light->field_A0.x;
        position.y = light->field_A0.y;
        position.z = light->field_A0.z;
        position.x = (float)(p->x * scale);
        position.y = p->y + gl_54d04_bias.value;
        position.z = (float)(p->z * scale);
        if (0) gl_54d04_normalize(&position.x);
        if (1) {
            nx = p->x * -1.0f;
            negative.x = nx;
            ny = p->y * -1.0f;
            negative.y = ny;
            nz = p->z * -1.0f;
            negative.z = nz;
        }
        tmp.v = negative;
        direction = tmp.v;
        gl_54d04_normalize(&direction.x);
        if (out != 0) {
            tmp.v = direction;
            out->x = tmp.v.x;
            out->y = tmp.v.y;
            out->z = tmp.v.z;
        }
        matrix = self + 0x12C;
        gl_54d04_project(matrix, &direction, plane);
        *(char **)(&gl_54d04_root + 0xE4) = matrix;
    } else {
        gl_54d04_report(&gl_54d04_strings + 0x210EC);
    }
}
