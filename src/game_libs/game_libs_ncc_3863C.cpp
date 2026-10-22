/* IDO 7.1 NCC (C++) donor for gl_func_0003863C (0xEC / 59 words): lock the owner's
 * sub-object, then for every child with flag 4 back-link it to the owner and
 * fire virtual slot 2 when its mask overlaps the callee's result. The entry
 * next() goes through an inline first() { return next(); }, which moves the
 * vptr after the second get() into v0 as in the target.
 * Spliced into game_libs_post0b.c.o via REPLACE_FUNC_BODY. */
extern "C" int gl_func_00000000(...);
struct Owner;
struct NodeB {
    int x0; int x4; char pad8[0xC]; Owner *x14; int flags; char pad1c[4]; int mask; char pad24[4];
    virtual void v0(); virtual void v1(); virtual void fire(); };
struct Link { NodeB *obj; Link *next; };
struct Iter {
    Link *cur, *nxt;
    Iter(Link *head) { nxt = head; }
    NodeB *next() { return (cur = nxt) ? (nxt = nxt->next, cur->obj) : 0; }
    NodeB *first() { return next(); }
    NodeB *get() { return cur->obj; }
};
struct Sub { void lock() { gl_func_00000000(this); } };
struct Owner { char pad[0xC]; Sub *x0c; Link *head; };
extern "C" void gl_func_0003863C(Owner *o) {
    Iter it(o->head);
    o->x0c->lock();
    for (NodeB *n = it.first(); n; n = it.next()) {
        if (it.get()->flags & 4) {
            if (it.get()->x4) it.get()->x14 = o;
            int m = gl_func_00000000();
            if (it.get()->mask & m) it.get()->fire();
        }
    }
    gl_func_00000000();
}
