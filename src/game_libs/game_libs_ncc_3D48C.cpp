/* IDO 7.1 NCC (C++) donor for gl_func_0003D48C (0xAC / 43 words): notify,
 * then walk the owner's child list with a stack iterator and hand every
 * object to the callee, then two closing calls.
 *
 * The `{cur, nxt}` pair at sp+24/28 is a list iterator object (memory
 * because its inline methods take `this`), and the owner pointer is kept in
 * its incoming home. The shape that settles the loop-bottom expansion
 * (pre-zeroed v0 result, the nxt load in a t-temp) is advancing through the
 * iterator's OWN nxt member: `(cur = nxt) ? (nxt = nxt->next, cur->obj) : 0`.
 * With `nxt = cur->next` the bottom copy colours the nxt load into v0 and
 * zeroes v0 in the else arm (11 words). cfe gets the bottom right but not
 * the ring position of the top expansion. Same iterator: 38964, 41EDC, 7328.
 * Spliced into game_libs_post0b.c.o via REPLACE_FUNC_BODY. */
extern "C" void gl_func_00000000(...);
extern "C" int D_00000000;
struct Link3D4 { int obj; Link3D4 *next; };
struct Iter3D4 {
    Link3D4 *cur, *nxt;
    Iter3D4(Link3D4 *head) { nxt = head; }
    int next() { return (cur = nxt) ? (nxt = nxt->next, cur->obj) : 0; }
    int get() { return cur->obj; }
};
struct Owner3D4 { char pad[0x10]; Link3D4 *head; char pad14[0x18]; int x2c; };
extern "C" void gl_func_0003D48C(Owner3D4 *o) {
    gl_func_00000000(&D_00000000, &o->x2c, 1);
    Iter3D4 it(o->head);
    while (it.next()) gl_func_00000000(it.get());
    gl_func_00000000(&D_00000000);
    gl_func_00000000(o);
}
