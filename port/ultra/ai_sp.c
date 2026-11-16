/* port/ultra: AI (audio DAC) and SP (RSP task) entry points.
 *
 * AI: the frequency maths is libultra's (2.0I aisetfreq.c), so the driver sees
 * the same "actual" rate as on hardware (32000 Hz requested -> 32006 returned).
 * Buffers go to the host audio stream (host_audio_queue); port/audio replaces
 * the RSP synthesis task in M4 (plan decision 3). osAiGetLength reports the
 * host's buffered bytes; the headless host reports 0, which keeps the driver's
 * per-frame sample counts deterministic.
 *
 * SP: osSpTaskStartGo hands the task to PortUltraConfig.rcpTask (Fast3D for
 * M_GFXTASK, the CPU audio mixer for M_AUDTASK), which runs it to completion
 * synchronously; the shim then posts OS_EVENT_SP (and OS_EVENT_DP for a
 * graphics task) as the RSP / RDP interrupts would. Nothing ever yields. */

#include "ultra_int.h"

#define AI_MIN_DAC_RATE 132
#define AI_MAX_BIT_RATE 16

static u32 sAiFrequency;
static OSTask_t *sLoadedTask;

s32 osAiSetFrequency(u32 frequency) {
    f32 f;
    u32 dacRate;
    u8 bitRate;

    f = (f32)osViClock / (f32)frequency + 0.5f;
    dacRate = (u32)f;
    if (dacRate < AI_MIN_DAC_RATE) {
        return -1;
    }
    bitRate = (u8)(dacRate / 66);
    if (bitRate > AI_MAX_BIT_RATE) {
        bitRate = AI_MAX_BIT_RATE;
    }
    (void)bitRate;
    sAiFrequency = (u32)(osViClock / (s32)dacRate);
    host_audio_set_frequency(sAiFrequency);
    return (s32)sAiFrequency;
}

s32 osAiSetNextBuffer(void *buf, u32 size) {
    host_audio_queue(buf, size);
    return 0;
}

u32 osAiGetLength(void) {
    return host_audio_buffered_bytes();
}

u32 osAiGetStatus(void) {
    return 0; /* never full, never busy */
}

void osSpTaskLoad(OSTask_t *task) {
    sLoadedTask = task;
}

void osSpTaskStartGo(OSTask_t *task) {
    if (task == NULL) {
        task = sLoadedTask;
    }
    port_ultra_st.spTasks++;
    port_ultra_activity++;
    if (port_ultra_cfg.rcpTask != NULL && task != NULL) {
        port_ultra_cfg.rcpTask(task);
    }
    port_ultra_post_event(OS_EVENT_SP);
    if (task != NULL && task->type == M_GFXTASK) {
        port_ultra_post_event(OS_EVENT_DP);
    }
}

void osSpTaskYield(void) {
}

u32 osSpTaskYielded(OSTask_t *task) {
    (void)task;
    return 0;
}

PORT_ALIAS(osAiGetLength, gl_osAiGetLength);
/* AudioThread_Update's private names (src/game_libs/game_libs_o2_26214.c) */
PORT_ALIAS(osAiGetLength, gl_26214_osAiGetLength);
PORT_ALIAS(osAiSetNextBuffer, gl_26214_osAiSetNextBuffer);
PORT_ALIAS(osSpTaskStartGo, gl_osSpTaskStartGo);
PORT_ALIAS(osSpTaskYield, gl_osSpTaskYield);
PORT_ALIAS(osSpTaskYielded, gl_osSpTaskYielded);
