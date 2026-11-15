#include "common.h"

/* strcpy-like byte copier: copy bytes from src to dst until (and including)
 * a null terminator. Same instruction shape as the standard libultra
 * __osStrcpy but takes (dst, src) instead of returning dst. */
void rmon_strcpy(u8* dst, u8* src) {
    register s32 c;
    register s32 saved;
    c = *src;
    dst++;
    saved = c;
    src++;
    *(dst - 1) = c;
    while (saved != 0) {
        c = *src;
        dst++;
        src++;
        saved = c;
        *(dst - 1) = c;
    }
}
