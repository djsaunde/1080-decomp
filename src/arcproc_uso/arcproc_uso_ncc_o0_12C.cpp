#include "../../include/uso_call.h"  /* NCC donors build without -I include */
/* IDO 7.1 NCC (C++) -O0 donor for arc_useRetry_0000012C (0x70 / 28 words).
 * edg -O0 has no closing-brace b-epilogue (cfe +2 words); the decrement is a
 * plain assignment (`*p -= 1` makes edg copy p into s1 first).
 * Spliced into arcproc_uso_o0_12C.c.o via REPLACE_FUNC_BODY. */
extern "C" {
int gl_func_00000000(...);
int arc_useRetry_0000012C(int *a0, int a1) {
    register int *p;
    CALL(arc_recordCourseTime_0000019C)(a0, a1);
    p = a0;
    *p = *p - 1;
    if (*a0 == 0) return 1;
    return 0;
}
}
