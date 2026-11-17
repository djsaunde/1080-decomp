/* port/gfx: the game side of the live presenter channel (port_live.c, live_shm.h,
 * PC port plan 19.11). All calls are no-ops unless PORT_LIVE=<name> is set. */
#ifndef PORT_GFX_PORT_LIVE_H
#define PORT_GFX_PORT_LIVE_H

#include <stddef.h>
#include "../host.h"

int port_live_active(void);                       /* PORT_LIVE set and the presenter attached */
int port_live_native(void);                       /* PORT_NATIVE: the in-process host (plan 21.9) */
void port_live_task(const void *dlmp, size_t size); /* append one task's DLMP image to the frame */
void port_live_frame_end(unsigned step);          /* publish the frame (end of a game step) */
int port_live_read_pads(HostPad pads[HOST_MAX_PADS]); /* 1 = pads filled from the presenter */
int port_live_audio_active(void);                 /* the presenter has an audio ring (plan 19.12) */
void port_live_audio_write(const void *samples, size_t bytes); /* one AI buffer into the ring */
int port_live_quit(void);                         /* the presenter asked the game to end */
void port_live_close(void);                       /* the game is ending */
/* after a gfx task's depth-clear model: store the presenter's newest depth / colour answers into
 * the images rq asks about (run 859; no-op without PORT_LIVE or a reply file) */
struct PortFbRequests;
void port_live_fb_apply(const struct PortFbRequests *rq);

#endif
