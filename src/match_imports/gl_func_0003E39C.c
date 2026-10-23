/* Native original-compiler source port from src/game_libs/game_libs_post0b_o2_3E39C.c. Target bytes unchanged. */
#include "context_game_libs_post0b_o2_3E39C.h"
void gl_func_0003E39C(GLLoadObject *arg0) {
    int comparison_word;
    GLLoadKey key;
    int argument_key;
    int count;
    GLLoadObject *child;
    GLLoadDescriptor *descriptor;
    int copied_key;
    GLCollectionMethods *methods;
    int compared_key;
    gl_E39C_reset(arg0);
    gl_E39C_push(gl_E39C_data + 0x1F2EC);
    if (0) snw_3E39C_count_origin(&compared_key);
    switch (0) {
    for (;;) {
    case 0:
        gl_E39C_read(gl_E39C_stream, &key, 4);
        if (key.field_00 == 0) goto done;
        snw_3E39C_count_origin(&count);
        *(GLLoadKey *)&copied_key = key;
        argument_key = copied_key;
        child = gl_E39C_find(arg0, *(GLLoadKey *)&argument_key);
        if (child == 0) {
            descriptor = arg0->field_30;
            if (descriptor != 0) {
                while (descriptor->field_00 != 0) {
                    compared_key = descriptor->field_00->field_00;
                    comparison_word = compared_key;
                    if (key.field_00 == compared_key) {
                        if (descriptor->field_08 & arg0->field_34)
                            child = gl_E39C_create(arg0, descriptor);
                        break;
                    }
                    descriptor++;
                }
            }
        }
        if (child == 0) gl_E39C_missing(gl_E39C_data + 0x1F2FC, &key);
        if (child != 0 && (arg0->field_34 & child->field_44)) {
            methods = child->base_00.methods_28;
            (*(void (**)(char *))((char *)methods + 0x4C))(
                *(short *)((char *)methods + 0x48) + (char *)child);
        } else {
            gl_E39C_seek(gl_E39C_stream, 1, count);
        }
    }
    }
done:
    gl_E39C_finish(arg0);
    gl_E39C_pop();
}
