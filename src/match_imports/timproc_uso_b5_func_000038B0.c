/* Native original-compiler source port from src/timproc_uso_b5/timproc_uso_b5.c. Target bytes unchanged. */
#include "context_timproc_uso_b5.h"
void timproc_uso_b5_func_000038B0(TimprocCoordinateOwner *owner, char *destination,
                                Vec3 *source, f32 blend_arg, f32 height) {
    f32 x, y, z;
    f32 result_x, result_y, result_z;
    f32 *out = (f32 *)(destination + 0x30);
    x = owner->f_164 * owner->f_2A0 + owner->p_29C->f_DC;
    y = owner->f_168 * owner->f_2A0 + owner->p_29C->f_E0 +
        height * (1.0f - owner->f_2A0);
    z = owner->f_16C * owner->f_2A0 + owner->p_29C->f_E4;
    result_x = source->x + (x - source->x) * blend_arg;
    result_y = source->y + (y - source->y) * blend_arg;
    result_z = source->z + (z - source->z) * blend_arg;
    out[12] = result_x;
    out[13] = result_y;
    out[14] = result_z;
}
