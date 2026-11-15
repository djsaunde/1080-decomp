/* Native original-compiler source port from src/kernel/kernel_000.c. Target bytes unchanged. */
#include "context_kernel_000.h"
u32 uso_alloc(u32 size, u32 alignment) {
    u32 v;
    u32 r;
    if ((size | 0) & (alignment - 1)) {
        size = (size + alignment) & ~(alignment - 1);
    }
    v = gUsoHeapCur;
    gUsoHeapCur = v + size;
    gUsoMemUsed += size;
    r = v;
    v = 0;
    if (gUsoHeapCur >= gUsoHeapEnd) {
        return 0;
    }
    return r;
}
