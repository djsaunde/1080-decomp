/* Native original-compiler source port from src/game_libs/game_libs_post0b.c. Target bytes unchanged. */
#include "context_game_libs_post0b.h"
void gl_func_00043BEC(GL43BECObject *arg0, GL470E4Object *arg1) {
    void *temp_a0;
    s32 temp_a0_2;
    s32 temp_a2;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v0_4;
    GL470E4Packet *temp_a1;
    GL470E4List *temp_a1_2;
    GL470E4Packet *temp_a3;
    GL43BECPrefix *temp_v0;
    GL470E4List *temp_v1;
    GL470E4Packet *temp_v1_2;
    GL470E4Object *temp_v1_3;
    s32 source_index;
    if (arg0->field_6C != 0) {
        temp_a0 = arg0->field_48;
        if ((temp_a0 != 0) && (((u32)temp_a0 | 0) != (u32)gl_43BEC_data.field_3C7C4)) {
            gl_43BEC_call_44540(temp_a0, arg1);
            gl_43BEC_data.field_3C7C4 = arg0->field_48;
        }
        temp_v0 = arg0->field_60;
        arg0->field_70 = (s32) arg1->list->count;
        if (temp_v0 != 0) {
            temp_v0_2 = gl_43BEC_virtual_to_physical(temp_v0->field_60);
            temp_v1 = arg1->list;
            temp_a0_2 = temp_v1->count;
            temp_v1->count = (s32) (temp_a0_2 + 1);
            temp_a1 = (GL470E4Packet *)((char *)arg1->list->packets + temp_a0_2 * 8);
            temp_a1->command = 0x06000000;
            temp_a1->color = temp_v0_2;
        }
        temp_v1_3 = arg0->field_64;
        source_index = arg0->field_88;
        temp_v0_3 = gl_43BEC_virtual_to_physical((s32)temp_v1_3->list->packets + (source_index * 8));
        temp_a1_2 = arg1->list;
        temp_a2 = temp_a1_2->count;
        temp_a1_2->count = (s32) (temp_a2 + 1);
        temp_a3 = (GL470E4Packet *)((char *)arg1->list->packets + temp_a2 * 8);
        temp_a3->command = 0x06000000;
        temp_a3->color = temp_v0_3;
        temp_v1_3 = arg0->field_6C;
        source_index = arg0->field_8A;
        temp_v0_4 = gl_43BEC_virtual_to_physical((s32)temp_v1_3->list->packets + (source_index * 8), temp_a1_2, temp_a2, temp_a3);
        temp_a1_2 = arg1->list;
        temp_a2 = temp_a1_2->count;
        temp_a1_2->count = (s32) (temp_a2 + 1);
        temp_v1_2 = (GL470E4Packet *)((char *)arg1->list->packets + temp_a2 * 8);
        temp_v1_2->command = 0x06000000;
        temp_v1_2->color = temp_v0_4;
    }
}
