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
project(tenshoe_port C CXX)

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
target_compile_options(game PRIVATE ${PORT_ARCH_FLAGS} -include ${CMAKE_CURRENT_SOURCE_DIR}/include/port.h
    $<$<COMPILE_LANGUAGE:C>:-std=gnu89 -fcommon>
    $<$<COMPILE_LANGUAGE:CXX>:-std=gnu++98 -fpermissive -fno-exceptions -fno-rtti>
    -Wno-int-conversion -Wno-incompatible-pointer-types -Wno-builtin-declaration-mismatch)

if(PORT_LINK)
  add_executable(tenshoe main.c)
  target_compile_options(tenshoe PRIVATE ${PORT_ARCH_FLAGS})
  target_link_options(tenshoe PRIVATE -m32 -no-pie -Wl,--gc-sections)
  # whole-archive: every shipped body is a root; the shim (port/ultra, port/uso)
  # resolves port/undefined.txt.
  target_link_libraries(tenshoe PRIVATE -Wl,--whole-archive game -Wl,--no-whole-archive)
endif()

# port/undefined.txt: what the library needs from outside (no 32-bit libc needed).
add_custom_target(undefined
  COMMAND ${Python3_EXECUTABLE} ${GAME_ROOT}/scripts/port-undefined.py $<TARGET_FILE:game>
          -o ${CMAKE_CURRENT_SOURCE_DIR}/undefined.txt
  DEPENDS game
  WORKING_DIRECTORY ${GAME_ROOT}
  VERBATIM)
