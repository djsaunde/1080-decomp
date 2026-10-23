/* IDO 7.1 NCC (C++) donor for gl_func_00035DAC (0xB0 / 44 words): an outer
 * loop that re-initialises an end marker through gl_ref_0004A110(&end),
 * copies a -1 sentinel into the cursor, and stops when the cursor equals the
 * end. Otherwise it steps a counter through gl_ref_0004A14C(&cnt) and then
 * calls gl_ref_0004A14C(&x) once per 4 counted.
 *
 * Load-bearing (agent-c run 133; cfe NM was 58.9 / 39 words):
 * - the outer loop starts at `end = 0; init(&end)` (the old NM ran the init
 *   once, before the loop);
 * - every value is a one-word stack object whose address is taken. Their
 *   addresses are loop-invariant, so NCC hoists them into s0-s4, and
 *   `cur = sent` is a copy through two of those pointers. cfe
 *   const-propagates the -1 instead;
 * - frame = declaration order: sent 76, end 72, cnt 68, x 64, two unused
 *   words (u1 / u2: each takes a slot because it is assigned), cur 52.
 *   Frame 80.
 * Spliced into game_libs_post0b.c.o via REPLACE_FUNC_BODY. */
extern "C" { void gl_ref_0004A110(...); void gl_ref_0004A14C(...); }
struct W { int v; };
extern "C" void gl_func_00035DAC(void) {
    W sent, end, cnt, x, u1, u2, cur;
    u1.v = 0; u2.v = 0;
    for (;;) {
        end.v = 0; gl_ref_0004A110(&end);
        sent.v = -1;
        cur = sent;
        if (cur.v == end.v) break;
        gl_ref_0004A14C(&cnt);
        while (cnt.v != 0) { gl_ref_0004A14C(&x); cnt.v -= 4; }
    }
}
