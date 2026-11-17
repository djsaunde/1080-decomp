# port/gfx: native build of the display-list dumper test (no game code) and the
# 64-bit replayer (port/gfx/replay).
#   make -C port/gfx test         64-bit host
#   make -C port/gfx test M32=1   the game's -m32 target (tools/i386-sysroot/gcc32 + run32)
#   make -C port/gfx replay-test  render the synthetic frame to synth.png (needs GL_SYSROOT)
REPO ?= $(abspath ../../../..)
REPO_GAME ?= $(abspath ../..)
ifdef M32
CC := $(REPO)/tools/i386-sysroot/gcc32
RUN := $(REPO)/tools/i386-sysroot/run32
else
CC ?= cc
RUN :=
endif
CFLAGS ?= -std=gnu99 -O2 -Wall -Wextra
OUT ?= .

$(OUT)/test_dlump: test_dlump.c dlump.c synth.c dlump.h synth.h
	$(CC) $(CFLAGS) -o $@ test_dlump.c dlump.c synth.c

# the real hook (port_gfx.c + mincore); meaningful on the -m32 target only
$(OUT)/test_port_gfx: test_port_gfx.c port_gfx.c dlump.c synth.c dlump.h synth.h
	$(CC) $(CFLAGS) -malign-double -DPORT -isystem $(REPO_GAME)/include -o $@ test_port_gfx.c port_gfx.c dlump.c synth.c

test: $(OUT)/test_dlump $(if $(M32),test-hook)
	$(RUN) $(OUT)/test_dlump $(OUT)/synth.dlmp
	python3 tools/dlump.py --check $(OUT)/synth.dlmp

# the OSTask_t layout asserts only hold on the 32-bit target
test-hook: $(OUT)/test_port_gfx
	$(RUN) $(OUT)/test_port_gfx $(OUT)
	python3 tools/dlump.py --check $(OUT)/gfx_000001.dlmp $(OUT)/gfx_000005.dlmp

replay-test: test
	$(MAKE) -C replay OUT=$(abspath $(OUT))
	$(abspath $(OUT))/dlreplay $(OUT)/synth.dlmp $(OUT)/synth.png

clean:
	rm -f $(OUT)/test_dlump $(OUT)/test_port_gfx $(OUT)/gfx_*.dlmp $(OUT)/synth.dlmp $(OUT)/synth.png $(OUT)/dlreplay

.PHONY: test test-hook replay-test clean
