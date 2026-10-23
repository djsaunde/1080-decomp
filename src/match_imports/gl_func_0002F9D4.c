/* Native original-compiler source port from src/game_libs/game_libs_post.c. Target bytes unchanged. */
#include "context_game_libs_post.h"
void gl_func_0002F9D4(s32 selector, s32 byte_value, s32 level, s32 raw_word,
                     s32 byte_10, s32 byte_14) {
    s32 *word_home = &raw_word;
    s32 op = (selector == 0) ? 1 : 5;
    selector = (op & 0xFF) << 8;
    op = byte_value;
    gl_2F9D4_byte(0x06000000 | selector | 5, (s8)op);
    gl_2F9D4_float(0x01000000 | selector, (f32)level / 127.0f);
    gl_2F9D4_word(0x04000000 | selector, raw_word);
    gl_2F9D4_byte(0x06000000 | selector, (s8)byte_10);
    gl_2F9D4_byte(0x03000000 | selector, (s8)byte_14);
}
