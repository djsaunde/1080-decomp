/* IDO 7.1 NCC (C++) donor for game_libs_func_0003DB38 (0xB4 / 45 words): among the
 * children, the one whose +0x38 value is the smallest above ref (a1's own value
 * or a pool default) and below a pool limit. The loop variable's registers are
 * colored unconstrained, in first-occurrence (bitpos) order: the dead `n = 0`
 * before the loop makes n occur before the next() temp, so n takes v0 and the
 * temp a0 as in the target.
 * Spliced into game_libs_post0b.c.o via REPLACE_FUNC_BODY. */
extern "C" float D_00000000[];
struct Node { char pad[0x38]; float v; };
struct Link { Node *obj; Link *next; };
struct Iter {
    Link *cur, *nxt;
    Iter(Link *head) { nxt = head; }
    Node *first() { return next(); }
    Node *next() { return (cur = nxt) ? (nxt = nxt->next, cur->obj) : 0; }
};
struct Owner { char pad[0x10]; Link *head; };
extern "C" Node *game_libs_func_0003DB38(Owner *o, Node *a1) {
    float ref = a1 ? a1->v : D_00000000[0x1AC4 / 4];
    Iter it(o->head);
    float lim = D_00000000[0x1AC8 / 4];
    Node *best = 0;
    Node *n = 0;
    for (n = it.first(); n; n = it.next())
        if (ref < n->v && n->v < lim) { best = n; lim = n->v; }
    return best;
}
