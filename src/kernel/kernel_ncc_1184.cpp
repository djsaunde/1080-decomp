/* IDO 7.1 NCC (C++) donor for func_80001184 (0x4C / 19 words): zero the
 * running allocation total D_80012D5C, then an 11-word table at 0x80012D30.
 *
 * The target shares ONE `lui at` between the 0x80012D30 / 0x80012D34 stores:
 * as1 does that only for accesses to the SAME file-local (static) symbol
 * (kit LX), so the table is module-local data -- `static int sTab[11]`. The
 * plain zeroing loop then compiles to the target exactly (IDO peels three
 * stores and unrolls the rest x4; the loop bound is &sTab[11] = 0x80012D5C).
 * cfe gives the identical bytes; NCC only because a .cpp donor stays out of
 * the C_FILES link. The splice renames the .bss reloc to func_80001184_bss,
 * pinned to 0x80012D30 in undefined_syms_auto.txt. */
extern "C" int D_80012D5C;
static int sTab[11];
extern "C" void func_80001184(void) {
    int i;

    D_80012D5C = 0;
    i = 0;
    do {
        sTab[i] = 0;
        i++;
    } while (i < 11);
}
