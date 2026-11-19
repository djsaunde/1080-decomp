/* port/win32: the GL entry points past 1.1 (win32_gl.h; plan section 27). */
#include <stdio.h>
#include "win32_gl.h"

#define PORT_GL_DEFINE(name, type) type port_##name;
PORT_GL_FUNCS(PORT_GL_DEFINE)
#undef PORT_GL_DEFINE

int port_gl_load(void *(*getproc)(const char *name)) {
    int missing = 0;
#define PORT_GL_LOAD(name, type)                                    \
    port_##name = (type)getproc(#name);                             \
    if (port_##name == NULL) {                                      \
        fprintf(stderr, "win32: OpenGL entry point %s missing\n", #name); \
        missing++;                                                  \
    }
    PORT_GL_FUNCS(PORT_GL_LOAD)
#undef PORT_GL_LOAD
    return missing;
}
