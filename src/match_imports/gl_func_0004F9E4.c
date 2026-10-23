/* Native original-compiler source port from src/game_libs/game_libs_post0b.c. Target bytes unchanged. */
#include "context_game_libs_post0b.h"
void gl_func_0004F9E4(Tbls_5330C *arg0) {
    s16 cur[8];
    GLRec4F9E4 *r;
    GLRec4F9E4 *base;
    u32 cnt;
    u32 j;
    u32 i;
    D_4F9E4_head = 0;
    if ((u32)arg0->field_40 >= 0x12CU) {
        gl_4F9E4_report((char *)&D_00000000 + 0x20E0C);
    }
    if (arg0->field_40 != 0) {
        i = 0;
        do {
            r = arg0->field_60 + i;
            cnt = D_4F9E4_head;
            j = 0; cur[0] = r->f0; cur[1] = r->f2; cur[2] = r->f4; if (cnt != 0) { base = arg0->field_60; do {
                    if ((cur[0] == base[D_4F9E4_itr[j]].f0) && (cur[1] == base[D_4F9E4_itr[j]].f2) &&
                        (cur[2] == base[D_4F9E4_itr[j]].f4)) {
                        break;
                    }
                    j++;
                } while (j < cnt);
            }
            if (j < cnt) {
                D_4F9E4_head = cnt + 1; D_4F9E4_tab[cnt] = D_4F9E4_tab[j];
            } else {
                D_4F9E4_head = cnt + 1; D_4F9E4_tab[cnt] = i;
            }
            i++;
        } while (i < (u32)arg0->field_40);
    }
}
