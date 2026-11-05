/* Native original-compiler source port from src/game_libs/game_libs_post.c (the gl_func_00030AF4 NON_MATCHING body,
 * comments dropped; see that file for the per-line lever history). agent-g run 288: the in-tree body compiles to the
 * target's 528 words once linked with the callees / arena addresses at their own values and the TU's .rodata (switch-1
 * table, the 0.1f literal, switch-2 / switch-3 tables) at the USO RoData 0x1874 (scripts/match-origins-gl_func_00030AF4.ld).
 * Void prototypes gl_func_emit_30AF4v / gl_func_00030AF4_{44F18,42364,460D0}v: calls whose result is never read. */
typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;
typedef float f32;
extern int gl_func_00041D0C();
extern int gl_func_00042364();
extern int gl_func_000460D0();
extern int gl_func_00041ED4();
extern int gl_func_00041F0C();
extern int gl_func_00041F44();
extern int gl_func_00044F18();
extern int gl_func_emit();
typedef struct {
    s32 f0, f4, f8, fC, f10, f14, f18;
} GlState30AF4;
extern GlState30AF4 gl_state_30AF4;
extern u32 gl_cmd_30AF4;
extern s32 D_30AF4_b, D_30AF4_c, D_30AF4_d, D_30AF4_e;
extern s32 D_30AF4_f, D_30AF4_g, D_30AF4_h, D_30AF4_i, D_30AF4_j, D_30AF4_k;
extern u32 D_30AF4_l;
extern s32 D_30AF4_m, D_30AF4_n, D_30AF4_o, D_30AF4_p, D_30AF4_q;
extern u32 D_30AF4_r;
extern s32 D_30AF4_s, D_30AF4_t, D_30AF4_v, D_30AF4_w, D_30AF4_x;
extern char gl_data_30AF4_45010, gl_data_30AF4_45F78;
#define CMD_B3 (((u8 *)&gl_cmd_30AF4)[3])
extern void gl_func_emit_30AF4v();
extern void gl_func_00030AF4_44F18v();
extern void gl_func_00030AF4_42364v();
extern void gl_func_00030AF4_460D0v();
void gl_func_00030AF4(mode, sub, arg2)
s32 mode;
u32 sub;
s32 arg2;
{
    s32 flag;
    s32 wv;
    s32 prev;
    s32 w;
    s32 sv;
    flag = 0;
    if (gl_state_30AF4.f0 == 1) {
        gl_func_00041D0C();
    }
    gl_func_emit_30AF4v(0xFB000000, &gl_data_30AF4_45010);
    prev = gl_state_30AF4.f0;
    gl_state_30AF4.f0 = mode;
    gl_func_emit(0);
    gl_func_emit_30AF4v(1);
    D_30AF4_b = 0;
    D_30AF4_c = 0;
    D_30AF4_d = 0;
    D_30AF4_e = 0;
    gl_state_30AF4.f4 = 0;
    gl_state_30AF4.f8 = 0;
    gl_state_30AF4.f14 = 0;
    gl_state_30AF4.f0 = 0;
    gl_state_30AF4.f18 = 0;
    gl_func_emit();
    gl_func_00030AF4_460D0v();
    sv = (s32)&gl_state_30AF4 + 0 * (int)(flag);
    if (sv) {}
    switch (mode) {
    case 0:
        gl_func_00042364(1);
        flag = 1;
        break;
    case 1:
        D_30AF4_f = 0;
        gl_func_00042364(0);
        gl_func_emit(2);
        flag = 1;
        break;
    case 2:
        gl_func_00042364(2);
        gl_cmd_30AF4 = 16;
        gl_func_emit(CMD_B3);
        flag = 1;
        break;
    case 3:
        if (prev != 0) {
            gl_func_00042364(5);
            flag = 1;
        }
        gl_func_emit(3);
        break;
    case 4:
        if (prev == 0) {
            D_30AF4_g = 12;
            break;
        }
        if (prev == 3) {
            for (;;) {
                if (gl_func_emit(2, 14, 0) < 100) {
                    break;
                }
                gl_func_emit();
            }
        }
        D_30AF4_h = sub + 0 * (int)(flag);
        sv = D_30AF4_h;
        gl_cmd_30AF4 = D_30AF4_h + 4; if (1) {}
        if (D_30AF4_i != 0 && D_30AF4_h == 6) {
            gl_cmd_30AF4 = 7 + 0 * (int)(D_30AF4_i + D_30AF4_i);
        }
        if (D_30AF4_i == 4) {
            gl_func_00042364(4);
            gl_cmd_30AF4 = D_30AF4_j + 18; if (1) {}
        } else if (sv + 0 * (int)(sv + sv + sv + sv + sv + sv) == 0) {
            gl_func_00042364(6);
        } else {
            gl_func_00042364(3);
        }
        flag = 1;
        D_30AF4_k = 14;
        break;
    case 5:
        if (gl_cmd_30AF4 == 13) {
            break;
        }
        if (gl_cmd_30AF4 == 15) {
            break;
        }
        if (gl_cmd_30AF4 == 14) {
            break;
        }
        if (prev == 8) {
            break;
        }
        if (D_30AF4_l == 14) {
            gl_cmd_30AF4 = 14;
        } else if (D_30AF4_l == 16) {
            gl_cmd_30AF4 = 16;
        } else {
            gl_cmd_30AF4 = 13;
        }
        gl_func_emit(CMD_B3);
        break;
    case 6:
        {
        w = 35;
        if (sv) {}
        if (D_30AF4_m == 4) {
            gl_cmd_30AF4 = 13;
            wv = 523 + 0 * (int)(sub);
        } else {
            switch (sub) {
            case 0:
                wv = 21;
                if (D_30AF4_m != 0) {
                    *(s32 *)&gl_cmd_30AF4 = 14;
                }
                break;
            case 1:
                gl_cmd_30AF4 = 13;
                wv = 20;
                break;
            case 2:
                gl_cmd_30AF4 = 13;
                wv = 22;
                w = 60;
                break;
            case 3:
            default:
                *(s32 *)&gl_cmd_30AF4 = 14;
                wv = 256;
                break;
            }
        }
        D_30AF4_n = gl_cmd_30AF4 + 0 * (int)(sv);
        if (sv) {}
        if (D_30AF4_m == 0 && sub == 0) {
            ((GlState30AF4 *)sv)->fC = 256;
            gl_func_emit(0x83010000, 2000);
            D_30AF4_o = 14;
        } else if (D_30AF4_p == 1) {
            gl_func_emit(0x46010000, 0);
            gl_func_emit(CMD_B3, w + 10);
        } else {
            gl_func_emit(0x83010000, 80);
            gl_func_emit(CMD_B3, w);
        }
        gl_func_emit(wv);
        }
        break;
    case 7:
        D_30AF4_q = 14;
        break;
    case 8:
        if (gl_cmd_30AF4 != 15) {
            gl_cmd_30AF4 = 16;
            D_30AF4_r = 16;
            gl_func_emit(CMD_B3);
        }
        break;
    case 9:
        gl_func_00030AF4_42364v(3);
        sv = (u32)&gl_state_30AF4;
        if (sv) {}
        *(s32 *)sv = sub;
        if (*(u32 *)sv == 1) {
            gl_cmd_30AF4 = 5;
        } else {
            gl_cmd_30AF4 = 17;
        }
        gl_func_emit(CMD_B3, 35);
        flag = 1;
        break;
    case 10:
        gl_func_00042364(7);
        gl_cmd_30AF4 = 18;
        gl_func_emit(0x81000F00, 0);
        sv = sub;
        D_30AF4_s = sv;
        if (sv == 2) {
            gl_func_emit(0x81001300, 0);
        }
        gl_func_emit(CMD_B3);
        flag = 1;
        break;
    case 11:
        gl_cmd_30AF4 = 19;
        gl_func_emit(CMD_B3, 80);
        break;
    case 12:
    case 13:
        break;
    }
    if (flag == 1) {
        gl_func_00041ED4();
        gl_func_emit();
        gl_func_00041F0C();
        gl_func_00041F44();
        if (mode != 4) {
            gl_func_emit(0, 0);
        }
    }
    sv = (u32)&gl_state_30AF4;
    if (D_30AF4_t != 5) {
        gl_func_00030AF4_44F18v(0);
    } else {
        *(f32 *)&((GlState30AF4 *)sv)->f14 = 0.1f;
        gl_func_emit(0, 100, ((GlState30AF4 *)sv)->f4);
        D_30AF4_v = 4;
    }
    switch (mode) {
    case 0:
        if (prev == 0) {
            break;
        }
        if (D_30AF4_w == 1) {
            gl_func_00030AF4_44F18v(1);
        } else if (D_30AF4_w == 0) {
            gl_func_00030AF4_44F18v(2);
        }
        if (D_30AF4_x == 1 || D_30AF4_x == 3) {
            switch (D_30AF4_w) {
            case 0:
                gl_func_emit_30AF4v(0x1F);
                break;
            case 1:
                gl_func_emit_30AF4v(0x1B);
                break;
            case 2:
                gl_func_emit_30AF4v(0x1D);
                break;
            case 3:
                gl_func_emit_30AF4v(0x19);
                break;
            case 4:
                gl_func_emit_30AF4v(0x1E);
                break;
            case 5:
                gl_func_emit_30AF4v(0x1C);
                break;
            case 6:
                gl_func_emit_30AF4v(0x1A);
                break;
            case 7:
                gl_func_emit_30AF4v(0x18);
                break;
            }
        }
        if (D_30AF4_w == 8) {
            gl_func_emit(0xBC);
        }
        break;
    case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8:
        break;
    case 9:
        gl_func_emit(0xF8000000, 0);
        gl_func_emit(0xA3);
        break;
    case 10:
        gl_func_emit(0xF8000000, 0);
        gl_func_emit(0, 0x3E99999A, 0);
        break;
    case 11:
        switch (sub) {
        case 0:
            gl_func_emit(14);
            break;
        case 1:
            gl_func_emit(15);
            break;
        }
        break;
    }
    gl_func_emit(0xFB000000, 0);
    gl_func_emit(0);
    gl_func_emit(0xFB000000, &gl_data_30AF4_45F78);
}
