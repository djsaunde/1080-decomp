/* IDO 7.1 NCC (C++) donor for gl_func_00028A68 (0xA4 / 41 words): init a list head and
 * register every 0xD0-byte record of the global pool with it, storing each
 * record's own address at +8 and clearing +0. Under NCC the addu operand order
 * follows the source (kit XX): the stored value is an int sum (off first), the
 * store address a pointer sum (base first), the call argument an int sum cast
 * back to a pointer.
 * Spliced into game_libs_post.c.o via REPLACE_FUNC_BODY. */
#define GL_COUNT_2070 (*(int*)((char*)&D_00000000 + 0x2070))
extern "C" {
extern void gl_func_00000000(...);
extern int D_00000000;
void gl_func_00028A68(void) {
    char *tbl = (char *)&D_00000000 + 0x5378;
    int i;
    int off;
    gl_func_00000000(tbl);
    for (i = 0, off = 0; i < GL_COUNT_2070; i++, off += 0xD0) {
        char *base = *(char **)((char *)&D_00000000 + 0x2CFC);
        *(int *)(base + off + 8) = off + (int)base;
        *(int *)(*(int *)((char *)&D_00000000 + 0x2CFC) + off) = 0;
        gl_func_00000000(tbl, (char *)(off + *(int *)((char *)&D_00000000 + 0x2CFC)));
    }
}

}
