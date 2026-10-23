/* Native original-compiler source port from src/game_libs/game_libs_post0b.c. Target bytes unchanged. */
#include "context_game_libs_post0b.h"
int gl_func_0003A58C(float *a, float *b, float *c, float r, float *out) {
    float step[3];
    float v_oc[3];
    float v_ca[3];
    float dir[3];
    float dot;
    float disc;
    float v_bc[3];
    volatile int pad_F8[1];
    Vec3 tmp;
    float pad_D8;
    Vec3 d1;
    float tx, ty, tz;
    float zero;
    volatile int pad5a[1];
    Vec3 d2;
    Vec3 d3;
    Vec3 d4;
    float work_x, pad_98, work_z;
    float pad_90, pad_8C;
    float scalar, pad_84;
    Vec3 q1;
    float pad_78;
    Vec3 pt;
    float scx;
    volatile int pad5b[4];
    Vec3 q2;
    volatile int pad2[2];
    Vec3 d5;
    Vec3 sc;
    volatile int bottom[1];
    zero = 0.0f;
    tx = c[0] - a[0];
    ty = c[1] - a[1];
    tz = c[2] - a[2];
    d1.x = tx; d1.y = ty; d1.z = tz;
    if (1) {
        tmp = d1;
        *(Tri3i *)v_ca = *(Tri3i *)&tmp;
    }
    tx = b[0] - a[0];
    ty = b[1] - a[1];
    tz = b[2] - a[2];
    d2.x = tx; d2.y = ty; d2.z = tz;
    if (1) {
        tmp = d2;
        *(Tri3i *)dir = *(Tri3i *)&tmp;
    }
    gl_func_00000000_3a58cn((Vec3 *)dir);
    dot = (v_ca[2] * dir[2]) + ((v_ca[1] * dir[1]) + (dir[0] * v_ca[0]));
    disc = (r * r) - (((v_ca[2] * v_ca[2]) + ((v_ca[1] * v_ca[1]) + (v_ca[0] * v_ca[0]))) - (dot * dot));
    if (disc < zero) {
        return 0;
    }
    tx = b[0] - c[0];
    ty = b[1] - c[1];
    tz = b[2] - c[2];
    d3.x = tx; d3.y = ty; d3.z = tz;
    if (1) {}
    tmp = d3;
    *(Tri3i *)v_bc = *(Tri3i *)&tmp;
    if (r < gl_func_00000000_3a58cs((v_bc[2] * v_bc[2]) + ((v_bc[1] * v_bc[1]) + (v_bc[0] * v_bc[0])))) {
        return 0;
    }
    scalar = dot - gl_3a58c_sqrt_082880(disc);
    work_x = dir[0] * scalar;
    tz = dir[1] * scalar;
    work_z = dir[2] * scalar;
    d4.x = work_x; d4.y = tz; d4.z = work_z;
    if (1) {
        tmp = d4;
        *(Tri3i *)step = *(Tri3i *)&tmp;
    }
    if (scalar) {}
    work_z = step[0] + a[0];
    tx = step[1] + a[1];
    ty = step[2] + a[2];
    pt.x = work_z; pt.y = tx; pt.z = ty;
    q1 = pt;
    q2 = q1;
    out[0] = q2.x;
    out[1] = q2.y;
    out[2] = q2.z;
    if (scalar) {}
    tx = out[0] - c[0];
    ty = out[1] - c[1];
    tz = out[2] - c[2];
    d5.x = tx; d5.y = ty; d5.z = tz;
    if (1) {
        tmp = d5;
        *(Tri3i *)v_oc = *(Tri3i *)&tmp;
    }
    work_z = v_oc[0] / r; scalar = v_oc[1] / r; work_x = v_oc[2] / r;
    sc.x = work_z; sc.y = scalar; sc.z = work_x;
    q1 = sc;
    q2 = q1;
    out[0] = q2.x;
    out[1] = q2.y;
    out[2] = q2.z;
    return 1;
}
