/* Native original-compiler source port from src/mgrproc_uso/mgrproc_uso.c. Target bytes unchanged. */
#include "context_mgrproc_uso.h"
void mgrproc_uso_func_00002850(MgrState *state, s32 x, s32 y, s32 count) {
    extern void import_0024E608(void *, s32, f32 *);
    extern void import_0024F2C8(void *);
    extern void import_0024F34C(void *, s32, s32, s32);
    f32 buf[4];
    volatile int pad0, pad1, pad2, pad3;
    int n;
    char *tgt;
    int i;
    int off;
    int p1;
    int p2;
    buf[0] = 1.0f; buf[1] = 1.0f; buf[2] = 1.0f; buf[3] = 1.0f;
    n = (int)((0, 255.0f) * state->field_7A4);
    import_0024E608(&import_8024CAF8, n, buf);
    tgt = &state->field_6F8;
    import_0024F2C8(tgt);
    i = count - 1;
    if (i >= 0) {
        off = i << 2;
        p1 = x + off;
        p2 = y - off;
        do {
            import_0024F34C(tgt, p1, p2, 3);
            off -= 4;
            p1 -= 4;
            p2 += 4;
        } while (off >= 0);
    }
}
