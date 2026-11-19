/* port/input_map.h: keyboard + gamepads -> the four N64 controllers (PC port plan section 27).
 *
 * The portable half of the input: no X11, no evdev, no Win32. A host backend feeds it
 *   - keys, by name: port_input_key("x", 1). The names are X keysym names (XKeysymToString of the
 *     unshifted keysym): letters / digits as themselves ("x", "7"), "space", "Return", "KP_Enter",
 *     "Shift_L", "Shift_R", "Control_L", "Alt_L", "Tab", "BackSpace", "Up", "Down", "Left", "Right",
 *     "F1".."F12", "comma", "period", "slash", "minus", "equal", "bracketleft", "semicolon", "KP_0".. .
 *     Matching ignores case. A Windows backend maps its virtual-key codes to these names.
 *   - gamepads, by state: port_input_set_gamepad(i, &state) each poll, i = 0..3 in the order the
 *     backend found them (today's choice is device 0: js0, else the first evdev gamepad).
 * and the host asks port_input_pads() for the N64 pads each poll.
 *
 * PortGamepad is the Linux joystick API's Xbox layout (live_gamepad.h):
 *   axis[0..7]  = left X, left Y, left trigger, right X, right Y, right trigger, hat X, hat Y, each
 *                 -32767..32767 (Y down positive; a trigger at rest is -32767; hat -32767 / 0 / 32767)
 *   buttons bit = 0 A, 1 B, 2 X, 3 Y, 4 LB, 5 RB, 6 Back, 7 Start, 8 Guide, 9 left stick, 10 right stick,
 *                 11..14 D-pad up / down / left / right (pads that report the D-pad as buttons)
 * Gamepad input names (settings pad.<action>): a b x y lb rb back start guide ls rs (buttons),
 *   dpad_up dpad_down dpad_left dpad_right (buttons 11..14), hat_up hat_down hat_left hat_right (the hat
 *   axes), lt rt (trigger axes above the trigger threshold), lx- lx+ ly- ly+ rx- rx+ ry- ry+ (stick axes
 *   past the threshold).
 *
 * The defaults (settings.c) reproduce the pre-M7 mapping exactly: keyboard + gamepad 0 -> controller 1,
 * controllers 2..4 unplugged. */
#ifndef PORT_INPUT_MAP_H
#define PORT_INPUT_MAP_H

#include "settings.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PORT_MAX_GAMEPADS 4

typedef struct PortGamepad {
    int connected;
    int axis[8];
    unsigned buttons;
} PortGamepad;

typedef struct PortN64Pad {
    unsigned short button; /* OSContPad bits (host.h) */
    signed char stick_x, stick_y;
    unsigned char connected;
} PortN64Pad;

/* a key went down / up (name: an X keysym name, see above) */
void port_input_key(const char* name, int down);
/* focus lost: no key stays held */
void port_input_release_keys(void);
/* gamepad i's state now (NULL or !connected: absent) */
void port_input_set_gamepad(int i, const PortGamepad* g);
const PortGamepad* port_input_gamepad(int i);
/* the menu is open: the game sees every controller released (still connected) */
void port_input_suspend(int on);
/* the buttons held on every gamepad now read as released until each is let go (the settings menu, opened or
 * closed from a gamepad, plan 31: the press that did it reaches neither the game nor the menu) */
void port_input_mask_held(void);
/* the four N64 controllers from the keys / gamepads held now and the settings' bindings */
void port_input_pads(const PortSettings* s, PortN64Pad out[PORT_SETTINGS_PLAYERS]);

/* gamepad input names: is name valid; the name of the first input of g pressed past threshold that was
 * not pressed in prev (the menu's "press a button"), or NULL */
int port_input_pad_name_valid(const char* name);
const char* port_input_pad_pressed(const PortGamepad* g, const PortGamepad* prev, int threshold);

#ifdef __cplusplus
}
#endif

#endif
