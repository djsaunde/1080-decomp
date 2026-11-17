/*
 * port/audio/port_audio.h: the game-side glue of the CPU audio path
 * (docs/1080_PC_PORT_PLAN.md section 15.6). Port and PORT-guarded game code
 * only.
 */
#ifndef PORT_AUDIO_PORT_AUDIO_H
#define PORT_AUDIO_PORT_AUDIO_H

/* One VI retrace of audio, in place of the RcpMgr thread's 'd' handling
 * (RcpMgr_service: start last retrace's audio task, then snd_AudioUpdate):
 * runs AudioThread_UpdateImpl and, when it returns a task, runs that task's
 * command list through the mixer at once. The AI buffers it fills reach the
 * host through the game's own osAiSetNextBuffer -> host_audio_queue.
 * Call it once per retrace: 2 per 30 Hz step (section 10.2). The audio task
 * must NOT also go to osSpTaskStartGo (port_audio_rcp_task_is_audio tells the rcpTask hook to drop it). */
void port_audio_frame(void);

/* Run one Acmd list (bytes = 8 x commands) through the mixer. */
void port_audio_run_list(const void *cmds, unsigned int bytes);

/* 1 if p is a host pointer into the port image (the driver's K0 'relocated' tests, plan 15.8). */
int port_audio_is_ptr(unsigned int p);

/* debug: called by port_audio_frame after the driver's update, before the mixer runs its list */
extern void (*port_audio_premix_hook)(const void *cmds, unsigned int bytes);

/* PortUltraConfig.rcpTask helper: returns 1 for an audio task (counted and
 * dropped, port_audio_frame ran it), 0 for anything else. */
int port_audio_rcp_task_is_audio(const void *task);

/* AudioThread_UpdateImpl under PORT: gWaveSamples[8] (the "noise" wave) =
 * this + (audioRandom & 0xFFF0). On the N64 it points into game_libs code
 * (AudioThread_Update + 0..0xFFF0); this is a copy of those ROM bytes as
 * host-order s16 (or an LCG fill without a ROM). */
unsigned char *port_audio_noise_base(void);

/* AudioLoad_Init / AudioLoad_RelocateFont under PORT (audio_swap.c). */
void port_audio_swap_static(void);
void port_audio_swap_font(void *font, int numInstruments, int numDrums, int numSfx);

/* --audio-out (tenshoe): every AI buffer to a 16-bit stereo 32000 Hz WAV, with a DAC model for
 * osAiGetLength; the music mode changes are logged.  open returns -1 if the file cannot be made. */
int port_audio_capture_open(const char *path);
void port_audio_capture_close(void);

/* Diagnostics. */
typedef struct PortAudioStats {
    unsigned int updates;   /* port_audio_frame calls */
    unsigned int tasks;     /* audio tasks run */
    unsigned int cmds;      /* commands run */
    unsigned int badAddr;   /* mixer translate failures */
    unsigned int unknownOps;
    unsigned int unk19;
    unsigned int droppedSpTasks;
    unsigned int rawPointers; /* command addresses that were host pointers, not K0 - 0x80000000 */
} PortAudioStats;
extern PortAudioStats port_audio_stats;

#endif
