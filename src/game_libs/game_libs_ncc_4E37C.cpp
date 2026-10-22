/* IDO 7.1 NCC (C++) donor for game_libs_func_0004E37C (0x78 / 30 words): append to a
 * {tbl, ?, cap, n} registry unless the global flag bit is set -- an inline
 * Reg::add() whose tbl[n++] expression temp gives the target's v1 count (kit XVII #1).
 * Spliced into game_libs_post0b.c.o via REPLACE_FUNC_BODY. */
extern "C" {
extern char D_00000000[];
void gl_func_00000000(...);
}
struct Reg { int *tbl; int pad; int cap; int n;
    void add(int o) { if (n >= cap) gl_func_00000000(D_00000000 + 0x204D4, o); tbl[n++] = o; } };
extern "C" void game_libs_func_0004E37C(Reg *r, int a1) {
    if ((*(int *)(D_00000000 + 0x1C4) & 1) == 0) r->add(a1);
}
