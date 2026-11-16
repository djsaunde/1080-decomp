#ifndef PORT_ULTRA_INT_H
#define PORT_ULTRA_INT_H

/* port/ultra internal: state shared between the shim's files. */

#include "../port_libc.h"
#include "../host.h"
#include "ultra.h"

/* Define `name` as a second symbol for `target` (same TU only). The game's
 * matching build calls several libultra functions under game_libs copy names
 * (gl_osRecvMesg, gl_26214_osAiGetLength, ...); under PORT those calls must
 * land here. gcc / clang / mingw only. */
#define PORT_ALIAS(target, name) \
    extern __typeof__(target) name __attribute__((alias(#target)))

extern PortUltraConfig port_ultra_cfg;
extern PortUltraStats port_ultra_st;
extern u32 port_ultra_activity; /* bumped by every completed message / event / DMA */

/* Block until ready(ctx) holds: pump the services, then advance idle VI
 * retraces. Returns 0 when ready, -1 on deadlock (logged; fatal if configured). */
int port_ultra_wait(int (*ready)(void *ctx), void *ctx, const char *what, const void *obj);

/* time.c side of the retrace */
OSTime port_time_now(void);
void port_timers_fire(void);

/* per-file resets, called by port_ultra_init */
void port_ultra_threads_reset(void);
void port_ultra_events_reset(void);
void port_vi_reset(void);
void port_pi_reset(void);
void port_si_reset(void);
void port_time_reset(void);

#endif /* PORT_ULTRA_INT_H */
