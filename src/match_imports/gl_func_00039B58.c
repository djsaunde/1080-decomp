/* Native original-compiler source port from src/game_libs/game_libs_post0b.c. Target bytes unchanged. */
#include "context_game_libs_post0b.h"
void gl_func_00039B58(GLTransformCommandObject *a0, GLCommandHeader *a1) {
    float translation[3];
    union { float values[3]; u32 words[3]; } rotations;
    volatile int gap[2];
    Vec3 uniform_scale;
    float component, angle_x, angle_z;
    char *matrix;
    angle_x = 0.0f;
    switch (a1->field_00) {
    case 7: {
        float *src = (float*)a1->field_04.pointer;
        a0->matrix_2C[3][0] = src[0];
        a0->matrix_2C[3][1] = src[1];
        a0->matrix_2C[3][2] = src[2];
        break;
    }
    case 19:
        translation[0] = a0->matrix_2C[3][0];
        translation[1] = a0->matrix_2C[3][1];
        translation[2] = a0->matrix_2C[3][2];
        matrix = (char *)a0->matrix_2C;
        if (matrix) {}
        gl_cmd_5E7EC(matrix);
        component = a0->field_70;
        if (matrix) {}
        uniform_scale.x = component; uniform_scale.y = component; uniform_scale.z = component;
        if (matrix) {}
        gl_cmd_5E8B0(matrix, &uniform_scale);
        angle_z = a0->angles_74.z;
        angle_x = a0->angles_74.x;
        component = a0->angles_74.y;
        rotations.values[2] = angle_z;
        do { rotations.values[0] = angle_x; } while (0);
        angle_x = rotations.values[0];
        rotations.values[0] = angle_x;
        rotations.values[1] = component;
        if (matrix) {}
        gl_39B58_rotate_bits(matrix, 2, rotations.words[2], 0);
        if (matrix) {}
        gl_39B58_rotate_bits(matrix, 0, rotations.words[0], 0);
        if (matrix) {}
        gl_39B58_rotate_bits(matrix, 1, rotations.words[1], 0);
        if (matrix) {}
        gl_cmd_5E8B0(matrix, (char*)&D_00000000 + 0x128);
        if (matrix) {}
        gl_cmd_5E87C(matrix, translation);
        break;
    default:
        gl_cmd_38830_ignored(a0, a1);
        break;
    }
}
