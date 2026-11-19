#pragma once
/* shim (port/gfx/fast3d), Windows (plan 27): libultraship's gfx_opengl takes GLEW on mingw; the port loads
 * the GL entry points itself (port/win32/win32_gl.h, port_gl_load after the context is made) */
#include "../../../../win32/win32_gl.h"
#define GLEW_OK 0
static inline int glewInit(void) {
    return GLEW_OK;
}
