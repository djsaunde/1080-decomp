#pragma once
/* shim (port/gfx/fast3d): the one Window type the backend API names. */
#include <cstdint>

namespace Ship {
struct WindowRect {
    int32_t X, Y;
    uint32_t Width, Height;
};
} // namespace Ship
