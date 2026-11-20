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

/* CLOCK_MONOTONIC seconds */
double live_now_s();

#ifdef PORT_SDL
/* The Windows build (plan 29): the same interface over SDL2 (live_window_sdl.cpp): an SDL window with a
 * desktop GL context, the keyboard and an SDL game controller (XInput / DirectInput / HID). */
struct LiveSDL {
    void* win = nullptr; /* SDL_Window */
    void* ctx = nullptr; /* SDL_GLContext */
    uint32_t w = 0, h = 0;
    bool fullscreen = false;
    const char* tag = "native";

    bool open(uint32_t width, uint32_t height, const char* title, int vsync = 0);
    void swap();
    void setVsync(int interval);
    void black();
    void setTitle(const char* title);
    void viewport(uint32_t* x, uint32_t* y, uint32_t* vw, uint32_t* vh) const;
    void toggleFullscreen(); /* F11 / Alt+Enter: borderless fullscreen desktop */
    /* a synthetic key (an X keysym name as on Linux: Return, x, Left, F11, ...) pushed as an SDL event */
    bool sendKey(const char* keysymName, bool down);
};
using LiveWin = LiveSDL;

/* the keyboard and the gamepads -> the N64 controllers through the portable mapping (port/input_map.h) and
 * the settings' bindings (port/settings.h), as on Linux (below): the keys go in by X keysym name (an SDL
 * keycode -> keysym table, live_window_sdl.cpp), the gamepads as PortGamepad states (LivePads). */
class SettingsMenu;
struct LiveInput {
    bool quit = false;
    LivePads pads;
    SettingsMenu* menu = nullptr; /* F1 (null: no menu) */
    bool perfToggle = false;      /* F3 pressed: the host toggles its perf log (plan 33) */

    void key(const char* name, bool down);
    void releaseAll();
    void openJoystick(const char* path);
    void pollJoystick();
    LivePad pad() const; /* controller 1 */
    void allPads(LivePad out[LIVE_PADS]) const;
};

/* drain the SDL events into the input (keys, F11, resize, close -> in.quit) */
int live_pump(LiveSDL& w, LiveInput& in, uint32_t step);

#else
struct _XDisplay;

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
    void setVsync(int interval); /* the swap interval now (the settings menu) */
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

/* the keyboard and the gamepads -> the N64 controllers, through the portable mapping (port/input_map.h) and
 * the settings' bindings (port/settings.h; plan 27). The default bindings are the pre-M7 layout:
 * Keys: arrows / WASD = stick (Shift = half), X or Space = A, Z = B, C = Z, Enter = START, Q = L, E = R,
 * I / J / K / L = C-up / left / down / right, T / F / G / H = D-pad, controller 1.
 * Fixed: Esc = quit, F11 = fullscreen, F1 = the settings menu (when the host gives one).
 * Gamepads (live_gamepad.h; /dev/input/js0.., else the evdev gamepads): Xbox layout, gamepad 0 -> controller 1. */
class SettingsMenu;
struct LiveInput {
    bool quit = false;
    LivePads pads;
    SettingsMenu* menu = nullptr; /* F1 (null: no menu) */
    bool perfToggle = false;      /* F3 pressed: the host toggles its perf log (plan 33) */

    void key(const char* name, bool down); /* an X keysym name (input_map.h) */
    void releaseAll(); /* focus lost: no key sticks (the gamepads keep their state) */
    void openJoystick(const char* path);
    void pollJoystick();
    LivePad pad() const; /* controller 1 */
    void allPads(LivePad out[LIVE_PADS]) const;
};

/* drain the window's X events into the input (keys, F11, resize, close -> in.quit). step: printed with
 * synthetic keys. Returns the number of events handled. */
int live_pump(LiveX11& w, LiveInput& in, uint32_t step);
using LiveWin = LiveX11;
#endif

#endif
