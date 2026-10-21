/* IDO 7.1 NCC (C++) donor for game_uso_func_00003AC0 (0x414 / 261 words).
 *
 * Twelve `list->add(new Item(name_k), -1)` statements: each reads one
 * 4-byte name from the USO import table, allocates a 0x58 Item through the
 * global operator new, runs the inline Item ctor (out-of-line base ctor
 * 051E64 with the name by value, then the vptr at +0x28) and registers it.
 *
 * edg / frame facts this body depends on (cfe capped at 253/261 words):
 *  - `new Item(q)` with an inline ctor and NO class-scope operator new: one
 *    alloc, beqz over the inlined ctor. The ctor's by-value Q parameter
 *    (trivial copy) is ONE pool slot shared by all twelve expansions and
 *    is filled before the alloc (the `sw tN,36(sp)` in the jal delay slot).
 *  - q0..q11 are outer-block named locals declared right before their
 *    statement: homed at function entry in declaration order, descending
 *    from sp+108 (the target's twelve slots).
 *  - z0..z3 are dead named locals declared last: edg/uopt still give each
 *    a 4-byte home, between q11 and the pool slot, which is the target's
 *    frame 112 (without them 96).
 * Spliced into game_uso.c.o via REPLACE_FUNC_BODY
 * (docs/MATCHING_WORKFLOW.md#game-uso-ncc-cplusplus-donor-recipe). Every
 * symbol is extern "C"; __nw__FUi resolves through undefined_syms. */
extern "C" {
void game_uso_func_052F70(void *, void *, int);
extern char game_uso_D_807FEC70, game_uso_D_807FEC74, game_uso_D_807FEC78, game_uso_D_807FEC7C;
extern char game_uso_D_807FEC80, game_uso_D_807FEC84, game_uso_D_807FEC88, game_uso_D_807FEC8C;
extern char game_uso_D_807FEC90, game_uso_D_807FEC94, game_uso_D_807FEC98, game_uso_D_807FEC9C;
extern char game_uso_D_807FEAA8;
}
struct P { int v; };
typedef P Name;
struct Q { P p; Q(const P &x) { p.v = x.v; } };
extern "C" void game_uso_func_051E64(void *, P, int, int);
struct Item {
    char pad[0x28];
    void *vt;
    char pad2[0x58 - 0x2C];
    Item(Q q) { game_uso_func_051E64(this, q.p, 0, 0); vt = &game_uso_D_807FEAA8; }
};
#define NM(s, o) (*(Name *)(&game_uso_D_##s + (o)))
#define BLK(k, s, o) Q q##k(NM(s, o)); game_uso_func_052F70(arg0, new Item(q##k), -1);
extern "C" void game_uso_func_00003AC0(void *arg0) {
    BLK(0, 807FEC70, 0x680)
    BLK(1, 807FEC74, 0x684)
    BLK(2, 807FEC78, 0x688)
    BLK(3, 807FEC7C, 0x68C)
    BLK(4, 807FEC80, 0x690)
    BLK(5, 807FEC84, 0x694)
    BLK(6, 807FEC88, 0x698)
    BLK(7, 807FEC8C, 0x69C)
    BLK(8, 807FEC90, 0x6A0)
    BLK(9, 807FEC94, 0x6A4)
    BLK(10, 807FEC98, 0x6A8)
    BLK(11, 807FEC9C, 0x6AC)
    int z0, z1, z2, z3;
    z0 = 0; z1 = 0; z2 = 0; z3 = 0;
}
