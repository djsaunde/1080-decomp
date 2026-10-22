/* IDO 7.1 NCC (C++) donor for gl_func_00041EDC (0xB4 / 45 words): walk the child
 * list and fire virtual slot 2 on every child with flag 4. Entry next() via an
 * inline first() { return next(); } (own return temp: v1); the body is an
 * inline Node::tick() { if (flags & 4) update(); } on it.get(), whose this
 * shares the loop temp's v0.
 * Spliced into game_libs_post0b.c.o via REPLACE_FUNC_BODY. */
extern "C" void gl_func_00000000(...);
extern "C" char D_00000000[];
struct Node {
    char pad[0x18]; int flags; char pad1c[0xC];
    virtual void v0(); virtual void v1(); virtual void update();
    void tick() { if (flags & 4) update(); }
};
struct Link { Node *obj; Link *next; };
struct Iter {
    Link *cur, *nxt;
    Iter(Link *head) { nxt = head; }
    Node *first() { return next(); }
    Node *next() { return (cur = nxt) ? (nxt = nxt->next, cur->obj) : 0; }
    Node *get() { return cur->obj; }
};
struct Owner { char pad[0x2C]; Link *head; };
extern "C" void gl_func_00041EDC(Owner *o) {
    gl_func_00000000(D_00000000 + 0x1F6D0);
    Iter it(o->head);
    for (Node *n = it.first(); n; n = it.next()) {
        it.get()->tick();
    }
    gl_func_00000000();
}
