/* IDO 7.1 NCC (C++) donor for gl_func_00066A50 (0x94 / 37 words): the panic
 * handler that logs, resets the two canary words to 0x12345678 (checked by
 * the sibling gl_func_00066AF0) and hangs. The "shared-$at canary cell cap"
 * (one `lui at` + `sw t8,-3312(at); sw t8,-3308(at)` with the constant in the
 * ugen temp t8) is ONE 64-bit static store: as1 shares the lui only for the
 * same static symbol (kit LX), and a `long long` store keeps 0x12345678 as a
 * single ugen temp (an int pair makes it a uopt constant candidate in v0).
 * The splice renames the donor's .bss reloc to gl_func_00066A50_bss, pinned
 * to the canary address 0x3F310 in undefined_syms_auto.txt.
 * gl_ncc_66a50_phase (0x1C) only sets the 32-byte phase of the dead epilogue
 * (5 nops) as in the original TU; it is not spliced.
 * Spliced into game_libs_post1b.c.o via REPLACE_FUNC_BODY (agent-g run 142). */
extern "C" {
extern int gl_func_00000000(...);
extern int D_00000000;
}
static long long s66a50_canary;
extern "C" void gl_ncc_66a50_phase(int *p) { p[0] = 0; p[1] = 0; p[2] = 0; p[3] = 0; p[4] = 0; p[5] = 0; }
extern "C" void gl_func_00066A50(int a0) {
    gl_func_00000000((char *)&D_00000000 + 0x3F160, 2, &D_00000000, 0,
                     (char *)&D_00000000 + 0x41310, 0xA);
    s66a50_canary = 0x1234567812345678LL;
    gl_func_00000000((char *)&D_00000000 + 0x3F160);
    gl_func_00000000(0, 0);
    for (;;) {
    }
}
