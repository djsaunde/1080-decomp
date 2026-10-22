/* IDO 7.1 NCC (C++) -O0 donor for arcproc_uso_func_0000012C (0x70 / 28 words).
 * edg -O0 has no closing-brace b-epilogue (cfe +2 words); the decrement is a
 * plain assignment (`*p -= 1` makes edg copy p into s1 first).
 * Spliced into arcproc_uso_o0_12C.c.o via REPLACE_FUNC_BODY. */
extern "C" {
int gl_func_00000000(...);
int arcproc_uso_func_0000012C(int *a0, int a1) {
    register int *p;
    gl_func_00000000(a0, a1);
    p = a0;
    *p = *p - 1;
    if (*a0 == 0) return 1;
    return 0;
}
}
