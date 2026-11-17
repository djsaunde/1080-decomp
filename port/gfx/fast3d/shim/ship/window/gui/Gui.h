#pragma once
/* shim (port/gfx/fast3d): no ImGui in the replayer. The real Gui.h pulls in the
 * resource manager and spdlog transitively; the backends rely on that. */
#include "imconfig.h"
#include "ship/resource/ResourceManager.h"
#include <spdlog/spdlog.h>
