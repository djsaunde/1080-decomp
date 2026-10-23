/* Native original-compiler source port from src/game_libs/game_libs_post0b.c. Target bytes unchanged. */
#include "context_game_libs_post0b.h"
s32 gl_func_0003CBB4(GLSpatialSphere *arg0, GLSpatialSphere *arg1, GLSpatialContact *arg2) {
    s32 ret;
    f32 *av;
    f32 *bv;
    Vec3 dv;
    s32 type;
    f32 rel[3];
    f32 apos[3];
    f32 start[3];
    f32 bpos[3];
    f32 out[3];
    f32 tmp[3];
    volatile int pad68[1];
    f32 dvel[3];
    f32 fzero;
    f32 tz;
    f32 ty;
    f32 tx;
    f32 sum[3];
    ret = 0;
    type = arg1->base.field_08;
    if (type == 1) {
        fzero = 0.0f;
        if (((dv.z = arg0->base.position_18.z - arg1->base.position_18.z) < fzero ? -(dv.z = arg0->base.position_18.z - arg1->base.position_18.z) : (dv.z = arg0->base.position_18.z - arg1->base.position_18.z)) < 1000.0f) {
            if (((dv.x = arg0->base.position_18.x - arg1->base.position_18.x) < fzero ? -(dv.x = arg0->base.position_18.x - arg1->base.position_18.x) : (dv.x = arg0->base.position_18.x - arg1->base.position_18.x)) < 1000.0f) {
                av = (f32 *)&arg0->base.vector_24;
                bv = (f32 *)&arg1->base.vector_24;
                if (((dv.y = arg0->base.position_18.y - arg1->base.position_18.y) < fzero ? -(dv.y = arg0->base.position_18.y - arg1->base.position_18.y) : (dv.y = arg0->base.position_18.y - arg1->base.position_18.y)) <
                    arg0->extent_38 + arg1->extent_38) {
                    tx = av[0] - bv[0];
                    ty = av[1] - bv[1];
                    tz = av[2] - bv[2];
                    dvel[0] = tx;
                    dvel[1] = ty;
                    dvel[2] = tz;
                    if (1) {
                        *(Tri3i *)tmp = *(Tri3i *)dvel;
                        *(Tri3i *)rel = *(Tri3i *)tmp;
                    }
                    *(Tri3i *)apos = *(Tri3i *)&arg0->base.position_18;
                    if (1) { tx = rel[0] + apos[0], ty = rel[1] + apos[1], tz = rel[2] + apos[2]; sum[2] = tz; sum[1] = ty; sum[0] = tx; }
                    *(Tri3i *)tmp = *(Tri3i *)sum;
                    *(Tri3i *)start = *(Tri3i *)tmp;
                    *(Tri3i *)bpos = *(Tri3i *)&arg1->base.position_18;
                    start[1] = fzero;
                    apos[1] = fzero;
                    bpos[1] = fzero;
                    if (gl_3cbb4_segment_3a58c(apos, start, bpos,
                            arg0->radius_34 + arg1->radius_34, out) != 0) {
                        gl_3cbb4_magnitude_5bbcc(rel[2] * rel[2] + (rel[1] * rel[1] + rel[0] * rel[0]));
                        arg2->field_0C = -(rel[2] * out[2] + (rel[1] * out[1] + out[0] * rel[0]));
                        if (arg2->field_0C > fzero) {
                            gl_3cbb4_normalize_5c9bc((f32 *)&dv);
                            ret = 1;
                            *(Tri3i *)tmp = *(Tri3i *)out;
                            arg2->normal_00.x = tmp[0];
                            arg2->normal_00.y = tmp[1];
                            arg2->normal_00.z = tmp[2];
                        }
                    } else if (arg1->base.flags_10 & 1) {
                        arg2->normal_00.x = fzero;
                        arg2->normal_00.y = fzero;
                        arg2->normal_00.z = fzero;
                        arg2->field_0C = fzero;
                        ret = 1;
                    }
                }
            }
        }
    } else {
        ret = arg1->base.dispatch_30->query_0C(
            arg1->base.dispatch_30->adjust_08 + (char *)arg1, arg0, arg2);
    }
    return ret;
}
