#include "common.h"
#include "uso_call.h"

/* gl___osSiRawWriteIo = libultra __osPiRawWriteIo (io/pirawwrite.c), statically
 * linked into the game_libs USO. Byte-exact masked-twin of matched kernel
 * __osSiRawWriteIo (kernel_046): PI-busy probe, then uncached KSEG1 write of data
 * to (devAddr | 0xA0000000). IDO 5.3 -O1, spliced into game_libs_post2b_d.c.o
 * via REPLACE_FUNC_BODY. The busy probe collapses to the address-0 placeholder
 * gl_func_00000000. (Write twin of the landed gl___osSiRawReadIo read.) */
extern int gl_func_00000000();

int gl___osSiRawWriteIo(int devAddr, int data) {
    if (CALL(gl___osSiDeviceBusy)() != 0) {
        return -1;
    }
    *(volatile int *)(0xA0000000 | devAddr) = data;
    return 0;
}
