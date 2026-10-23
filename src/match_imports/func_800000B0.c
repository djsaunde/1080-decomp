/* Native original-compiler source port from src/kernel/kernel_000.c. Target bytes unchanged. */
#include "context_kernel_000.h"
u32 func_800000B0(u32 size, u32 alignment) {
    u32 v;
    u32 r;
    if ((size | 0) & (alignment - 1)) {
        size = (size + alignment) & ~(alignment - 1);
    }
    v = D_8000A2D8;
    D_8000A2D8 = v + size;
    D_80012D5C += size;
    r = v;
    v = 0;
    if (D_8000A2D8 >= D_8000A2DC) {
        return 0;
    }
    return r;
}
