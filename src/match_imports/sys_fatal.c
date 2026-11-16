/* Original compiler context for sys_fatal (same IDO 7.1 -O2 recipe).
 * bootup Text uses the documented game_libs +0x1466C shim. The contiguous
 * 610F4..61478 range starts at Text 0x75760, a 32-byte boundary. Its
 * preceding functions (five asm entries, four C functions -- copied verbatim
 * from their exact host definitions in src/game_libs/game_libs_post0b.c)
 * occupy 0x384 bytes, so the C entry has the original residue 4 mod 32.
 * IDO itself emits .align 5 before the unreachable epilogue: preserving that
 * context reproduces its two NOPs. The preceding functions are context only;
 * they are not counted again and this auxiliary object is never linked or
 * counted as another unit. Keep them identical to their host definitions.
 * Only the unmodified compiler output for sys_fatal is spliced.
 * The host retains this same C definition; keep both definitions identical.
 */
#include "common.h"
extern void gl_ref_00034810(char *, ...);
extern int gl_ref_00034240();
extern int D_00000000;
extern int gSysImDepth_6170C_idx;
extern int gSysImStack_6170C_arr[];
/* console_itoa: one C function spans the original 0x610F4 and
 * 0x6110C asm entries (0x18 + 0xD8 = 0xF0; splat split the a1 != 0 path). */
char *console_itoa(char *a0, int a1, int a2) {
    char *q;
    int v0;
    if (a1 == 0) {
        q = a0; a0++; *q = '0';
        *a0 = 0;
        return a0;
    }
    if (a1 < 0) {
        a1 = -a1;
        q = a0;
        a0++; if (1) { *q = '-'; }
    }
    while (a2 > 0) {
        if (a1 >= a2) {
            *(int *)&D_00000000 = 1;
            v0 = 0;
            do {
                a1 -= a2;
                if (a1 < 0) {
                    a1 += a2;
                    break;
                }
                v0++;
            } while (v0 != 9);
            q = a0; a0++; if (1) { *q = v0 + '0'; }
        } else if (*(int *)&D_00000000 != 0) {
            q = a0; a0++; if (1) { *q = '0'; }
        }
        a2 /= 10;
    }
    *a0 = 0;
    return a0;
}

int console_pow10Scale(int a0, int a1) {
    int i;
    for (i = 1; i < a1; i++) {
        a0 *= 10;
    }
    return a0;
}

typedef union FI_0006126C {
    f32 f;
    int i;
    f64 d;
} FI_0006126C;
extern int gl_func_00034458_6126C(); /* blank alias for the in-unit callee dbg_printThreads (ROM words are jal 0) */
char *console_formatValue(char *arg0, FI_0006126C arg2, u8 *fmt) {
    char *out = arg0;
    int B = 6;
    int A = 6;
    u8 *q;
    int c;
    int ch;

    while ((q = fmt, fmt = q + 1, *q) != 0) {
        c = *q + 0 * *q;
            if (*q == 'd') {
                D_00000000 = 0;
                out = gl_func_00034458_6126C(out, arg2.i, 0x3B9ACA00);
            } else if (c == 'x') {
                D_00000000 = 1;
                out = gl_func_00034458_6126C(out, arg2.i);
            } else if (c == 'f') {
                int ip = (int) arg2.f;
                int frac;
                char *dot;

                frac = (f32) gl_func_00034458_6126C(0xA, B) * (arg2.f - (f32) ip);
                D_00000000 = 0;
                dot = gl_func_00034458_6126C(out, ip, gl_func_00034458_6126C(0xA, A));
                *dot = '.';
                out = dot + 1;
                D_00000000 = 1;
                out = gl_func_00034458_6126C(out, frac, gl_func_00034458_6126C(0xA, B - 1)); /* B - 1 de-named (lever-sweep 93.30 -> 93.46) */
            } else if ((ch = c, c >= '0') && (c < ':')) {
                ch -= '0';
                A = (u8)ch;
                if ((q = fmt, fmt = q + 1, *q) != 0) {
                    B = (q = fmt, fmt = q + 1, *q) - '0';
                }
            } else {
                gl_func_00034458_6126C(out, (char *)&D_00000000 + 0x21E08);
                out += 0xA;
            }
    }
    return out;
}

extern int gl_func_00000000();
int math_singularFatal_00061458() {
    return gl_func_00000000();
}

void sys_fatal(u8 *fmt, ...) {
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
                              /* RAW: data-table: va_arg slot reads from the varargs area (args - 4 / - 8), not a struct */
                              *(int *)((args = (char *)(((unsigned int)args + 7) & ~3)) - 4));
            goto next;
fmt_f:
            args = (char *)(((unsigned int)args + 15) & ~7);
            gl_ref_00034810((char *)&D_00000000 + 0x21E4C,
                              *(double *)(args - 8)); /* RAW: data-table (as above) */
            goto next;
fmt_s:
            args = (char *)(((unsigned int)args + 7) & ~3);
            gl_ref_00034810((char *)&D_00000000 + 0x21E50,
                              *(char **)(args - 4)); /* RAW: data-table (as above) */
            goto next;
fmt_c:
            args = (char *)(((unsigned int)args + 7) & ~3);
            gl_ref_00034810((char *)&D_00000000 + 0x21E54,
                              *(u8 *)(args - 1)); /* RAW: data-table (as above) */
            goto next;
fmt_x:
            args = (char *)(((unsigned int)args + 7) & ~3);
            gl_ref_00034810((char *)&D_00000000 + 0x21E58,
                              *(unsigned int *)(args - 4)); /* RAW: data-table (as above) */
            goto next;
fmt_r:
            gl_ref_00034810((char *)gSysImStack_6170C_arr[gSysImDepth_6170C_idx - 1]);
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
    for (i = 0; i < gSysImDepth_6170C_idx; i++) {
        gl_ref_00034810((char *)&D_00000000 + 0x21E70, (char *)gSysImStack_6170C_arr[i]);
    }
    gl_ref_00034810((char *)&D_00000000 + 0x21E7C);
    gl_ref_00034810((char *)&D_00000000 + 0x21E80);
    for (;;) {
        gl_ref_00034240();
    }
}
