#include "common.h"

/* word memcpy: copies n words from src to dst. Uses an internal counter
 * variable so the loop guard is independent of the per-iteration count. */
void __rmonCopyWords(u32* dst, u32* src, s32 n) {
    register s32 ctr = n;
    char pad[4];
    n--;
    if (ctr != 0) {
        do {
            *dst = *src;
            ctr = n;
            n--;
            dst++;
            src++;
        } while (ctr != 0);
    }
}
