/* port/gfx/fast3d: draws the settings menu's image over the window's frame (settings_menu.h, plan 27).
 * One texture behind a read framebuffer, blitted (nearest, integer scale when it fits) to the default
 * framebuffer's centre: no shader, no blending, nothing of Fast3D's GL state is touched but the framebuffer
 * bindings and the scissor test, which are restored. GL 3.0 (glBlitFramebuffer); no window-system code, so a
 * Windows host uses it as is. */
#include "menu_overlay_gl.h"

#include <algorithm>

#ifdef PORT_WIN32
#include "../../win32/win32_gl.h" /* plan 27: the GL entry points past 1.1 */
#else
#define GL_GLEXT_PROTOTYPES 1
#include <GL/gl.h>
#include <GL/glext.h>
#endif

namespace {
GLuint sTex, sFbo;
}

void menu_overlay_draw(const uint8_t* rgba, int w, int h, uint32_t winW, uint32_t winH) {
    GLint drawFb = 0, readFb = 0, tex = 0;
    GLboolean scissor = glIsEnabled(GL_SCISSOR_TEST);
    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &drawFb);
    glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &readFb);
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &tex);
    if (sTex == 0) {
        glGenTextures(1, &sTex);
        glBindTexture(GL_TEXTURE_2D, sTex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glGenFramebuffers(1, &sFbo);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, sFbo);
        glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, sTex, 0);
    }
    glBindTexture(GL_TEXTURE_2D, sTex);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, rgba);
    /* the largest integer scale that fits, else the largest fit (nearest) */
    float k = std::min((float)winW / (float)w, (float)winH / (float)h);
    if (k >= 1.0f) {
        k = (float)(int)k;
    }
    int dw = (int)((float)w * k), dh = (int)((float)h * k);
    int dx = ((int)winW - dw) / 2, dy = ((int)winH - dh) / 2;
    glBindFramebuffer(GL_READ_FRAMEBUFFER, sFbo);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glDisable(GL_SCISSOR_TEST);
    /* the image's first row is its top: flip into GL's bottom-up window */
    glBlitFramebuffer(0, 0, w, h, dx, dy + dh, dx + dw, dy, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, (GLuint)readFb);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, (GLuint)drawFb);
    glBindTexture(GL_TEXTURE_2D, (GLuint)tex);
    if (scissor) {
        glEnable(GL_SCISSOR_TEST);
    }
}
