/* IDO 7.1 NCC (C++) donor for gl_func_00038728 (0x108 / 66 words): lock the
 * owner's sub-object, walk the child list ({cur, nxt} iterator of
 * gl_func_0003D48C) and, for each object with flag 8, rebuild its matrix
 * pair when 0x200 is set and neither 0x20000 nor the +0x2C bit is, attach
 * the owner, and fire virtual slot 3; unlock.
 * The first call's `lw a1,12(a0); jal; or a0,a1` is an inline Sub::lock()
 * whose `this` lands in a1 (a plain gl_func(o->x0c) loads straight into a0).
 * Spliced into game_libs_post0b.c.o via REPLACE_FUNC_BODY. */
extern "C" int gl_func_00000000(...);
struct Owner387;
struct Node387B {
    int x0; int x4; int x8; char padc[8]; Owner387 *x14; int x18; char pad1c[0xC];
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void upd(); };
struct Node387 : Node387B { int x2c; char m30[0x40]; char m70[0x40]; };
struct Link387 { Node387 *obj; Link387 *next; };
struct Iter387 {
    Link387 *cur, *nxt;
    Iter387(Link387 *head) { nxt = head; }
    Node387 *next() { return (cur = nxt) ? (nxt = nxt->next, cur->obj) : 0; }
    Node387 *get() { return cur->obj; }
};
struct Sub387 { void lock() { gl_func_00000000(this); } };
struct Owner387 { char pad[0xC]; Sub387 *x0c; Link387 *head; };
extern "C" void gl_func_00038728(Owner387 *o) {
    Iter387 it(o->head);
    o->x0c->lock();
    for (Node387 *n = it.next(); n; n = it.next()) {
        if (n->x18 & 8) {
            if ((n->x8 & 0x200) && !(n->x8 & 0x20000) && !(n->x2c & 1)) gl_func_00000000(n->m30, n->m70);
            if (n->x4) n->x14 = o;
            n->upd();
        }
    }
    gl_func_00000000();
}
