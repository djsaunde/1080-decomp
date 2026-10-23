/* IDO 7.1 NCC (C++) donor for gl_func_0005165C (0x40 / 16 words incl. its
 * 2-word hoisted head at 0x51654): alloc-or-passthrough zeroing setter.
 * The old "shared-$at store-pair cap" (one `lui at` + `sw zero,4(at); sw
 * zero,0(at)`) is a file-local STATIC pair written through two references
 * in one block (kit LX, agent-c 34A78); as1 shares the lui only for the same
 * static symbol. The splice renames the donor's .bss reloc to
 * gl_func_0005165C_bss, pinned to 0 (base-0 USO data) in
 * undefined_syms_auto.txt. Head: `(unsigned)p == 0` keeps the alloc test.
 * Spliced into game_libs_post0b.c.o via REPLACE_FUNC_BODY (agent-g run 141). */
extern "C" {
extern int gl_func_00000000(...);
extern int D_5165C_arr[];
}
static int s5165c[2];
extern "C" void gl_func_0005165C(void) {
    int *p = D_5165C_arr;
    if ((unsigned)p == 0) {
        p = (int*)gl_func_00000000(4);
        if (p == 0) {
            goto skip;
        }
    }
    *p = 0;
skip:
    { int &a = s5165c[1]; int &b = s5165c[0]; a = 0; b = 0; }
}
