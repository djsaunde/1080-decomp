#include "common.h"

extern int gl_func_00000000();

/* EXACT 2026-10-22 (agent-g run 118) via an IDO 7.1 NCC -O0 donor:
 * src/arcproc_uso/arcproc_uso_ncc_o0_12C.cpp. The "extra-branch" residual above is cfe's -O0
 * closing-brace b-epilogue, which edg does not emit. Placeholder spliced over. */
int arcproc_uso_func_0000012C(int *a0, int a1) {
    return 0;
}

/* arcproc_uso_func_0000019C: -O0 size allocator-state update (41 insns, 0xA4).
 * Adds allocation of size a1 to slab at a0. If count==0, store at slot 0;
 * else sum existing allocations and store new at slot=count with value
 * (a1 - sum). Both paths fall through to a0[13] = a1 (cached total).
 *
 * Promoted to exact via if/else fall-through merge (replacing the
 * earlier `if (==0) {store; return}` early-return form). The single
 * 1-byte diff at the early-exit branch displacement (build `b +29` to
 * epilogue vs expected `b +26` to the a0[13] = a1 store) is fixed by
 * letting both arms fall through to the merge. */
void arcproc_uso_func_0000019C(int *a0, int a1) {
    int i, j;
    if (a0[1] == 0) {
        a0[8 + a0[1]] = a1;
    } else {
        i = 0;
        j = 0;
        if (a0[1] > 0) {
            do {
                i += a0[8 + j];
            } while (++j < a0[1]);
        }
        a0[8 + a0[1]] = a1 - i;
    }
    a0[13] = a1;
}
