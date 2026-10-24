/* Sibling of timproc_uso_b5_func_000038B0 (identical opcode sequence; fields
 * 0x184/0x188/0x18C instead of 0x164/0x168/0x16C). Plain cfe -O2 C with the
 * same frozen context as 38B0. The shared context still declares this
 * function with the old (char *, s32, char *, f32, f32) placeholder
 * prototype; it is renamed out of the way so the typed definition below is
 * the only declaration. */
#define timproc_uso_b5_func_0000396C timproc_uso_b5_func_0000396C_ctx_placeholder
#include "context_timproc_uso_b5.h"
#undef timproc_uso_b5_func_0000396C
void timproc_uso_b5_func_0000396C(TimprocCoordinateOwner *owner, char *destination,
                                Vec3 *source, f32 blend_arg, f32 height) {
    f32 x, y, z;
    f32 result_x, result_y, result_z;
    f32 *out = (f32 *)(destination + 0x30);
    x = owner->f_184 * owner->f_2A0 + owner->p_29C->f_DC;
    y = owner->f_188 * owner->f_2A0 + owner->p_29C->f_E0 +
        height * (1.0f - owner->f_2A0);
    z = owner->f_18C * owner->f_2A0 + owner->p_29C->f_E4;
    result_x = source->x + (x - source->x) * blend_arg;
    result_y = source->y + (y - source->y) * blend_arg;
    result_z = source->z + (z - source->z) * blend_arg;
    out[12] = result_x;
    out[13] = result_y;
    out[14] = result_z;
}
