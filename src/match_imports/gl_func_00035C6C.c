/* Native original-compiler source port from src/game_libs/game_libs_o2_35C6C.c. Target bytes unchanged. */
#include "context_game_libs_o2_35C6C.h"
int gl_func_00035C6C(SnwRecordCollection *list) {
    SnwRecordNode *initial = list->field_04;
    Gl35C6CWord sentinel_obj;
    Gl35C6CWord current_obj;
    Gl35C6CWord *sentinel;
    Gl35C6CWord *key;
    Gl35C6CWord *current;
    Gl35C6CWord key_obj;
    int count_obj;
    Gl35C6CWord value_obj;
    SnwRecordNode *node;
    sentinel = &sentinel_obj;
    key = &key_obj;
    current = &current_obj;
    if (initial != 0) {
        do {
            current_obj.value = 0;
            if (initial) {}
            gl_35c6c_read_tag(&current->value);
            if (list) {}
            sentinel_obj.value = -1;
            *key = *sentinel;
            if (key_obj.value == current_obj.value) break;
            node = list->field_04;
            if (node != 0) {
                *key = *current;
                do {
                    if (node->field_04 == key_obj.value) {
                        node->field_0C->field_0C((void *)(node->field_0C->field_08 + (int)node));
                        break;
                    }
                    node = node->field_00;
                } while (node != 0);
            }
            if (node == 0) {
                gl_35c6c_print((char *)&D_00000000 + 0x1E6C0, &current->value);
                gl_35c6c_read_word(&count_obj);
                if (count_obj != 0) {
                    do {
                        gl_35c6c_read_word(&value_obj.value);
                        count_obj -= 4;
                    } while (count_obj != 0);
                }
            }
        } while (initial != 0);
    }
}
