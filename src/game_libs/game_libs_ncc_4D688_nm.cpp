/* IDO 7.1 NCC (C++) NM-only comparison donor for gl_func_0004D688 (0x3B8 / 238
 * words): the debug-console event loop (guard-pair check, poll, dispatch on the
 * event code 'e' / 'f' / 'd' / 'g' / 'h' / 'l'). agent-c run 137, cfe NM 73.76 ->
 * 81.37 (236 words). Load-bearing, from kits LXI / LXII: the cfe goto ladder is a
 * SOURCE-ORDER switch under NCC (plain beq chain, no branch-likely); the 0x3CB00
 * guard pair is ONE static long long compared against 0x1234567812345678 (each
 * half compared through the `bne rX,imm` macro, constant rematerialised in at);
 * `if (0)` taking &sp38 keeps the polled object in a stack home (target spills
 * it around the poll call at 56(sp)).
 * Residual: the target holds &D_4D688_g2 in s7 across the loop (here re-`lui
 * at` per store, so the frame is 72 vs 80 and s8 is used); for(;;) / while(1)
 * loops, a static flag, typed / array externs: no hold.
 * Spliced into build/non_matching/ only via REPLACE_FUNC_BODY. */
typedef int s32;
#define FW(p, o) (*(int *)((char *)(p) + (o)))
extern "C" {
extern char D_00000000;
extern int gl_func_00034458(...);
extern char D_4D688_g1;
extern char D_4D688_g2;
}
#define DSTR(o) ((char *)&D_00000000 + (o))
static long long sG;

extern "C" void gl_func_0004D688(char *arg0) {
    char *sp38;
    s32 temp_a1;
    s32 temp_t9;
    s32 var_s3;
    s32 var_s4;
    char *temp_a0;
    char *temp_a0_2;
    char *temp_a0_3;
    char *var_s2;
    char *var_s6;
    s32 *g2 = (s32 *)&D_4D688_g2;
#define g1 (&D_4D688_g1)

    FW(arg0, 0x48) = 0;
    FW(arg0, 0x18) = 0;
    FW(arg0, 0x40) = (s32) DSTR(0x2021C);
    FW(arg0, 0x10) = 0;
    var_s2 = 0;
    var_s6 = 0;
    var_s3 = 0;
    var_s4 = 0;
    gl_func_00034458(g1);
loop_1:
    if (sG != 0x1234567812345678LL) {
        gl_func_00034458(DSTR(0x20224));
    }
    sp38 = *(char **)(g1 + 0x1C);
    *g2 = 0;
    gl_func_00034458(sp38, sp38 + 0x1C, 1);
    temp_a1 = FW(sp38, 0x1C);
    if (0) gl_func_00034458(&sp38);
    *g2 = 1;
    switch (temp_a1) {
    case 0x65: {
        FW(arg0, 0x20) = 0;
        if (FW(arg0, 0x18) != 0) {
            gl_func_00034458(DSTR(0x20240), temp_a1);
            temp_a0_2 = (char *)FW(arg0, 0xC);
            if ((temp_a0_2 != 0) && (FW(temp_a0_2, 0x5C) != 0)) {
                FW(arg0, 0x3C) = (s32) DSTR(0x20248);
                gl_func_00034458(temp_a0_2);
                FW(arg0, 0x20) = 1;
                FW(arg0, 0x40) = (s32) DSTR(0x20250);
            } else {
                gl_func_00034458(arg0);
                gl_func_00034458(arg0);
            }
            FW(arg0, 0x18) = 0;
        } else if (var_s4 != 0) {
            var_s4 = 0;
            FW(FW(arg0, 0xC), 0x5C) = gl_func_00034458(FW(arg0, 0xC) + 0x10, temp_a1);
            FW(arg0, 0x18) = (s32) var_s6;
            gl_func_00034458();
            FW(arg0, 0x3C) = (s32) DSTR(0x2025C);
            gl_func_00034458(DSTR(0x20260));
            gl_func_00034458(FW(arg0, 0x18));
            gl_func_00034458(FW(arg0, 0x18));
            FW(arg0, 0x40) = (s32) DSTR(0x20268);
            FW(arg0, 0x20) = 1;
        } else {
            gl_func_00034458(arg0, temp_a1);
            gl_func_00034458(arg0);
        }
        goto loop_1;
    }
    case 0x66: {
        FW(arg0, 0x1C) = 0;
        gl_func_00034458(DSTR(0x2026C), temp_a1);
        goto loop_1;
    }
    case 0x64: {
        var_s3 += 1;
        if (var_s3 & 1) {
            gl_func_00034458(g1, temp_a1);
            gl_func_00034458(g1);
        }
        var_s6 = var_s2;
        if (var_s2 != 0) {
            if (FW(arg0, 0xC) != 0) {
                var_s4 = 1;
                gl_func_00034458();
            } else {
                FW(arg0, 0x18) = (s32) var_s2;
                gl_func_00034458();
                FW(arg0, 0x3C) = (s32) DSTR(0x20270);
                gl_func_00034458(DSTR(0x20274));
                gl_func_00034458(FW(arg0, 0x18));
                gl_func_00034458(FW(arg0, 0x18));
                FW(arg0, 0x40) = (s32) DSTR(0x2027C);
                FW(arg0, 0x20) = 1;
            }
        }
        FW(arg0, 0x3C) = (s32) DSTR(0x20280);
        gl_func_00034458(DSTR(0x20288));
        var_s2 = (char *)gl_func_00034458();
        gl_func_00034458(DSTR(0x20290));
        temp_a0_3 = (char *)FW(arg0, 0x44);
        FW(arg0, 0x3C) = (s32) DSTR(0x20298);
        FW(arg0, 0x24) = (s32) (FW(arg0, 0x24) + 1);
        if ((temp_a0_3 != 0) && (FW(arg0, 0x24) & 1)) {
            gl_func_00034458(temp_a0_3, 0x64, 1);
        }
        goto loop_1;
    }
    case 0x67: case 0x68: {
        if (FW(arg0, 0xC) != 0) {
            gl_func_00034458(DSTR(0x202A0), temp_a1);
        }
        if (FW(arg0, 0x4) != 0) {
            gl_func_00034458(DSTR(0x202B4));
        }
        temp_t9 = FW(arg0, 0x8);
        FW(arg0, 0x8) = 0;
        FW(arg0, 0x28) = 0;
        FW(arg0, 0x4) = temp_t9;
        if (FW(arg0, 0x18) == 0) {
            gl_func_00034458(arg0);
        }
        gl_func_00034458(DSTR(0x202D0));
        goto loop_1;
    }
    case 0x6C: {
        gl_func_00034458(g1, temp_a1);
        gl_func_00034458();
        FW(arg0, 0x48) = 1;
        goto loop_1;
    }
    default:
    gl_func_00034458(DSTR(0x202D4), temp_a1);
    goto loop_1;
    }
}
