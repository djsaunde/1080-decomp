/* port/gfx/fast3d: the Linux gamepads, for the presenter and the native host (PC port plan 19.12, M7 27).
 * No SDL. Up to PORT_MAX_GAMEPADS devices: the Linux joystick API (/dev/input/js0, then js1..js3) or,
 * when js0 is absent, every evdev device (/dev/input/event*) that reports gamepad buttons (BTN_GAMEPAD) and
 * two absolute axes, in directory order. Device 0 is the one the pre-M7 code used (js0, else the first evdev
 * gamepad). Their state goes to the portable mapping (port/input_map.h) as PortGamepad: the js layout, an
 * Xbox-style pad (axes 0..7 = left X / Y, left trigger, right X / Y, right trigger, hat X / Y, each
 * -32767..32767, a trigger at rest -32767; buttons bit i = js button i: 0 A, 1 B, 2 X, 3 Y, 4 LB, 5 RB,
 * 6 Back, 7 Start, 8 Guide, 9 / 10 the sticks; evdev D-pad buttons = bits 11..14). */
#ifndef PORT_GFX_FAST3D_LIVE_GAMEPAD_H
#define PORT_GFX_FAST3D_LIVE_GAMEPAD_H

#include <cstdint>

extern "C" {
#include "../../input_map.h"
}

#ifdef PORT_WIN32
/* The Windows build (plan 29): SDL2's game controllers (XInput pads, the DirectInput / HID pads SDL has a
 * mapping for; any other joystick read raw in the js layout), in the order SDL found them, up to 4
 * (live_window_sdl.cpp). The same PortGamepad states and virtual pads as below. */
struct LivePads {
    void* ctl[PORT_MAX_GAMEPADS] = {}; /* SDL_GameController */
    void* joy[PORT_MAX_GAMEPADS] = {}; /* SDL_Joystick, when SDL has no controller mapping */
    int id[PORT_MAX_GAMEPADS] = { -1, -1, -1, -1 }; /* SDL instance ids */
    PortGamepad st[PORT_MAX_GAMEPADS] = {};
    void open(const char* unused); /* every device plugged in now (again on hot-plug) */
    void poll();
    PortGamepad virt[PORT_MAX_GAMEPADS] = {};
    bool virtOn[PORT_MAX_GAMEPADS] = {};
};
#else
struct EvdevPad {
    int fd = -1;
    int lo[8] = {}, hi[8] = {};
    /* open every evdev gamepad (quietly when none), at most max, skipping none; returns the count */
    static int openAll(EvdevPad* out, int max);
    void poll(PortGamepad* g); /* drain the events into g; unplugged: g->connected = 0 */
    unsigned bits = 0;
};

struct LivePads {
    int js[PORT_MAX_GAMEPADS] = { -1, -1, -1, -1 };
    EvdevPad ev[PORT_MAX_GAMEPADS];
    PortGamepad st[PORT_MAX_GAMEPADS] = {};
    /* jsPath: the first joystick (default /dev/input/js0); js1..js3 follow it when its name ends in 0 */
    void open(const char* jsPath);
    /* drain every device and give the states to port_input_set_gamepad (virtual pads override: tests) */
    void poll();
    PortGamepad virt[PORT_MAX_GAMEPADS] = {};
    bool virtOn[PORT_MAX_GAMEPADS] = {};
};
#endif

#endif
