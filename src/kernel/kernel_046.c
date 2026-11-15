#include "common.h"

extern s32 func_800056C0(void);

/* PI bus write primitive (write counterpart of __osSiRawReadIo) */
s32 __osSiRawWriteIo(s32 devAddr, s32 data) {
    if (func_800056C0() != 0) {
        return -1;
    }
    *(volatile s32*)(0xA0000000 | devAddr) = data;
    return 0;
}
