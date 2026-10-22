/* IDO 7.1 NCC (C++) donor for game_uso_func_000043D8 (0xF0 / 60 words).
 *
 * Nearest neighbours of a1's value among the owner's children (skipping
 * `skip`): the closest one above (smallest d >= 0) wins, else the closest
 * below. The {cur, nxt} stack iterator with first()/next() is the
 * game_libs 3DB38 one (docs/IDO_CODEGEN.md kit XVIII/XIX); it gives the
 * target's two sp slots, the nxt reload in the skip-test delay slot and the
 * v0/v1/a0/a1/a3 colours on its own. The two seeds are the unit's
 * .rodata +0x94/+0x98 floats (+/-FLT_MAX). They are read through two
 * DISTINCT zero-valued externs so each gets its own `lui at` with the %lo
 * folded (one shared symbol gives a lui/addiu base register), and the
 * offsets are in the addends: the game_uso block is extracted from the .o
 * without a link, so a pinned symbol value would not reach its bytes. The
 * host placeholder re-emits both .rodata words. */
extern "C" char D_43D8_fmin, D_43D8_fmax; /* = 0 (undefined_syms_auto.txt) */
struct Node { char pad[0x38]; float v; };
struct Link { Node *obj; Link *next; };
struct Iter {
    Link *cur, *nxt;
    Iter(Link *head) { nxt = head; }
    Node *first() { return next(); }
    Node *next() { return (cur = nxt) ? (nxt = nxt->next, cur->obj) : 0; }
};
struct Owner { char pad[0x10]; Link *head; };
struct Ref { char pad[8]; float v; };
extern "C" Node *game_uso_func_000043D8(Owner *o, Ref *a1, Node *skip) {
    float fmin = *(float *)(&D_43D8_fmin + 0x94); /* FLT_MAX */
    float fmax = *(float *)(&D_43D8_fmax + 0x98); /* -FLT_MAX */
    Node *above = 0;
    Node *below = 0;
    Iter it(o->head);
    Node *n;
    for (n = it.first(); n; n = it.next()) {
        if (n != skip) {
            float d = a1->v - n->v;
            if (d < 0.0f) {
                if (fmax < d) { below = n; fmax = d; }
            } else {
                if (d < fmin) { above = n; fmin = d; }
            }
        }
    }
    return above ? above : below;
}
