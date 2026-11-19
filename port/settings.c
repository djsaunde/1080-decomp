/* port/settings.c: the settings file (settings.h, PC port plan section 27). */
#include "settings.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* const port_input_action_key[PIA_COUNT] = {
    "a", "b", "z", "start", "l", "r", "c_up", "c_down", "c_left", "c_right",
    "d_up", "d_down", "d_left", "d_right", "stick_up", "stick_down", "stick_left", "stick_right", "half_tilt",
};
const char* const port_input_action_label[PIA_COUNT] = {
    "A", "B", "Z", "Start", "L", "R", "C-Up", "C-Down", "C-Left", "C-Right",
    "D-Up", "D-Down", "D-Left", "D-Right", "Stick up", "Stick down", "Stick left", "Stick right", "Half tilt",
};

/* today's keyboard / gamepad layout (live_window.h before M7): the defaults reproduce it exactly */
static const char* const kDefaultKeys[PIA_COUNT] = {
    "x space", "z", "c", "Return KP_Enter", "q", "e", "i", "k", "j", "l",
    "t", "g", "f", "h", "Up w", "Down s", "Left a", "Right d", "Shift_L Shift_R",
};
static const char* const kDefaultPad[PIA_COUNT] = {
    "a", "b x", "lb lt", "start", "back", "rb rt", "y ry-", "ry+", "rx-", "rx+",
    "hat_up", "hat_down", "hat_left", "hat_right", "", "", "", "", "",
};

static const char* const kShade[] = { "n64", "persp", NULL };
static const char* const kFilter[] = { "n64", "linear", "nearest", NULL };

enum EnvKind { ENV_NONE, ENV_NOT0, ENV_EQ1, ENV_SHADE, ENV_FILTER, ENV_SCALE, ENV_INT, ENV_LATENCY };

typedef struct IntField {
    const char* section;
    const char* key;
    size_t off;
    const char* const* names; /* enum names (the value is the index), or NULL: a number */
    const char* env;
    int envKind;
    const char* comment;
} IntField;

#define F(m) offsetof(PortSettings, m)
static const IntField kFields[] = {
    { "graphics", "clip", F(gfx_clip), NULL, "PORT_GFX_CLIP", ENV_NOT0, "1: the F3DEX clipper (N64); 0: the GPU's (restart)" },
    { "graphics", "shade", F(gfx_shade), kShade, "PORT_GFX_SHADE", ENV_SHADE, "n64 (screen-space linear) | persp (restart)" },
    { "graphics", "filter", F(gfx_filter), kFilter, "PORT_GFX_FILTER", ENV_FILTER, "n64 (3-point) | linear | nearest (restart)" },
    { "graphics", "clear", F(gfx_clear), NULL, "PORT_GFX_CLEAR", ENV_EQ1, "0: the N64 framebuffers persist; 1: clear each frame (restart)" },
    { "graphics", "widescreen", F(widescreen), NULL, NULL, ENV_NONE, "rendering extra (agent-c): 0 = 4:3" },
    { "graphics", "resolution", F(resolution), NULL, NULL, ENV_NONE, "rendering extra (agent-c): 0 = the window's, N = N x 320x240" },
    { "graphics", "fps", F(fps), NULL, NULL, ENV_NONE, "rendering extra (agent-c): 30 = the game's step" },
    { "window", "scale", F(scale), NULL, "PORT_NATIVE_SCALE", ENV_SCALE, "window = scale x 320x240 (restart)" },
    { "window", "vsync", F(vsync), NULL, "PORT_NATIVE_VSYNC", ENV_INT, "0: the game's 30 Hz step paces the frames" },
    { "window", "fullscreen", F(fullscreen), NULL, NULL, ENV_NONE, "1: start fullscreen (F11 toggles)" },
    { "audio", "latency_ms", F(latency_ms), NULL, "PORT_NATIVE_LATENCY", ENV_LATENCY, "PulseAudio buffer (restart)" },
    { "audio", "mute", F(mute), NULL, "PORT_NATIVE_MUTE", ENV_NOT0, "1: no sound" },
    { "audio", "volume", F(volume), NULL, NULL, ENV_NONE, "0..100 (100: the game's samples unchanged)" },
};
#undef F
#define NFIELDS (sizeof(kFields) / sizeof(kFields[0]))

typedef struct PlayerField {
    const char* key;
    size_t off;
} PlayerField;
#define P(m) offsetof(PortPlayerInput, m)
static const PlayerField kPlayerInts[] = {
    { "enabled", P(enabled) }, { "deadzone", P(deadzone) }, { "deadzone_rescale", P(deadzone_rescale) },
    { "range", P(range) }, { "sensitivity", P(sensitivity) }, { "key_stick", P(key_stick) },
    { "half_tilt", P(half_tilt) }, { "trigger", P(trigger) },
};
#undef P
#define NPLAYERINTS (sizeof(kPlayerInts) / sizeof(kPlayerInts[0]))

static PortSettings sSettings;
static int sInit;
static char sPath[1024];
static int sFileVal[NFIELDS]; /* the value the file gave (or the default), before the environment */
static unsigned char sEnvOver[NFIELDS], sTouched[NFIELDS];

static int* field_ptr(PortSettings* s, const IntField* f) {
    return (int*)((char*)s + f->off);
}

void port_settings_player_defaults(PortPlayerInput* p, int player) {
    int a;
    memset(p, 0, sizeof(*p));
    p->enabled = player == 0;
    p->stick = 0;
    p->deadzone = 3000;
    p->deadzone_rescale = 0;
    p->range = 80;
    p->sensitivity = 100;
    p->key_stick = 80;
    p->half_tilt = 50;
    p->trigger = 16000;
    if (player == 0) {
        strcpy(p->pads, "0");
        for (a = 0; a < PIA_COUNT; a++) {
            strcpy(p->key[a], kDefaultKeys[a]);
            strcpy(p->pad[a], kDefaultPad[a]);
        }
    } else {
        /* a second player is off until enabled; then it takes the next gamepad and the same pad layout */
        snprintf(p->pads, sizeof(p->pads), "%d", player);
        for (a = 0; a < PIA_COUNT; a++) {
            strcpy(p->pad[a], kDefaultPad[a]);
        }
    }
}

void port_settings_defaults(PortSettings* s) {
    int i;
    memset(s, 0, sizeof(*s));
    s->gfx_clip = 1;
    s->gfx_shade = 0;
    s->gfx_filter = 0;
    s->gfx_clear = 0;
    s->widescreen = 0;
    s->resolution = 0;
    s->fps = 30;
    s->scale = 3;
    s->vsync = 0;
    s->fullscreen = 0;
    s->latency_ms = 80;
    s->mute = 0;
    s->volume = 100;
    for (i = 0; i < PORT_SETTINGS_PLAYERS; i++) {
        port_settings_player_defaults(&s->player[i], i);
    }
}

PortSettings* port_settings(void) {
    if (!sInit) {
        port_settings_defaults(&sSettings);
        sInit = 1;
    }
    return &sSettings;
}

const char* port_settings_default_path(void) {
    const char* e = getenv("PORT_SETTINGS");
    return (e != NULL && *e != '\0') ? e : "1080.ini";
}

const char* port_settings_path(void) {
    return sPath[0] != '\0' ? sPath : port_settings_default_path();
}

static char* trim(char* p) {
    char* e;
    while (isspace((unsigned char)*p)) {
        p++;
    }
    e = p + strlen(p);
    while (e > p && isspace((unsigned char)e[-1])) {
        *--e = '\0';
    }
    return p;
}

static int parse_int(const char* v, const char* const* names, int* out) {
    char* end;
    long n;
    int i;
    if (names != NULL) {
        for (i = 0; names[i] != NULL; i++) {
            if (strcmp(v, names[i]) == 0) {
                *out = i;
                return 1;
            }
        }
    }
    if (strcmp(v, "on") == 0 || strcmp(v, "yes") == 0 || strcmp(v, "true") == 0) {
        *out = 1;
        return 1;
    }
    if (strcmp(v, "off") == 0 || strcmp(v, "no") == 0 || strcmp(v, "false") == 0) {
        *out = 0;
        return 1;
    }
    n = strtol(v, &end, 10);
    if (end == v || *end != '\0') {
        return 0;
    }
    *out = (int)n;
    return 1;
}

static void set_player(PortPlayerInput* p, const char* key, const char* v) {
    size_t i;
    int a, n;
    if (strcmp(key, "pads") == 0) {
        snprintf(p->pads, sizeof(p->pads), "%s", strcmp(v, "none") == 0 ? "" : v);
        return;
    }
    if (strcmp(key, "stick") == 0) {
        p->stick = strcmp(v, "right") == 0 || strcmp(v, "1") == 0;
        return;
    }
    for (i = 0; i < NPLAYERINTS; i++) {
        if (strcmp(key, kPlayerInts[i].key) == 0) {
            if (parse_int(v, NULL, &n)) {
                *(int*)((char*)p + kPlayerInts[i].off) = n;
            }
            return;
        }
    }
    for (a = 0; a < PIA_COUNT; a++) {
        if (strncmp(key, "key.", 4) == 0 && strcmp(key + 4, port_input_action_key[a]) == 0) {
            snprintf(p->key[a], PORT_BIND_LEN, "%s", strcmp(v, "none") == 0 ? "" : v);
            return;
        }
        if (strncmp(key, "pad.", 4) == 0 && strcmp(key + 4, port_input_action_key[a]) == 0) {
            snprintf(p->pad[a], PORT_BIND_LEN, "%s", strcmp(v, "none") == 0 ? "" : v);
            return;
        }
    }
}

static int read_file(PortSettings* s, const char* path) {
    char line[512], section[64] = "";
    FILE* f = fopen(path, "r");
    if (f == NULL) {
        return 0;
    }
    while (fgets(line, sizeof(line), f) != NULL) {
        char *p, *eq, *key, *val;
        size_t i;
        p = strpbrk(line, "#;");
        if (p != NULL) {
            *p = '\0';
        }
        p = trim(line);
        if (*p == '\0') {
            continue;
        }
        if (*p == '[') {
            char* e = strchr(p, ']');
            if (e != NULL) {
                *e = '\0';
            }
            snprintf(section, sizeof(section), "%s", trim(p + 1));
            continue;
        }
        eq = strchr(p, '=');
        if (eq == NULL) {
            continue;
        }
        *eq = '\0';
        key = trim(p);
        val = trim(eq + 1);
        if (strncmp(section, "input.p", 7) == 0) {
            int n = atoi(section + 7);
            if (n >= 1 && n <= PORT_SETTINGS_PLAYERS) {
                set_player(&s->player[n - 1], key, val);
            }
            continue;
        }
        for (i = 0; i < NFIELDS; i++) {
            if (strcmp(section, kFields[i].section) == 0 && strcmp(key, kFields[i].key) == 0) {
                int v;
                if (parse_int(val, kFields[i].names, &v)) {
                    *field_ptr(s, &kFields[i]) = v;
                }
                break;
            }
        }
    }
    fclose(f);
    return 1;
}

/* the PORT_* variable's value, as the code that read it before M7 interpreted it */
static void apply_env(PortSettings* s) {
    size_t i;
    for (i = 0; i < NFIELDS; i++) {
        const IntField* f = &kFields[i];
        const char* v = f->env != NULL ? getenv(f->env) : NULL;
        int* p = field_ptr(s, f);
        sFileVal[i] = *p;
        sEnvOver[i] = 0;
        sTouched[i] = 0;
        if (v == NULL || *v == '\0') {
            continue;
        }
        switch (f->envKind) {
            case ENV_NOT0: *p = strcmp(v, "0") != 0; break;
            case ENV_EQ1: *p = strcmp(v, "1") == 0; break;
            case ENV_SHADE: *p = strcmp(v, "persp") == 0; break;
            case ENV_FILTER: *p = strcmp(v, "linear") == 0 ? 1 : strcmp(v, "nearest") == 0 ? 2 : 0; break;
            case ENV_SCALE: *p = atoi(v) < 1 ? 1 : atoi(v); break;
            case ENV_INT: *p = atoi(v); break;
            case ENV_LATENCY: *p = atoi(v) < 20 ? 20 : atoi(v); break;
            default: continue;
        }
        sEnvOver[i] = 1;
    }
}

int port_settings_load(PortSettings* s, const char* path) {
    int ok;
    if (path == NULL) {
        path = port_settings_default_path();
    }
    snprintf(sPath, sizeof(sPath), "%s", path);
    port_settings_defaults(s);
    if (s == &sSettings) {
        sInit = 1;
    }
    ok = read_file(s, path);
    apply_env(s);
    return ok;
}

static int field_index(const char* field) {
    size_t i, n;
    for (i = 0; i < NFIELDS; i++) {
        n = strlen(kFields[i].section);
        if (strncmp(field, kFields[i].section, n) == 0 && field[n] == '.' && strcmp(field + n + 1, kFields[i].key) == 0) {
            return (int)i;
        }
    }
    return -1;
}

const char* port_settings_env_overridden(const char* field) {
    int i = field_index(field);
    return (i >= 0 && sEnvOver[i] && !sTouched[i]) ? kFields[i].env : NULL;
}

void port_settings_touch(const char* field) {
    int i = field_index(field);
    if (i >= 0) {
        sTouched[i] = 1;
    }
}

int port_settings_save(const PortSettings* s, const char* path) {
    char tmp[1100];
    const char* section = "";
    size_t i;
    int n, a;
    FILE* f;
    if (path == NULL) {
        path = port_settings_path();
    }
    snprintf(tmp, sizeof(tmp), "%s.tmp", path);
    f = fopen(tmp, "w");
    if (f == NULL) {
        return 0;
    }
    fprintf(f, "# 1080 Snowboarding PC port settings (port/settings.h). Written by the settings menu (F1).\n"
               "# The PORT_* environment variables override these values.\n");
    for (i = 0; i < NFIELDS; i++) {
        const IntField* fd = &kFields[i];
        int v = (sEnvOver[i] && !sTouched[i]) ? sFileVal[i] : *(const int*)((const char*)s + fd->off);
        if (strcmp(section, fd->section) != 0) {
            section = fd->section;
            fprintf(f, "\n[%s]\n", section);
        }
        if (fd->names != NULL) {
            fprintf(f, "%s = %s", fd->key, fd->names[v < 0 ? 0 : v]);
        } else {
            fprintf(f, "%s = %d", fd->key, v);
        }
        fprintf(f, "    ; %s%s%s\n", fd->comment, fd->env ? "; env " : "", fd->env ? fd->env : "");
    }
    for (n = 0; n < PORT_SETTINGS_PLAYERS; n++) {
        const PortPlayerInput* p = &s->player[n];
        fprintf(f, "\n[input.p%d]\n", n + 1);
        fprintf(f, "enabled = %d\npads = %s\nstick = %s\n", p->enabled, p->pads[0] ? p->pads : "none",
                p->stick ? "right" : "left");
        for (i = 1; i < NPLAYERINTS; i++) {
            fprintf(f, "%s = %d\n", kPlayerInts[i].key, *(const int*)((const char*)p + kPlayerInts[i].off));
        }
        for (a = 0; a < PIA_COUNT; a++) {
            fprintf(f, "key.%s = %s\n", port_input_action_key[a], p->key[a][0] ? p->key[a] : "none");
        }
        for (a = 0; a < PIA_COUNT; a++) {
            fprintf(f, "pad.%s = %s\n", port_input_action_key[a], p->pad[a][0] ? p->pad[a] : "none");
        }
    }
    if (fclose(f) != 0) {
        return 0;
    }
    return rename(tmp, path) == 0;
}
