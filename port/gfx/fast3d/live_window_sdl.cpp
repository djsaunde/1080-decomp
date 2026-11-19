/* port/gfx/fast3d: the live window and its input over SDL2, for the Windows build (live_window.h, plan
 * section 29) and the macOS one (plan 30). The interface and the key / pad mapping are the X11 build's (live_window.cpp). */
#include "live_window.h"
#include "live_audio.h"
#include "settings_menu.h"

#define SDL_MAIN_HANDLED 1
#include <SDL2/SDL.h>

#include <algorithm>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "port_gl.h"

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
     * shaders and its client-side state work in it. macOS (plan 30) has no compatibility profile past 2.1: a 4.1
     * core context, for which gfx_opengl uses GLSL 4.10 and a vertex array object (its __APPLE__ paths) */
#ifdef __APPLE__
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
#endif
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
    if (fullscreen) { /* the settings asked for it before the window existed (native_host.cpp) */
        SDL_SetWindowFullscreen(W, SDL_WINDOW_FULLSCREEN_DESKTOP);
    }
    ctx = SDL_GL_CreateContext(W);
    if (ctx == nullptr) {
        std::fprintf(stderr, "%s: no OpenGL context: %s (update the graphics driver)\n", tag, SDL_GetError());
        return false;
    }
    SDL_GL_MakeCurrent(W, (SDL_GLContext)ctx);
#ifdef PORT_WIN32
    if (port_gl_load(gl_getproc) != 0) {
        std::fprintf(stderr, "%s: the OpenGL driver lacks entry points the renderer needs (above)\n", tag);
        return false;
    }
#else
    (void)gl_getproc;
#endif
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

void LiveSDL::setVsync(int interval) {
    SDL_GL_SetSwapInterval(interval);
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
    if (win == nullptr) {
        return;
    }
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

/* ---------------- input: the portable mapping (port/input_map.h), as live_window.cpp ---------------- */

/* an SDL keycode -> its X keysym name (input_map.h): what the settings file and the menu bind */
static const char* keysym_name(SDL_Keycode k) {
    static char one[2];
    if ((k >= SDLK_a && k <= SDLK_z) || (k >= SDLK_0 && k <= SDLK_9)) {
        one[0] = (char)k;
        one[1] = '\0';
        return one;
    }
    if (k >= SDLK_F1 && k <= SDLK_F12) {
        static char f[4];
        std::snprintf(f, sizeof(f), "F%d", (int)(k - SDLK_F1) + 1);
        return f;
    }
    if (k >= SDLK_KP_1 && k <= SDLK_KP_9) {
        static char kp[5];
        std::snprintf(kp, sizeof(kp), "KP_%d", (int)(k - SDLK_KP_1) + 1);
        return kp;
    }
    switch (k) {
        case SDLK_RETURN: return "Return";
        case SDLK_KP_ENTER: return "KP_Enter";
        case SDLK_ESCAPE: return "Escape";
        case SDLK_SPACE: return "space";
        case SDLK_TAB: return "Tab";
        case SDLK_BACKSPACE: return "BackSpace";
        case SDLK_DELETE: return "Delete";
        case SDLK_INSERT: return "Insert";
        case SDLK_HOME: return "Home";
        case SDLK_END: return "End";
        case SDLK_PAGEUP: return "Prior";
        case SDLK_PAGEDOWN: return "Next";
        case SDLK_UP: return "Up";
        case SDLK_DOWN: return "Down";
        case SDLK_LEFT: return "Left";
        case SDLK_RIGHT: return "Right";
        case SDLK_LSHIFT: return "Shift_L";
        case SDLK_RSHIFT: return "Shift_R";
        case SDLK_LCTRL: return "Control_L";
        case SDLK_RCTRL: return "Control_R";
        case SDLK_LALT: return "Alt_L";
        case SDLK_RALT: return "Alt_R";
        case SDLK_CAPSLOCK: return "Caps_Lock";
        case SDLK_COMMA: return "comma";
        case SDLK_PERIOD: return "period";
        case SDLK_SLASH: return "slash";
        case SDLK_BACKSLASH: return "backslash";
        case SDLK_MINUS: return "minus";
        case SDLK_EQUALS: return "equal";
        case SDLK_SEMICOLON: return "semicolon";
        case SDLK_QUOTE: return "apostrophe";
        case SDLK_BACKQUOTE: return "grave";
        case SDLK_LEFTBRACKET: return "bracketleft";
        case SDLK_RIGHTBRACKET: return "bracketright";
        case SDLK_KP_0: return "KP_0";
        case SDLK_KP_PLUS: return "KP_Add";
        case SDLK_KP_MINUS: return "KP_Subtract";
        case SDLK_KP_MULTIPLY: return "KP_Multiply";
        case SDLK_KP_DIVIDE: return "KP_Divide";
        case SDLK_KP_PERIOD: return "KP_Decimal";
        default: return nullptr;
    }
}

void LiveInput::key(const char* name, bool isDown) {
    port_input_key(name, isDown ? 1 : 0);
}

void LiveInput::releaseAll() {
    port_input_release_keys();
}

void LiveInput::openJoystick(const char* path) {
    pads.open(path);
}

void LiveInput::pollJoystick() {
    pads.poll();
    if (menu != nullptr) {
        menu->tick();
    }
}

void LiveInput::allPads(LivePad out[LIVE_PADS]) const {
    PortN64Pad p[PORT_SETTINGS_PLAYERS];
    port_input_pads(port_settings(), p);
    for (int i = 0; i < LIVE_PADS; i++) {
        out[i] = LivePad{};
        if (i < PORT_SETTINGS_PLAYERS) {
            out[i].button = p[i].button;
            out[i].stick_x = p[i].stick_x;
            out[i].stick_y = p[i].stick_y;
            out[i].connected = p[i].connected;
        }
    }
}

LivePad LiveInput::pad() const {
    LivePad p[LIVE_PADS];
    allPads(p);
    return p[0];
}

/* every pad plugged in that is not open yet, into the free slots (in SDL's order) */
void LivePads::open(const char*) {
    for (int d = 0; d < SDL_NumJoysticks(); d++) {
        SDL_JoystickID jid = SDL_JoystickGetDeviceInstanceID(d);
        bool have = false;
        int freeSlot = -1;
        for (int i = 0; i < PORT_MAX_GAMEPADS; i++) {
            have |= id[i] == jid;
            if (freeSlot < 0 && ctl[i] == nullptr && joy[i] == nullptr) {
                freeSlot = i;
            }
        }
        if (have || freeSlot < 0) {
            continue;
        }
        if (SDL_IsGameController(d)) {
            ctl[freeSlot] = SDL_GameControllerOpen(d);
        }
        if (ctl[freeSlot] == nullptr) {
            joy[freeSlot] = SDL_JoystickOpen(d);
        }
        if (ctl[freeSlot] != nullptr || joy[freeSlot] != nullptr) {
            id[freeSlot] = jid;
            std::printf("gamepad %d: %s (%s)\n", freeSlot,
                        ctl[freeSlot] ? SDL_GameControllerName((SDL_GameController*)ctl[freeSlot])
                                      : SDL_JoystickName((SDL_Joystick*)joy[freeSlot]),
                        ctl[freeSlot] ? "game controller" : "joystick, raw layout");
            std::fflush(stdout);
        }
    }
}

/* the js layout of input_map.h: axes left X / Y, LT, right X / Y, RT, hat X / Y (a trigger at rest -32767);
 * buttons 0 A, 1 B, 2 X, 3 Y, 4 LB, 5 RB, 6 Back, 7 Start, 8 Guide, 9 / 10 the sticks, 11..14 the D-pad */
void LivePads::poll() {
    for (int i = 0; i < PORT_MAX_GAMEPADS; i++) {
        PortGamepad* g = &st[i];
        *g = PortGamepad{};
        if (ctl[i] != nullptr) {
            SDL_GameController* c = (SDL_GameController*)ctl[i];
            if (!SDL_GameControllerGetAttached(c)) {
                SDL_GameControllerClose(c);
                ctl[i] = nullptr;
                id[i] = -1;
            } else {
                auto ax = [c](SDL_GameControllerAxis a) { return (int)SDL_GameControllerGetAxis(c, a); };
                auto bt = [c](SDL_GameControllerButton b) { return SDL_GameControllerGetButton(c, b) != 0; };
                g->connected = 1;
                g->axis[0] = std::max(-32767, ax(SDL_CONTROLLER_AXIS_LEFTX));
                g->axis[1] = std::max(-32767, ax(SDL_CONTROLLER_AXIS_LEFTY));
                g->axis[2] = ax(SDL_CONTROLLER_AXIS_TRIGGERLEFT) * 2 - 32767;
                g->axis[3] = std::max(-32767, ax(SDL_CONTROLLER_AXIS_RIGHTX));
                g->axis[4] = std::max(-32767, ax(SDL_CONTROLLER_AXIS_RIGHTY));
                g->axis[5] = ax(SDL_CONTROLLER_AXIS_TRIGGERRIGHT) * 2 - 32767;
                g->axis[6] = bt(SDL_CONTROLLER_BUTTON_DPAD_LEFT) ? -32767 : bt(SDL_CONTROLLER_BUTTON_DPAD_RIGHT) ? 32767 : 0;
                g->axis[7] = bt(SDL_CONTROLLER_BUTTON_DPAD_UP) ? -32767 : bt(SDL_CONTROLLER_BUTTON_DPAD_DOWN) ? 32767 : 0;
                static const SDL_GameControllerButton kB[] = {
                    SDL_CONTROLLER_BUTTON_A, SDL_CONTROLLER_BUTTON_B, SDL_CONTROLLER_BUTTON_X, SDL_CONTROLLER_BUTTON_Y,
                    SDL_CONTROLLER_BUTTON_LEFTSHOULDER, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER, SDL_CONTROLLER_BUTTON_BACK,
                    SDL_CONTROLLER_BUTTON_START, SDL_CONTROLLER_BUTTON_GUIDE, SDL_CONTROLLER_BUTTON_LEFTSTICK,
                    SDL_CONTROLLER_BUTTON_RIGHTSTICK
                };
                for (int b = 0; b < 11; b++) {
                    g->buttons |= bt(kB[b]) ? (1u << b) : 0;
                }
            }
        } else if (joy[i] != nullptr) {
            SDL_Joystick* j = (SDL_Joystick*)joy[i];
            if (!SDL_JoystickGetAttached(j)) {
                SDL_JoystickClose(j);
                joy[i] = nullptr;
                id[i] = -1;
            } else {
                g->connected = 1;
                int na = std::min(8, SDL_JoystickNumAxes(j));
                for (int a = 0; a < na; a++) {
                    g->axis[a] = std::max(-32767, (int)SDL_JoystickGetAxis(j, a));
                }
                if (SDL_JoystickNumHats(j) > 0) {
                    Uint8 hat = SDL_JoystickGetHat(j, 0);
                    g->axis[6] = (hat & SDL_HAT_LEFT) ? -32767 : (hat & SDL_HAT_RIGHT) ? 32767 : 0;
                    g->axis[7] = (hat & SDL_HAT_UP) ? -32767 : (hat & SDL_HAT_DOWN) ? 32767 : 0;
                }
                int nb = std::min(16, SDL_JoystickNumButtons(j));
                for (int b = 0; b < nb; b++) {
                    g->buttons |= SDL_JoystickGetButton(j, b) ? (1u << b) : 0;
                }
            }
        }
        port_input_set_gamepad(i, virtOn[i] ? &virt[i] : g);
    }
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
                const SDL_Keycode k = ev.key.keysym.sym;
                const char* name = keysym_name(k);
                if (ev.key.padding2 == 1) {
                    std::printf("%s: synthetic %s %s (step %u)\n", w.tag, name ? name : "?", down ? "down" : "up", step);
                }
                if (down && ev.key.repeat == 0 && (k == SDLK_F11 || (k == SDLK_RETURN && (ev.key.keysym.mod & KMOD_ALT)))) {
                    w.toggleFullscreen();
                    break;
                }
                if (name == nullptr || (ev.key.repeat != 0 && !(in.menu != nullptr && in.menu->isOpen()))) {
                    break; /* no name: nothing is bound to it; auto-repeat only drives the menu */
                }
                if (in.menu != nullptr && (in.menu->isOpen() || (down && k == SDLK_F1))) {
                    if (!in.menu->isOpen()) {
                        in.menu->toggle();
                    } else {
                        in.menu->key(name, down);
                    }
                } else if (down && k == SDLK_ESCAPE) {
                    in.quit = true;
                } else {
                    in.key(name, down);
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
