/* IDO 7.1 NCC (C++) donor for game_libs_func_0003DE48 (0x110 / 68 words): return the
 * child nearest to *p by |dx| + |dy| + |dz|, skipping one node. The ABS is a
 * preprocessor macro, not an inline function: edg evaluates inline-call operands
 * of a sum out of source order (temps), a plain ?: expression left to right, so
 * only the macro gives the target's x, y, x+y, z, (x+y)+z. Boundary fix: the
 * old .s carried 3DF58's first word (gl_func_0003DF5C renamed gl_func_0003DF58).
 * Spliced into game_libs_post0b.c.o via REPLACE_FUNC_BODY. */
extern "C" float D_00000000[];
struct V3 { float x, y, z; };
#define adiff(a, b) ((a) < (b) ? -((a) - (b)) : ((a) - (b)))
struct Node { char pad[0x5C]; V3 pos; };
struct Link { Node *obj; Link *next; };
struct Iter {
    Link *cur, *nxt;
    Iter(Link *head) { nxt = head; }
    Node *first() { return next(); }
    Node *next() { return (cur = nxt) ? (nxt = nxt->next, cur->obj) : 0; }
};
struct Owner { char pad[0x10]; Link *head; };
extern "C" Node *game_libs_func_0003DE48(Owner *o, V3 *p, Node *skip) {
    float bestd = D_00000000[0x1ACC / 4];
    Node *best = 0;
    Iter it(o->head);
    for (Node *n = it.first(); n; n = it.next()) {
        if (n != skip) {
            float d = adiff(n->pos.x, p->x) + adiff(n->pos.y, p->y) + adiff(n->pos.z, p->z);
            if (d < bestd) { best = n; bestd = d; }
        }
    }
    return best;
}
