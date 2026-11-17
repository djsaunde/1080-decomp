/* port/gfx/fast3d: Fast3D's window backend over the live X11 / EGL window (live_window.h). Events are
 * handled by the owner's loop (live_pump); SwapBuffersEnd presents. Shared by the presenter and the
 * native 64-bit host (plan 21.9). */
#pragma once

#include "fast/backends/gfx_window_manager_api.h"
#include "live_window.h"

class LiveWindow final : public Fast::GfxWindowBackend {
  public:
    explicit LiveWindow(LiveX11* w) : mWin(w) {}
    void Init(const char*, const char*, bool, uint32_t, uint32_t, int32_t, int32_t) override {}
    void Close() override {}
    void SetKeyboardCallbacks(bool (*)(int), bool (*)(int), void (*)()) override {}
    void SetMouseCallbacks(bool (*)(int), bool (*)(int)) override {}
    void SetFullscreenChangedCallback(void (*)(bool)) override {}
    void SetFullscreen(bool) override {}
    void GetActiveWindowRefreshRate(uint32_t* r) override { *r = 60; }
    void SetCursorVisibility(bool) override {}
    void SetMousePos(int32_t, int32_t) override {}
    void GetMousePos(int32_t* x, int32_t* y) override { *x = *y = 0; }
    void GetMouseDelta(int32_t* x, int32_t* y) override { *x = *y = 0; }
    void GetMouseWheel(float* x, float* y) override { *x = *y = 0; }
    bool GetMouseState(uint32_t) override { return false; }
    void SetMouseCapture(bool) override {}
    bool IsMouseCaptured() override { return false; }
    void GetDimensions(uint32_t* w, uint32_t* h, int32_t* x, int32_t* y) override {
        *w = mWin->w;
        *h = mWin->h;
        *x = *y = 0;
    }
    void SetDimensions(uint32_t, uint32_t, int32_t, int32_t) override {}
    Ship::WindowRect GetPrimaryMonitorRect() override { return { 0, 0, mWin->w, mWin->h }; }
    void HandleEvents() override {}
    bool IsFrameReady() override { return true; }
    void SwapBuffersBegin() override {}
    void SwapBuffersEnd() override {
        if (mSwap) {
            mWin->swap();
        }
    }
    double GetTime() override { return live_now_s(); }
    int GetTargetFps() override { return 30; }
    void SetTargetFps(int) override {}
    void SetMaxFrameLatency(int) override {}
    const char* GetKeyName(int) override { return ""; }
    bool CanDisableVsync() override { return true; }
    bool IsRunning() override { return true; }
    void Destroy() override {}
    bool IsFullscreen() override { return false; }

    bool mSwap = true; /* false: EndFrame does not present (the owner swaps) */

  private:
    LiveX11* mWin;
};
