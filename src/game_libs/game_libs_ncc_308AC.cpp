/* IDO 7.1 NCC (C++) donor for game_libs_func_000308AC (0xF8 / 62 words = the old
 * 7-word head game_libs_func_000308AC + gl_func_000308C8, one function; neither
 * 0x308AC's successor 0x308C8 is exported nor called). Mode setter for a
 * module-local state block: mode / f8 / fC / f1C in the head, per-mode f4 / f14 /
 * f1C, then f10 = f14 and two blank-import emits ((s8)f4 as `lb a1,7(a1)`, the
 * float f10 in a1 as `lw a1,16(a1)`).
 * The block is FILE-LOCAL data (static): as1 reuses one `lui at` for the tail
 * f14 load + f10 store only for the same local symbol, and keeps the store ahead
 * of the jal (lb in the delay) only when the lb hits that same symbol. Per-site
 * C++ references stop uopt from CSE-ing &A308 into v0 outside the head (the head's
 * four stores are the only v0 CSE in the target). The donor's .bss reloc is
 * renamed game_libs_func_000308AC_bss by the splice and pinned to 0 (the USO
 * relocs bake section-relative offsets 0x0..0x1C); D_308AC_k* are the second
 * data block (offsets 0x1868..0x1870), pinned to 0.
 * Spliced into game_libs_post.c.o via REPLACE_FUNC_BODY (agent-c run 136). */
typedef float f32;
typedef signed char s8;
typedef struct { int mode; int f4; int f8; int fC; f32 f10; f32 f14; f32 f18; f32 f1C; } St308;
static St308 A308;
extern "C" int gl_func_00000000(...);
extern "C" void gl_func_00000000_f(int, f32);
extern "C" char D_308AC_k0[], D_308AC_k1[], D_308AC_k2[];
extern "C" void game_libs_func_000308AC(int mode) {
    A308.mode = mode;
    A308.f8 = 0;
    A308.fC = 0;
    A308.f1C = *(f32 *)(D_308AC_k0 + 0x1868);
    switch (mode) {
    case 0:
        { f32 &r = A308.f14; r = *(f32 *)(D_308AC_k1 + 0x186C); }
        gl_func_00000000_f(0x01000800, 0.0f);
        return;
    case 1:
        { int &r = A308.f4; r = 0; }
        { f32 &r = A308.f14; r = 0.5f; }
        break;
    case 2:
        { int &r = A308.f4; r = 8; }
        { f32 &r = A308.f14; r = 0.5f; }
        { f32 &r = A308.f1C; r = *(f32 *)(D_308AC_k2 + 0x1870); }
        break;
    }
    { f32 &r = A308.f10; f32 &q = A308.f14; r = q; }
    { int &r = A308.f4; gl_func_00000000(0x06000800, (s8)r); }
    { f32 &r = A308.f10; gl_func_00000000_f(0x01000800, r); }
}
