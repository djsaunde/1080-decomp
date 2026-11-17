/* port/gfx/fast3d: definitions for the Ship framework shim (shim/README). Plan 19.6.
 *
 * The replayer feeds Fast3D raw display lists only, so the resource manager serves
 * one thing: the GLSL shader templates LUS's OpenGL backend loads by path
 * ("shaders/opengl/default.shader.glsl"), read from LUS_FAST_DIR (libultraship's
 * src/fast, baked in at build time, overridable by the environment). Every OTR
 * resource lookup misses (nullptr), which Fast3D treats as "not a resource path". */

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <memory>
#include <sstream>
#include <string>

#include "ship/config/ConsoleVariable.h"
#include "ship/resource/ResourceManager.h"
#include "ship/resource/factory/ShaderFactory.h"

#ifndef LUS_FAST_DIR
#error "build with -DLUS_FAST_DIR=\"<libultraship>/src/fast\""
#endif

namespace Ship {

static std::string fast_dir() {
    const char* e = std::getenv("LUS_FAST_DIR");
    return e != nullptr ? e : LUS_FAST_DIR;
}

std::shared_ptr<IResource> ResourceManager::LoadResource(const std::string& path, bool,
                                                         std::shared_ptr<ResourceInitData> initData) {
    if (initData == nullptr || initData->Type != (uint32_t)ResourceType::Shader) {
        return nullptr;
    }
    std::ifstream f(fast_dir() + "/" + path, std::ios::binary);
    if (!f) {
        std::fprintf(stderr, "[fast3d shim] no shader %s/%s\n", fast_dir().c_str(), path.c_str());
        return nullptr;
    }
    std::ostringstream o;
    o << f.rdbuf();
    initData->Identifier.Path = path;
    auto s = std::make_shared<Shader>(initData);
    s->Data = o.str();
    return s;
}

std::shared_ptr<IResource> ResourceManager::LoadResourceProcess(const std::string&) {
    return nullptr;
}

void* ResourceManager::GetResourceRawPointer(const char*) {
    return nullptr;
}

void* ResourceManager::GetResourceRawPointer(uint64_t) {
    return nullptr;
}

std::shared_ptr<ArchiveManager> ResourceManager::GetArchiveManager() {
    static auto sArchives = std::make_shared<ArchiveManager>();
    return sArchives;
}

bool ResourceManager::OtrSignatureCheck(const char*) {
    return false;
}

const char* ArchiveManager::HashToCString(uint64_t) {
    return nullptr;
}

static const char* cvar_env(const char* name) {
    std::string k = std::string("LUS_CVAR_") + name;
    return std::getenv(k.c_str());
}

int32_t ConsoleVariable::GetInteger(const char* name, int32_t defaultValue) {
    const char* v = cvar_env(name);
    return v != nullptr ? (int32_t)std::strtol(v, nullptr, 0) : defaultValue;
}

float ConsoleVariable::GetFloat(const char* name, float defaultValue) {
    const char* v = cvar_env(name);
    return v != nullptr ? std::strtof(v, nullptr) : defaultValue;
}

} // namespace Ship
