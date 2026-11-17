#pragma once
/* shim (port/gfx/fast3d): Ship::Shader, a resource whose raw pointer is the source text. */
#include <string>
#include "ship/resource/Resource.h"

namespace Ship {
class Shader final : public Resource<void> {
  public:
    using Resource::Resource;
    void* GetPointer() override { return &Data; }
    size_t GetPointerSize() override { return sizeof(std::string); }
    std::string Data;
};
} // namespace Ship
