# 1080 Snowboarding PC port: build skeleton (docs/1080_PC_PORT_PLAN.md section 13, M1 prep).
#
#   scripts/port-gen-build.py                 # regenerate port/gen/ from port/sourcemap.tsv
#   cmake -S port -B /tmp/claude-1000/port-build -G Ninja
#   cmake --build /tmp/claude-1000/port-build # -> libgame.a (+ game, with -DPORT_LINK=ON)
#
# Every function is compiled exactly once, from the file port/sourcemap.tsv names
# as its shipped body.  The selection is done by port/tools/cc-select.py (the
# compiler launcher) from the per-unit headers in port/gen/tu/; src/ is not
# touched.  The kernel, the game_libs libultra / libc copies and the 43
# hand-written asm functions are not compiled (plan decision 6): the port/ultra
# shim provides them (port/undefined.txt lists what the library needs).
cmake_minimum_required(VERSION 3.16)
project(tenshoe_port C CXX ASM)

get_filename_component(GAME_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/.." ABSOLUTE)
set(PORT_GEN "${CMAKE_CURRENT_SOURCE_DIR}/gen")

find_package(Python3 REQUIRED COMPONENTS Interpreter)
set(CMAKE_C_COMPILER_LAUNCHER   ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tools/cc-select.py)
set(CMAKE_CXX_COMPILER_LAUNCHER ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tools/cc-select.py)

option(PORT_LINK "link the stub executable (needs a 32-bit libc: gcc-multilib)" OFF)
# DEBUG ONLY (plan 18.9): link tenshoe with an aborting trap per still-undefined symbol, so the
# boot can be walked before the link is at 0.  Needs PORT_LINK.  Never on in a normal build.
option(PORT_TRAP_UNDEFINED "debug only: every undefined symbol becomes an aborting trap" OFF)
if(PORT_TRAP_UNDEFINED AND NOT PORT_LINK)
  message(FATAL_ERROR "PORT_TRAP_UNDEFINED needs PORT_LINK=ON")
endif()
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif()

# 32-bit host (plan decision 1).  -malign-double: i386 aligns u64 / f64 to 4 in
# structs, which breaks the OSThread and other offset asserts (section 12.2).
# SSE single precision, no FMA contraction (plan section 6).  -fcommon: IDO
# merges tentative definitions (the match_imports context headers define
# gl_storage_66B64_threadRegion in several units).  -ffunction-sections lets
# --gc-sections drop the renamed-dead placeholder copies.
# PORT_64 (M6, plan section 21): the x86-64 build.  No -m32 / -malign-double (x86-64 aligns u64 / f64
# to 8 already); PORT64 selects the 64-bit spellings under PORT.  -no-pie + the small code model keep
# the image (text, data, bss, the blobs) below 2 GB, the low-address model of plan 21.
option(PORT_64 "M6: build for x86-64 (plan section 21); default is the 32-bit i386 build" OFF)
if(PORT_64)
  # Every game-visible unit (the game, port/uso, port/ultra, the host layer that reads game structs)
  # is compiled with -mx32: x86-64 code with the ILP32 data model, so every struct, blob and cfront
  # layout stays the N64's.  cc-select.py assembles those units as ELF64 objects; they run below 2 GB
  # (non-PIE image, brk heap, a MAP_32BIT stack: host/main64.c).  PORT_X32_CHECK only parses the
  # glibc headers: their gnu/stubs-x32.h comes from port/include/x32 (no x32 libc is linked).
  set(PORT_M32)
  set(PORT_X32_SYSINC "/usr/include/x86_64-linux-gnu" CACHE PATH "the x86-64 glibc multiarch headers (parsed by -mx32)")
  set(PORT_ARCH_FLAGS
      $<$<COMPILE_LANGUAGE:C,CXX>:-mx32 -isystem${CMAKE_CURRENT_SOURCE_DIR}/include/x32 -isystem${PORT_X32_SYSINC}>
      -msse2 -mfpmath=sse -ffp-contract=off -fno-strict-aliasing
      -fno-pie -fno-stack-protector -ffunction-sections -fdata-sections
      $<$<COMPILE_LANGUAGE:C,CXX>:-fcf-protection=none>)
  set(PORT_DEFS PORT PORT64)
else()
  set(PORT_M32 -m32)
  set(PORT_ARCH_FLAGS -m32 -malign-double -msse2 -mfpmath=sse -ffp-contract=off -fno-strict-aliasing
      -fno-pie -fno-stack-protector -ffunction-sections -fdata-sections)
  set(PORT_DEFS PORT)
endif()

include(${PORT_GEN}/sources.cmake)

add_library(game STATIC ${PORT_GAME_SOURCES})
target_compile_definitions(game PRIVATE ${PORT_DEFS})
target_include_directories(game PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include ${GAME_ROOT}/include ${GAME_ROOT}/src)
# -fno-inline: a port-bind follows the function label (cc-select renames the token inside
# FUNC and its .part / .isra / .constprop clones); a body gcc inlined into its caller would
# keep the unbound placeholder (plan 18.6).  C only: a C++ donor's inline members are
# not ROM functions (NCC inlined them), so their sites belong to the caller's binds.
target_compile_options(game PRIVATE ${PORT_ARCH_FLAGS} -include ${CMAKE_CURRENT_SOURCE_DIR}/include/port.h
    $<$<COMPILE_LANGUAGE:C>:-std=gnu89 -fcommon -fno-inline>
    $<$<COMPILE_LANGUAGE:CXX>:-std=gnu++98 -fpermissive -fno-exceptions -fno-rtti>
    -Wno-int-conversion -Wno-incompatible-pointer-types -Wno-builtin-declaration-mismatch)

# port/uso: the module registry and the USO data blobs (plan section 16.5).  The blobs are
# generated here from the user's ROM (no ROM data in the repository): extract.py writes the
# per-USO sections, gen-uso-blobs.py turns the ones port/gen/uso_blobs.tsv lists into an object.
set(PORT_ROM "${GAME_ROOT}/baserom.z64" CACHE FILEPATH "the US 1.0 ROM (z64) the data blobs come from")
set(PORT_EXTRACTED "" CACHE PATH "an existing port/tools/extract output (default: extract PORT_ROM into the build dir)")
if(PORT_EXTRACTED)
  set(PORT_EXT_DIR "${PORT_EXTRACTED}")
  set(PORT_EXT_DEP "${PORT_EXTRACTED}/manifest.json")
else()
  set(PORT_EXT_DIR "${CMAKE_CURRENT_BINARY_DIR}/extracted")
  set(PORT_EXT_DEP "${PORT_EXT_DIR}/manifest.json")
  add_custom_command(OUTPUT ${PORT_EXT_DEP}
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tools/extract/extract.py
            --rom ${PORT_ROM} --out ${PORT_EXT_DIR}
    DEPENDS ${PORT_ROM} ${CMAKE_CURRENT_SOURCE_DIR}/tools/extract/extract.py
    COMMENT "Extracting the ROM filesystem (port/tools/extract)"
    VERBATIM)
endif()
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/uso_blobs.s
  COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tools/gen-uso-blobs.py
          ${PORT_EXT_DIR} ${PORT_GEN}/uso_blobs.tsv ${CMAKE_CURRENT_BINARY_DIR}/uso_blobs.s
  DEPENDS ${PORT_EXT_DEP} ${PORT_GEN}/uso_blobs.tsv ${PORT_GEN}/blob_types.tsv
          ${CMAKE_CURRENT_SOURCE_DIR}/tools/gen-uso-blobs.py ${CMAKE_CURRENT_SOURCE_DIR}/tools/blob_swap.py
  COMMENT "Generating the USO data blobs"
  VERBATIM)
add_library(uso STATIC uso/uso.c ${PORT_GEN}/uso_modules.c ${CMAKE_CURRENT_BINARY_DIR}/uso_blobs.s)
target_include_directories(uso PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/uso)
target_compile_options(uso PRIVATE ${PORT_ARCH_FLAGS})

# port/ultra (the libultra shim, plan section 17) and the host side (main loop, headless host,
# trace logger).  Without PORT_LINK (no 32-bit libc on the machine) they are still compiled, with
# PORT_LIBC_DECLS_ONLY (port_libc.h declares the libc functions by hand), so the undefined
# target can check the whole link.
set(PORT_ULTRA_SOURCES ultra/thread.c ultra/message.c ultra/vi.c ultra/pi.c ultra/si.c
    ultra/ai_sp.c ultra/misc.c ultra/va_pack.c)
set(PORT_HOST_SOURCES main.c host_headless.c host/input_script.c trace.c)
# port/gfx (plan section 19, M2): port_gfx_task (the M_GFXTASK hook) and the PORT_GFX_DUMP writer
list(APPEND PORT_HOST_SOURCES gfx/port_gfx.c gfx/dlump.c gfx/port_fb.c gfx/port_live.c)
# plan 19.7: option (a), Fast3D in process (gfx/fast3d/port_fast3d.cpp + libultraship's src/fast, pinned + the
# patch series, fetched by gfx/fast3d/setup-lus.sh). Needs a C++20 compiler, libstdc++, EGL / GL and the GL
# headers for the target ABI: -m32 here, which this machine does not have (plan 19.7); the option stays OFF and
# the dump + f3dreplay (option (b)) is the default. PORT_FAST3D_FLAGS overrides the arch flags (empty = a 64-bit
# compile of the library alone).
option(PORT_GFX_FAST3D "render gfx tasks in process with libultraship's Fast3D (needs target-ABI C++ / GL)" OFF)
# agent-g run 619: the SysApi table and the cartridge file system (host side); agent-c run 844:
# the SRAM save's byte order (fs/backup_sram.c, port/include/port_backup.h)
foreach(f sysapi.c fs/fs.c fs/backup_sram.c)
  if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${f})
    list(APPEND PORT_HOST_SOURCES ${f})
  endif()
endforeach()
set(PORT_HOST_FLAGS ${PORT_ARCH_FLAGS} -std=gnu99 -fno-math-errno)
set(PORT_HOST_DEFS ${PORT_DEFS})
if(NOT PORT_LINK)
  list(APPEND PORT_HOST_DEFS PORT_LIBC_DECLS_ONLY)
endif()
add_library(ultra STATIC ${PORT_ULTRA_SOURCES})
add_library(porthost OBJECT ${PORT_HOST_SOURCES})
# port/audio (plan 15.6): the CPU mixer in place of the RSP's aspMain, the driver glue
# (port_audio_frame) and the hooks the PORT source calls: port_audio_swap_static /
# port_audio_swap_font (audio data to host order) and port_audio_noise_base (gWaveSamples[8]).
set(PORT_AUDIO_SOURCES audio/mixer.c audio/abi_decode.c audio/audio_swap.c audio/port_audio.c)
add_library(audio STATIC ${PORT_AUDIO_SOURCES})
# PORT_TRACE_FN (plan 21.8): a diagnostic build whose game library logs every function entry of one
# frame (port/trace.h; port/tools/fn-trace-diff.py compares two builds' logs).  Off by default.
# -pg -mfentry: one `call __fentry__` before each prologue and no exit hook (port/trace.c).
option(PORT_TRACE_FN "instrument the game library's functions for the per-function trace" OFF)
if(PORT_TRACE_FN)
  target_compile_options(game PRIVATE -pg -mfentry)
  target_compile_definitions(porthost PRIVATE PORT_TRACE_FN)
endif()
foreach(t ultra porthost audio)
  target_compile_definitions(${t} PRIVATE ${PORT_HOST_DEFS})
  target_include_directories(${t} PRIVATE ${GAME_ROOT}/include ${CMAKE_CURRENT_SOURCE_DIR})
  target_compile_options(${t} PRIVATE ${PORT_HOST_FLAGS})
endforeach()

if(PORT_GFX_FAST3D)
  set(PORT_LUS "${GAME_ROOT}/../../tools/lus" CACHE PATH "libultraship + prism (gfx/fast3d/setup-lus.sh)")
  set(PORT_GL_DEV "${GAME_ROOT}/../../tools/gl-dev/root/usr/include" CACHE PATH "EGL / GL headers")
  set(PORT_FAST3D_FLAGS "-m32;-msse2;-mfpmath=sse;-fno-pie" CACHE STRING "arch flags of the Fast3D library")
  if(NOT EXISTS "${PORT_LUS}/.complete")
    message(FATAL_ERROR "PORT_GFX_FAST3D: ${PORT_LUS} missing; run port/gfx/fast3d/setup-lus.sh")
  endif()
  set(_lus "${PORT_LUS}/libultraship")
  file(GLOB _prism_cpp "${PORT_LUS}/prism/src/prism/*.cpp" "${PORT_LUS}/prism/src/prism/utils/*.cpp")
  file(GLOB _prism_c "${PORT_LUS}/prism/src/prism/utils/*.c")
  add_library(port_fast3d STATIC gfx/fast3d/port_fast3d.cpp gfx/fast3d/ship_shim.cpp gfx/replay/egl_png.c
              ${_lus}/src/fast/interpreter.cpp ${_lus}/src/fast/backends/gfx_opengl.cpp
              ${_lus}/src/fast/debug/GfxDebugger.cpp ${_lus}/src/fast/resource/type/Texture.cpp
              ${_prism_cpp} ${_prism_c})
  target_include_directories(port_fast3d PRIVATE gfx/fast3d/shim ${_lus}/include ${_lus}/include/fast ${_lus}/src
                             ${PORT_LUS}/prism/src ${PORT_GL_DEV})
  target_compile_definitions(port_fast3d PRIVATE ENABLE_OPENGL CVAR_MSAA_VALUE="gMSAAValue"
                             CVAR_INTERNAL_RESOLUTION="gInternalResolution" CVAR_Z_FIGHTING_MODE="gZFightingMode"
                             LUS_FAST_DIR="${_lus}/src/fast")
  target_compile_options(port_fast3d PRIVATE ${PORT_FAST3D_FLAGS} -w $<$<COMPILE_LANGUAGE:CXX>:-std=c++20>)
  target_compile_definitions(porthost PRIVATE PORT_GFX_FAST3D)
endif()

# PORT_NATIVE (plan 21.9): the native single-process 64-bit game. tenshoe links the in-process host
# (gfx/fast3d/native_host.cpp: the presenter's X11 + EGL window and input, live_audio.cpp, Fast3D compositing a
# step per frame) and, run with PORT_NATIVE=1, opens its own window: no presenter, no shared memory. The host is
# ordinary LP64 code: its boundary with the x32 game is gfx/port_native.h (u32 arguments only). Needs PORT_64,
# tools/lus (setup-lus.sh), tools/gl-dev, tools/x11-dev and tools/pulse-dev (headers only; libpulse-simple is
# dlopen'd). PORT_NATIVE_X11_DEV / _PULSE_DEV / PORT_GL_DEV locate them.
option(PORT_NATIVE "M6 / plan 21.9: link the in-process window / audio / Fast3D host into the 64-bit tenshoe" OFF)
if(PORT_NATIVE)
  if(NOT PORT_64)
    message(FATAL_ERROR "PORT_NATIVE needs PORT_64")
  endif()
  set(PORT_LUS "${GAME_ROOT}/../../tools/lus" CACHE PATH "libultraship + prism (gfx/fast3d/setup-lus.sh)")
  set(PORT_GL_DEV "${GAME_ROOT}/../../tools/gl-dev/root/usr/include" CACHE PATH "EGL / GL headers")
  set(PORT_NATIVE_X11_DEV "${GAME_ROOT}/../../tools/x11-dev/root/usr/include" CACHE PATH "Xlib headers")
  set(PORT_NATIVE_PULSE_DEV "${GAME_ROOT}/../../tools/pulse-dev/root/usr/include" CACHE PATH "libpulse headers")
  set(PORT_NATIVE_LIBDIR "/lib/x86_64-linux-gnu" CACHE PATH "libEGL.so.1 / libGL.so.1 / libX11.so.6")
  if(NOT EXISTS "${PORT_LUS}/.complete")
    message(FATAL_ERROR "PORT_NATIVE: ${PORT_LUS} missing; run port/gfx/fast3d/setup-lus.sh")
  endif()
  set(_lus "${PORT_LUS}/libultraship")
  file(GLOB _prism_cpp "${PORT_LUS}/prism/src/prism/*.cpp" "${PORT_LUS}/prism/src/prism/utils/*.cpp")
  file(GLOB _prism_c "${PORT_LUS}/prism/src/prism/utils/*.c")
  add_library(port_native STATIC gfx/fast3d/native_host.cpp gfx/fast3d/live_window.cpp gfx/fast3d/live_audio.cpp
              gfx/fast3d/live_gamepad.cpp gfx/fast3d/port_fast3d.cpp gfx/fast3d/ship_shim.cpp gfx/replay/egl_png.c
              ${_lus}/src/fast/interpreter.cpp ${_lus}/src/fast/backends/gfx_opengl.cpp
              ${_lus}/src/fast/debug/GfxDebugger.cpp ${_lus}/src/fast/resource/type/Texture.cpp
              ${_prism_cpp} ${_prism_c})
  target_include_directories(port_native PRIVATE gfx/fast3d/shim ${_lus}/include ${_lus}/include/fast ${_lus}/src
                             ${PORT_LUS}/prism/src ${PORT_GL_DEV} ${PORT_NATIVE_X11_DEV} ${PORT_NATIVE_PULSE_DEV})
  target_compile_definitions(port_native PRIVATE ENABLE_OPENGL CVAR_MSAA_VALUE="gMSAAValue"
                             CVAR_INTERNAL_RESOLUTION="gInternalResolution" CVAR_Z_FIGHTING_MODE="gZFightingMode"
                             LUS_FAST_DIR="${_lus}/src/fast")
  target_compile_options(port_native PRIVATE -O2 -fno-pie -w $<$<COMPILE_LANGUAGE:CXX>:-std=c++20>)
  target_compile_definitions(porthost PRIVATE PORT_GFX_NATIVE)
endif()

if(PORT_64)
  # the LP64 entry (plan 21): low stack, low heap, argv / environ copied below 2 GB, then port_main
  add_library(porthost64 OBJECT host/main64.c host/x32_glue.c)
  target_compile_options(porthost64 PRIVATE -fno-pie -O2 -g)
  if(PORT_NATIVE)
    target_compile_definitions(porthost64 PRIVATE PORT_GFX_NATIVE)
  endif()
  target_compile_definitions(porthost PRIVATE main=port_main)
  set(PORT_ENTRY64 $<TARGET_OBJECTS:porthost64>)
  # the libc calls whose x32 / LP64 memory layouts differ go through host/x32_glue.c (plan 21.4)
  set(PORT64_WRAP)
  set(PORT64_WRAP_FNS __vfprintf_chk __vsnprintf_chk __vsprintf_chk vfprintf vsnprintf vsprintf strtol strtoul strtod
      fseek sbrk __errno_location mmap sigaction backtrace backtrace_symbols_fd)
  foreach(f ${PORT64_WRAP_FNS})
    list(APPEND PORT64_WRAP -Wl,--wrap=${f})
  endforeach()
endif()
if(PORT_LINK)
  add_executable(tenshoe $<TARGET_OBJECTS:porthost> ${PORT_ENTRY64})
  target_link_options(tenshoe PRIVATE ${PORT_M32} -no-pie ${PORT64_WRAP} -Wl,--gc-sections)
  # whole-archive: every shipped body is a root; the shim (port/ultra, port/uso)
  # resolves port/undefined.txt.
  # port/gen/aliases.ld: the token aliases (scripts/port-gen-binds.py), an implicit linker script
  target_link_libraries(tenshoe PRIVATE -Wl,--whole-archive game uso ultra audio -Wl,--no-whole-archive
                        ${PORT_GEN}/aliases.ld m)
  if(PORT_GFX_FAST3D)
    target_link_libraries(tenshoe PRIVATE port_fast3d EGL GL stdc++)
  endif()
  if(PORT_NATIVE)
    # The link's --wrap list (PORT64_WRAP) turns every reference to those libc calls into the x32 glue's
    # converters; the host is LP64 and must reach the real ones, so its archive refers to __real_<f> (which
    # --wrap resolves to <f>) instead.
    set(_redef "")
    foreach(f ${PORT64_WRAP_FNS})
      string(APPEND _redef "${f} __real_${f}\n")
    endforeach()
    file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/port_native.redefine "${_redef}")
    add_custom_command(TARGET port_native POST_BUILD
      COMMAND objcopy --redefine-syms=${CMAKE_CURRENT_BINARY_DIR}/port_native.redefine $<TARGET_FILE:port_native>
      COMMENT "port_native: the LP64 host calls the real libc (not the x32 --wrap glue)" VERBATIM)
    target_link_libraries(tenshoe PRIVATE port_native ${PORT_NATIVE_LIBDIR}/libEGL.so.1 ${PORT_NATIVE_LIBDIR}/libGL.so.1
                          ${PORT_NATIVE_LIBDIR}/libX11.so.6 stdc++ dl pthread)
  endif()

  # PORT_TRAP_UNDEFINED (DEBUG ONLY, plan 18.9; never on in a normal build): tenshoe links
  # while symbols are still undefined.  A probe link with --warn-unresolved-symbols lists them;
  # port/tools/gen-port-traps.py turns each into a trap (port/trap/trap.c): a code symbol
  # prints "PORT TRAP: <symbol>" + a backtrace and aborts, a data symbol is a PROT_NONE page
  # whose first touch does the same.  Nothing is stubbed: every trap aborts.
  if(PORT_TRAP_UNDEFINED)
    message(WARNING "PORT_TRAP_UNDEFINED=ON: debug build, undefined symbols become aborting traps")
    add_executable(tenshoe_probe $<TARGET_OBJECTS:porthost>)
    target_link_options(tenshoe_probe PRIVATE ${PORT_M32} -no-pie -Wl,--gc-sections -Wl,--export-dynamic
                        -Wl,--warn-unresolved-symbols)  # the same roots as tenshoe below
    target_link_libraries(tenshoe_probe PRIVATE -Wl,--whole-archive game uso ultra audio -Wl,--no-whole-archive
                          ${PORT_GEN}/aliases.ld m)
    set(_nm "${CMAKE_NM}")
    if(NOT _nm)
      set(_nm nm)
    endif()
    set(_od "${CMAKE_OBJDUMP}")
    if(NOT _od)
      set(_od objdump)
    endif()
    add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/port_traps.c
      COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tools/gen-port-traps.py
              --probe $<TARGET_FILE:tenshoe_probe> --out ${CMAKE_CURRENT_BINARY_DIR}/port_traps.c
              --nm ${_nm} --objdump ${_od}
              $<TARGET_FILE:game> $<TARGET_FILE:uso> $<TARGET_FILE:ultra> $<TARGET_FILE:audio>
      DEPENDS tenshoe_probe ${CMAKE_CURRENT_SOURCE_DIR}/tools/gen-port-traps.py
      COMMENT "PORT_TRAP_UNDEFINED: generating the undefined-symbol traps"
      VERBATIM)
    target_sources(tenshoe PRIVATE trap/trap.c ${CMAKE_CURRENT_BINARY_DIR}/port_traps.c)
    set_source_files_properties(trap/trap.c ${CMAKE_CURRENT_BINARY_DIR}/port_traps.c
                                PROPERTIES COMPILE_OPTIONS "${PORT_M32};-fno-pie;-g")
    target_link_options(tenshoe PRIVATE -Wl,--export-dynamic)
  endif()

  # port/audio/tools/seqrender (plan 15.7): the audio driver + CPU mixer without the boot:
  # renders a sequence to a WAV.  The host sources minus main.c (it has its own main).
  set(PORT_SEQRENDER_HOST ${PORT_HOST_SOURCES})
  list(REMOVE_ITEM PORT_SEQRENDER_HOST main.c)
  add_executable(seqrender audio/tools/seqrender.c audio/tools/rsp_lle.c audio/tools/lle_check.c ${PORT_SEQRENDER_HOST} ${PORT_ENTRY64})
  target_compile_definitions(seqrender PRIVATE ${PORT_HOST_DEFS} $<$<BOOL:${PORT_64}>:main=port_main>)
  target_include_directories(seqrender PRIVATE ${GAME_ROOT}/include ${CMAKE_CURRENT_SOURCE_DIR})
  target_compile_options(seqrender PRIVATE ${PORT_HOST_FLAGS})
  target_link_options(seqrender PRIVATE ${PORT_M32} -no-pie ${PORT64_WRAP} -Wl,--gc-sections)
  target_link_libraries(seqrender PRIVATE -Wl,--whole-archive game uso ultra audio -Wl,--no-whole-archive
                        ${PORT_GEN}/aliases.ld m)

  # the shim's self-test (plan 17.6): no game code, no ROM
  add_executable(test_ultra ultra/test_ultra.c host_headless.c ${PORT_ENTRY64})
  target_compile_definitions(test_ultra PRIVATE ${PORT_DEFS} $<$<BOOL:${PORT_64}>:main=port_main>)
  target_include_directories(test_ultra PRIVATE ${GAME_ROOT}/include ${CMAKE_CURRENT_SOURCE_DIR})
  target_compile_options(test_ultra PRIVATE ${PORT_HOST_FLAGS})
  target_link_options(test_ultra PRIVATE ${PORT_M32} -no-pie ${PORT64_WRAP})
  target_link_libraries(test_ultra PRIVATE ultra m)

  # the file-system host test: fs.c and the ROM only
  if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/fs/test_fs.c)
    add_executable(test_fs fs/test_fs.c fs/fs.c ${PORT_ENTRY64})
    target_compile_definitions(test_fs PRIVATE ${PORT_DEFS} $<$<BOOL:${PORT_64}>:main=port_main>)
    target_include_directories(test_fs PRIVATE ${GAME_ROOT}/include ${CMAKE_CURRENT_SOURCE_DIR})
    target_compile_options(test_fs PRIVATE ${PORT_HOST_FLAGS})
    target_link_options(test_fs PRIVATE ${PORT_M32} -no-pie ${PORT64_WRAP})
  endif()

  # the SRAM save format test (plan 20): fs/backup_sram.c alone; with a save file and the
  # extracted bootup Data it also checks a game-written save against the ROM's default records
  add_executable(test_backup fs/test_backup.c fs/backup_sram.c ${PORT_ENTRY64})
  target_compile_definitions(test_backup PRIVATE ${PORT_DEFS} $<$<BOOL:${PORT_64}>:main=port_main>)
  target_include_directories(test_backup PRIVATE ${GAME_ROOT}/include ${CMAKE_CURRENT_SOURCE_DIR})
  target_compile_options(test_backup PRIVATE ${PORT_HOST_FLAGS})
  target_link_options(test_backup PRIVATE ${PORT_M32} -no-pie ${PORT64_WRAP})
endif()

# port/undefined.txt: what the library needs from outside (no 32-bit libc needed).
add_custom_target(undefined
  COMMAND ${Python3_EXECUTABLE} ${GAME_ROOT}/scripts/port-undefined.py $<TARGET_FILE:game>
          --extra $<TARGET_FILE:uso> --extra $<TARGET_FILE:ultra> --extra $<TARGET_FILE:audio>
          --extra-objects "$<TARGET_OBJECTS:porthost>" -o ${CMAKE_CURRENT_SOURCE_DIR}/undefined.txt
  DEPENDS game uso ultra audio porthost
  COMMAND_EXPAND_LISTS
  WORKING_DIRECTORY ${GAME_ROOT}
  VERBATIM)
