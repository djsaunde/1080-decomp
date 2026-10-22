/* IDO 7.1 NCC (C++) donor for gl_func_00043654 (0x16C / 91 words): the cfe NM body
 * compiled as C++, plus a dead `if (!sp34) {}` between the two stores of the
 * else-branch display-list append (permuter find, agent-c run 120): it moves the
 * append pointer from v1 to the target's a1. Spliced into game_libs_post0b.c.o via
 * REPLACE_FUNC_BODY. */
typedef int s32; typedef short s16;
#define FW(p, o) (*(int *)((char *)(p) + (o)))
extern "C" {
extern char D_00000000;
int gl_func_00000000(...);
void gl_func_00043654(char *arg0, char *arg1) {
    char *sp34;
    char *var_a3;
    char *temp_v0;
    char *temp_a0;
    char *temp_a0_2;
    char *temp_a1;
    char *temp_a2;
    s32 idx;
    s32 idx2;

    FW(arg0, 0x64) = (int) *(char **)((char *)&D_00000000 + 0x214);
    var_a3 = (char *) FW(arg0, 0x64);
    *(s16 *)(arg0 + 0x88) = FW((char *)FW((char *)FW(arg0, 0x64), 0xC), 0x4);
    if (FW(arg0, 0x48) == 0) {
        gl_func_00000000(FW(arg0, 0x64), arg1);
        gl_func_00000000(FW(arg0, 0x64), arg1);
        gl_func_00000000(FW(arg0, 0x64), arg1);
        gl_func_00000000(FW(arg0, 0x64), arg1);
        var_a3 = (char *) FW(arg0, 0x64);
    }
    if (FW(arg0, 0x38) & 0x400000) {
        gl_func_00000000((char *)&D_00000000, var_a3, arg0);
        if (FW(arg0, 0x38) & 0x100000) {
            FW(arg0, 0x68) = gl_func_00000000(0, (char *)&D_00000000 + 0x1FD1C, 0xA, 0, 5, 0, 0);
            gl_func_00000000(FW(arg0, 0x68)); if (1) {}
            temp_v0 = (char *) FW(arg0, 0x68);
            sp34 = (char *) FW(arg0, 0x64);
            temp_v0 = (char *) gl_func_00000000(FW(temp_v0, 0x60));
            temp_a0 = (char *) FW(sp34, 0xC);
            idx = FW(temp_a0, 0x4); if (1) {}
            FW(temp_a0, 0x4) = idx + 1;
            temp_a2 = (char *) FW((char *)FW(sp34, 0xC), 0x0) + (idx * 8); if (1) {}
            FW(temp_a2, 0x0) = 0x06000000;
            FW(temp_a2, 0x4) = (int) temp_v0;
        }
        gl_func_00000000(FW(arg0, 0x64));
    } else {
        temp_a0_2 = (char *) FW(var_a3, 0xC); if (1) {}
        idx2 = FW(temp_a0_2, 0x4);
        FW(temp_a0_2, 0x4) = idx2 + 1;
        temp_a1 = (char *) FW((char *)FW(var_a3, 0xC), 0x0) + (idx2 * 8);
        FW(temp_a1, 0x0) = 0xBB040000;
        if (!sp34) {}
        FW(temp_a1, 0x4) = -1;
    }
    gl_func_00000000(FW(arg0, 0x64));
}
}
