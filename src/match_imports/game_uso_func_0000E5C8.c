/* Native original-compiler source port from src/game_uso/game_uso.c. Target bytes unchanged. */
#include "context_game_uso.h"
void game_uso_func_0000E5C8(char *a0, int a1) {
    GameState *a3 = (GameState *)a0;
    unsigned int src;
    unsigned int v0;
    if (((GameState *)a0)->field_f4 != 0) {
        src = ((GameState *)a0)->field_b4->field_800->field_18;
    } else {
        src = ((GameState *)a0)->field_b4->field_800->field_10;
    }
    if (src & 0x200) a3->field_e8 |= 0x2;
    if (src & 0x1000) a3->field_e8 |= 0x4;
    if (src & 0x400) a3->field_e8 |= 0x1;
    if (src & 0x10000) a3->field_e8 |= 0x10;
    if (src & 0x4000) a3->field_e8 |= 0x20;
    if (src & 0x2000) a3->field_e8 |= 0x40;
    if (src & 0x8000) a3->field_e8 |= 0x80;
    v0 = a3->field_b4->field_a58;
    {
        int cond;
        if (1) { cond = (v0 & 0x20) != 0; }
        if (cond != 0) {
            cond = (v0 & 0x40) != 0;
        }
        game_uso_e5c8_eaf4(a3, cond, v0 & 0x20);
    }
    v0 = a3->field_12c;
    if (v0 != 0) {
        if (v0 == 5) goto state5;
        if (v0 == 6) goto state6;
        if (v0 == 2) goto state2;
        if (v0 == 1) goto state1;
        if (v0 == 4) goto state4;
        if (v0 != 3) goto state_done;
        goto state3;
    state5:
        if (1) {
            a3->field_e8 |= 0x20000;
        }
        goto state_done;
    state6:
        if (1) {
            a3->field_e8 |= 0x10000;
        }
        goto state_done;
    state2:
        if (1) {
            a3->field_e8 |= 0x4000;
        }
        goto state_done;
    state1:
        if (1) {
            a3->field_e8 |= 0x1000;
        }
        goto state_done;
    state4:
        if (1) {
            a3->field_e8 |= 0x8000;
        }
        goto state_done;
    state3:
        if (1) {
            a3->field_e8 |= 0x2000;
        }
    state_done:;
    }
    {
        v0 = a3->field_e8;
        if (v0 & 0x4) {
            GameStateNode *f4 = a3->field_f4;
            a3->field_e8 = v0 & 0x3F007;
            if (f4 != 0) {
                unsigned int m = f4->field_2c;
                if (m & 0x5000) {
                    a3->field_e8 &= 0xFFFF5FFF;
                    m = (*(GameStateNode * volatile *)&a3->field_f4)->field_2c;
                }
                if (m & 0xA000) {
                    a3->field_e8 &= 0xFFFFAFFF;
                    m = (*(GameStateNode * volatile *)&a3->field_f4)->field_2c;
                }
                if (m & 0xC000) {
                    if (m & 0x100000) {
                        a3->field_e8 |= 0x200000;
                    } else if (!(m & 0x200000)) {
                        a3->field_e8 |= 0x100000;
                    }
                }
            }
            v0 = a3->field_e8;
        }
    }
    if (v0 != 0) {
        a3->field_ec += 1;
    } else {
        a3->field_ec = 0;
    }
    {
        unsigned int t0;
        if (1) { t0 = a3->field_ec; }
        if (a3->field_ec > a3->field_214) {
            unsigned int count = game_uso_e5c8_count;
            GameStateActor *obj;
            unsigned int v1;
            GameStateNode *e;
            v1 = 0;
            if (count != 0) {
                obj = a3->field_b4;
                if (1) { e = *(GameStateNode **)&game_uso_D_807FFBC0; }
                do {
                    if (t0) {}
                    if ((obj->field_8c0 & e->field_34) == 0) goto next_entry;
                    if (a3->field_e8 != e->field_2c) goto next_entry;
                    if ((e->field_38 & 0x4) && !(obj->field_9a8 & 0x1)) goto next_entry;
                    if (obj->field_938 != 0) goto next_entry;
                    a3->field_f0 = *(GameStateNode **)&game_uso_D_807FFBC0 + v1;
                    a3->field_e8 = 0;
                    a3->field_12c = 0;
                    a3->field_ec = 0;
                    t0 = 0;
                    break;
                next_entry:
                    e++;
                } while (++v1 < count);
            }
            a3->field_e8 &= 0x6;
            if ((unsigned int)t0 >= 21) {
                a3->field_ec = 0;
                a3->field_e8 = 0;
            }
        }
    }
    (void)a1;
}
