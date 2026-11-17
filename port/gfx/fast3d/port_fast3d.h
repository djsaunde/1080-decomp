/* port/gfx/fast3d: the in-process renderer (PC port plan 19.7, option (a)): libultraship's
 * Fast3D (pinned + the patch series) called from port_gfx_task when the port is built with
 * -DPORT_GFX_FAST3D=ON. C interface; port_fast3d.cpp is the C++ side. */
#ifndef PORT_FAST3D_H
#define PORT_FAST3D_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Create the GL context, the window backend and the interpreter (ucode F3DEX 1 + G_LINE3D,
 * native 320 x 240). width / height: the output size. 0 on success. */
int port_fast3d_init(unsigned int width, unsigned int height);

/* Render one M_GFXTASK: root = the task's data_ptr (the game address of the root list).
 * Returns the number of G_DL targets that could not be followed (64-bit hosts only; 0 on the
 * 32-bit game, which hands its lists over in place). */
int port_fast3d_task(unsigned int root);

/* port_fast3d_task, then the framebuffer model's requests for this task (../port_fb_impl.h, plan
 * 19.10): each colour image the CPU reads and the task drew into (low 24 bits of its last
 * G_SETCIMG) gets the frame, RGBA5551 big-endian at 320x240; each requested depth pixel of the
 * task's Z image gets its N64 Z, big-endian. rq may be NULL. */
struct PortFbRequests;
int port_fast3d_task_fb(unsigned int root, const struct PortFbRequests *rq);

/* The last task's colour and depth image addresses (as Fast3D holds them: G_SETCIMG / G_SETZIMG w1). */
void port_fast3d_last_images(unsigned int *cimg, unsigned int *zimg);

/* The CPU rewrote [addr, addr + size) (texels or a palette): drop the cached textures there (patch
 * 0004). The port's gfx_flushTexture / CPU texture writers call it (plan 11.2). */
void port_fast3d_invalidate(const void *addr, size_t size);

/* The last frame as RGBA8, top row first (width * height * 4 bytes): tests and screenshots. */
int port_fast3d_read_rgba(unsigned char *dst);

#ifdef __cplusplus
}
#endif
#endif
