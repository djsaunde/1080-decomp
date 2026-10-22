/* IDO 7.1 NCC (C++) donor for game_libs_func_0003DA14 (0x124 / 73 words): twin of
 * 3DE48 (pos at +0x30): nearest child to *p by |dx| + |dy| + |dz| through a
 * V3 *q = &n->pos (the target's addiu a1,a0,48 in both ABS arms), ABS as a
 * preprocessor macro so edg evaluates the sum left to right.
 * Spliced into game_libs_post0b.c.o via REPLACE_FUNC_BODY. */
extern "C" float D_00000000[];
struct V3 { float x, y, z; };
#define adiff(a, b) ((a) < (b) ? -((a) - (b)) : ((a) - (b)))
struct Node { char pad[0x30]; V3 pos; };
struct Link { Node *obj; Link *next; };
struct Iter {
    Link *cur, *nxt;
    Iter(Link *head) { nxt = head; }
    Node *first() { return next(); }
    Node *next() { return (cur = nxt) ? (nxt = nxt->next, cur->obj) : 0; }
};
struct Owner { char pad[0x10]; Link *head; };
extern "C" Node *game_libs_func_0003DA14(Owner *o, V3 *p, Node *skip) {
    float bestd = D_00000000[0x1AC0 / 4];
    Node *best = 0;
    Iter it(o->head);
    for (Node *n = it.first(); n; n = it.next()) {
        if (n != skip) {
            V3 *q = &n->pos;
            float d = adiff(q->x, p->x) + adiff(q->y, p->y) + adiff(q->z, p->z);
            if (d < bestd) { best = n; bestd = d; }
        }
    }
    return best;
}
