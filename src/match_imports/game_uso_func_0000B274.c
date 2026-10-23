/* Native original-compiler source port from src/game_uso/game_uso_pool_B274.c. Target bytes unchanged. */
#include "context_game_uso_pool_B274.h"
void game_uso_func_0000B274(GameUsoB274Object *s0) {
    float vec[4];
    GameUsoB274Dispatch *table;
    float r;
    if ((double)game_uso_B274_context[0].field_2A0 > *(double *)(&game_uso_D_807FFA40 + 0x120)) return;
    vec[1] = s0->field_A0[0];
    vec[2] = s0->field_A0[1];
    vec[3] = s0->field_A0[2];
    if (game_uso_func_063734(game_uso_B274_context[0].context, &vec[1], 1, 0) == 0) return;
    s0->field_D0 = (int)vec[1];
    s0->field_D4 = (int)vec[2];
    import_000F8678(game_uso_B274_context, &s0->field_DC);
    s0->field_D8 = game_uso_B274_context[0].context;
    s0->field_C8 = s0->field_CC;
    if (0.7 < (double)game_uso_B274_context[0].field_2A0) {
        s0->field_C8 *= (1.0 - (((double)game_uso_B274_context[0].field_2A0 - 0.7) / *(double *)(&game_uso_D_807FFA50 + 0x130)));
    }
    if (!(*(double *)(&game_uso_D_807FFA58 + 0x138) < (double)s0->field_C8)) return;
    game_uso_func_04D700(s0);
    if (1) {
        *(float *)((char *)(unsigned)(&import_8005C1A8 + 0xA0) + 0x30) = s0->field_A0[0];
        *(float *)((char *)(unsigned)(&import_8005C1A8 + 0xA0) + 0x34) = s0->field_A0[1];
        *(float *)((char *)(unsigned)(&import_8005C1A8 + 0xA0) + 0x38) = s0->field_A0[2];
    }
    r = s0->field_134 * s0->field_C8;
    game_uso_B274_set_prim_color(game_uso_B274_context, s0->field_164);
    ((GameUsoB274Child *)s0->field_C4.word)->field_C0 = s0->field_14C;
    ((GameUsoB274Child *)s0->field_C4.word)->field_B0 = r;
    ((GameUsoB274Child *)s0->field_C4.word)->field_B4 = r;
    table = s0->field_C4.child->field_28;
    s0 = (GameUsoB274Object *)s0->field_C4.child;
    table->field_1C(table->field_18 + (char *)s0);
}
