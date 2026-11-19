/* port/gfx/fast3d: the in-game settings menu (settings_menu.h, PC port plan section 27). */
#include "settings_menu.h"

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <strings.h>

#include "menu_font.h"

namespace {

const char* const kPages[] = { "Graphics", "Window", "Audio", "P1", "P2", "P3", "P4" };
const int kNPages = 7;
const int kCols = SettingsMenu::kW / 8, kRows = SettingsMenu::kH / 16;
const int kListTop = 3, kListRows = 15; /* item rows 3..17 */

struct Rgb {
    uint8_t r, g, b;
};
const Rgb kBg = { 14, 18, 30 }, kBar = { 34, 52, 92 }, kSel = { 46, 84, 150 }, kText = { 230, 232, 240 },
          kDim = { 140, 146, 166 }, kWarn = { 240, 200, 90 }, kEnv = { 240, 140, 80 }, kHi = { 255, 255, 255 },
          kLine = { 70, 90, 140 }, kCap = { 120, 60, 20 };

bool is(const char* a, const char* b) {
    return strcasecmp(a, b) == 0;
}

} // namespace

/* one menu row */
struct SettingsMenu::Item {
    enum Kind { ENUM, INT, BIND, PADS, ACTION, HEADER } kind = ENUM;
    std::string label, note;
    const char* field = nullptr; /* "section.key" (env override / touch / apply), or null */
    int* value = nullptr;
    std::vector<std::string> names; /* ENUM: names[value - base] */
    std::vector<int> vals;          /* ENUM over these values (instead of 0..n-1) */
    int lo = 0, hi = 0, step = 1;
    bool restart = false, live = false;
    std::function<std::string(int)> fmt;
    int player = 0, action = 0; /* BIND / PADS / ACTION */
};

void SettingsMenu::toggle() {
    mOpen = !mOpen;
    mCapture = 0;
    mDirty = true;
    port_input_suspend(mOpen ? 1 : 0);
    port_input_release_keys(); /* the keys held when it opened were released into the menu */
    if (mOpen && mStatus.empty()) {
        FILE* f = std::fopen(port_settings_path(), "r");
        mStatus = std::string(f ? "Settings: " : "Defaults (no file yet): ") + port_settings_path();
        if (f != nullptr) {
            std::fclose(f);
        }
    }
}

void SettingsMenu::setPage(int p) {
    mPage = std::clamp(p, 0, kNPages - 1);
    mSel = mTop = mCol = 0;
    mDirty = true;
}

void SettingsMenu::build(std::vector<Item>& v) {
    PortSettings* s = port_settings();
    auto en = [&](const char* label, const char* field, int* val, std::vector<std::string> names, bool restart,
                  const char* note = "") {
        Item it;
        it.kind = Item::ENUM;
        it.label = label;
        it.field = field;
        it.value = val;
        it.names = std::move(names);
        it.restart = restart;
        it.live = !restart;
        it.note = note;
        v.push_back(it);
        return &v.back();
    };
    auto num = [&](const char* label, const char* field, int* val, int lo, int hi, int step, bool restart,
                   const char* note = "") {
        Item it;
        it.kind = Item::INT;
        it.label = label;
        it.field = field;
        it.value = val;
        it.lo = lo;
        it.hi = hi;
        it.step = step;
        it.restart = restart;
        it.live = !restart;
        it.note = note;
        v.push_back(it);
        return &v.back();
    };
    switch (mPage) {
        case 0:
            en("Clipping", "graphics.clip", &s->gfx_clip, { "GPU (depth clamp)", "F3DEX (N64)" }, true,
               "The near / side planes: the RSP's clipper, or the GPU's.");
            en("Shading", "graphics.shade", &s->gfx_shade, { "N64 (screen linear)", "Perspective" }, true,
               "Shade colour and fog interpolation.");
            en("Texture filter", "graphics.filter", &s->gfx_filter, { "N64 (3-point)", "Linear", "Nearest" }, true,
               "The RDP's 3-point filter, GL bilinear, or none.");
            en("Clear each frame", "graphics.clear", &s->gfx_clear, { "No (N64)", "Yes" }, true,
               "No: the N64 framebuffers persist between frames.");
            en("Widescreen", "graphics.widescreen", &s->widescreen, { "Off (4:3)", "On" }, false,
               "On: the picture fills the window's width (wider view); 2D screens stay 4:3 in the middle.");
            en("HUD at the edges", "graphics.hud_edges", &s->hud_edges, { "No (centred 4:3)", "Yes" }, false,
               "Widescreen: the race HUD panels move to the window's left / right edges.");
            num("Internal resolution", "graphics.resolution", &s->resolution, 0, 8, 1, false,
                "Window: draw at the window's size. Nx: N x 240 lines, scaled to the window.")
                ->fmt = [](int x) { return x == 0 ? std::string("Window") : std::to_string(x) + "x (" +
                                                       std::to_string(240 * x) + " lines)"; };
            en("Scaling filter", "graphics.scale_filter", &s->scale_filter, { "Nearest", "Linear" }, false,
               "How an internal resolution other than the window's is scaled to it.");
            {
                Item* it = en("Anti-aliasing", "graphics.msaa", &s->msaa, { "Off", "2x MSAA", "4x MSAA", "8x MSAA" },
                              false, "Multisampling. With it on, undrawn pixels are black (no N64 framebuffer reuse).");
                it->vals = { 1, 2, 4, 8 };
            }
            {
                Item* it = en("Frame rate", "graphics.fps", &s->fps,
                              { "30 (N64)", "60 (interpolated)", "120 (interpolated)", "144 (interpolated)",
                                "240 (interpolated)" },
                              false, "Above 30: frames between the game's 30 Hz steps are interpolated.");
                it->vals = { 30, 60, 120, 144, 240 };
            }
            break;
        case 1:
            num("Window scale", "window.scale", &s->scale, 1, 8, 1, true, "The window = scale x 320x240 at start.")
                ->fmt = [](int x) { return std::to_string(x) + "x (" + std::to_string(320 * x) + "x" +
                                           std::to_string(240 * x) + ")"; };
            en("Vsync", "window.vsync", &s->vsync, { "Off", "On" }, false,
               "Off: the game's 30 Hz step paces the frames.");
            en("Fullscreen", "window.fullscreen", &s->fullscreen, { "Off", "On" }, false, "F11 also toggles it.");
            break;
        case 2:
            num("Volume", "audio.volume", &s->volume, 0, 100, 5, false, "100: the game's samples unchanged.")
                ->fmt = [](int x) { return std::to_string(x) + " %"; };
            en("Mute", "audio.mute", &s->mute, { "Off", "On" }, false,
               "Started muted (play.sh -m): turning it off needs a restart.");
            num("Buffer", "audio.latency_ms", &s->latency_ms, 20, 500, 10, true,
                "PulseAudio's buffer: raise it if the sound crackles.")
                ->fmt = [](int x) { return std::to_string(x) + " ms"; };
            break;
        default: {
            int n = mPage - 3;
            PortPlayerInput* p = &s->player[n];
            en("Controller plugged in", nullptr, &p->enabled, { "No", "Yes" }, false,
               n == 0 ? "Controller 1." : "Turn on for a second player (2P VS).");
            {
                Item it;
                it.kind = Item::PADS;
                it.label = "Gamepad";
                it.player = n;
                it.note = "Which gamepad feeds this controller (the keyboard too, if bound).";
                v.push_back(it);
            }
            en("Analog stick", nullptr, &p->stick, { "Left", "Right" }, false, "The gamepad stick that steers.");
            num("Deadzone", nullptr, &p->deadzone, 0, 16000, 500, false,
                "Of 32767: below it on both axes the keys steer.");
            en("Rescale past deadzone", nullptr, &p->deadzone_rescale, { "No", "Yes" }, false,
               "Yes: the stick starts at 0 at the deadzone's edge.");
            num("Range", nullptr, &p->range, 20, 127, 1, false, "Full deflection's N64 value (the N64 stick: ~80).");
            num("Sensitivity", nullptr, &p->sensitivity, 25, 300, 5, false, "The stick before the range clamp.")
                ->fmt = [](int x) { return std::to_string(x) + " %"; };
            num("Key stick value", nullptr, &p->key_stick, 10, 127, 1, false, "A stick key's N64 value.");
            num("Half tilt", nullptr, &p->half_tilt, 10, 100, 5, false, "Of the key stick, while Half tilt is held.")
                ->fmt = [](int x) { return std::to_string(x) + " %"; };
            num("Trigger threshold", nullptr, &p->trigger, 2000, 32000, 1000, false,
                "An axis bound as a button presses above it (of 32767).");
            {
                Item it;
                it.kind = Item::HEADER;
                it.label = "Bindings";
                it.note = "Each N64 input: the keys and the gamepad inputs that press it.";
                v.push_back(it);
            }
            for (int a = 0; a < PIA_COUNT; a++) {
                Item it;
                it.kind = Item::BIND;
                it.label = port_input_action_label[a];
                it.player = n;
                it.action = a;
                it.note = "Enter: set, +: add another, Backspace: clear. Left / Right: keyboard / gamepad.";
                v.push_back(it);
            }
            Item it;
            it.kind = Item::ACTION;
            it.label = "Reset this controller to defaults";
            it.player = n;
            v.push_back(it);
            break;
        }
    }
}

void SettingsMenu::saved(const char* field, bool restart) {
    if (field != nullptr) {
        port_settings_touch(field);
    }
    bool ok = port_settings_save(port_settings(), nullptr) != 0;
    mStatus = ok ? std::string("Saved to ") + port_settings_path() : std::string("Cannot write ") + port_settings_path();
    if (restart) {
        mStatus = "Saved: applies after a restart";
    }
    if (field != nullptr && onApply) {
        onApply(field);
    }
}

void SettingsMenu::change(Item& it, int dir) {
    PortSettings* s = port_settings();
    if (it.kind == Item::ENUM) {
        if (!it.vals.empty()) {
            auto f = std::find(it.vals.begin(), it.vals.end(), *it.value);
            int i = f == it.vals.end() ? 0 : (int)(f - it.vals.begin());
            i = (i + dir + (int)it.vals.size()) % (int)it.vals.size();
            *it.value = it.vals[i];
        } else {
            int n = (int)it.names.size();
            *it.value = ((*it.value + dir) % n + n) % n;
        }
    } else if (it.kind == Item::INT) {
        *it.value = std::clamp(*it.value + dir * it.step, it.lo, it.hi);
    } else if (it.kind == Item::PADS) {
        static const char* const opts[] = { "", "0", "1", "2", "3" };
        char* pads = s->player[it.player].pads;
        int i = 0;
        for (int k = 0; k < 5; k++) {
            if (std::strcmp(pads, opts[k]) == 0) {
                i = k;
            }
        }
        i = (i + dir + 5) % 5;
        std::snprintf(pads, sizeof(s->player[0].pads), "%s", opts[i]);
    } else if (it.kind == Item::BIND) {
        mCol = dir < 0 ? 0 : 1;
        return;
    } else {
        return;
    }
    saved(it.field, it.restart);
}

void SettingsMenu::activate(Item& it, bool add) {
    if (it.kind == Item::BIND) {
        mCapture = mCol == 0 ? 1 : 2;
        mCaptureAdd = add;
        mCapPlayer = it.player;
        mCapAction = it.action;
        for (int i = 0; i < PORT_MAX_GAMEPADS; i++) {
            mCapPrev[i] = *port_input_gamepad(i);
        }
    } else if (it.kind == Item::ACTION) {
        port_settings_player_defaults(&port_settings()->player[it.player], it.player);
        saved(nullptr, false);
        mStatus = "Controller " + std::to_string(it.player + 1) + " reset; " + mStatus;
    } else if (!add) {
        change(it, 1);
    }
}

void SettingsMenu::bind(const std::string& name) {
    PortPlayerInput* p = &port_settings()->player[mCapPlayer];
    char* b = mCapture == 1 ? p->key[mCapAction] : p->pad[mCapAction];
    std::string cur = mCaptureAdd ? b : "";
    if (!cur.empty()) {
        cur += " ";
    }
    cur += name;
    if (cur.size() < PORT_BIND_LEN) {
        std::snprintf(b, PORT_BIND_LEN, "%s", cur.c_str());
    }
    mCapture = 0;
    saved(nullptr, false);
}

void SettingsMenu::tick() {
    if (!mOpen || mCapture != 2) {
        return;
    }
    for (int i = 0; i < PORT_MAX_GAMEPADS; i++) {
        const PortGamepad* g = port_input_gamepad(i);
        const char* n = port_input_pad_pressed(g, &mCapPrev[i], 24000);
        mCapPrev[i] = *g;
        if (n != nullptr) {
            bind(n);
            mDirty = true;
            return;
        }
    }
}

bool SettingsMenu::key(const char* name, bool down) {
    if (!mOpen) {
        return false;
    }
    if (!down) {
        return true;
    }
    mDirty = true;
    std::vector<Item> items;
    build(items);
    if (mCapture != 0) {
        if (is(name, "Escape") || is(name, "F1")) {
            mCapture = 0;
        } else if (is(name, "BackSpace") || is(name, "Delete")) {
            PortPlayerInput* p = &port_settings()->player[mCapPlayer];
            (mCapture == 1 ? p->key[mCapAction] : p->pad[mCapAction])[0] = '\0';
            mCapture = 0;
            saved(nullptr, false);
        } else if (mCapture == 1) {
            bind(name);
        }
        return true;
    }
    int n = (int)items.size();
    if (is(name, "Escape") || is(name, "F1")) {
        toggle();
    } else if (is(name, "Tab") || is(name, "Next") || is(name, "bracketright")) {
        setPage((mPage + 1) % kNPages);
    } else if (is(name, "ISO_Left_Tab") || is(name, "Prior") || is(name, "bracketleft")) {
        setPage((mPage + kNPages - 1) % kNPages);
    } else if (is(name, "Up") || is(name, "w")) {
        mSel = (mSel + n - 1) % n;
    } else if (is(name, "Down") || is(name, "s")) {
        mSel = (mSel + 1) % n;
    } else if (is(name, "Left") || is(name, "a")) {
        change(items[mSel], -1);
    } else if (is(name, "Right") || is(name, "d")) {
        change(items[mSel], 1);
    } else if (is(name, "Return") || is(name, "KP_Enter") || is(name, "space")) {
        activate(items[mSel], false);
    } else if (is(name, "plus") || is(name, "equal") || is(name, "KP_Add") || is(name, "Insert")) {
        activate(items[mSel], true);
    } else if ((is(name, "BackSpace") || is(name, "Delete")) && items[mSel].kind == Item::BIND) {
        PortPlayerInput* p = &port_settings()->player[items[mSel].player];
        (mCol == 0 ? p->key[items[mSel].action] : p->pad[items[mSel].action])[0] = '\0';
        saved(nullptr, false);
    }
    return true;
}

/* ---------------- drawing ---------------- */

namespace {

struct Canvas {
    uint8_t* px;
    void fill(int x0, int y0, int w, int h, Rgb c) {
        for (int y = std::max(0, y0); y < std::min(SettingsMenu::kH, y0 + h); y++) {
            for (int x = std::max(0, x0); x < std::min(SettingsMenu::kW, x0 + w); x++) {
                uint8_t* p = px + ((size_t)y * SettingsMenu::kW + x) * 4;
                p[0] = c.r;
                p[1] = c.g;
                p[2] = c.b;
                p[3] = 255;
            }
        }
    }
    /* text at character cell (col, row), cut at maxCols */
    void text(int col, int row, const std::string& s, Rgb c, int maxCols = 1000) {
        int n = std::min((int)s.size(), std::min(maxCols, kCols - col));
        for (int i = 0; i < n; i++) {
            unsigned ch = (unsigned char)s[i];
            if (i == n - 1 && (int)s.size() > n) {
                ch = '~';
            }
            if (ch < 32 || ch > 126) {
                ch = '?';
            }
            const unsigned char* g = kMenuFont[ch - 32];
            for (int y = 0; y < 16; y++) {
                for (int x = 0; x < 8; x++) {
                    if (g[y] & (0x80 >> x)) {
                        uint8_t* p = px + ((size_t)(row * 16 + y) * SettingsMenu::kW + (col + i) * 8 + x) * 4;
                        p[0] = c.r;
                        p[1] = c.g;
                        p[2] = c.b;
                    }
                }
            }
        }
    }
};

} // namespace

void SettingsMenu::render() {
    mPix.assign((size_t)kW * kH * 4, 0);
    Canvas cv{ mPix.data() };
    cv.fill(0, 0, kW, kH, kBg);
    cv.fill(0, 0, kW, 16, kBar);
    cv.text(1, 0, "1080 Snowboarding - Settings", kHi);
    cv.text(kCols - 14, 0, "F1/Esc: close", kText);
    int col = 1;
    for (int p = 0; p < kNPages; p++) {
        int w = (int)std::strlen(kPages[p]) + 2;
        if (p == mPage) {
            cv.fill(col * 8, 16, w * 8, 16, kSel);
        }
        cv.text(col + 1, 1, kPages[p], p == mPage ? kHi : kDim);
        col += w + 1;
    }
    cv.fill(0, 2 * 16 + 7, kW, 2, kLine);

    std::vector<Item> items;
    build(items);
    int n = (int)items.size();
    mSel = std::clamp(mSel, 0, n - 1);
    if (mSel < mTop) {
        mTop = mSel;
    }
    if (mSel >= mTop + kListRows) {
        mTop = mSel - kListRows + 1;
    }
    const PortSettings* s = port_settings();
    for (int r = 0; r < kListRows && mTop + r < n; r++) {
        const Item& it = items[mTop + r];
        int row = kListTop + r, i = mTop + r;
        bool sel = i == mSel;
        if (sel && it.kind != Item::BIND) {
            cv.fill(0, row * 16, kW, 16, kSel);
        }
        if (it.kind == Item::BIND) {
            const PortPlayerInput* p = &s->player[it.player];
            const char* k = p->key[it.action];
            const char* g = p->pad[it.action];
            bool capHere = mCapture != 0 && mCapPlayer == it.player && mCapAction == it.action;
            if (sel) {
                cv.fill(0, row * 16, 17 * 8, 16, kSel);
                cv.fill((mCol == 0 ? 17 : 39) * 8, row * 16, (mCol == 0 ? 22 : 21) * 8, 16, capHere ? kCap : kSel);
            }
            cv.text(1, row, it.label, sel ? kHi : kText, 15);
            cv.text(18, row, capHere && mCapture == 1 ? "press a key..." : (*k ? k : "-"), *k ? kText : kDim, 20);
            cv.text(40, row, capHere && mCapture == 2 ? "press a button..." : (*g ? g : "-"), *g ? kText : kDim, 19);
            continue;
        }
        if (it.kind == Item::HEADER) {
            cv.text(1, row, it.label, kDim);
            cv.text(18, row, "Keyboard", kDim);
            cv.text(40, row, "Gamepad", kDim);
            continue;
        }
        cv.text(1, row, it.label, sel ? kHi : kText, 27);
        std::string val;
        if (it.kind == Item::ENUM) {
            int idx = *it.value;
            if (!it.vals.empty()) {
                auto f = std::find(it.vals.begin(), it.vals.end(), *it.value);
                idx = f == it.vals.end() ? -1 : (int)(f - it.vals.begin());
            }
            val = idx >= 0 && idx < (int)it.names.size() ? it.names[idx] : std::to_string(*it.value);
        } else if (it.kind == Item::INT) {
            val = it.fmt ? it.fmt(*it.value) : std::to_string(*it.value);
        } else if (it.kind == Item::PADS) {
            const char* pads = s->player[it.player].pads;
            if (*pads == '\0') {
                val = "None (keyboard only)";
            } else {
                int d = std::atoi(pads);
                const PortGamepad* g = port_input_gamepad(d);
                val = std::string("Gamepad ") + pads + (g != nullptr && g->connected ? " (connected)" : " (absent)");
            }
        }
        if (it.kind != Item::ACTION && it.kind != Item::HEADER) {
            cv.text(29, row, "< " + val + " >", sel ? kHi : kText, 30);
        }
        const char* env = it.field ? port_settings_env_overridden(it.field) : nullptr;
        if (env != nullptr) {
            cv.text(kCols - 5, row, "env", kEnv);
        } else if (it.restart) {
            cv.text(kCols - 3, row, "R", kWarn);
        }
    }
    if (mTop > 0) {
        cv.text(kCols - 2, kListTop, "^", kDim);
    }
    if (mTop + kListRows < n) {
        cv.text(kCols - 2, kListTop + kListRows - 1, "v", kDim);
    }
    if (mPage >= 3) {
        cv.fill(0, (kListTop + kListRows) * 16 + 7, kW, 1, kLine);
    }
    cv.fill(0, (kRows - 4) * 16 + 7, kW, 2, kLine);
    const Item& cur = items[mSel];
    std::string note = cur.note;
    if (cur.field != nullptr && port_settings_env_overridden(cur.field) != nullptr) {
        note = std::string("Set by ") + port_settings_env_overridden(cur.field) + " for this run. " + note;
    } else if (cur.restart) {
        note += " (R: restart to apply)";
    }
    if (mCapture == 1) {
        note = "Press a key (Esc: cancel, Backspace: clear).";
    } else if (mCapture == 2) {
        note = "Press a gamepad button or move an axis (Esc: cancel).";
    }
    size_t cut = note.size();
    if (cut > (size_t)kCols - 2) {
        cut = note.rfind(' ', kCols - 2);
        cut = cut == std::string::npos ? kCols - 2 : cut;
    }
    cv.text(1, kRows - 3, note.substr(0, cut), kWarn);
    if (cut < note.size()) {
        cv.text(1, kRows - 2, note.substr(note[cut] == ' ' ? cut + 1 : cut), kWarn, kCols - 2);
    } else {
        cv.text(1, kRows - 2, "Up/Down: select  Left/Right: change  Tab: page  Enter: set", kDim);
    }
    cv.text(1, kRows - 1, mStatus, kDim, kCols - 2);
}

const uint8_t* SettingsMenu::image() {
    if (mDirty || mPix.empty() || mCapture != 0 || mPage >= 3) {
        /* the gamepad rows show live connection state; capture blinks nothing: re-render each frame (cheap) */
        render();
        mDirty = false;
    }
    return mPix.data();
}
