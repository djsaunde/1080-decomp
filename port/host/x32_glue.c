/* port/host/x32_glue.c -- the libc boundary of the 64-bit port (PORT_64, docs/1080_PC_PORT_PLAN.md 21.4).
 *
 * The game and the port layer are compiled -mx32 (ILP32 data in x86-64 code) and call the system's
 * LP64 glibc directly.  Most calls are compatible as they stand: integer and pointer arguments arrive
 * zero-extended in 64-bit registers, every pointer the x32 code holds is below 2 GB (host/main64.c),
 * and the x32 kernel-facing structs (timespec, off_t) were laid out to match LP64.  The functions
 * here are the exceptions, the calls whose memory layout differs; the link wraps each one
 * (-Wl,--wrap=NAME, port/CMakeLists.txt PORT64_WRAP), so __wrap_NAME below receives the x32 code's
 * call and __real_NAME is glibc's:
 *   va_list           x32's is {u32 gp, u32 fp, u32 overflow, u32 save}, LP64's has 8-byte pointers
 *                     (the register save area and the 8-byte overflow slots are the same psABI);
 *   char **endptr     strtol / strtoul / strtod would store 8 bytes into a 4-byte x32 slot;
 *   long by value     fseek's offset (x32 long is 32-bit: sign-extend it), sbrk's increment;
 *   errno            __errno_location points into the TLS block (above 4 GB): a low copy;
 *   mmap              an unhinted mapping lands above 4 GB: MAP_32BIT;
 *   struct sigaction  4-byte handler / restorer;
 *   void *frames[]    backtrace / backtrace_symbols_fd arrays of 4-byte pointers.
 * This file is itself LP64 (no -mx32). */
#define _GNU_SOURCE
#include <errno.h>
#include <execinfo.h>
#include <signal.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

typedef uint32_t p32; /* an x32 pointer */

/* ---- va_list */
typedef struct { uint32_t gp, fp; p32 overflow, save; } VaX32;
typedef struct { uint32_t gp, fp; void *overflow, *save; } VaLp64;

#define VA_FROM_X32(dst, ap32)                                                                  \
    va_list dst;                                                                                \
    do {                                                                                        \
        VaLp64 *d_ = (VaLp64 *)(void *)(dst);                                                   \
        d_->gp = (ap32)->gp;                                                                    \
        d_->fp = (ap32)->fp;                                                                    \
        d_->overflow = (void *)(uintptr_t)(ap32)->overflow;                                    \
        d_->save = (void *)(uintptr_t)(ap32)->save;                                            \
    } while (0)

int __real___vfprintf_chk(FILE *f, int flag, const char *fmt, va_list ap);
int __real___vsnprintf_chk(char *s, size_t n, int flag, size_t slen, const char *fmt, va_list ap);
int __real___vsprintf_chk(char *s, int flag, size_t slen, const char *fmt, va_list ap);
int __real_vfprintf(FILE *f, const char *fmt, va_list ap);
int __real_vsnprintf(char *s, size_t n, const char *fmt, va_list ap);
int __real_vsprintf(char *s, const char *fmt, va_list ap);

int __wrap___vfprintf_chk(FILE *f, int flag, const char *fmt, VaX32 *ap32) {
    VA_FROM_X32(ap, ap32);
    return __real___vfprintf_chk(f, flag, fmt, ap);
}
int __wrap___vsnprintf_chk(char *s, uint32_t n, int flag, uint32_t slen, const char *fmt, VaX32 *ap32) {
    VA_FROM_X32(ap, ap32);
    return __real___vsnprintf_chk(s, n, flag, slen, fmt, ap);
}
int __wrap___vsprintf_chk(char *s, int flag, uint32_t slen, const char *fmt, VaX32 *ap32) {
    VA_FROM_X32(ap, ap32);
    return __real___vsprintf_chk(s, flag, slen, fmt, ap);
}
int __wrap_vfprintf(FILE *f, const char *fmt, VaX32 *ap32) {
    VA_FROM_X32(ap, ap32);
    return __real_vfprintf(f, fmt, ap);
}
int __wrap_vsnprintf(char *s, uint32_t n, const char *fmt, VaX32 *ap32) {
    VA_FROM_X32(ap, ap32);
    return __real_vsnprintf(s, n, fmt, ap);
}
int __wrap_vsprintf(char *s, const char *fmt, VaX32 *ap32) {
    VA_FROM_X32(ap, ap32);
    return __real_vsprintf(s, fmt, ap);
}

/* ---- end pointers (an x32 `char *` slot is 4 bytes) */
long __real_strtol(const char *s, char **end, int base);
unsigned long __real_strtoul(const char *s, char **end, int base);
double __real_strtod(const char *s, char **end);

int32_t __wrap_strtol(const char *s, p32 *end32, int base) {
    char *e;
    long v = __real_strtol(s, &e, base);
    if (end32 != NULL) {
        *end32 = (p32)(uintptr_t)e;
    }
    /* x32 long is 32 bits: clamp like a 32-bit strtol */
    return v > INT32_MAX ? INT32_MAX : v < INT32_MIN ? INT32_MIN : (int32_t)v;
}
uint32_t __wrap_strtoul(const char *s, p32 *end32, int base) {
    char *e;
    unsigned long v = __real_strtoul(s, &e, base);
    if (end32 != NULL) {
        *end32 = (p32)(uintptr_t)e;
    }
    /* "-1" gives ULONG_MAX: keep the 32-bit result (0xFFFFFFFF) either way */
    return v > UINT32_MAX && (long)v >= 0 ? UINT32_MAX : (uint32_t)v;
}
double __wrap_strtod(const char *s, p32 *end32) {
    char *e;
    double v = __real_strtod(s, &e);
    if (end32 != NULL) {
        *end32 = (p32)(uintptr_t)e;
    }
    return v;
}

/* ---- 32-bit long arguments */
int __real_fseek(FILE *f, long off, int whence);
void *__real_sbrk(intptr_t inc);
int __wrap_fseek(FILE *f, int32_t off, int whence) { return __real_fseek(f, off, whence); }
void *__wrap_sbrk(int32_t inc) { return __real_sbrk(inc); }

/* ---- errno: a low copy, refreshed at each call (read right after the failing call) */
int *__real___errno_location(void);
static int sErrnoLow;
int *__wrap___errno_location(void) {
    sErrnoLow = *__real___errno_location();
    return &sErrnoLow;
}

/* ---- mappings below 2 GB */
void *__real_mmap(void *addr, size_t len, int prot, int flags, int fd, off_t off);
void *__wrap_mmap(void *addr, uint32_t len, int prot, int flags, int fd, int64_t off) {
    void *p = __real_mmap(addr, len, prot, flags | (addr == NULL ? MAP_32BIT : 0), fd, off);
    if (p != MAP_FAILED && (uintptr_t)p + len > 0x80000000u) {
        munmap(p, len);
        return MAP_FAILED;
    }
    return p;
}

/* ---- struct sigaction: x32 {handler u32, sigset_t (128 bytes), int flags, restorer u32} */
typedef struct { p32 handler; unsigned char mask[128]; int flags; p32 restorer; } SigactionX32;
int __real_sigaction(int sig, const struct sigaction *sa, struct sigaction *old);
int __wrap_sigaction(int sig, const SigactionX32 *sa32, SigactionX32 *old32) {
    struct sigaction sa, old;
    int r;
    memset(&sa, 0, sizeof sa);
    if (sa32 != NULL) {
        sa.sa_handler = (void (*)(int))(uintptr_t)sa32->handler;
        memcpy(&sa.sa_mask, sa32->mask, sizeof sa32->mask < sizeof sa.sa_mask ? sizeof sa32->mask : sizeof sa.sa_mask);
        sa.sa_flags = sa32->flags & ~0x04000000; /* SA_RESTORER: glibc supplies its own */
    }
    r = __real_sigaction(sig, sa32 != NULL ? &sa : NULL, &old);
    if (r == 0 && old32 != NULL) {
        memset(old32, 0, sizeof *old32);
        old32->handler = (p32)(uintptr_t)old.sa_handler;
        old32->flags = old.sa_flags;
    }
    return r;
}

/* ---- backtraces: arrays of 4-byte pointers (frames above 4 GB, i.e. libc, come out truncated) */
int __real_backtrace(void **frames, int n);
void __real_backtrace_symbols_fd(void *const *frames, int n, int fd);
int __wrap_backtrace(p32 *frames32, int n) {
    void *f[128];
    int i, k = __real_backtrace(f, n < 128 ? n : 128);
    for (i = 0; i < k; i++) {
        frames32[i] = (p32)(uintptr_t)f[i];
    }
    return k;
}
void __wrap_backtrace_symbols_fd(const p32 *frames32, int n, int fd) {
    void *f[128];
    int i;
    for (i = 0; i < n && i < 128; i++) {
        f[i] = (void *)(uintptr_t)frames32[i];
    }
    __real_backtrace_symbols_fd(f, i, fd);
}
