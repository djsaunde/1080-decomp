#include "../../include/uso_call.h"  /* NCC donors build without -I include */
/* IDO 7.1 NCC (C++) donor for Node_printContents (0xC4 / 49 words): debug dump
 * of an owner's child list -- the header line (&D+0x1ECB8, o->0xC) only at
 * depth 0, then per child: indent(depth + 1), the line (&D+0x1ECCC,
 * n->0xC), and the child's own dump(n, depth + 1). Walked with the stack
 * {cur, nxt} iterator of 3D48C / 3D54C (cur sp+40, nxt sp+44).
 *
 * Load-bearing (agent-c run 133; cfe NM was 60.9): each two-argument print
 * reads its int argument into a block-scope local first. With the field read
 * in the argument list NCC evaluates the format address first and the field
 * load lands in the jal delay (plus an `or a2,a0` copy of o, 50 words); the
 * target loads the field, then builds the format address.
 * Spliced into game_libs_post0b.c.o via REPLACE_FUNC_BODY. */
extern "C" {
int gl_func_00000000(...);
extern int D_00000000;
}
struct Node { char pad[0xC]; int x0c; };
struct Link { Node *obj; Link *next; };
struct Iter {
    Link *cur, *nxt;
    Iter(Link *head) { nxt = head; }
    Node *next() { return (cur = nxt) ? (nxt = nxt->next, cur->obj) : 0; }
};
struct Owner { char pad[0xC]; int x0c; Link *head; };
extern "C" void Node_printContents(Owner *o, int a1) {
    Iter it(o->head);
    if (a1 == 0) { int v = o->x0c; CALL(dbg_printf)((char *)&D_00000000 + 0x1ECB8, v); }
    a1++;
    for (Node *n = it.next(); n; n = it.next()) {
        CALL(dbg_printIndent)(a1);
        { int v = n->x0c; CALL(dbg_printf)((char *)&D_00000000 + 0x1ECCC, v); }
#ifdef PORT
        Node_printContents((Owner *)n, a1); /* a child Node is an Owner (head at +0x10) */
#else
        CALL(Node_printContents)(n, a1);
#endif
    }
}
