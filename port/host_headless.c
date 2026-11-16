/* Headless host implementation (port/host.h): no window, no audio device.
 *
 * - Video: remembers the mode and counts frames; host_present() is a no-op.
 * - Input: returns whatever host_headless_set_pads() last stored (default:
 *   controller 1 connected, nothing pressed, no rumble pak). The trace
 *   injector in main.c overrides input one level higher, in pad_update.
 * - Audio: accepts every buffer and reports it as played at once
 *   (host_audio_buffered_bytes() == 0), which keeps the audio driver's
 *   sample-count arithmetic deterministic.
 * - Clock: CLOCK_MONOTONIC, used only for real-time pacing. */

#include "port_libc.h"
#include "host.h"

static HostPad sPads[HOST_MAX_PADS] = { { 0, 0, 0, 1, 0 } };
static host_u32 sAudioRate;
static host_u32 sFramesReady;
static void *sLastFrame;

int host_init(const HostConfig *cfg) {
    (void)cfg;
    return 0;
}

void host_shutdown(void) {
    fflush(stdout);
    fflush(stderr);
}

int host_poll_events(void) {
    return 1;
}

void host_read_pads(HostPad pads[HOST_MAX_PADS]) {
    memcpy(pads, sPads, sizeof(sPads));
}

void host_headless_set_pads(const HostPad pads[HOST_MAX_PADS]) {
    memcpy(sPads, pads, sizeof(sPads));
}

void host_set_rumble(int channel, int on) {
    (void)channel;
    (void)on;
}

void host_video_mode(host_u32 width, host_u32 height, int interlaced) {
    host_log("host: video mode %ux%u%s", width, height, interlaced ? " interlaced" : "");
}

void host_video_black(int black) {
    (void)black;
}

void host_frame_ready(void *framebuffer) {
    sLastFrame = framebuffer;
    sFramesReady++;
}

void host_present(void) {
}

host_u32 host_audio_set_frequency(host_u32 hz) {
    sAudioRate = hz;
    return hz;
}

void host_audio_queue(const void *samples, host_u32 bytes) {
    (void)samples;
    (void)bytes;
}

host_u32 host_audio_buffered_bytes(void) {
    return 0;
}

double host_time_seconds(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + (double)ts.tv_nsec * 1e-9;
}

void host_sleep_seconds(double s) {
    struct timespec ts;
    if (s <= 0.0) {
        return;
    }
    ts.tv_sec = (time_t)s;
    ts.tv_nsec = (long)((s - (double)ts.tv_sec) * 1e9);
    nanosleep(&ts, NULL);
}

int host_file_read_all(const char *path, void **data, host_u32 *size) {
    FILE *f = fopen(path, "rb");
    long n;
    void *buf;

    *data = NULL;
    *size = 0;
    if (f == NULL) {
        return -1;
    }
    if (fseek(f, 0, SEEK_END) != 0 || (n = ftell(f)) < 0 || fseek(f, 0, SEEK_SET) != 0) {
        fclose(f);
        return -1;
    }
    buf = malloc(n > 0 ? (size_t)n : 1);
    if (buf == NULL || fread(buf, 1, (size_t)n, f) != (size_t)n) {
        free(buf);
        fclose(f);
        return -1;
    }
    fclose(f);
    *data = buf;
    *size = (host_u32)n;
    return 0;
}

int host_file_write_all(const char *path, const void *data, host_u32 size) {
    FILE *f = fopen(path, "wb");
    int ok;

    if (f == NULL) {
        return -1;
    }
    ok = fwrite(data, 1, size, f) == size;
    if (fclose(f) != 0) {
        ok = 0;
    }
    return ok ? 0 : -1;
}

void host_free(void *p) {
    free(p);
}

void host_vlog(const char *fmt, va_list ap) {
    char buf[1024];
    int n = vsnprintf(buf, sizeof(buf), fmt, ap);

    if (n < 0) {
        return;
    }
    if ((size_t)n >= sizeof(buf)) {
        n = sizeof(buf) - 1;
    }
    if (n > 0 && buf[n - 1] == '\n') {
        buf[--n] = '\0';
    }
    fprintf(stderr, "%s\n", buf);
}

void host_log(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    host_vlog(fmt, ap);
    va_end(ap);
}

void host_fatal(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    host_vlog(fmt, ap);
    va_end(ap);
    host_shutdown();
    exit(1);
}

/* keep the frame bookkeeping visible to a debugger */
host_u32 host_headless_frames_ready(void) {
    (void)sLastFrame;
    (void)sAudioRate;
    return sFramesReady;
}
