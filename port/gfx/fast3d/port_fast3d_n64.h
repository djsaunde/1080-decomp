/* port/gfx/fast3d: the N64 renderer settings (PC port plan section 25), applied by every Fast3D embedding
 * (port_fast3d.cpp: the native game and test_inproc; f3dreplay) right after Interpreter::Init and
 * gfx_set_target_ucode, before the first shader is built. Faithful by default; the environment picks the
 * modern alternatives:
 *
 *   PORT_GFX_CLIP=0          leave the near / side planes to the GPU (LUS's NoN behaviour: depth clamp, no near
 *                            clip). Default 1: the F3DEX clipper (patch 0012), as the RSP.
 *   PORT_GFX_SHADE=persp     perspective-correct shade colour and fog. Default: screen-space linear (the RDP's,
 *                            patch 0013).
 *   PORT_GFX_FILTER=linear   GL bilinear for the game's bilinear textures; =nearest; default (n64): the RDP's
 *                            3-point filter (LUS FILTER_THREE_POINT, the shader's filter3point).
 *   PORT_GFX_CLEAR=1         clear the frame to black before every frame (LUS). Default 0: the N64 framebuffers
 *                            persist (patch 0017): each frame-sized colour image (G_SETCIMG address) keeps its
 *                            own target, and a frame is drawn over what that framebuffer last held, as the RDP
 *                            draws into RDRAM that nothing clears (plan 26).
 *
 * Always on: the framebuffer read-back's RDP dither (the 2P VS results fade bleaches as on the N64).
 *
 * Header-only so f3dreplay (which does not link port_fast3d.cpp) shares it.
 *
 * The native host and the presenter take these from the settings file instead (port/settings.h, plan 27: the
 * file, then the same PORT_GFX_* variables over it) by pointing gPortN64Render at its values before Fast3D's
 * init; unset (f3dreplay, test_inproc, the i386 in-process renderer), the environment is read as above. */
#ifndef PORT_FAST3D_N64_H
#define PORT_FAST3D_N64_H

#include <cstdlib>
#include <cstring>

#include "fast/interpreter.h"
#include "fast/backends/gfx_opengl.h"

/* the settings (settings.h's gfx_clip / gfx_shade / gfx_filter / gfx_clear values) */
struct PortN64Render {
    int clip;   /* 1 F3DEX clipper */
    int shade;  /* 0 n64, 1 persp */
    int filter; /* 0 n64, 1 linear, 2 nearest */
    int clear;  /* 0 persist */
};
inline const PortN64Render* gPortN64Render = nullptr;

/* after Interpreter::Init (the shaders are built later, on first use) */
static inline void port_fast3d_n64_rapi(Fast::GfxRenderingAPIOGL* rapi) {
    const PortN64Render* r = gPortN64Render;
    const char* shade = std::getenv("PORT_GFX_SHADE");
    rapi->SetAffineShading(r != nullptr ? r->shade == 0 : (shade == nullptr || std::strcmp(shade, "persp") != 0));
    const char* filter = std::getenv("PORT_GFX_FILTER");
    Fast::FilteringMode mode = Fast::FILTER_THREE_POINT;
    if (r != nullptr ? r->filter == 1 : (filter != nullptr && std::strcmp(filter, "linear") == 0)) {
        mode = Fast::FILTER_LINEAR;
    } else if (r != nullptr ? r->filter == 2 : (filter != nullptr && std::strcmp(filter, "nearest") == 0)) {
        mode = Fast::FILTER_NONE;
    }
    rapi->SetTextureFilter(mode);
    /* read-backs quantize like the RDP's 16-bit colour image (patch 0016): the Fader's screen feedback */
    rapi->SetFramebufferDither(true);
}

/* after Interpreter::Init and gfx_set_target_ucode */
static inline void port_fast3d_n64_interp() {
    const PortN64Render* r = gPortN64Render;
    const char* clip = std::getenv("PORT_GFX_CLIP");
    gfx_set_rsp_clipping(r != nullptr ? r->clip != 0 : (clip == nullptr || std::strcmp(clip, "0") != 0));
    const char* clear = std::getenv("PORT_GFX_CLEAR");
    gfx_set_persistent_color_images(r != nullptr ? r->clear == 0 : (clear == nullptr || std::strcmp(clear, "1") != 0));
}

#endif
