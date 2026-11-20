# 1080 Snowboarding n64 decompilation

A matching decompilation of 1080 Snowboarding (USA), mostly driven by Claude. Uses https://github.com/djsaunde/decomp for N64 common tooling, Claude skills, etc.

## Status

**Complete: 100 % matched.**

- Code: 762,652 / 762,652 bytes; data: 1,680 / 1,680 bytes; functions: 3,288 / 3,288.
- `make` builds a `tenshoe.z64` byte-identical to the original ROM (md5 `fa27089c425dbab99f19245c5c997613`). `make verify` (part of the default target) and `make verify-blocks` (the six Yay0-compressed code blocks, compared decompressed) hard-fail on any difference.
- A match means C or C++ source compiles to the original bytes; no instruction bytes are patched in after compilation.

Toolchains, per file as the original was built:

- **IDO 7.1** (`cc`, mostly `-O2` for game code, `-O1` for most libultra) for the bulk of the C;
- **IDO 5.3** for a set of libultra/libc objects (`*_ido53_*` files);
- the IDO 7.1 **NCC** C++ front end for the C++ sources (`*.cpp`);
- the **MIPSpro 7.2.1** C++ front end (EDG, with the IDO 7.1 back end; `*_mp72_*` files, `tools/ncc721.sh`) for a few functions.

USO overlays ship in a custom relocatable format (some Yay0-compressed); they are matched on their pre-relocation bytes by splatting at `VRAM=0` with per-segment symbol prefixes (`gl_func_`, `gui_func_`, etc.).

| Segment | Functions | Code bytes | Notes |
|---|---|---|---|
| `kernel` | 197 | 40,288 | libultra + USO loader + audio (`0x80000000`) |
| `bootup_uso` | 349 | 83,512 | Giles Goddard's libgdl engine (C++) |
| `game_libs` | 1,891 | 418,428 | Support libraries (incl. statically-linked libultra) |
| `gui_uso` | 39 | 18,564 | GUI/menu helpers |
| `titproc_uso` | 46 | 10,776 | Title-screen process |
| `arcproc_uso` | 59 | 10,620 | Arcade-mode process |
| `h2hproc_uso` | 40 | 6,960 | Head-to-head process |
| `eddproc_uso` | 15 | 1,148 | Edit-mode process |
| `n64proc_uso` | 7 | 1,064 | N64-specific process |
| `boarder{1..5}_uso` | 30 | 1,964 | Per-character snowboarder USOs |
| `mgrproc_uso` | 57 | 13,324 | Manager process (Yay0) |
| `game_uso` | 240 | 72,464 | Main game loop (Yay0) |
| `timproc_uso_b{1,3,5}` | 315 | 83,344 | Timer-process variants (Yay0) |
| `map4_data_uso_b2` | 3 | 196 | Map overlay code |
| **Total** | **3,288** | **762,652** | all matched |

Counts are from `report.json` (objdiff); units are grouped by source segment. Pure data assets (audio banks, character meshes, textures) stay as `bin` segments.

Matching notes (IDO codegen quirks, asm-shape patterns, the matching workflow, N64/USO forensics) live in the `docs/` directory of the [djsaunde/decomp](https://github.com/djsaunde/decomp) monorepo, not in this repo.

## Readability status

With every function matched, the current phase makes the source readable while keeping the ROM byte-identical: real names for functions and globals (`renames.tsv` is the ledger of every rename), shared struct headers in `include/game/` and `include/gdl/`, and raw `*(T *)(p + 0xNN)` accesses converted to fields wherever the bytes allow. Every change passes a gate that rebuilds the ROM and checks all 3,288 functions.

Start with **`docs/1080_PORTING_GUIDE.md`** in the [djsaunde/decomp](https://github.com/djsaunde/decomp) monorepo. It covers the module architecture, the frame loop from `app_run` down to the rider physics, render and audio, a "where is X" index, the data formats, the N64-specific parts a port must replace, the naming and matching conventions, and the open questions. It links to the area docs: physics, camera, graphics, course, audio, system layer, modes, engine core, game classes and the USO format.

Some of the source is deliberately unidiomatic, and the guide explains why: raw offsets left where a typed field would change codegen, `&D_00000000 + K` USO section-base accesses, `gl_func_00000000` cross-module calls, donor files spliced by `REPLACE_FUNC_BODY`, and `NON_MATCHING` reference bodies.

## Building

Requires: `mips-linux-gnu` binutils, Python 3, IDO 7.1 and IDO 5.3 (ido-static-recomp), and the MIPSpro 7.2.1 front end (`tools/fetch-mipspro721.sh`).

```bash
# 1. Place your 1080 Snowboarding (USA) ROM as baserom.z64
# 2. Extract IDO 7.1:
#    https://github.com/decompals/ido-static-recomp/releases/latest
#    Extract into tools/ido-static-recomp/build/7.1/out/ and .../build/5.3/out/
#    MIPSpro 7.2.1 front end: tools/fetch-mipspro721.sh
# 3. Extract asset bins from baserom via splat (one-time):
make setup
# 4. Build:
make
```

ROM-derived binary extracts (`assets/*.bin`) are not committed — they are regenerated from your local baserom by `make setup`.

The build picks the compiler and optimization level per file (see the toolchain list above and the Makefile).

### Optional: Ghidra integration

A persistent Ghidra project at `build/ghidra-project/tenshoe.gpr` complements `m2c` for struct-recovery, xref discovery, and type-decorated decomp output. Setup (one-time, ~7 min):

```bash
# Tools (downloaded into ../tools/, gitignored):
#   - JDK 21 (Adoptium portable)
#   - Ghidra 12.x (NSA release)
#   - pyghidra-mcp (uv tool install pyghidra-mcp)
# See ~/.claude/projects/.../memory/feedback_pyghidra_mcp_setup_for_n64_decomp.md

bash scripts/setup-ghidra.sh           # build project + import 2,000+ symbols
bash scripts/ghidra-decompile-func.sh __rmonClearBreak   # one-shot decomp
```

Use Ghidra by trigger only (struct shape unknown, function family >3, stuck <50% fuzzy with structural unknowns) — m2c remains the default. See the `/decompile` skill for the trigger list.

## PC port: Windows

`port/` builds the game as a native program (docs in `../docs/1080_PC_PORT_PLAN.md` of the monorepo, section 29).
The Windows build is one self-contained 32-bit `tenshoe.exe`; it contains no ROM data and reads your own
ROM at start-up. CI uploads it as the `tenshoe-windows-i686` artifact; to cross-compile it on Linux (no sudo):

```bash
port/win32/setup-mingw.sh            # mingw-w64 GCC + SDL2 into ../../tools (gitignored)
port/gfx/fast3d/setup-lus.sh         # libultraship (Fast3D)
cmake -S port -B build-win -G Ninja -DCMAKE_TOOLCHAIN_FILE=$PWD/port/win32/mingw-i686.cmake -DPORT_WIN32=ON -DPORT_LINK=ON
cmake --build build-win --target tenshoe
```

To play: put `tenshoe.exe` and your US 1.0 ROM as `baserom.z64` in one folder and double-click the exe (without
a ROM it asks with a file dialog). Keys: arrows / WASD stick, X or Space = A, Z = B, C = Z, Q / E = L / R,
Enter = START, IJKL = C buttons, TFGH = D-pad, F1 settings (rebinding), F11 fullscreen, Esc quit; XInput /
DirectInput gamepads use the Xbox layout. The save, `1080.ini` and `1080.log` go next to the exe.
`port/win32/README-windows.txt` is the player's README.

## PC port: macOS (Apple Silicon)

macOS has no 32-bit processes and arm64 allows no mappings below 4 GB, so the game cannot keep the N64's 4-byte
pointers as host addresses there. The macOS build compiles the game to WebAssembly (wasm32, an ILP32 target: the
same struct layouts as the i386 build), translates it to C with WABT's `wasm2c`, and compiles that natively against
the SDL2 / OpenGL 4.1 host (plan section 30). The `macOS port` workflow builds `1080.app` on a macos-15 runner and
uploads it (zipped, with a README) as the `1080-pc-macos-arm64` artifact; it contains no ROM data.

To play: unzip, and the first time allow it (the app is not notarized): on macOS 15+ click Done, then System
Settings > Privacy & Security > Open Anyway; on 14 and earlier right-click `1080.app` > Open; on any version
`xattr -dr com.apple.quarantine 1080.app`), and give it your US 1.0 ROM: `baserom.z64` next to the app, or in
`~/Library/Application Support/1080-pc/`, or pick it in the file dialog it shows. Saves, `1080.ini` and `1080.log`
live in that folder. Keys and pads as on Windows; `port/wasm/macos/README-macos.txt` is the player's README.

Building it (also on Linux, where the same wasm2c output passes the i386 state.log / gfx / SRAM comparisons):

```bash
# wasi-sdk 25 and WABT's wasm2c (tools/wasm in the monorepo, or Homebrew / the release tarballs)
cmake -S port -B build-wasm -G Ninja -DPORT_WASM=ON -DCMAKE_TOOLCHAIN_FILE=<wasi-sdk>/share/cmake/wasi-sdk.cmake \
      -DWASI_SDK_PREFIX=<wasi-sdk> -DWASM2C=<wabt>/wasm2c
cmake --build build-wasm                                   # tenshoe.wasm -> build-wasm/w2c/game_*.c
port/gfx/fast3d/setup-lus.sh
cmake -S port/wasm/host -B build-host -G Ninja -DW2C_DIR=$PWD/build-wasm/w2c -DWASM2C_RT_DIR=<wabt-src>/wasm2c \
      -DWASM_HOST_NATIVE=ON                                # OFF: headless (the test runs)
cmake --build build-host
port/wasm/macos/make-app.sh build-host/tenshoe dist        # macOS: dist/1080-pc-macos.zip
```

## PC port: Android (arm64)

The Android build reuses the macOS route: the same wasm2c translation of the game, compiled for arm64 with the NDK,
inside an SDLActivity app whose host is the SDL2 one with OpenGL ES 3.0 (libultraship patches 20 / 21) and SDL's
AAudio / OpenSL ES audio (plan section 31). Built for handhelds with gamepad controls (the AYN Thor): Select + Start
or Android's Back opens the settings menu, which the gamepad drives. The `Android port` workflow builds the APK on
ubuntu-24.04 (`1080-snowboarding-android-arm64`, signed with a throwaway CI key); it contains no ROM data.

To play: install `1080-snowboarding.apk` (allow unknown apps, or `adb install -r`), start it and pick your US 1.0
ROM in the file picker it shows (it is copied into the app), or put it at
`Android/data/dev.djsaunde.tenshoe/files/baserom.z64` first. Saves, `1080.ini` and `1080.log` live in the app's
private files; `adb logcat -s tenshoe` shows the log. `port/android/README-android.txt` is the player's README.

Building it (Linux, no sudo; tools/wasm and the patched LUS as for macOS):

```bash
port/android/setup-android.sh        # JDK 17, SDK 34, NDK r27c, SDL2 source into tools/android
port/gfx/fast3d/setup-lus.sh         # libultraship with the whole patch series (20 / 21: OpenGL ES)
port/android/build-apk.sh            # -> build/android/1080-snowboarding.apk (release key: tools/android/*.jks)
```

The OpenGL ES renderer can be checked on Linux without a device: `make -C port/gfx test OUT=<dir>`, then
`make -C port/gfx/fast3d OUT=<dir> <dir>/f3dreplay-gles <dir>/f3dreplay-fb` and `port/gfx/fast3d/golden-gles.sh
<dir>` (the golden scenes through GLES on Mesa llvmpipe), or the whole game with
`port/android/linux-gles-check.sh`.

## References

- **[libreultra](https://github.com/n64decomp/libreultra)** — libultra decomp reference; matching functions, structs, `register` keyword usage
- **[splat](https://github.com/ethteck/splat)** — ROM splitting tool
- **[asm-processor](https://github.com/simonlindholm/asm-processor)** — Mixes C decompilation with INCLUDE_ASM stubs
- **[spimdisasm](https://github.com/Decompollaborate/spimdisasm)** — MIPS disassembler (via splat)
- **[decomp.me](https://decomp.me)** / **[objdiff](https://github.com/encounter/objdiff)** — Function-level matching and progress tracking; progress is published on [bytematch.dev](https://bytematch.dev/djsaunde/1080-decomp)

## Game Structure

1080 Snowboarding uses a **USO (Universal Shared Objects)** overlay system loaded at runtime. The ROM has the kernel + two large always-resident USOs (`bootup_uso`, `game_libs`), then a bag of process-specific USOs (`game_uso`, `gui_uso`, `*proc_uso`, `boarder*_uso`, `timproc_uso_b*`) that are loaded/unloaded as gameplay state changes. Yay0-compressed USOs are decompressed at splat time.

- **Kernel** (39 KB, 197 functions, VRAM `0x80000000`) — libultra, USO loader, audio
- **bootup_uso** (82 KB, 349 functions, VRAM `0`) — libgdl game engine (C++, debug strings preserved)
- **game_libs** (409 KB, 1,891 functions, VRAM `0`) — support libraries (stripped); the biggest segment
- **game_uso** (71 KB, 240 functions, VRAM `0`) — main game loop (Yay0-decompressed)
- **timproc_uso_b{1,3,5}** (12+12+57 KB, 315 functions total) — timer-process variants (Yay0)
- **Other process USOs** (1–18 KB each) — `gui_uso`, `titproc_uso`, `arcproc_uso`, `h2hproc_uso`, `eddproc_uso`, `n64proc_uso`, `mgrproc_uso`, `boarder{1..5}_uso`, `map4_data_uso_b2`

All USOs splat at synthetic `VRAM=0` to match unrelocated ROM bytes; distinct symbol prefixes (`func_` vs `gl_func_` vs `gui_func_` vs `game_uso_func_` vs `<usoname>_func_`) avoid linker collisions across USOs while letting each share its own internal label scope.
