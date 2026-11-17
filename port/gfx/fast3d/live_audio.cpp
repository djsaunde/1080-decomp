/* port/gfx/fast3d: the presenter's audio thread (live_audio.h, PC port plan 19.12).
 *
 * The loop: take what the game has published in the ring (at most 10 ms at a time), pass it to
 * pa_simple_write, which blocks while PulseAudio's buffer holds latencyMs; so the thread runs at
 * the sound card's pace, the ring stays nearly empty, and the end-to-end delay is about
 * latencyMs plus the game's own one-retrace AI queue. Two guards keep it bounded:
 *   - the ring ran dry (the game stalled, a load): nothing is written, PulseAudio plays silence
 *     and waits for its prebuffer again (counted in LiveAudio.underruns when PulseAudio itself
 *     is down to its last 5 ms, i.e. an audible gap);
 *   - the ring holds more than latencyMs + 100 ms (the sound card's clock is slower than the
 *     game's wall-clock DAC model, or the thread was descheduled): the excess beyond 20 ms is
 *     skipped (LiveAudio.trimmed), so the audio never trails the picture by more than that.
 *     In practice this fires once, at boot, where the game's first retraces come in a burst.
 * Muted, or with no PulseAudio, the ring is drained as it fills. Every byte taken from the ring
 * (played, skipped or drained) goes into an FNV-1a 64 hash and the optional --audio-dump file,
 * the same hash port_live.c prints for every byte it wrote: equal hashes = the presenter got
 * exactly the game's PCM. */
#include "live_audio.h"

#include <dlfcn.h>
#include <time.h>
#include <unistd.h>

#include <algorithm>
#include <atomic>
#include <cstdio>
#include <cstring>
#include <thread>
#include <vector>

#if __has_include(<pulse/simple.h>)
#include <pulse/error.h>
#include <pulse/simple.h>
#define LIVE_HAVE_PULSE 1
#else
#define LIVE_HAVE_PULSE 0
#endif

namespace {

LiveAudio* sA;
uint8_t* sData;
LiveAudioOptions sOpt;
std::thread sThread;
std::atomic<bool> sStop{ false };
std::atomic<uint64_t> sReceived{ 0 }, sPlayed{ 0 };
uint64_t sHash = 0xcbf29ce484222325ull;
FILE* sDump;
std::atomic<const char*> sOutput{ "none" };

#if LIVE_HAVE_PULSE
struct Pulse {
    void* lib = nullptr;
    void* libPulse = nullptr;
    decltype(&pa_simple_new) newFn = nullptr;
    decltype(&pa_simple_write) write = nullptr;
    decltype(&pa_simple_drain) drain = nullptr;
    decltype(&pa_simple_free) free = nullptr;
    decltype(&pa_simple_get_latency) latency = nullptr;
    decltype(&pa_strerror) strerror = nullptr;
    pa_simple* s = nullptr;

    bool open(uint32_t rate, uint32_t channels, uint32_t latencyMs) {
        lib = dlopen("libpulse-simple.so.0", RTLD_NOW);
        if (lib != nullptr) {
            libPulse = dlopen("libpulse.so.0", RTLD_NOW);
        }
        if (lib == nullptr || libPulse == nullptr) {
            std::printf("presenter: audio: %s; silent (port/gfx/fast3d/setup-pulse-dev.sh, and play.sh's "
                        "LD_LIBRARY_PATH)\n", dlerror());
            return false;
        }
        newFn = (decltype(newFn))dlsym(lib, "pa_simple_new");
        write = (decltype(write))dlsym(lib, "pa_simple_write");
        drain = (decltype(drain))dlsym(lib, "pa_simple_drain");
        free = (decltype(free))dlsym(lib, "pa_simple_free");
        latency = (decltype(latency))dlsym(lib, "pa_simple_get_latency");
        strerror = (decltype(strerror))dlsym(libPulse, "pa_strerror");
        if (!newFn || !write || !drain || !free || !latency || !strerror) {
            std::printf("presenter: audio: libpulse-simple lacks the pa_simple API; silent\n");
            return false;
        }
        pa_sample_spec ss;
        ss.format = PA_SAMPLE_S16LE;
        ss.rate = rate;
        ss.channels = (uint8_t)channels;
        pa_buffer_attr ba;
        ba.maxlength = (uint32_t)-1;
        ba.tlength = (uint32_t)((uint64_t)rate * channels * 2 * latencyMs / 1000);
        ba.prebuf = (uint32_t)-1; /* = tlength: start (and restart after a dry spell) with a full buffer */
        ba.minreq = (uint32_t)-1;
        ba.fragsize = (uint32_t)-1;
        /* $PULSE_SERVER, else WSLg's socket, else libpulse's default search */
        const char* server = getenv("PULSE_SERVER");
        if ((server == nullptr || *server == '\0') && access("/mnt/wslg/PulseServer", F_OK) == 0) {
            server = "unix:/mnt/wslg/PulseServer";
        }
        int err = 0;
        s = newFn(server, "1080 Snowboarding", PA_STREAM_PLAYBACK, nullptr, "game", &ss, nullptr, &ba, &err);
        if (s == nullptr) {
            std::printf("presenter: audio: pa_simple_new(%s) failed: %s; silent\n", server ? server : "default",
                        strerror(err));
            return false;
        }
        std::printf("presenter: audio: PulseAudio %s, %u Hz x %u, %u ms buffer\n", server ? server : "(default)", rate,
                    channels, latencyMs);
        return true;
    }
    /* microseconds PulseAudio still holds (its buffer + the sink's), -1 if unknown */
    long queued() {
        int err = 0;
        pa_usec_t l = latency(s, &err);
        return err != 0 ? -1 : (long)l;
    }
    bool put(const uint8_t* p, size_t n) {
        int err = 0;
        if (write(s, p, n, &err) < 0) {
            std::printf("presenter: audio: pa_simple_write failed: %s; silent from now on\n", strerror(err));
            free(s);
            s = nullptr;
            return false;
        }
        return true;
    }
    void close() {
        if (s != nullptr) {
            int err = 0;
            drain(s, &err);
            free(s);
            s = nullptr;
        }
    }
};
#else
struct Pulse {
    void* s = nullptr;
    bool open(uint32_t, uint32_t, uint32_t) {
        std::printf("presenter: audio: built without <pulse/simple.h> (port/gfx/fast3d/setup-pulse-dev.sh); silent\n");
        return false;
    }
    long queued() { return -1; }
    bool put(const uint8_t*, size_t) { return false; }
    void close() {}
};
#endif

void sleep_ms(int ms) {
    struct timespec ts = { ms / 1000, (long)(ms % 1000) * 1000000L };
    nanosleep(&ts, nullptr);
}

double now_s() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + ts.tv_nsec * 1e-9;
}

/* n bytes from the ring at tail into out (or nowhere), hashed and dumped, then released */
void take(uint32_t tail, uint32_t n, uint8_t* out) {
    const uint32_t size = sA->bytes;
    for (uint32_t i = 0; i < n; i++) {
        uint8_t b = sData[(tail + i) & (size - 1)];
        sHash = (sHash ^ b) * 0x100000001b3ull;
        if (out != nullptr) {
            out[i] = b;
        }
    }
    if (sDump != nullptr) {
        uint32_t off = tail & (size - 1), first = std::min(n, size - off);
        std::fwrite(sData + off, 1, first, sDump);
        std::fwrite(sData, 1, n - first, sDump);
    }
    sReceived += n;
    __atomic_store_n(&sA->tail, tail + n, __ATOMIC_RELEASE);
}

void run() {
    Pulse pa;
    bool playing = false;
    if (sOpt.mute) {
        sA->state = LIVE_AUDIO_MUTED;
        sOutput = "muted";
        std::printf("presenter: audio: muted\n");
    } else if (pa.open(sA->rate, sA->channels, sOpt.latencyMs)) {
        sA->state = LIVE_AUDIO_PULSE;
        sOutput = "pulse";
        playing = true;
    } else {
        sA->state = LIVE_AUDIO_OFF;
        sOutput = "none";
    }
    std::fflush(stdout);
    const uint32_t frameBytes = sA->channels * 2;
    const uint32_t bytesPerMs = sA->rate * frameBytes / 1000;
    const uint32_t chunk = 10 * bytesPerMs;
    const uint32_t trimAbove = (sOpt.latencyMs + 100) * bytesPerMs, trimTo = 20 * bytesPerMs;
    std::vector<uint8_t> buf(chunk);
    double dryStart = 0;
    bool counted = false;
    uint32_t trims = 0;
    while (!sStop.load()) {
        uint32_t head = __atomic_load_n(&sA->head, __ATOMIC_ACQUIRE), tail = sA->tail, avail = head - tail;
        if (!playing) { /* muted / no output: drain as it fills */
            if (avail != 0) {
                take(tail, avail, nullptr);
            }
            sleep_ms(5);
            continue;
        }
        if (avail == 0) {
            double t = now_s();
            if (dryStart == 0) {
                dryStart = t;
                counted = false;
            } else if (!counted && pa.queued() >= 0 && pa.queued() < 5000) {
                /* the ring is dry and PulseAudio is about to run out too: an audible gap */
                __atomic_fetch_add(&sA->underruns, 1u, __ATOMIC_RELAXED);
                counted = true;
                if (sA->underruns <= 5) {
                    std::printf("presenter: audio: underrun at %.2f s received (dry for %.0f ms)\n",
                                sReceived.load() / (4.0 * 32000.0), (t - dryStart) * 1000);
                }
            }
            sleep_ms(2);
            continue;
        }
        dryStart = 0;
        if (avail > trimAbove) {
            uint32_t skip = (avail - trimTo) / frameBytes * frameBytes;
            take(tail, skip, nullptr);
            __atomic_fetch_add(&sA->trimmed, skip, __ATOMIC_RELAXED);
            if (++trims <= 5) {
                std::printf("presenter: audio: %u ms behind at %.2f s received: skipped %u ms\n", avail / bytesPerMs,
                            sReceived.load() / (4.0 * 32000.0), skip / bytesPerMs);
            }
            continue;
        }
        uint32_t n = std::min(avail, chunk) / frameBytes * frameBytes;
        if (n == 0) {
            sleep_ms(1);
            continue;
        }
        take(tail, n, buf.data());
        if (pa.put(buf.data(), n)) {
            sPlayed += n;
        } else {
            playing = false;
            sA->state = LIVE_AUDIO_OFF;
            sOutput = "none (write failed)";
        }
    }
    /* the rest of the ring (the game's last buffers), for the hash and the dump */
    uint32_t head = __atomic_load_n(&sA->head, __ATOMIC_ACQUIRE);
    if (head != sA->tail) {
        take(sA->tail, head - sA->tail, nullptr);
    }
    pa.close();
}

} // namespace

void live_audio_layout(LiveAudio* a, uint32_t dataOffset, uint32_t bytes) {
    std::memset(a, 0, sizeof(*a));
    a->rate = 32000;
    a->channels = 2;
    a->bytes = bytes;
    a->dataOffset = dataOffset;
    a->magic = LIVE_AUDIO_MAGIC;
}

void live_audio_start(uint8_t* base, const LiveAudioOptions& o) {
    sA = (LiveAudio*)(base + LIVE_AUDIO_HDR);
    sData = base + sA->dataOffset;
    sOpt = o;
    if (!o.dumpPath.empty()) {
        sDump = std::fopen(o.dumpPath.c_str(), "wb");
        if (sDump == nullptr) {
            std::printf("presenter: audio: cannot write %s\n", o.dumpPath.c_str());
        }
    }
    sStop = false;
    sThread = std::thread(run);
}

void live_audio_stop() {
    if (!sThread.joinable()) {
        return;
    }
    sStop = true;
    sThread.join();
    if (sDump != nullptr) {
        std::fclose(sDump);
        sDump = nullptr;
    }
    std::printf("presenter: audio: %llu bytes (%.2f s) received, fnv1a64 %016llx; %.2f s played (%s); "
                "%u underruns, %u bytes trimmed, %u game overruns\n",
                (unsigned long long)sReceived.load(), sReceived.load() / (4.0 * 32000.0), (unsigned long long)sHash,
                sPlayed.load() / (4.0 * 32000.0), sOutput.load(), sA->underruns, sA->trimmed, sA->overruns);
}

std::string live_audio_status() {
    if (sA == nullptr) {
        return "";
    }
    char s[160];
    uint32_t backlog = __atomic_load_n(&sA->head, __ATOMIC_ACQUIRE) - sA->tail;
    std::snprintf(s, sizeof(s), ", audio %s %.1f s, ring %u ms, underruns %u, trimmed %u", sOutput.load(),
                  sReceived.load() / (4.0 * 32000.0), backlog / 128, sA->underruns, sA->trimmed);
    return s;
}
