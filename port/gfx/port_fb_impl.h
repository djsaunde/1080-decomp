/* port/gfx: the renderer side of the framebuffer model (../include/port_fb.h, plan 19.10).
 *
 * port_gfx_task takes the pending requests for each gfx task (port_fb_requests), hands them to
 * the in-process renderer or records them in the dump, and then calls port_fb_task_done. */
#ifndef PORT_FB_IMPL_H
#define PORT_FB_IMPL_H

#ifdef __cplusplus
extern "C" {
#endif

#define PORT_FB_W 320
#define PORT_FB_H 240
#define PORT_FB_MAX_COLOR 4   /* colour images read at once (the main window's two buffers) */
#define PORT_FB_MAX_DEPTH 64  /* depth pixels per task (a Sun reads 9) */
#define PORT_FB_MAX_WRITE 64  /* CPU-written rectangles per task */
#define PORT_FB_KEEP_TASKS 4  /* a colour read stays wanted this many tasks after its last request */

typedef struct PortFbWrite {
    const void *img;
    int x, y, w, h;
} PortFbWrite;

typedef struct PortFbRequests {
    const void *color[PORT_FB_MAX_COLOR]; /* colour images to read back after the task */
    int nColor;
    const void *depthImg[PORT_FB_MAX_DEPTH]; /* depth pixels to read back after the task */
    int depthIdx[PORT_FB_MAX_DEPTH];
    int nDepth;
    PortFbWrite write[PORT_FB_MAX_WRITE]; /* CPU rectangles to composite after the task */
    int nWrite;
} PortFbRequests;

/* The requests in force for the task about to run (valid until port_fb_task_done). */
const PortFbRequests *port_fb_requests(void);

/* The task ran. imgs: the low 24 bits of every G_SETCIMG / G_SETZIMG address in it (n of them),
 * or n < 0 when unknown. Ages the colour reads; the one-shot depth reads and CPU writes are dropped
 * once a task that names their image has run (a 2D-only task leaves the Sun's depth reads for the
 * 3D task after it), or when n < 0. */
void port_fb_task_done(const unsigned int *imgs, int n);

/* Counters since boot (PORT_FB_LOG=1 prints them every 100 tasks): requests made by the game. */
typedef struct PortFbStats {
    unsigned long colorReads, depthReads, cpuWrites, textureWrites;
} PortFbStats;
const PortFbStats *port_fb_stats(void);

#ifdef __cplusplus
}
#endif
#endif
