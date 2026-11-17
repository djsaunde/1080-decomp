/* port/gfx: the native 64-bit game's in-process host (plan 21.9): window, input, audio and the Fast3D
 * frame in the game's own process, with no shared memory and no presenter.
 *
 * The boundary: the game and port_live.c / port_gfx.c are x32 (-mx32, 4-byte pointers, plan 21.2);
 * fast3d/native_host.cpp and everything under it (Fast3D, Xlib, EGL, libpulse) are ordinary LP64. So
 * these entry points take and return only 32-bit integers (a game address is a u32: every one is below
 * 2 GB), and the memory both sides share is live_shm.h's 4-byte-field layout: one private, low block
 * holding a LiveShmHeader (pads, quit), the LiveAudio ring and a LiveFbReply, which port_live.c reads
 * exactly as it reads the presenter's shared files. Only built with -DPORT_NATIVE=ON (PORT_GFX_NATIVE). */
#ifndef PORT_GFX_PORT_NATIVE_H
#define PORT_GFX_PORT_NATIVE_H

#include "live_shm.h"

/* the block: header page (LiveShmHeader, LiveAudio at LIVE_AUDIO_HDR), the audio data, the reply */
#define PORT_NATIVE_AUDIO_DATA LIVE_HEADER_BYTES
#define PORT_NATIVE_FB_REPLY (LIVE_HEADER_BYTES + LIVE_AUDIO_DEFAULT_BYTES)

#ifdef __cplusplus
extern "C" {
#endif

/* open the window (env: PORT_NATIVE_SCALE, _VSYNC, _MUTE, _LATENCY, ...; native_host.cpp), start the audio
 * thread; returns the block's address, 0 when there is no window (the message says why) */
unsigned port_native_open(void);
/* one gfx task: root = its data_ptr, rq = the game's PortFbRequests (x32 layout), imgs = a u32[maxImgs]
 * that receives the low 24 bits of its G_SETCIMG / G_SETZIMG. Returns the number of images. */
int port_native_task(unsigned root, unsigned rq, unsigned imgs, int maxImgs);
/* the step's end: draw its tasks as one frame, answer the read requests, present, pump the window */
void port_native_frame(unsigned step);
/* pump the window's events into the pads; 1 = quit (Esc, the window closed) */
int port_native_poll(void);
/* the game is ending: stop the audio, print the frame-time summary */
void port_native_close(void);

#ifdef __cplusplus
}
#endif
#endif
