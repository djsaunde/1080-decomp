/* port/gfx/fast3d: the evdev gamepad (live_gamepad.h, PC port plan 19.12). */
#include "live_gamepad.h"

#include <dirent.h>
#include <fcntl.h>
#include <linux/input.h>
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

bool EvdevPad::open() {
    DIR* d = opendir("/dev/input");
    if (d == nullptr) {
        return false;
    }
    struct dirent* e;
    while (fd < 0 && (e = readdir(d)) != nullptr) {
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
        for (int i = 0; i < 8; i++) {
            struct input_absinfo ai = {};
            lo[i] = -1;
            hi[i] = 1;
            if (test_bit(abs, kAbs[i]) && ioctl(f, EVIOCGABS(kAbs[i]), &ai) == 0 && ai.maximum > ai.minimum) {
                lo[i] = ai.minimum;
                hi[i] = ai.maximum;
            }
        }
        char name[128] = "?";
        ioctl(f, EVIOCGNAME(sizeof(name)), name);
        std::printf("presenter: gamepad %s (evdev): %s\n", path.c_str(), name);
        fd = f;
    }
    closedir(d);
    return fd >= 0;
}

void EvdevPad::poll(int axis[8], uint16_t* buttons) {
    if (fd < 0) {
        return;
    }
    struct input_event ev;
    ssize_t r;
    while ((r = ::read(fd, &ev, sizeof(ev))) == (ssize_t)sizeof(ev)) {
        if (ev.type == EV_ABS) {
            for (int i = 0; i < 8; i++) {
                if (ev.code == kAbs[i]) {
                    long v = (long)(ev.value - lo[i]) * 65534 / (hi[i] - lo[i]) - 32767;
                    axis[i] = (int)(v < -32767 ? -32767 : v > 32767 ? 32767 : v);
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
                default: break;
            }
            if (b >= 0) {
                bits = ev.value ? (uint16_t)(bits | (1u << b)) : (uint16_t)(bits & ~(1u << b));
            }
        }
    }
    if (r < 0 && errno != EAGAIN) { /* unplugged */
        ::close(fd);
        fd = -1;
        bits = 0;
    }
    *buttons = bits;
}
