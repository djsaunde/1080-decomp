/* PC port: the game-side PORT glue (docs/1080_PC_PORT_PLAN.md 9.6 and 17.4).
 *
 * The game code reaches these from `#ifdef PORT` blocks in src/; the matching build never
 * sees them.  New PORT functions name everything directly (no CALL() macros: those are
 * checked against the ROM relocations, and new functions have no relocations), and they reach
 * the bootup.uso globals through the port/uso blobs, not through D_00000000 placeholder tokens
 * (the port binds a placeholder per function from the ROM relocations, so a new function's
 * placeholder would bind to nothing).
 *
 * Plain C types only: this header is included by C and by the NCC C++ donors. */
#ifndef PORT_GAME_H
#define PORT_GAME_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- bootup.uso globals (port/uso blobs; offsets are the bootup Bss / Data offsets) ---- */
extern char port_bootup_bss[];
extern char port_bootup_data[];
#define PORT_GAMEFLOW (port_bootup_bss + 0xF8)      /* InputMgr / GameFlow (import_80020098) */
#define PORT_PADDEV (port_bootup_bss + 0x3A4E8)     /* PadDev gSysDevBlock_3A4E8 (gdl/pad.h) */
#define PORT_GFXLIB (port_bootup_bss + 0x3C168)     /* GfxLib gGfxLib (gdl/gfxlib.h) */
#define PORT_SYSLIB (port_bootup_bss + 0x3C740)     /* SysLib gSysLib (gdl/sysapi.h) */
#define PORT_RANDSTATE (port_bootup_data + 0x21E04) /* sys_randf state (plan 9.3) */

/* ---- the frame (game_libs_ncc_34EB4.cpp) ---- */
void port_game_boot(void);              /* boot_main_00066850 .. App_main -> app_runInit */
void app_runInit(void *sysLib, void *app); /* app_run up to its inner loop (App_main calls it) */
void app_runOneFrame(void);             /* one iteration of app_run's inner loop */
unsigned int port_game_frame_count(void); /* SysLib.frameCount */
unsigned int port_game_retrace_count(void); /* RcpMgr.retraceCount (virtual: 2 per frame) */

/* ---- the RCP manager thread, as direct calls (game_libs_ncc_4D688.cpp) ---- */
/* Handle every message pending on SysLib.msgQueue (the RcpMgr thread's queue) the way
 * RcpMgr_service does, without blocking.  Returns the number handled. */
int RcpMgr_portDrain(void *rcpMgr);
/* RcpMgr_update is the thread's entry: this is its port_ultra service step. */
int RcpMgr_portStep(void *rcpMgr);
/* A wait on the RCP queue that the drain cannot advance (port/main.c: logged, then fatal). */
void port_rcp_stuck(void *rcpMgr, const char *where);

/* ---- the pad thread, as direct calls (game_libs_post1b.c) ---- */
int pad_portDrain(char *padDev);        /* pad_threadLoop_00066EEC's switch, per pending message */
void pad_portFrame(void);               /* request + complete one controller read */

/* ---- the kernel's SysApi table and ROM file system (port/sysapi.c, port/fs) ---- */
/* Fill the port SysApi (memory range, usoLoad / usoUnload, file services, symLookup) and store it
 * in gSysApi (bootup Bss 0x416A0).  port_game_boot calls it after port_uso_init, before boot_main. */
void port_sysapi_install(void);

/* ---- host hooks the game calls (port/) ---- */
/* Once per VI retrace (the RcpMgr 'd' message): the audio driver frame.  Replaces RcpMgr_service's
 * `cur = snd_AudioUpdate()` and the audio task start.  main.c has a weak no-op; port/audio
 * (agent-c) defines the real one. */
void port_audio_frame(void);

/* trace hooks (port/trace.h) */
void port_trace_pad(int pad, unsigned int frameCount, int replayFrame, unsigned int *held,
                    signed char *rawX, signed char *rawY);
void port_trace_frame_end(unsigned int frameCount);

#ifdef __cplusplus
}
#endif

#endif /* PORT_GAME_H */
