/* port/gfx/fast3d: the live window and its input over SDL2, for the Windows build (live_window.h, plan
 * section 27). The interface and the key / pad mapping are the X11 build's (live_window.cpp). */
#include "live_window.h"
#include "live_audio.h"

#define SDL_MAIN_HANDLED 1
#include <SDL2/SDL.h>

#include <algorithm>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "../../win32/win32_gl.h"

double live_now_s() {
    using namespace std::chrono;
    return duration<double>(steady_clock::now().time_since_epoch()).count();
}

#define W ((SDL_Window*)win)

static void* gl_getproc(const char* name) {
    return SDL_GL_GetProcAddress(name);
}

bool LiveSDL::open(uint32_t width, uint32_t height, const char* title, int vsync) {
    w = width;
    h = height;
    SDL_SetMainReady();
    SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS, "1");
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | SDL_INIT_EVENTS) != 0) {
        std::fprintf(stderr, "%s: SDL_Init failed: %s\n", tag, SDL_GetError());
        return false;
    }
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    /* a compatibility context, as EGL's default desktop-GL context on Linux: gfx_opengl's GLSL 1.30 / 4.x
     * shaders and its client-side state work in it */
    const char* hidden = std::getenv("PORT_NATIVE_HIDDEN");
    Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI;
    if (hidden != nullptr && *hidden != '\0' && std::strcmp(hidden, "0") != 0) {
        flags |= SDL_WINDOW_HIDDEN;
    }
    win = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (int)w, (int)h, flags);
    if (win == nullptr) {
        std::fprintf(stderr, "%s: SDL_CreateWindow failed: %s\n", tag, SDL_GetError());
        return false;
    }
    SDL_SetWindowMinimumSize(W, 320, 240);
    ctx = SDL_GL_CreateContext(W);
    if (ctx == nullptr) {
        std::fprintf(stderr, "%s: no OpenGL context: %s (update the graphics driver)\n", tag, SDL_GetError());
        return false;
    }
    SDL_GL_MakeCurrent(W, (SDL_GLContext)ctx);
    if (port_gl_load(gl_getproc) != 0) {
        std::fprintf(stderr, "%s: the OpenGL driver lacks entry points the renderer needs (above)\n", tag);
        return false;
    }
    SDL_GL_SetSwapInterval(vsync);
    int dw = 0, dh = 0;
    SDL_GL_GetDrawableSize(W, &dw, &dh);
    w = (uint32_t)std::max(1, dw);
    h = (uint32_t)std::max(1, dh);
    std::printf("%s: %ux%u window, SDL %d.%d.%d, %s / %s%s\n", tag, w, h, SDL_MAJOR_VERSION, SDL_MINOR_VERSION,
                SDL_PATCHLEVEL, (const char*)glGetString(GL_RENDERER), (const char*)glGetString(GL_VERSION),
                vsync ? ", vsync" : "");
    return true;
}

void LiveSDL::swap() {
    SDL_GL_SwapWindow(W);
}

void LiveSDL::black() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_SCISSOR_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void LiveSDL::setTitle(const char* title) {
    SDL_SetWindowTitle(W, title);
}

void LiveSDL::viewport(uint32_t* x, uint32_t* y, uint32_t* vw, uint32_t* vh) const {
    *vw = w;
    *vh = w * 3 / 4;
    if (*vh > h) {
        *vh = h;
        *vw = h * 4 / 3;
    }
    *x = (w - *vw) / 2;
    *y = (h - *vh) / 2;
}

void LiveSDL::toggleFullscreen() {
    fullscreen = !fullscreen;
    SDL_SetWindowFullscreen(W, fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
}

bool LiveSDL::sendKey(const char* keysymName, bool down) {
    SDL_Keycode k = SDL_GetKeyFromName(keysymName);
    if (k == SDLK_UNKNOWN) {
        return false;
    }
    SDL_Event e = {};
    e.type = down ? SDL_KEYDOWN : SDL_KEYUP;
    e.key.state = down ? SDL_PRESSED : SDL_RELEASED;
    e.key.keysym.sym = k;
    e.key.keysym.scancode = SDL_GetScancodeFromKey(k);
    e.key.windowID = SDL_GetWindowID(W);
    e.key.padding2 = 1; /* marks it synthetic for the log in live_pump */
    return SDL_PushEvent(&e) == 1;
}

/* ---------------- input (the Linux build's mapping, live_window.cpp) ---------------- */

enum : uint16_t {
    B_A = 0x8000, B_B = 0x4000, B_Z = 0x2000, B_START = 0x1000, D_U = 0x0800, D_D = 0x0400, D_L = 0x0200,
    D_R = 0x0100, B_L = 0x0020, B_R = 0x0010, C_U = 0x0008, C_D = 0x0004, C_L = 0x0002, C_R = 0x0001
};

void LiveInput::key(int ks, bool isDown) {
    uint16_t bit = 0;
    switch (ks) {
        case SDLK_LEFT: case SDLK_a: left = isDown; return;
        case SDLK_RIGHT: case SDLK_d: right = isDown; return;
        case SDLK_UP: case SDLK_w: up = isDown; return;
        case SDLK_DOWN: case SDLK_s: down = isDown; return;
        case SDLK_LSHIFT: case SDLK_RSHIFT: shift = isDown; return;
        case SDLK_ESCAPE: if (isDown) quit = true; return;
        case SDLK_x: case SDLK_SPACE: bit = B_A; break;
        case SDLK_z: bit = B_B; break;
        case SDLK_c: bit = B_Z; break;
        case SDLK_RETURN: case SDLK_KP_ENTER: bit = B_START; break;
        case SDLK_q: bit = B_L; break;
        case SDLK_e: bit = B_R; break;
        case SDLK_i: bit = C_U; break;
        case SDLK_k: bit = C_D; break;
        case SDLK_j: bit = C_L; break;
        case SDLK_l: bit = C_R; break;
        case SDLK_t: bit = D_U; break;
        case SDLK_g: bit = D_D; break;
        case SDLK_f: bit = D_L; break;
        case SDLK_h: bit = D_R; break;
        default: return;
    }
    keys = isDown ? (uint16_t)(keys | bit) : (uint16_t)(keys & ~bit);
}

void LiveInput::releaseAll() {
    left = right = up = down = shift = false;
    keys = 0;
}

void LiveInput::openJoystick(const char*) {
    if (ctl != nullptr || joy != nullptr) {
        return;
    }
    for (int i = 0; i < SDL_NumJoysticks(); i++) {
        if (SDL_IsGameController(i)) {
            ctl = SDL_GameControllerOpen(i);
            if (ctl != nullptr) {
                std::printf("gamepad: %s (game controller)\n", SDL_GameControllerName((SDL_GameController*)ctl));
                std::fflush(stdout);
                return;
            }
        }
    }
    if (SDL_NumJoysticks() > 0) {
        joy = SDL_JoystickOpen(0);
        if (joy != nullptr) {
            std::printf("gamepad: %s (joystick, raw layout)\n", SDL_JoystickName((SDL_Joystick*)joy));
            std::fflush(stdout);
        }
    }
}

/* axes 0..7 and buttons in the Linux js layout that pad() maps (live_window.cpp): left X / Y, left
 * trigger, right X / Y, right trigger, hat X / Y; a trigger at rest is -32767 */
void LiveInput::pollJoystick() {
    jsButtons = 0;
    if (ctl != nullptr) {
        SDL_GameController* c = (SDL_GameController*)ctl;
        if (!SDL_GameControllerGetAttached(c)) {
            SDL_GameControllerClose(c);
            ctl = nullptr;
            std::fill(axis, axis + 8, 0);
            return;
        }
        auto ax = [c](SDL_GameControllerAxis a) { return (int)SDL_GameControllerGetAxis(c, a); };
        auto bt = [c](SDL_GameControllerButton b) { return SDL_GameControllerGetButton(c, b) != 0; };
        axis[0] = ax(SDL_CONTROLLER_AXIS_LEFTX);
        axis[1] = ax(SDL_CONTROLLER_AXIS_LEFTY);
        axis[2] = ax(SDL_CONTROLLER_AXIS_TRIGGERLEFT) * 2 - 32767;
        axis[3] = ax(SDL_CONTROLLER_AXIS_RIGHTX);
        axis[4] = ax(SDL_CONTROLLER_AXIS_RIGHTY);
        axis[5] = ax(SDL_CONTROLLER_AXIS_TRIGGERRIGHT) * 2 - 32767;
        axis[6] = bt(SDL_CONTROLLER_BUTTON_DPAD_LEFT) ? -32767 : bt(SDL_CONTROLLER_BUTTON_DPAD_RIGHT) ? 32767 : 0;
        axis[7] = bt(SDL_CONTROLLER_BUTTON_DPAD_UP) ? -32767 : bt(SDL_CONTROLLER_BUTTON_DPAD_DOWN) ? 32767 : 0;
        /* the js map: 0 A, 1 B, 2 X, 3 Y, 4 LB, 5 RB, 6 Back, 7 Start */
        jsButtons |= bt(SDL_CONTROLLER_BUTTON_A) ? B_A : 0;
        jsButtons |= bt(SDL_CONTROLLER_BUTTON_B) ? B_B : 0;
        jsButtons |= bt(SDL_CONTROLLER_BUTTON_X) ? B_B : 0;
        jsButtons |= bt(SDL_CONTROLLER_BUTTON_Y) ? C_U : 0;
        jsButtons |= bt(SDL_CONTROLLER_BUTTON_LEFTSHOULDER) ? B_Z : 0;
        jsButtons |= bt(SDL_CONTROLLER_BUTTON_RIGHTSHOULDER) ? B_R : 0;
        jsButtons |= bt(SDL_CONTROLLER_BUTTON_BACK) ? B_L : 0;
        jsButtons |= bt(SDL_CONTROLLER_BUTTON_START) ? B_START : 0;
    } else if (joy != nullptr) {
        static const uint16_t map[] = { B_A, B_B, B_B, C_U, B_Z, B_R, B_L, B_START };
        SDL_Joystick* j = (SDL_Joystick*)joy;
        if (!SDL_JoystickGetAttached(j)) {
            SDL_JoystickClose(j);
            joy = nullptr;
            std::fill(axis, axis + 8, 0);
            return;
        }
        int na = std::min(8, SDL_JoystickNumAxes(j));
        for (int i = 0; i < 8; i++) {
            axis[i] = i < na ? SDL_JoystickGetAxis(j, i) : 0;
        }
        if (SDL_JoystickNumHats(j) > 0) {
            Uint8 hat = SDL_JoystickGetHat(j, 0);
            axis[6] = (hat & SDL_HAT_LEFT) ? -32767 : (hat & SDL_HAT_RIGHT) ? 32767 : 0;
            axis[7] = (hat & SDL_HAT_UP) ? -32767 : (hat & SDL_HAT_DOWN) ? 32767 : 0;
        }
        int nb = std::min(8, SDL_JoystickNumButtons(j));
        for (int i = 0; i < nb; i++) {
            jsButtons |= SDL_JoystickGetButton(j, i) ? map[i] : 0;
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
    if (ctl != nullptr || joy != nullptr) {
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

int live_pump(LiveSDL& w, LiveInput& in, uint32_t step) {
    int n = 0;
    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
        n++;
        switch (ev.type) {
            case SDL_KEYDOWN:
            case SDL_KEYUP: {
                const bool down = ev.type == SDL_KEYDOWN;
                if (ev.key.padding2 == 1) {
                    std::printf("%s: synthetic %s %s (step %u)\n", w.tag, SDL_GetKeyName(ev.key.keysym.sym),
                                down ? "down" : "up", step);
                }
                if (down && ev.key.repeat == 0 && (ev.key.keysym.sym == SDLK_F11 ||
                                                   (ev.key.keysym.sym == SDLK_RETURN && (ev.key.keysym.mod & KMOD_ALT)))) {
                    w.toggleFullscreen();
                    break;
                }
                if (ev.key.repeat == 0) {
                    in.key(ev.key.keysym.sym, down);
                }
                break;
            }
            case SDL_WINDOWEVENT:
                if (ev.window.event == SDL_WINDOWEVENT_SIZE_CHANGED || ev.window.event == SDL_WINDOWEVENT_RESIZED) {
                    int dw = 0, dh = 0;
                    SDL_GL_GetDrawableSize((SDL_Window*)w.win, &dw, &dh);
                    w.w = (uint32_t)std::max(1, dw);
                    w.h = (uint32_t)std::max(1, dh);
                } else if (ev.window.event == SDL_WINDOWEVENT_FOCUS_LOST) {
                    in.releaseAll();
                } else if (ev.window.event == SDL_WINDOWEVENT_CLOSE) {
                    in.quit = true;
                }
                break;
            case SDL_QUIT:
                in.quit = true;
                break;
            case SDL_CONTROLLERDEVICEADDED:
            case SDL_JOYDEVICEADDED:
                in.openJoystick(nullptr);
                break;
            default:
                break;
        }
    }
    return n;
}
