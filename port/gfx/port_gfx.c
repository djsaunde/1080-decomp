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
 *   PORT_GFX_DUMP_STEPS=<list> dump only the tasks of these steps (SysLib.frameCount, = main.c's
 *                              step and state.log's F number): "700,750-760,900" (replaces
 *                              FIRST / EVERY; COUNT still applies). Every dump, with or without
 *                              it, adds a "task step" line to <dir>/steps.txt.
 *   PORT_LIVE=<name>          send every task, as the same DLMP image, to the live presenter
 *                              (/dev/shm/<name>; port_live.c, plan 19.11; port/tools/play.sh)
 *   PORT_FB_LOG=1              log the framebuffer-model request counters every 100 tasks
 *
 * The format is in dlump.h; port/gfx/tools/dlump.py reads and checks it, and
 * port/gfx/replay renders it. */

#include "../port_libc.h"
#include "../ultra/ultra.h"
#include "dlump.h"
#include "port_fb_impl.h"
#include "port_live.h"
#ifdef PORT_GFX_NATIVE
#include "port_native.h"
#endif
#ifdef PORT_GFX_FAST3D
#include "fast3d/port_fast3d.h"
#endif

#if defined(__linux__)
int mincore(void *addr, size_t len, unsigned char *vec);
#define HAVE_MINCORE 1
#elif defined(_WIN32)
/* the Windows build (plan 29): mapped() asks VirtualQuery */
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
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
#ifndef PORT_ERRNO
#define PORT_ERRNO errno
#endif

typedef struct DumpCfg {
    int inited;
    const char *dir;
    unsigned long first, every, count;
    unsigned long written;
} DumpCfg;

static DumpCfg sDump;
static const char *sDumpSteps; /* PORT_GFX_DUMP_STEPS, or NULL */
extern u32 port_game_frame_count(void) __attribute__((weak));

static u32 cur_step(void) {
    return port_game_frame_count != NULL ? port_game_frame_count() : 0;
}

/* step in the PORT_GFX_DUMP_STEPS list ("a,b-c,...") */
static int step_listed(u32 step) {
    const char *p = sDumpSteps;
    while (*p != '\0') {
        char *e;
        unsigned long a = strtoul(p, &e, 10), b = a;
        if (e == p) {
            return 0;
        }
        if (*e == '-') {
            b = strtoul(e + 1, &e, 10);
        }
        if (step >= a && step <= b) {
            return 1;
        }
        p = *e == ',' ? e + 1 : e;
        if (*e != ',' && *e != '\0') {
            return 0;
        }
    }
    return 0;
}
static u32 sGfxTasks;
static int sFbLog; /* PORT_FB_LOG=1: the framebuffer-model request counters every 100 tasks */

static unsigned long env_ul(const char *name, unsigned long def) {
    const char *v = getenv(name);
    return (v != NULL && *v != '\0') ? strtoul(v, NULL, 0) : def;
}

#ifdef HAVE_MINCORE
/* [p, p + n) is mapped: mincore fails with ENOMEM on an unmapped page. */
#ifdef _WIN32
static int mapped(const void *p, size_t n) {
    size_t a = (size_t)p, e = (size_t)p + n;
    MEMORY_BASIC_INFORMATION mi;
    if (e < a) {
        return 0;
    }
    while (a < e || (n == 0 && a == e)) {
        if (VirtualQuery((const void *)a, &mi, sizeof(mi)) != sizeof(mi) || mi.State != MEM_COMMIT ||
            (mi.Protect & (PAGE_NOACCESS | PAGE_GUARD)) != 0) {
            return 0;
        }
        a = (size_t)mi.BaseAddress + mi.RegionSize;
        if (n == 0) {
            break;
        }
    }
    return 1;
}
#else
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
#else
static int mapped(const void *p, size_t n) {
    (void)n;
    return (size_t)p >= 0x1000;
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
            if (port_mkdir(tmp) != 0 && PORT_ERRNO != EEXIST) {
#if !defined(_WIN32)
                return;
#endif
                /* Windows: a UNC path's \\server and \\server\share cannot be made; go on */
            }
            tmp[i] = c;
        }
    }
}

#ifndef PORT_GFX_FAST3D
/* The depth-clear model (plan 19.10, run 855): with no renderer in process (headless, or
 * PORT_GFX_DUMP for f3dreplay) nothing draws into the N64 Z image, so the game's CPU reader
 * (Sun_measureVisibility: 9 pixels, 0xFFFC = sky) saw whatever the memory held (0xFFFF from the
 * allocator, i.e. "not far": the sun and lens flare never showed). The RDP's own part that does not
 * need a rasteriser is the clear: each frame the game sets the colour image to the Z image and fills it
 * (G_SETCIMG z; G_SETFILLCOLOR 0xFFFCFFFC; G_FILLRECT 0,0 319,239: GPACK_ZDZ(G_MAXFIX, 0)). This walk
 * applies those fill rectangles to the image in game memory, as the RDP would, so far pixels read
 * the cleared value. Geometry is NOT modelled: every pixel reads "far", so headless the sun counts
 * as unoccluded wherever it is on screen (behind a mountain too). It runs only for a task that has
 * pending depth reads (port_fb_read_depth), and fills only a requested image that this or an earlier
 * task named with G_SETZIMG (a 16-bit colour clear of the frame buffer is not written: the colour image stays
 * the renderer's). PORT_FB_ZCLEAR=0 turns it off. */
#define ZC_MAX_FILLS 8
static unsigned int sZImgs[4]; /* low 24 bits of the Z images seen (G_SETZIMG) */
static int sNZImgs;

typedef struct ZcFill {
    u32 img, widthM1;
    u32 color;
    u32 ulx, uly, lrx, lry;
} ZcFill;

static int zc_is_zimg(u32 a) {
    int i;
    for (i = 0; i < sNZImgs; i++) {
        if (sZImgs[i] == (a & 0xFFFFFFu)) {
            return 1;
        }
    }
    return 0;
}

static void zc_note_zimg(u32 a) {
    if (!zc_is_zimg(a) && sNZImgs < 4) {
        sZImgs[sNZImgs++] = a & 0xFFFFFFu;
    }
}

static void depth_clear_model(u32 root, const PortFbRequests *rq) {
    u32 stack[DLUMP_DL_STACK], seg[16];
    ZcFill fills[ZC_MAX_FILLS];
    int sp = 0, nFills = 0, i;
    u32 pc = root, cimg = 0, cimgW0 = 0, fillColor = 0, n = 0, cycle = 0;
    size_t lastPage = (size_t)-1;

    memset(seg, 0, sizeof(seg));
    for (; n < DLUMP_MAX_COMMANDS; n++) {
        u32 w0, w1;
        if ((pc & 7) != 0) {
            break;
        }
        if (((size_t)pc >> 12) != lastPage) {
            if (pc < 0x1000u || !mapped((const void *)(size_t)pc, 8)) {
                break;
            }
            lastPage = (size_t)pc >> 12;
        }
        memcpy(&w0, (const void *)(size_t)pc, 4);
        memcpy(&w1, (const void *)(size_t)(pc + 4), 4);
        pc += 8;
        switch (w0 >> 24) {
            case 0x06: /* G_DL */ {
                u32 t = seg[(w1 >> 24) & 15] != 0 ? seg[(w1 >> 24) & 15] + (w1 & 0xFFFFFFu) : w1;
                if (((w0 >> 16) & 0xFF) == 0) {
                    if (sp >= DLUMP_DL_STACK) {
                        goto done;
                    }
                    stack[sp++] = pc;
                }
                pc = t;
                break;
            }
            case 0xB8: /* G_ENDDL */
                if (sp == 0) {
                    goto done;
                }
                pc = stack[--sp];
                break;
            case 0xBC: /* G_MOVEWORD SEGMENT */
                if ((w0 & 0xFF) == 0x06) {
                    seg[((w0 >> 8) & 0xFFFF) / 4 & 15] = w1;
                }
                break;
            case 0xBA: /* G_SETOTHERMODE_H: G_MDSFT_CYCLETYPE (shift 20, 2 bits) */
                if (((w0 >> 8) & 0xFF) == 20 && (w0 & 0xFF) == 2) {
                    cycle = (w1 >> 20) & 3;
                }
                break;
            case 0xEF: /* G_RDPSETOTHERMODE */
                cycle = (w0 >> 20) & 3;
                break;
            case 0xFF: /* G_SETCIMG */
                cimg = w1;
                cimgW0 = w0;
                break;
            case 0xFE: /* G_SETZIMG */
                zc_note_zimg(w1);
                break;
            case 0xF7: /* G_SETFILLCOLOR */
                fillColor = w1;
                break;
            case 0xF6: /* G_FILLRECT: w0 = lrx, lry (10.2); w1 = ulx, uly */
                /* fill mode (cycle 3) writes the packed colour, lr inclusive; 16-bit images only */
                if (cycle == 3 && ((cimgW0 >> 19) & 3) == 2 && nFills < ZC_MAX_FILLS) {
                    ZcFill *f = &fills[nFills++];
                    f->img = cimg;
                    f->widthM1 = cimgW0 & 0xFFF;
                    f->color = fillColor;
                    f->lrx = (w0 >> 14) & 0x3FF;
                    f->lry = (w0 >> 2) & 0x3FF;
                    f->ulx = (w1 >> 14) & 0x3FF;
                    f->uly = (w1 >> 2) & 0x3FF;
                }
                break;
            default:
                break;
        }
    }
done:
    for (i = 0; i < nFills; i++) {
        const ZcFill *f = &fills[i];
        u32 width = f->widthM1 + 1, x, y, lrx = f->lrx, lry = f->lry;
        unsigned char *img = NULL;
        int k;
        /* the game masks G_SETCIMG / G_SETZIMG to 24 bits (port_fb.h): the full pointer is the one the
         * depth reader passed to port_fb_read_depth */
        for (k = 0; k < rq->nDepth; k++) {
            if (((u32)(size_t)rq->depthImg[k] & 0xFFFFFFu) == (f->img & 0xFFFFFFu)) {
                img = (unsigned char *)rq->depthImg[k];
                break;
            }
        }
        if (img == NULL || !zc_is_zimg(f->img) || width > 1024) {
            continue;
        }
        if (lrx >= width) {
            lrx = width - 1;
        }
        if (lry >= PORT_FB_H) {
            lry = PORT_FB_H - 1;
        }
        if (f->ulx > lrx || f->uly > lry ||
            !mapped(img + ((size_t)f->uly * width + f->ulx) * 2, ((size_t)(lry - f->uly) * width + lrx - f->ulx + 1) * 2)) {
            continue;
        }
        for (y = f->uly; y <= lry; y++) {
            for (x = f->ulx; x <= lrx; x++) {
                /* the 32-bit fill colour packs two 16-bit pixels: the high half on even x; the image is
                 * big-endian in memory */
                u32 v = (x & 1) ? f->color & 0xFFFF : f->color >> 16;
                unsigned char *px = img + ((size_t)y * width + x) * 2;
                px[0] = (unsigned char)(v >> 8);
                px[1] = (unsigned char)v;
            }
        }
    }
}
#endif

/* the task's images (plan 19.10): the low 24 bits of every G_SETCIMG / G_SETZIMG the walk saw */
static unsigned int sTaskImgs[16];
static int sNTaskImgs;

static void collect_images(const DlumpBuf *b) {
    size_t off = sizeof(DlumpHeader);

    sNTaskImgs = 0;
    while (off + sizeof(DlumpChunk) <= b->size) {
        const DlumpChunk *c = (const DlumpChunk *)(b->data + off);
        if (c->kind == DLUMP_END) {
            break;
        }
        if (c->kind == DLUMP_IMAGE && sNTaskImgs < 16) {
            sTaskImgs[sNTaskImgs++] = c->addr & 0xFFFFFFu;
        }
        off += sizeof(DlumpChunk) + ((c->size + 3) & ~3u);
    }
}

/* the task's DLMP image (dlump.h) into buf: the PORT_GFX_DUMP file and the live frame (port_live.c)
 * are the same bytes. Returns dlump_task's error count, -1 when out of memory. */
static int build_task(OSTask_t *task, u32 seq, DlumpBuf *buf) {
    DlumpOptions opt;
    int errs;

    DlumpExtra extra[PORT_FB_MAX_COLOR + PORT_FB_MAX_DEPTH + PORT_FB_MAX_WRITE];
    dlump_u32 depthIdx[PORT_FB_MAX_DEPTH];
    unsigned char *writeData[PORT_FB_MAX_WRITE];
    const PortFbRequests *rq = port_fb_requests();
    size_t nExtra = 0;
    int i, nWriteData = 0;

    memset(&opt, 0, sizeof(opt));
    opt.arenaBase = 0;
#ifdef HAVE_MINCORE
    opt.readable = mapped;
#else
    opt.readable = NULL;
#endif
    /* the framebuffer model (plan 19.10): what the game's CPU reads / wrote, for f3dreplay */
    for (i = 0; i < rq->nColor; i++) {
        DlumpExtra *e = &extra[nExtra++];
        e->kind = DLUMP_FB_READ;
        e->addr = (dlump_u32)(size_t)rq->color[i];
        e->size = 0;
        e->aux = PORT_FB_W;
        e->data = NULL;
    }
    for (i = 0; i < rq->nDepth; i++) {
        /* one chunk per Z image; the requests of one image are adjacent in practice */
        int j, n = 0;
        if (i > 0 && rq->depthImg[i] == rq->depthImg[i - 1]) {
            continue;
        }
        for (j = i; j < rq->nDepth && rq->depthImg[j] == rq->depthImg[i]; j++) {
            depthIdx[i + n++] = (dlump_u32)rq->depthIdx[j];
        }
        extra[nExtra].kind = DLUMP_FB_DEPTH;
        extra[nExtra].addr = (dlump_u32)(size_t)rq->depthImg[i];
        extra[nExtra].size = (dlump_u32)n * 4;
        extra[nExtra].aux = PORT_FB_W;
        extra[nExtra].data = &depthIdx[i];
        nExtra++;
    }
    for (i = 0; i < rq->nWrite; i++) {
        const PortFbWrite *r = &rq->write[i];
        size_t bytes = 8 + (size_t)r->w * r->h * 2;
        unsigned char *d = (unsigned char *)malloc(bytes);
        unsigned short hdr[4];
        int y;
        if (d == NULL || !mapped((const char *)r->img + ((size_t)r->y * PORT_FB_W + r->x) * 2,
                                 ((size_t)(r->h - 1) * PORT_FB_W + r->w) * 2)) {
            free(d);
            continue;
        }
        hdr[0] = (unsigned short)r->x;
        hdr[1] = (unsigned short)r->y;
        hdr[2] = (unsigned short)r->w;
        hdr[3] = (unsigned short)r->h;
        memcpy(d, hdr, 8);
        for (y = 0; y < r->h; y++) {
            memcpy(d + 8 + (size_t)y * r->w * 2, (const char *)r->img + ((size_t)(r->y + y) * PORT_FB_W + r->x) * 2,
                   (size_t)r->w * 2);
        }
        writeData[nWriteData++] = d;
        extra[nExtra].kind = DLUMP_FB_WRITE;
        extra[nExtra].addr = (dlump_u32)(size_t)r->img;
        extra[nExtra].size = (dlump_u32)bytes;
        extra[nExtra].aux = PORT_FB_W;
        extra[nExtra].data = d;
        nExtra++;
    }
    opt.extra = extra;
    opt.nExtra = nExtra;
    errs = dlump_task(buf, &opt, seq, (dlump_u32)(size_t)task, task->type, (dlump_u32)(size_t)task->data_ptr,
                      task->data_size);
    for (i = 0; i < nWriteData; i++) {
        free(writeData[i]);
    }
    if (errs >= 0) {
        collect_images(buf);
    }
    return errs;
}

static void dump_task(OSTask_t *task, u32 seq) {
    DlumpBuf buf = { 0 };
    char path[512];
    FILE *f;
    int errs = build_task(task, seq, &buf);

    if (errs < 0) {
        port_ultra_log("gfx: dump %u: out of memory", seq);
        goto done;
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
done:
    dlump_buf_free(&buf);
}

/* PORT_LIVE (plan 19.11): the same image to the live presenter, one frame per game step */
static u32 sLiveStep;   /* the step of the frame's first task (steps.txt's step) */
static int sLiveTasks;  /* tasks in the frame so far */

static void live_task(OSTask_t *task, u32 seq) {
    static DlumpBuf buf; /* kept between tasks: dlump_task appends, so reuse its capacity */
    if (sLiveTasks++ == 0) {
        sLiveStep = cur_step();
    }
    buf.size = 0;
    if (build_task(task, seq, &buf) < 0) {
        port_ultra_log("gfx: live task %u: out of memory", seq);
        buf.oom = 0;
        return;
    }
    port_live_task(buf.data, buf.size);
}

#ifdef PORT_GFX_NATIVE
/* PORT_NATIVE (plan 21.9): the task to the in-process renderer, which draws the step's tasks at its end;
 * the boundary takes u32s only (port_native.h) and reads the requests in this ILP32 layout */
typedef char port_fb_requests_x32_[sizeof(PortFbRequests) ==
                                       4 * (PORT_FB_MAX_COLOR + 1 + 2 * PORT_FB_MAX_DEPTH + 1 + 5 * PORT_FB_MAX_WRITE + 1)
                                   ? 1 : -1];

static void native_task(OSTask_t *task) {
    if (sLiveTasks++ == 0) {
        sLiveStep = cur_step();
    }
    sNTaskImgs = port_native_task((unsigned)(size_t)task->data_ptr, (unsigned)(size_t)port_fb_requests(),
                                  (unsigned)(size_t)sTaskImgs, 16);
}
#endif

/* main.c, at the end of every step: publish the step's tasks as one live frame */
void port_gfx_step_end(void) {
    if (sLiveTasks != 0) {
        port_live_frame_end(sLiveStep); /* SysLib.frameCount has already advanced here */
        sLiveTasks = 0;
    }
}

/* PORT_TRACE_SCREEN=1 (soak.py, run 869): the step's gfx tasks' content hashes, combined, for the
 * state trace's scr= field (port/trace.c): a screen-level progress signal in the menus */
static int sScreenOn = -1;
static u32 sScreenHash;

int port_gfx_screen_hash(u32 *out) {
    if (sScreenOn <= 0) {
        return 0;
    }
    *out = sScreenHash;
    sScreenHash = 0;
    return 1;
}

void port_gfx_task(OSTask_t *task) {
    u32 seq = sGfxTasks++;

    if (sScreenOn < 0) {
        sScreenOn = (int)env_ul("PORT_TRACE_SCREEN", 0) != 0;
    }
    if (sScreenOn) {
        DlumpOptions so;
        memset(&so, 0, sizeof(so));
#ifdef HAVE_MINCORE
        so.readable = mapped;
#endif
        sScreenHash = sScreenHash * 31u + dlump_screen_hash(&so, (dlump_u32)(size_t)task->data_ptr);
    }

    sNTaskImgs = -1; /* unknown unless the task is dumped or rendered */

    if (!sDump.inited) {
        sDump.inited = 1;
        sDump.dir = getenv("PORT_GFX_DUMP");
        if (sDump.dir != NULL && *sDump.dir == '\0') {
            sDump.dir = NULL;
        }
        sDump.first = env_ul("PORT_GFX_DUMP_FIRST", 0);
        sDump.every = env_ul("PORT_GFX_DUMP_EVERY", 1);
        sDump.count = env_ul("PORT_GFX_DUMP_COUNT", (unsigned long)-1);
        sDumpSteps = getenv("PORT_GFX_DUMP_STEPS");
        if (sDumpSteps != NULL && *sDumpSteps == '\0') {
            sDumpSteps = NULL;
        }
        sFbLog = (int)env_ul("PORT_FB_LOG", 0);
        if (sDump.every == 0) {
            sDump.every = 1;
        }
        if (sDump.dir != NULL) {
            make_dirs(sDump.dir);
        }
    }
    if (sDump.dir != NULL && sDump.written < sDump.count &&
        (sDumpSteps != NULL ? step_listed(cur_step())
                            : seq >= sDump.first && (seq - sDump.first) % sDump.every == 0)) {
        char path[512];
        FILE *f;
        dump_task(task, seq);
        snprintf(path, sizeof(path), "%s/steps.txt", sDump.dir);
        if ((f = fopen(path, "ab")) != NULL) {
            fprintf(f, "%06u %u\n", (unsigned)seq, (unsigned)cur_step());
            fclose(f);
        }
    }
    if (port_live_active()) {
#ifdef PORT_GFX_NATIVE
        if (port_live_native()) {
            native_task(task);
        } else
#endif
        live_task(task, seq);
    }
#ifdef PORT_GFX_FAST3D
    /* option (a): render in process (the dump above, if any, saw the same lists), then serve the
     * framebuffer model's read-backs / CPU writes (plan 19.10) */
    port_fast3d_task_fb((u32)(size_t)task->data_ptr, port_fb_requests());
    port_fast3d_last_images(&sTaskImgs[0], &sTaskImgs[1]);
    sNTaskImgs = 2;
#endif
    /* option (b), the default here: the dump goes to fast3d/f3dreplay, the DLUMP_FB_* chunks with it.
     * The game itself gets the depth-clear model (above) for its depth reads, and with PORT_LIVE the
     * presenter's answers over it (port_live_fb_apply). */
#ifndef PORT_GFX_FAST3D
    {
        static int zclear = -1;
        if (zclear < 0) {
            zclear = (int)env_ul("PORT_FB_ZCLEAR", 1);
        }
        if (zclear && port_fb_requests()->nDepth > 0) {
            const PortFbRequests *rq = port_fb_requests();
            depth_clear_model((u32)(size_t)task->data_ptr, rq);
        }
        /* PORT_LIVE (run 859): the presenter's real depth / colour over the clear, where it answered */
        port_live_fb_apply(port_fb_requests());
    }
#endif
    port_fb_task_done(sTaskImgs, sNTaskImgs);
    if (sFbLog && seq % 100 == 0) {
        const PortFbStats *st = port_fb_stats();
        port_ultra_log("gfx: task %u: fb requests so far: %lu colour reads, %lu depth pixels, %lu CPU writes, "
                       "%lu texture writes", seq, st->colorReads, st->depthReads, st->cpuWrites, st->textureWrites);
    }
}
