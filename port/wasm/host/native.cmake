# port/wasm/host/native.cmake (plan 30): the window / audio / input / Fast3D host around the wasm2c game, from the
# plan 21.9 sources (port/gfx/fast3d) built with PORT_WASM_HOST (game addresses = memory base + offset).
#   macOS: SDL2 (window, keyboard, game controllers, CoreAudio output) and the OpenGL framework (4.1 core);
#   Linux: X11 + EGL + PulseAudio, as the x86-64 PORT_NATIVE build (tools/x11-dev, tools/pulse-dev, tools/gl-dev).
set(PORT_LUS "${GAME_ROOT}/../../tools/lus" CACHE PATH "libultraship + prism (port/gfx/fast3d/setup-lus.sh)")
if(NOT EXISTS "${PORT_LUS}/.complete")
  message(FATAL_ERROR "WASM_HOST_NATIVE: ${PORT_LUS} missing; run port/gfx/fast3d/setup-lus.sh")
endif()
find_package(Python3 REQUIRED COMPONENTS Interpreter)
set(_lus "${PORT_LUS}/libultraship")
set(_fd "${PORT_DIR}/gfx/fast3d")
file(GLOB _prism_cpp "${PORT_LUS}/prism/src/prism/*.cpp" "${PORT_LUS}/prism/src/prism/utils/*.cpp")
file(GLOB _prism_c "${PORT_LUS}/prism/src/prism/utils/*.c")
set(_shaders shaders/opengl/default.shader.glsl)
set(_gen ${CMAKE_CURRENT_BINARY_DIR}/gen)
file(MAKE_DIRECTORY ${_gen})
add_custom_command(OUTPUT ${_gen}/fast_shaders.inc
  COMMAND ${Python3_EXECUTABLE} ${PORT_DIR}/win32/embed-files.py ${_lus}/src/fast ${_gen}/fast_shaders.inc ${_shaders}
  DEPENDS ${PORT_DIR}/win32/embed-files.py ${_lus}/src/fast/${_shaders}
  COMMENT "Embedding libultraship's shaders" VERBATIM)
set(_native_src ${_fd}/native_host.cpp ${_fd}/live_audio.cpp ${PORT_DIR}/settings.c ${PORT_DIR}/input_map.c
    ${_fd}/settings_menu.cpp ${_fd}/menu_overlay_gl.cpp ${_fd}/port_fast3d.cpp ${_fd}/ship_shim.cpp
    ${PORT_DIR}/gfx/replay/egl_png.c ${_gen}/fast_shaders.inc
    ${_lus}/src/fast/interpreter.cpp ${_lus}/src/fast/backends/gfx_opengl.cpp ${_lus}/src/fast/debug/GfxDebugger.cpp
    ${_lus}/src/fast/resource/type/Texture.cpp ${_prism_cpp} ${_prism_c})
set(_native_defs PORT_WASM_HOST PORT_GFX_NATIVE PORT_EMBED_SHADERS ENABLE_OPENGL CVAR_MSAA_VALUE="gMSAAValue"
    CVAR_INTERNAL_RESOLUTION="gInternalResolution" CVAR_Z_FIGHTING_MODE="gZFightingMode" LUS_FAST_DIR="${_lus}/src/fast")
set(_native_inc ${_fd}/shim ${_lus}/include ${_lus}/include/fast ${_lus}/src ${PORT_LUS}/prism/src ${_gen})
if(APPLE)
  find_package(SDL2 REQUIRED)
  list(APPEND _native_src ${_fd}/live_window_sdl.cpp)
  list(APPEND _native_defs PORT_SDL SDL_MAIN_HANDLED)
  # SDL2 linked in when the install has the static library (Homebrew's has): one executable in the .app, no
  # dylib to bundle and re-path
  if(TARGET SDL2::SDL2-static)
    set(_native_libs SDL2::SDL2-static "-framework OpenGL")
  else()
    set(_native_libs SDL2::SDL2 "-framework OpenGL")
  endif()
  get_target_property(_sdlinc SDL2::SDL2 INTERFACE_INCLUDE_DIRECTORIES)
  if(_sdlinc)
    foreach(_d ${_sdlinc}) # live_*.cpp include <SDL2/SDL.h>: the parent of SDL2's own include dir
      get_filename_component(_p "${_d}" DIRECTORY)
      list(APPEND _native_inc ${_d} ${_p})
    endforeach()
  endif()
else()
  set(PORT_GL_DEV "${GAME_ROOT}/../../tools/gl-dev/root/usr/include" CACHE PATH "EGL / GL headers")
  set(PORT_NATIVE_X11_DEV "${GAME_ROOT}/../../tools/x11-dev/root/usr/include" CACHE PATH "Xlib headers")
  set(PORT_NATIVE_PULSE_DEV "${GAME_ROOT}/../../tools/pulse-dev/root/usr/include" CACHE PATH "libpulse headers")
  set(PORT_NATIVE_LIBDIR "/lib/x86_64-linux-gnu" CACHE PATH "libEGL.so.1 / libGL.so.1 / libX11.so.6")
  list(APPEND _native_src ${_fd}/live_window.cpp ${_fd}/live_gamepad.cpp)
  list(APPEND _native_inc ${PORT_GL_DEV} ${PORT_NATIVE_X11_DEV} ${PORT_NATIVE_PULSE_DEV})
  set(_native_libs ${PORT_NATIVE_LIBDIR}/libEGL.so.1 ${PORT_NATIVE_LIBDIR}/libGL.so.1 ${PORT_NATIVE_LIBDIR}/libX11.so.6
      dl pthread)
endif()
add_library(port_native STATIC ${_native_src})
target_include_directories(port_native PRIVATE ${_native_inc})
if(APPLE)
  target_link_libraries(port_native PUBLIC ${_native_libs})
endif()
target_compile_definitions(port_native PRIVATE ${_native_defs})
target_compile_options(port_native PRIVATE -O2 -w $<$<COMPILE_LANGUAGE:CXX>:-std=c++20>)
list(APPEND _host ${CMAKE_CURRENT_SOURCE_DIR}/port_imports_native.c)

function(port_wasm_native tgt)
  target_compile_definitions(${tgt} PRIVATE PORT_WASM_HOST PORT_GFX_NATIVE)
  target_link_libraries(${tgt} PRIVATE port_native ${_native_libs})
  set_target_properties(${tgt} PROPERTIES LINKER_LANGUAGE CXX)
endfunction()
