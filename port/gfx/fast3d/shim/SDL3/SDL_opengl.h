#pragma once
#ifdef PORT_WIN32
/* shim (port/gfx/fast3d), Windows (plan 27): opengl32 has GL 1.1; the rest are pointers loaded at start-up */
#include "../../../../win32/win32_gl.h"
#else
/* shim (port/gfx/fast3d): desktop GL with prototypes; libGL (glvnd) exports the whole API. */
#ifndef GL_GLEXT_PROTOTYPES
#define GL_GLEXT_PROTOTYPES 1
#endif
#include <GL/gl.h>
#include <GL/glext.h>
#endif
