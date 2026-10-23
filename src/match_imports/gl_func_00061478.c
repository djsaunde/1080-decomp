/* Original compiler context for gl_func_00061478 (same IDO 7.1 -O2 recipe).
 * bootup Text uses the documented game_libs +0x1466C shim. The contiguous
 * 610F4..61478 range starts at Text 0x75760, a 32-byte boundary. Its five
 * genuine preceding assembly functions occupy 0x384 bytes, so the C entry
 * has the original residue 4 mod 32. IDO itself emits .align 5 before the
 * unreachable epilogue: preserving that context reproduces its two NOPs.
 * These preceding functions are context only; they are NOT new C matches
 * and this auxiliary object is never linked or counted as another unit.
 * Only the unmodified compiler output for gl_func_00061478 is spliced.
 * The host retains this same C definition; keep both definitions identical.
 */
#include "common.h"
extern void gl_ref_00034810(char *, ...);
extern int gl_ref_00034240();
extern int D_00000000;
extern int D_6170C_idx;
extern int D_6170C_arr[];
INCLUDE_ASM("src/match_imports/original_context", game_libs_func_000610F4);
INCLUDE_ASM("src/match_imports/original_context", game_libs_func_0006110C);
INCLUDE_ASM("src/match_imports/original_context", game_libs_func_000611E4);
INCLUDE_ASM("src/match_imports/original_context", gl_func_0006126C);
INCLUDE_ASM("src/match_imports/original_context", gl_func_00061458);

void gl_func_00061478(u8 *fmt, ...) {
    char *args;
    int i;
    char *start;
    int c;
    u8 *old;

    gl_ref_00034810((char *)&D_00000000 + 0x21E14);
    args = (char *)(((unsigned int)&fmt + 7) & ~3);
    start = args;
    old = fmt;
    fmt = old + 1;
    if (*old != 0) {
        do {
            c = *old;
            if (*old == '%') goto percent;
            if (*old == '\\') goto slash;
            if (*old != '\n') goto character;
            goto newline;
percent:
            old = fmt;
            fmt = old + 1;
            c = *old;
            if (*old == 'd') goto fmt_d;
            if (*old == 'f') goto fmt_f;
            if (*old == 's') goto fmt_s;
            if (*old == 'c') goto fmt_c;
            if (*old == 'x') goto fmt_x;
            if (*old != 'r') goto fmt_default;
            goto fmt_r;
fmt_d:
            gl_ref_00034810((char *)&D_00000000 + 0x21E48,
                              *(int *)((args = (char *)(((unsigned int)args + 7) & ~3)) - 4));
            goto next;
fmt_f:
            args = (char *)(((unsigned int)args + 15) & ~7);
            gl_ref_00034810((char *)&D_00000000 + 0x21E4C,
                              *(double *)(args - 8));
            goto next;
fmt_s:
            args = (char *)(((unsigned int)args + 7) & ~3);
            gl_ref_00034810((char *)&D_00000000 + 0x21E50,
                              *(char **)(args - 4));
            goto next;
fmt_c:
            args = (char *)(((unsigned int)args + 7) & ~3);
            gl_ref_00034810((char *)&D_00000000 + 0x21E54,
                              *(u8 *)(args - 1));
            goto next;
fmt_x:
            args = (char *)(((unsigned int)args + 7) & ~3);
            gl_ref_00034810((char *)&D_00000000 + 0x21E58,
                              *(unsigned int *)(args - 4));
            goto next;
fmt_r:
            gl_ref_00034810((char *)D_6170C_arr[D_6170C_idx - 1]);
            goto next;
fmt_default:
            gl_ref_00034810((char *)&D_00000000 + 0x21E5C, c);
            goto next;
slash:
            gl_ref_00034810((char *)&D_00000000 + 0x21E60, c);
            goto next;
newline:
            gl_ref_00034810((char *)&D_00000000 + 0x21E64, c);
            goto next;
character:
            gl_ref_00034810((char *)&D_00000000 + 0x21E68, c);
next:
            old = fmt;
            fmt = old + 1;
        } while (*old != 0);
    }
    gl_ref_00034810((char *)&D_00000000 + 0x21E6C);
    for (i = 0; i < D_6170C_idx; i++) {
        gl_ref_00034810((char *)&D_00000000 + 0x21E70, (char *)D_6170C_arr[i]);
    }
    gl_ref_00034810((char *)&D_00000000 + 0x21E7C);
    gl_ref_00034810((char *)&D_00000000 + 0x21E80);
    for (;;) {
        gl_ref_00034240();
    }
}
