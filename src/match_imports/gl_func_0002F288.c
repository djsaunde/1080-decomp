/* Native original-compiler source port from src/game_libs/game_libs_post.c. Target bytes unchanged. */
#include "context_game_libs_post.h"
void gl_func_0002F288(GlState2E0C4 *arg0) {
    f32 value;
    f32 level;
    f32 scale;
    u8 mode;
    u8 amount;
    s16 kind;
    s32 aux;
    s32 state;
    value = arg0->field_00;
    kind = arg0->field_14;
    state = arg0->field_21;
    if ((state != 2) && (arg0->field_1C == 0)) {
        mode = state == 0 ? 3 : 7;
        {
            s32 base;
            base = *(s32 *)((char *)&D_00000000 + 4);
            value = (f32)base * gl_2F288_pool1730.field_1730 + value;
            if ((kind == 0) && (arg0->field_38 >= 21)) {
                switch (*(s32 *)((char *)&D_00000000 + 0x10)) {
                default:
                case 0:
                    amount = (u8)(s32)(value * 0.5f);
                    scale = 1.0f;
                    break;
                case 1:
                case 2:
                    amount = (u8)(s32)(value / 3.5f);
                    scale = gl_2F288_pool1734[0x1734 / 4];
                    break;
                }
            } else {
                amount = (u8)(s32)(value * 0.25f);
                scale = gl_2F288_pool1738[0x1738 / 4];
            }
            aux = arg0->field_1F;
            amount = (u8)(amount + base);
            if (aux != 0) {
                base = *(s32 *)(&gl_2F288_countdown + 0x1C740) - 1;
                *(s32 *)(&gl_2F288_countdown_store + 0x1C740) = base;
                if (base == 0) {
                    if (kind == 0) {
                        base = aux / 2;
                        if (amount >= base) {
                            amount = (u8)(amount - base);
                        }
                    } else {
                        amount = (u8)(amount + aux);
                    }
                    base = 3 - (*(s32 *)(&gl_2F288_random + 0x2154) & 1);
                    *(s32 *)(&gl_2F288_countdown_reset + 0x1C740) = base;
                } else if ((kind != 0) && (amount >= 10)) {
                    amount = (u8)(amount - 10);
                }
            }
        }
        aux = arg0->field_32;
        level = value / 80.0f;
        level += gl_2F288_pool173C[0x173C / 4];
        if (aux != 0) {
            level *= *(f32 *)((u32)&gl_2F288_tableC330 + (aux * 4) + 0x1C330);
            scale *= *(f32 *)((u32)&gl_2F288_tableC3B0 + (aux * 4) + 0x1C3B0);
        }
        if (kind == 0xFF) {
            aux = (mode & 0xFF) << 8;
            gl_2F288_emit_byte((aux | 0x06000000) | 6, 1);
            level = 1.0f;
        } else {
            aux = (mode & 0xFF) << 8;
            gl_2F288_emit_byte((aux | 0x06000000) | 6, 0);
        }
        if (arg0->field_1C & 1) {
            scale = 0.0f;
        }
        gl_2F288_emit_byte((aux | 0x06000000) | 5, (s8)amount);
        gl_2F288_emit_float(aux | 0x04000000, level);
        gl_2F288_emit_float(aux | 0x01000000, scale);
    }
}
