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

#ifdef PORT_LIBC_DECLS_ONLY
void *realloc(void *p, size_t n);
#endif
int open(const char *path, int flags, ...);
int close(int fd);
void *mmap(void *addr, size_t len, int prot, int flags, int fd, long off);
int getpid(void);
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
static LiveAudio *sAudio; /* NULL: the presenter has no audio ring */
static unsigned char *sAudioData;
static unsigned long long sAudioHash = 0xcbf29ce484222325ull; /* FNV-1a 64 of every byte written */
static unsigned long long sAudioBytes;

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
    sAudio = (LiveAudio *)((unsigned char *)sShm + LIVE_AUDIO_HDR);
    if (sAudio->magic != LIVE_AUDIO_MAGIC || sAudio->bytes == 0 || (sAudio->bytes & (sAudio->bytes - 1)) != 0 ||
        sAudio->dataOffset < LIVE_HEADER_BYTES || (size_t)sAudio->dataOffset + sAudio->bytes > total) {
        sAudio = NULL;
    } else {
        sAudioData = (unsigned char *)sShm + sAudio->dataOffset;
    }
    live_log("attached to %s: %u slots of %u MB, %s", path, sShm->slotCount, sShm->slotBytes >> 20,
             sAudio != NULL ? "audio ring" : "no audio");
}

int port_live_active(void) {
    if (!sInited) {
        live_open();
    }
    return sShm != NULL;
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

int port_live_read_pads(HostPad pads[HOST_MAX_PADS]) {
    int i;
    if (!port_live_active()) {
        return 0;
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
    return port_live_active() && __atomic_load_n(&sShm->quit, __ATOMIC_ACQUIRE) != 0;
}

void port_live_close(void) {
    if (sShm != NULL) {
        __atomic_store_n(&sShm->gameExited, 1, __ATOMIC_RELEASE);
        live_log("%u frames, %u published, %u dropped (ring full), %u too big", sFrames, sPublished,
                 sShm->dropped, sShm->tooBig);
        if (sAudio != NULL) {
            live_log("audio: %llu bytes (%.2f s) sent, fnv1a64 %016llx, %u overruns", sAudioBytes,
                     sAudioBytes / (4.0 * 32000.0), sAudioHash, sAudio->overruns);
        }
    }
}
