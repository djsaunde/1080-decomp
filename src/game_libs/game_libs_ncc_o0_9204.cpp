/* IDO 7.1 NCC (C++) -O0 donors for gl_func_00009204 (60 words) and
 * gl_func_000092F4 (58 words), siblings of gl_func_00008FFC: re-decoded on the
 * 8FFC shape (plain obj home + register base/o/tmp aliases, D_8FFC_base for
 * the &D+8 read) and compiled with NCC -O0 (no cfe closing-brace b-epilogue).
 * The old "s-reg promotion cap" note was the missing obj home + aliases.
 * Spliced into game_libs_mid.c.o via REPLACE_FUNC_BODY. */
extern "C" {
int gl_func_00000000(...);
extern char D_00000000;
extern int D_0000004C, D_00000058, D_00000064;
extern char D_8FFC_base;

int gl_func_00009204(int a0, int a1, int a2) {
    int obj;
    register char *base;
    register int *o;
    register char *tmp;

    gl_func_00000000(&D_00000000, *(int *)((char *)&D_8FFC_base + 8));
    gl_func_00000000(D_0000004C, D_00000058);
    obj = gl_func_00000000(&D_00000000, D_00000064, 6, a2);
    base = (char *)&D_00000000;
    o = (int *)obj;
    tmp = base + 0x10;
    gl_func_00000000(tmp, o);
    if (o[0x14 / 4] != 0) {
        o[0x4 / 4] = 1;
    }
    tmp = base;
    o[0x14 / 4] = (int)tmp;
    gl_func_00000000(&D_00000000, 0);
    (void)a0;
    (void)a1;
    return obj;
}

int gl_func_000092F4(int a0, int a1) {
    int obj;
    register char *base;
    register int *o;
    register char *tmp;

    gl_func_00000000(&D_00000000, *(int *)((char *)&D_8FFC_base + 8));
    gl_func_00000000(D_0000004C, D_0000004C);
    obj = gl_func_00000000(&D_00000000, 8, 2, a1);
    base = (char *)&D_00000000;
    o = (int *)obj;
    tmp = base + 0x10;
    gl_func_00000000(tmp, o);
    if (o[0x14 / 4] != 0) {
        o[0x4 / 4] = 1;
    }
    tmp = base;
    o[0x14 / 4] = (int)tmp;
    gl_func_00000000(&D_00000000, 0);
    (void)a0;
    return obj;
}
}
