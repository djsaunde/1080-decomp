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
};

/* fill the LiveAudio block of a new shared file (before LiveShmHeader.magic is written) */
void live_audio_layout(LiveAudio* a, uint32_t dataOffset, uint32_t bytes);
/* start / stop the thread; base is the mapped file */
void live_audio_start(uint8_t* base, const LiveAudioOptions& o);
void live_audio_stop();
/* one line for the presenter's per-second status */
std::string live_audio_status();

#endif
