# port/third_party

Third-party code used by the PC port (docs/1080_PC_PORT_PLAN.md). The matching build uses none of it.

## libultraship

- **What:** libultraship (LUS), <https://github.com/Kenix3/libultraship>. The port uses its Fast3D display-list
  interpreter and rendering backends (decision 2, section 11.5 of the plan). It does not use the OTR resource
  pipeline.
- **Pinned commit:** `759b13401cd6903daa67a00a753066f791622e78` (2026-09-02). Every change we need on top of it
  is in `libultraship-patches/`, applied in order (see the README there). Move the pin only together with a
  rebased patch series.
- **Licence:** MIT.
  - The top-level `LICENSE` is "Copyright (c) 2022 kenix3".
  - The Fast3D sources carry a second MIT licence, `src/fast/LICENSE.txt` (also in `include/fast/LICENSE.txt`):
    "Copyright (c) 2020 Emill, MaikelChan".
  - Both notices must ship with any binary distribution of the port: copy both LICENSE files into the release
    archive's `licenses/libultraship/`.
  - Our patches are offered under the same MIT terms.

### How it is pulled in

**Chosen: a git submodule at the pinned commit, with the patches applied at configure time.** A submodule
records the exact commit in our tree, works offline once cloned, and lets `git submodule status` show the pin.
It is not added yet. The command, to be run when M2 starts:

```sh
git submodule add https://github.com/Kenix3/libultraship port/third_party/libultraship
git -C port/third_party/libultraship checkout 759b13401cd6903daa67a00a753066f791622e78
git add .gitmodules port/third_party/libultraship
# clones of the repo then need:  git submodule update --init port/third_party/libultraship
```

The submodule stays pristine. The port's CMake applies the series to it, once per checkout:

```cmake
# port/CMakeLists.txt (sketch)
set(LUS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/libultraship)
file(GLOB LUS_PATCHES ${CMAKE_CURRENT_SOURCE_DIR}/third_party/libultraship-patches/*.patch)
list(SORT LUS_PATCHES)
list(GET LUS_PATCHES -1 LUS_LAST_PATCH)
# "already patched" = the last patch reverse-applies. (--check of several patches at once is not cumulative,
# so test one; a plain `git apply` of the whole list is.)
execute_process(COMMAND git -C ${LUS_DIR} apply --reverse --check ${LUS_LAST_PATCH}
                RESULT_VARIABLE LUS_ALREADY_PATCHED OUTPUT_QUIET ERROR_QUIET)
if(NOT LUS_ALREADY_PATCHED EQUAL 0)
    execute_process(COMMAND git -C ${LUS_DIR} apply ${LUS_PATCHES} COMMAND_ERROR_IS_FATAL ANY)
endif()
add_subdirectory(${LUS_DIR} libultraship)
```

**Alternative: CMake `FetchContent`.** No submodule; CMake clones at configure time and applies the series
with `PATCH_COMMAND`:

```cmake
include(FetchContent)
FetchContent_Declare(libultraship
    GIT_REPOSITORY https://github.com/Kenix3/libultraship
    GIT_TAG        759b13401cd6903daa67a00a753066f791622e78
    PATCH_COMMAND  git apply ${CMAKE_CURRENT_SOURCE_DIR}/third_party/libultraship-patches/0001-fast-G_LINE3D-for-F3DEX-1.x-ucode_f3dex_line3d.patch
                             ${CMAKE_CURRENT_SOURCE_DIR}/third_party/libultraship-patches/0002-fast-read-the-cycle-1-blender-fields-for-invisible-d.patch
                             ${CMAKE_CURRENT_SOURCE_DIR}/third_party/libultraship-patches/0003-fast-deferred-colour-depth-read-back-API-for-ports.patch
                             ${CMAKE_CURRENT_SOURCE_DIR}/third_party/libultraship-patches/0004-fast-texture-cache-invalidation-hooks-for-CPU-writte.patch
    UPDATE_DISCONNECTED TRUE)
FetchContent_MakeAvailable(libultraship)
```

This alternative is simpler for users, but it needs network access at configure time, and the pin then lives
only in CMake. It remains a fallback if submodules prove awkward.

### Build notes for M2

- LUS at this commit is C++20 with CMake >= 3.24. On Linux it needs these installed system packages:
  - **SDL3** (`find_package(SDL3)` in `cmake/dependencies/linux.cmake`);
  - OpenGL;
  - nlohmann_json, tinyxml2 and spdlog (`find_package(... REQUIRED)` in `src/CMakeLists.txt`).

  On Windows it uses vcpkg.
- CMake fetches the rest itself:
  - ImGui, BS::thread_pool, prism, Monocypher and stb_image;
  - libgfxd only with `GFX_DEBUG_DISASSEMBLER`;
  - StormLib only with `INCLUDE_MPQ_SUPPORT` (default OFF).
- Keep `ENABLE_SCRIPTING` OFF (the default). It would pull in TinyCC, which is **LGPL**.
- Before shipping, review the licence of each dependency CMake actually fetches or links. Not yet audited:
  prism-processor's licence.
- `GBI_UCODE` (default `F3DEX_GBI_2`) only selects the GBI macros that LUS itself compiles with. The game's
  lists are decoded by the ucode id we set at run time (`ucode_f3dex_line3d`, patch 1), not by this option.
