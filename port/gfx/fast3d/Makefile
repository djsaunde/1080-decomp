# port/gfx/fast3d: f3dreplay, libultraship's Fast3D (pinned + the plan-14 patch series)
# behind the DLMP dump, rendered offscreen through EGL. Plan section 19.6.
#   ./setup-lus.sh                       fetch LUS + prism into tools/lus, apply the patches
#   ../replay/setup-gl-dev.sh            the EGL / GL headers (no sudo)
#   make                                 build $(OUT)/f3dreplay
# Only three LUS sources are compiled (interpreter, gfx_opengl, GfxDebugger) plus
# Texture.cpp, prism and our Ship shim (shim/README); no SDL, ImGui or spdlog.
REPO ?= $(abspath ../../../../..)
GL_DEV ?= $(REPO)/tools/gl-dev/root
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
XX := $(CXX) -std=c++20 $(CXXFLAGS) $(LUS_DEFS) $(INC)
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
$(OBJ)/f3dreplay.o: f3dreplay.cpp ../dlump.h ../replay/egl_png.h $(SHIM_H) | $(OBJ)
	$(XX) -Wall -Wextra -Wno-unused-parameter -c $< -o $@
$(OBJ)/egl_png.o: ../replay/egl_png.c ../replay/egl_png.h | $(OBJ)
	$(CC) -std=gnu99 -O2 -Wall -Wextra -I$(GL_DEV)/usr/include -c $< -o $@

$(OBJ) $(OBJ)/lus $(OBJ)/prism:
	mkdir -p $@

.PHONY: all
all: $(OUT)/f3dreplay
