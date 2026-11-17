/* port/gfx: the host side of an M_GFXTASK (PC port plan section 19, M2).
 *
 * osSpTaskStartGo -> PortUltraConfig.rcpTask (port/main.c rcp_task) ->
 * port_gfx_task, which replaces main.c's weak no-op. Rendering (Fast3D) plugs
 * in here: built with -DPORT_GFX_FAST3D=ON (plan 19.7, option (a)) every task is rendered in
 * process by libultraship's Fast3D (fast3d/port_fast3d.cpp); in every build the task can be dumped:
 *
 *   PORT_GFX_DUMP=<dir>        write <dir>/gfx_NNNNNN.dlmp for each gfx task (dir created, mkdir -p)
 *   PORT_GFX_DUMP_FIRST=<n>    skip the first n tasks (default 0)
 *   PORT_GFX_DUMP_EVERY=<n>    then dump every n-th task (default 1)
 *   PORT_GFX_DUMP_COUNT=<n>    at most n files (default unlimited)
 *
 * The format is in dlump.h; port/gfx/tools/dlump.py reads and checks it, and
 * port/gfx/replay renders it. */

#include "../port_libc.h"
#include "../ultra/ultra.h"
#include "dlump.h"
#ifdef PORT_GFX_FAST3D
#include "fast3d/port_fast3d.h"
#endif

#if defined(__linux__)
int mincore(void *addr, size_t len, unsigned char *vec);
#define HAVE_MINCORE 1
#endif

#if defined(_WIN32)
#include <direct.h>
#define port_mkdir(p) _mkdir(p)
#else
#include <sys/stat.h>
#define port_mkdir(p) mkdir((p), 0777)
#endif
#include <errno.h>

typedef struct DumpCfg {
    int inited;
    const char *dir;
    unsigned long first, every, count;
    unsigned long written;
} DumpCfg;

static DumpCfg sDump;
static u32 sGfxTasks;

static unsigned long env_ul(const char *name, unsigned long def) {
    const char *v = getenv(name);
    return (v != NULL && *v != '\0') ? strtoul(v, NULL, 0) : def;
}

#ifdef HAVE_MINCORE
/* [p, p + n) is mapped: mincore fails with ENOMEM on an unmapped page. */
static int mapped(const void *p, size_t n) {
    static unsigned char vec[256];
    size_t a = (size_t)p & ~(size_t)4095, e = ((size_t)p + n + 4095) & ~(size_t)4095;
    while (a < e) {
        size_t len = e - a;
        if (len > sizeof(vec) * 4096) {
            len = sizeof(vec) * 4096;
        }
        if (mincore((void *)a, len, vec) != 0) {
            return 0;
        }
        a += len;
    }
    return 1;
}
#endif

/* mkdir -p: create dir and its missing parents. Errors are left to the fopen
 * that follows (it logs "cannot write"). */
static void make_dirs(const char *dir) {
    char tmp[512];
    size_t n = strlen(dir), i;

    if (n == 0 || n >= sizeof(tmp)) {
        return;
    }
    memcpy(tmp, dir, n + 1);
    for (i = 1; i <= n; i++) {
        if (tmp[i] == '/' || tmp[i] == '\0'
#if defined(_WIN32)
            || tmp[i] == '\\'
#endif
        ) {
            char c = tmp[i];
            tmp[i] = '\0';
            if (port_mkdir(tmp) != 0 && errno != EEXIST) {
                return;
            }
            tmp[i] = c;
        }
    }
}

static void dump_task(OSTask_t *task, u32 seq) {
    DlumpBuf buf = { 0 };
    DlumpOptions opt;
    char path[512];
    FILE *f;
    int errs;

    opt.arenaBase = 0;
#ifdef HAVE_MINCORE
    opt.readable = mapped;
#else
    opt.readable = NULL;
#endif
    errs = dlump_task(&buf, &opt, seq, (dlump_u32)(size_t)task, task->type, (dlump_u32)(size_t)task->data_ptr,
                      task->data_size);
    if (errs < 0) {
        port_ultra_log("gfx: dump %u: out of memory", seq);
        dlump_buf_free(&buf);
        return;
    }
    snprintf(path, sizeof(path), "%s/gfx_%06u.dlmp", sDump.dir, seq);
    f = fopen(path, "wb");
    if (f == NULL) {
        port_ultra_log("gfx: cannot write %s", path);
    } else {
        fwrite(buf.data, 1, buf.size, f);
        fclose(f);
        sDump.written++;
        if (errs > 0) {
            port_ultra_log("gfx: %s: %d walk errors", path, errs);
        }
    }
    dlump_buf_free(&buf);
}

void port_gfx_task(OSTask_t *task) {
    u32 seq = sGfxTasks++;

    if (!sDump.inited) {
        sDump.inited = 1;
        sDump.dir = getenv("PORT_GFX_DUMP");
        if (sDump.dir != NULL && *sDump.dir == '\0') {
            sDump.dir = NULL;
        }
        sDump.first = env_ul("PORT_GFX_DUMP_FIRST", 0);
        sDump.every = env_ul("PORT_GFX_DUMP_EVERY", 1);
        sDump.count = env_ul("PORT_GFX_DUMP_COUNT", (unsigned long)-1);
        if (sDump.every == 0) {
            sDump.every = 1;
        }
        if (sDump.dir != NULL) {
            make_dirs(sDump.dir);
        }
    }
    if (sDump.dir != NULL && seq >= sDump.first && (seq - sDump.first) % sDump.every == 0 &&
        sDump.written < sDump.count) {
        dump_task(task, seq);
    }
#ifdef PORT_GFX_FAST3D
    /* option (a): render in process (the dump above, if any, saw the same lists) */
    port_fast3d_task((u32)(size_t)task->data_ptr);
#endif
    /* option (b), the default here: the dump goes to fast3d/f3dreplay. */
}
