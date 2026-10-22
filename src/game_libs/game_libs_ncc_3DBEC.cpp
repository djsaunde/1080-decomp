/* IDO 7.1 NCC (C++) donor for game_libs_func_0003DBEC (0xA4 / 41 words): walk the
 * owner's child list; when the child equals the argument, copy its position
 * into *out (by-value V3 operator= through the sp+4 temp) and return 1.
 * The first next() goes through an inline first() { return next(); }: the
 * nested inline gives the entry call its own return temp, so the loop
 * variable is a phi of two temps and the compare reads its v1 copy
 * (a single next() shares one temp and the compare is copy-propagated to v0).
 * Spliced into game_libs_post0b.c.o via REPLACE_FUNC_BODY. */
struct V3 {
    float x, y, z;
    void set(const V3 &v) { x = v.x; y = v.y; z = v.z; }
    void operator=(V3 v) { set(v); }
};
struct Node3DB { char pad[0x30]; V3 pos; };
struct Link3DB { Node3DB *obj; Link3DB *next; };
struct Iter3DB {
    Link3DB *cur, *nxt;
    Iter3DB(Link3DB *h) { nxt = h; }
    Node3DB *first() { return next(); }
    Node3DB *next() { return (cur = nxt) ? (nxt = nxt->next, cur->obj) : 0; }
};
struct Owner3DB { char pad[0x10]; Link3DB *head; };
extern "C" int game_libs_func_0003DBEC(Owner3DB *o, Node3DB *t, V3 *out) {
    Iter3DB it(o->head);
    for (Node3DB *n = it.first(); n; n = it.next()) {
        if (n == t) { *out = n->pos; return 1; }
    }
    return 0;
}
