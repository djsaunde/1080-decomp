/* port_fb.h: the framebuffer model of the PC port (docs/1080_PC_PORT_PLAN.md 11.2 and 19.10).
 *
 * On the N64 the RDP draws into RDRAM, so the CPU can read the colour and depth images and write
 * pixels into them. On the port Fast3D draws on the GPU and those images in game memory are never
 * written. The model: **the image in game memory stays the place the game reads and writes**, in
 * the N64 format (RGBA5551 / 14.2 Z, big-endian 16-bit, 320 wide), and the port keeps it in step
 * with the GPU on demand:
 *
 *   - A CPU reader declares what it reads, every frame it reads it. After the next gfx task that
 *     renders into that image, the renderer reads the frame back into the image (patch 0003:
 *     native 320x240, big-endian), so the reader sees the last completed frame one frame later,
 *     as it does on the N64 (it reads the buffer that is on screen).
 *   - Depth is read per pixel (Sun_measureVisibility samples 9 points): each requested pixel is
 *     read back and stored at its place in the Z image. With no renderer in process (headless,
 *     PORT_GFX_DUMP) port_gfx.c applies the game's Z clear (its G_FILLRECT of 0xFFFC on the Z
 *     image) to the image in memory instead, so far pixels read 0xFFFC; geometry is not modelled
 *     (the sun counts as unoccluded). PORT_FB_ZCLEAR=0 turns that off.
 *   - A CPU writer declares the rectangle it wrote. After the next task that renders into that
 *     image the port composites the rectangle (opaque) over the frame.
 *   - A CPU texture writer declares the texels it rewrote, so the texture cache drops them
 *     (patch 0004).
 *
 * In-process (PORT_GFX_FAST3D) the renderer does this at the end of each task. With
 * PORT_GFX_DUMP (option b) the requests are recorded in the dump instead (DLUMP_FB_* chunks,
 * port/gfx/dlump.h) and port/gfx/fast3d/f3dreplay carries them out.
 *
 * Images are matched to a task by the low 24 bits of the address: gfx_setColorImage /
 * gfx_setDepthImage mask G_SETCIMG / G_SETZIMG to 24 bits, and the hooks get the full pointer.
 * The hooks are cheap (a table of a few entries) and do nothing when no renderer or dump runs. */
#ifndef PORT_FB_H
#define PORT_FB_H

#ifdef __cplusplus
extern "C" {
#endif

/* The CPU reads the colour image at img (320x240 RGBA5551) this frame. */
void port_fb_read_color(const void *img);

/* The CPU reads pixel idx (y * 320 + x) of the depth image at zimg this frame. */
void port_fb_read_depth(const void *zimg, int idx);

/* The CPU wrote the w x h rectangle at (x, y) of the colour image at img (320 wide). */
void port_fb_cpu_wrote(const void *img, int x, int y, int w, int h);

/* The CPU rewrote [addr, addr + size) of texels or a palette. */
void port_fb_texture_written(const void *addr, unsigned int size);

#ifdef __cplusplus
}
#endif
#endif
