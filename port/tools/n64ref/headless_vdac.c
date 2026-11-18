/* Headless replacement for angrylion-rdp-plus' output/vdac.c and plugin/mupen64plus/screen.c
 * (PC port plan 22): no GL, no window. vdac_write keeps a copy of the last VI frame; the
 * frontend (n64ref.c) reads it through n64ref_last_frame (dlsym), ReadScreen2 gets RGB. */
#include "core/n64video.h"
#include "output/vdac.h"
#include "output/screen.h"
#include <stdlib.h>
#include <string.h>

static struct n64video_pixel *g_pix;
static uint32_t g_w, g_h, g_hout;
static unsigned long g_count;
int32_t win_width = 640, win_height = 480, win_fullscreen = 0;

void vdac_init(struct n64video_config *config) { (void)config; }
void vdac_close(void) { free(g_pix); g_pix = NULL; g_w = g_h = 0; }
static void (*g_hook)(void);
void n64ref_set_vi_hook(void (*fn)(void)) { g_hook = fn; }
/* once per VI (UpdateScreen), after vdac_write when the VI had a frame */
void vdac_sync(bool valid) { (void)valid; if (g_hook) g_hook(); }

void vdac_write(struct n64video_frame_buffer *fb)
{
    if (fb->width != g_w || fb->height != g_h) {
        free(g_pix);
        g_pix = malloc((size_t)fb->width * fb->height * sizeof *g_pix);
        g_w = fb->width; g_h = fb->height;
    }
    for (uint32_t y = 0; y < fb->height; y++)
        memcpy(g_pix + (size_t)y * g_w, fb->pixels + (size_t)y * fb->pitch, g_w * sizeof *g_pix);
    g_hout = fb->height_out;
    g_count++;
}

/* RGB, width x height_out (rows repeated when the VI output is taller than the buffer) */
void vdac_read(struct n64video_frame_buffer *fb, bool alpha)
{
    fb->width = g_w; fb->height = fb->height_out = g_hout ? g_hout : g_h; fb->pitch = g_w;
    if (!fb->pixels || !g_pix) return;
    uint8_t *d = (uint8_t *)fb->pixels;
    for (uint32_t y = 0; y < fb->height; y++) {
        const struct n64video_pixel *s = g_pix + (size_t)(y * g_h / fb->height) * g_w;
        for (uint32_t x = 0; x < g_w; x++) {
            *d++ = s[x].r; *d++ = s[x].g; *d++ = s[x].b;
            if (alpha) *d++ = 0xFF;
        }
    }
}

/* raw access for the frontend: returns the VI frame count so far */
unsigned long n64ref_last_frame(const struct n64video_pixel **pix, uint32_t *w, uint32_t *h, uint32_t *hout)
{
    *pix = g_pix; *w = g_w; *h = g_h; *hout = g_hout;
    return g_count;
}

void screen_init(struct n64video_config *config) { (void)config; }
void screen_adjust(int32_t wo, int32_t ho, int32_t *w, int32_t *h, int32_t *x, int32_t *y)
{ *w = wo; *h = ho; *x = 0; *y = 0; }
void screen_update(void) {}
void screen_toggle_fullscreen(void) {}
void screen_close(void) {}
