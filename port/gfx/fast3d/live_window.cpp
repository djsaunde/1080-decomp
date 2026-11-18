/* port/gfx/fast3d: the live window (X11 + EGL) and its input; see live_window.h. Moved out of
 * presenter.cpp (runs 856 / 858 / 861) so the native 64-bit game (native_host.cpp, plan 21.9) uses the
 * same code. */
#include "live_window.h"
#include "live_audio.h"

#include <fcntl.h>
#include <linux/joystick.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

#include <EGL/egl.h>
#include <X11/XKBlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#define GL_GLEXT_PROTOTYPES 1
#include <GL/gl.h>
#include <GL/glext.h>

double live_now_s() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + ts.tv_nsec * 1e-9;
}

/* why XOpenDisplay failed, as far as can be told (WSLg's socket, the X socket of a local display) */
static void explain_display(const char* disp) {
    bool wsl = false;
    if (FILE* f = std::fopen("/proc/version", "r")) {
        char v[512] = {};
        size_t n = std::fread(v, 1, sizeof(v) - 1, f);
        v[n] = '\0';
        std::fclose(f);
        wsl = std::strstr(v, "microsoft") != nullptr || std::strstr(v, "Microsoft") != nullptr;
    }
    std::string sock;
    if (disp[0] == ':') {
        sock = "/tmp/.X11-unix/X" + std::string(disp + 1).substr(0, std::string(disp + 1).find('.'));
    }
    struct stat st;
    if (!sock.empty() && stat(sock.c_str(), &st) != 0) {
        std::fprintf(stderr, "  the X socket %s does not exist: no X server on that display.\n", sock.c_str());
        if (wsl) {
            std::fprintf(stderr, "  WSL2: WSLg is not running or not mounted (/tmp/.X11-unix should link to /mnt/wslg/.X11-unix).\n"
                                 "  From Windows: `wsl --shutdown`, reopen the distro; check `wsl --version` lists WSLg\n"
                                 "  and .wslconfig does not set guiApplications=false.\n");
        }
    } else if (!sock.empty()) {
        std::fprintf(stderr, "  %s exists but the server did not accept the connection (not running, or no access:\n"
                             "  XAUTHORITY / xhost).%s\n", sock.c_str(),
                     wsl ? " WSL2: a stale WSLg session; `wsl --shutdown` from Windows restarts it." : "");
    } else {
        std::fprintf(stderr, "  a remote / TCP display: check that it is reachable and accepts this client.\n");
    }
}

#define D ((Display*)dpy)
#define ED ((EGLDisplay)edpy)

bool LiveX11::open(uint32_t width, uint32_t height, const char* title, int vsync) {
    w = width;
    h = height;
    const char* disp = getenv("DISPLAY");
    if (disp == nullptr || *disp == '\0') {
        std::fprintf(stderr, "%s: DISPLAY is not set: the window needs an X server.\n"
                             "  WSL2: WSLg provides one at DISPLAY=:0 (export DISPLAY=:0; WSLg must be on: `wsl --version`\n"
                             "  on Windows lists it; .wslconfig guiApplications=true). Elsewhere start X or use ssh -X.\n", tag);
        return false;
    }
    dpy = XOpenDisplay(nullptr);
    if (dpy == nullptr) {
        std::fprintf(stderr, "%s: cannot open the X display DISPLAY=%s.\n", tag, disp);
        explain_display(disp);
        return false;
    }
    int scr = DefaultScreen(D);
    win = XCreateSimpleWindow(D, RootWindow(D, scr), 0, 0, w, h, 0, BlackPixel(D, scr), BlackPixel(D, scr));
    XStoreName(D, win, title);
    /* resizable (run 861): the frame is letterboxed at 4:3 into whatever size the window has (viewport()) */
    XSizeHints* sh = XAllocSizeHints();
    sh->flags = PMinSize;
    sh->min_width = 320;
    sh->min_height = 240;
    XSetWMNormalHints(D, win, sh);
    XFree(sh);
    XSelectInput(D, win, KeyPressMask | KeyReleaseMask | StructureNotifyMask | FocusChangeMask | ExposureMask);
    Atom del = XInternAtom(D, "WM_DELETE_WINDOW", 0);
    wmDelete = del;
    XSetWMProtocols(D, win, &del, 1);
    int supported = 0;
    XkbSetDetectableAutoRepeat(D, 1, &supported); /* no fake KeyRelease while a key is held */
    XMapRaised(D, win);
    XFlush(D);

    edpy = eglGetDisplay((EGLNativeDisplayType)D);
    EGLint maj, min;
    if (edpy == EGL_NO_DISPLAY || !eglInitialize(ED, &maj, &min)) {
        std::fprintf(stderr, "%s: eglInitialize failed on the X display (0x%x): no usable GL driver for it;\n"
                             "  try software GL (play.sh -S, or LIBGL_ALWAYS_SOFTWARE=1)\n", tag, eglGetError());
        return false;
    }
    eglBindAPI(EGL_OPENGL_API);
    const EGLint cfgAttr[] = { EGL_SURFACE_TYPE, EGL_WINDOW_BIT, EGL_RED_SIZE, 8, EGL_GREEN_SIZE, 8, EGL_BLUE_SIZE, 8,
                               EGL_DEPTH_SIZE, 24, EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT, EGL_NONE };
    EGLConfig cfg;
    EGLint n = 0;
    if (!eglChooseConfig(ED, cfgAttr, &cfg, 1, &n) || n == 0) {
        std::fprintf(stderr, "%s: no EGL window config (RGB8 + depth 24, desktop GL); try software GL "
                             "(play.sh -S, or LIBGL_ALWAYS_SOFTWARE=1)\n", tag);
        return false;
    }
    ctx = eglCreateContext(ED, cfg, EGL_NO_CONTEXT, nullptr);
    surf = eglCreateWindowSurface(ED, cfg, (EGLNativeWindowType)win, nullptr);
    if (ctx == EGL_NO_CONTEXT || surf == EGL_NO_SURFACE || !eglMakeCurrent(ED, (EGLSurface)surf, (EGLSurface)surf, (EGLContext)ctx)) {
        std::fprintf(stderr, "%s: EGL context / window surface failed (0x%x); try software GL (play.sh -S, "
                             "or LIBGL_ALWAYS_SOFTWARE=1)\n", tag, eglGetError());
        return false;
    }
    /* 0: frames are shown as they arrive (the game paces itself); 1: each swap waits for the display */
    eglSwapInterval(ED, vsync);
    std::printf("%s: %ux%u window, EGL %d.%d, %s / %s%s\n", tag, w, h, maj, min, (const char*)glGetString(GL_RENDERER),
                (const char*)glGetString(GL_VERSION), vsync ? ", vsync" : "");
    return true;
}

void LiveX11::swap() {
    eglSwapBuffers(ED, (EGLSurface)surf);
}

void LiveX11::black() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_SCISSOR_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void LiveX11::setTitle(const char* title) {
    XStoreName(D, win, title);
}

int LiveX11::fd() const {
    return ConnectionNumber(D);
}

void LiveX11::viewport(uint32_t* x, uint32_t* y, uint32_t* vw, uint32_t* vh) const {
    *vw = w;
    *vh = w * 3 / 4;
    if (*vh > h) {
        *vh = h;
        *vw = h * 4 / 3;
    }
    *x = (w - *vw) / 2;
    *y = (h - *vh) / 2;
}

void LiveX11::toggleFullscreen() {
    fullscreen = !fullscreen;
    XEvent ev = {};
    ev.xclient.type = ClientMessage;
    ev.xclient.window = win;
    ev.xclient.message_type = XInternAtom(D, "_NET_WM_STATE", 0);
    ev.xclient.format = 32;
    ev.xclient.data.l[0] = fullscreen ? 1 : 0; /* _NET_WM_STATE_ADD / _REMOVE */
    ev.xclient.data.l[1] = (long)XInternAtom(D, "_NET_WM_STATE_FULLSCREEN", 0);
    ev.xclient.data.l[3] = 1; /* a normal application */
    XSendEvent(D, RootWindow(D, DefaultScreen(D)), 0, SubstructureRedirectMask | SubstructureNotifyMask, &ev);
    XFlush(D);
}

bool LiveX11::sendKey(const char* keysymName, bool down) {
    KeySym ks = XStringToKeysym(keysymName);
    if (ks == NoSymbol) {
        return false;
    }
    XEvent ev = {};
    ev.xkey.type = down ? KeyPress : KeyRelease;
    ev.xkey.display = D;
    ev.xkey.window = win;
    ev.xkey.root = DefaultRootWindow(D);
    ev.xkey.keycode = XKeysymToKeycode(D, ks);
    ev.xkey.same_screen = 1;
    XSendEvent(D, win, 1, down ? KeyPressMask : KeyReleaseMask, &ev);
    XFlush(D);
    return true;
}

/* ---------------- input ---------------- */

enum : uint16_t {
    B_A = 0x8000, B_B = 0x4000, B_Z = 0x2000, B_START = 0x1000, D_U = 0x0800, D_D = 0x0400, D_L = 0x0200,
    D_R = 0x0100, B_L = 0x0020, B_R = 0x0010, C_U = 0x0008, C_D = 0x0004, C_L = 0x0002, C_R = 0x0001
};

void LiveInput::key(unsigned long ks, bool isDown) {
    uint16_t bit = 0;
    switch (ks) {
        case XK_Left: case XK_a: case XK_A: left = isDown; return;
        case XK_Right: case XK_d: case XK_D: right = isDown; return;
        case XK_Up: case XK_w: case XK_W: up = isDown; return;
        case XK_Down: case XK_s: case XK_S: down = isDown; return;
        case XK_Shift_L: case XK_Shift_R: shift = isDown; return;
        case XK_Escape: if (isDown) quit = true; return;
        case XK_x: case XK_X: case XK_space: bit = B_A; break;
        case XK_z: case XK_Z: bit = B_B; break;
        case XK_c: case XK_C: bit = B_Z; break;
        case XK_Return: case XK_KP_Enter: bit = B_START; break;
        case XK_q: case XK_Q: bit = B_L; break;
        case XK_e: case XK_E: bit = B_R; break;
        case XK_i: case XK_I: bit = C_U; break;
        case XK_k: case XK_K: bit = C_D; break;
        case XK_j: case XK_J: bit = C_L; break;
        case XK_l: case XK_L: bit = C_R; break;
        case XK_t: case XK_T: bit = D_U; break;
        case XK_g: case XK_G: bit = D_D; break;
        case XK_f: case XK_F: bit = D_L; break;
        case XK_h: case XK_H: bit = D_R; break;
        default: return;
    }
    keys = isDown ? (uint16_t)(keys | bit) : (uint16_t)(keys & ~bit);
}

void LiveInput::releaseAll() {
    left = right = up = down = shift = false;
    keys = 0;
}

void LiveInput::openJoystick(const char* path) {
    js = ::open(path, O_RDONLY | O_NONBLOCK);
    if (js >= 0) {
        char name[128] = "?";
        ioctl(js, JSIOCGNAME(sizeof(name)), name);
        std::printf("gamepad %s: %s\n", path, name);
    } else {
        ev.open();
    }
}

/* an Xbox-style layout: A = A, B / X = B, LB / left trigger = Z, RB / right trigger = R, Back = L,
 * Start = START, right stick = C buttons, hat = D-pad */
void LiveInput::pollJoystick() {
    static const uint16_t map[] = { B_A, B_B, B_B, C_U, B_Z, B_R, B_L, B_START };
    if (js < 0) {
        uint16_t bits = 0;
        if (ev.fd >= 0) {
            ev.poll(axis, &bits);
            jsButtons = 0;
            for (int i = 0; i < 8; i++) {
                jsButtons |= (bits >> i) & 1 ? map[i] : 0;
            }
        }
        return;
    }
    js_event e;
    while (::read(js, &e, sizeof(e)) == (ssize_t)sizeof(e)) {
        uint8_t t = e.type & ~JS_EVENT_INIT;
        if (t == JS_EVENT_AXIS && e.number < 8) {
            axis[e.number] = e.value;
        } else if (t == JS_EVENT_BUTTON && e.number < 8) {
            jsButtons = e.value ? (uint16_t)(jsButtons | map[e.number]) : (uint16_t)(jsButtons & ~map[e.number]);
        }
    }
}

LivePad LiveInput::pad() const {
    LivePad p = {};
    int sx = (right ? 80 : 0) - (left ? 80 : 0), sy = (up ? 80 : 0) - (down ? 80 : 0);
    if (shift) {
        sx /= 2;
        sy /= 2;
    }
    uint16_t b = keys | jsButtons;
    if (js >= 0 || ev.fd >= 0) {
        if (std::abs(axis[0]) > 3000 || std::abs(axis[1]) > 3000) {
            sx = axis[0] * 80 / 32767;
            sy = -axis[1] * 80 / 32767;
        }
        const int T = 16000;
        b |= axis[2] > T ? B_Z : 0;
        b |= axis[5] > T ? B_R : 0;
        b |= axis[3] < -T ? C_L : axis[3] > T ? C_R : 0;
        b |= axis[4] < -T ? C_U : axis[4] > T ? C_D : 0;
        b |= axis[6] < -T ? D_L : axis[6] > T ? D_R : 0;
        b |= axis[7] < -T ? D_U : axis[7] > T ? D_D : 0;
    }
    p.button = b;
    p.stick_x = (live_s8)std::clamp(sx, -80, 80);
    p.stick_y = (live_s8)std::clamp(sy, -80, 80);
    p.connected = 1;
    return p;
}

int live_pump(LiveX11& w, LiveInput& in, uint32_t step) {
    int n = 0;
    while (XPending((Display*)w.dpy) > 0) {
        XEvent ev;
        XNextEvent((Display*)w.dpy, &ev);
        n++;
        if (ev.type == KeyPress || ev.type == KeyRelease) {
            KeySym ks = XLookupKeysym(&ev.xkey, 0);
            in.key(ks, ev.type == KeyPress);
            if (ev.xkey.send_event) {
                std::printf("%s: synthetic %s %s (step %u)\n", w.tag, XKeysymToString(ks),
                            ev.type == KeyPress ? "down" : "up", step);
            }
            if (ev.type == KeyPress && ks == XK_F11) {
                /* WSLg maps the window again: no stream may be playing then (live_audio.h, run 867) */
                live_audio_suspend(2500);
                w.toggleFullscreen();
            }
        } else if (ev.type == ConfigureNotify) {
            w.w = (uint32_t)std::max(1, ev.xconfigure.width);
            w.h = (uint32_t)std::max(1, ev.xconfigure.height);
        } else if (ev.type == ClientMessage && (unsigned long)ev.xclient.data.l[0] == w.wmDelete) {
            in.quit = true;
        } else if (ev.type == FocusOut) {
            in.releaseAll();
        }
    }
    return n;
}
