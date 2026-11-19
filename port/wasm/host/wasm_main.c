/* port/wasm/host/wasm_main.c -- main() of the wasm2c-translated game (plan section 30).
 *
 * Sets the FPU as the i386 / x86-64 builds' main.c does (flush-to-zero and denormals-are-zero, round to
 * nearest: the VR4300's FCSR; the translated code is plain C float arithmetic, so the host's mode applies),
 * passes argv and the environment (plus PORT_WASM_CWD, the directory the game's relative paths start
 * from), instantiates the module and runs its _start. A trap (an out-of-bounds access, unreachable, an
 * indirect call through a null slot) is reported and exits 134. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#if defined(__x86_64__) || defined(__i386__)
#include <xmmintrin.h>
#endif

#include "wasm_host.h"
#include "posix_launch.h"
#include "wasm-rt.h"
#include "wasm-rt-exceptions.h"
#include "wasm-rt-impl.h"

struct w2c_port {
    int dummy;
};

static w2c_game sGame;
static struct w2c_port sPort;
static struct w2c_wasi__snapshot__preview1 *sWasi;
static int sRunning;

uint8_t *port_wasm_mem_base(void) {
    return sGame.w2c_memory.data;
}

uint64_t port_wasm_mem_size(void) {
    return sGame.w2c_memory.size;
}

uint32_t port_wasm_malloc(uint32_t size) {
    return w2c_game_malloc(&sGame, size);
}

void port_wasm_exit(int code) {
    fflush(stdout);
    fflush(stderr);
    if (code != 0 && port_posix_gui()) {
        char msg[512];
        snprintf(msg, sizeof(msg), "The game stopped (exit %d). The reason is in %s/1080.log.", code,
                 port_posix_data_dir());
        port_posix_alert(msg);
    }
    exit(code);
}

static void fpu_setup(void) {
#if defined(__x86_64__) || defined(__i386__)
    unsigned int csr = _mm_getcsr();
    csr |= 0x8040u;  /* FTZ | DAZ */
    csr &= ~0x6000u; /* round to nearest */
    csr |= 0x1F80u;  /* exceptions masked */
    _mm_setcsr(csr);
#elif defined(__aarch64__)
    unsigned long fpcr;
    __asm__ volatile("mrs %0, fpcr" : "=r"(fpcr));
    fpcr |= 1ul << 24;            /* FZ: flush denormal inputs and results to zero */
    fpcr &= ~(3ul << 22);         /* RMode = round to nearest */
    __asm__ volatile("msr fpcr, %0" : : "r"(fpcr));
#endif
}

#if defined(__linux__) || defined(__APPLE__)
#include <execinfo.h>
#endif

/* WASM_RT_TRAP_HANDLER (wasm-rt-impl.c is compiled with -DWASM_RT_TRAP_HANDLER=port_wasm_trap): the trap,
 * then the native return addresses (the wasm2c functions are w2c_game_<name>; addr2line -f -e <exe>) */
void port_wasm_trap(wasm_rt_trap_t code) {
    fflush(stdout);
    fprintf(stderr, "PORT: wasm trap: %s\n", wasm_rt_strerror(code));
    if (getenv("PORT_WASM_PEEK") != NULL) { /* debugging: PORT_WASM_PEEK=<game address>: 8 words there */
        uint32_t a = (uint32_t)strtoul(getenv("PORT_WASM_PEEK"), NULL, 0), i, w;
        for (i = 0; i < 8 && a + 4 * i + 4 <= sGame.w2c_memory.size; i++) {
            memcpy(&w, sGame.w2c_memory.data + a + 4 * i, 4);
            fprintf(stderr, "  [0x%08x] 0x%08x\n", a + 4 * i, w);
        }
        fprintf(stderr, "  table size %u\n", (unsigned)sGame.w2c_T0.size);
    }
#if defined(__linux__) || defined(__APPLE__)
    {
        void *frames[48];
        int n = backtrace(frames, 48);
        backtrace_symbols_fd(frames, n, 2);
    }
#endif
    port_wasm_exit(134);
}

#if defined(__linux__) || defined(__APPLE__)
#include <signal.h>
/* PORT_WASM_STACK_SIGNAL: kill -USR1 prints where the game is (a hang) */
static void on_usr1(int sig) {
    void *frames[48];
    int n = backtrace(frames, 48);
    (void)sig;
    backtrace_symbols_fd(frames, n, 2);
}
#endif

/* PORT_WASM_NULLCHECK builds: a load / store below the 16 MB global base (port/wasm/w2c-post.py) */
void port_wasm_null_access(uint64_t a) {
    fprintf(stderr, "PORT: access at 0x%llx (a null pointer + offset)\n", (unsigned long long)a);
    wasm_rt_trap(WASM_RT_TRAP_OOB);
}

/* PORT_WASM_NULLCHECK builds: PORT_WASM_WATCH=<addr>[:<bytes>] reports every load / store that touches it, with
 * the native stack (debugging a corrupted variable) */
uint64_t port_wasm_watch_lo = ~0ull, port_wasm_watch_hi = 0;
void port_wasm_watch_hit(uint64_t a, unsigned n) {
    static int count;
    if (count++ < 20000) {
        fprintf(stderr, "PORT: watch: access at 0x%llx (%u bytes)\n", (unsigned long long)a, n);
#if defined(__linux__) || defined(__APPLE__)
        {
            void *frames[16];
            int k = backtrace(frames, 16);
            backtrace_symbols_fd(frames, k, 2);
        }
#endif
    }
}

/* an indirect call through a null or out-of-range table slot (port/wasm/w2c-post.py) */
void port_wasm_bad_icall(uint32_t x, uint32_t size) {
    fprintf(stderr, "PORT: indirect call through table slot %u (table size %u)\n", (unsigned)x, (unsigned)size);
    wasm_rt_trap(WASM_RT_TRAP_CALL_INDIRECT);
}

#if defined(__linux__) || defined(__APPLE__)
/* a native fault that is not a guard-page trap (wasm-rt's handler passes those on): where, then the default */
static struct sigaction sPrevSegv;
static void on_segv(int sig, siginfo_t *si, void *uc) {
    uint8_t *base = sGame.w2c_memory.data;
    uint8_t *a = (uint8_t *)si->si_addr;
    if (a >= base && a < base + (8ull << 30)) { /* inside the reserved memory: wasm-rt reports the trap */
        if (sPrevSegv.sa_flags & SA_SIGINFO) {
            sPrevSegv.sa_sigaction(sig, si, uc);
        }
        return;
    }
    fprintf(stderr, "PORT: native fault at %p (signal %d)\n", si->si_addr, sig);
    {
        void *frames[48];
        int n = backtrace(frames, 48);
        backtrace_symbols_fd(frames, n, 2);
    }
    signal(sig, SIG_DFL);
}
#endif

extern char **environ;

int main(int argc, char **argv) {
    char cwd[4096], *cwdEnv;
    char **env;
    int n, i, trap;

    argv = port_posix_launch(argc, argv, &argc); /* no arguments: the double-click set-up (data dir, ROM) */
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        strcpy(cwd, "/");
    }
    for (n = 0; environ[n] != NULL; n++) {
    }
    env = (char **)calloc((size_t)n + 2, sizeof(char *));
    for (i = 0; i < n; i++) {
        env[i] = environ[i];
    }
    cwdEnv = (char *)malloc(strlen(cwd) + 16);
    sprintf(cwdEnv, "PORT_WASM_CWD=%s", cwd);
    env[n] = cwdEnv;
    port_wasi_init(argc, argv, env);

#if defined(__linux__) || defined(__APPLE__)
    signal(SIGUSR1, on_usr1);
#endif
    if (getenv("PORT_WASM_WATCH") != NULL) {
        char *e;
        port_wasm_watch_lo = strtoull(getenv("PORT_WASM_WATCH"), &e, 0);
        port_wasm_watch_hi = port_wasm_watch_lo + (*e == ':' ? strtoull(e + 1, NULL, 0) : 1);
    }
    fpu_setup();
    wasm_rt_init();
#if defined(__linux__) || defined(__APPLE__)
    {
        struct sigaction sa;
        memset(&sa, 0, sizeof(sa));
        sa.sa_sigaction = on_segv;
        sa.sa_flags = SA_SIGINFO | SA_ONSTACK;
        sigemptyset(&sa.sa_mask);
        sigaction(SIGSEGV, &sa, &sPrevSegv);
        sigaction(SIGBUS, &sa, NULL);
    }
#endif
    wasm2c_game_instantiate(&sGame, &sPort, sWasi);
    sRunning = 1;
    trap = wasm_rt_impl_try();
    if (trap == 0) {
        w2c_game_0x5Fstart(&sGame);
        port_wasm_exit(0);
    }
    fflush(stdout);
    fprintf(stderr, "PORT: wasm trap: %s\n", wasm_rt_strerror((wasm_rt_trap_t)trap));
    return 134;
}
