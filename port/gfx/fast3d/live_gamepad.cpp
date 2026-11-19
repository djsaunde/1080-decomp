/* port/gfx/fast3d: the Linux gamepads (live_gamepad.h, PC port plan 19.12 / 27). */
#include "live_gamepad.h"

#include <dirent.h>
#include <fcntl.h>
#include <linux/input.h>
#include <linux/joystick.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <cerrno>
#include <cstdio>
#include <cstring>
#include <string>

namespace {
const int kAbs[8] = { ABS_X, ABS_Y, ABS_Z, ABS_RX, ABS_RY, ABS_RZ, ABS_HAT0X, ABS_HAT0Y };
bool test_bit(const unsigned long* bits, int n) {
    return (bits[n / (8 * sizeof(long))] >> (n % (8 * sizeof(long)))) & 1;
}
} // namespace

int EvdevPad::openAll(EvdevPad* out, int max) {
    DIR* d = opendir("/dev/input");
    if (d == nullptr) {
        return 0;
    }
    int n = 0;
    struct dirent* e;
    while (n < max && (e = readdir(d)) != nullptr) {
        if (std::strncmp(e->d_name, "event", 5) != 0) {
            continue;
        }
        std::string path = std::string("/dev/input/") + e->d_name;
        int f = ::open(path.c_str(), O_RDONLY | O_NONBLOCK);
        if (f < 0) {
            continue;
        }
        unsigned long keys[KEY_MAX / (8 * sizeof(long)) + 1] = {}, abs[ABS_MAX / (8 * sizeof(long)) + 1] = {};
        ioctl(f, EVIOCGBIT(EV_KEY, sizeof(keys)), keys);
        ioctl(f, EVIOCGBIT(EV_ABS, sizeof(abs)), abs);
        if (!test_bit(keys, BTN_GAMEPAD) || !test_bit(abs, ABS_X) || !test_bit(abs, ABS_Y)) {
            ::close(f);
            continue;
        }
        EvdevPad& p = out[n];
        for (int i = 0; i < 8; i++) {
            struct input_absinfo ai = {};
            p.lo[i] = -1;
            p.hi[i] = 1;
            if (test_bit(abs, kAbs[i]) && ioctl(f, EVIOCGABS(kAbs[i]), &ai) == 0 && ai.maximum > ai.minimum) {
                p.lo[i] = ai.minimum;
                p.hi[i] = ai.maximum;
            }
        }
        char name[128] = "?";
        ioctl(f, EVIOCGNAME(sizeof(name)), name);
        std::printf("presenter: gamepad %d = %s (evdev): %s\n", n, path.c_str(), name);
        p.fd = f;
        n++;
    }
    closedir(d);
    return n;
}

void EvdevPad::poll(PortGamepad* g) {
    if (fd < 0) {
        g->connected = 0;
        return;
    }
    g->connected = 1;
    struct input_event ev;
    ssize_t r;
    while ((r = ::read(fd, &ev, sizeof(ev))) == (ssize_t)sizeof(ev)) {
        if (ev.type == EV_ABS) {
            for (int i = 0; i < 8; i++) {
                if (ev.code == kAbs[i]) {
                    long v = (long)(ev.value - lo[i]) * 65534 / (hi[i] - lo[i]) - 32767;
                    g->axis[i] = (int)(v < -32767 ? -32767 : v > 32767 ? 32767 : v);
                }
            }
        } else if (ev.type == EV_KEY) {
            int b = -1;
            switch (ev.code) {
                case BTN_SOUTH: b = 0; break;
                case BTN_EAST: b = 1; break;
                case BTN_WEST: b = 2; break;
                case BTN_NORTH: b = 3; break;
                case BTN_TL: b = 4; break;
                case BTN_TR: b = 5; break;
                case BTN_SELECT: b = 6; break;
                case BTN_START: b = 7; break;
                case BTN_MODE: b = 8; break;
                case BTN_THUMBL: b = 9; break;
                case BTN_THUMBR: b = 10; break;
                case BTN_DPAD_UP: b = 11; break;
                case BTN_DPAD_DOWN: b = 12; break;
                case BTN_DPAD_LEFT: b = 13; break;
                case BTN_DPAD_RIGHT: b = 14; break;
                default: break;
            }
            if (b >= 0) {
                bits = ev.value ? (bits | (1u << b)) : (bits & ~(1u << b));
            }
        }
    }
    if (r < 0 && errno != EAGAIN) { /* unplugged */
        ::close(fd);
        fd = -1;
        bits = 0;
        g->connected = 0;
    }
    g->buttons = bits;
}

void LivePads::open(const char* jsPath) {
    js[0] = ::open(jsPath, O_RDONLY | O_NONBLOCK);
    if (js[0] >= 0) {
        std::string base = jsPath;
        for (int i = 0; i < PORT_MAX_GAMEPADS; i++) {
            if (i > 0) {
                if (base.empty() || base.back() != '0') {
                    break;
                }
                std::string p = base.substr(0, base.size() - 1) + std::to_string(i);
                js[i] = ::open(p.c_str(), O_RDONLY | O_NONBLOCK);
                if (js[i] < 0) {
                    continue;
                }
            }
            char name[128] = "?";
            ioctl(js[i], JSIOCGNAME(sizeof(name)), name);
            if (i == 0) {
                std::printf("gamepad %s: %s\n", jsPath, name);
            } else {
                std::printf("gamepad %d = %s%d: %s\n", i, base.substr(0, base.size() - 1).c_str(), i, name);
            }
        }
    } else {
        EvdevPad::openAll(ev, PORT_MAX_GAMEPADS);
    }
}

void LivePads::poll() {
    for (int i = 0; i < PORT_MAX_GAMEPADS; i++) {
        PortGamepad* g = &st[i];
        if (js[i] >= 0) {
            g->connected = 1;
            js_event e;
            ssize_t r;
            while ((r = ::read(js[i], &e, sizeof(e))) == (ssize_t)sizeof(e)) {
                uint8_t t = e.type & ~JS_EVENT_INIT;
                if (t == JS_EVENT_AXIS && e.number < 8) {
                    g->axis[e.number] = e.value;
                } else if (t == JS_EVENT_BUTTON && e.number < 16) {
                    g->buttons = e.value ? (g->buttons | (1u << e.number)) : (g->buttons & ~(1u << e.number));
                }
            }
        } else if (ev[i].fd >= 0) {
            ev[i].poll(g);
        } else {
            g->connected = 0;
        }
        port_input_set_gamepad(i, virtOn[i] ? &virt[i] : g);
    }
}
