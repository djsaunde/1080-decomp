/* port/gfx/fast3d: a Fast3D window backend over an EGL pbuffer (../replay/egl_png.c's
 * gl_init context): no SDL, no events, a fixed size. Shared by f3dreplay and the in-process
 * renderer (port_fast3d.cpp, PORT_GFX_FAST3D), where a desktop build would put LUS's SDL
 * window instead. Plan sections 19.6 / 19.7. */
#pragma once

#include <cstdint>

#include "fast/backends/gfx_window_manager_api.h"

class EglWindow final : public Fast::GfxWindowBackend {
  public:
    EglWindow(uint32_t w, uint32_t h) : mW(w), mH(h) {}
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
        *w = mW;
        *h = mH;
        *x = *y = 0;
    }
    void SetDimensions(uint32_t, uint32_t, int32_t, int32_t) override {}
    Ship::WindowRect GetPrimaryMonitorRect() override { return { 0, 0, mW, mH }; }
    void HandleEvents() override {}
    bool IsFrameReady() override { return true; }
    void SwapBuffersBegin() override {}
    void SwapBuffersEnd() override {}
    double GetTime() override { return 0.0; }
    int GetTargetFps() override { return 60; }
    void SetTargetFps(int) override {}
    void SetMaxFrameLatency(int) override {}
    const char* GetKeyName(int) override { return ""; }
    bool CanDisableVsync() override { return false; }
    bool IsRunning() override { return true; }
    void Destroy() override {}
    bool IsFullscreen() override { return false; }

  private:
    uint32_t mW, mH;
};
