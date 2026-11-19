/* port/gfx/fast3d: the OpenGL API for the in-process hosts.
 *   Windows (plan 29): opengl32's GL 1.1 plus the entry points port/win32/win32_gl.h loads after the context;
 *   macOS (plan 30): the OpenGL framework's 4.1 core profile (deprecated by Apple, still shipped);
 *   Linux: libGL (glvnd) with prototypes. */
#pragma once
#if defined(PORT_WIN32)
#include "../../win32/win32_gl.h"
#elif defined(__APPLE__)
#ifndef GL_SILENCE_DEPRECATION
#define GL_SILENCE_DEPRECATION 1
#endif
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#else
#ifndef GL_GLEXT_PROTOTYPES
#define GL_GLEXT_PROTOTYPES 1
#endif
#include <GL/gl.h>
#include <GL/glext.h>
#endif
