/* port/va_pack.c -- the game's MIPS-style varargs on x86-64 (plan 21.5).
 *
 * The game's varargs functions walk their arguments in memory, the IDO / MIPS way: a char * cursor
 * from the last named parameter's home, 4-byte slots, a double 8-byte aligned (sys_fatal.c VA_ARG,
 * the Plauger _Printf's va_list).  On i386 (cdecl: every argument on the stack, in order) that walk
 * reads the caller's arguments, and port/ultra/misc.c's _Printf hands the cursor to vsnprintf as an
 * i386 va_list.  On x86-64 the variadic arguments arrive in registers and a va_list is a struct, so
 * under PORT64:
 *   - each game varargs function packs its arguments, with the standard va_list, into an IDO-layout
 *     buffer (port_va_pack_printf) and hands that buffer on as the cursor, as on the N64;
 *   - _Printf turns an IDO-layout cursor into an x86-64 va_list (port_va_from_ido): every argument
 *     in an 8-byte overflow slot, the register areas marked used.
 * For a printf-family format the argument types come from the format. */
#include <stdarg.h>

#define PACK_MAX 64 /* IDO words */

/* the next conversion's argument kinds: 'i' an int-class word (also each `*`), 'd' a double,
 * 'q' an 8-byte integer (`L` / `ll`); returns the format position after it, 0 at the end */
static const char *next_conv(const char *s, char *kinds, int *nk)
{
    int isLong = 0;
    *nk = 0;
    for (;;) {
        while (*s != 0 && *s != '%') {
            s++;
        }
        if (*s == 0) {
            return 0;
        }
        s++;
        if (*s == '%') {
            s++;
            continue;
        }
        break;
    }
    while (*s == ' ' || *s == '+' || *s == '-' || *s == '#' || *s == '0') {
        s++;
    }
    for (;;) { /* width, then .precision: digits or '*' */
        if (*s == '*') {
            kinds[(*nk)++] = 'i';
            s++;
        } else {
            while (*s >= '0' && *s <= '9') {
                s++;
            }
        }
        if (*s != '.') {
            break;
        }
        s++;
    }
    while (*s == 'h' || *s == 'l' || *s == 'L') {
        isLong += *s == 'l' ? 1 : *s == 'L' ? 2 : 0;
        s++;
    }
    if (*s == 0) {
        return 0;
    }
    switch (*s) {
    case 'e': case 'E': case 'f': case 'g': case 'G':
        kinds[(*nk)++] = 'd';
        break;
    default:
        kinds[(*nk)++] = isLong >= 2 ? 'q' : 'i';
        break;
    }
    return s + 1;
}

/* fmt's arguments from ap into buf (IDO layout, PACK_MAX words); returns the words used */
int port_va_pack_printf(const char *fmt, va_list ap, unsigned int *buf)
{
    unsigned int n = 0;
    const char *s = fmt;
    char kinds[3];
    int nk, k;

    while (s != 0 && (s = next_conv(s, kinds, &nk)) != 0) {
        for (k = 0; k < nk && n < PACK_MAX - 3; k++) {
            if (kinds[k] == 'i') {
                buf[n++] = va_arg(ap, unsigned int); /* int, char, pointer (x32: 4 bytes) */
                continue;
            }
            n = (n + 1) & ~1u; /* 8 bytes, 8-aligned */
            if (kinds[k] == 'd') {
                double d = va_arg(ap, double);
                __builtin_memcpy(&buf[n], &d, 8);
            } else {
                long long v = va_arg(ap, long long);
                __builtin_memcpy(&buf[n], &v, 8);
            }
            n += 2;
        }
    }
    return (int)n;
}

#ifdef PORT64
/* The x86-64 psABI va_list (as the x32 code sees it: 4-byte pointers; host/x32_glue.c widens it). */
typedef struct { unsigned int gp, fp, overflow, save; } PortVaX64;

/* an IDO-layout cursor -> slots (8 bytes per argument, the psABI overflow area) + a va_list over
 * them; slots needs PACK_MAX entries */
void port_va_from_ido(const char *fmt, const char *cursor, unsigned long long *slots, void *vaOut)
{
    PortVaX64 *va = (PortVaX64 *)vaOut;
    unsigned int off = 0, n = 0;
    const char *s = fmt;
    char kinds[3];
    int nk, k;

    while (s != 0 && (s = next_conv(s, kinds, &nk)) != 0) {
        for (k = 0; k < nk && n < PACK_MAX; k++) {
            if (kinds[k] == 'i') {
                unsigned int w;
                __builtin_memcpy(&w, cursor + off, 4);
                slots[n++] = w;
                off += 4;
            } else {
                off = (off + 7) & ~7u;
                __builtin_memcpy(&slots[n++], cursor + off, 8);
                off += 8;
            }
        }
    }
    va->gp = 48;  /* the six GP registers used */
    va->fp = 176; /* the eight vector registers used */
    va->overflow = (unsigned int)(unsigned long)slots;
    va->save = (unsigned int)(unsigned long)slots;
}
#endif
