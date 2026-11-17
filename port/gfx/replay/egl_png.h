#ifndef PORT_GFX_REPLAY_EGL_PNG_H
#define PORT_GFX_REPLAY_EGL_PNG_H

/* port/gfx/replay: shared by dlreplay and f3dreplay (plan section 19). */

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* A w x h pbuffer with a 24-bit depth buffer, made current. EGL on Mesa's
 * surfaceless platform, falling back to the default display. Returns 0 on failure. */
int gl_init(int w, int h);

/* Write w x h RGBA rows (bottom-up, as glReadPixels returns them) as an RGB PNG
 * with stored deflate blocks (no zlib), so equal pixels give equal files. */
int write_png(const char *path, const uint8_t *rgba, int w, int h);

#ifdef __cplusplus
}
#endif

#endif
