# port/gfx/replay: the 64-bit DLMP replayer (PC port plan 19). Needs the EGL / GL
# headers: ./setup-gl-dev.sh (no sudo) or libegl-dev + libgl-dev.
REPO ?= $(abspath ../../../../..)
GL_DEV ?= $(REPO)/tools/gl-dev/root
CC ?= cc
CFLAGS ?= -std=gnu99 -O2 -Wall -Wextra
OUT ?= .
LIBDIR ?= /lib/x86_64-linux-gnu

$(OUT)/dlreplay: dlreplay.c ../dlump.h
	$(CC) $(CFLAGS) -I$(GL_DEV)/usr/include -o $@ dlreplay.c $(LIBDIR)/libEGL.so.1 $(LIBDIR)/libGL.so.1 -lm

.PHONY: all
all: $(OUT)/dlreplay
