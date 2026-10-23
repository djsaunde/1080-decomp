/* Native original-compiler source port from src/game_libs/game_libs_post1b.c. Target bytes unchanged. */
#include "context_game_libs_post1b.h"
void gl_func_000693A4(GLVertexElement *arg0, GLVertexContext *arg1, GLVertexS16 *arg2) {
    f32 kscale;
    f32 v[7];
    f32 o0;
    f32 o1;
    f32 o2;
    f32 ex;
    f32 ey;
    f32 ez;
    f32 scale;
    s16 idx;
    s16 mode;
    s32 var_a3;
    s32 var_v1;
    GLVertexEntry *entry;
    GLVertexS16 *src;
    f32 *mtx;
    GLVertexS16 *out;
    ex = arg0->transform->matrix[12] - arg1->field_38->matrix[12];
    ey = arg0->transform->matrix[13] - arg1->field_38->matrix[13];
    ez = arg0->transform->matrix[14] - arg1->field_38->matrix[14];
    var_a3 = 0;
    if (arg0->count > 0) {
        mode = 0;
        o0 = ex;
        o1 = ey;
        o2 = ez;
        kscale = gl_693A4_rodata.weight_scale;
        var_v1 = 0;
        do {
            entry = (GLVertexEntry *)((s32)arg0->entries + var_v1);
            mode = entry->weight;
            if (mode > 0) {
                idx = entry->index;
                src = (GLVertexS16 *)((s32)arg0->vertices + (var_a3 * 6));
                v[0] = (f32)src->x;
                v[1] = (f32)src->y;
                v[2] = (f32)src->z;
                mtx = arg0->transform->matrix;
                o0 = mtx[8] * v[2] + (mtx[4] * v[1] + v[0] * mtx[0]);
                o1 = mtx[9] * v[2] + (mtx[5] * v[1] + v[0] * mtx[1]);
                o2 = mtx[10] * v[2] + (mtx[6] * v[1] + v[0] * mtx[2]);
                v[0] = o0 + ex; v[1] = o1 + ey;
                v[2] = o2 + ez;
                out = (GLVertexS16 *)((s32)arg2 + (idx * 6));
                if (mode == 0x64) {
                    out->x = (s16) (s32) v[0];
                    out->y = (s16) (s32) v[1];
                    out->z = (s16)(s32) v[2];
                    goto vertex_done;
                }
                scale = (f32) mode * kscale;
                out->x += (s32)(v[0] * scale);
                out->y += (s32)(v[1] * scale);
                out->z += (s32)(v[2] * scale);
vertex_done:
                ;
            }
            var_a3 += 1;
            var_v1 += 4;
        } while (var_a3 < arg0->count);
    }
}
