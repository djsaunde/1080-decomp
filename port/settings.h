/* port/settings.h: the PC port's settings file (PC port plan M7, section 27).
 *
 * One struct (PortSettings), loaded from an INI file and saved back by the in-game settings menu
 * (port/gfx/fast3d/settings_menu.h, F1). Faithful by default: with no file every value is today's
 * behaviour, and every extra is off. The PORT_* environment variables still override the file (they are
 * applied after it; port_settings_env_overridden() names them, and the file keeps its own value).
 *
 * Location: $PORT_SETTINGS if set (play.sh sets <run dir>/1080.ini, next to 1080.sram), else 1080.ini in
 * the current directory. Windows: the host passes its own path (next to the exe, or %APPDATA%) through
 * port_settings_load / PORT_SETTINGS.
 *
 * Format (written by port_settings_save; unknown keys are kept by nobody: they are ignored on load):
 *
 *   [graphics]          clip = 1, shade = n64|persp, filter = n64|linear|nearest, clear = 0,
 *                       widescreen = 0, hud_edges = 1, resolution = 0, msaa = 1, fps = 30, scale_filter = 1
 *                       (the rendering extras, plan 28)
 *   [window]            scale = 3, vsync = 0, fullscreen = 0
 *   [audio]             latency_ms = 80, mute = 0, volume = 100
 *   [input.p1] .. [input.p4]
 *                       enabled = 1 (p1) / 0, pads = 0 (gamepad indices, space-separated, or none),
 *                       stick = left|right, deadzone = 3000, deadzone_rescale = 0, range = 80,
 *                       sensitivity = 100, key_stick = 80, half_tilt = 50, trigger = 16000,
 *                       key.<action> = <key names>, pad.<action> = <gamepad input names>
 *
 * Plain C: the 32-bit presenter, the native host (LP64) and a Windows host all compile it. No X11 here. */
#ifndef PORT_SETTINGS_H
#define PORT_SETTINGS_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define PORT_SETTINGS_PLAYERS 4
#define PORT_BIND_LEN 64

/* the N64 controller inputs a key / gamepad input can be bound to (port/input_map.h) */
enum PortInputAction {
    PIA_A, PIA_B, PIA_Z, PIA_START, PIA_L, PIA_R,
    PIA_CU, PIA_CD, PIA_CL, PIA_CR,
    PIA_DU, PIA_DD, PIA_DL, PIA_DR,
    PIA_STICK_UP, PIA_STICK_DOWN, PIA_STICK_LEFT, PIA_STICK_RIGHT,
    PIA_HALF_TILT, /* held: the digital stick (keys / pad binds) at half_tilt percent */
    PIA_COUNT
};
extern const char* const port_input_action_key[PIA_COUNT];   /* the INI key suffix: "a", "start", "c_up", ... */
extern const char* const port_input_action_label[PIA_COUNT]; /* the menu label: "A", "Start", "C-Up", ... */

typedef struct PortPlayerInput {
    int enabled;          /* the N64 controller is plugged in (pads[i].connected): p1 1, others 0 */
    char pads[32];        /* gamepad device indices feeding this player ("0", "1", "0 1"; "" = none) */
    int stick;            /* the analog stick: 0 left, 1 right */
    int deadzone;         /* |x| or |y| above this (of 32767) uses the stick, else the keys: 3000 */
    int deadzone_rescale; /* 1: the range starts at the deadzone (no jump); 0: linear from 0 (today) */
    int range;            /* full deflection = this N64 value (the N64 stick reaches about +-80): 80 */
    int sensitivity;      /* percent of the axis before the range clamp: 100 */
    int key_stick;        /* a stick key's N64 value: 80 */
    int half_tilt;        /* percent of key_stick while half tilt is held: 50 */
    int trigger;          /* an axis bound as a button is pressed above this (of 32767): 16000 */
    char key[PIA_COUNT][PORT_BIND_LEN]; /* key names (X keysym names: x, space, Return, Shift_L, F5 ...) */
    char pad[PIA_COUNT][PORT_BIND_LEN]; /* gamepad input names (input_map.h) */
} PortPlayerInput;

typedef struct PortSettings {
    /* [graphics]: the plan-25 N64 render settings (PORT_GFX_CLIP / SHADE / FILTER / CLEAR) */
    int gfx_clip;   /* 1: the F3DEX clipper (default); 0: the GPU's */
    int gfx_shade;  /* 0 n64 (screen-space linear, default), 1 persp */
    int gfx_filter; /* 0 n64 (3-point, default), 1 linear, 2 nearest */
    int gfx_clear;  /* 0: the N64 framebuffers persist (default); 1: clear each frame */
    /* [graphics]: the rendering extras (plan 28; all off by default, read every frame by the native renderer) */
    int widescreen;   /* 0: the 4:3 picture letterboxed (default); 1: fill the window's aspect (Hor+ field of view,
                       * 2D screens pillarboxed) (PORT_WIDESCREEN) */
    int hud_edges;    /* widescreen: 1 = HUD panels at the screen's left / right edge move to the window's edges
                       * (default); 0 = all 2D stays in the centred 4:3 (PORT_HUD_EDGES) */
    int resolution;   /* internal resolution: 0 = the window's (default), N = N x 240 lines (1 = the N64's), scaled
                       * to the window (PORT_INTERNAL_RES) */
    int msaa;         /* 1 (default: off), 2, 4, 8 samples (PORT_MSAA) */
    int fps;          /* 30 (default: one frame per game step); 60, 120, 144 ... = frames presented per second, the
                       * ones between two 30 Hz steps interpolated (the game still steps at 30 Hz) (PORT_FPS) */
    int scale_filter; /* resolution != window: 1 = linear (default), 0 = nearest (PORT_SCALE_FILTER) */
    /* [window] */
    int scale;      /* window = scale x 320x240: 3 (PORT_NATIVE_SCALE) */
    int vsync;      /* 0 (PORT_NATIVE_VSYNC) */
    int fullscreen; /* 0: start windowed */
    /* [audio] */
    int latency_ms; /* 80 (PORT_NATIVE_LATENCY; Android 100) */
    int mute;       /* 0 (PORT_NATIVE_MUTE) */
    int volume;     /* 0..100: 100 = the samples unchanged (default) */
    /* [input] */
    int pad_menu; /* 1: a gamepad's Back + Start opens / closes the settings menu and the gamepads drive it (plan 31;
                   * default 0, Android 1) */
    /* [input.pN] */
    PortPlayerInput player[PORT_SETTINGS_PLAYERS];
} PortSettings;

/* the process's settings (defaults until port_settings_load) */
PortSettings* port_settings(void);
void port_settings_defaults(PortSettings* s);
void port_settings_player_defaults(PortPlayerInput* p, int player);
/* defaults, then the file (path NULL: port_settings_default_path()), then the PORT_* environment.
 * Returns 1 if the file was read. The path is remembered for port_settings_save(s, NULL). */
int port_settings_load(PortSettings* s, const char* path);
/* write the file (NULL: the loaded path). Fields the environment overrides keep the file's value unless
 * the menu changed them (port_settings_touch). Returns 1 on success. */
int port_settings_save(const PortSettings* s, const char* path);
const char* port_settings_default_path(void);
const char* port_settings_path(void); /* the path loaded / to save */
/* field: the INI "section.key" ("graphics.clip"). The environment variable that overrides it, or NULL */
const char* port_settings_env_overridden(const char* field);
/* the menu set this field: the saved file takes the value now in the struct */
void port_settings_touch(const char* field);

#ifdef __cplusplus
}
#endif

#endif
