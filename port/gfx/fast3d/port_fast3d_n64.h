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
 *
 * Always on: the framebuffer read-back's RDP dither (the 2P VS results fade bleaches as on the N64).
 *
 * Header-only so f3dreplay (which does not link port_fast3d.cpp) shares it. */
#ifndef PORT_FAST3D_N64_H
#define PORT_FAST3D_N64_H

#include <cstdlib>
#include <cstring>

#include "fast/interpreter.h"
#include "fast/backends/gfx_opengl.h"

/* after Interpreter::Init (the shaders are built later, on first use) */
static inline void port_fast3d_n64_rapi(Fast::GfxRenderingAPIOGL* rapi) {
    const char* shade = std::getenv("PORT_GFX_SHADE");
    rapi->SetAffineShading(shade == nullptr || std::strcmp(shade, "persp") != 0);
    const char* filter = std::getenv("PORT_GFX_FILTER");
    Fast::FilteringMode mode = Fast::FILTER_THREE_POINT;
    if (filter != nullptr && std::strcmp(filter, "linear") == 0) {
        mode = Fast::FILTER_LINEAR;
    } else if (filter != nullptr && std::strcmp(filter, "nearest") == 0) {
        mode = Fast::FILTER_NONE;
    }
    rapi->SetTextureFilter(mode);
    /* read-backs quantize like the RDP's 16-bit colour image (patch 0016): the Fader's screen feedback */
    rapi->SetFramebufferDither(true);
}

/* after Interpreter::Init and gfx_set_target_ucode */
static inline void port_fast3d_n64_interp() {
    const char* clip = std::getenv("PORT_GFX_CLIP");
    gfx_set_rsp_clipping(clip == nullptr || std::strcmp(clip, "0") != 0);
}

#endif
