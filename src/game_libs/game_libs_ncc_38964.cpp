/* IDO 7.1 NCC (C++) donor for Node_broadcastMsg (0xC4 / 49 words): walk the
 * owner's child list with a stack iterator and fire two virtual hooks
 * (slots 4 and 5, vptr at +0x28) on every object with the same context.
 *
 * Same list iterator as ItemType_hostioParms: next() advances through its own
 * nxt member, `(cur = nxt) ? (nxt = nxt->next, cur->obj) : 0`; each hook
 * re-reads the current object through get(). The cfe wrap (98.88) needed
 * address-taken locals and dead `if (0)` stores for the same shape.
 * Spliced into game_libs_post0b.c.o via REPLACE_FUNC_BODY. */
#ifdef PORT /* cfront layout: explicit vptr, virtuals as slot calls (port_cfront.h, plan 17.8) */
#include "port_cfront.h"
struct Node389 {
    char pad[0x28];
    char *vptr; /* cfront vptr */
    CF_V0(void, v0, 0) CF_V0(void, v1, 1) CF_V0(void, v2, 2) CF_V0(void, v3, 3)
    CF_V1(void, f1, 4, void *) CF_V1(void, f2, 5, void *)
};
#else
struct Node389 {
    char pad[0x28];
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3();
    virtual void f1(void *ctx); virtual void f2(void *ctx);
};
#endif
struct Link389 { Node389 *obj; Link389 *next; };
struct Iter389 {
    Link389 *cur, *nxt;
    Iter389(Link389 *head) { nxt = head; }
    Node389 *next() { return (cur = nxt) ? (nxt = nxt->next, cur->obj) : 0; }
    Node389 *get() { return cur->obj; }
};
struct Owner389 { char pad[0x10]; Link389 *head; };
extern "C" void Node_broadcastMsg(Owner389 *o, void *ctx) {
    Iter389 it(o->head);
    for (Node389 *n = it.next(); n; n = it.next()) {
        it.get()->f1(ctx);
        it.get()->f2(ctx);
    }
}
