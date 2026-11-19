#pragma once
/* shim (port/gfx/fast3d): libultraship's gfx_opengl takes GLEW on mingw and macOS; the port has the GL API from
 * port_gl.h instead (Windows: port/win32/win32_gl.h, loaded after the context is made; macOS: the OpenGL framework) */
#include "../../port_gl.h"
#define GLEW_OK 0
static inline int glewInit(void) {
    return GLEW_OK;
}
