/* port/gfx/fast3d: a gamepad through evdev (/dev/input/event*), for the presenter when the
 * joystick API's /dev/input/js0 is absent (PC port plan 19.12). No SDL: the first event device
 * that reports gamepad buttons (BTN_GAMEPAD) and two absolute axes is used. Its state is given in
 * the js layout presenter.cpp's Input already maps (an Xbox-style pad): axes 0..7 = left X / Y,
 * left trigger, right X / Y, right trigger, hat X / Y, each scaled to -32767..32767 (a trigger
 * at rest is -32767); buttons bit i = js button i (0 A, 1 B, 2 X, 3 Y, 4 LB, 5 RB, 6 Back,
 * 7 Start). Nothing found or readable: open() returns false and nothing is printed. */
#ifndef PORT_GFX_FAST3D_LIVE_GAMEPAD_H
#define PORT_GFX_FAST3D_LIVE_GAMEPAD_H

#include <cstdint>

struct EvdevPad {
    int fd = -1;
    int lo[8] = {}, hi[8] = {};
    bool open();                              /* scan /dev/input/event* (quietly) */
    void poll(int axis[8], uint16_t* buttons); /* drain the events; *buttons = js button bits */
    uint16_t bits = 0;
};

#endif
