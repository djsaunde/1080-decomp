/* port/gfx/fast3d: the settings menu's image over the frame (menu_overlay_gl.cpp, plan 27) */
#ifndef PORT_GFX_FAST3D_MENU_OVERLAY_GL_H
#define PORT_GFX_FAST3D_MENU_OVERLAY_GL_H

#include <cstdint>

/* rgba: w x h, top row first; centred in the winW x winH default framebuffer. Call with the GL context
 * current, after the frame is drawn and before the swap. */
void menu_overlay_draw(const uint8_t* rgba, int w, int h, uint32_t winW, uint32_t winH);

#endif
