# libultraship patch series for the 1080 port

These patches apply to **libultraship (LUS) commit `759b13401cd6903daa67a00a753066f791622e78`**
(<https://github.com/Kenix3/libultraship>, 2026-09-02, "Load TCC-compiled scripts in-memory to avoid
antivirus false-positives (#1266)"). They change only the Fast3D interpreter (`src/fast/`, `include/fast/`).
Why each change is needed: docs/1080_PC_PORT_PLAN.md, section 11 (graphics coverage) and section 14 (M2 prep).

Apply them in order, on top of the pinned commit:

```sh
git -C <libultraship> checkout 759b13401cd6903daa67a00a753066f791622e78
git -C <libultraship> am  "$PWD"/port/third_party/libultraship-patches/*.patch   # keeps the commits
# or, without commits:
git -C <libultraship> apply "$PWD"/port/third_party/libultraship-patches/*.patch
```

The files were made with `git format-patch --zero-commit --no-signature 759b134`. To change a patch, apply the
series in a scratch clone, edit or fix up the commit, and run `format-patch` again over the whole series.
Keep the series small and in libultraship's own style (`.clang-format`: 4 spaces, 120 columns), so that it
rebases easily when the pin moves.

## The patches

| # | file | what it does |
|---|---|---|
| 1 | `0001-fast-G_LINE3D-for-F3DEX-1.x-ucode_f3dex_line3d.patch` | Adds the `ucode_f3dex_line3d` ucode id: the F3DEX table with **0xB5 decoded as G_LINE3D** (the L3DEX meaning) instead of G_QUAD. Adds `Interpreter::GfxSpLine3D(v0, v1, width)`, which draws the line as a quad between the two loaded vertices, extruded perpendicular to the line on screen. Its thickness is 1.5 + width / 2 native pixels (the gbi.h definition of `gSPLineW3D`), scaled by the vertical output ratio. Culling is off for lines. A line with an endpoint behind the eye (w <= 0) is dropped (no near clip yet). The enum value is appended, so the existing ucode ids (stored in OTR display-list resources) do not move. |
| 2 | `0002-fast-read-the-cycle-1-blender-fields-for-invisible-d.patch` | The "invisible" blender test (`A = 0, M = MEM`, a Z-only write) reads the **cycle 1** fields (bits 26 / 22) in 1-cycle mode, and the cycle 2 fields (bits 24 / 20) otherwise, as before. 1080 builds its 1-cycle render modes from one `GBL_c1` word and leaves cycle 2 at zero, so its Z-only mode 0x0C4C3078 was drawn opaque. Render modes that set both cycles to the same value, as the stock `G_RM_*` pairs do, behave exactly as before. |
| 3 | `0003-fast-deferred-colour-depth-read-back-API-for-ports.patch` | Adds a **read-back API** for game code that reads the N64 colour or depth buffer from the CPU. Requests are queued before `Run()`. `ServiceReadbacks()` fulfils them at the end of `Run()`, after the frame is drawn and before it is resolved or presented, so the game thread never touches the GPU. The results are ready at the next update: one frame of latency, the same as reading the last completed N64 framebuffer. Colour: `ReadbackRequestRgba5551(x, y, w, h, dst, bswap)` / `gfx_readback_rgba5551`. It returns a region in **native coordinates (320x240)** as RGBA5551, **big-endian when `bswap` is set** (the N64 memory order that the texture importers read back), taken from the native-aspect centre of the game view and scaled to the native size with a nearest-neighbour blit. Depth: `ReadbackRequestDepth(x, y)`, then `ReadbackGetDepth(x, y, &z)` (`gfx_readback_depth_request` / `_get`), batched into one `GetPixelDepth` call. The units are LUS's (the top 14 bits of the 24-bit depth, << 2), so the far plane / cleared value reads **0xFFFC**, the N64 Z clear value that `Sun_measureVisibility` tests for. |
| 4 | `0004-fast-texture-cache-invalidation-hooks-for-CPU-writte.patch` | Adds **texture-cache invalidation hooks**. `TextureCacheInvalidateRange(addr, size)` drops every cached texture whose texels or either palette start inside `[addr, addr + size)`, and marks both loaded tiles changed so the next draw looks them up again. The C entry points are `gfx_texture_cache_invalidate_range(addr, size)` and `gfx_texture_cache_delete(addr)` (the existing exact-address `TextureCacheDelete`). The port calls the range form from the `PORT` body of `gfx_flushTexture` (the `osWritebackDCache` site) and after each CPU texture writer: ScreenThumbMat_grab, Fader_buildTexture, Tv1Tmpl_blitFrame. |
| 5 | `0005-fast-let-a-port-register-low-address-ranges-as-real-.patch` | Adds `gfx_register_raw_address_range(start, size)`. `gfx_set_timg_handler_rdp` drops a G_SETTIMG whose resolved address is at or below 0x0FFFFFFF unless `dladdr` places it in a loaded module (such an address is usually an unresolved segment). The 1080 port's lists point at memory no module owns in that range: in a 32-bit non-PIE process the brk heap follows the executable at 0x08xxxxxx, and the 64-bit replayer (`port/gfx/fast3d/f3dreplay`) rebuilds a dump at its original addresses. Without the patch every texture and palette load from there is silently skipped (found by the golden tests, plan 19.6). Addresses in a registered range pass the check; nothing changes until a port registers one. |

The patches add 275 lines and remove 11 (patch 1: +93 / -6; patch 2: +5 / -2; patch 3: +129 / -2; patch 4:
+33 / -0; patch 5: +15 / -1).

## How the port uses them

- Register the memory the lists point into with `gfx_register_raw_address_range` (patch 5): the game's heap
  / arena in process, every mapped range of the dump in `f3dreplay`.
- Call `gfx_set_target_ucode` **after** `Interpreter::Init`, which resets the ucode to F3DEX2.
- Select the ucode once with `Fast::gfx_set_target_ucode(ucode_f3dex_line3d)`, for both `RcpTask_setUcodeF3DEX`
  and F3DLP.Rej. 1080 emits no G_QUAD (section 11.1), so decoding 0xB5 as a line costs nothing.
- `SetNativeDimensions(320, 240)`, so that the read-back and depth coordinates are the game's own.
- The C entry points are declared `extern "C"` in `include/fast/interpreter.h`, a C++ header. The game is C, so
  `port/gfx/` declares them again in a small C header.
- The results of `gfx_readback_rgba5551` are written into game memory at the end of `Run()`. Call `Run()` on
  the simulation thread (decision 5), or keep `dst` from being read while the frame renders.

## Verification

Run 824 (plan 19.6): the whole series is built and **run** in `port/gfx/fast3d/f3dreplay` (64-bit, EGL, no
SDL / ImGui) and exercised by 11 golden-image scenes (`make -C port/gfx test`), G_LINE3D (patch 1) included.
Patch 5 was made with `format-patch` like the others; patches 1-4 were not regenerated (their only
difference would be the `[PATCH n/5]` subject counter).

What was checked (run 611):
- The series applies cleanly with `git am` to a fresh clone at 759b134. The resulting tree is identical to the
  development branch.
- `src/fast/interpreter.cpp` compiles to an object (`g++ 11.4 -std=c++20 -Wall -Wextra -c`) after each patch.
  It produces no new warnings compared with the unpatched file.
  - LUS's CMake dependencies are not installed on the build machine. The compile used the header-only
    nlohmann/json, spdlog and ImGui / SDL2 headers found locally, plus two stubs: `BS_thread_pool.hpp` (the
    types `ResourceManager.h` names) and `imconfig.h` (`ImTextureID`).
  - It also needed `-DCVAR_MSAA_VALUE` / `-DCVAR_INTERNAL_RESOLUTION`, which the game normally defines.
- `check_logic.cpp` (`g++ -std=c++20 check_logic.cpp && ./a.out`) tests copies of the new expressions. These
  copies are not linked against LUS. The checks:
  - the new invisible test over all 38 constants in gfx_renderModeWord / gfx_renderMode2Word: only
    0x0C4C3078 changes, from visible to invisible, and 2-cycle results are unchanged;
  - the G_LINE3D field decode;
  - the on-screen thickness and edge parallelism of the extruded quad, for w != 1;
  - region clipping and the big-endian store;
  - the invalidation range test.

Not verified:
- Nothing was linked or run: no SDL3, OpenGL or D3D on this machine.
- `DisplayListFactory.cpp` (patch 1 adds one `case` label) was not compiled, because tinyxml2 is missing.
- A `-m32` build: the host has no 32-bit libstdc++.
- Lines, the read-back orientation (row 0 = top) and the depth values need checking on a real backend in M2.
  See "Open items" in docs/1080_PC_PORT_PLAN.md section 14.
