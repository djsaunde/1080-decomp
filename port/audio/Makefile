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

clean:
	rm -f test_mixer test_audio_swap

.PHONY: test clean
