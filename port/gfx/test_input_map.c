/* port/gfx/test_input_map.c: the M7 input mapping (port/input_map.c, plan 27) against the pre-M7 code.
 *
 * 1. Defaults = today: random key / gamepad states through port_input_pads() with the default settings give
 *    exactly what live_window.cpp's LiveInput::pad() gave before M7 (its logic is copied below verbatim, as
 *    old_pad), controller 1 connected and 2..4 not.
 * 2. Rebinding, a second player on gamepad 1, the right stick, deadzone / range / sensitivity, half tilt.
 * 3. The settings file round-trips (save, load) and the environment overrides it without being saved.
 * make -C port/gfx test runs it (the port gate's gfx step). */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../input_map.h"
#include "../settings.h"

static int sFails;
#define CHECK(c, ...)                                                                                  \
    do {                                                                                               \
        if (!(c)) {                                                                                    \
            if (sFails++ < 20) {                                                                       \
                printf("FAIL %s:%d: ", __FILE__, __LINE__);                                            \
                printf(__VA_ARGS__);                                                                   \
                printf("\n");                                                                          \
            }                                                                                          \
        }                                                                                              \
    } while (0)

/* ---- the pre-M7 mapping (live_window.cpp LiveInput::key / pollJoystick / pad), keysyms as names ---- */
enum {
    B_A = 0x8000, B_B = 0x4000, B_Z = 0x2000, B_START = 0x1000, D_U = 0x0800, D_D = 0x0400, D_L = 0x0200,
    D_R = 0x0100, B_L = 0x0020, B_R = 0x0010, C_U = 0x0008, C_D = 0x0004, C_L = 0x0002, C_R = 0x0001
};
typedef struct Old {
    int left, right, up, down, shift;
    unsigned short keys, jsButtons;
    int dev; /* js >= 0 || ev.fd >= 0 */
    int axis[8];
} Old;
static void old_key(Old* o, const char* k, int isDown) {
    unsigned short bit = 0;
#define IS(s) (strcmp(k, s) == 0)
    if (IS("Left") || IS("a") || IS("A")) { o->left = isDown; return; }
    if (IS("Right") || IS("d") || IS("D")) { o->right = isDown; return; }
    if (IS("Up") || IS("w") || IS("W")) { o->up = isDown; return; }
    if (IS("Down") || IS("s") || IS("S")) { o->down = isDown; return; }
    if (IS("Shift_L") || IS("Shift_R")) { o->shift = isDown; return; }
    if (IS("x") || IS("X") || IS("space")) bit = B_A;
    else if (IS("z") || IS("Z")) bit = B_B;
    else if (IS("c") || IS("C")) bit = B_Z;
    else if (IS("Return") || IS("KP_Enter")) bit = B_START;
    else if (IS("q") || IS("Q")) bit = B_L;
    else if (IS("e") || IS("E")) bit = B_R;
    else if (IS("i") || IS("I")) bit = C_U;
    else if (IS("k") || IS("K")) bit = C_D;
    else if (IS("j") || IS("J")) bit = C_L;
    else if (IS("l") || IS("L")) bit = C_R;
    else if (IS("t") || IS("T")) bit = D_U;
    else if (IS("g") || IS("G")) bit = D_D;
    else if (IS("f") || IS("F")) bit = D_L;
    else if (IS("h") || IS("H")) bit = D_R;
    else return;
#undef IS
    o->keys = isDown ? (unsigned short)(o->keys | bit) : (unsigned short)(o->keys & ~bit);
}
static void old_buttons(Old* o, unsigned raw) {
    static const unsigned short map[] = { B_A, B_B, B_B, C_U, B_Z, B_R, B_L, B_START };
    int i;
    o->jsButtons = 0;
    for (i = 0; i < 8; i++) {
        o->jsButtons |= (raw >> i) & 1 ? map[i] : 0;
    }
}
static int clampi(int v, int lo, int hi) { return v < lo ? lo : v > hi ? hi : v; }
static PortN64Pad old_pad(const Old* o) {
    PortN64Pad p;
    int sx = (o->right ? 80 : 0) - (o->left ? 80 : 0), sy = (o->up ? 80 : 0) - (o->down ? 80 : 0);
    unsigned short b;
    memset(&p, 0, sizeof(p));
    if (o->shift) {
        sx /= 2;
        sy /= 2;
    }
    b = o->keys | o->jsButtons;
    if (o->dev) {
        const int* axis = o->axis;
        const int T = 16000;
        if (abs(axis[0]) > 3000 || abs(axis[1]) > 3000) {
            sx = axis[0] * 80 / 32767;
            sy = -axis[1] * 80 / 32767;
        }
        b |= axis[2] > T ? B_Z : 0;
        b |= axis[5] > T ? B_R : 0;
        b |= axis[3] < -T ? C_L : axis[3] > T ? C_R : 0;
        b |= axis[4] < -T ? C_U : axis[4] > T ? C_D : 0;
        b |= axis[6] < -T ? D_L : axis[6] > T ? D_R : 0;
        b |= axis[7] < -T ? D_U : axis[7] > T ? D_D : 0;
    }
    p.button = b;
    p.stick_x = (signed char)clampi(sx, -80, 80);
    p.stick_y = (signed char)clampi(sy, -80, 80);
    p.connected = 1;
    return p;
}

static const char* const kKeys[] = { "Left", "a", "Right", "d", "Up", "w", "Down", "s", "Shift_L", "Shift_R",
                                     "x", "space", "z", "c", "Return", "KP_Enter", "q", "e", "i", "k", "j", "l",
                                     "t", "g", "f", "h", "b", "y", "F5", "Tab", "1" };
#define NKEYS (sizeof(kKeys) / sizeof(kKeys[0]))

static unsigned rng = 12345;
static unsigned rnd(void) {
    rng = rng * 1103515245u + 12345u;
    return rng >> 8;
}
static int rnd_axis(void) {
    switch (rnd() % 6) {
        case 0: return 0;
        case 1: return -32767;
        case 2: return 32767;
        case 3: return (int)(rnd() % 6001) - 3000; /* around the deadzone */
        case 4: return (int)(rnd() % 4001) + 14000; /* around the threshold */
        default: return (int)(rnd() % 65535) - 32767;
    }
}

static void test_defaults_equal_old(void) {
    PortSettings* s = port_settings();
    Old o;
    PortGamepad g;
    int it, i, k;
    int held[NKEYS] = { 0 };
    memset(&o, 0, sizeof(o));
    memset(&g, 0, sizeof(g));
    port_settings_defaults(s);
    port_input_release_keys();
    for (it = 0; it < 200000; it++) {
        PortN64Pad want, got[PORT_SETTINGS_PLAYERS];
        int r = (int)(rnd() % 10);
        if (r < 5) { /* a key event (the old code saw keysym case as X delivered it: lower case mostly) */
            int ki = (int)(rnd() % NKEYS), down = (int)(rnd() & 1), j;
            /* the old code kept one flag per N64 input, so releasing one of two held keys bound to the same
             * input released it (holding Left and A, then letting go of A, stopped steering): the one
             * deliberate difference. The model replays the held set through old_key instead. */
            held[ki] = down;
            o.left = o.right = o.up = o.down = o.shift = 0;
            o.keys = 0;
            for (j = 0; j < (int)NKEYS; j++) {
                if (held[j]) {
                    old_key(&o, kKeys[j], 1);
                }
            }
            port_input_key(kKeys[ki], down);
        } else if (r < 8) {
            g.axis[rnd() % 8] = rnd_axis();
        } else if (r < 9) {
            g.buttons = rnd() & 0x7FFF;
            old_buttons(&o, g.buttons);
        } else {
            g.connected = (rnd() % 4) != 0;
            o.dev = g.connected;
        }
        if (!g.connected) {
            /* the old code kept jsButtons from its last poll only while a device was open */
            old_buttons(&o, 0);
            g.buttons = 0;
        }
        memcpy(o.axis, g.axis, sizeof(o.axis));
        port_input_set_gamepad(0, &g);
        want = old_pad(&o);
        port_input_pads(s, got);
        CHECK(memcmp(&want, &got[0], sizeof(want)) == 0, "iter %d: old %04x %d,%d new %04x %d,%d", it, want.button,
              want.stick_x, want.stick_y, got[0].button, got[0].stick_x, got[0].stick_y);
        for (i = 1; i < PORT_SETTINGS_PLAYERS; i++) {
            CHECK(got[i].connected == 0 && got[i].button == 0, "iter %d: pad %d connected", it, i + 1);
        }
        (void)k;
    }
    port_input_release_keys();
    port_input_set_gamepad(0, NULL);
}

static void test_rebind_and_players(void) {
    PortSettings* s = port_settings();
    PortN64Pad p[PORT_SETTINGS_PLAYERS];
    PortGamepad g0, g1;
    port_settings_defaults(s);
    memset(&g0, 0, sizeof(g0));
    memset(&g1, 0, sizeof(g1));
    /* START on p (Return no longer), A on the pad's Y */
    strcpy(s->player[0].key[PIA_START], "p");
    strcpy(s->player[0].pad[PIA_A], "y");
    port_input_key("Return", 1);
    port_input_pads(s, p);
    CHECK(p[0].button == 0, "Return unbound but gave %04x", p[0].button);
    port_input_key("P", 1); /* case folded */
    port_input_pads(s, p);
    CHECK(p[0].button == B_START, "p -> START, got %04x", p[0].button);
    port_input_release_keys();
    g0.connected = 1;
    g0.buttons = 1u << 3; /* Y */
    port_input_set_gamepad(0, &g0);
    port_input_pads(s, p);
    CHECK(p[0].button == (B_A | C_U), "Y -> A + C-Up (both bound), got %04x", p[0].button);
    g0.buttons = 0;
    /* player 2 on gamepad 1, player 1 keyboard only */
    s->player[1].enabled = 1;
    strcpy(s->player[1].pads, "1");
    strcpy(s->player[0].pads, "");
    g1.connected = 1;
    g1.buttons = 1u << 7; /* Start */
    g1.axis[0] = 32767;
    g1.axis[1] = -16384;
    port_input_set_gamepad(0, &g0);
    port_input_set_gamepad(1, &g1);
    port_input_key("Left", 1);
    port_input_pads(s, p);
    CHECK(p[0].connected && p[0].button == 0 && p[0].stick_x == -80 && p[0].stick_y == 0, "p1 keyboard: %04x %d,%d",
          p[0].button, p[0].stick_x, p[0].stick_y);
    CHECK(p[1].connected && p[1].button == B_START && p[1].stick_x == 80 && p[1].stick_y == 40, "p2 pad 1: %d %04x %d,%d",
          p[1].connected, p[1].button, p[1].stick_x, p[1].stick_y);
    CHECK(!p[2].connected && !p[3].connected, "p3 / p4 connected");
    /* half tilt on a pad button, the right stick, range / sensitivity / deadzone */
    port_input_release_keys();
    strcpy(s->player[1].pad[PIA_HALF_TILT], "lb");
    strcpy(s->player[1].pad[PIA_STICK_LEFT], "hat_left");
    g1.axis[0] = g1.axis[1] = 0;
    g1.axis[6] = -32767;
    g1.buttons = 1u << 4;
    port_input_set_gamepad(1, &g1);
    port_input_pads(s, p);
    CHECK(p[1].stick_x == -40 && (p[1].button & B_Z), "hat left half tilt: %d %04x", p[1].stick_x, p[1].button);
    g1.axis[6] = 0;
    g1.buttons = 0;
    s->player[1].stick = 1;
    s->player[1].range = 100;
    s->player[1].sensitivity = 150;
    g1.axis[3] = 32767 / 2;
    port_input_set_gamepad(1, &g1);
    port_input_pads(s, p);
    CHECK(p[1].stick_x == 74, "right stick half, range 100 x 1.5: %d", p[1].stick_x);
    g1.axis[3] = 32767;
    port_input_set_gamepad(1, &g1);
    port_input_pads(s, p);
    CHECK(p[1].stick_x == 100, "clamped to range 100: %d", p[1].stick_x);
    s->player[1].sensitivity = 100;
    s->player[1].deadzone = 8000;
    s->player[1].deadzone_rescale = 1;
    g1.axis[3] = 8001;
    port_input_set_gamepad(1, &g1);
    port_input_pads(s, p);
    CHECK(p[1].stick_x == 0, "rescaled deadzone starts at 0: %d", p[1].stick_x);
    g1.axis[3] = 0;
    g1.axis[4] = 7999;
    g1.axis[0] = 32767; /* the left stick no longer counts */
    port_input_set_gamepad(1, &g1);
    port_input_pads(s, p);
    CHECK(p[1].stick_x == 0 && p[1].stick_y == 0, "inside the deadzone: %d,%d", p[1].stick_x, p[1].stick_y);
    /* the menu: everything released, still connected */
    port_input_suspend(1);
    g1.buttons = 0xFF;
    port_input_set_gamepad(1, &g1);
    port_input_pads(s, p);
    CHECK(p[1].connected && p[1].button == 0 && p[1].stick_x == 0, "suspended");
    port_input_suspend(0);
    /* the menu's "press a button" */
    {
        PortGamepad prev = g1;
        const char* n;
        g1.buttons |= 1u << 9;
        n = port_input_pad_pressed(&g1, &prev, 24000);
        CHECK(n != NULL && strcmp(n, "ls") == 0, "pressed ls: %s", n ? n : "-");
        prev = g1;
        g1.axis[5] = 32767;
        n = port_input_pad_pressed(&g1, &prev, 24000);
        CHECK(n != NULL && strcmp(n, "rt") == 0, "pressed rt: %s", n ? n : "-");
    }
    port_input_set_gamepad(0, NULL);
    port_input_set_gamepad(1, NULL);
}

static void test_file(const char* dir) {
    char path[1024];
    PortSettings a, b;
    FILE* f;
    snprintf(path, sizeof(path), "%s/test_settings.ini", dir);
    remove(path);
    unsetenv("PORT_GFX_FILTER");
    unsetenv("PORT_NATIVE_SCALE");
    CHECK(port_settings_load(&a, path) == 0, "no file");
    port_settings_defaults(&b);
    CHECK(memcmp(&a, &b, sizeof(a)) == 0, "no file = defaults");
    a.gfx_filter = 2;
    a.volume = 40;
    a.player[1].enabled = 1;
    strcpy(a.player[1].key[PIA_A], "KP_1 KP_2");
    strcpy(a.player[0].key[PIA_START], "");
    CHECK(port_settings_save(&a, path), "save");
    CHECK(port_settings_load(&b, path) == 1, "load");
    CHECK(memcmp(&a, &b, sizeof(a)) == 0, "round trip");
    /* the environment wins over the file and is not written back */
    setenv("PORT_GFX_FILTER", "linear", 1);
    setenv("PORT_NATIVE_SCALE", "5", 1);
    port_settings_load(&b, path);
    CHECK(b.gfx_filter == 1 && b.scale == 5, "env override: filter %d scale %d", b.gfx_filter, b.scale);
    CHECK(port_settings_env_overridden("graphics.filter") != NULL, "filter overridden");
    CHECK(port_settings_env_overridden("audio.volume") == NULL, "volume not overridden");
    b.volume = 55;
    port_settings_save(&b, path);
    unsetenv("PORT_GFX_FILTER");
    unsetenv("PORT_NATIVE_SCALE");
    port_settings_load(&b, path);
    CHECK(b.gfx_filter == 2 && b.scale == 3 && b.volume == 55, "env not saved: filter %d scale %d volume %d",
          b.gfx_filter, b.scale, b.volume);
    /* a hand-written file: names, comments, unknown keys */
    f = fopen(path, "w");
    fprintf(f, "# hand\n[graphics]\nfilter = nearest ; c\nbogus = 3\n[window]\nvsync = on\n[input.p2]\nenabled = yes\n"
               "pads = none\nkey.start = Return\n[input.p9]\nenabled = 1\n");
    fclose(f);
    port_settings_load(&b, path);
    CHECK(b.gfx_filter == 2 && b.vsync == 1 && b.player[1].enabled == 1 && b.player[1].pads[0] == '\0' &&
          strcmp(b.player[1].key[PIA_START], "Return") == 0, "hand-written file");
    remove(path);
}

/* plan 31: port_input_mask_held hides the buttons held now until each is released; the defaults keep pad_menu off */
static void test_mask_held(void) {
    PortSettings s;
    PortN64Pad p[PORT_SETTINGS_PLAYERS];
    PortGamepad g;
    port_settings_defaults(&s);
    CHECK(s.pad_menu == 0, "pad_menu on by default");
    port_input_release_keys();
    memset(&g, 0, sizeof(g));
    g.connected = 1;
    g.axis[2] = g.axis[5] = -32767;
    g.buttons = (1u << 7) | (1u << 0); /* Start + A held */
    port_input_set_gamepad(0, &g);
    port_input_mask_held();
    port_input_pads(&s, p);
    CHECK(p[0].button == 0, "masked Start + A still pressed: %04x", p[0].button);
    g.buttons = 1u << 7; /* A released, Start still held */
    port_input_set_gamepad(0, &g);
    g.buttons = (1u << 7) | (1u << 0); /* A pressed again: no longer masked */
    port_input_set_gamepad(0, &g);
    port_input_pads(&s, p);
    CHECK(p[0].button == B_A, "A after its release + press: %04x", p[0].button);
    g.buttons = 0;
    port_input_set_gamepad(0, &g);
    g.buttons = 1u << 7;
    port_input_set_gamepad(0, &g);
    port_input_pads(&s, p);
    CHECK(p[0].button == B_START, "Start after its release + press: %04x", p[0].button);
    port_input_set_gamepad(0, NULL);
}

int main(int argc, char** argv) {
    test_defaults_equal_old();
    test_mask_held();
    test_rebind_and_players();
    test_file(argc > 1 ? argv[1] : ".");
    printf("test_input_map: %s (%d failures)\n", sFails ? "FAIL" : "ok", sFails);
    return sFails != 0;
}
