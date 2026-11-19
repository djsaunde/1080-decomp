# port/wasm/host (plan section 30): the native executable around the wasm2c translation of the game.
#
#   cmake -S port -B <bw> -G Ninja -DPORT_WASM=ON \
#         -DCMAKE_TOOLCHAIN_FILE=<wasi-sdk>/share/cmake/wasi-sdk.cmake -DWASI_SDK_PREFIX=<wasi-sdk>
#   cmake --build <bw>                                   # -> <bw>/tenshoe.wasm, <bw>/w2c/game_*.c
#   cmake -S port/wasm/host -B <bh> -G Ninja -DW2C_DIR=<bw>/w2c [-DWASM_HOST_NATIVE=ON]
#   cmake --build <bh>                                   # -> <bh>/tenshoe (the native host's compiler)
#
# WASM_HOST_NATIVE=OFF: headless (the gate's state.log / gfx-dump runs). ON: the in-process window, audio, input
# and Fast3D host of plan 21.9 over SDL2 (port/gfx/fast3d/native_host.cpp, live_window_sdl.cpp ...), reading the
# game's display lists and audio ring in the linear memory (PORT_WASM_HOST: game address + memory base).
cmake_minimum_required(VERSION 3.16)
project(tenshoe_wasm_host C CXX)

get_filename_component(PORT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../.." ABSOLUTE)
get_filename_component(GAME_ROOT "${PORT_DIR}/.." ABSOLUTE)
set(W2C_DIR "" CACHE PATH "the wasm2c output of the PORT_WASM build (<bw>/w2c)")
if(NOT EXISTS "${W2C_DIR}/game.h")
  message(FATAL_ERROR "W2C_DIR: no game.h in '${W2C_DIR}' (build port with -DPORT_WASM=ON first)")
endif()
# wasm2c's runtime: WABT's source tree (wasm2c/) or an install (share/wabt/wasm2c, e.g. Homebrew)
find_path(WASM2C_RT_DIR wasm-rt-impl.c
  HINTS "${GAME_ROOT}/../../tools/wasm/src/wabt-1.0.36/wasm2c" /opt/homebrew/share/wabt/wasm2c
        /usr/local/share/wabt/wasm2c /usr/share/wabt/wasm2c
  DOC "the directory holding wasm-rt-impl.c")
if(NOT WASM2C_RT_DIR)
  message(FATAL_ERROR "no wasm2c runtime (wasm-rt-impl.c); set -DWASM2C_RT_DIR")
endif()
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()
option(WASM_HOST_NATIVE "the window / audio / input / Fast3D host (SDL2); OFF = headless" OFF)
if(ANDROID)
  # plan 31: explicit bounds checks instead of guard pages + a SIGSEGV handler (ART chains signals; no surprises),
  # and the call depth counted instead of a stack-exhaustion signal. Every unit including wasm-rt.h must agree.
  add_compile_definitions(WASM_RT_MEMCHECK_BOUNDS_CHECK=1)
endif()
option(WASM_HOST_NULLCHECK "debug: report every load / store below the 16 MB global base (a null pointer)" OFF)

set(_game)
foreach(i RANGE 7)
  list(APPEND _game ${W2C_DIR}/game_${i}.c)
endforeach()
set(_rt ${WASM2C_RT_DIR}/wasm-rt-impl.c ${WASM2C_RT_DIR}/wasm-rt-mem-impl.c ${WASM2C_RT_DIR}/wasm-rt-exceptions-impl.c)
add_library(w2cgame OBJECT ${_game} ${_rt})
target_include_directories(w2cgame PUBLIC ${W2C_DIR} ${WASM2C_RT_DIR})
# the translated game: plain C float arithmetic, so no FMA contraction (arm64 clang contracts by default) and the
# FPU mode main() sets (flush-to-zero) applies, as on the i386 build
target_compile_options(w2cgame PRIVATE -ffp-contract=off -fno-strict-aliasing -w)
set_source_files_properties(${_rt} PROPERTIES COMPILE_DEFINITIONS "WASM_RT_TRAP_HANDLER=port_wasm_trap")
if(WASM_HOST_NULLCHECK)
  target_compile_definitions(w2cgame PRIVATE PORT_WASM_NULLCHECK)
endif()

set(_host ${CMAKE_CURRENT_SOURCE_DIR}/wasm_main.c ${CMAKE_CURRENT_SOURCE_DIR}/wasi_host.c
          ${CMAKE_CURRENT_SOURCE_DIR}/posix_launch.c)
if(WASM_HOST_NATIVE)
  include(${CMAKE_CURRENT_SOURCE_DIR}/native.cmake)
else()
  list(APPEND _host ${CMAKE_CURRENT_SOURCE_DIR}/port_imports_headless.c)
endif()
if(ANDROID)
  # plan 31: libmain.so, which SDLActivity loads and whose SDL_main (port/android/android_main.c) runs the game
  add_library(tenshoe SHARED ${_host} ${PORT_DIR}/android/android_main.c $<TARGET_OBJECTS:w2cgame>)
  set_target_properties(tenshoe PROPERTIES OUTPUT_NAME main)
else()
  add_executable(tenshoe ${_host} $<TARGET_OBJECTS:w2cgame>)
endif()
target_include_directories(tenshoe PRIVATE ${W2C_DIR} ${WASM2C_RT_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(tenshoe PRIVATE m)
if(WASM_HOST_NATIVE)
  port_wasm_native(tenshoe)
endif()
