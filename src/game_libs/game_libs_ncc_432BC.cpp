/* IDO 7.1 NCC (C++) donor for gl_func_000432BC (0x1A8 / 106 words): for every
 * child without flag 0x80, dst = src (by-value V3 operator=, the sp+68 temp) or
 * dst = -src, then a callback, an inline 3x3 transform of dst, and a second callback.
 * The iterator's next() is written as an if/return (not ?:): each call then gets
 * its own return value, the loop variable is not copy-propagated onto v0, and no
 * first() pool slot is needed (frame 104).
 * Spliced into game_libs_post0b.c.o via REPLACE_FUNC_BODY. */
extern "C" void gl_func_00000000(...);
extern "C" char D_00000000[];
struct V3 {
    float x, y, z;
    void set(const V3 &v) { x = v.x; y = v.y; z = v.z; }
    void operator=(V3 v) { set(v); }
    void set(float a, float b, float c) { x = a; y = b; z = c; }
    void xform(const float *m) {
        set(m[0] * x + m[4] * y + m[8] * z, m[1] * x + m[5] * y + m[9] * z, m[2] * x + m[6] * y + m[10] * z);
    }
};
struct Obj { char p[0xC4]; int flags; char p2[0xE4 - 0xC8]; V3 src; V3 dst; };
struct Link { Obj *obj; Link *next; };
struct Iter {
    Link *cur, *nxt;
    Iter(Link *h) { nxt = h; }
    Obj *next() { if ((cur = nxt) != 0) { nxt = nxt->next; return cur->obj; } return 0; }
};
extern "C" void gl_func_000432BC(int a0, float *m, int neg) {
    Iter it(*(Link **)(D_00000000 + 0x18C));
    Obj *o;
    while (o = it.next()) {
        if (!(o->flags & 0x80)) {
            if (neg) { o->dst.x = -o->src.x; o->dst.y = -o->src.y; o->dst.z = -o->src.z; }
            else o->dst = o->src;
            gl_func_00000000(&o->dst, a0);
            o->dst.xform(m);
            gl_func_00000000(&o->dst);
        }
    }
}
