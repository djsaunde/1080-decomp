/* port/gfx/fast3d: Fast3D's window backend over the live X11 / EGL window (live_window.h). Events are
 * handled by the owner's loop (live_pump); SwapBuffersEnd presents. Shared by the presenter and the
 * native 64-bit host (plan 21.9). */
#pragma once

#include <cstdint>

#include "fast/backends/gfx_window_manager_api.h"
#include "fast/interpreter.h"
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

/* the N64 Z (0..0xFFFC) of the w x h pixels from (x0, y0) of the frame Fast3D drew last, read now into z
 * (row-major); false if any pixel did not read. Shared by the native host (port_fast3d_depth_now, plan
 * 22.4) and the presenter (LiveDepthAsk, plan 22.10). Always two or more pixels: Fast3D's one-pixel path
 * (GfxRenderingAPIOGL::GetPixelDepth) reads GL_DEPTH_STENCIL straight from the framebuffer, which fails
 * here (GL_INVALID_OPERATION, the value left uninitialised: 0x15xx); the blit path of two or more reads
 * right. */
inline bool live_depth_now(Fast::Interpreter& gfx, uint32_t x0, uint32_t y0, uint32_t w, uint32_t h, uint16_t* z) {
    for (uint32_t y = y0; y < y0 + h; y++) {
        for (uint32_t x = x0; x < x0 + w; x++) {
            gfx.ReadbackRequestDepth(x, y);
        }
    }
    if (w * h == 1) {
        gfx.ReadbackRequestDepth(x0 == 0 ? 1 : x0 - 1, y0);
    }
    gfx.ServiceReadbacks();
    bool ok = true;
    for (uint32_t y = y0; y < y0 + h; y++) {
        for (uint32_t x = x0; x < x0 + w; x++) {
            ok &= gfx.ReadbackGetDepth(x, y, &z[(y - y0) * w + (x - x0)]);
        }
    }
    return ok;
}
