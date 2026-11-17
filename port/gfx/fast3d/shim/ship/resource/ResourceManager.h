#pragma once
/* shim (port/gfx/fast3d): the ResourceManager calls Fast3D makes. The replayer has no
 * archives: OTR-path resources resolve to nullptr, and LoadResource serves the GLSL
 * shader templates from libultraship's src/fast/shaders (Fast3dShaderDir()). */
#include <memory>
#include <string>
#include "ship/resource/Resource.h"

namespace Ship {
class ArchiveManager {
  public:
    const char* HashToCString(uint64_t hash);
};

class ResourceManager {
  public:
    std::shared_ptr<IResource> LoadResource(const std::string& path, bool sync = false,
                                            std::shared_ptr<ResourceInitData> initData = nullptr);
    std::shared_ptr<IResource> LoadResourceProcess(const std::string& path);
    void* GetResourceRawPointer(const char* path);
    void* GetResourceRawPointer(uint64_t hash);
    std::shared_ptr<ArchiveManager> GetArchiveManager();
    bool OtrSignatureCheck(const char* imgData);
};
} // namespace Ship
