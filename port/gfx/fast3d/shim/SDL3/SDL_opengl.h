#pragma once
/* shim (port/gfx/fast3d): desktop GL with prototypes; libGL (glvnd) exports the whole API. */
#ifndef GL_GLEXT_PROTOTYPES
#define GL_GLEXT_PROTOTYPES 1
#endif
#include <GL/gl.h>
#include <GL/glext.h>
