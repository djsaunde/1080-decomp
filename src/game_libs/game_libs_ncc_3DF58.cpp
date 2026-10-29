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
 * change.
 * EXACT (agent-g run 236): a dead `while (0) { n = 0; it.next(); }` ahead of the pass-1 loop moves the first
 * occurrence of n and the next() temps earlier, so the unconstrained (bitpos-order) colouring assigns them the
 * target's v0 / a0 / a1. Spliced over the game_libs_post0b.c placeholder on both build paths; the pool constants
 * are gl_3df58_k* zero pins (target: lui at,0 / lwc1 0x1AD0.. with no reloc). */
extern "C" float gl_3df58_k1ad0[], gl_3df58_k1ad4[], gl_3df58_k1ad8[], gl_3df58_k1adc[];
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
        float ref = a1 ? a1->f : gl_3df58_k1ad0[0x1AD0 / 4];
        float lim = gl_3df58_k1ad4[0x1AD4 / 4];
        Node *n;
        /* agent-g run 236: dead loop = earlier first occurrence (bitpos) of n and of the next() temps, so the
         * unconstrained colouring gives pass-1 n v0, the nxt temp a0 and the shared next() result a1 (target) */
        while (0) { n = 0; it.next(); }
        for (n = it.first(); n; n = it.next()) {
            float v = n->f;
            if (v < ref && lim < v) { best = n; lim = v; }
        }
    } else {
        float ref = a1 ? a1->f : gl_3df58_k1ad8[0x1AD8 / 4];
        float lim = gl_3df58_k1adc[0x1ADC / 4];
        Node *n;
        for (n = it.next(); n; n = it.next()) {
            float v = n->f;
            if (ref < v && v < lim) { best = n; lim = v; }
        }
    }
    return best;
}
