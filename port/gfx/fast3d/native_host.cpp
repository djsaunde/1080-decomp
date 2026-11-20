/* port/gfx/fast3d: the native 64-bit game's in-process host (plan 21.9; ../port_native.h).
 *
 * The 64-bit tenshoe built with -DPORT_NATIVE=ON and run with PORT_NATIVE=1 opens its own window and
 * renders, plays and reads input itself: no shared memory, no presenter process.
 *   - window / input: live_window.cpp (the presenter's X11 + EGL window and keyboard / gamepad code);
 *   - frames: port_fast3d.cpp's step compositing (port_fast3d_native.h) into the window, presented once
 *     per game step;
 *   - audio: live_audio.cpp's thread, unchanged, over a LiveAudio ring in this process's block;
 *   - read-backs: a LiveFbReply in the block, which port_live.c takes as it takes the presenter's.
 *
 * Environment (port/tools/play.sh --native sets them from its options):
 *   PORT_NATIVE=1             use this host (port_live.c)
 *   PORT_NATIVE_SCALE=N       window = N x 320x240 (default 3); the window resizes, F11 = fullscreen
 *   PORT_NATIVE_VSYNC=1       swap interval 1 (default 0: the game's 30 Hz step paces the frames)
 *   PORT_NATIVE_MUTE=1, PORT_NATIVE_LATENCY=MS (default 80), PORT_NATIVE_AUDIO_DUMP=FILE
 *   PORT_NATIVE_AUDIO_STATS=1 the `audio-stats:` summary line at exit (port/tools/audio-check.sh --native)
 *   PORT_NATIVE_AUDIO_OPEN_DELAY=MS  open PulseAudio this long after the window (default 2500; WSLg, run 867)
 *   PORT_NATIVE_PNG=DIR       DIR/native_sSTEP.png after each frame (PORT_NATIVE_PNG_STEPS=A-B,C,... limits it)
 *   PORT_NATIVE_INJECT=STEP:KEY:d|u,...   synthetic X keys (tests), as presenter --inject
 *   PORT_NATIVE_PAD_INJECT=STEP:PAD:INPUT:VALUE,...  virtual gamepads (tests, plan 27): from STEP, gamepad PAD
 *                             (0..3) is a virtual one with INPUT (a button name of port/input_map.h: a, start,
 *                             lb ... = VALUE 1 / 0; or an axis: lx ly lt rx ry rt hatx haty = VALUE -32767..32767)
 *   PORT_SETTINGS=FILE        the settings file (port/settings.h; default ./1080.ini); the variables above win
 *   PORT_NATIVE_MENU=0        no settings menu (F1)
 *   PORT_NATIVE_JS=PATH       the joystick (default /dev/input/js0, else the first evdev gamepad)
 *   PORT_NATIVE_FB_LOG=1      the read-back answers per frame
 *   PORT_NATIVE_SIZE=WxH      the window's size (default 320x240 x scale), e.g. 1280x720
 *   PORT_NATIVE_PNG_INTERP=1  with PORT_NATIVE_PNG and an fps above 30: also DIR/native_sSTEP_fK.png, the
 *                             step's interpolated frames (K = 1 .. frames per step - 1)
 *   PORT_INTERP_LOG=1         the camera change per step (cut detection of the fps interpolation)
 * The M7 rendering extras (port/settings.h [graphics]: widescreen, hud_edges, resolution, msaa, fps,
 * scale_filter; PORT_WIDESCREEN, PORT_HUD_EDGES, PORT_INTERNAL_RES, PORT_MSAA, PORT_FPS, PORT_SCALE_FILTER) are
 * read every frame (plan 28).
 * Keys: live_window.h, rebindable in the settings file / menu (plan 27). At exit: frames, render ms mean / p50 / p95 / p99 / max, frames over 33 ms, the
 * step interval, audio. */
#include <chrono>
#ifdef PORT_WIN32
#include "../../win32/win32_gl.h" /* windows.h (VirtualAlloc) and the GL entry points (plan 29) */
#else
#include <sys/mman.h>
#include <unistd.h>
#endif

#include <algorithm>
#include <atomic>
#include <cmath>
#include <thread>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

#include "port_gl.h"

#include "live_audio.h"
#include "live_backend.h"
#include "live_window.h"
#include "menu_overlay_gl.h"
#include "port_fast3d.h"
#include "port_fast3d_n64.h"
#include "settings_menu.h"
#include "port_fast3d_native.h"
extern "C" {
#include "../port_fb_impl.h"
#include "../port_native.h"
#include "../replay/egl_png.h"
#include "../../settings.h"
}

namespace {

/* PortFbRequests as the x32 game lays it out (every field 4 bytes) */
struct FbRequestsX32 {
    uint32_t color[PORT_FB_MAX_COLOR];
    int32_t nColor;
    uint32_t depthImg[PORT_FB_MAX_DEPTH];
    int32_t depthIdx[PORT_FB_MAX_DEPTH];
    int32_t nDepth;
    struct {
        uint32_t img;
        int32_t x, y, w, h;
    } write[PORT_FB_MAX_WRITE];
    int32_t nWrite;
};

struct Inject {
    uint32_t step;
    std::string key;
    bool down, sent;
};

/* PORT_NATIVE_PAD_INJECT: a virtual gamepad's input from a step on */
struct PadInject {
    uint32_t step;
    int pad;
    std::string input;
    int value;
    bool sent;
};

/* PORT_PERF=1 / [debug] perf = 1 / F3 (plan 33): where the main loop's time goes, once a second on stderr (1080.log
 * on macOS, Windows and Android), and every loop iteration over 50 ms with its breakdown. The host sees the
 * loop through its entry points: port_native_poll starts an iteration (host_poll_events), the game simulates
 * until port_native_frame (render + present; once per step, several when the loop catches up), then
 * port_native_idle and the loop's sleep until the next poll. */
struct PerfAcc {
    double sim = 0, queue = 0, depthNow = 0, render = 0, rb = 0, present = 0, idle = 0, events = 0, shader = 0;
    double simMax = 0, renderMax = 0, presentMax = 0, iterMax = 0, busyMax = 0;
    uint32_t steps = 0, presented = 0, depthNowN = 0, rbColor = 0, rbDepth = 0, iters = 0, spikes = 0, catchUp = 0;
    uint32_t shaders = 0, skipped = 0;
};
struct Perf {
    bool on = false;
    double iterStart = 0;   /* this iteration's port_native_poll */
    double gameSince = 0;   /* control went back to the game (simulation runs from here) */
    double idleSince = 0;   /* port_native_idle: the loop's wait began */
    double secStart = 0, startT = 0;
    uint32_t flow = 0, step = 0, iterSteps = 0;
    PerfAcc it, sec;
    PortFast3dReadbacks rb0;
    uint32_t underruns0 = 0, trimmed0 = 0, overruns0 = 0;
    uint64_t totalSteps = 0, totalSpikes = 0;
    double worstIter = 0;
    uint32_t worstStep = 0;
};
Perf sPerf;

struct Host {
    bool black = false; /* osViBlack (port_native_black) */
    uint8_t* block = nullptr;
    size_t blockBytes = 0;
    LiveShmHeader* H = nullptr;
    LiveFbReply* reply = nullptr;
    LiveWin win;
    LiveWindow* wapi = nullptr;
    LiveInput in;
    LivePad lastPads[LIVE_PADS] = {};
    SettingsMenu menu;
    std::vector<PadInject> padInjects;
    std::string pngDir;
    std::vector<std::pair<uint32_t, uint32_t>> pngSteps;
    std::vector<Inject> injects;
    bool fbLog = false, audio = false, audioStats = false;
    uint32_t seq = 0, lastStep = 0, frames = 0, empty = 0;
    uint32_t rtt = 0, depthFar = 0, depthNear = 0, colorServed = 0, unresolved = 0;
    std::vector<float> renderMs, stepMs;
    double lastFrameT = 0, secStart = 0;
    uint32_t secFrames = 0;
    /* PORT_FPS (plan 27.3) */
    bool pngInterp = false;
    bool paced = false;   /* the main loop calls port_native_idle (real-time play): frames are spread over it */
    bool pending = false; /* the step's frame is drawn (read-backs answered) but not presented yet */
    bool realtime = false;      /* the main loop paces itself (it calls port_native_idle) */
    bool swapPending = false;   /* the step's frame is drawn but not swapped yet (swap_pending) */
    bool swapEveryStep = false; /* PORT_NATIVE_SWAP_EVERY_STEP=1: swap each step at once, as before plan 33 */
    uint32_t skippedSwaps = 0;
    uint32_t interpFrames = 0, presented = 0;
    bool asyncReadbacks = true;
};
Host* sHost;

const char* env(const char* name) {
    const char* v = std::getenv(name);
    return (v != nullptr && *v != '\0') ? v : nullptr;
}

bool png_wanted(const Host& h, uint32_t step) {
    if (h.pngDir.empty()) {
        return false;
    }
    if (h.pngSteps.empty()) {
        return true;
    }
    for (const auto& r : h.pngSteps) {
        if (step >= r.first && step <= r.second) {
            return true;
        }
    }
    return false;
}

void write_window_png(const Host& h, uint32_t step, int sub = 0) {
    std::vector<uint8_t> px((size_t)h.win.w * h.win.h * 4);
    char path[1024];
    glFinish();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadPixels(0, 0, (GLsizei)h.win.w, (GLsizei)h.win.h, GL_RGBA, GL_UNSIGNED_BYTE, px.data());
    if (sub > 0) {
        std::snprintf(path, sizeof(path), "%s/native_s%06u_f%d.png", h.pngDir.c_str(), step, sub);
    } else {
        std::snprintf(path, sizeof(path), "%s/native_s%06u.png", h.pngDir.c_str(), step);
    }
    write_png(path, px.data(), (int)h.win.w, (int)h.win.h);
}

/* the frame's viewport: the 4:3 picture letterboxed, or the whole window (PORT_WIDESCREEN) */
void view(const Host& h, uint32_t* vx, uint32_t* vy, uint32_t* vw, uint32_t* vh) {
    if (port_settings()->widescreen) {
        *vx = *vy = 0;
        *vw = h.win.w;
        *vh = h.win.h;
        return;
    }
    h.win.viewport(vx, vy, vw, vh);
}

int frames_per_step() {
    const int fps = port_settings()->fps;
    return fps > 30 ? std::max(1, (int)lround(fps / 30.0)) : 1;
}

void perf_set(bool on);

void pump(Host& h) {
    live_pump(h.win, h.in, h.lastStep);
    h.in.pollJoystick();
    if (h.in.perfToggle) {
        h.in.perfToggle = false;
        perf_set(!sPerf.on);
    }
    if (h.in.quit) {
        __atomic_store_n(&h.H->quit, 1, __ATOMIC_RELEASE);
    }
    LivePad pads[LIVE_PADS];
    h.in.allPads(pads);
    if (std::memcmp(pads, h.lastPads, sizeof(pads)) != 0) {
        std::memcpy(h.H->pads, pads, sizeof(pads));
        __atomic_fetch_add(&h.H->padSeq, 1, __ATOMIC_RELEASE);
        std::memcpy(h.lastPads, pads, sizeof(pads));
    }
}

float pct(std::vector<float> v, double p) {
    if (v.empty()) {
        return 0;
    }
    std::sort(v.begin(), v.end());
    return v[std::min(v.size() - 1, (size_t)(p * (double)(v.size() - 1) + 0.5))];
}

std::string summary(const std::vector<float>& v, float over) {
    double sum = 0;
    float mx = 0;
    uint32_t n = 0;
    for (float x : v) {
        sum += x;
        mx = std::max(mx, x);
        n += x > over;
    }
    char b[256];
    std::snprintf(b, sizeof(b), "mean %.2f / p50 %.2f / p95 %.2f / p99 %.2f / max %.1f ms, %u > %.0f ms",
                  v.empty() ? 0.0 : sum / (double)v.size(), pct(v, 0.5), pct(v, 0.95), pct(v, 0.99), mx, n, over);
    return b;
}

void perf_take_readbacks(PerfAcc& a) {
    const PortFast3dReadbacks r = port_fast3d_readbacks();
    a.rbColor += (uint32_t)(r.colorReads - sPerf.rb0.colorReads);
    a.rbDepth += (uint32_t)(r.depthReads - sPerf.rb0.depthReads);
    a.rb += (double)((r.colorUs - sPerf.rb0.colorUs) + (r.depthUs - sPerf.rb0.depthUs)) / 1000.0;
    a.shaders += (uint32_t)(r.shaders - sPerf.rb0.shaders);
    a.shader += (double)(r.shaderUs - sPerf.rb0.shaderUs) / 1000.0;
    sPerf.rb0 = r;
}

void perf_add(PerfAcc& d, const PerfAcc& a) {
    d.sim += a.sim;
    d.queue += a.queue;
    d.depthNow += a.depthNow;
    d.render += a.render;
    d.rb += a.rb;
    d.present += a.present;
    d.idle += a.idle;
    d.events += a.events;
    d.shader += a.shader;
    d.shaders += a.shaders;
    d.skipped += a.skipped;
    d.simMax = std::max(d.simMax, a.simMax);
    d.renderMax = std::max(d.renderMax, a.renderMax);
    d.presentMax = std::max(d.presentMax, a.presentMax);
    d.steps += a.steps;
    d.presented += a.presented;
    d.depthNowN += a.depthNowN;
    d.rbColor += a.rbColor;
    d.rbDepth += a.rbDepth;
}

void perf_set(bool on) {
    Perf& p = sPerf;
    if (on == p.on) {
        return;
    }
    p.on = on;
    live_audio_perf_enable(on);
    p.it = PerfAcc();
    p.sec = PerfAcc();
    p.rb0 = port_fast3d_readbacks();
    p.secStart = p.iterStart = live_now_s();
    if (p.startT == 0) {
        p.startT = p.secStart;
    }
    p.gameSince = p.idleSince = 0;
    const LiveAudioPerf a = live_audio_perf(true);
    p.underruns0 = a.underruns;
    p.trimmed0 = a.trimmedMs;
    p.overruns0 = a.overruns;
    std::fprintf(stderr, "perf: %s (PORT_PERF=1 or [debug] perf = 1 in the settings; F3 toggles)\n", on ? "on" : "off");
}

/* the iteration that began at p.iterStart ended at t (the next poll) */
void perf_iteration_end(double t) {
    Perf& p = sPerf;
    PerfAcc& a = p.it;
    const double iter = (t - p.iterStart) * 1000.0;
    if (p.idleSince != 0) {
        a.idle += (t - p.idleSince) * 1000.0;
    } else if (p.gameSince != 0) {
        a.sim += (t - p.gameSince) * 1000.0; /* the game ran to this poll without a frame */
    }
    const double busy = iter - a.idle;
    if (iter > 50.0) {
        a.spikes++;
        p.totalSpikes++;
        const LiveAudioPerf au = live_audio_perf(false);
        std::fprintf(stderr,
                     "perf: SLOW %.1f ms at step %u flow %u,%u,%u: %u steps, sim %.1f (queue %.1f, depth-now %u x = %.1f), "
                     "render %.1f (read-backs %u colour + %u depth = %.1f, %u new shaders = %.1f), present %.1f (%u), wait %.1f, "
                     "events %.1f; "
                     "audio ring %u ms, output %d ms\n",
                     iter, p.step, p.flow >> 16 & 0xFF, p.flow >> 8 & 0xFF, p.flow & 0xFF, a.steps,
                     a.sim, a.queue, a.depthNowN, a.depthNow, a.render, a.rbColor, a.rbDepth, a.rb, a.shaders, a.shader, a.present, a.presented,
                     a.idle, a.events, au.ringMs, au.queuedMs);
    }
    if (iter > p.worstIter) {
        p.worstIter = iter;
        p.worstStep = p.step;
    }
    a.catchUp += a.steps > 1;
    PerfAcc& S = p.sec;
    perf_add(S, a);
    S.iterMax = std::max(S.iterMax, iter);
    S.busyMax = std::max(S.busyMax, busy);
    S.spikes += a.spikes;
    S.catchUp += a.catchUp;
    S.iters++;
    p.it = PerfAcc();
    p.gameSince = p.idleSince = 0;
    if (t - p.secStart >= 1.0) {
        const double secs = t - p.secStart;
        const LiveAudioPerf au = live_audio_perf(true);
        const double n = std::max(1u, S.steps);
        std::fprintf(stderr,
                     "perf: %.0f s step %u flow %u,%u,%u: %.1f steps/s, %.1f frames/s (%u not shown), %u iterations (%u caught up, %u > 50 ms, "
                     "max %.1f, busy max %.1f) | sim %.1f / %.1f | queue %.2f | depth-now %u = %.1f ms | render %.1f / %.1f | "
                     "read-backs %u colour + %u depth = %.1f ms | shaders %u = %.1f ms | present %.1f / %.1f | wait %.1f ms/s | events %.1f ms/s | "
                     "audio %s ring %u ms, output %d ms, dry max %.0f ms, write max %.0f ms, underruns +%u, trimmed +%u ms, "
                     "game overruns +%u\n",
                     t - p.startT, p.step, p.flow >> 16 & 0xFF, p.flow >> 8 & 0xFF, p.flow & 0xFF, S.steps / secs, S.presented / secs, S.skipped, S.iters,
                     S.catchUp, S.spikes, S.iterMax, S.busyMax, S.sim / n, S.simMax, S.queue / n, S.depthNowN, S.depthNow,
                     S.render / n, S.renderMax, S.rbColor, S.rbDepth, S.rb, S.shaders, S.shader, S.present / std::max(1u, S.presented), S.presentMax,
                     S.idle / secs, S.events / secs, au.output, au.ringMs, au.queuedMs, au.dryMaxMs, au.putMaxMs,
                     au.underruns - p.underruns0, au.trimmedMs - p.trimmed0, au.overruns - p.overruns0);
        p.underruns0 = au.underruns;
        p.trimmed0 = au.trimmedMs;
        p.overruns0 = au.overruns;
        p.totalSteps += S.steps;
        S = PerfAcc();
        p.secStart = t;
    }
}

/* plan 33: the last frame's read-backs into the reply (the GPU has had the whole wait since the frame for them) */
void complete(Host& h) {
    PortFast3dStep st;
    if (!port_fast3d_complete(&st)) {
        return;
    }
    h.depthFar += st.depthFar;
    h.depthNear += st.depthNear;
    h.colorServed += st.colorServed;
    if (h.fbLog && (st.depthFar + st.depthNear + st.colorServed) != 0) {
        std::printf("native: step %u answered: depth far %u near %u other %u, colour %u\n", st.step, st.depthFar,
                    st.depthNear, st.depthOther, st.colorServed);
    }
}

/* a setting the menu changed that applies now (settings_menu.h onApply) */
void apply_setting(Host& h, const char* field) {
    const PortSettings* s = port_settings();
    if (std::strcmp(field, "window.vsync") == 0) {
        h.win.setVsync(s->vsync);
    } else if (std::strcmp(field, "window.fullscreen") == 0) {
        if ((s->fullscreen != 0) != h.win.fullscreen) {
            live_audio_suspend(2500); /* WSLg maps the window again (live_window.cpp, F11) */
            h.win.toggleFullscreen();
        }
    } else if (std::strcmp(field, "audio.volume") == 0 || std::strcmp(field, "audio.mute") == 0) {
        live_audio_set_volume(s->mute ? 0 : s->volume);
    }
}

void pad_inject(Host& h, const PadInject& j) {
    static const char* const kButtons[] = { "a", "b", "x", "y", "lb", "rb", "back", "start", "guide", "ls", "rs",
                                            "dpad_up", "dpad_down", "dpad_left", "dpad_right" };
    static const char* const kAxes[] = { "lx", "ly", "lt", "rx", "ry", "rt", "hatx", "haty" };
    if (j.pad < 0 || j.pad >= PORT_MAX_GAMEPADS) {
        return;
    }
    PortGamepad& g = h.in.pads.virt[j.pad];
    if (!h.in.pads.virtOn[j.pad]) {
        g = PortGamepad{};
        g.axis[2] = g.axis[5] = -32767; /* the triggers at rest */
        h.in.pads.virtOn[j.pad] = true;
    }
    g.connected = 1;
    for (int i = 0; i < 15; i++) {
        if (j.input == kButtons[i]) {
            g.buttons = j.value ? (g.buttons | (1u << i)) : (g.buttons & ~(1u << i));
            return;
        }
    }
    for (int i = 0; i < 8; i++) {
        if (j.input == kAxes[i]) {
            g.axis[i] = std::clamp(j.value, -32767, 32767);
            return;
        }
    }
    std::fprintf(stderr, "native: PORT_NATIVE_PAD_INJECT: unknown input %s\n", j.input.c_str());
}

} // namespace

#ifdef PORT_WASM_HOST
extern "C" uint32_t port_wasm_malloc(uint32_t size); /* the game's malloc (port/wasm/host/wasm_main.c) */
#endif

extern "C" unsigned port_native_open(void) {
    if (sHost != nullptr) {
        return port_gaddr(sHost->block);
    }
    Host* h = new Host();
    h->win.tag = "native";
    /* the settings file, then PORT_NATIVE_SCALE / VSYNC / MUTE / LATENCY / PORT_GFX_* over it (plan 27) */
    PortSettings* cfg = port_settings();
    const bool haveFile = port_settings_load(cfg, nullptr) != 0;
    const int scale = std::max(1, cfg->scale);
    const int vsync = cfg->vsync;
    uint32_t ww = 320u * scale, wh = 240u * scale;
    if (env("PORT_NATIVE_SIZE")) {
        unsigned a = 0, b = 0;
        if (std::sscanf(env("PORT_NATIVE_SIZE"), "%ux%u", &a, &b) == 2 && a >= 160 && b >= 120) {
            ww = a;
            wh = b;
        }
    }
    if (!h->win.open(ww, wh, "1080 Snowboarding (native)", vsync)) {
        return 0;
    }
    if (cfg->fullscreen) {
        h->win.toggleFullscreen(); /* before the audio opens (WSLg) */
    }
    /* the block the game reads (live_shm.h layout): low, as every address the x32 code holds must be */
    h->blockBytes = PORT_NATIVE_FB_REPLY + sizeof(LiveFbReply);
#if defined(PORT_WASM_HOST)
    /* plan 30: in the game's linear memory (its malloc), as every address the wasm game holds must be */
    const uint32_t ga = port_wasm_malloc((uint32_t)h->blockBytes);
    void* m = ga != 0 ? port_gptr(ga) : (void*)-1;
    if (ga != 0) {
        std::memset(m, 0, h->blockBytes);
    }
    const void* const kFailed = (void*)-1;
#elif defined(PORT_WIN32)
    /* a 32-bit process without /LARGEADDRESSAWARE: every address is below 2 GB (plan 29) */
    void* m = VirtualAlloc(nullptr, h->blockBytes, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (m == nullptr) {
        m = (void*)-1;
    }
    const void* const kFailed = (void*)-1;
#else
    void* m = mmap(nullptr, h->blockBytes, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_32BIT, -1, 0);
    const void* const kFailed = MAP_FAILED;
#endif
    if (m == kFailed || !port_gmem_ok(port_gaddr(m), h->blockBytes)) {
        std::fprintf(stderr, "native: no low memory for the host block\n");
        return 0;
    }
    h->block = (uint8_t*)m;
    h->H = (LiveShmHeader*)m;
    h->H->version = LIVE_VERSION;
    h->in.allPads(h->H->pads); /* controller 1 connected (the defaults); more if the settings plug them in */
    std::memcpy(h->lastPads, h->H->pads, sizeof(h->lastPads));
    live_audio_layout((LiveAudio*)(h->block + LIVE_AUDIO_HDR), PORT_NATIVE_AUDIO_DATA, LIVE_AUDIO_DEFAULT_BYTES);
    h->reply = (LiveFbReply*)(h->block + PORT_NATIVE_FB_REPLY);
    h->reply->magic = LIVE_FB_MAGIC;
    h->reply->version = LIVE_FB_VERSION;
    h->H->magic = LIVE_MAGIC;

    /* plan 25's render settings from the file (+ PORT_GFX_* over it): read once, at Fast3D's init */
    static PortN64Render render;
    render = { cfg->gfx_clip, cfg->gfx_shade, cfg->gfx_filter, cfg->gfx_clear };
    gPortN64Render = &render;
    h->wapi = new LiveWindow(&h->win);
    h->wapi->mSwap = true;
    port_fast3d_init_window(h->wapi, h->win.w, h->win.h);
    /* plan 33: the frame's read-backs finish while the game waits for its next step, not in the step */
    h->asyncReadbacks = !(env("PORT_READBACK_SYNC") && std::strcmp(env("PORT_READBACK_SYNC"), "0") != 0);
    port_fast3d_async_readbacks(h->asyncReadbacks);
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    h->win.swap();
    h->in.openJoystick(env("PORT_NATIVE_JS") ? env("PORT_NATIVE_JS") : "/dev/input/js0");

    LiveAudioOptions ao;
    ao.mute = cfg->mute != 0;
    ao.latencyMs = (uint32_t)std::max(20, cfg->latency_ms);
    live_audio_set_volume(cfg->volume);
    if (env("PORT_NATIVE_AUDIO_DUMP")) {
        ao.dumpPath = env("PORT_NATIVE_AUDIO_DUMP");
    }
    ao.stats = env("PORT_NATIVE_AUDIO_STATS") != nullptr && std::strcmp(env("PORT_NATIVE_AUDIO_STATS"), "0") != 0;
#ifdef PORT_SDL
    ao.openDelayMs = 0; /* the 2.5 s wait is for WSLg's PulseAudio (run 867); Windows' audio opens at once */
#endif
    if (env("PORT_NATIVE_AUDIO_OPEN_DELAY")) {
        ao.openDelayMs = (uint32_t)std::max(0, std::atoi(env("PORT_NATIVE_AUDIO_OPEN_DELAY")));
    }
    /* the window is open and mapped (above): live_audio's openDelayMs counts from here, so PulseAudio
     * opens after the map, as WSLg needs (run 867; audio-check.sh --native measures it) */
    live_audio_start(h->block, ao);
    h->audio = true;
    h->audioStats = ao.stats;

    if (env("PORT_NATIVE_PNG")) {
        h->pngDir = env("PORT_NATIVE_PNG");
        std::string spec = env("PORT_NATIVE_PNG_STEPS") ? env("PORT_NATIVE_PNG_STEPS") : "";
        for (size_t p = 0; p < spec.size();) {
            size_t e = spec.find(',', p);
            std::string item = spec.substr(p, e == std::string::npos ? std::string::npos : e - p);
            size_t d = item.find('-');
            uint32_t a = (uint32_t)std::strtoul(item.c_str(), nullptr, 10);
            uint32_t b = d == std::string::npos ? a : (uint32_t)std::strtoul(item.c_str() + d + 1, nullptr, 10);
            h->pngSteps.push_back({ a, b });
            p = e == std::string::npos ? spec.size() : e + 1;
        }
    }
    if (env("PORT_NATIVE_INJECT")) {
        std::string spec = env("PORT_NATIVE_INJECT");
        for (size_t p = 0; p < spec.size();) {
            size_t e = spec.find(',', p);
            std::string item = spec.substr(p, e == std::string::npos ? std::string::npos : e - p);
            size_t c1 = item.find(':'), c2 = item.rfind(':');
            if (c1 != std::string::npos && c2 > c1) {
                h->injects.push_back({ (uint32_t)std::strtoul(item.c_str(), nullptr, 10), item.substr(c1 + 1, c2 - c1 - 1),
                                       item.substr(c2 + 1) == "d", false });
            }
            p = e == std::string::npos ? spec.size() : e + 1;
        }
    }
    if (env("PORT_NATIVE_PAD_INJECT")) {
        std::string spec = env("PORT_NATIVE_PAD_INJECT");
        for (size_t p = 0; p < spec.size();) {
            size_t e = spec.find(',', p);
            std::string item = spec.substr(p, e == std::string::npos ? std::string::npos : e - p);
            size_t c1 = item.find(':'), c2 = item.find(':', c1 + 1), c3 = item.rfind(':');
            if (c1 != std::string::npos && c2 != std::string::npos && c3 > c2) {
                h->padInjects.push_back({ (uint32_t)std::strtoul(item.c_str(), nullptr, 10),
                                          std::atoi(item.c_str() + c1 + 1), item.substr(c2 + 1, c3 - c2 - 1),
                                          std::atoi(item.c_str() + c3 + 1), false });
            }
            p = e == std::string::npos ? spec.size() : e + 1;
        }
    }
    if (!(env("PORT_NATIVE_MENU") && std::strcmp(env("PORT_NATIVE_MENU"), "0") == 0)) {
        h->in.menu = &h->menu;
        h->menu.onApply = [h](const char* field) { apply_setting(*h, field); };
    }
    h->fbLog = env("PORT_NATIVE_FB_LOG") != nullptr;
    h->swapEveryStep = env("PORT_NATIVE_SWAP_EVERY_STEP") != nullptr && std::strcmp(env("PORT_NATIVE_SWAP_EVERY_STEP"), "0") != 0;
    h->pngInterp = env("PORT_NATIVE_PNG_INTERP") != nullptr && std::strcmp(env("PORT_NATIVE_PNG_INTERP"), "0") != 0;
    port_fast3d_interp_log(env("PORT_INTERP_LOG") != nullptr);
    h->secStart = live_now_s();
    sHost = h;
    if (cfg->perf) {
        perf_set(true);
    }
    std::printf("native: in-process host: window %ux%u, audio %s, block at %p\n", h->win.w, h->win.h,
                ao.mute ? "muted" : "on", (void*)h->block);
    std::printf("native: settings %s (%s)%s\n", port_settings_path(), haveFile ? "read" : "none: the defaults",
                h->in.menu ? "; F1 = settings menu" : "");
    std::fflush(stdout);
    return port_gaddr(h->block);
}

extern "C" int port_native_task(unsigned root, unsigned rq, unsigned imgs, int maxImgs) {
    if (sHost == nullptr) {
        return 0;
    }
    const double t0 = sPerf.on ? live_now_s() : 0;
    complete(*sHost); /* before the game takes the reply (port_live_fb_apply, after this task) */
    if (sPerf.on) {
        perf_take_readbacks(sPerf.it);
    }
    const FbRequestsX32* r = (const FbRequestsX32*)port_gptr(rq);
    if (r != nullptr) {
        for (int i = 0; i < r->nColor && i < PORT_FB_MAX_COLOR; i++) {
            port_fast3d_ask_color(r->color[i]);
        }
        for (int i = 0; i < r->nDepth && i < PORT_FB_MAX_DEPTH; i++) {
            port_fast3d_ask_depth(r->depthImg[i], (uint32_t)r->depthIdx[i]);
        }
    }
    const int n = port_fast3d_queue(root, (uint32_t*)port_gptr(imgs), maxImgs);
    if (sPerf.on) {
        sPerf.it.queue += (live_now_s() - t0) * 1000.0;
    }
    return n;
}

extern "C" void port_native_invalidate(unsigned addr, unsigned size) {
    if (sHost == nullptr || size == 0) {
        return;
    }
    /* as the presenter (presenter.cpp): the cache key is the G_SETTIMG address, which can precede a
     * LOADTILE's first byte */
    const unsigned lo = addr > 0x8000 ? addr - 0x8000 : 0;
    port_fast3d_invalidate(port_gptr(lo), (size_t)(addr + size - lo));
}

extern "C" void port_native_black(int black) {
    if (sHost != nullptr) {
        sHost->black = black != 0;
    }
}

extern "C" int port_native_depth(unsigned img, unsigned idx) {
    if (sHost == nullptr) {
        return -1;
    }
    if (!sPerf.on) {
        return port_fast3d_depth_now(img, idx);
    }
    const double t0 = live_now_s();
    const int z = port_fast3d_depth_now(img, idx);
    sPerf.it.depthNow += (live_now_s() - t0) * 1000.0;
    sPerf.it.depthNowN++;
    PerfAcc counted; /* its read-back is counted here, not in the next frame's */
    perf_take_readbacks(counted);
    return z;
}

namespace {

/* the window's back buffer to the screen */
void swap(Host& h) {
    h.swapPending = false;
    const double t0 = sPerf.on ? live_now_s() : 0;
    port_fast3d_end_frame(); /* -> LiveWindow::SwapBuffersEnd -> eglSwapBuffers */
    if (sPerf.on) {
        const double ms = (live_now_s() - t0) * 1000.0;
        sPerf.it.present += ms;
        sPerf.it.presentMax = std::max(sPerf.it.presentMax, ms);
        sPerf.it.presented++;
    }
    h.presented++;
    h.secFrames++;
}

/* plan 33: real-time play swaps the step's frame when the main loop is done stepping (port_native_idle, or the next
 * poll when it is behind). A loop catching up runs several steps back to back; each is drawn as before (its
 * read-backs answered), but only the last is swapped, as the N64's VI shows only the last of several frames drawn
 * within one of its frames: a swap can block (WSLg: 5-15 ms), and swapping every caught-up step made the catch-up
 * itself slow, so the loop fell behind again. */
void swap_pending(Host& h) {
    if (h.swapPending) {
        swap(h);
    }
}

/* show the frame drawn last: osViBlack, the PNG, present */
void present(Host& h, uint32_t step, int sub, bool deferSwap = false) {
    if (h.black) {
        /* osViBlack (plan 22.5): the VI outputs black while the RDP goes on drawing, so the frame is drawn
         * (its read-backs answered) and the window cleared over it. The faders blank the screen between
         * a fade-out and the next scene's fade-in; the port showed the stale frame there. */
        h.win.black();
    }
    if (h.menu.isOpen()) {
        menu_overlay_draw(h.menu.image(), SettingsMenu::kW, SettingsMenu::kH, h.win.w, h.win.h);
    }
    if ((sub == 0 || h.pngInterp) && png_wanted(h, step)) {
        write_window_png(h, step, sub);
    }
    if (deferSwap) {
        if (h.swapPending) {
            h.skippedSwaps++; /* the step before was never shown: the loop is catching up */
            if (sPerf.on) {
                sPerf.it.skipped++;
            }
        }
        h.swapPending = true;
        return;
    }
    swap(h);
}

/* PORT_FPS: draw and present the frames between the previous step and this one (sub 1 .. k - 1 of k) */
void present_interp(Host& h, uint32_t step, int sub, int k) {
    uint32_t vx, vy, vw, vh;
    bool lerped = false;
    view(h, &vx, &vy, &vw, &vh);
    if (port_fast3d_redraw(vx, vy, vw, vh, sub < k ? (float)sub / (float)k : 1.0f, &lerped)) {
        h.interpFrames += lerped;
        present(h, step, sub < k ? sub : 0);
    }
}

} // namespace

extern "C" void port_native_frame(unsigned step) {
    Host* h = sHost;
    if (h == nullptr) {
        return;
    }
    const double t0 = live_now_s();
    Perf& P = sPerf;
    if (P.on) {
        if (P.gameSince != 0) {
            const double ms = (t0 - P.gameSince) * 1000.0;
            P.it.sim += ms;
            P.it.simMax = std::max(P.it.simMax, ms);
        }
        P.step = step;
        P.it.steps++;
    }
    uint32_t vx, vy, vw, vh;
    view(*h, &vx, &vy, &vw, &vh);
    PortFast3dStep st;
    h->lastStep = step;
    h->pending = false;
    const bool drew = port_fast3d_step(vx, vy, vw, vh, h->reply, step, h->seq++, &st);
    if (P.on) {
        const double ms = (live_now_s() - t0) * 1000.0;
        P.it.render += ms;
        P.it.renderMax = std::max(P.it.renderMax, ms);
        perf_take_readbacks(P.it);
    }
    if (!drew) {
        h->empty++;
        pump(*h);
        if (P.on) {
            P.gameSince = live_now_s();
        }
        return;
    }
    const int k = frames_per_step();
    if (k > 1 && h->paced) {
        /* real-time play: port_native_idle presents this step's frames over the wait for the next step */
        h->pending = true;
    } else {
        if (k > 1) {
            /* no pacing (a headless run, the first step): the interpolated frames at once, then the step's */
            for (int i = 1; i <= k; i++) {
                present_interp(*h, step, i, k);
            }
        } else {
            present(*h, step, 0, h->realtime && !h->swapEveryStep);
        }
    }
    const double t1 = live_now_s();
    h->renderMs.push_back((float)((t1 - t0) * 1000.0));
    if (h->lastFrameT != 0) {
        h->stepMs.push_back((float)((t1 - h->lastFrameT) * 1000.0));
    }
    h->lastFrameT = t1;
    h->frames++;
    h->rtt += st.rttTasks;
    h->depthFar += st.depthFar;
    h->depthNear += st.depthNear;
    h->colorServed += st.colorServed;
    h->unresolved += st.unresolved;
    if (h->fbLog && (st.depthFar + st.depthNear + st.colorServed + st.rttTasks) != 0) {
        std::printf("native: step %u: %u tasks, rtt %u, depth far %u near %u other %u, colour %u\n", step, st.tasks,
                    st.rttTasks, st.depthFar, st.depthNear, st.depthOther, st.colorServed);
    }
    for (Inject& j : h->injects) {
        if (!j.sent && step >= j.step) {
            if (!h->win.sendKey(j.key.c_str(), j.down)) {
                std::fprintf(stderr, "native: unknown key %s\n", j.key.c_str());
            }
            j.sent = true;
        }
    }
    for (PadInject& j : h->padInjects) {
        if (!j.sent && step >= j.step) {
            pad_inject(*h, j);
            std::printf("native: virtual gamepad %d %s = %d (step %u)\n", j.pad, j.input.c_str(), j.value, step);
            j.sent = true;
        }
    }
    pump(*h);
    if (P.on) {
        P.gameSince = live_now_s();
    }
    if (t1 - h->secStart >= 1.0) {
        char title[160];
        std::snprintf(title, sizeof(title), "1080 Snowboarding (native) - step %u - %u fps", step, h->secFrames);
        h->win.setTitle(title);
        h->secFrames = 0;
        h->secStart = t1;
    }
}

extern "C" int port_native_idle(unsigned us) {
    Host* h = sHost;
    const int k = frames_per_step();
    if (h != nullptr) {
        h->realtime = true;
        swap_pending(*h);
    }
    if (sPerf.on && h != nullptr) {
        sPerf.idleSince = live_now_s();
    }
    if (h == nullptr || k <= 1) {
        return 0;
    }
    h->paced = true;
    if (!h->pending) {
        return 0;
    }
    /* the wait until the next step holds n frames at the display rate; the last is the step's own frame (its
     * depth image is what the next step's reads see, plan 22.4) */
    const double t0 = live_now_s(), wait = us * 1e-6, dt = 1.0 / std::max(31, port_settings()->fps);
    const int n = std::max(1, (int)std::floor(wait / dt + 0.5));
    for (int i = 0; i < n; i++) {
        const double at = t0 + i * dt;
        const double now = live_now_s();
        if (at > now) {
            std::this_thread::sleep_for(std::chrono::microseconds((long long)((at - now) * 1e6)));
        }
        /* sub i + 1 of n: alpha (i + 1) / n */
        uint32_t vx, vy, vw, vh;
        bool lerped = false;
        view(*h, &vx, &vy, &vw, &vh);
        const float alpha = i + 1 < n ? (float)(i + 1) / (float)n : 1.0f;
        if (port_fast3d_redraw(vx, vy, vw, vh, alpha, &lerped)) {
            h->interpFrames += lerped;
            present(*h, h->lastStep, i + 1 < n ? i + 1 : 0);
        }
        pump(*h);
    }
    h->pending = false;
    const double left = t0 + wait - live_now_s();
    if (left > 0) {
        std::this_thread::sleep_for(std::chrono::microseconds((long long)(left * 1e6)));
    }
    return 1;
}

extern "C" int port_native_poll(void) {
    Host* h = sHost;
    if (h == nullptr) {
        return 0;
    }
    swap_pending(*h); /* the loop was behind: it did not wait (port_native_idle) */
    if (sPerf.on) {
        const double t = live_now_s();
        if (sPerf.iterStart != 0) {
            perf_iteration_end(t);
        }
        sPerf.iterStart = t;
    }
    pump(*h);
    if (sPerf.on) {
        sPerf.it.events += (live_now_s() - sPerf.iterStart) * 1000.0;
        sPerf.gameSince = live_now_s();
    }
    return __atomic_load_n(&h->H->quit, __ATOMIC_ACQUIRE) != 0;
}

extern "C" void port_native_close(void) {
    Host* h = sHost;
    if (h == nullptr) {
        return;
    }
    complete(*h);
    swap_pending(*h);
    std::string a = live_audio_status();
    if (a.size() > 2 && a.compare(0, 2, ", ") == 0) {
        a.erase(0, 2);
    }
    /* the audio thread can sit in a blocking pa_simple_write (plan 19.14's open PulseAudio stall): stop it
     * from a helper and wait at most 2 s; if it does not stop, the process ends with _exit at exit time,
     * after the game's own teardown (its trace / save files are closed by then), instead of hanging in
     * the join or std::terminate in live_audio.cpp's static std::thread destructor */
    static std::atomic<bool> stopped{ false };
    std::thread([] {
        live_audio_stop();
        stopped = true;
    }).detach();
    for (int i = 0; i < 200 && !stopped; i++) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    if (!stopped) {
        std::printf("native: the audio thread did not stop (blocked in PulseAudio): abandoned at exit\n");
        if (h->audioStats) {
            std::printf("native: %s\n", live_audio_stats_line().c_str()); /* live_audio_stop did not print it */
        }
        std::atexit([] {
            std::fflush(nullptr);
            _exit(0);
        });
    }
    std::printf("native: %u frames (%u steps without tasks), %u unresolved G_DL, %u render-to-texture passes, depth "
                "answers far %u / near %u, %u colour images\n", h->frames, h->empty, h->unresolved, h->rtt, h->depthFar,
                h->depthNear, h->colorServed);
    std::printf("native: render %s\n", summary(h->renderMs, 33.3f).c_str());
    if (h->skippedSwaps != 0) {
        std::printf("native: %u caught-up steps drawn but not shown (only the last of a catch-up is swapped)\n",
                    h->skippedSwaps);
    }
    std::printf("native: frame interval %s\n", summary(h->stepMs, 50.0f).c_str());
    if (frames_per_step() > 1) {
        uint32_t f, c, sm, sv, lr;
        port_fast3d_interp_stats(&f, &c, &sm, &sv, &lr);
        std::printf("native: PORT_FPS %d: %u frames presented, %u interpolated (%u camera cuts drawn as the step, "
                    "%u matrices / %u vertex buffers snapped, %u rectangles lerped)\n",
                    port_settings()->fps, h->presented, f, c, sm, sv, lr);
    }
    std::printf("native: audio %s\n", a.c_str());
    if (sPerf.on) {
        std::fprintf(stderr, "perf: total %llu steps, %llu iterations over 50 ms, the worst %.1f ms at step %u\n",
                     (unsigned long long)sPerf.totalSteps, (unsigned long long)sPerf.totalSpikes, sPerf.worstIter,
                     sPerf.worstStep);
    }
    std::fflush(stdout);
}

extern "C" void port_native_flow(unsigned flow) {
    sPerf.flow = flow;
}
