/* IDO 7.1 NCC (C++) donor for gl_func_0003D2C8 (0xFC / 63 words): count the
 * owner's children with one iterator, pass &count to gl_ref_0005166C (baked
 * USO jal), then fire virtual slot 7 on every child with a second iterator.
 * The counting loop's entry call goes through an inline
 * first() { return next(); } (own return temp, v1); with it the second loop's
 * get() object lands in v0 as in the target.
 * Spliced into game_libs_post0b.c.o via REPLACE_FUNC_BODY. */
extern "C" void gl_ref_0005166C(int *);
struct NodeB { char pad[0x28];
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4(); virtual void v5(); virtual void v6(); virtual void reset(); };
struct Link { NodeB *obj; Link *next; };
struct Iter {
    Link *cur, *nxt;
    Iter(Link *head) { nxt = head; }
    NodeB *next() { return (cur = nxt) ? (nxt = nxt->next, cur->obj) : 0; }
    NodeB *first() { return next(); }
    NodeB *get() { return cur->obj; }
};
struct Owner { char pad[0x10]; Link *head; };
extern "C" void gl_func_0003D2C8(Owner *o) {
    Iter a(o->head);
    int n = 0;
    if (a.first()) do n++; while (a.next());
    gl_ref_0005166C(&n);
    Iter b(o->head);
    while (b.next()) b.get()->reset();
}
