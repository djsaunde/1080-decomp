/* port/gfx/fast3d: the live window and its input, shared by the presenter (presenter.cpp, the
 * 32-bit game's window over shared memory, plan 19.11) and the native 64-bit game's in-process host
 * (native_host.cpp, plan 21.9). X11 + EGL, no SDL; a Linux joystick or evdev gamepad (live_gamepad.cpp).
 *
 * No Xlib / LUS headers here (Xlib's macros collide with LUS names): live_backend.h holds Fast3D's
 * window backend over a LiveX11. */
#ifndef PORT_GFX_FAST3D_LIVE_WINDOW_H
#define PORT_GFX_FAST3D_LIVE_WINDOW_H

#include <cstdint>

extern "C" {
#include "../live_shm.h"
}
#include "live_gamepad.h"

struct _XDisplay;

/* CLOCK_MONOTONIC seconds */
double live_now_s();

/* an X11 window with an EGL desktop-GL context current on the calling thread */
struct LiveX11 {
    _XDisplay* dpy = nullptr;
    unsigned long win = 0, wmDelete = 0;
    void* edpy = nullptr; /* EGLDisplay */
    void* surf = nullptr; /* EGLSurface */
    void* ctx = nullptr;  /* EGLContext */
    uint32_t w = 0, h = 0;
    bool fullscreen = false;
    const char* tag = "presenter"; /* message prefix */

    /* open the window (resizable, min 320x240) and make its context current; messages explain a missing
     * display / GL driver. vsync: the swap interval (0 = frames shown as they come) */
    bool open(uint32_t width, uint32_t height, const char* title, int vsync = 0);
    void swap();
    /* osViBlack (plan 22.5 / 22.10): clear the window's back buffer to black over the frame drawn (the frame is
     * still drawn: its read-backs need it); the next swap shows black */
    void black();
    void setTitle(const char* title);
    /* the largest centred 4:3 rectangle of the window (x, y from the top-left) */
    void viewport(uint32_t* x, uint32_t* y, uint32_t* vw, uint32_t* vh) const;
    /* F11: EWMH fullscreen through the window manager */
    void toggleFullscreen();
    /* a synthetic key (an X keysym name: Return, x, Left, F11, ...) to the window: it comes back through
     * the X server into pump() like a real key (tests: --inject). false: unknown name */
    bool sendKey(const char* keysymName, bool down);
    /* the connection's file descriptor (poll) */
    int fd() const;
};

/* the keyboard and a gamepad -> N64 pad 1.
 * Keys: arrows / WASD = stick (Shift = half), X or Space = A, Z = B, C = Z, Enter = START, Q = L, E = R,
 * I / J / K / L = C-up / left / down / right, T / F / G / H = D-pad, Esc = quit, F11 = fullscreen.
 * A Linux joystick (/dev/input/js0, else the first evdev gamepad): Xbox layout. */
struct LiveInput {
    bool left = false, right = false, up = false, down = false, shift = false, quit = false;
    uint16_t keys = 0;
    int js = -1;
    EvdevPad ev; /* when there is no js device */
    int axis[8] = {};
    uint16_t jsButtons = 0;

    void key(unsigned long keysym, bool down);
    void releaseAll(); /* focus lost: nothing sticks (the joystick keeps its state) */
    void openJoystick(const char* path);
    void pollJoystick();
    LivePad pad() const;
};

/* drain the window's X events into the input (keys, F11, resize, close -> in.quit). step: printed with
 * synthetic keys. Returns the number of events handled. */
int live_pump(LiveX11& w, LiveInput& in, uint32_t step);

#endif
