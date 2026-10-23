/* IDO 7.1 NCC (C++) donor for timproc_uso_b5_func_00008AFC (0x120 / 72 words):
 * per-group element refresh through the cfront virtual slot 0x58/0x5C.
 * K = 0.9f is a source literal (donor .rodata pinned at RoData +0x238 as
 * timproc_uso_b5_func_00008AFC_rodata); the this-adjust is spelled
 * `(char *)(delta + (int)e)` for the target's addu operand order.
 * Spliced into timproc_uso_b5.c.o via REPLACE_FUNC_BODY. */
typedef short s16;
typedef float f32;
extern "C" {
extern int timproc_uso_b5_func_00008A38(...);
}
extern "C" void timproc_uso_b5_func_00008AFC(char *scr) {
    f32 K = 0.9f;
    int g, i;
    char *e;
    char *vt;
    char *o;

    for (g = 0; g < 4; g++) {
        for (i = 0; i < *(int *)(*(char **)(*(char **)(scr + 0x40C) + g * 4 + 0x40) + 0x6C); i++) {
            o = (char *)timproc_uso_b5_func_00008A38(scr);
            e = *(char **)(*(char **)(*(char **)(scr + 0x40C) + g * 4 + 0x40) + i * 4 + 0x3C);
            if (o != e) {
                o = *(char **)(e + 0x28);
                ((void (*)(char *, f32))*(int *)(o + 0x5C))((char *)(*(s16 *)(o + 0x58) + (int)e), 1.0f - *(f32 *)(scr + 0x480) * K);
            }
        }
    }
    o = (char *)timproc_uso_b5_func_00008A38(scr);
    vt = *(char **)(o + 0x28);
    ((void (*)(char *, f32))*(int *)(vt + 0x5C))((char *)(*(s16 *)(vt + 0x58) + (int)o), 1.0f);
}
