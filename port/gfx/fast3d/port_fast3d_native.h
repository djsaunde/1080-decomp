/* port/gfx/fast3d: the step-composited in-process renderer of the native 64-bit game (plan 21.9).
 * C++ (LP64) only: native_host.cpp drives it; the game (x32) reaches it through native_host.cpp's C
 * entry points, which take only 32-bit integers.
 *
 * A step's gfx tasks are queued as the game submits them (their lists widened at once, since the game
 * rewrites some lists in place) and drawn at the step's end as ONE frame, the way the presenter and
 * f3dreplay --then composite a step: each Run clears the window, so a Run per task would show only
 * the last task. Render-to-texture tasks (all colour images narrower than the frame) are drawn first in
 * their own pass and read back into their image in game memory. The game's read requests (colour
 * images, depth pixels and their +-LIVE_FB_DEPTH_R windows) ride on the frame's Run and are answered in
 * a LiveFbReply, which the game takes like the presenter's (port_live_fb_apply). */
#pragma once

#include <cstddef>
#include <cstdint>

namespace Fast {
class GfxWindowBackend;
}
extern "C" {
#include "../live_shm.h"
}

/* Game addresses (plan 30). The x32 / i386 game's addresses ARE host addresses; the wasm32 game's are offsets in
 * its linear memory (PORT_WASM_HOST: the wasm2c host, port/wasm/host). port_gptr turns a game address into a
 * host pointer, port_gaddr the reverse; port_gmem_ok says [a, a + n) is game memory. */
#ifdef PORT_WASM_HOST
extern "C" uint8_t* port_wasm_mem_base(void);
extern "C" uint64_t port_wasm_mem_size(void);
static inline void* port_gptr(uint64_t a) { return a == 0 ? nullptr : (void*)(port_wasm_mem_base() + (uint32_t)a); }
static inline uint32_t port_gaddr(const void* p) {
    return p == nullptr ? 0 : (uint32_t)((const uint8_t*)p - port_wasm_mem_base());
}
static inline bool port_gmem_ok(uint32_t a, size_t n) {
    return a >= 0x1000000u && (uint64_t)a + n <= port_wasm_mem_size(); /* the 16 MB global base */
}
#else
static inline void* port_gptr(uint64_t a) { return (void*)(uintptr_t)a; }
static inline uint32_t port_gaddr(const void* p) { return (uint32_t)(uintptr_t)p; }
static inline bool port_gmem_ok(uint32_t a, size_t n) { return a >= 0x10000 && (uint64_t)a + n <= 0x80000000u; }
#endif

/* the interpreter on an existing window backend whose GL context is current (no pbuffer) */
int port_fast3d_init_window(Fast::GfxWindowBackend* wapi, uint32_t width, uint32_t height);

/* queue one task of the current step; imgs (up to maxImgs): the low 24 bits of its G_SETCIMG /
 * G_SETZIMG addresses, for port_fb_task_done. Returns the number of images. */
int port_fast3d_queue(uint32_t root, uint32_t* imgs, int maxImgs);
/* the game's read requests for the step (their full pointers) */
void port_fast3d_ask_color(uint32_t img);
void port_fast3d_ask_depth(uint32_t img, uint32_t idx);

/* the last drawn frame's N64 Z at pixel idx of image img (its full pointer), read now; -1 if the last
 * frame's Z image is not img */
int port_fast3d_depth_now(uint32_t img, uint32_t idx);

struct PortFast3dStep {
    uint32_t tasks = 0, rttTasks = 0, unresolved = 0;
    uint32_t depthFar = 0, depthNear = 0, depthOther = 0, colorServed = 0;
};
/* draw the queued tasks as one frame into the viewport (x, y from the window's top-left); answer the
 * requests into reply (may be null, stamped with step / seq). false: nothing was queued */
bool port_fast3d_step(uint32_t vx, uint32_t vy, uint32_t vw, uint32_t vh, LiveFbReply* reply, uint32_t step,
                      uint32_t seq, PortFast3dStep* st);
/* draw the last step's frame again (plan 27.3): alpha < 1 interpolates between the step before it and it
 * (*interpolated = false when it could not: no previous step, a camera cut), alpha >= 1 draws it as it was. No
 * read-backs are answered. false: nothing was drawn yet */
bool port_fast3d_redraw(uint32_t vx, uint32_t vy, uint32_t vw, uint32_t vh, float alpha, bool* interpolated);
void port_fast3d_interp_stats(uint32_t* frames, uint32_t* cuts, uint32_t* snappedMtx, uint32_t* snappedVtx,
                              uint32_t* lerpedRects);
void port_fast3d_interp_log(bool on);
/* EndFrame: the window backend presents */
void port_fast3d_end_frame();
