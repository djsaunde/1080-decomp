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

#ifndef PORT_WIN32
#define GL_GLEXT_PROTOTYPES 1
#include <GL/gl.h>
#endif

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
    uint32_t interpFrames = 0, presented = 0;
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

void pump(Host& h) {
    live_pump(h.win, h.in, h.lastStep);
    h.in.pollJoystick();
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

extern "C" unsigned port_native_open(void) {
    if (sHost != nullptr) {
        return (unsigned)(uintptr_t)sHost->block;
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
#ifdef PORT_WIN32
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
    if (m == kFailed || (uintptr_t)m + h->blockBytes > 0x80000000u) {
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
#ifdef PORT_WIN32
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
    h->pngInterp = env("PORT_NATIVE_PNG_INTERP") != nullptr && std::strcmp(env("PORT_NATIVE_PNG_INTERP"), "0") != 0;
    port_fast3d_interp_log(env("PORT_INTERP_LOG") != nullptr);
    h->secStart = live_now_s();
    sHost = h;
    std::printf("native: in-process host: window %ux%u, audio %s, block at %p\n", h->win.w, h->win.h,
                ao.mute ? "muted" : "on", (void*)h->block);
    std::printf("native: settings %s (%s)%s\n", port_settings_path(), haveFile ? "read" : "none: the defaults",
                h->in.menu ? "; F1 = settings menu" : "");
    std::fflush(stdout);
    return (unsigned)(uintptr_t)h->block;
}

extern "C" int port_native_task(unsigned root, unsigned rq, unsigned imgs, int maxImgs) {
    if (sHost == nullptr) {
        return 0;
    }
    const FbRequestsX32* r = (const FbRequestsX32*)(uintptr_t)rq;
    if (r != nullptr) {
        for (int i = 0; i < r->nColor && i < PORT_FB_MAX_COLOR; i++) {
            port_fast3d_ask_color(r->color[i]);
        }
        for (int i = 0; i < r->nDepth && i < PORT_FB_MAX_DEPTH; i++) {
            port_fast3d_ask_depth(r->depthImg[i], (uint32_t)r->depthIdx[i]);
        }
    }
    return port_fast3d_queue(root, (uint32_t*)(uintptr_t)imgs, maxImgs);
}

extern "C" void port_native_invalidate(unsigned addr, unsigned size) {
    if (sHost == nullptr || size == 0) {
        return;
    }
    /* as the presenter (presenter.cpp): the cache key is the G_SETTIMG address, which can precede a
     * LOADTILE's first byte */
    const unsigned lo = addr > 0x8000 ? addr - 0x8000 : 0;
    port_fast3d_invalidate((const void*)(uintptr_t)lo, (size_t)(addr + size - lo));
}

extern "C" void port_native_black(int black) {
    if (sHost != nullptr) {
        sHost->black = black != 0;
    }
}

extern "C" int port_native_depth(unsigned img, unsigned idx) {
    return sHost == nullptr ? -1 : port_fast3d_depth_now(img, idx);
}

namespace {

/* show the frame drawn last: osViBlack, the PNG, present */
void present(Host& h, uint32_t step, int sub) {
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
    port_fast3d_end_frame(); /* -> LiveWindow::SwapBuffersEnd -> eglSwapBuffers */
    h.presented++;
    h.secFrames++;
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
    uint32_t vx, vy, vw, vh;
    view(*h, &vx, &vy, &vw, &vh);
    PortFast3dStep st;
    h->lastStep = step;
    h->pending = false;
    if (!port_fast3d_step(vx, vy, vw, vh, h->reply, step, h->seq++, &st)) {
        h->empty++;
        pump(*h);
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
            present(*h, step, 0);
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
    pump(*h);
    return __atomic_load_n(&h->H->quit, __ATOMIC_ACQUIRE) != 0;
}

extern "C" void port_native_close(void) {
    Host* h = sHost;
    if (h == nullptr) {
        return;
    }
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
    std::printf("native: frame interval %s\n", summary(h->stepMs, 50.0f).c_str());
    if (frames_per_step() > 1) {
        uint32_t f, c, sm, sv, lr;
        port_fast3d_interp_stats(&f, &c, &sm, &sv, &lr);
        std::printf("native: PORT_FPS %d: %u frames presented, %u interpolated (%u camera cuts drawn as the step, "
                    "%u matrices / %u vertex buffers snapped, %u rectangles lerped)\n",
                    port_settings()->fps, h->presented, f, c, sm, sv, lr);
    }
    std::printf("native: audio %s\n", a.c_str());
    std::fflush(stdout);
}
