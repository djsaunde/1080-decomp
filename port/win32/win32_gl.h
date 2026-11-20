/* port/win32: OpenGL entry points for the Windows build (plan section 29).
 *
 * opengl32.dll exports only GL 1.1; everything newer that Fast3D (libultraship's gfx_opengl.cpp), the
 * in-process compositor (port_fast3d.cpp) and the native host call is a function pointer here, loaded
 * from the current context by port_gl_load (SDL_GL_GetProcAddress) right after the window's context is
 * made. Each `glFoo` below is a macro for the pointer `port_glFoo`. Included by
 * gfx/fast3d/shim/SDL3/SDL_opengl.h and by the native host's own GL users when PORT_WIN32 is defined. */
#ifndef PORT_WIN32_GL_H
#define PORT_WIN32_GL_H

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 1
#endif
#ifndef NOMINMAX
#define NOMINMAX 1
#endif
#include <windows.h>
/* windows.h's 16-bit leftovers collide with ordinary names (port_fast3d.cpp's `near`) */
#undef near
#undef far
#include <GL/gl.h>
#include <GL/glext.h>

/* (name, PFN type) of every entry point past GL 1.1 the port calls */
#define PORT_GL_FUNCS(X) \
    X(glActiveTexture, PFNGLACTIVETEXTUREPROC) \
    X(glAttachShader, PFNGLATTACHSHADERPROC) \
    X(glBindBuffer, PFNGLBINDBUFFERPROC) \
    X(glBindFramebuffer, PFNGLBINDFRAMEBUFFERPROC) \
    X(glBindRenderbuffer, PFNGLBINDRENDERBUFFERPROC) \
    X(glBindVertexArray, PFNGLBINDVERTEXARRAYPROC) \
    X(glBlitFramebuffer, PFNGLBLITFRAMEBUFFERPROC) \
    X(glBufferData, PFNGLBUFFERDATAPROC) \
    X(glCheckFramebufferStatus, PFNGLCHECKFRAMEBUFFERSTATUSPROC) \
    X(glClientWaitSync, PFNGLCLIENTWAITSYNCPROC) \
    X(glCompileShader, PFNGLCOMPILESHADERPROC) \
    X(glCreateProgram, PFNGLCREATEPROGRAMPROC) \
    X(glCreateShader, PFNGLCREATESHADERPROC) \
    X(glDeleteBuffers, PFNGLDELETEBUFFERSPROC) \
    X(glDeleteFramebuffers, PFNGLDELETEFRAMEBUFFERSPROC) \
    X(glDeleteProgram, PFNGLDELETEPROGRAMPROC) \
    X(glDeleteRenderbuffers, PFNGLDELETERENDERBUFFERSPROC) \
    X(glDeleteShader, PFNGLDELETESHADERPROC) \
    X(glDeleteSync, PFNGLDELETESYNCPROC) \
    X(glDeleteVertexArrays, PFNGLDELETEVERTEXARRAYSPROC) \
    X(glDisableVertexAttribArray, PFNGLDISABLEVERTEXATTRIBARRAYPROC) \
    X(glEnableVertexAttribArray, PFNGLENABLEVERTEXATTRIBARRAYPROC) \
    X(glFenceSync, PFNGLFENCESYNCPROC) \
    X(glFramebufferRenderbuffer, PFNGLFRAMEBUFFERRENDERBUFFERPROC) \
    X(glFramebufferTexture2D, PFNGLFRAMEBUFFERTEXTURE2DPROC) \
    X(glGenBuffers, PFNGLGENBUFFERSPROC) \
    X(glGenFramebuffers, PFNGLGENFRAMEBUFFERSPROC) \
    X(glGenRenderbuffers, PFNGLGENRENDERBUFFERSPROC) \
    X(glGenVertexArrays, PFNGLGENVERTEXARRAYSPROC) \
    X(glGetAttribLocation, PFNGLGETATTRIBLOCATIONPROC) \
    X(glGetProgramInfoLog, PFNGLGETPROGRAMINFOLOGPROC) \
    X(glGetProgramiv, PFNGLGETPROGRAMIVPROC) \
    X(glGetShaderInfoLog, PFNGLGETSHADERINFOLOGPROC) \
    X(glGetShaderiv, PFNGLGETSHADERIVPROC) \
    X(glGetUniformLocation, PFNGLGETUNIFORMLOCATIONPROC) \
    X(glLinkProgram, PFNGLLINKPROGRAMPROC) \
    X(glRenderbufferStorage, PFNGLRENDERBUFFERSTORAGEPROC) \
    X(glRenderbufferStorageMultisample, PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC) \
    X(glShaderSource, PFNGLSHADERSOURCEPROC) \
    X(glUniform1f, PFNGLUNIFORM1FPROC) \
    X(glUniform1i, PFNGLUNIFORM1IPROC) \
    X(glUniform1iv, PFNGLUNIFORM1IVPROC) \
    X(glUniform2i, PFNGLUNIFORM2IPROC) \
    X(glUseProgram, PFNGLUSEPROGRAMPROC) \
    X(glVertexAttribPointer, PFNGLVERTEXATTRIBPOINTERPROC)

#ifdef __cplusplus
extern "C" {
#endif
#define PORT_GL_DECLARE(name, type) extern type port_##name;
PORT_GL_FUNCS(PORT_GL_DECLARE)
#undef PORT_GL_DECLARE
/* load every pointer through getproc; returns the number missing (each named on stderr) */
int port_gl_load(void *(*getproc)(const char *name));
#ifdef __cplusplus
}
#endif

#define glActiveTexture port_glActiveTexture
#define glAttachShader port_glAttachShader
#define glBindBuffer port_glBindBuffer
#define glBindFramebuffer port_glBindFramebuffer
#define glBindRenderbuffer port_glBindRenderbuffer
#define glBindVertexArray port_glBindVertexArray
#define glBlitFramebuffer port_glBlitFramebuffer
#define glBufferData port_glBufferData
#define glCheckFramebufferStatus port_glCheckFramebufferStatus
#define glClientWaitSync port_glClientWaitSync
#define glCompileShader port_glCompileShader
#define glCreateProgram port_glCreateProgram
#define glCreateShader port_glCreateShader
#define glDeleteBuffers port_glDeleteBuffers
#define glDeleteFramebuffers port_glDeleteFramebuffers
#define glDeleteProgram port_glDeleteProgram
#define glDeleteRenderbuffers port_glDeleteRenderbuffers
#define glDeleteShader port_glDeleteShader
#define glDeleteSync port_glDeleteSync
#define glDeleteVertexArrays port_glDeleteVertexArrays
#define glDisableVertexAttribArray port_glDisableVertexAttribArray
#define glEnableVertexAttribArray port_glEnableVertexAttribArray
#define glFenceSync port_glFenceSync
#define glFramebufferRenderbuffer port_glFramebufferRenderbuffer
#define glFramebufferTexture2D port_glFramebufferTexture2D
#define glGenBuffers port_glGenBuffers
#define glGenFramebuffers port_glGenFramebuffers
#define glGenRenderbuffers port_glGenRenderbuffers
#define glGenVertexArrays port_glGenVertexArrays
#define glGetAttribLocation port_glGetAttribLocation
#define glGetProgramInfoLog port_glGetProgramInfoLog
#define glGetProgramiv port_glGetProgramiv
#define glGetShaderInfoLog port_glGetShaderInfoLog
#define glGetShaderiv port_glGetShaderiv
#define glGetUniformLocation port_glGetUniformLocation
#define glLinkProgram port_glLinkProgram
#define glRenderbufferStorage port_glRenderbufferStorage
#define glRenderbufferStorageMultisample port_glRenderbufferStorageMultisample
#define glShaderSource port_glShaderSource
#define glUniform1f port_glUniform1f
#define glUniform1i port_glUniform1i
#define glUniform1iv port_glUniform1iv
#define glUniform2i port_glUniform2i
#define glUseProgram port_glUseProgram
#define glVertexAttribPointer port_glVertexAttribPointer

#endif
