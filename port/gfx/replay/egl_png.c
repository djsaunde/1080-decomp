/* port/gfx/replay: the EGL offscreen context and the PNG writer shared by
 * dlreplay (the bring-up interpreter) and f3dreplay (libultraship's Fast3D).
 * PC port plan section 19. Taken out of dlreplay.c unchanged. */

#ifndef PORT_SDL
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GL/gl.h>
#endif

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "egl_png.h"

/* ---------------- PNG writer (stored deflate, no zlib) ---------------- */

static uint32_t crc_tab[256];
static uint32_t crc(uint32_t c, const uint8_t *p, size_t n) {
    size_t i;
    if (crc_tab[1] == 0) {
        uint32_t k, j, x;
        for (k = 0; k < 256; k++) {
            for (x = k, j = 0; j < 8; j++) {
                x = (x & 1) ? 0xEDB88320u ^ (x >> 1) : x >> 1;
            }
            crc_tab[k] = x;
        }
    }
    c = ~c;
    for (i = 0; i < n; i++) {
        c = crc_tab[(c ^ p[i]) & 0xFF] ^ (c >> 8);
    }
    return ~c;
}

static void be32(uint8_t *p, uint32_t v) {
    p[0] = v >> 24;
    p[1] = v >> 16;
    p[2] = v >> 8;
    p[3] = v;
}

static void png_chunk(FILE *f, const char *type, const uint8_t *d, uint32_t n) {
    uint8_t h[8];
    uint32_t c;
    be32(h, n);
    memcpy(h + 4, type, 4);
    fwrite(h, 1, 8, f);
    fwrite(d, 1, n, f);
    c = crc(crc(0, h + 4, 4), d, n);
    be32(h, c);
    fwrite(h, 1, 4, f);
}

int write_png(const char *path, const uint8_t *rgba, int w, int h) {
    size_t raw = (size_t)h * (1 + (size_t)w * 3), nblk = (raw + 65534) / 65535, i, o = 2;
    uint8_t *r = malloc(raw), *z = malloc(raw + nblk * 5 + 6), ihdr[13];
    uint32_t a = 1, b = 0;
    FILE *f;
    int x, y;
    for (y = 0; y < h; y++) { /* GL rows are bottom-up */
        uint8_t *row = r + (size_t)y * (1 + w * 3);
        const uint8_t *src = rgba + (size_t)(h - 1 - y) * w * 4;
        row[0] = 0;
        for (x = 0; x < w; x++) {
            memcpy(row + 1 + x * 3, src + x * 4, 3);
        }
    }
    z[0] = 0x78;
    z[1] = 0x01;
    for (i = 0; i < raw; i += 65535) {
        size_t n = raw - i < 65535 ? raw - i : 65535;
        z[o++] = (i + n == raw);
        z[o++] = n & 0xFF;
        z[o++] = n >> 8;
        z[o++] = ~n & 0xFF;
        z[o++] = (~n >> 8) & 0xFF;
        memcpy(z + o, r + i, n);
        o += n;
    }
    for (i = 0; i < raw; i++) {
        a = (a + r[i]) % 65521;
        b = (b + a) % 65521;
    }
    be32(z + o, (b << 16) | a);
    o += 4;
    f = fopen(path, "wb");
    if (f == NULL) {
        return 0;
    }
    fwrite("\x89PNG\r\n\x1a\n", 1, 8, f);
    be32(ihdr, w);
    be32(ihdr + 4, h);
    ihdr[8] = 8;  /* depth */
    ihdr[9] = 2;  /* RGB */
    ihdr[10] = ihdr[11] = ihdr[12] = 0;
    png_chunk(f, "IHDR", ihdr, 13);
    png_chunk(f, "IDAT", z, (uint32_t)o);
    png_chunk(f, "IEND", NULL, 0);
    fclose(f);
    free(r);
    free(z);
    return 1;
}

/* ---------------- EGL ---------------- */

#ifdef PORT_SDL
/* the SDL hosts (Windows, plan 29; macOS, plan 30) render into its SDL window's context only: no offscreen EGL */
int gl_init(int w, int h) {
    (void)w;
    (void)h;
    return 0;
}
#else
int gl_init(int w, int h) {
    PFNEGLGETPLATFORMDISPLAYEXTPROC gpd = (PFNEGLGETPLATFORMDISPLAYEXTPROC)eglGetProcAddress("eglGetPlatformDisplayEXT");
    EGLint ca[] = { EGL_SURFACE_TYPE, EGL_PBUFFER_BIT, EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT, EGL_RED_SIZE, 8,
                    EGL_GREEN_SIZE, 8, EGL_BLUE_SIZE, 8, EGL_DEPTH_SIZE, 24, EGL_NONE };
    EGLint pa[] = { EGL_WIDTH, w, EGL_HEIGHT, h, EGL_NONE };
    EGLDisplay d = gpd ? gpd(EGL_PLATFORM_SURFACELESS_MESA, EGL_DEFAULT_DISPLAY, NULL) : EGL_NO_DISPLAY;
    EGLConfig c;
    EGLint n = 0, ma, mi;
    EGLSurface s;
    EGLContext x;
    if (d == EGL_NO_DISPLAY || !eglInitialize(d, &ma, &mi)) {
        d = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        if (!eglInitialize(d, &ma, &mi)) {
            return 0;
        }
    }
    if (!eglChooseConfig(d, ca, &c, 1, &n) || n < 1) {
        return 0;
    }
    s = eglCreatePbufferSurface(d, c, pa);
    eglBindAPI(EGL_OPENGL_API);
    x = eglCreateContext(d, c, EGL_NO_CONTEXT, NULL);
    if (s == EGL_NO_SURFACE || x == EGL_NO_CONTEXT || !eglMakeCurrent(d, s, s, x)) {
        return 0;
    }
    fprintf(stderr, "replay: GL %s\n", (const char *)glGetString(GL_RENDERER));
    return 1;
}
#endif
