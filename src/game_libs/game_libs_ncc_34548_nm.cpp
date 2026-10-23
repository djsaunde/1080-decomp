/* IDO 7.1 NCC (C++) NM-only comparison donor for gl_func_00034548: the NM C body lifted under NCC (agent-c run 134 sweep, cfe 73.84).
 * Still NON_MATCHING: spliced into build/non_matching/ only via REPLACE_FUNC_BODY. */
typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef unsigned short u16;
typedef int s32; typedef unsigned int u32; typedef long long s64; typedef unsigned long long u64;
typedef float f32; typedef double f64;
#undef NULL
#define NULL 0
extern "C" {
extern int D_00000000;
extern int gl_func_00000000(...);
}
extern "C" void gl_func_00034548(void *a0) {
    char *g = (char *)&D_00000000;
    char *item;
    char *s0;
    int sp50, sp54, sp5C;
    int r5;
    (void)a0;
    gl_func_00000000(0xC, g + 0x3BB18, 0x10);
    *(int *)(g + 0x3BB38) = 0;
    for (;;) {
        do {
            gl_func_00000000(g + 0x3BB18, &sp5C, 1);
            item = (char *)gl_func_00000000();
        } while (item == 0);
        *(int *)(g + 0x3BB34) = (int)item;
        s0 = item + 0x20;
        (*(int (**)(...))(*(char **)(*(char **)g + 0x70)))(*(int *)(s0 + 0xFC), &sp50, &sp54);
        gl_func_00000000(g + 0x1E424);
        gl_func_00000000(item);
        r5 = gl_func_00000000(*(int *)(item + 0x14));
        gl_func_00000000(g + 0x1E438, sp50, sp54, *(int *)(s0 + 0xFC), r5, *(int *)(item + 0x14));
    }
}
