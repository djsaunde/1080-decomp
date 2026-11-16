#include "../../include/uso_call.h"  /* NCC donors build without -I include */
/* IDO 7.1 NCC (C++) donor for QNode_updateChildren (0x174 / 93 words): for every child
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
#ifdef PORT /* cfront layout: explicit vptr, virtuals as slot calls (port_cfront.h, plan 17.8) */
#include "port_cfront.h"
struct ObjB { char pad[8]; int flags; char pad0c[0x1C];
    char *vptr; /* cfront vptr */
    CF_V0(void, v0, 0) CF_V0(void, tick, 1)
};
#else
struct ObjB { char pad[8]; int flags; char pad0c[0x1C];
    virtual void v0(); virtual void tick(); };
#endif
struct Obj : ObjB { char pad2c[0x88]; Rec src; Rec dst; };
struct Link { Obj *obj; Link *next; };
struct Iter {
    Link *cur, *nxt;
    Iter(Link *h) { nxt = h; }
    Obj *first() { return next(); }
    Obj *next() { return (cur = nxt) ? (nxt = nxt->next, cur->obj) : 0; }
};
struct Own { char pad[0x10]; Link *head; char pad14[0x18]; int flags; char pad30[0xAC]; char xdc[4]; };
extern "C" void QNode_updateChildren(Own *ow) {
    Iter it(ow->head);

    for (Obj *o = it.first(); o; o = it.next()) {
        if (o->flags & 0x20000) {
            if (ow->flags & 2) o->dst = o->src;
            else CALL(math_xformCombine)(ow->xdc, &o->src, &o->dst);
        }
        o->tick();
        { int z0; z0 = 0; }
    }
}
