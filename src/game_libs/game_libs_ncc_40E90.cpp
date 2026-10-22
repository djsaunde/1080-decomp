/* IDO 7.1 NCC (C++) donor for gl_func_00040E90 (0x174 / 93 words): for every child
 * with flag 0x20000, copy its source record (three V3 + a float, the implicit
 * Rec::operator= through the by-value V3 temp at sp+60) into the cached one,
 * or hand both to a callee with the owner's 0xDC block; then fire virtual
 * slot 1. Entry next() via an inline first() { return next(); }; one dead
 * block-scope int at the end of the loop body buys the bottom frame slots.
 * Spliced into game_libs_post0b.c.o via REPLACE_FUNC_BODY. */
extern "C" void gl_func_00000000(...);
struct V3 {
    float x, y, z;
    void set(const V3 &v) { x = v.x; y = v.y; z = v.z; }
    void operator=(V3 v) { set(v); }
};
struct Rec { V3 a, b, c; float w; };
struct ObjB { char pad[8]; int flags; char pad0c[0x1C];
    virtual void v0(); virtual void tick(); };
struct Obj : ObjB { char pad2c[0x88]; Rec src; Rec dst; };
struct Link { Obj *obj; Link *next; };
struct Iter {
    Link *cur, *nxt;
    Iter(Link *h) { nxt = h; }
    Obj *first() { return next(); }
    Obj *next() { return (cur = nxt) ? (nxt = nxt->next, cur->obj) : 0; }
};
struct Own { char pad[0x10]; Link *head; char pad14[0x18]; int flags; char pad30[0xAC]; char xdc[4]; };
extern "C" void gl_func_00040E90(Own *ow) {
    Iter it(ow->head);

    for (Obj *o = it.first(); o; o = it.next()) {
        if (o->flags & 0x20000) {
            if (ow->flags & 2) o->dst = o->src;
            else gl_func_00000000(ow->xdc, &o->src, &o->dst);
        }
        o->tick();
        { int z0; z0 = 0; }
    }
}
