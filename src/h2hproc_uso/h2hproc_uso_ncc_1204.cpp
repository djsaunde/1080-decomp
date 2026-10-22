/* IDO 7.1 NCC (C++) donor for h2hproc_uso_func_00001204 (0x15C / 87 words): fade a
 * 0..720 alpha up or down by 16, then run the registered state callback. Decode fix:
 * each clamp is NESTED inside its step (`if (a < 0xFF) { a += 16; if (a >= 0x100)
 * a = 0x2D0; }`) -- both branches of the target skip to the shared tail; the old NM
 * clamped unconditionally. The two calls whose results are discarded go through a
 * void alias (kit XVIII #3) so the record base takes v0. Spliced into
 * h2hproc_uso.c.o via REPLACE_FUNC_BODY. */
extern "C" {
extern char D_00000000;
int gl_func_00000000(...); void h2h_void_1204(...);
void h2hproc_uso_func_00001204(char *a0) {
    char *p1;
    char *v0;

    if (*(int*)(*(char**)(a0 + 0x2C) + 0x4F4) < 2) {
        int *fp;
        v0 = *(char**)(a0 + 0x34);
        fp = (int *)(v0 + 0x3C);
        if (*fp < 0xFF) {
            *fp += 0x10;
            v0 = *(char**)(a0 + 0x34);
            if (*(int*)(v0 + 0x3C) >= 0x100) {
                *(int*)(v0 + 0x3C) = 0x2D0;
            }
        }
    } else {
        int *fp;
        v0 = *(char**)(a0 + 0x34);
        fp = (int *)(v0 + 0x3C);
        if (*fp > 0) {
            *fp -= 0x10;
            v0 = *(char**)(a0 + 0x34);
            if (*(int*)(v0 + 0x3C) < 0) {
                *(int*)(v0 + 0x3C) = 0;
            }
        }
    }
    if (gl_func_00000000(&D_00000000, 0x40100) != 0) {
        if (gl_func_00000000(*(int*)(&D_00000000 + 0x190)) != 0) {
            p1 = *(char**)(a0 + 0x30);
            if (*(int*)(p1 + *(int*)(p1 + 0x7C) * 0x28 + 0x90) != 0) {
                h2h_void_1204(5);
                *(int*)((char*)&D_00000000 + 0x4) = *(int*)(a0 + 0x30);
                *(int*)((char*)&D_00000000 + 0x0) = *(int*)(a0 + 0x2C);
                p1 = *(char**)(a0 + 0x30);
                (*(void (**)(void))(p1 + *(int*)(p1 + 0x7C) * 0x28 + 0x90))();
            }
        }
    }
    h2h_void_1204(a0);
}
}
