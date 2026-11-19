# port/gfx/fast3d: f3dreplay, libultraship's Fast3D (pinned + the plan-14 patch series)
# behind the DLMP dump, rendered offscreen through EGL. Plan section 19.6.
#   ./setup-lus.sh                       fetch LUS + prism into tools/lus, apply the patches
#   ../replay/setup-gl-dev.sh            the EGL / GL headers (no sudo)
#   make                                 build $(OUT)/f3dreplay
#   ./setup-x11-dev.sh; make presenter   the live window (plan 19.11; port/tools/play.sh)
# Only three LUS sources are compiled (interpreter, gfx_opengl, GfxDebugger) plus
# Texture.cpp, prism and our Ship shim (shim/README); no SDL, ImGui or spdlog.
REPO ?= $(abspath ../../../../..)
GL_DEV ?= $(REPO)/tools/gl-dev/root
X11_DEV ?= $(REPO)/tools/x11-dev/root
PULSE_DEV ?= $(REPO)/tools/pulse-dev/root
LUS_TOOLS ?= $(REPO)/tools/lus
LUS := $(LUS_TOOLS)/libultraship
PRISM := $(LUS_TOOLS)/prism
LIBDIR ?= /lib/x86_64-linux-gnu
OUT ?= .
OBJ := $(OUT)/f3d-obj
CXX ?= g++
CC ?= cc
CXXFLAGS ?= -O2
LUS_DEFS := -DENABLE_OPENGL -DCVAR_MSAA_VALUE='"gMSAAValue"' -DCVAR_INTERNAL_RESOLUTION='"gInternalResolution"' \
            -DCVAR_Z_FIGHTING_MODE='"gZFightingMode"' -DLUS_FAST_DIR='"$(LUS)/src/fast"'
INC := -Ishim -I$(LUS)/include -I$(LUS)/include/fast -I$(LUS)/src -I$(PRISM)/src -I$(GL_DEV)/usr/include
# -MMD: objects follow the (patched) LUS headers, so a new patch rebuilds what it touches
XX := $(CXX) -std=c++20 $(CXXFLAGS) $(LUS_DEFS) $(INC) -MMD -MP
ifeq ($(wildcard $(LUS_TOOLS)/.complete),)
$(error $(LUS_TOOLS) is missing: run port/gfx/fast3d/setup-lus.sh)
endif

LUS_SRC := $(LUS)/src/fast/interpreter.cpp $(LUS)/src/fast/backends/gfx_opengl.cpp \
           $(LUS)/src/fast/debug/GfxDebugger.cpp $(LUS)/src/fast/resource/type/Texture.cpp
PRISM_SRC := $(wildcard $(PRISM)/src/prism/*.cpp $(PRISM)/src/prism/utils/*.cpp)
PRISM_CSRC := $(wildcard $(PRISM)/src/prism/utils/*.c)
OBJS := $(patsubst %.cpp,$(OBJ)/lus/%.o,$(notdir $(LUS_SRC))) $(patsubst %.cpp,$(OBJ)/prism/%.o,$(notdir $(PRISM_SRC))) \
        $(patsubst %.c,$(OBJ)/prism/%.c.o,$(notdir $(PRISM_CSRC))) \
        $(OBJ)/ship_shim.o $(OBJ)/egl_png.o
SHIM_H := $(shell find shim -name '*.h')

$(OUT)/f3dreplay: $(OBJ)/f3dreplay.o $(OBJS)
	$(CXX) -o $@ $^ $(LIBDIR)/libEGL.so.1 $(LIBDIR)/libGL.so.1 -lm

# the in-process renderer (port_fast3d.cpp, plan 19.7) driven by test_inproc on the 64-bit host
$(OUT)/test_inproc: $(OBJ)/test_inproc.o $(OBJ)/port_fast3d.o $(OBJ)/settings.o $(OBJS)
	$(CXX) -o $@ $^ $(LIBDIR)/libEGL.so.1 $(LIBDIR)/libGL.so.1 -lm

# the live presenter (plan 19.11): X11 + EGL window, the game's frames over shared memory
# (+ plan 19.12: the audio thread, libpulse-simple dlopen'd, headers from setup-pulse-dev.sh; an evdev gamepad)
# (+ plan 27: the settings file, the input mapping, the settings menu)
M7_OBJS := $(OBJ)/settings.o $(OBJ)/input_map.o $(OBJ)/settings_menu.o $(OBJ)/menu_overlay_gl.o
$(OUT)/presenter: $(OBJ)/presenter.o $(OBJ)/live_window.o $(OBJ)/live_audio.o $(OBJ)/live_gamepad.o $(OBJ)/input_script.o $(M7_OBJS) $(OBJS)
	$(CXX) -o $@ $^ $(LIBDIR)/libEGL.so.1 $(LIBDIR)/libGL.so.1 $(LIBDIR)/libX11.so.6 -lm -ldl -lpthread
$(OBJ)/presenter.o: presenter.cpp port_fast3d_n64.h ../dlump.h ../live_shm.h ../../host.h ../replay/egl_png.h live_audio.h live_gamepad.h \
               live_window.h live_backend.h $(SHIM_H) | $(OBJ)
	$(XX) -I$(X11_DEV)/usr/include -Wall -Wextra -Wno-unused-parameter -c $< -o $@
# the window (X11 + EGL) and its input, shared by the presenter and the native host (plan 21.9)
$(OBJ)/live_window.o: live_window.cpp live_window.h live_audio.h live_gamepad.h settings_menu.h ../live_shm.h ../../input_map.h ../../settings.h | $(OBJ)
	$(CXX) -std=c++20 $(CXXFLAGS) -I$(X11_DEV)/usr/include -I$(GL_DEV)/usr/include -Wall -Wextra -c $< -o $@
$(OBJ)/live_audio.o: live_audio.cpp live_audio.h ../live_shm.h | $(OBJ)
	$(CXX) -std=c++20 $(CXXFLAGS) -I$(PULSE_DEV)/usr/include -Wall -Wextra -c $< -o $@
# --input (run 865): the game's own script reader (port/host/input_script.c); presenter.cpp supplies its host calls
$(OBJ)/input_script.o: ../../host/input_script.c ../../host.h ../../port_libc.h | $(OBJ)
	$(CC) -O2 -I../.. -Wall -c $< -o $@
$(OBJ)/live_gamepad.o: live_gamepad.cpp live_gamepad.h ../../input_map.h | $(OBJ)
	$(CXX) -std=c++20 $(CXXFLAGS) -Wall -Wextra -c $< -o $@
$(OBJ)/settings.o: ../../settings.c ../../settings.h | $(OBJ)
	$(CC) -std=gnu99 -O2 -Wall -Wextra -c $< -o $@
$(OBJ)/input_map.o: ../../input_map.c ../../input_map.h ../../settings.h | $(OBJ)
	$(CC) -std=gnu99 -O2 -Wall -Wextra -c $< -o $@
$(OBJ)/settings_menu.o: settings_menu.cpp settings_menu.h menu_font.h ../../input_map.h ../../settings.h | $(OBJ)
	$(CXX) -std=c++20 $(CXXFLAGS) -Wall -Wextra -c $< -o $@
$(OBJ)/menu_overlay_gl.o: menu_overlay_gl.cpp menu_overlay_gl.h | $(OBJ)
	$(CXX) -std=c++20 $(CXXFLAGS) -I$(GL_DEV)/usr/include -Wall -Wextra -c $< -o $@

$(OBJ)/lus/%.o: $(LUS)/src/fast/%.cpp $(SHIM_H) | $(OBJ)/lus
	$(XX) -w -c $< -o $@
$(OBJ)/lus/%.o: $(LUS)/src/fast/backends/%.cpp $(SHIM_H) | $(OBJ)/lus
	$(XX) -w -c $< -o $@
$(OBJ)/lus/%.o: $(LUS)/src/fast/debug/%.cpp $(SHIM_H) | $(OBJ)/lus
	$(XX) -w -c $< -o $@
$(OBJ)/lus/%.o: $(LUS)/src/fast/resource/type/%.cpp $(SHIM_H) | $(OBJ)/lus
	$(XX) -w -c $< -o $@
$(OBJ)/prism/%.o: $(PRISM)/src/prism/%.cpp $(SHIM_H) | $(OBJ)/prism
	$(XX) -w -c $< -o $@
$(OBJ)/prism/%.o: $(PRISM)/src/prism/utils/%.cpp | $(OBJ)/prism
	$(XX) -w -c $< -o $@
$(OBJ)/prism/%.c.o: $(PRISM)/src/prism/utils/%.c | $(OBJ)/prism
	$(CC) -O2 -w -c $< -o $@
$(OBJ)/ship_shim.o: ship_shim.cpp $(SHIM_H) | $(OBJ)
	$(XX) -Wall -Wextra -Wno-unused-parameter -c $< -o $@
$(OBJ)/f3dreplay.o: f3dreplay.cpp egl_window.h ../dlump.h ../replay/egl_png.h $(SHIM_H) | $(OBJ)
	$(XX) -Wall -Wextra -Wno-unused-parameter -c $< -o $@
$(OBJ)/port_fast3d.o: port_fast3d.cpp port_fast3d.h egl_window.h ../replay/egl_png.h ../../settings.h $(SHIM_H) | $(OBJ)
	$(XX) -Wall -Wextra -Wno-unused-parameter -c $< -o $@
$(OBJ)/test_inproc.o: test_inproc.c port_fast3d.h ../dlump.h | $(OBJ)
	$(CC) -std=gnu99 -O2 -Wall -Wextra -c $< -o $@
$(OBJ)/egl_png.o: ../replay/egl_png.c ../replay/egl_png.h | $(OBJ)
	$(CC) -std=gnu99 -O2 -Wall -Wextra -I$(GL_DEV)/usr/include -c $< -o $@

$(OBJ) $(OBJ)/lus $(OBJ)/prism:
	mkdir -p $@

-include $(wildcard $(OBJ)/*.d $(OBJ)/lus/*.d $(OBJ)/prism/*.d)

.PHONY: all
all: $(OUT)/f3dreplay $(OUT)/test_inproc

# the live-audio check (run 867): a windowed Match Race in lockstep, played / received >= 95 %.
# Needs a display and PulseAudio (WSLg), so it is not part of the port gate; AUDIO_CHECK_ARGS go to
# port/tools/audio-check.sh (-f STEPS, -m MIN_PERCENT, -o DIR).
.PHONY: audio-check
audio-check:
	../../tools/audio-check.sh $(AUDIO_CHECK_ARGS)

# the same check on the native 64-bit game (plan 21.10: play.sh --native, its own window and audio)
.PHONY: audio-check-native
audio-check-native:
	../../tools/audio-check.sh --native $(AUDIO_CHECK_ARGS)
