/* Headless host implementation (port/host.h): no window, no audio device.
 *
 * - Video: remembers the mode and counts frames; host_present() is a no-op.
 * - Input: the --input script's pads for the step (port/host/input_script.c), or
 *   whatever host_headless_set_pads() last stored (default: controller 1
 *   connected, nothing pressed, no rumble pak). The trace
 *   injector in main.c overrides input one level higher, in pad_update.
 * - Audio: accepts every buffer and reports it as played at once
 *   (host_audio_buffered_bytes() == 0), which keeps the audio driver's
 *   sample-count arithmetic deterministic.
 * - Clock: CLOCK_MONOTONIC, used only for real-time pacing. */

#include "port_libc.h"
#include "host.h"

/* PORT_LIVE (port/gfx/port_live.c, plan 19.11): the live presenter's keyboard / gamepad */
extern int port_live_read_pads(HostPad pads[HOST_MAX_PADS]) __attribute__((weak));
extern void port_live_black(int black) __attribute__((weak));
extern int port_live_quit(void) __attribute__((weak));
extern int port_live_idle(double seconds) __attribute__((weak)); /* PORT_FPS (plan 27.3) */

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
    return !(port_live_quit != NULL && port_live_quit());
}

/* weak: test_ultra links this file without the script reader */
extern int host_input_script_active(void) __attribute__((weak));
extern void host_input_script_step(HostPad pads[HOST_MAX_PADS]) __attribute__((weak));
extern void host_input_script_boot(HostPad pads[HOST_MAX_PADS]) __attribute__((weak));

void host_boot_pads(HostPad pads[HOST_MAX_PADS]) {
    if (host_input_script_active != NULL && host_input_script_active()) {
        host_input_script_boot(pads);
        return;
    }
    memcpy(pads, sPads, sizeof(sPads));
}

void host_read_pads(HostPad pads[HOST_MAX_PADS]) {
    if (host_input_script_active != NULL && host_input_script_active()) {
        host_input_script_step(pads);
        return;
    }
    if (port_live_read_pads != NULL && port_live_read_pads(pads)) {
        return;
    }
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
    /* plan 22.5 / 22.10: the native window and the presenter blank as the VI does (f3dreplay does not) */
    if (port_live_black != NULL) {
        port_live_black(black);
    }
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

static void (*sAudioSink)(const void *samples, host_u32 bytes);

void host_headless_set_audio_sink(void (*sink)(const void *samples, host_u32 bytes)) {
    sAudioSink = sink;
}

void host_audio_queue(const void *samples, host_u32 bytes) {
    if (sAudioSink != NULL) {
        sAudioSink(samples, bytes);
    }
}

static host_u32 (*sAudioBuffered)(void);

void host_headless_set_audio_clock(host_u32 (*buffered)(void)) {
    sAudioBuffered = buffered;
}

host_u32 host_audio_buffered_bytes(void) {
    return sAudioBuffered != NULL ? sAudioBuffered() : 0;
}

#ifdef PORT_WIN32
/* the Windows build (plan 29): QueryPerformanceCounter, and Sleep at the 1 ms timer period main.c sets */
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
double host_time_seconds(void) {
    static LARGE_INTEGER freq;
    LARGE_INTEGER t;
    if (freq.QuadPart == 0) {
        QueryPerformanceFrequency(&freq);
    }
    QueryPerformanceCounter(&t);
    return (double)t.QuadPart / (double)freq.QuadPart;
}

void host_sleep_seconds(double s) {
    if (s <= 0.0 || (port_live_idle != NULL && port_live_idle(s))) {
        return;
    }
    Sleep((DWORD)(s * 1000.0));
}
#else
double host_time_seconds(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + (double)ts.tv_nsec * 1e-9;
}

void host_sleep_seconds(double s) {
    struct timespec ts;
    if (s <= 0.0 || (port_live_idle != NULL && port_live_idle(s))) {
        return;
    }
    ts.tv_sec = (time_t)s;
    ts.tv_nsec = (long)((s - (double)ts.tv_sec) * 1e9);
    nanosleep(&ts, NULL);
}
#endif

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

#ifdef PORT_WIN32
extern void port_win32_message(const char *title, const char *text);
#endif

void host_fatal(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    host_vlog(fmt, ap);
    va_end(ap);
#ifdef PORT_WIN32
    {
        char buf[1024];
        va_start(ap, fmt);
        vsnprintf(buf, sizeof(buf), fmt, ap);
        va_end(ap);
        port_win32_message("1080 Snowboarding", buf); /* no console: a message box (plan 29) */
    }
#endif
    host_shutdown();
    exit(1);
}

/* keep the frame bookkeeping visible to a debugger */
host_u32 host_headless_frames_ready(void) {
    (void)sLastFrame;
    (void)sAudioRate;
    return sFramesReady;
}
