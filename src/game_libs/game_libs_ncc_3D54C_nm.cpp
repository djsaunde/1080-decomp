/* IDO 7.1 NCC (C++) NM-only comparison donor for game_libs_func_0003D54C
 * (0x70 / 28 words): return the value of the n-th node of the owner's child
 * list (0 when the list runs out), walked with the stack {cur, nxt} iterator
 * of 3D48C / 38964 (sp+0 / sp+4, next() advancing through nxt).
 * NCC 94.5 vs cfe 80.2 (agent-c run 133). Still NON_MATCHING: the target
 * keeps next()'s ?: result in a0 and copies it into the result variable
 * (v1) in both loop-test delay slots; every spelling tried here (result
 * variable initialised / block-scoped / separate copy, for / do / while,
 * pointer or unsigned value type, inline nth()) coalesces the two (8 words).
 * Spliced into build/non_matching/ only via REPLACE_FUNC_BODY. */
struct Link { int obj; Link *next; };
struct Iter {
    Link *cur, *nxt;
    Iter(Link *head) { nxt = head; }
    int next() { return (cur = nxt) ? (nxt = nxt->next, cur->obj) : 0; }
    int get() { return cur->obj; }
};
struct Owner { char pad[0x10]; Link *head; };
extern "C" int game_libs_func_0003D54C(Owner *o, int n) {
    Iter it(o->head);
    int i = 0;
    int r;
    while ((r = it.next()) != 0) {
        if (i == n) break;
        i++;
    }
    return r;
}
