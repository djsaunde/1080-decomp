# port/android/jni (plan 31): the APK's native libraries, built by the Gradle project's externalNativeBuild
# (port/android/build-apk.sh) or by hand with the NDK's toolchain file:
#   libSDL2.so  SDL2 from its release source (SDL2_SRC; port/android/setup-android.sh)
#   libmain.so  the wasm2c translation of the game (W2C_DIR: a PORT_WASM build's w2c/) + the SDL2 / OpenGL ES host
#               (port/wasm/host with WASM_HOST_NATIVE, Android branch) + SDL_main (port/android/android_main.c)
cmake_minimum_required(VERSION 3.16)
project(tenshoe_android C CXX)
get_filename_component(PORT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../.." ABSOLUTE)
set(SDL2_SRC "" CACHE PATH "SDL2's release source (tools/android/src/SDL2-<version>)")
set(PORT_ANDROID_JOBS 4 CACHE STRING "parallel compiles (the game's translated C is large; the machine is shared)")
if(NOT EXISTS "${SDL2_SRC}/CMakeLists.txt")
  message(FATAL_ERROR "SDL2_SRC: no SDL2 source in '${SDL2_SRC}' (port/android/setup-android.sh)")
endif()
set_property(GLOBAL APPEND PROPERTY JOB_POOLS port_compile=${PORT_ANDROID_JOBS} port_link=1)
set(CMAKE_JOB_POOL_COMPILE port_compile)
set(CMAKE_JOB_POOL_LINK port_link)

set(SDL_SHARED ON CACHE BOOL "" FORCE)
set(SDL_STATIC OFF CACHE BOOL "" FORCE)
set(SDL_TEST OFF CACHE BOOL "" FORCE)
add_subdirectory(${SDL2_SRC} SDL2)

set(WASM_HOST_NATIVE ON CACHE BOOL "" FORCE)
add_subdirectory(${PORT_DIR}/wasm/host host)
