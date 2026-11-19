/* port/gfx/fast3d: the in-game settings menu (PC port plan M7, section 27).
 *
 * F1 opens it over the game; while it is open the game sees every controller released
 * (port_input_suspend). Pages: Graphics, Window, Audio, P1..P4 (bindings: "press a key / button").
 * Every change is written to the settings file at once (port/settings.h); the ones that need a restart say so.
 *
 * Portable: no X11 / GL. The host gives it key names (input_map.h names), calls tick() each poll (the
 * gamepads, for "press a button"), and draws image() (RGBA, kW x kH) over the frame while open
 * (menu_overlay_gl.cpp: one texture, one framebuffer blit). onApply(field) is called for each setting that
 * applies live ("window.vsync", "window.fullscreen", "audio.volume", "audio.mute"; the input settings apply
 * by themselves). */
#ifndef PORT_GFX_FAST3D_SETTINGS_MENU_H
#define PORT_GFX_FAST3D_SETTINGS_MENU_H

#include <cstdint>
#include <functional>
#include <string>
#include <vector>

extern "C" {
#include "../../input_map.h"
#include "../../settings.h"
}

class SettingsMenu {
  public:
    static const int kW = 480, kH = 360;

    bool isOpen() const { return mOpen; }
    void toggle();
    /* a key while open (the host routes every key here then, except F11); returns true if handled */
    bool key(const char* name, bool down);
    /* each poll: gamepad capture */
    void tick();
    /* the RGBA image (kW x kH, top row first) */
    const uint8_t* image();
    /* for tests (PORT_NATIVE_INJECT / screenshots): the page to show */
    void setPage(int p);

    std::function<void(const char* field)> onApply;

  private:
    struct Item;
    void build(std::vector<Item>& items);
    void change(Item& it, int dir);
    void activate(Item& it, bool add);
    void saved(const char* field, bool restart);
    void bind(const std::string& name);
    void render();

    bool mOpen = false, mDirty = true;
    int mPage = 0, mSel = 0, mCol = 0, mTop = 0;
    int mCapture = 0; /* 0 none, 1 key, 2 gamepad */
    bool mCaptureAdd = false;
    int mCapPlayer = 0, mCapAction = 0;
    PortGamepad mCapPrev[PORT_MAX_GAMEPADS] = {};
    std::string mStatus;
    std::vector<uint8_t> mPix;
};

#endif
