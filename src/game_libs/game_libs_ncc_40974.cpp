/* IDO 7.1 NCC (C++) donor for gl_func_00040974 (0x338 / 206 words): lock the
 * owner, blend its 0xDC block from its matrix, then for every child with flag 8:
 * publish the owner, back-link, and either copy/blend the child's record
 * (flag 0x20000, implicit Rec::operator= over by-value V3 copies) or rebuild
 * its matrix from the owner's (flag 0x200, the natural triple loop), then fire
 * virtual slot 3. Matched on the first port except the frame: two dead
 * function-scope ints and one dead block-scope int restore the edg slot layout.
 * Spliced into game_libs_post0b.c.o via REPLACE_FUNC_BODY. */
extern "C" void gl_func_00000000(...);
extern "C" char D_00000000[];
struct V3 {
    float x, y, z;
    void set(const V3 &v) { x = v.x; y = v.y; z = v.z; }
    void operator=(V3 v) { set(v); }
};
struct Rec { V3 a, b, c; float w; };
struct Sub { void lock() { gl_func_00000000(this); } };
struct NodeB {
    char pad[4]; int x04; int flags; char pad0c[8]; void *owner; int x18; char pad1c[0xC];
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void tick();
};
struct Node : NodeB { int x2c; float m30[16]; float m70[16]; char padb0[4]; Rec src; Rec dst; };
struct Link { Node *obj; Link *next; };
struct Iter {
    Link *cur, *nxt;
    Iter(Link *h) { nxt = h; }
    Node *next() { return (cur = nxt) ? (nxt = nxt->next, cur->obj) : 0; }
};
struct Root { char pad[0xC]; Sub *sub; Link *head; char pad14[0x18]; int x2c; char pad30[0x40]; float m70[16]; char padb0[0x2C]; char xdc[4]; };
extern "C" void gl_func_00040974(Root *r) {
    Iter it(r->head);
    int y0, y1; y0 = 0; y1 = 0;
    r->sub->lock();
    gl_func_00000000(r->xdc, r->m70);
    for (Node *n = it.next(); n; n = it.next()) {
        if (n->x18 & 8) {
            if (r->x2c & 2) *(Root **)D_00000000 = r;
            if (n->x04) n->owner = r;
            if (n->flags & 0x20000) {
                if (!(n->x2c & 1)) {
                    if (r->x2c & 2) n->dst = n->src;
                    else gl_func_00000000(r->xdc, &n->src, &n->dst);
                }
            } else if (n->flags & 0x200) {
                float *src = n->m30, *w = r->m70; int e0; e0 = 0;
                float res[16];
                for (int i = 0; i < 4; i++)
                    for (int j = 0; j < 4; j++) {
                        res[i * 4 + j] = 0.0f;
                        for (int k = 0; k < 4; k++)
                            res[i * 4 + j] += src[i * 4 + k] * w[k * 4 + j];
                    }
                gl_func_00000000(res, n->m70);
            }
            n->tick();
        }
    }
    gl_func_00000000();
}
