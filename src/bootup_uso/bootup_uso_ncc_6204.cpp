/* IDO 7.1 NCC (C++) donor for func_00006204 (0x24 / 9 words): NOT a hand-written
 * function -- it is the static initializer (edg `__sti__<stamp>`) that NCC
 * generates for a namespace-scope object whose class has an out-of-line
 * default constructor. Body = `T::T(&obj)`.
 *
 * Why every C spelling failed (agent-c run 349, toolchain forensics): the
 * target schedules `lui a0` ABOVE `sw ra`. In a normal function the prologue
 * and the first statement share the `{` line, so as1's tie-break keeps
 * `sw ra` first (kit CDXVIII). edg emits the __sti__ at the END of the
 * translation unit (its prologue carries the last source line) while the
 * constructor call carries the OBJECT DEFINITION's line, which is earlier, so
 * the argument `lui` outranks `sw ra`. Requires at least one source line after
 * the definition (a definition on the file's last line gives `sw ra` first).
 *
 * Symbols renamed after compile (names only, no bytes change):
 * __sti__<timestamp> -> func_00006204, __ct__8Bu6204ObFv -> func_00000000.
 * The object keeps C linkage under its pinned name D_00000000. */
struct Bu6204Ob {
    Bu6204Ob();
    int w;
};

extern "C" {
Bu6204Ob D_00000000;
}

/* end of the translation unit: the __sti__ prologue takes this later line */
