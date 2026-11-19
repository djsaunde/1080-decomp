/* port/input_map.c: the portable input mapping (input_map.h, PC port plan section 27). */
#include "input_map.h"

#include <stdlib.h>
#include <string.h>
#include <strings.h>

/* OSContPad bits */
enum {
    B_A = 0x8000, B_B = 0x4000, B_Z = 0x2000, B_START = 0x1000, D_U = 0x0800, D_D = 0x0400, D_L = 0x0200,
    D_R = 0x0100, B_L = 0x0020, B_R = 0x0010, C_U = 0x0008, C_D = 0x0004, C_L = 0x0002, C_R = 0x0001
};
static const unsigned short kActionBit[PIA_COUNT] = {
    B_A, B_B, B_Z, B_START, B_L, B_R, C_U, C_D, C_L, C_R, D_U, D_D, D_L, D_R, 0, 0, 0, 0, 0,
};

#define MAX_HELD 32
static char sHeld[MAX_HELD][32];
static int sNHeld;
static PortGamepad sPads[PORT_MAX_GAMEPADS];
static int sSuspended;

void port_input_key(const char* name, int down) {
    int i;
    for (i = 0; i < sNHeld; i++) {
        if (strcasecmp(sHeld[i], name) == 0) {
            if (!down) {
                memmove(sHeld[i], sHeld[i + 1], (size_t)(sNHeld - i - 1) * sizeof(sHeld[0]));
                sNHeld--;
            }
            return;
        }
    }
    if (down && sNHeld < MAX_HELD && strlen(name) < sizeof(sHeld[0])) {
        strcpy(sHeld[sNHeld++], name);
    }
}

void port_input_release_keys(void) {
    sNHeld = 0;
}

void port_input_set_gamepad(int i, const PortGamepad* g) {
    if (i < 0 || i >= PORT_MAX_GAMEPADS) {
        return;
    }
    if (g == NULL) {
        memset(&sPads[i], 0, sizeof(sPads[i]));
    } else {
        sPads[i] = *g;
    }
}

const PortGamepad* port_input_gamepad(int i) {
    return (i >= 0 && i < PORT_MAX_GAMEPADS) ? &sPads[i] : NULL;
}

void port_input_suspend(int on) {
    sSuspended = on;
}

/* ---- gamepad input names ---- */

enum { IN_BTN, IN_POS, IN_NEG };
typedef struct PadInput {
    const char* name;
    int kind, idx;
} PadInput;
static const PadInput kPadInputs[] = {
    { "a", IN_BTN, 0 }, { "b", IN_BTN, 1 }, { "x", IN_BTN, 2 }, { "y", IN_BTN, 3 }, { "lb", IN_BTN, 4 },
    { "rb", IN_BTN, 5 }, { "back", IN_BTN, 6 }, { "start", IN_BTN, 7 }, { "guide", IN_BTN, 8 }, { "ls", IN_BTN, 9 },
    { "rs", IN_BTN, 10 }, { "dpad_up", IN_BTN, 11 }, { "dpad_down", IN_BTN, 12 }, { "dpad_left", IN_BTN, 13 },
    { "dpad_right", IN_BTN, 14 },
    { "lt", IN_POS, 2 }, { "rt", IN_POS, 5 },
    { "hat_up", IN_NEG, 7 }, { "hat_down", IN_POS, 7 }, { "hat_left", IN_NEG, 6 }, { "hat_right", IN_POS, 6 },
    { "lx-", IN_NEG, 0 }, { "lx+", IN_POS, 0 }, { "ly-", IN_NEG, 1 }, { "ly+", IN_POS, 1 },
    { "rx-", IN_NEG, 3 }, { "rx+", IN_POS, 3 }, { "ry-", IN_NEG, 4 }, { "ry+", IN_POS, 4 },
};
#define NPADINPUTS (sizeof(kPadInputs) / sizeof(kPadInputs[0]))

static const PadInput* pad_input(const char* name, size_t len) {
    size_t i;
    for (i = 0; i < NPADINPUTS; i++) {
        if (strlen(kPadInputs[i].name) == len && strncasecmp(kPadInputs[i].name, name, len) == 0) {
            return &kPadInputs[i];
        }
    }
    return NULL;
}

int port_input_pad_name_valid(const char* name) {
    return pad_input(name, strlen(name)) != NULL;
}

static int pad_input_on(const PadInput* in, const PortGamepad* g, int t) {
    switch (in->kind) {
        case IN_BTN: return (g->buttons >> in->idx) & 1;
        case IN_POS: return g->axis[in->idx] > t;
        default: return g->axis[in->idx] < -t;
    }
}

const char* port_input_pad_pressed(const PortGamepad* g, const PortGamepad* prev, int threshold) {
    size_t i;
    if (g == NULL || !g->connected) {
        return NULL;
    }
    for (i = 0; i < NPADINPUTS; i++) {
        if (pad_input_on(&kPadInputs[i], g, threshold) && (prev == NULL || !pad_input_on(&kPadInputs[i], prev, threshold))) {
            return kPadInputs[i].name;
        }
    }
    return NULL;
}

/* ---- bindings ---- */

static int key_held(const char* name, size_t len) {
    int i;
    for (i = 0; i < sNHeld; i++) {
        if (strlen(sHeld[i]) == len && strncasecmp(sHeld[i], name, len) == 0) {
            return 1;
        }
    }
    return 0;
}

/* the next space / comma separated token of *list (its length in *len), or NULL */
static const char* next_token(const char** list, size_t* len) {
    const char* t = *list + strspn(*list, " ,\t");
    if (*t == '\0') {
        return NULL;
    }
    *len = strcspn(t, " ,\t");
    *list = t + *len;
    return t;
}

static int keys_on(const char* list) {
    const char* t;
    size_t n;
    while ((t = next_token(&list, &n)) != NULL) {
        if (key_held(t, n)) {
            return 1;
        }
    }
    return 0;
}

static int pads_on(const char* list, const PortGamepad* const* pads, int npads, int threshold) {
    const char* t;
    size_t n;
    while ((t = next_token(&list, &n)) != NULL) {
        const PadInput* in = pad_input(t, n);
        int i;
        if (in == NULL) {
            continue;
        }
        for (i = 0; i < npads; i++) {
            if (pad_input_on(in, pads[i], threshold)) {
                return 1;
            }
        }
    }
    return 0;
}

static int clampi(int v, int lo, int hi) {
    return v < lo ? lo : v > hi ? hi : v;
}

/* the analog stick: axis (-32767..32767) -> N64 value, sign kept, before the range clamp */
static int analog(const PortPlayerInput* p, int v) {
    if (p->deadzone_rescale && p->deadzone > 0 && p->deadzone < 32767) {
        int m = abs(v) - p->deadzone;
        m = m < 0 ? 0 : m;
        v = (int)((long)(v < 0 ? -m : m) * 32767 / (32767 - p->deadzone));
    }
    /* deadzone_rescale 0, sensitivity 100: v * range / 32767 (the pre-M7 v * 80 / 32767) */
    return (int)((long)v * p->range * p->sensitivity / 100 / 32767);
}

static void player_pad(const PortPlayerInput* p, PortN64Pad* out) {
    const PortGamepad* pads[PORT_MAX_GAMEPADS];
    int npads = 0, a, sx, sy, up, dn, lf, rt, ks, i;
    unsigned short b = 0;
    memset(out, 0, sizeof(*out));
    if (!p->enabled) {
        return;
    }
    out->connected = 1;
    if (sSuspended) {
        return;
    }
    const char* list = p->pads;
    const char* t;
    size_t n;
    while ((t = next_token(&list, &n)) != NULL) {
        int idx = atoi(t);
        if (idx >= 0 && idx < PORT_MAX_GAMEPADS && sPads[idx].connected && npads < PORT_MAX_GAMEPADS) {
            pads[npads++] = &sPads[idx];
        }
    }
    for (a = 0; a < PIA_COUNT; a++) {
        if (kActionBit[a] != 0 && (keys_on(p->key[a]) || pads_on(p->pad[a], pads, npads, p->trigger))) {
            b |= kActionBit[a];
        }
    }
#define ON(act) (keys_on(p->key[act]) || pads_on(p->pad[act], pads, npads, p->trigger))
    up = ON(PIA_STICK_UP);
    dn = ON(PIA_STICK_DOWN);
    lf = ON(PIA_STICK_LEFT);
    rt = ON(PIA_STICK_RIGHT);
    ks = p->key_stick;
    sx = (rt ? ks : 0) - (lf ? ks : 0);
    sy = (up ? ks : 0) - (dn ? ks : 0);
    if (ON(PIA_HALF_TILT)) {
        sx = sx * p->half_tilt / 100;
        sy = sy * p->half_tilt / 100;
    }
#undef ON
    /* the first of this player's gamepads whose stick is past the deadzone overrides the keys */
    for (i = 0; i < npads; i++) {
        int ax = pads[i]->axis[p->stick ? 3 : 0], ay = pads[i]->axis[p->stick ? 4 : 1];
        if (abs(ax) > p->deadzone || abs(ay) > p->deadzone) {
            sx = analog(p, ax);
            sy = -analog(p, ay);
            break;
        }
    }
    out->button = b;
    sx = clampi(sx, -p->range, p->range);
    sy = clampi(sy, -p->range, p->range);
    out->stick_x = (signed char)clampi(sx, -127, 127);
    out->stick_y = (signed char)clampi(sy, -127, 127);
}

void port_input_pads(const PortSettings* s, PortN64Pad out[PORT_SETTINGS_PLAYERS]) {
    int i;
    for (i = 0; i < PORT_SETTINGS_PLAYERS; i++) {
        player_pad(&s->player[i], &out[i]);
    }
}
