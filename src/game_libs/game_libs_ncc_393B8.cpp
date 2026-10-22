/* IDO 7.1 NCC (C++) donor for gl_func_000393B8 (0x26C / 155 words): lock the owner,
 * then for every child with flag 8: if flag 0x200, not 0x20000 and not x2c&1,
 * pass its matrix pair to a callback (owner flag 2) or multiply it by the
 * owner's 4x4; publish the owner, back-link, fire virtual slot 3. The if/return
 * next() keeps the loop variable a copy of v0; the dead `if (n) {}` in the j
 * loop lifts n above the hoisted constant 16 for s0; the dead e0 is the extra
 * frame slot above res (as in 40974).
 * Spliced into game_libs_post0b.c.o via REPLACE_FUNC_BODY. */
extern "C" void gl_func_00000000(...);
extern "C" int gl_func_00000000_i(...);
extern "C" char D_00000000[];
struct Sub393 { void lock() { gl_func_00000000(this); } };
struct NodeB393 {
    char pad[4]; int x04; int flags; char pad0c[8]; void *owner; int x18; char pad1c[0xC];
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void tick();
};
struct Node393 : NodeB393 { int x2c; float m30[16]; float m70[16]; };
struct Link393 { Node393 *obj; Link393 *next; };
struct Iter393 {
    Link393 *cur, *nxt;
    Iter393(Link393 *h) { nxt = h; }
    Node393 *next() { if ((cur = nxt) != 0) { nxt = nxt->next; return cur->obj; } return 0; }
};
struct Root393 { char pad[0xC]; Sub393 *sub; Link393 *head; char pad14[0x18]; int x2c; char pad30[0x40]; float m70[16]; };
extern "C" int gl_func_000393B8(Root393 *r) {
    Iter393 it(r->head);
    r->sub->lock();
    Node393 *n = it.next();
    while (n) {
        if (n->x18 & 8) {
            if ((n->flags & 0x200) && !(n->flags & 0x20000) && !(n->x2c & 1)) {
                if (r->x2c & 2) {
                    gl_func_00000000(n->m30, n->m70);
                } else {
                    float *src = n->m30, *w = r->m70; int e0; e0 = 0;
                    float res[16];
                    for (int i = 0; i < 4; i++)
                        for (int j = 0; j < 4; j++) {
                            if (n) {} res[i * 4 + j] = 0.0f;
                            for (int k = 0; k < 4; k++)
                                res[i * 4 + j] += src[i * 4 + k] * w[k * 4 + j];
                        }
                    gl_func_00000000(res, n->m70);
                }
            }
            if (r->x2c & 2) *(Root393 **)D_00000000 = r;
            if (n->x04) n->owner = r;
            n->tick();
        }
        n = it.next();
    }
    return gl_func_00000000_i();
}
