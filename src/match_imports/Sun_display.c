/* Native original-compiler source port from src/game_uso/game_uso_pool_B274.c. Target bytes unchanged. */
#include "context_game_uso_pool_B274.h"
#include "game/game_uso_data.h" /* kRoD_* thresholds */
#include "gdl/gfxlib.h"
#include "uso_call.h"
void Sun_display(GameUsoB274Object *s0) {
    float vec[4];
    GameUsoB274Dispatch *table;
    float r;
    if ((double)game_uso_B274_context[0].field_2A0 > *(double *)(&game_uso_D_807FFA40 + kRoD_0p98_120)) return;
    vec[1] = s0->field_A0[0];
    vec[2] = s0->field_A0[1];
    vec[3] = s0->field_A0[2];
    if (XCALL(game_uso_func_063734, Window_projectPoint)(game_uso_B274_context[0].context, &vec[1], 1, 0) == 0) return;
    s0->field_D0 = (int)vec[1];
    s0->field_D4 = (int)vec[2];
    import_000F8678(game_uso_B274_context, &s0->field_DC);
    s0->field_D8 = game_uso_B274_context[0].context;
    s0->field_C8 = s0->field_CC;
    if (0.7 < (double)game_uso_B274_context[0].field_2A0) {
        s0->field_C8 *= (1.0 - (((double)game_uso_B274_context[0].field_2A0 - 0.7) / *(double *)(&game_uso_D_807FFA50 + kRoD_0p3_130)));
    }
    if (!(*(double *)(&game_uso_D_807FFA58 + kRoD_0p1_138) < (double)s0->field_C8)) return;
    XCALL(game_uso_func_04D700, MNode_display)(s0);
    if (1) {
        /* the sun's model-view translation; import_8005C1A8 is gGfxLib (see game_uso_ncc_B750.cpp) */
        /* (unsigned) keeps the target's separate +0xA0 (modelView) add before the [3][i] stores */
        ((float (*)[4])(unsigned)GFXLIB(&import_8005C1A8)->modelView)[3][0] = s0->field_A0[0];
        ((float (*)[4])(unsigned)GFXLIB(&import_8005C1A8)->modelView)[3][1] = s0->field_A0[1];
        ((float (*)[4])(unsigned)GFXLIB(&import_8005C1A8)->modelView)[3][2] = s0->field_A0[2];
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
