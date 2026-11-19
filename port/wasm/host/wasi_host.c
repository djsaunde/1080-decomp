/* port/wasm/host/wasi_host.c -- the WASI (preview 1) calls the wasm32 game makes, for its wasm2c translation
 * (plan section 30).
 *
 * The game is compiled against wasi-libc; its stdio, fopen, clock_gettime, nanosleep, getenv and mkdir end
 * in these imports. This is not a sandbox: one preopened directory "/" is the host's root, the game's
 * relative paths are made absolute by main.c (it chdir()s to PORT_WASM_CWD, the host's current directory),
 * so every path the game names is the host path it names on Linux. Guest pointers are offsets in the
 * instance's linear memory (MEM(a)). POSIX hosts only (Linux, macOS). */
#define _GNU_SOURCE 1
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <time.h>
#include <unistd.h>

#include "wasm_host.h"

#define WASI_ESUCCESS 0
#define WASI_EBADF 8
#define WASI_EINVAL 28
#define WASI_ENOSYS 52

#define MAX_FDS 256

struct w2c_wasi__snapshot__preview1 {
    int dummy;
};

static int sFd[MAX_FDS]; /* wasi fd -> host fd, -1 free */
static int sArgc;
static char **sArgv;
static char **sEnv;
static int sEnvc;

static uint8_t *mem_base(void) {
    return port_wasm_mem_base();
}
#define MEM(a) (mem_base() + (uint32_t)(a))

static uint16_t wasi_errno(int e) {
    switch (e) {
    case 0: return 0;
    case E2BIG: return 1;
    case EACCES: return 2;
    case EAGAIN: return 6;
    case EBADF: return 8;
    case EBUSY: return 10;
    case EEXIST: return 20;
    case EFAULT: return 21;
    case EFBIG: return 22;
    case EINTR: return 27;
    case EINVAL: return 28;
    case EIO: return 29;
    case EISDIR: return 31;
    case ELOOP: return 32;
    case EMFILE: return 33;
    case ENAMETOOLONG: return 37;
    case ENOENT: return 44;
    case ENOMEM: return 48;
    case ENOSPC: return 51;
    case ENOSYS: return 52;
    case ENOTDIR: return 54;
    case ENOTEMPTY: return 55;
    case EPERM: return 63;
    case EPIPE: return 64;
    case EROFS: return 69;
    case ESPIPE: return 70;
    default: return 29;
    }
}

static void put32(uint32_t a, uint32_t v) { memcpy(MEM(a), &v, 4); }
static void put64(uint32_t a, uint64_t v) { memcpy(MEM(a), &v, 8); }
static uint32_t get32(uint32_t a) { uint32_t v; memcpy(&v, MEM(a), 4); return v; }

static int host_fd(uint32_t fd) {
    return fd < MAX_FDS ? sFd[fd] : -1;
}

void port_wasi_init(int argc, char **argv, char **envp) {
    int i, root;
    for (i = 0; i < MAX_FDS; i++) {
        sFd[i] = -1;
    }
    sFd[0] = 0;
    sFd[1] = 1;
    sFd[2] = 2;
    root = open("/", O_RDONLY | O_DIRECTORY);
    if (root < 0) {
        perror("wasi: open /");
        exit(1);
    }
    sFd[3] = root; /* the one preopen: "/" */
    sArgc = argc;
    sArgv = argv;
    sEnv = envp;
    for (sEnvc = 0; envp != NULL && envp[sEnvc] != NULL; sEnvc++) {
    }
}

/* ---- args / environ ---- */
static uint32_t strings_get(char **v, int n, uint32_t ptrs, uint32_t buf) {
    int i;
    for (i = 0; i < n; i++) {
        size_t l = strlen(v[i]) + 1;
        put32(ptrs + 4 * i, buf);
        memcpy(MEM(buf), v[i], l);
        buf += (uint32_t)l;
    }
    return WASI_ESUCCESS;
}
static uint32_t strings_sizes(char **v, int n, uint32_t count, uint32_t size) {
    int i;
    uint32_t total = 0;
    for (i = 0; i < n; i++) {
        total += (uint32_t)strlen(v[i]) + 1;
    }
    put32(count, (uint32_t)n);
    put32(size, total);
    return WASI_ESUCCESS;
}
u32 w2c_wasi__snapshot__preview1_args_get(struct w2c_wasi__snapshot__preview1 *w, u32 argv, u32 buf) {
    (void)w;
    return strings_get(sArgv, sArgc, argv, buf);
}
u32 w2c_wasi__snapshot__preview1_args_sizes_get(struct w2c_wasi__snapshot__preview1 *w, u32 count, u32 size) {
    (void)w;
    return strings_sizes(sArgv, sArgc, count, size);
}
u32 w2c_wasi__snapshot__preview1_environ_get(struct w2c_wasi__snapshot__preview1 *w, u32 env, u32 buf) {
    (void)w;
    return strings_get(sEnv, sEnvc, env, buf);
}
u32 w2c_wasi__snapshot__preview1_environ_sizes_get(struct w2c_wasi__snapshot__preview1 *w, u32 count, u32 size) {
    (void)w;
    return strings_sizes(sEnv, sEnvc, count, size);
}

/* ---- clocks, random, exit ---- */
u32 w2c_wasi__snapshot__preview1_clock_time_get(struct w2c_wasi__snapshot__preview1 *w, u32 id, u64 prec, u32 out) {
    struct timespec ts;
    clockid_t c = id == 0 ? CLOCK_REALTIME : id == 1 ? CLOCK_MONOTONIC : CLOCK_PROCESS_CPUTIME_ID;
    (void)w, (void)prec;
    if (clock_gettime(c, &ts) != 0) {
        return wasi_errno(errno);
    }
    put64(out, (uint64_t)ts.tv_sec * 1000000000ull + (uint64_t)ts.tv_nsec);
    return WASI_ESUCCESS;
}
u32 w2c_wasi__snapshot__preview1_clock_res_get(struct w2c_wasi__snapshot__preview1 *w, u32 id, u32 out) {
    (void)w, (void)id;
    put64(out, 1000);
    return WASI_ESUCCESS;
}
u32 w2c_wasi__snapshot__preview1_random_get(struct w2c_wasi__snapshot__preview1 *w, u32 buf, u32 len) {
    /* deterministic: only libc internals ask (the game has its own generator) */
    static uint32_t x = 0x1080u;
    uint32_t i;
    (void)w;
    for (i = 0; i < len; i++) {
        x = x * 1664525u + 1013904223u;
        MEM(buf)[i] = (uint8_t)(x >> 24);
    }
    return WASI_ESUCCESS;
}
void w2c_wasi__snapshot__preview1_proc_exit(struct w2c_wasi__snapshot__preview1 *w, u32 code) {
    (void)w;
    port_wasm_exit((int)code);
}
u32 w2c_wasi__snapshot__preview1_sched_yield(struct w2c_wasi__snapshot__preview1 *w) {
    (void)w;
    return WASI_ESUCCESS;
}

/* poll_oneoff: only the clock subscriptions nanosleep makes (subscription: userdata u64, tag u8 at 8,
 * clock: id u32 at 16, timeout u64 at 24, precision u64 at 32, flags u16 at 40; 48 bytes) */
u32 w2c_wasi__snapshot__preview1_poll_oneoff(struct w2c_wasi__snapshot__preview1 *w, u32 in, u32 out, u32 n,
                                              u32 nout) {
    uint32_t i;
    (void)w;
    for (i = 0; i < n; i++) {
        uint32_t s = in + 48 * i;
        uint64_t timeout;
        uint16_t flags;
        memcpy(&timeout, MEM(s + 24), 8);
        memcpy(&flags, MEM(s + 40), 2);
        if (MEM(s + 8)[0] == 0) {
            struct timespec ts;
            if (flags & 1) { /* absolute */
                struct timespec now;
                uint64_t t;
                clock_gettime(CLOCK_MONOTONIC, &now);
                t = (uint64_t)now.tv_sec * 1000000000ull + (uint64_t)now.tv_nsec;
                timeout = timeout > t ? timeout - t : 0;
            }
            ts.tv_sec = (time_t)(timeout / 1000000000ull);
            ts.tv_nsec = (long)(timeout % 1000000000ull);
            nanosleep(&ts, NULL);
        }
        memset(MEM(out + 32 * i), 0, 32);
        memcpy(MEM(out + 32 * i), MEM(s), 8);
        MEM(out + 32 * i)[10] = MEM(s + 8)[0];
    }
    put32(nout, n);
    return WASI_ESUCCESS;
}

/* ---- file descriptors ---- */
static int alloc_fd(int h) {
    int i;
    for (i = 4; i < MAX_FDS; i++) {
        if (sFd[i] < 0) {
            sFd[i] = h;
            return i;
        }
    }
    return -1;
}

u32 w2c_wasi__snapshot__preview1_fd_close(struct w2c_wasi__snapshot__preview1 *w, u32 fd) {
    int h = host_fd(fd);
    (void)w;
    if (h < 0 || fd <= 3) {
        return WASI_EBADF;
    }
    sFd[fd] = -1;
    return close(h) == 0 ? WASI_ESUCCESS : wasi_errno(errno);
}

u32 w2c_wasi__snapshot__preview1_fd_fdstat_get(struct w2c_wasi__snapshot__preview1 *w, u32 fd, u32 out) {
    int h = host_fd(fd);
    struct stat st;
    uint8_t type = 0;
    (void)w;
    if (h < 0 || fstat(h, &st) != 0) {
        return WASI_EBADF;
    }
    if (S_ISDIR(st.st_mode)) {
        type = 3;
    } else if (S_ISREG(st.st_mode)) {
        type = 4;
    } else if (S_ISCHR(st.st_mode)) {
        type = 2;
    } else if (S_ISFIFO(st.st_mode)) {
        type = 6; /* socket_stream: no seek */
    }
    memset(MEM(out), 0, 24);
    MEM(out)[0] = type;
    put64(out + 8, type == 2 ? ~(uint64_t)((1u << 2) | (1u << 5)) : ~0ull); /* a tty: no seek / tell */
    put64(out + 16, ~0ull);
    return WASI_ESUCCESS;
}
u32 w2c_wasi__snapshot__preview1_fd_fdstat_set_flags(struct w2c_wasi__snapshot__preview1 *w, u32 fd, u32 flags) {
    (void)w, (void)flags;
    return host_fd(fd) < 0 ? WASI_EBADF : WASI_ESUCCESS;
}

/* filestat: dev u64, ino u64, filetype u8 (at 16), nlink u64 (24), size u64 (32), atim, mtim, ctim (40..64) */
static void put_filestat(uint32_t out, const struct stat *st) {
    uint8_t type = S_ISDIR(st->st_mode) ? 3 : S_ISREG(st->st_mode) ? 4 : S_ISCHR(st->st_mode) ? 2 : 0;
    memset(MEM(out), 0, 64);
    put64(out, (uint64_t)st->st_dev);
    put64(out + 8, (uint64_t)st->st_ino);
    MEM(out)[16] = type;
    put64(out + 24, (uint64_t)st->st_nlink);
    put64(out + 32, (uint64_t)st->st_size);
    put64(out + 40, (uint64_t)st->st_atime * 1000000000ull);
    put64(out + 48, (uint64_t)st->st_mtime * 1000000000ull);
    put64(out + 56, (uint64_t)st->st_ctime * 1000000000ull);
}
u32 w2c_wasi__snapshot__preview1_fd_filestat_get(struct w2c_wasi__snapshot__preview1 *w, u32 fd, u32 out) {
    int h = host_fd(fd);
    struct stat st;
    (void)w;
    if (h < 0 || fstat(h, &st) != 0) {
        return WASI_EBADF;
    }
    put_filestat(out, &st);
    return WASI_ESUCCESS;
}

u32 w2c_wasi__snapshot__preview1_fd_prestat_get(struct w2c_wasi__snapshot__preview1 *w, u32 fd, u32 out) {
    (void)w;
    if (fd != 3) {
        return WASI_EBADF;
    }
    memset(MEM(out), 0, 8);
    put32(out + 4, 1); /* "/" */
    return WASI_ESUCCESS;
}
u32 w2c_wasi__snapshot__preview1_fd_prestat_dir_name(struct w2c_wasi__snapshot__preview1 *w, u32 fd, u32 path,
                                                      u32 len) {
    (void)w;
    if (fd != 3 || len < 1) {
        return WASI_EBADF;
    }
    MEM(path)[0] = '/';
    return WASI_ESUCCESS;
}

static uint32_t do_iov(int h, uint32_t iovs, uint32_t n, uint32_t out, int writing) {
    struct iovec v[64];
    uint32_t i;
    ssize_t r;
    if (n > 64) {
        n = 64;
    }
    for (i = 0; i < n; i++) {
        v[i].iov_base = MEM(get32(iovs + 8 * i));
        v[i].iov_len = get32(iovs + 8 * i + 4);
    }
    r = writing ? writev(h, v, (int)n) : readv(h, v, (int)n);
    if (r < 0) {
        return wasi_errno(errno);
    }
    put32(out, (uint32_t)r);
    return WASI_ESUCCESS;
}
u32 w2c_wasi__snapshot__preview1_fd_read(struct w2c_wasi__snapshot__preview1 *w, u32 fd, u32 iovs, u32 n, u32 out) {
    int h = host_fd(fd);
    (void)w;
    return h < 0 ? WASI_EBADF : do_iov(h, iovs, n, out, 0);
}
u32 w2c_wasi__snapshot__preview1_fd_write(struct w2c_wasi__snapshot__preview1 *w, u32 fd, u32 iovs, u32 n, u32 out) {
    int h = host_fd(fd);
    (void)w;
    return h < 0 ? WASI_EBADF : do_iov(h, iovs, n, out, 1);
}
u32 w2c_wasi__snapshot__preview1_fd_seek(struct w2c_wasi__snapshot__preview1 *w, u32 fd, u64 off, u32 whence,
                                          u32 out) {
    int h = host_fd(fd);
    off_t r;
    (void)w;
    if (h < 0) {
        return WASI_EBADF;
    }
    r = lseek(h, (off_t)(int64_t)off, whence == 0 ? SEEK_SET : whence == 1 ? SEEK_CUR : SEEK_END);
    if (r < 0) {
        return wasi_errno(errno);
    }
    put64(out, (uint64_t)r);
    return WASI_ESUCCESS;
}
u32 w2c_wasi__snapshot__preview1_fd_tell(struct w2c_wasi__snapshot__preview1 *w, u32 fd, u32 out) {
    return w2c_wasi__snapshot__preview1_fd_seek(w, fd, 0, 1, out);
}
u32 w2c_wasi__snapshot__preview1_fd_sync(struct w2c_wasi__snapshot__preview1 *w, u32 fd) {
    int h = host_fd(fd);
    (void)w;
    return h < 0 ? WASI_EBADF : (fsync(h) == 0 ? WASI_ESUCCESS : wasi_errno(errno));
}

static int path_arg(uint32_t path, uint32_t len, char *buf, size_t cap) {
    if (len + 1 > cap) {
        return -1;
    }
    memcpy(buf, MEM(path), len);
    buf[len] = '\0';
    return 0;
}

u32 w2c_wasi__snapshot__preview1_path_open(struct w2c_wasi__snapshot__preview1 *w, u32 dirfd, u32 dirflags, u32 path,
                                            u32 len, u32 oflags, u64 rights, u64 inherit, u32 fdflags, u32 out) {
    char p[4096];
    int d = host_fd(dirfd), flags = 0, h, fd;
    int rd = (rights & (1ull << 1)) != 0, wr = (rights & (1ull << 6)) != 0;
    (void)w, (void)dirflags, (void)inherit;
    if (d < 0 || path_arg(path, len, p, sizeof(p)) != 0) {
        return WASI_EBADF;
    }
    flags = rd && wr ? O_RDWR : wr ? O_WRONLY : O_RDONLY;
    if (oflags & 1) flags |= O_CREAT;
    if (oflags & 2) flags |= O_DIRECTORY;
    if (oflags & 4) flags |= O_EXCL;
    if (oflags & 8) flags |= O_TRUNC;
    if (fdflags & 1) flags |= O_APPEND;
    h = openat(d, p[0] != '\0' ? p : ".", flags | O_CLOEXEC, 0666);
    if (h < 0 && errno == EISDIR && !(oflags & 2)) {
        h = openat(d, p, O_RDONLY | O_DIRECTORY | O_CLOEXEC);
    }
    if (h < 0) {
        return wasi_errno(errno);
    }
    fd = alloc_fd(h);
    if (fd < 0) {
        close(h);
        return 33;
    }
    put32(out, (uint32_t)fd);
    return WASI_ESUCCESS;
}
u32 w2c_wasi__snapshot__preview1_path_create_directory(struct w2c_wasi__snapshot__preview1 *w, u32 dirfd, u32 path,
                                                        u32 len) {
    char p[4096];
    int d = host_fd(dirfd);
    (void)w;
    if (d < 0 || path_arg(path, len, p, sizeof(p)) != 0) {
        return WASI_EBADF;
    }
    return mkdirat(d, p, 0777) == 0 ? WASI_ESUCCESS : wasi_errno(errno);
}
u32 w2c_wasi__snapshot__preview1_path_filestat_get(struct w2c_wasi__snapshot__preview1 *w, u32 dirfd, u32 flags,
                                                    u32 path, u32 len, u32 out) {
    char p[4096];
    int d = host_fd(dirfd);
    struct stat st;
    (void)w;
    if (d < 0 || path_arg(path, len, p, sizeof(p)) != 0) {
        return WASI_EBADF;
    }
    if (fstatat(d, p, &st, (flags & 1) ? 0 : AT_SYMLINK_NOFOLLOW) != 0) {
        return wasi_errno(errno);
    }
    put_filestat(out, &st);
    return WASI_ESUCCESS;
}
u32 w2c_wasi__snapshot__preview1_path_unlink_file(struct w2c_wasi__snapshot__preview1 *w, u32 dirfd, u32 path,
                                                   u32 len) {
    char p[4096];
    int d = host_fd(dirfd);
    (void)w;
    if (d < 0 || path_arg(path, len, p, sizeof(p)) != 0) {
        return WASI_EBADF;
    }
    return unlinkat(d, p, 0) == 0 ? WASI_ESUCCESS : wasi_errno(errno);
}
u32 w2c_wasi__snapshot__preview1_path_rename(struct w2c_wasi__snapshot__preview1 *w, u32 fd1, u32 p1, u32 l1, u32 fd2,
                                              u32 p2, u32 l2) {
    char a[4096], b[4096];
    int d1 = host_fd(fd1), d2 = host_fd(fd2);
    (void)w;
    if (d1 < 0 || d2 < 0 || path_arg(p1, l1, a, sizeof(a)) != 0 || path_arg(p2, l2, b, sizeof(b)) != 0) {
        return WASI_EBADF;
    }
    return renameat(d1, a, d2, b) == 0 ? WASI_ESUCCESS : wasi_errno(errno);
}
