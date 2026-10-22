/* IDO 7.1 NCC (C++) donor for gl_func_00038598 (0xA4 / 41 words): walk the
 * owner's child list; for each object with flag 0x200 rebuild its matrix
 * pair (callee(&m30, &m70)), then fire virtual slot 1.
 * The {cur, nxt} list iterator of gl_func_0003D48C (next() advances through
 * nxt->next); the object is held in s0 across the calls. The vptr at +0x28
 * needs the virtuals declared in a base whose data ends at +0x28 (declared
 * in Node itself, edg puts the vptr after all of Node's data).
 * Spliced into game_libs_post0b.c.o via REPLACE_FUNC_BODY. */
extern "C" void gl_func_00000000(...);
struct Node385B {
    char pad[8]; int flags; char padc[0x1C];
    virtual void v0(); virtual void tick(); };
struct Node385 : Node385B {
    char pad2c[4];
    char m30[0x40];
    char m70[0x40];
};
struct Link385 { Node385 *obj; Link385 *next; };
struct Iter385 {
    Link385 *cur, *nxt;
    Iter385(Link385 *head) { nxt = head; }
    Node385 *next() { return (cur = nxt) ? (nxt = nxt->next, cur->obj) : 0; }
};
struct Owner385 { char pad[0x10]; Link385 *head; };
extern "C" void gl_func_00038598(Owner385 *o) {
    Iter385 it(o->head);
    for (Node385 *n = it.next(); n; n = it.next()) {
        if (n->flags & 0x200) gl_func_00000000(n->m30, n->m70);
        n->tick();
    }
}
