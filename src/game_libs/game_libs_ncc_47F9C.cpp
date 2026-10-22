/* IDO 7.1 NCC (C++) donor for gl_func_00047F9C (0x168 / 90 words): for every
 * child, reset its dirty word from a global, fetch its position into a V3
 * local through a callee, set dirty if it differs from the cached copy, then
 * store the cache (by-value V3 operator=, the sp+60 temp) and a 16-byte record.
 * The dead block-scope pair after the record copy buys the two bottom frame
 * slots (frame 112). The global is a baked USO address, pinned as
 * gl_d_0001F970 in undefined_syms (a D_00000000 + 0x1F970 form makes uopt
 * split the hoisted address into base + 0x7970).
 * Spliced into game_libs_post0b.c.o via REPLACE_FUNC_BODY. */
extern "C" void gl_func_00000000(...);
extern "C" int gl_d_0001F970;
struct V3 {
    float x, y, z;
    void set(const V3 &v) { x = v.x; y = v.y; z = v.z; }
    void operator=(V3 v) { set(v); }
    int operator!=(const V3 &o) const { return x != o.x || y != o.y || z != o.z; }
};
struct Q4 { int a, b, c, d; };
struct Obj { char pad[0xA0]; V3 a0; char padac[0x34]; int e0; V3 e4; char padf0[0xC]; Q4 fc; char pad10c[0x18]; Q4 q124; };
struct Link { Obj *obj; Link *next; };
struct Iter {
    Link *cur, *nxt;
    Iter(Link *h) { nxt = h; }
    Obj *next() { return (cur = nxt) ? (nxt = nxt->next, cur->obj) : 0; }
};
struct Own { char pad[0x18C]; Link *head; };
extern "C" void gl_func_00047F9C(Own *ow) {
    Iter it(ow->head);
    for (Obj *o = it.next(); o; o = it.next()) {
        o->e0 = gl_d_0001F970;
        V3 p;
        p.set(o->a0);
        gl_func_00000000(&p);
        if (p.x != o->e4.x || p.y != o->e4.y || p.z != o->e4.z) o->e0 = 1;
        o->e4 = p;
        o->fc = o->q124;
        { int z0, z1; z0 = 0; z1 = 0; }
    }
}
