#pragma once
/* shim (port/gfx/fast3d): Ship::Math::clamp. */
namespace Ship {
namespace Math {
inline float clamp(float d, float min, float max) {
    const float t = d < min ? min : d;
    return t > max ? max : t;
}
} // namespace Math
} // namespace Ship
