/* IDO 7.1 NCC (C++) NM-only comparison donor for gl_func_0003DF58 (0x168 / 90
 * words): extremal search over the owner's child list through the stack
 * {cur, nxt} iterator of 3D48C / 3D54C (cur sp+4, nxt sp+8): with o->0x38 set,
 * the largest +0x64 value below ref (a1's or the 0x1AD0 literal) and above the
 * 0x1AD4 bound; otherwise the smallest above ref (0x1AD8) and below 0x1ADC.
 * NCC 99.0 vs cfe 82.2 (agent-c run 133). The old cfe note's "uninitialised
 * iterator seed" was the first word `lw t6,0x10(a0)` (o->head) once the
 * 3DF5C boundary moved to 3DF58. Pass 1 enters through an inline first() (the
 * 3DDC0 / 3D54C edg-temp trick: node copied out of the next() temp), pass 2
 * through next() (coalesced), both as in the target.
 * Still NON_MATCHING (16 words): the two node webs swap v0 / a1 (target
 * pass 1 node v0 with temps a0 / a1, pass 2 node a1); declaration order and
 * scope, a separate pass-2 variable, `= 0` inits, param reuse, if/else ref,
 * loop shapes, branch order and psweep ifpos / w0pos / w1pos / all3: no
 * change. Spliced into build/non_matching/ only via REPLACE_FUNC_BODY. */
extern "C" float D_3DF5C_1AD0[], D_3DF5C_1AD4[], D_3DF5C_1AD8[], D_3DF5C_1ADC[];
struct Node { char pad[0x64]; float f; };
struct Link { Node *obj; Link *next; };
struct Iter {
    Link *cur, *nxt;
    Iter(Link *head) { nxt = head; }
    Node *first() { return next(); }
    Node *next() { return (cur = nxt) ? (nxt = nxt->next, cur->obj) : 0; }
};
struct Owner { char pad[0x10]; Link *head; char pad14[0x24]; int x38; };
extern "C" Node *gl_func_0003DF58(Owner *o, Node *a1) {
    Node *best = 0;
    Iter it(o->head);
    if (o->x38) {
        float ref = a1 ? a1->f : D_3DF5C_1AD0[0x1AD0 / 4];
        float lim = D_3DF5C_1AD4[0x1AD4 / 4];
        Node *n;
        for (n = it.first(); n; n = it.next()) {
            float v = n->f;
            if (v < ref && lim < v) { best = n; lim = v; }
        }
    } else {
        float ref = a1 ? a1->f : D_3DF5C_1AD8[0x1AD8 / 4];
        float lim = D_3DF5C_1ADC[0x1ADC / 4];
        Node *n;
        for (n = it.next(); n; n = it.next()) {
            float v = n->f;
            if (ref < v && v < lim) { best = n; lim = v; }
        }
    }
    return best;
}
