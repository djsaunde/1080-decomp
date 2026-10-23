/* Native original-compiler source port from src/game_libs/game_libs_o2_40640.c. Target bytes unchanged. */
#include "context_game_libs_o2_40640.h"
void gl_func_00040640(GlObjectPrefix *root) {
    GlObjectLink *iter[2];
    register GlObjectPrefix *node;
    register GlObjectPrefix *obj;
    register GlObjectDispatch *vt;
    register f32 x;
    GL40640Matrix matrix;
    Vec3 scaled;
    register f32 y, z;
    register GlObjectPrefix *current;
    register GlObjectDispatch *current_vt;
    register u32 mask;
    register GlObjectPrefix *masked;
    volatile char pad_38[0x34];
    Vec3 scaled_direct;
    gl40640_push((void *)root->field_0C, root->field_0C);
    iter[1] = root->field_10;
    if (root->field_B0 != 0) {
        switch ((u32)root->field_B0) {
        case 0:
            break;
        case 6:
            gl40640_copy(&gl40640_context, &gl40640_context.field_E8);
            gl40640_mul(&gl40640_context, root->field_70);
            break;
        case 7:
            gl40640_copy(&gl40640_context, &gl40640_context.field_E8);
            break;
        case 1:
            gl40640_copy(&gl40640_context, root->field_70);
            break;
        case 2:
            gl40640_mul(&gl40640_context, root->field_70);
            break;
        case 3:
            gl40640_copy(&gl40640_context, &gl40640_context.field_E8);
            if (1) {
                current_vt = gl40640_current->field_28;
                current = gl40640_current;
                gl40640_mul(&gl40640_context,
                    (void *)current_vt->field_5C(current_vt->field_58 + (s32)current));
            }
            gl40640_mul(&gl40640_context, root->field_70);
            break;
        case 4:
            gl40640_copy(&gl40640_context, &gl40640_context.field_E8);
            if (1) {
                current_vt = gl40640_current->field_28;
                current = gl40640_current;
                gl40640_mul(&gl40640_context,
                    (void *)current_vt->field_5C(current_vt->field_58 + (s32)current));
            }
            break;
        case 5:
            gl40640_reset(&gl40640_context);
            break;
        case 8:
            matrix = gl40640_context.field_E8;
            if (1) {
                x = root->field_70[12] * gl40640_context.field_128.x;
                y = root->field_70[13] * gl40640_context.field_128.y;
                z = root->field_70[14] * gl40640_context.field_128.z;
                scaled.x = x;
                scaled.y = y;
                scaled.z = z;
            }
            gl40640_translate(&matrix, &scaled);
            gl40640_copy(&gl40640_context, &matrix);
            break;
        case 9:
            if (1) {
                x = root->field_70[12] * gl40640_context.field_128.x;
                y = root->field_70[13] * gl40640_context.field_128.y;
                z = root->field_70[14] * gl40640_context.field_128.z;
                scaled_direct.x = x;
                scaled_direct.y = y;
                scaled_direct.z = z;
            }
            gl40640_translate(&gl40640_context.field_A0, &scaled_direct);
            break;
        }
    }
    iter[0] = iter[1];
    node = iter[1] != ((void*)0)
        ? (iter[1] = iter[0]->field_04, iter[0]->field_00) : ((void*)0);
    if (node != ((void*)0)) {
        do {
            obj = iter[0]->field_00;
            if (obj->field_18 & 4) {
                if (root->field_2C & 2) {
                    gl40640_current = root;
                }
                mask = gl40640_mask();
                masked = iter[0]->field_00;
                if (masked->field_20 & mask) {
                    masked->field_14 = root;
                    obj = iter[0]->field_00;
                    vt = obj->field_28;
                    vt->field_1C(vt->field_18 + (s32)obj, obj);
                }
            }
            iter[0] = iter[1];
            node = iter[1] != ((void*)0)
                ? (iter[1] = iter[0]->field_04, iter[0]->field_00) : ((void*)0);
        } while (node != ((void*)0));
    }
    gl40640_pop();
}
