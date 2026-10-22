/* IDO 7.1 NCC (C++) donor for gl_func_00038DC0 (0x220 / 136 words): for every child
 * with flag 0x200 and not x2c&1, either pass its matrix pair to a callback (owner
 * flag 2) or compute child = m30 * owner.m70 with the natural triple loop, then
 * fire virtual slot 1. Entry through first() + an if/return next() gives the
 * target frame (one extra pool slot); the dead `if (n) {}` inside the j loop adds
 * loop-weighted refs to n so it outranks the hoisted constant 16 for s0
 * (uopt compute_save priority, docs/IDO_CODEGEN.md kit XIX).
 * Spliced into game_libs_post0b.c.o via REPLACE_FUNC_BODY. */
extern "C" void gl_func_00000000(...);
struct NodeB { char pad[8]; int flags; char pad0c[0x1C];
    virtual void v0(); virtual void tick(); };
struct Node : NodeB { int x2c; float m30[16]; float m70[16]; };
struct Link { Node *obj; Link *next; };
struct Iter {
    Link *cur, *nxt;
    Iter(Link *h) { nxt = h; }
    Node *first() { return next(); }
    Node *next() { if ((cur = nxt) != 0) { nxt = nxt->next; return cur->obj; } return 0; }
};
struct Root { char pad[0x10]; Link *head; char pad14[0x18]; int x2c; char pad30[0x40]; float m70[16]; };
extern "C" void gl_func_00038DC0(Root *r) {
    Iter it(r->head);
    for (Node *n = it.first(); n; n = it.next()) {
        if ((n->flags & 0x200) && !(n->x2c & 1)) {
            if (r->x2c & 2) {
                gl_func_00000000(n->m30, n->m70);
            } else {
                float *src = n->m30, *w = r->m70;
                float res[16];
                for (int i = 0; i < 4; i++)
                    for (int j = 0; j < 4; j++) {
                        if (n) {} res[i * 4 + j] = 0.0f;
                        for (int k = 0; k < 4; k++)
                            res[i * 4 + j] += src[i * 4 + k] * w[k * 4 + j];
                    }
                gl_func_00000000(res, n->m70);
            }
        }
        n->tick();
    }
}
