/* Native original-compiler source port from src/game_libs/game_libs_post1b.c. Target bytes unchanged. */
#include "context_game_libs_post1b.h"
void gl_func_000697C4(GlState695F4 *arg0) {
    register f32 temp_f14;
    register f32 temp_f4;
    register f32 temp_f8;
    f32 sp38;
    f32 sp34;
    f32 sp30;
    struct Cursor697C4 { char **node; char **next; } cursor;
    struct Cursor697C4 *pc = &cursor;
    f32 sp24;
    f32 sp20;
    f32 temp_f0;
    f32 temp_f0_2;
    f32 temp_f6;
    s16 *temp_v0_4;
    s16 temp_t5;
    s32 temp_a0;
    u32 product;
    s32 temp_t1;
    s32 amplitude;
    s32 temp_v0;
    s32 temp_v1_3;
    s32 var_a0;
    s32 var_a1;
    u32 var_a3;
    u32 var_v0;
    char **temp_t6;
    char **temp_t9;
    char *temp_v0_2;
    char *temp_v0_3;
    char *temp_v0_5;
    char *temp_v0_6;
    char *var_v0_2;
    char *var_v0_3;
    u32 *rng;
    f32 *fbar;
    char ***pbar;
    if (1) {
        fbar = &sp38; fbar = &sp34; fbar = &sp30;
        fbar = &sp24; fbar = &sp20;
        pbar = &pc->next; pbar = &pc->node;
    }
    temp_v0 = gl_697C4_tick.value;
    var_a3 = (u32)arg0->field_4C - (u32)temp_v0;
    if (var_a3 != 0) {
        arg0->field_4C = temp_v0;
        if (arg0->field_38 == 0) {
            gl_697C4_report((char*)((char*)&D_00000000 + 0x2C5CC));
        }
        var_a3 = 0;
        var_a0 = 0;
        var_v0 = *(u32 *)((char *)arg0->field_34 + 0x40);
        var_a1 = 0;
        if (var_v0 != 0) {
            f32 mult = gl_697C4_scale.value;
            do {
                temp_f0 = (f32) *(s16*)((char *)arg0->field_30 + var_a0);
                if (1) {}
                if (temp_f0 != 0.0f) {
                    temp_f0 *= mult;
                    temp_v1_3 = (s32)((char *)arg0->field_3C + var_a1);
                    sp30 = (f32) *(s16*)((char*)temp_v1_3 + 0x0);
                    sp34 = (f32) *(s16*)((char*)temp_v1_3 + 0x2);
                    sp38 = (f32) *(s16*)((char*)temp_v1_3 + 0x4);
                    temp_v0_3 = (char *)arg0->field_38;
                    if (1) { temp_v0_3 += 0x70; }
                    temp_f14 = (*(f32*)((char*)temp_v0_3 + 0x20) * sp38) + ((*(f32*)((char*)temp_v0_3 + 0x10) * sp34) + (*(f32*)((char*)temp_v0_3 + 0x0) * sp30));
                    temp_f4 = (*(f32*)((char*)temp_v0_3 + 0x24) * sp38) + ((*(f32*)((char*)temp_v0_3 + 0x14) * sp34) + (*(f32*)((char*)temp_v0_3 + 0x4) * sp30));
                    temp_f8 = (*(f32*)((char*)temp_v0_3 + 0x28) * sp38) + ((*(f32*)((char*)temp_v0_3 + 0x18) * sp34) + (*(f32*)((char*)temp_v0_3 + 0x8) * sp30));
                    sp34 = temp_f4;
                    sp38 = temp_f8;
                    sp30 = temp_f14;
                    temp_f0_2 = temp_f0;
                    *(s16*)((char *)arg0->field_40 + var_a1) = (s16) (s32) (sp30 * temp_f0_2);
                    *(s16*)(((char *)arg0->field_40 + var_a1) + 0x2) = (s16) (s32) (sp34 * temp_f0_2);
                    *(s16*)(((char *)arg0->field_40 + var_a1) + 0x4) = (s16) (s32) (sp38 * temp_f0_2);
                } else {
                    *(s16*)(((char *)arg0->field_40 + var_a1) + 0x4) = 0;
                    *(s16*)(((char *)arg0->field_40 + var_a1) + 0x2) = 0;
                    *(s16*)((char *)arg0->field_40 + var_a1) = 0;
                }
                var_a3 += 1;
                var_a0 += 2;
                var_v0 = *(u32 *)((char *)arg0->field_34 + 0x40);
                var_a1 += 6;
            } while (var_a3 < var_v0);
        }
        temp_v1_3 = arg0->field_94;
        if (temp_v1_3 != 0) {
            amplitude = temp_v1_3;
            temp_t1 = temp_v1_3 / 2;
            var_a3 = 0;
            if (var_v0 != 0) {
                rng = (u32 *)&D_00000000;
                if (1) { rng += 0x2C4F0 / 4; }
                var_a1 = 0;
                do {
                    var_a3 += 1;
                    temp_v1_3 = (*rng * 4) + 2;
                    product = (u32)temp_v1_3 + 1;
                    temp_v1_3 = ((u32)temp_v1_3 * product) >> 2;
                    *rng = temp_v1_3;
                    temp_v0_4 = (s16 *)((char *)arg0->field_40 + var_a1);
                    *temp_v0_4 = (*temp_v0_4 + ((s32) (((u32)temp_v1_3 & 0xFFFF) * (u32)amplitude) >> 0x10)) - temp_t1;
                    temp_a0 = (*rng * 4) + 2;
                    product = (u32)temp_a0 + 1;
                    temp_a0 = ((u32)temp_a0 * product) >> 2;
                    *rng = temp_a0;
                    temp_v0_4 = (s16 *)((char *)arg0->field_40 + var_a1 + 2);
                    if (1) { temp_v0_4 = temp_v0_4; }
                    *temp_v0_4 = (s16) ((*temp_v0_4 + ((s32) (((u32)temp_a0 & 0xFFFF) * (u32)amplitude) >> 0x10)) - temp_t1);
                    temp_v1_3 = (*rng * 4) + 2;
                    product = (u32)temp_v1_3 + 1;
                    temp_v1_3 = ((u32)temp_v1_3 * product) >> 2;
                    *rng = temp_v1_3;
                    temp_v0_4 = (s16 *)((char *)arg0->field_40 + var_a1 + 4);
                    if (1) { temp_v0_4 = temp_v0_4; }
                    *temp_v0_4 = (*temp_v0_4 + ((s32) (((u32)temp_v1_3 & 0xFFFF) * (u32)amplitude) >> 0x10)) - temp_t1;
                    var_a1 += 6;
                } while (var_a3 < (u32) *(u32 *)((char *)arg0->field_34 + 0x40));
            }
            arg0->field_94 = 0;
        }
        pc->next = (char **)arg0->field_48;
        pc->node = pc->next;
        if (pc->node != 0) {
            pc->next = (*(int *)((char *)(pc->next) + (0x4)));
            var_v0_2 = (*(int *)((char *)(pc->node) + (0x0)));
        } else {
            var_v0_2 = 0;
        }
        if (var_v0_2 != 0) {
            do {
                gl_697C4_modifier(*(GLVertexElement **)pc->node, arg0, arg0->field_40);
                pc->node = pc->next;
                if (pc->node != 0) {
                    pc->next = (*(int *)((char *)(pc->next) + (0x4)));
                    var_v0_3 = (*(int *)((char *)(pc->node) + (0x0)));
                } else {
                    var_v0_3 = 0;
                }
            } while (var_v0_3 != 0);
        }
        gl_697C4_finish(arg0->field_34, (char *)arg0->field_40);
    }
}
