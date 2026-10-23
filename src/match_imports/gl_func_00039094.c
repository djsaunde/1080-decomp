/* Native original-compiler source port from src/game_libs/game_libs_o2_39094.c. Target bytes unchanged. */
#include "context_game_libs_o2_39094.h"
void gl_func_00039094(GlObjectPrefix *root) {
    GlObjectLink *iter2[2];
    GlObjectPrefix *node;
    GlObjectDispatch *vt;
    GlObjectPrefix *gr;
    union { int word; const GlObjectMatrix *matrix; } ret;
    iter2[1] = root->field_10;
    snw_39094_push((ret.word = root->field_0C), root->field_0C);
    if (root->field_B0 != 0)
    switch (root->field_B0) {
        case 0:
            break;
        case 7:
            snw_39094_apply(&D_39094_a1, (const GlObjectMatrix *)(D_39094_dat + 0xE8));
            break;
        case 6: {
            snw_39094_apply(&D_39094_a2a, (const GlObjectMatrix *)(D_39094_dat + 0xE8));
            ret.matrix = (const GlObjectMatrix *)root->field_28->field_5C((s32)(root->field_28->field_58 + (char *)root));
            snw_39094_transform(&D_39094_a2b, ret.matrix);
            break;
        }
        case 1: {
            ret.matrix = (const GlObjectMatrix *)root->field_28->field_5C((s32)(root->field_28->field_58 + (char *)root));
            snw_39094_apply(&D_39094_a3, ret.matrix);
            break;
        }
        case 2: {
            ret.matrix = (const GlObjectMatrix *)root->field_28->field_5C((s32)(root->field_28->field_58 + (char *)root));
            snw_39094_transform(&D_39094_a4, ret.matrix);
            break;
        }
        case 3: {
            vt = (*(GlObjectPrefix **)&D_00000000)->field_28;
            gr = *(GlObjectPrefix **)&D_00000000;
            ret.matrix = (const GlObjectMatrix *)vt->field_5C((s32)(vt->field_58 + (char *)gr));
            snw_39094_apply(&D_39094_a5a, ret.matrix);
            ret.matrix = (const GlObjectMatrix *)root->field_28->field_5C((s32)(root->field_28->field_58 + (char *)root));
            snw_39094_transform(&D_39094_a5b, ret.matrix);
            break;
        }
        case 4: {
            vt = (*(GlObjectPrefix **)&D_00000000)->field_28;
            gr = *(GlObjectPrefix **)&D_00000000;
            ret.matrix = (const GlObjectMatrix *)vt->field_5C((s32)(vt->field_58 + (char *)gr));
            snw_39094_apply(&D_39094_a6a, ret.matrix);
            break;
        }
        case 5:
            snw_39094_enable(&D_39094_a7);
            break;
        case 8: {
            GlObjectMatrix vec;
            float scaled[3];
            float x, y, z;
            float *gv;
            vec = *(GlObjectMatrix*)((char*)&D_39094_dat + 0xE8);
            do {
                gv = (float*)((char*)&D_39094_dat + 0x128);
                x = gv[0] * *(float*)((char*)root + 0xA0);
                y = gv[1] * *(float*)((char*)root + 0xA4);
                z = gv[2] * *(float*)((char*)root + 0xA8);
                scaled[0] = x;
                scaled[1] = y;
                scaled[2] = z;
                snw_39094_translate(&vec, scaled);
            } while (0);
            snw_39094_apply(&D_39094_a8b, &vec);
            break;
        }
    }
    iter2[0] = iter2[1];
    node = iter2[1] != ((void*)0)
        ? (iter2[1] = iter2[0]->field_04, (GlObjectPrefix *)iter2[0]->field_00)
        : ((void*)0);
    while (node != ((void*)0)) {
        GlObjectPrefix *n;
        n = (GlObjectPrefix *)iter2[0]->field_00;
        if (((volatile GlObjectPrefix *)n)->field_18 & 4) {
            if (((volatile GlObjectPrefix *)n)->field_18 & 4) {
                if (root->field_2C & 2) {
                    *(GlObjectPrefix **)&D_00000000 = root;
                }
                n = (GlObjectPrefix *)iter2[0]->field_00;
                if (n->field_04 != 0)
                    n->field_14 = root;
                ret.word = snw_39094_mask();
                n = (GlObjectPrefix *)iter2[0]->field_00;
                if (n->field_20 & ret.word) {
                    n->field_28->field_1C(n->field_28->field_18 + (char *)n);
                }
            }
        }
        iter2[0] = iter2[1];
        node = iter2[1] != ((void*)0)
            ? (iter2[1] = iter2[0]->field_04, (GlObjectPrefix *)iter2[0]->field_00)
            : ((void*)0);
    }
    snw_39094_release();
}
