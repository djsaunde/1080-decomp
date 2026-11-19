#ifndef PORT_LIBC_H
#define PORT_LIBC_H

/* Host C library access for the port's own code (port/main.c, port/host_*.c,
 * port/ultra/). Game code never includes this.
 *
 * Normally this just includes the system headers. PORT_LIBC_DECLS_ONLY is for
 * compile-checking on a machine without 32-bit libc headers (the analysis
 * machine has gcc -m32 but no i386 glibc): it declares the handful of libc
 * functions the port uses by hand instead. gcc's own freestanding headers
 * (stddef.h, stdarg.h) are always available. */

#include <stddef.h>
#include <stdarg.h>

#ifndef PORT_LIBC_DECLS_ONLY
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#ifdef PORT_WIN32
/* mingw's stdlib.h defines errno as a macro, which would rename the libultra OSContPad / OSContStatus field
 * `errno` (glibc's does only in errno.h). The port reads the C library's errno as PORT_ERRNO (plan 29). */
#include <errno.h>
#undef errno
#define PORT_ERRNO (*_errno())
#endif
#else
typedef struct _IO_FILE FILE;
extern FILE *stdout;
extern FILE *stderr;
FILE *fopen(const char *path, const char *mode);
int fclose(FILE *f);
size_t fread(void *p, size_t sz, size_t n, FILE *f);
size_t fwrite(const void *p, size_t sz, size_t n, FILE *f);
int fseek(FILE *f, long off, int whence);
long ftell(FILE *f);
int fflush(FILE *f);
int fputc(int c, FILE *f);
char *fgets(char *s, int n, FILE *f);
int fprintf(FILE *f, const char *fmt, ...);
int vfprintf(FILE *f, const char *fmt, va_list ap);
int printf(const char *fmt, ...);
int snprintf(char *s, size_t n, const char *fmt, ...);
int vsnprintf(char *s, size_t n, const char *fmt, va_list ap);
int vsprintf(char *s, const char *fmt, va_list ap);
int sscanf(const char *s, const char *fmt, ...);
void *malloc(size_t n);
void *calloc(size_t n, size_t sz);
void free(void *p);
void exit(int code);
void abort(void);
long strtol(const char *s, char **end, int base);
unsigned long strtoul(const char *s, char **end, int base);
double strtod(const char *s, char **end);
char *getenv(const char *name);
void *memcpy(void *d, const void *s, size_t n);
void *memmove(void *d, const void *s, size_t n);
void *memset(void *d, int c, size_t n);
int memcmp(const void *a, const void *b, size_t n);
size_t strlen(const char *s);
int strcmp(const char *a, const char *b);
int strncmp(const char *a, const char *b, size_t n);
char *strncpy(char *d, const char *s, size_t n);
float sqrtf(float x);
typedef long time_t;
struct timespec { time_t tv_sec; long tv_nsec; };
#define CLOCK_MONOTONIC 1
int clock_gettime(int clk, struct timespec *ts);
int nanosleep(const struct timespec *req, struct timespec *rem);
#define SEEK_SET 0
#define SEEK_END 2
#endif

#endif /* PORT_LIBC_H */
