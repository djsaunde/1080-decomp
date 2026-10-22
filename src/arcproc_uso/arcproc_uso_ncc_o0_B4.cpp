/* IDO 7.1 NCC (C++) -O0 donor for arcproc_uso_func_000000B4 (0x78 / 30 words).
 * arcproc is C++: edg -O0 has no closing-brace b-epilogue (cfe +2 words), and
 * edg keeps source evaluation order, so the compare is spelled with the
 * target's first-evaluated side on the left (`a0[1] + 1 == a0[2]`) and the
 * increment as a plain assignment (`+=` makes edg take &p[1] into s1).
 * Spliced into arcproc_uso_o0_50.c.o via REPLACE_FUNC_BODY. */
extern "C" {
int gl_func_00000000(...);
int arcproc_uso_func_000000B4(int *a0, int a1) {
    register int *p;
    gl_func_00000000(a0, a1);
    if (a0[1] + 1 == a0[2]) {
        return 1;
    }
    p = a0;
    p[1] = p[1] + 1;
    return 0;
}
}
