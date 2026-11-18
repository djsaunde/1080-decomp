/* port/gfx: the game side of the live presenter channel (PC port plan 19.11).
 *
 *   PORT_LIVE=<name>   attach to /dev/shm/<name> (created by the presenter,
 *                      port/gfx/fast3d/presenter.cpp; port/tools/play.sh starts both)
 *
 * Each gfx task's DLMP image (port_gfx.c builds it exactly as PORT_GFX_DUMP would) is
 * appended to the current frame; main.c's step end publishes the frame into the ring
 * (live_shm.h). The pads the presenter writes replace the headless host's pads
 * (host_headless.c asks port_live_read_pads), and closing the window ends the game
 * (port_live_quit -> host_poll_events). The game's AI output goes into the audio ring
 * (port_live_audio_write, from port/audio/port_audio.c's sink; plan 19.12). The layout is
 * in live_shm.h.
 *
 * Framebuffer replies (run 859): the presenter answers the frame's depth / colour read requests in
 * /dev/shm/<name>-fb (LiveFbReply); port_gfx.c calls port_live_fb_apply after its depth-clear model
 * at each gfx task, which stores the newest answers into the images the game asked about. With no
 * reply file (an older presenter) or no PORT_LIVE nothing changes: the headless run (the gate) never
 * reaches this code.
 *
 * No game headers: only libc and the POSIX calls declared below (as port_gfx.c declares
 * mincore), so the file also compiles in the PORT_LIBC_DECLS_ONLY configuration. */

#include "../port_libc.h"
#include "../host.h"
#include "live_shm.h"
#include "port_live.h"
#include "port_fb_impl.h"
#ifdef PORT_GFX_NATIVE
#include "port_native.h" /* plan 21.9: the native 64-bit game's in-process host */
#endif

#ifdef PORT_LIBC_DECLS_ONLY
void *realloc(void *p, size_t n);
#endif
int open(const char *path, int flags, ...);
int close(int fd);
void *mmap(void *addr, size_t len, int prot, int flags, int fd, long off);
int getpid(void);
int kill(int pid, int sig);
int unlink(const char *path);
#define LIVE_O_RDWR 2
#define LIVE_PROT_RW 3
#define LIVE_MAP_SHARED 1
#define LIVE_MAP_FAILED ((void *)-1)

/* stderr directly, not host_log: test_port_gfx links port_gfx.c + this file without the host */
#define live_log(...) (fprintf(stderr, "live: " __VA_ARGS__), fputc('\n', stderr))
#define live_fatal(...) (live_log(__VA_ARGS__), exit(1))

static int sInited;
static LiveShmHeader *sShm;
static unsigned char *sSlots;
static unsigned char *sFrame;     /* the frame being built: a private copy of a slot */
static size_t sFrameBytes, sFrameCap;
static unsigned sFrameTasks;
static unsigned sFrames, sPublished;
/* the framebuffer replies (live_shm.h, run 859) */
static LiveFbReply *sFb;
static live_u32 sFbSeq = 1; /* the reply last taken (odd: none yet) */
static LiveFbDepth sFbDepth[LIVE_FB_DEPTH];
static unsigned sFbNDepth;
static unsigned sFbTaken, sFbTorn, sFbDepthStored, sFbColorStored;
static unsigned short sFbPixels[LIVE_FB_W * LIVE_FB_H];
static LiveAudio *sAudio; /* NULL: the presenter has no audio ring */
static unsigned char *sAudioData;
static unsigned long long sAudioHash = 0xcbf29ce484222325ull; /* FNV-1a 64 of every byte written */
static unsigned long long sAudioBytes;
static live_u32 sPadReads;     /* scripted input: the pad reads asked for (live_shm.h padWant) */
static unsigned sQuitPolls;    /* port_live_quit calls, for the presenter liveness check */
static int sPresenterGone;
static char sShmPath[256]; /* /dev/shm/<name>: removed here when the presenter died without doing it */
static int sNative;        /* plan 21.9: the in-process host's block, not a presenter's files */
static int sBlack;         /* osViBlack (port_live_black): LIVE_FRAME_BLACK on the frames published */
/* depth reads now (plan 22.10, live_shm.h LiveDepthAsk): the presenter's last answer, kept for the
 * step's other reads in its window */
static LiveDepthAsk *sDepth;
static struct {
    int valid, ok;
    live_u32 frames, img, x0, y0, w, h;
    unsigned short z[LIVE_DEPTH_SIDE * LIVE_DEPTH_SIDE];
} sDepthCache;
static unsigned sDepthAsks, sDepthHits, sDepthTimeouts, sDepthOther, sDepthPolls, sDepthMaxPolls;
static int live_presenter_alive(void);

static void live_sleep_ms(int ms) {
    struct timespec ts;
    ts.tv_sec = ms / 1000;
    ts.tv_nsec = (long)(ms % 1000) * 1000000L;
    nanosleep(&ts, NULL);
}

static void live_open(void) {
    const char *name = getenv("PORT_LIVE");
    char path[256];
    int fd = -1, tries;
    LiveShmHeader *h = (LiveShmHeader *)LIVE_MAP_FAILED;
    size_t total;

    sInited = 1;
#ifdef PORT_GFX_NATIVE
    /* PORT_NATIVE=1 (plan 21.9): the same structures, in a low block of this process that
     * fast3d/native_host.cpp fills; frames go to port_native_frame instead of the ring */
    {
        const char *nat = getenv("PORT_NATIVE");
        if (nat != NULL && *nat != '\0' && strcmp(nat, "0") != 0) {
            unsigned base = port_native_open();
            if (base == 0) {
                live_fatal("PORT_NATIVE: no window (the reason is above)");
            }
            sNative = 1;
            sShm = (LiveShmHeader *)(size_t)base;
            sAudio = (LiveAudio *)((unsigned char *)sShm + LIVE_AUDIO_HDR);
            sAudioData = (unsigned char *)sShm + sAudio->dataOffset;
            sFb = (LiveFbReply *)((unsigned char *)sShm + PORT_NATIVE_FB_REPLY);
            live_log("native: in-process window, audio and read-backs");
            return;
        }
    }
#endif
    if (name == NULL || *name == '\0') {
        return;
    }
    snprintf(path, sizeof(path), "/dev/shm/%s", name);
    /* the presenter creates the file and writes the magic last: wait for it (10 s) */
    for (tries = 0; tries < 1000; tries++) {
        if (fd < 0) {
            fd = open(path, LIVE_O_RDWR);
        }
        if (fd >= 0 && h == (LiveShmHeader *)LIVE_MAP_FAILED) {
            h = (LiveShmHeader *)mmap(NULL, LIVE_HEADER_BYTES, LIVE_PROT_RW, LIVE_MAP_SHARED, fd, 0);
        }
        if (h != (LiveShmHeader *)LIVE_MAP_FAILED && __atomic_load_n(&h->magic, __ATOMIC_ACQUIRE) == LIVE_MAGIC) {
            break;
        }
        live_sleep_ms(10);
    }
    if (fd < 0 || tries == 1000) {
        live_fatal("no presenter at %s (start it first; port/tools/play.sh does)", path);
    }
    if (h->version != LIVE_VERSION || h->slotCount == 0 || h->slotBytes < 4096) {
        live_fatal("%s: version %u / %u slots of %u bytes not supported", path, h->version, h->slotCount,
                   h->slotBytes);
    }
    total = (size_t)LIVE_HEADER_BYTES + (size_t)h->slotCount * h->slotBytes;
    {
        const LiveAudio *a = (const LiveAudio *)((const unsigned char *)h + LIVE_AUDIO_HDR);
        if (a->magic == LIVE_AUDIO_MAGIC && a->bytes != 0 && (a->bytes & (a->bytes - 1)) == 0 &&
            a->dataOffset >= LIVE_HEADER_BYTES && (size_t)a->dataOffset + a->bytes > total) {
            total = (size_t)a->dataOffset + a->bytes;
        }
    }
    sShm = (LiveShmHeader *)mmap(NULL, total, LIVE_PROT_RW, LIVE_MAP_SHARED, fd, 0);
    close(fd);
    if ((void *)sShm == LIVE_MAP_FAILED) {
        live_fatal("cannot map %s (%lu bytes)", path, (unsigned long)total);
    }
    sSlots = (unsigned char *)sShm + LIVE_HEADER_BYTES;
    sShm->gamePid = (live_u32)getpid();
    snprintf(sShmPath, sizeof(sShmPath), "%s", path);
    /* a game started again under a presenter that is still open (--stay): this one has not ended */
    __atomic_store_n(&sShm->gameExited, 0, __ATOMIC_RELEASE);
    sAudio = (LiveAudio *)((unsigned char *)sShm + LIVE_AUDIO_HDR);
    if (sAudio->magic != LIVE_AUDIO_MAGIC || sAudio->bytes == 0 || (sAudio->bytes & (sAudio->bytes - 1)) != 0 ||
        sAudio->dataOffset < LIVE_HEADER_BYTES || (size_t)sAudio->dataOffset + sAudio->bytes > total) {
        sAudio = NULL;
    } else {
        sAudioData = (unsigned char *)sShm + sAudio->dataOffset;
    }
    sDepth = (LiveDepthAsk *)((unsigned char *)sShm + LIVE_DEPTH_HDR);
    if (sDepth->magic != LIVE_DEPTH_MAGIC) {
        sDepth = NULL; /* an older presenter: the asynchronous answers only */
    }
    live_log("attached to %s: %u slots of %u MB, %s%s", path, sShm->slotCount, sShm->slotBytes >> 20,
             sAudio != NULL ? "audio ring" : "no audio",
             sShm->padScripted ? ", scripted pads (lockstep)" : "");
    /* the reply file: optional (created by the presenter before the magic) */
    snprintf(path, sizeof(path), "/dev/shm/%s-fb", name);
    fd = open(path, LIVE_O_RDWR);
    if (fd >= 0) {
        LiveFbReply *r = (LiveFbReply *)mmap(NULL, sizeof(LiveFbReply), LIVE_PROT_RW, LIVE_MAP_SHARED, fd, 0);
        close(fd);
        if ((void *)r != LIVE_MAP_FAILED && r->magic == LIVE_FB_MAGIC && r->version == LIVE_FB_VERSION) {
            sFb = r;
            live_log("framebuffer replies from %s", path);
        }
    }
    if (sFb == NULL) {
        live_log("no framebuffer replies (%s): depth reads keep the clear model", path);
    }
}

int port_live_active(void) {
    if (!sInited) {
        live_open();
    }
    return sShm != NULL;
}

int port_live_native(void) {
    return port_live_active() && sNative;
}

void port_live_task(const void *dlmp, size_t size) {
    if (!port_live_active()) {
        return;
    }
    if (sFrameBytes == 0) {
        sFrameBytes = sizeof(LiveFrame);
        sFrameTasks = 0;
    }
    if (sFrameBytes + size > sFrameCap) {
        size_t cap = sFrameCap != 0 ? sFrameCap : (4u << 20);
        unsigned char *p;
        while (cap < sFrameBytes + size) {
            cap *= 2;
        }
        p = (unsigned char *)realloc(sFrame, cap);
        if (p == NULL) {
            live_log("out of memory for a %lu-byte frame", (unsigned long)(sFrameBytes + size));
            return;
        }
        sFrame = p;
        sFrameCap = cap;
    }
    memcpy(sFrame + sFrameBytes, dlmp, size);
    sFrameBytes += size;
    sFrameTasks++;
}

void port_live_frame_end(unsigned step) {
    LiveFrame *f;
    live_u32 head, tail;

#ifdef PORT_GFX_NATIVE
    if (port_live_native()) {
        sFrames++;
        sPublished++;
        port_native_frame(step);
        return;
    }
#endif
    if (!port_live_active() || sFrameBytes == 0) {
        return;
    }
    sFrames++;
    f = (LiveFrame *)sFrame;
    f->bytes = (live_u32)sFrameBytes;
    f->nTasks = sFrameTasks;
    f->step = step;
    head = sShm->head;
    tail = __atomic_load_n(&sShm->tail, __ATOMIC_ACQUIRE);
    f->seq = head;
    f->flags = sBlack ? LIVE_FRAME_BLACK : 0;
    if (sFrameBytes > sShm->slotBytes) {
        sShm->tooBig++;
        if (sShm->tooBig < 10) {
            live_log("step %u: a %lu-byte frame does not fit a %u-byte slot (dropped)", step,
                     (unsigned long)sFrameBytes, sShm->slotBytes);
        }
    } else if (head - tail >= sShm->slotCount) {
        sShm->dropped++; /* the presenter is behind: keep real time, drop the frame */
    } else {
        memcpy(sSlots + (size_t)(head % sShm->slotCount) * sShm->slotBytes, sFrame, sFrameBytes);
        sShm->gameStep = step;
        __atomic_store_n(&sShm->head, head + 1, __ATOMIC_RELEASE);
        sPublished++;
    }
    sFrameBytes = 0;
    sFrameTasks = 0;
}

/* the newest reply, if a new one is complete: its depth answers are kept (they are stored again after
 * every task's depth clear); a colour answer is stored at once into a requested image */
static void live_fb_take(const PortFbRequests *rq) {
    live_u32 s1 = __atomic_load_n(&sFb->seq, __ATOMIC_ACQUIRE), n, i;
    LiveFbDepth depth[LIVE_FB_DEPTH];
    int k;

    if ((s1 & 1) != 0 || s1 == sFbSeq) {
        return;
    }
    n = sFb->nDepth < LIVE_FB_DEPTH ? sFb->nDepth : LIVE_FB_DEPTH;
    memcpy(depth, (const void *)sFb->depth, n * sizeof(LiveFbDepth));
    __atomic_thread_fence(__ATOMIC_ACQUIRE);
    if (__atomic_load_n(&sFb->seq, __ATOMIC_ACQUIRE) != s1) {
        sFbTorn++;
        return;
    }
    memcpy(sFbDepth, depth, n * sizeof(LiveFbDepth));
    sFbNDepth = n;
    sFbSeq = s1;
    sFbTaken++;
    /* colour: the frame, into the image the game reads, only if it still asks for it */
    n = sFb->nColor < LIVE_FB_COLOR ? sFb->nColor : LIVE_FB_COLOR;
    for (i = 0; i < n; i++) {
        live_u32 img = sFb->color[i].img;
        for (k = 0; k < rq->nColor; k++) {
            if ((live_u32)(size_t)rq->color[k] == img) {
                break;
            }
        }
        if (k == rq->nColor) {
            continue;
        }
        memcpy(sFbPixels, (const void *)sFb->pixels[i], sizeof(sFbPixels));
        __atomic_thread_fence(__ATOMIC_ACQUIRE);
        if (__atomic_load_n(&sFb->seq, __ATOMIC_ACQUIRE) != s1) {
            sFbTorn++;
            return;
        }
        memcpy((void *)rq->color[k], sFbPixels, sizeof(sFbPixels));
        sFbColorStored++;
    }
}

void port_live_fb_apply(const PortFbRequests *rq) {
    unsigned i;
    int k;

    if (!port_live_active() || sFb == NULL || rq == NULL) {
        return;
    }
    live_fb_take(rq);
    /* every answered pixel (the asked ones and their window, live_shm.h LIVE_FB_DEPTH_R), not only
     * the ones asked again: the reply is a step or two old, and the clear model has just reset the
     * whole image, so the pixels the game reads next (a moving sun) must hold the answers too */
    for (i = 0; i < sFbNDepth; i++) {
        const LiveFbDepth *d = &sFbDepth[i];
        for (k = 0; k < rq->nDepth; k++) {
            if ((live_u32)(size_t)rq->depthImg[k] == d->img) {
                unsigned char *px = (unsigned char *)rq->depthImg[k] + (size_t)d->idx * 2;
                px[0] = (unsigned char)(d->z >> 8); /* big-endian, as the RDP writes it */
                px[1] = (unsigned char)d->z;
                sFbDepthStored++;
                break;
            }
        }
    }
}

void port_live_black(int black) {
    if (!port_live_active()) {
        return;
    }
    sBlack = black != 0;
#ifdef PORT_GFX_NATIVE
    if (sNative) {
        port_native_black(black);
    }
#endif
}

/* plan 22.10: the presenter's depth now. One request per step and Z image: the presenter answers the
 * pixel's window from the last frame it drew (the frame this game published last, rendered first if
 * it was not yet), and the other reads of the step (a sun's 9 pixels) come from that window. */
int port_live_depth(const void *zimg, int idx) {
    live_u32 img = (live_u32)(size_t)zimg, frames, ask, x, y;
    unsigned n;

    if (!port_live_active() || idx < 0 || idx >= LIVE_FB_W * LIVE_FB_H) {
        return -1;
    }
#ifdef PORT_GFX_NATIVE
    if (sNative) {
        return port_native_depth(img, (unsigned)idx); /* in process: at once (plan 22.4) */
    }
#endif
    if (sDepth == NULL || sPresenterGone) {
        return -1;
    }
    frames = __atomic_load_n(&sShm->head, __ATOMIC_ACQUIRE);
    x = (live_u32)idx % LIVE_FB_W;
    y = (live_u32)idx / LIVE_FB_W;
    if (!(sDepthCache.valid && sDepthCache.frames == frames && sDepthCache.img == img && x >= sDepthCache.x0 &&
          x < sDepthCache.x0 + sDepthCache.w && y >= sDepthCache.y0 && y < sDepthCache.y0 + sDepthCache.h)) {
        if (sDepthCache.valid && sDepthCache.frames == frames && sDepthCache.img == img && !sDepthCache.ok) {
            return -1; /* the presenter has said this frame has no such Z image, or did not answer */
        }
        sDepth->img = img;
        sDepth->idx = (live_u32)idx;
        sDepth->frames = frames;
        ask = sDepth->ask + 1;
        __atomic_store_n(&sDepth->ask, ask, __ATOMIC_RELEASE);
        sDepthAsks++;
        /* the presenter renders the frame first if it is behind: wait up to ~0.5 s, then keep the
         * asynchronous answers (a stalled or slow presenter must not stop the game) */
        for (n = 1; __atomic_load_n(&sDepth->done, __ATOMIC_ACQUIRE) != ask; n++) {
            if (n > 5000 || __atomic_load_n(&sShm->quit, __ATOMIC_ACQUIRE) ||
                (n % 2000 == 0 && !live_presenter_alive())) {
                sDepthTimeouts++;
                if (sDepthTimeouts <= 5) {
                    live_log("depth read: no answer from the presenter in time (frame %u)", frames);
                }
                sDepthCache.valid = 1;
                sDepthCache.ok = 0;
                sDepthCache.frames = frames;
                sDepthCache.img = img;
                return -1;
            }
            {
                struct timespec ts = { 0, 100000 }; /* 0.1 ms */
                nanosleep(&ts, NULL);
            }
        }
        sDepthPolls += n - 1;
        if (n - 1 > sDepthMaxPolls) {
            sDepthMaxPolls = n - 1;
        }
        sDepthCache.valid = 1;
        sDepthCache.ok = sDepth->ok != 0;
        sDepthCache.frames = frames;
        sDepthCache.img = img;
        sDepthCache.x0 = sDepth->x0;
        sDepthCache.y0 = sDepth->y0;
        sDepthCache.w = sDepth->w;
        sDepthCache.h = sDepth->h;
        if (sDepthCache.w * sDepthCache.h > LIVE_DEPTH_SIDE * LIVE_DEPTH_SIDE) {
            sDepthCache.ok = 0;
        }
        if (sDepthCache.ok) {
            memcpy(sDepthCache.z, (const void *)sDepth->z, sDepthCache.w * sDepthCache.h * sizeof(sDepthCache.z[0]));
        } else {
            sDepthOther++;
            return -1;
        }
        if (!(x >= sDepthCache.x0 && x < sDepthCache.x0 + sDepthCache.w && y >= sDepthCache.y0 &&
              y < sDepthCache.y0 + sDepthCache.h)) {
            return -1;
        }
    } else {
        sDepthHits++;
    }
    return sDepthCache.z[(y - sDepthCache.y0) * sDepthCache.w + (x - sDepthCache.x0)];
}

int port_live_audio_active(void) {
    return port_live_active() && sAudio != NULL;
}

/* one AI buffer (32000 Hz stereo s16) into the audio ring: all of it or, when the presenter is
 * that far behind, none (an overrun) */
void port_live_audio_write(const void *samples, size_t bytes) {
    live_u32 head, tail, size, off, first;
    const unsigned char *p = (const unsigned char *)samples;
    size_t i;
    if (!port_live_audio_active() || bytes == 0) {
        return;
    }
    size = sAudio->bytes;
    head = sAudio->head;
    tail = __atomic_load_n(&sAudio->tail, __ATOMIC_ACQUIRE);
    if (bytes > size - (head - tail)) {
        sAudio->overruns++;
        return;
    }
    off = head & (size - 1);
    first = size - off;
    if (first > bytes) {
        first = (live_u32)bytes;
    }
    memcpy(sAudioData + off, p, first);
    memcpy(sAudioData, p + first, bytes - first);
    for (i = 0; i < bytes; i++) {
        sAudioHash = (sAudioHash ^ p[i]) * 0x100000001b3ull;
    }
    sAudioBytes += bytes;
    __atomic_store_n(&sAudio->head, head + (live_u32)bytes, __ATOMIC_RELEASE);
}

/* the presenter process still exists (checked about every 0.2 s by the callers) */
static int live_presenter_alive(void) {
    live_u32 pid = sShm->presenterPid;
    if (sPresenterGone) {
        return 0;
    }
    if (pid != 0 && kill((int)pid, 0) != 0) {
        char fb[sizeof(sShmPath) + 4];
        live_log("the presenter (pid %u) is gone: ending the game", pid);
        sPresenterGone = 1;
        /* its files (the mappings stay valid until the game exits) */
        snprintf(fb, sizeof(fb), "%s-fb", sShmPath);
        unlink(sShmPath);
        unlink(fb);
        return 0;
    }
    return 1;
}

int port_live_read_pads(HostPad pads[HOST_MAX_PADS]) {
    int i;
    if (!port_live_active()) {
        return 0;
    }
    if (__atomic_load_n(&sShm->padScripted, __ATOMIC_ACQUIRE) && !sPresenterGone) {
        /* lockstep (live_shm.h): ask for this read's states and wait for them */
        unsigned n;
        live_u32 want = ++sPadReads;
        __atomic_store_n(&sShm->padWant, want, __ATOMIC_RELEASE);
        for (n = 1; __atomic_load_n(&sShm->padStep, __ATOMIC_ACQUIRE) != want; n++) {
            if (__atomic_load_n(&sShm->quit, __ATOMIC_ACQUIRE) || (n % 2000 == 0 && !live_presenter_alive())) {
                break;
            }
            {
                struct timespec ts = { 0, 100000 }; /* 0.1 ms */
                nanosleep(&ts, NULL);
            }
        }
    }
    __atomic_thread_fence(__ATOMIC_ACQUIRE);
    for (i = 0; i < HOST_MAX_PADS && i < LIVE_PADS; i++) {
        const volatile LivePad *p = &sShm->pads[i];
        pads[i].button = p->button;
        pads[i].stick_x = p->stick_x;
        pads[i].stick_y = p->stick_y;
        pads[i].connected = p->connected;
        pads[i].hasRumble = p->hasRumble;
    }
    return 1;
}

int port_live_quit(void) {
    if (!port_live_active()) {
        return 0;
    }
#ifdef PORT_GFX_NATIVE
    if (sNative) {
        return port_native_poll(); /* the window's events, the pads, Esc / close */
    }
#endif
    if (__atomic_load_n(&sShm->quit, __ATOMIC_ACQUIRE) != 0) {
        return 1;
    }
    /* a presenter that died without saying so (killed, crashed, its X display gone): end the game
     * instead of rendering into a ring nobody reads (called once per host poll, ~30 / s) */
    return (++sQuitPolls % 6 == 0 || sPresenterGone) && !live_presenter_alive();
}

void port_live_close(void) {
    if (sShm != NULL) {
        __atomic_store_n(&sShm->gameExited, 1, __ATOMIC_RELEASE);
        live_log("%u frames, %u published, %u dropped (ring full), %u too big", sFrames, sPublished,
                 sShm->dropped, sShm->tooBig);
        if (sFb != NULL) {
            live_log("framebuffer replies: %u taken (%u torn), %u depth pixels and %u colour images stored",
                     sFbTaken, sFbTorn, sFbDepthStored, sFbColorStored);
        }
        if (sDepth != NULL) {
            live_log("depth reads now: %u asked (%u waits of 0.1 ms, at most %u), %u from the window, %u not in "
                     "the frame, %u timed out", sDepthAsks, sDepthPolls, sDepthMaxPolls, sDepthHits, sDepthOther,
                     sDepthTimeouts);
        }
        if (sAudio != NULL) {
            live_log("audio: %llu bytes (%.2f s) sent, fnv1a64 %016llx, %u overruns", sAudioBytes,
                     sAudioBytes / (4.0 * 32000.0), sAudioHash, sAudio->overruns);
        }
#ifdef PORT_GFX_NATIVE
        if (sNative) {
            port_native_close();
        }
#endif
    }
}
