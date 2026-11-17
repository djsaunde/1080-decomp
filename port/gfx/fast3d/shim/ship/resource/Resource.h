#pragma once
/* shim (port/gfx/fast3d): Ship::IResource / Resource<T> and the init data. */
#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include "ship/utils/binarytools/endianness.h" /* the real LUS header: Ship::Endianness, BE16SWAP */

#define RESOURCE_FORMAT_BINARY 0
#define RESOURCE_FORMAT_XML 1

namespace Ship {
enum class ResourceType : uint32_t { None = 0, Shader = 0x53484452 };

struct ResourceIdentifier {
    std::string Path;
    const std::string& GetPath() const { return Path; }
};

struct ResourceInitData {
    ResourceIdentifier Identifier;
    Endianness ByteOrder = Endianness::Native;
    uint32_t Type = 0;
    int32_t ResourceVersion = 0;
    uint64_t Id = 0;
    bool IsCustom = false;
    uint32_t Format = RESOURCE_FORMAT_BINARY;
};

class ResourceManager;

class IResource {
  public:
    inline static const std::string gAltAssetPrefix = "alt/";
    IResource(std::shared_ptr<ResourceInitData> initData) : mInitData(std::move(initData)) {}
    virtual ~IResource() = default;
    virtual void* GetRawPointer() = 0;
    virtual size_t GetPointerSize() = 0;
    bool IsDirty() { return mIsDirty; }
    void Dirty() { mIsDirty = true; }
    std::shared_ptr<ResourceInitData> GetInitData() { return mInitData; }

  private:
    std::shared_ptr<ResourceInitData> mInitData;
    bool mIsDirty = false;
};

template <class T> class Resource : public IResource {
  public:
    using IResource::IResource;
    virtual T* GetPointer() = 0;
    void* GetRawPointer() override { return static_cast<void*>(GetPointer()); }
};
} // namespace Ship
