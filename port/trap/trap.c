/* 1080 PC port: PORT_TRAP_UNDEFINED runtime (debug builds only; plan 18.9).
 *
 * With -DPORT_TRAP_UNDEFINED=ON the CMake links a probe executable with
 * --warn-unresolved-symbols, lists what is still undefined (nm -u, unversioned),
 * and port/tools/gen-port-traps.py writes one trap per symbol into
 * <build>/port_traps.c:
 *
 *   - a code symbol (called from .text, R_386_PC32) becomes a function that
 *     calls port_trap_code("<symbol>"): it prints "PORT TRAP: <symbol>", a
 *     backtrace, and aborts;
 *   - a data symbol becomes an address in a PROT_NONE region (.port_trap_data,
 *     two pages per symbol, the symbol at the start of its second page).  Any
 *     load / store within +-4 KiB of it faults; the SIGSEGV handler below names
 *     the symbol ("PORT TRAP: <symbol> (data, +off)"), prints the backtrace and
 *     aborts.  Taking the address alone does not trap.
 *
 * Nothing here returns: a trap is a missing feature, reported loudly.  This is
 * NOT a stub.  The option is OFF by default and a normal build never contains
 * this file; the tenshoe link then fails on the same symbols, as it should.
 */
#define _GNU_SOURCE
#include <execinfo.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

/* generated (port_traps.c) */
extern const char *const port_trap_data_names[];
extern const unsigned port_trap_data_count;
extern char port_trap_data_start[], port_trap_data_end[];

#define TRAP_PAGE 4096u

static void trap_backtrace(void) {
    void *frames[64];
    int n = backtrace(frames, 64);
    fprintf(stderr, "PORT TRAP: backtrace (%d frames; addr2line -f -e tenshoe <addr>):\n", n);
    fflush(stderr);
    backtrace_symbols_fd(frames, n, 2);
}

__attribute__((noreturn, noinline)) void port_trap_code(const char *sym) {
    fprintf(stderr, "PORT TRAP: %s (code: called from %p)\n", sym, __builtin_return_address(0));
    trap_backtrace();
    abort();
}

static void trap_segv(int sig, siginfo_t *si, void *uctx) {
    uintptr_t a = (uintptr_t)si->si_addr;
    uintptr_t lo = (uintptr_t)port_trap_data_start, hi = (uintptr_t)port_trap_data_end;
    (void)uctx;
    if (a >= lo && a < hi) {
        /* symbol i sits at lo + (2i + 1) pages; it owns [sym - 1 page, sym + 1 page) */
        unsigned i = (unsigned)((a - lo) / (2 * TRAP_PAGE));
        long off;
        if (i >= port_trap_data_count) {
            i = port_trap_data_count - 1;
        }
        off = (long)(a - (lo + (2 * i + 1) * TRAP_PAGE));
        fprintf(stderr, "PORT TRAP: %s (data, %+ld, fault address %p)\n", port_trap_data_names[i], off,
                si->si_addr);
        trap_backtrace();
        abort();
    }
    /* not ours: report it, restore the default action, and let the access fault again */
    fprintf(stderr, "PORT: SIGSEGV at %p (not a trap)\n", si->si_addr);
    trap_backtrace();
    signal(sig, SIG_DFL);
}

__attribute__((constructor)) static void port_trap_install(void) {
    struct sigaction sa;
    static char altstack[64 * 1024];
    stack_t ss;
    size_t len = (size_t)(port_trap_data_end - port_trap_data_start);
    if (len != 0 && mprotect(port_trap_data_start, len, PROT_NONE) != 0) {
        perror("PORT TRAP: mprotect");
        abort();
    }
    ss.ss_sp = altstack;
    ss.ss_size = sizeof(altstack);
    ss.ss_flags = 0;
    sigaltstack(&ss, NULL);
    memset(&sa, 0, sizeof(sa));
    sa.sa_sigaction = trap_segv;
    sa.sa_flags = SA_SIGINFO | SA_ONSTACK;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGSEGV, &sa, NULL);
    fprintf(stderr, "PORT TRAP: PORT_TRAP_UNDEFINED build: %u data traps armed (debug only)\n",
            port_trap_data_count);
}
