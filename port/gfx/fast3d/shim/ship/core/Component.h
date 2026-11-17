#pragma once
/* shim (port/gfx/fast3d): Ship::Component, as far as GfxDebugger uses it. */
#include <string>

namespace Ship {
class Component {
  public:
    explicit Component(const std::string& name) : mName(name) {}
    virtual ~Component() = default;
    void MarkInitialized() { mInitialized = true; }
    bool IsInitialized() const { return mInitialized; }

  private:
    std::string mName;
    bool mInitialized = false;
};
} // namespace Ship
