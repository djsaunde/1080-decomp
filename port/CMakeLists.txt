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
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif()

# 32-bit host (plan decision 1).  -malign-double: i386 aligns u64 / f64 to 4 in
# structs, which breaks the OSThread and other offset asserts (section 12.2).
# SSE single precision, no FMA contraction (plan section 6).  -fcommon: IDO
# merges tentative definitions (the match_imports context headers define
# gl_storage_66B64_threadRegion in several units).  -ffunction-sections lets
# --gc-sections drop the renamed-dead placeholder copies.
set(PORT_ARCH_FLAGS -m32 -malign-double -msse2 -mfpmath=sse -ffp-contract=off -fno-strict-aliasing
    -fno-pie -fno-stack-protector -ffunction-sections -fdata-sections)
set(PORT_DEFS PORT)

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
    ultra/ai_sp.c ultra/misc.c)
set(PORT_HOST_SOURCES main.c host_headless.c trace.c)
# agent-g run 619: the SysApi table and the cartridge file system (host side)
foreach(f sysapi.c fs/fs.c)
  if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${f})
    list(APPEND PORT_HOST_SOURCES ${f})
  endif()
endforeach()
set(PORT_HOST_FLAGS ${PORT_ARCH_FLAGS} -std=gnu99 -fno-math-errno)
set(PORT_HOST_DEFS PORT)
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
foreach(t ultra porthost audio)
  target_compile_definitions(${t} PRIVATE ${PORT_HOST_DEFS})
  target_include_directories(${t} PRIVATE ${GAME_ROOT}/include ${CMAKE_CURRENT_SOURCE_DIR})
  target_compile_options(${t} PRIVATE ${PORT_HOST_FLAGS})
endforeach()

if(PORT_LINK)
  add_executable(tenshoe $<TARGET_OBJECTS:porthost>)
  target_link_options(tenshoe PRIVATE -m32 -no-pie -Wl,--gc-sections)
  # whole-archive: every shipped body is a root; the shim (port/ultra, port/uso)
  # resolves port/undefined.txt.
  # port/gen/aliases.ld: the token aliases (scripts/port-gen-binds.py), an implicit linker script
  target_link_libraries(tenshoe PRIVATE -Wl,--whole-archive game uso ultra audio -Wl,--no-whole-archive
                        ${PORT_GEN}/aliases.ld m)

  # the shim's self-test (plan 17.6): no game code, no ROM
  add_executable(test_ultra ultra/test_ultra.c host_headless.c)
  target_compile_definitions(test_ultra PRIVATE PORT)
  target_include_directories(test_ultra PRIVATE ${GAME_ROOT}/include ${CMAKE_CURRENT_SOURCE_DIR})
  target_compile_options(test_ultra PRIVATE ${PORT_HOST_FLAGS})
  target_link_options(test_ultra PRIVATE -m32 -no-pie)
  target_link_libraries(test_ultra PRIVATE ultra m)

  # the file-system host test: fs.c and the ROM only
  if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/fs/test_fs.c)
    add_executable(test_fs fs/test_fs.c fs/fs.c)
    target_compile_definitions(test_fs PRIVATE PORT)
    target_include_directories(test_fs PRIVATE ${GAME_ROOT}/include ${CMAKE_CURRENT_SOURCE_DIR})
    target_compile_options(test_fs PRIVATE ${PORT_HOST_FLAGS})
    target_link_options(test_fs PRIVATE -m32 -no-pie)
  endif()
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
