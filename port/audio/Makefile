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

test: test_mixer
	./test_mixer ../assets/extracted

clean:
	rm -f test_mixer

.PHONY: test clean
