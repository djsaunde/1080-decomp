#include "common.h"

extern s32 func_800077DC(s32);
extern s32 func_800096B0(s32);
extern s32 __osActiveQueue;

/* split from kernel_039.c - 2026-06-10 kernel ROM-order relayout */


/* __osGetActiveQueue */
s32 __osGetActiveQueue(void) {
    return __osActiveQueue;
}
