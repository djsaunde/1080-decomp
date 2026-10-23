/* IDO 7.1 NCC (C++) donor for game_libs_func_0003D54C (0x70 / 28 words):
 * return the value of the n-th node of the owner's child list (0 when the
 * list runs out), walked with the stack {cur, nxt} iterator of 3D48C / 38964
 * (cur at sp+0, nxt at sp+4; next() advances through nxt).
 *
 * Load-bearing (agent-c run 133; cfe NM was 80.2, 9 words):
 * - the entry call goes through an inline first() { return next(); } (the
 *   3DDC0 trick): its own edg return temp keeps next()'s ?: result in a0 and
 *   the loop variable as a separate v1 copy in both loop-test delay slots
 *   (with next() at entry the two coalesce: 8 words);
 * - `int r = 0;` initialised at its declaration: without it the copy is
 *   there but r and the temp swap a0 / v1 (9 words).
 * Spliced into game_libs_post0b.c.o via REPLACE_FUNC_BODY. */
struct Link { int obj; Link *next; };
struct Iter {
    Link *cur, *nxt;
    Iter(Link *head) { nxt = head; }
    int first() { return next(); }
    int next() { return (cur = nxt) ? (nxt = nxt->next, cur->obj) : 0; }
};
struct Owner { char pad[0x10]; Link *head; };
extern "C" int game_libs_func_0003D54C(Owner *o, int n) {
    Iter it(o->head);
    int i = 0;
    int r = 0;
    for (r = it.first(); r; r = it.next()) {
        if (i == n) break;
        i++;
    }
    return r;
}
