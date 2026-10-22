/* IDO 7.1 NCC (C++) donor for game_libs_func_0003DDC0 (0x88 / 34 words): return 1 and
 * copy the matching child's position (+0x5C) to *out, else 0. Twin of
 * gl_func_0003E1B0 (+0xA0) and game_libs_func_0003DBEC. The entry next() goes
 * through an inline first() { return next(); }, which gives it its own edg
 * return temp, so the compare reads the loop variable's v1 copy.
 * Spliced into game_libs_post0b.c.o via REPLACE_FUNC_BODY. */
struct V3 { float x, y, z; void set(const V3 &v) { x = v.x; y = v.y; z = v.z; } V3 &operator=(const V3 &v) { set(v); return *this; } };
struct Node { char pad[0x5C]; V3 pos; };
struct Link { Node *obj; Link *next; };
struct Iter {
    Link *cur, *nxt;
    Iter(Link *head) { nxt = head; }
    Node *first() { return next(); }
    Node *next() { return (cur = nxt) ? (nxt = nxt->next, cur->obj) : 0; }
    Node *get() { return cur->obj; }
};
struct Owner { char pad[0x10]; Link *head; };
extern "C" int game_libs_func_0003DDC0(Owner *o, Node *target, V3 *out) {
    Iter it(o->head);
    for (Node *n = it.first(); n; n = it.next()) {
        if (n == target) {
            *out = n->pos;
            return 1;
        }
    }
    return 0;
}
