/* port/host/main64.c -- the LP64 entry of the 64-bit port (PORT_64, docs/1080_PC_PORT_PLAN.md 21).
 *
 * Everything else in tenshoe is compiled -mx32: x86-64 code with the N64's ILP32 data model, so every
 * struct, ROM blob and cfront layout keeps its 32-bit shape.  x32 code keeps pointers in 32 bits and
 * computes its stack pointer in 32 bits (`sub $n, %esp`), so it may only ever see addresses below
 * 4 GB (below 2 GB in practice: the game's int-held pointers are signed).  This unit, the only LP64
 * code besides libc, establishes that before any x32 code runs:
 *   - the heap: glibc malloc may not use mmap (high addresses); the brk heap of a non-PIE image
 *     starts after .bss, below 2 GB;
 *   - argv and environ are copied into that heap (the kernel puts them on the high initial stack);
 *   - stdout / stderr become FILEs allocated there (x32 code reads the low half of the variables);
 *   - a stack mapped MAP_32BIT, entered with swapcontext; port_main (port/main.c's main) runs on it.
 * Every address check aborts with a message rather than letting a truncated pointer through. */
#define _GNU_SOURCE
#include <execinfo.h>
#include <malloc.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/personality.h>
#include <ucontext.h>
#include <unistd.h>

extern int port_main(int argc, char **argv);
/* the link wraps these for the x32 code (host/x32_glue.c); this LP64 unit calls glibc's own */
extern int __real_sigaction(int sig, const struct sigaction *sa, struct sigaction *old);
extern void *__real_mmap(void *addr, size_t len, int prot, int flags, int fd, off_t off);
extern int __real_backtrace(void **frames, int n);
extern void __real_backtrace_symbols_fd(void *const *frames, int n, int fd);
extern char **environ;

#define LOW_LIMIT 0x80000000ul
#define GAME_STACK_SIZE (64u << 20)

static ucontext_t sHostCtx, sGameCtx;
static int sArgc, sRet;
static char **sArgv;

static void need_low(const void *p, const char *what) {
    if ((uintptr_t)p >= LOW_LIMIT) {
        fprintf(stderr, "main64: %s at %p is above 2 GB (the x32 game code cannot address it)\n", what, p);
        abort();
    }
}

static char **copy_strv(char **v, int n) {
    char **out = malloc(((size_t)n + 1) * sizeof(char *));
    int i;
    need_low(out, "a vector copy");
    for (i = 0; i < n; i++) {
        out[i] = strdup(v[i]);
        need_low(out[i], "a string copy");
    }
    out[n] = NULL;
    return out;
}

/* a fault report (no debugger needed): the faulting address, rip and rsp; `addr2line -fe tenshoe <rip>` */
static void on_fault(int sig, siginfo_t *si, void *ucv) {
    ucontext_t *uc = (ucontext_t *)ucv;
    char buf[160];
    int n = snprintf(buf, sizeof buf, "main64: signal %d at address %p, rip 0x%llx rsp 0x%llx\n", sig, si->si_addr,
                     (unsigned long long)uc->uc_mcontext.gregs[REG_RIP],
                     (unsigned long long)uc->uc_mcontext.gregs[REG_RSP]);
    if (write(2, buf, (size_t)n) < 0) {
    }
    {
        void *bt[24];
        __real_backtrace_symbols_fd(bt, __real_backtrace(bt, 24), 2);
    }
    signal(sig, SIG_DFL);
    raise(sig);
}

static void game_entry(void) {
    /* argv as the x32 code sees it: an array of 4-byte pointers */
    uint32_t *argv32 = malloc(((size_t)sArgc + 1) * 4);
    int i;
    need_low(argv32, "argv");
    for (i = 0; i <= sArgc; i++) {
        argv32[i] = (uint32_t)(uintptr_t)sArgv[i];
    }
    sRet = port_main(sArgc, (char **)argv32);
}

int main(int argc, char **argv) {
    int n;
    void *stack;

    /* Run 869 (the 64-bit soak: ~2.5 % of runs died at step 0 reading the ROM): with ASLR the kernel puts
     * the brk heap of a non-PIE image anywhere in the 1 GB after .bss, so it can start just below
     * 0x40000000, where the MAP_32BIT game stack (below) and every MAP_32BIT mmap (x32_glue.c) land.  The
     * heap then cannot grow, glibc malloc falls back to mmap above 4 GB, and the x32 code truncates the
     * pointer.  Re-exec once without address randomisation: brk then starts right after .bss, ~1 GB of
     * room below the MAP_32BIT window (and the layout is the same on every run).  PORT64_ASLR=1 keeps
     * ASLR; the heap's room is checked below either way. */
    if (getenv("PORT64_ASLR") == NULL) {
        int pers = personality(0xffffffff);
        if (pers != -1 && !(pers & ADDR_NO_RANDOMIZE) && personality(pers | ADDR_NO_RANDOMIZE) != -1) {
            execv("/proc/self/exe", argv);
            personality(pers); /* the exec failed: carry on as we are */
        }
    }
    if ((uintptr_t)sbrk(0) > 0x40000000ul - (256ul << 20)) {
        fprintf(stderr, "main64: the brk heap starts at %p, less than 256 MB below the MAP_32BIT window "
                "(0x40000000): malloc would fall back to high mmap (unset PORT64_ASLR)\n", sbrk(0));
        return 1;
    }
    mallopt(M_MMAP_MAX, 0);          /* brk only: every malloc block stays low */
    mallopt(M_ARENA_MAX, 1);
    sArgc = argc;
    sArgv = copy_strv(argv, argc);
    /* environ: x32 code (getenv inside libc is LP64 and fine) never walks it; keep libc's copy low */
    for (n = 0; environ[n] != NULL; n++) {
    }
    environ = copy_strv(environ, n);
    stdout = fdopen(1, "w");
    stderr = fdopen(2, "w");
    need_low(stdout, "stdout");
    need_low(stderr, "stderr");
    setvbuf(stderr, NULL, _IONBF, 0);

    if (getenv("PORT64_NO_FAULT_REPORT") == NULL) {
        static char altstack[65536];
        stack_t ss;
        struct sigaction sa;
        ss.ss_sp = altstack;
        ss.ss_size = sizeof altstack;
        ss.ss_flags = 0;
        sigaltstack(&ss, NULL);
        memset(&sa, 0, sizeof sa);
        sa.sa_sigaction = on_fault;
        sa.sa_flags = SA_SIGINFO | SA_ONSTACK;
        __real_sigaction(SIGSEGV, &sa, NULL);
        __real_sigaction(SIGBUS, &sa, NULL);
        __real_sigaction(SIGILL, &sa, NULL);
        __real_sigaction(SIGFPE, &sa, NULL);
    }
    stack = __real_mmap(NULL, GAME_STACK_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_32BIT, -1, 0);
    if (stack == MAP_FAILED) {
        perror("main64: mmap MAP_32BIT stack");
        return 1;
    }
    need_low((char *)stack + GAME_STACK_SIZE - 1, "the game stack");
    getcontext(&sGameCtx);
    sGameCtx.uc_stack.ss_sp = stack;
    sGameCtx.uc_stack.ss_size = GAME_STACK_SIZE;
    sGameCtx.uc_link = &sHostCtx;
    makecontext(&sGameCtx, game_entry, 0);
    swapcontext(&sHostCtx, &sGameCtx);
    fflush(stdout);
    return sRet;
}
