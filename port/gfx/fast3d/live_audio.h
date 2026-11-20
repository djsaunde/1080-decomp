/* port/gfx/fast3d: the presenter's audio thread (PC port plan 19.12).
 *
 * The game (tenshoe, PORT_LIVE) appends its AI output, 32000 Hz stereo s16, to the audio ring
 * of the shared file (live_shm.h LiveAudio). This thread plays it through PulseAudio's simple
 * API (WSLg: $PULSE_SERVER = unix:/mnt/wslg/PulseServer), on its own, so a presenter that falls
 * behind on video never stalls the sound. libpulse-simple is dlopen'd: without it, without a
 * server, or with --mute, the ring is still drained (and hashed) and the video runs as before. */
#ifndef PORT_GFX_FAST3D_LIVE_AUDIO_H
#define PORT_GFX_FAST3D_LIVE_AUDIO_H

#include <cstdint>
#include <string>

extern "C" {
#include "../live_shm.h"
}

struct LiveAudioOptions {
    bool mute = false;
    uint32_t latencyMs = 80;   /* PulseAudio's target buffer (tlength) */
    std::string dumpPath;      /* every byte received from the ring, raw (tests) */
    /* PulseAudio is opened this long after live_audio_start (the ring is drained meanwhile). WSLg
     * (run 867): an X window mapped while a PulseAudio stream is playing stalls the whole server
     * (its RDP sink) for as long as the window lives, so the presenter's own window must be up
     * first. Measured: 1 s after the map stalls, 2 s does not. */
    uint32_t openDelayMs = 2500;
    bool stats = false;        /* --audio-stats: one machine-readable summary line at exit */
};

/* fill the LiveAudio block of a new shared file (before LiveShmHeader.magic is written) */
void live_audio_layout(LiveAudio* a, uint32_t dataOffset, uint32_t bytes);
/* start / stop the thread; base is the mapped file */
void live_audio_start(uint8_t* base, const LiveAudioOptions& o);
void live_audio_stop();
/* close the PulseAudio stream now and reopen it ms later (the ring is drained meanwhile): call it
 * before the window is mapped again (F11 fullscreen), which would otherwise stall WSLg's server */
void live_audio_suspend(uint32_t ms);
/* the output gain, 0..100 percent (the settings menu, plan 27): 100 (the default) writes the game's samples
 * unchanged; the dump (dumpPath) and the counters see the samples before it */
void live_audio_set_volume(int percent);
/* the counters, in seconds of audio (32000 Hz stereo s16) */
struct LiveAudioStats {
    double received;   /* everything taken from the ring */
    double beforeOpen; /* drained before the output was opened (openDelayMs) */
    double played;     /* written to PulseAudio */
    double trimmed;    /* skipped because the output fell behind */
};
LiveAudioStats live_audio_stats();
/* the `audio-stats:` summary (--audio-stats; port/tools/audio-check.sh parses it) */
std::string live_audio_stats_line();
/* one line for the presenter's per-second status */
std::string live_audio_status();
/* PORT_PERF (plan 33): the output's state for the per-second perf line. With perf on, the thread also asks
 * the output how much it holds (PulseAudio: a server round trip) about every 250 ms. dryMaxMs (the longest
 * time the ring stayed empty while playing; the game fills it every step, so above ~35 ms the game fell
 * behind the sound card) and putMaxMs (the longest write into the output: the
 * output stalled) count since the last call with reset. */
struct LiveAudioPerf {
    uint32_t ringMs = 0;   /* the game's output not yet taken from the ring */
    int32_t queuedMs = -1; /* the output's own buffer, -1 unknown */
    uint32_t underruns = 0, trimmedMs = 0, overruns = 0;
    double dryMaxMs = 0, putMaxMs = 0;
    const char* output = "none";
};
void live_audio_perf_enable(bool on);
LiveAudioPerf live_audio_perf(bool reset);

#endif
