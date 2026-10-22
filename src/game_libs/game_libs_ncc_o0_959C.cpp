/* IDO 7.1 NCC (C++) -O0 donor for gl_func_0000959C (0xD4 + the 1-word zero
 * pad the .s carried): the cfe NM body unchanged; edg -O0 emits no closing-brace
 * b-epilogue marker. Spliced into game_libs_o0_949C.c.o via REPLACE_FUNC_BODY. */
extern "C" {
int gl_func_00000000(...);
extern int D_00000000;
int *gl_func_0000959C(int *a0) {
    int *ret;
    register int *s0;
    register int *s1;
    register int *s2;

    s1 = 0;
    if (s1 != 0 || (s1 = (int *)gl_func_00000000(0x40)) != 0) {
        gl_func_00000000(s1);
        s1[0x28 / 4] = (int)&D_00000000;
        s1[0x3C / 4] = 0;
    }
    ret = s0 = s1;
    if (a0[0x40 / 4] != 0) {
        s0 = (int *)a0[0x40 / 4];
        s1 = ret;
        s2 = (int *)((char *)s1 + 0x10);
        gl_func_00000000(s2, s0);
        if (s0[0x14 / 4] != 0) {
            s0[1] = 1;
        }
        s2 = s1;
        s0[0x14 / 4] = (int)s2;
    }
    return ret;
}
}
