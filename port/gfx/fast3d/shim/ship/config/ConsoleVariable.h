#pragma once
/* shim (port/gfx/fast3d): console variables. Every lookup returns its default, except
 * names set in the environment as LUS_CVAR_<name>=<value> (e.g. LUS_CVAR_gMSAAValue). */
#include <cstdint>

namespace Ship {
class ConsoleVariable {
  public:
    int32_t GetInteger(const char* name, int32_t defaultValue);
    float GetFloat(const char* name, float defaultValue);
};
} // namespace Ship
