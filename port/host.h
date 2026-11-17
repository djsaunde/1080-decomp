#ifndef PORT_HOST_H
#define PORT_HOST_H

/* Host platform layer: window, input, audio device, clock, files, log.
 *
 * Everything the port needs from the operating system goes through this
 * interface, so the rest of the port (main.c, ultra/, later gfx/ and audio/)
 * has no SDL or OS dependency. Implementations:
 *   - port/host_headless.c: no window, no audio device, no real input; pads
 *     come from an input script (main.c --input, port/host/input_script.c),
 *     the trace injector (main.c --replay) or stay idle. Used for
 *     the trace / differential tests and on machines without SDL.
 *   - port/host_sdl.c (M2, not written yet): SDL2 window + GL context,
 *     SDL_GameController / keyboard, SDL audio stream.
 *
 * Plain C, no game headers: fixed-width types only. */

#include <stdarg.h>

typedef unsigned char host_u8;
typedef signed char host_s8;
typedef unsigned short host_u16;
typedef unsigned int host_u32;

#define HOST_MAX_PADS 4

/* One N64 controller as the SI sees it (OSContPad bits). */
typedef struct HostPad {
    host_u16 button;    /* N64 button bits: A 0x8000, B 0x4000, Z 0x2000, START 0x1000,
                           U/D/L/R 0x0800/0x0400/0x0200/0x0100, L 0x0020, R 0x0010,
                           C-U/D/L/R 0x0008/0x0004/0x0002/0x0001 */
    host_s8 stick_x;    /* raw N64 range, about -80..80 */
    host_s8 stick_y;
    host_u8 connected;  /* 0 = no controller in this port */
    host_u8 hasRumble;  /* 1 = report a rumble pak in this port */
} HostPad;

typedef struct HostConfig {
    const char *title;
    int windowWidth;     /* 0 = default (640) */
    int windowHeight;    /* 0 = default (480) */
    int headless;        /* 1 = never open a window or an audio device */
} HostConfig;

/* lifecycle */
int host_init(const HostConfig *cfg);  /* 0 on success */
void host_shutdown(void);
int host_poll_events(void);            /* pump OS events; 0 = quit requested */

/* input: called once per simulation step by main.c's input_poll() */
void host_read_pads(HostPad pads[HOST_MAX_PADS]);
void host_set_rumble(int channel, int on);
/* which controllers (and rumble paks) are plugged in at boot: port_ultra_init's SI reset,
 * before the first step, so osContInit sees them */
void host_boot_pads(HostPad pads[HOST_MAX_PADS]);

/* video: osViSetMode / osViBlack / osViSwapBuffer land here */
void host_video_mode(host_u32 width, host_u32 height, int interlaced);
void host_video_black(int black);
void host_frame_ready(void *framebuffer); /* game finished a frame (osViSwapBuffer) */
void host_present(void);                  /* show the newest frame (main loop, once per host frame) */

/* audio: osAiSetFrequency / osAiSetNextBuffer / osAiGetLength land here */
host_u32 host_audio_set_frequency(host_u32 hz); /* returns the rate actually used */
void host_audio_queue(const void *samples, host_u32 bytes);
host_u32 host_audio_buffered_bytes(void); /* bytes of the current buffer not yet played */

/* clock: real time, only for pacing (never fed to the game) */
double host_time_seconds(void);
void host_sleep_seconds(double s);

/* files */
int host_file_read_all(const char *path, void **data, host_u32 *size); /* 0 on success, malloc'd */
int host_file_write_all(const char *path, const void *data, host_u32 size);
void host_free(void *p);

/* log: one line per call, newline added if missing */
void host_log(const char *fmt, ...) __attribute__((format(printf, 1, 2)));
void host_vlog(const char *fmt, va_list ap);
void host_fatal(const char *fmt, ...) __attribute__((format(printf, 1, 2), noreturn));

/* headless-only: set the pads host_read_pads() returns (tests, scripted input) */
void host_headless_set_pads(const HostPad pads[HOST_MAX_PADS]);
host_u32 host_headless_frames_ready(void); /* osViSwapBuffer count */
/* headless-only: scripted controller input (--input FILE; port/host/input_script.c has the format).
 * Once opened, host_read_pads() returns the script's pads for each step instead of the set pads. */
int host_input_script_open(const char *path); /* 0 on success */
int host_input_script_active(void);
void host_input_script_step(HostPad pads[HOST_MAX_PADS]); /* this step's pads; advances one step */
void host_input_script_boot(HostPad pads[HOST_MAX_PADS]); /* step 0's pads; does not advance */
/* headless-only: receive every AI buffer (osAiSetNextBuffer: host-order s16, L R interleaved);
 * NULL = drop them (the default).  port/audio/tools/seqrender writes them to a WAV. */
void host_headless_set_audio_sink(void (*sink)(const void *samples, host_u32 bytes));
/* headless-only: what host_audio_buffered_bytes (osAiGetLength) reports; NULL = 0, i.e. every
 * buffer played at once, so the driver always picks its longest buffer (560 frames, plan 10.2).
 * seqrender models a DAC draining 32006 / 60 frames per retrace instead. */
void host_headless_set_audio_clock(host_u32 (*buffered)(void));

#endif /* PORT_HOST_H */
