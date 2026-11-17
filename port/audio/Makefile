# port/audio: native build of the CPU audio mixer tests (no game code).
#   make -C port/audio test        (64-bit host)
#   make -C port/audio test M32=1  (the port's -m32 target)
CC ?= cc
CFLAGS ?= -std=c99 -O2 -Wall -Wextra -pedantic
ifdef M32
CFLAGS += -m32
endif

test_mixer: test_mixer.c mixer.c abi_decode.c mixer.h
	$(CC) $(CFLAGS) -o $@ test_mixer.c mixer.c abi_decode.c

test_audio_swap: test_audio_swap.c audio_swap.c mixer.c abi_decode.c audio_swap.h mixer.h
	$(CC) $(CFLAGS) -o $@ test_audio_swap.c audio_swap.c mixer.c abi_decode.c -lm

ASSETS ?= ../assets/extracted

test: test_mixer test_audio_swap
	./test_mixer $(ASSETS)
	./test_audio_swap $(ASSETS)

# M4 regression sweep (plan 15.8): every sequence, every course start and a set of sound effects
# through the real driver (port/audio/tools/seqrender, CMake target seqrender with PORT_LINK=ON),
# checked against port/audio/tools/regress-golden.tsv.
#   make -C port/audio regress SEQRENDER=<build>/seqrender [RUN32=<monorepo>/tools/i386-sysroot/run32]
#   make -C port/audio regress-update SEQRENDER=...   (rewrite the golden hashes after an intended change)
SEQRENDER ?=
RUN32 ?= $(abspath ../../../../tools/i386-sysroot/run32)
ROM ?= $(abspath ../../baserom.z64)
REGRESS = python3 tools/regress.py --seqrender $(SEQRENDER) --run32 $(RUN32) --rom $(ROM)

regress:
	@test -n "$(SEQRENDER)" || { echo "set SEQRENDER=<cmake build>/seqrender"; exit 2; }
	$(REGRESS)

regress-update:
	@test -n "$(SEQRENDER)" || { echo "set SEQRENDER=<cmake build>/seqrender"; exit 2; }
	$(REGRESS) --update

clean:
	rm -f test_mixer test_audio_swap

.PHONY: test clean regress regress-update
