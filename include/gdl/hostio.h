#ifndef GDL_HOSTIO_H
#define GDL_HOSTIO_H

/* The HostIO device object: gHostio (bootup.uso Bss+0x3C768, Sym 338, 8 bytes; include/gdl/gl_data.h),
 * the first argument of every hostio_* call (the 47 <Class>_hostioParms functions, the file
 * services, the dispatcher). Most hostio_* message builders ignore it (homed, unused); the
 * connection state lives in its two words (agent-b run 560):
 *   - hostio_init (0x3EDBC) sends / receives the type-0 handshake record and sets flags bit 0;
 *   - hostio_connect (0x3FC2C) sends message 0x21 and sets connected = 1
 *     (hostio_start and hostio_dispatch's case 22 call it);
 *   - hostio_reset (0x3ED8C) clears connected and flags bit 0, then re-runs hostio_init.
 * app_staticInit_000412E8 (game_libs_ncc_412E8.cpp, gHostio_412E8_m) zeroes the first word.
 * Plain C types only, so the NCC donors can include it. */
typedef struct HostioDev {
    /* 0x0 */ s32 flags;           /* HOSTIO_READY */
    /* 0x4 */ s32 connected;       /* 1 after hostio_connect, 0 after hostio_reset */
} HostioDev;

#define HOSTIO_READY 1             /* flags: the hostio_init handshake succeeded */

#define HOSTIODEV(p) ((HostioDev *)(p))

#ifndef __cplusplus
typedef char HostioDev_size_check[sizeof(HostioDev) == 8 ? 1 : -1];
#endif

#endif /* GDL_HOSTIO_H */
