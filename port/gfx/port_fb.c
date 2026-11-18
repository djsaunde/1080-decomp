/* port/gfx: the framebuffer model (../include/port_fb.h; PC port plan 11.2 and 19.10).
 *
 * The game's CPU readers and writers of the colour / depth images call the port_fb_* hooks
 * (#ifdef PORT sites listed in plan 19.10). This file only keeps the requests; port_gfx.c hands
 * them to the renderer for each gfx task: port_fast3d.cpp in process, or DLUMP_FB_* chunks in
 * the dump for f3dreplay. */
#include "../include/port_fb.h"
#include "port_fb_impl.h"
#ifdef PORT_GFX_FAST3D
#include "fast3d/port_fast3d.h"
#endif
#ifdef PORT_GFX_NATIVE
#include "port_live.h"
#include "port_native.h"
#endif

typedef struct ColorWant {
    const void *img;
    int ttl; /* tasks left */
} ColorWant;

static ColorWant sColor[PORT_FB_MAX_COLOR];
static PortFbRequests sReq;
static PortFbStats sStats;

void port_fb_read_color(const void *img) {
    int i, freeSlot = -1;

    if (img == 0) {
        return;
    }
    sStats.colorReads++;
    for (i = 0; i < PORT_FB_MAX_COLOR; i++) {
        if (sColor[i].img == img) {
            sColor[i].ttl = PORT_FB_KEEP_TASKS;
            return;
        }
        if (sColor[i].ttl <= 0 && freeSlot < 0) {
            freeSlot = i;
        }
    }
    if (freeSlot >= 0) {
        sColor[freeSlot].img = img;
        sColor[freeSlot].ttl = PORT_FB_KEEP_TASKS;
    }
}

void port_fb_read_depth(const void *zimg, int idx) {
    int i;

    if (zimg == 0 || idx < 0 || idx >= PORT_FB_W * PORT_FB_H) {
        return;
    }
    sStats.depthReads++;
    for (i = 0; i < sReq.nDepth; i++) {
        if (sReq.depthImg[i] == zimg && sReq.depthIdx[i] == idx) {
            return;
        }
    }
    if (sReq.nDepth < PORT_FB_MAX_DEPTH) {
        sReq.depthImg[sReq.nDepth] = zimg;
        sReq.depthIdx[sReq.nDepth] = idx;
        sReq.nDepth++;
    }
#ifdef PORT_GFX_NATIVE
    /* plan 22.4: the native game answers at once from the last frame's depth, still in Fast3D's
     * framebuffer, as the N64 reads the Z image the RDP finished. The asynchronous reply (a step
     * later, +-LIVE_FB_DEPTH_R around the pixel) missed a fast sun, whose pixels then read the
     * clear model's far value: the title demo's sun and flare showed behind the slope. */
    if (port_live_native()) {
        int z = port_native_depth((unsigned)(unsigned long)zimg, (unsigned)idx);
        if (z >= 0) {
            unsigned char *px = (unsigned char *)zimg + (unsigned)idx * 2;
            px[0] = (unsigned char)(z >> 8); /* big-endian, as the RDP writes it */
            px[1] = (unsigned char)z;
        }
    }
#endif
}

void port_fb_cpu_wrote(const void *img, int x, int y, int w, int h) {
    PortFbWrite *r;

    /* clip to the 320x240 image */
    if (x < 0) {
        w += x;
        x = 0;
    }
    if (y < 0) {
        h += y;
        y = 0;
    }
    if (x + w > PORT_FB_W) {
        w = PORT_FB_W - x;
    }
    if (y + h > PORT_FB_H) {
        h = PORT_FB_H - y;
    }
    if (img == 0 || w <= 0 || h <= 0) {
        return;
    }
    sStats.cpuWrites++;
    if (sReq.nWrite > 0) {
        /* merge into the previous rectangle of the same image (glyph runs, the splash's clear + blit) */
        r = &sReq.write[sReq.nWrite - 1];
        if (r->img == img) {
            int x0 = r->x < x ? r->x : x, y0 = r->y < y ? r->y : y;
            int x1 = r->x + r->w > x + w ? r->x + r->w : x + w, y1 = r->y + r->h > y + h ? r->y + r->h : y + h;
            r->x = x0;
            r->y = y0;
            r->w = x1 - x0;
            r->h = y1 - y0;
            return;
        }
    }
    if (sReq.nWrite < PORT_FB_MAX_WRITE) {
        r = &sReq.write[sReq.nWrite++];
        r->img = img;
        r->x = x;
        r->y = y;
        r->w = w;
        r->h = h;
    }
}

void port_fb_texture_written(const void *addr, unsigned int size) {
    sStats.textureWrites++;
#ifdef PORT_GFX_FAST3D
    port_fast3d_invalidate(addr, size); /* patch 0004 */
#elif defined(PORT_GFX_NATIVE)
    /* plan 22.3: the native game's Fast3D caches textures by address too; without this the logo
     * dissolve (Fader_buildTexture) and the TV screens kept their first upload */
    if (port_live_native()) {
        port_native_invalidate((unsigned)(unsigned long)addr, size);
    }
#else
    (void)addr; /* a dump is rendered from scratch: nothing is cached */
    (void)size;
#endif
}

const PortFbRequests *port_fb_requests(void) {
    int i;

    sReq.nColor = 0;
    for (i = 0; i < PORT_FB_MAX_COLOR; i++) {
        if (sColor[i].ttl > 0) {
            sReq.color[sReq.nColor++] = sColor[i].img;
        }
    }
    return &sReq;
}

static int named(const void *img, const unsigned int *imgs, int n) {
    int i;

    if (n < 0) {
        return 1;
    }
    for (i = 0; i < n; i++) {
        if (((unsigned int)(unsigned long)img & 0xFFFFFFu) == (imgs[i] & 0xFFFFFFu)) {
            return 1;
        }
    }
    return 0;
}

void port_fb_task_done(const unsigned int *imgs, int n) {
    int i, k;

    for (i = 0; i < PORT_FB_MAX_COLOR; i++) {
        if (sColor[i].ttl > 0) {
            sColor[i].ttl--;
        }
    }
    for (i = k = 0; i < sReq.nDepth; i++) {
        if (!named(sReq.depthImg[i], imgs, n)) {
            sReq.depthImg[k] = sReq.depthImg[i];
            sReq.depthIdx[k++] = sReq.depthIdx[i];
        }
    }
    sReq.nDepth = k;
    for (i = k = 0; i < sReq.nWrite; i++) {
        if (!named(sReq.write[i].img, imgs, n)) {
            sReq.write[k++] = sReq.write[i];
        }
    }
    sReq.nWrite = k;
}

const PortFbStats *port_fb_stats(void) {
    return &sStats;
}
